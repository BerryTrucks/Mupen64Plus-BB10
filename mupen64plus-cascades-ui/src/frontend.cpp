/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "frontend.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>

#include <bb/cascades/QmlDocument>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/DropDown>
#include <bb/cascades/Divider>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/Label>
#include <bb/cascades/FontStyle>
#include <bb/cascades/TextStyle>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/Theme>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/VisualStyle>
#include <bb/cascades/Option>
#include <QSettings>
#include <bb/cascades/Window>
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FileType>
#include <QSignalMapper>
#include <bb/data/XmlDataAccess>

#include <bb/system/SystemToast>
#include <bps/bps.h>
#include <bps/screen.h>
#include <bps/event.h>
#include <sys/keycodes.h>

#include "main.h"
#include "bbutil.h"

#include <sys/neutrino.h>

using namespace bb::cascades;
using namespace bb::device;
using namespace bb::system;
using namespace bb::data;

//screen_context_t screen_cxt_map;
screen_window_t screen_win_map;
screen_buffer_t screen_buf[2];
int rect[4];
SystemToast *toast;
SystemToast *toastButton;

Emulator *m64p;
QString CheatList = "";

int chid = -1, coid = -1;

extern bool debug_mode;

enum GamePadButton{
   A_BUTTON=0,
   B_BUTTON,
   C_BUTTON,
   X_BUTTON,
   Y_BUTTON,
   Z_BUTTON,
   MENU1_BUTTON,
   MENU2_BUTTON,
   MENU3_BUTTON,
   MENU4_BUTTON,
   L1_BUTTON,
   L2_BUTTON,
   L3_BUTTON,
   R1_BUTTON,
   R2_BUTTON,
   R3_BUTTON,
   DPAD_UP_BUTTON,
   DPAD_DOWN_BUTTON,
   DPAD_LEFT_BUTTON,
   DPAD_RIGHT_BUTTON,
   NO_BUTTON,
   EXT_BUTTON_L4,
   EXT_BUTTON_R4
 };

struct GameController {
  // Static device info.
  screen_device_t handle;
  int type;
  int analogCount;
  int buttonCount;
  char id[64];
  char name[64];

  // Current state.
  int buttons;
  int analog0[3];
  int analog1[3];

  // Text to display to the user about this controller.
  char deviceString[256];
 };

GameController _controllers[4];

void initController(GameController* controller, int player)
{
    // Initialize controller values.
    controller->handle = 0;
    controller->type = 0;
    controller->analogCount = 0;
    controller->buttonCount = 0;
    controller->buttons = 0;
    controller->analog0[0] = controller->analog0[1] = controller->analog0[2] = 0;
    controller->analog1[0] = controller->analog1[1] = controller->analog1[2] = 0;
    sprintf(controller->deviceString, "Player %d: No device detected.", player + 1);
}

void Frontend::create_button_mapper() {
	const int usage2 = SCREEN_USAGE_NATIVE | SCREEN_USAGE_WRITE | SCREEN_USAGE_READ;
	int rc;

	screen_create_context(&screen_cxt, SCREEN_APPLICATION_CONTEXT);
	screen_create_window_type(&screen_win_map, screen_cxt, SCREEN_CHILD_WINDOW);

	screen_join_window_group(screen_win_map, Application::instance()->mainWindow()->groupId().toAscii().constData());
	int format = SCREEN_FORMAT_RGBA8888;
	screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_FORMAT, &format);

	screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_USAGE, &usage2);

    /*int ndisplays;
    screen_get_context_property_iv(screen_cxt, SCREEN_PROPERTY_DISPLAY_COUNT, &ndisplays);
    fprintf(stderr, "Display Count: %d\n", ndisplays);
    if (ndisplays > 1) {
    	screen_display_t *screen_dpy = (screen_display_t*)calloc(ndisplays, sizeof(screen_display_t));
    	screen_get_context_property_pv(screen_cxt, SCREEN_PROPERTY_DISPLAYS, (void**)screen_dpy);
    	screen_set_window_property_pv(screen_win_map, SCREEN_PROPERTY_DISPLAY, (void**)&screen_dpy[1]);
    	free(screen_dpy);
    }*/

	const char *env = getenv("WIDTH");

	if (0 == env) {
		perror("failed getenv for WIDTH");
	}

	int width2 = atoi(env);

	env = getenv("HEIGHT");

	if (0 == env) {
		perror("failed getenv for HEIGHT");
	}

	int height2 = atoi(env);
	rect[0] = 0;
	rect[1] = 0;
	rect[2] = width2;
	rect[3] = height2;

	rc = screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
	if (rc) {
		perror("screen_set_window_property_iv");
	}

	int z2 = -10;
	if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z2) != 0) {
		return;
	}

	rc = screen_create_window_buffers(screen_win_map, 2);
	if (rc) {
		perror("screen_create_window_buffers");
	}

	screen_get_window_property_pv(screen_win_map, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf);

	int bg[] = { SCREEN_BLIT_COLOR, 0x00000000,
				 SCREEN_BLIT_GLOBAL_ALPHA, 0x80,
				 SCREEN_BLIT_END };
	screen_fill(screen_cxt, screen_buf[0], bg);
	screen_fill(screen_cxt, screen_buf[1], bg);

	screen_pixmap_t screen_pix2;
	screen_create_pixmap( &screen_pix2, screen_cxt );

	int format2 = SCREEN_FORMAT_RGBA8888;
	screen_set_pixmap_property_iv(screen_pix2, SCREEN_PROPERTY_FORMAT, &format2);

	int pix_usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
	screen_set_pixmap_property_iv(screen_pix2, SCREEN_PROPERTY_USAGE, &pix_usage);

	screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

	bps_initialize();
	screen_request_events(screen_cxt);
}

QDataStream& operator<<(QDataStream& out, const Game &obj)
{
	obj.serialize(out);
	return out;
}

QDataStream& operator>>(QDataStream& in, Game &obj)
{
	obj.deserialize(in);
	return in;
}

Frontend::Frontend(int theme)
{
	ThemeSupport* themeSupport = Application::instance()->themeSupport();
	Theme* currentTheme = themeSupport->theme();
	ColorTheme* colorTheme = currentTheme->colorTheme();
	VisualStyle::Type style = colorTheme->style();
	m_color = (ThemeColor)theme;
	switch (style)
	{
	case VisualStyle::Bright:
		m_isbright = true;
		break;
	case VisualStyle::Dark:
		m_isbright = false;
		break;
	default:
		m_isbright = !hasKeyboard();
		break;
	}
	qmlRegisterType<bb::cascades::pickers::FilePicker>("bb.cascades.pickers", 1, 0, "FilePicker");
	qmlRegisterUncreatableType<bb::cascades::pickers::FileType>("bb.cascades.pickers", 1, 0, "FileType", "");
	qmlRegisterType<ImageLoader>();
	qRegisterMetaType<Game>("Game");
	qRegisterMetaTypeStreamOperators<Game>("Game");

	//Set up a
	m_hdmiInfo = NULL;
	m_emuRunning = false;
	m_boxart = 0;
	mVideoPlugin = 0;
	m_boxartLoaded = false;
	mAudio = 0;
	m_menuOffset = 0;
	m_settings = new QSettings("emulators", "mupen64p");
	m_devices = new QMapListDataModel();
	m_history = new QMapListDataModel();
	m_menuAnimation = new QPropertyAnimation(this, "menuOffset");
	m_animationLock = new QMutex();
	m_menuAnimation->setDuration(250);
	m_coverImage = "asset:///images/mupen64plus.png";

	if(access("shared/misc/n64/", F_OK) != 0){
		mkdir("shared/misc/n64/", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/data", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/roms", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/.boxart", S_IRWXU | S_IRWXG);

		char buf[8192];
		size_t size;

		FILE* source = fopen("app/native/mupen64plus.cfg.2", "rb");
		FILE* dest = fopen("shared/misc/n64/data/mupen64plus.cfg", "wb");

		// clean and more secure
		// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

		while ((size = fread(buf, 1, 8192, source))) {
			fwrite(buf, 1, size, dest);
		}

		fclose(source);
		fclose(dest);
	}

	m64p = new Emulator((char *)Application::instance()->mainWindow()->groupId().toAscii().constData(), (char*)QString("emulator_m64p").toAscii().constData());
	//m64p->print_controller_config();

	connect(m_menuAnimation, SIGNAL(finished()), SLOT(showMenuFinished()));
	connect(this, SIGNAL(menuOffsetChanged()), SLOT(onMenuOffsetChanged()));
	connect(this, SIGNAL(createOption(QString,QUrl)), SLOT(onCreateOption(QString,QUrl)));

	toast = new SystemToast(this);
	toastButton = new SystemToast(this);

	chid = ChannelCreate(0);
	coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);

    // Set the application organization and name, which is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Mupen64Plus-BB");

    // Then we load the application.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_frontend", this);

    if (!qml->hasErrors()) {
        m_tab = qml->createRootObject<TabbedPane>();
        if (m_tab) {
        	mCheatsContainer = m_tab->findChild<Container*>("cheats");

            Application::instance()->setScene(m_tab);

        	TwitterRequest* request = new TwitterRequest(this);
        	connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onVersionRecieved(QString, bool)));
        	request->requestVersion();

            start();
        }
    }
}

Frontend::~Frontend()
{
	printf("Saving Controllers!\n");fflush(stdout);
	m64p->save_controller_config(0);
	m64p->save_controller_config(1);
	m64p->save_controller_config(2);
	m64p->save_controller_config(3);
	printf("Finished Saving Controllers!\n");fflush(stdout);
	delete m_settings;
	delete m_devices;
	delete m_history;
	delete m_menuAnimation;
	delete m_animationLock;
	if (m_hdmiInfo)
		delete m_hdmiInfo;
}

void Frontend::setBright(int index)
{
	if (index == (int)m_color)
		return;
	QFile::remove("data/dark.dat");
	QFile::remove("data/bright.dat");
	if (index == 0)
	{
		QFile file("data/bright.dat");
		file.open(QFile::WriteOnly);
		file.close();
		m_color = Bright;
	}
	else
	{
		QFile file("data/dark.dat");
		file.open(QFile::WriteOnly);
		file.close();
		m_color = Dark;
	}
}

bool Frontend::debugMode()
{
	return debug_mode;
}

void Frontend::onManualExit(){
	QDir dir("data/screens/");
	dir.setNameFilters(QStringList() << "*.*");
	dir.setFilter(QDir::Files);
	foreach (QString dirFile, dir.entryList())
	{
		dir.remove(dirFile);
	}
	printf("OnManualExit!\n");fflush(stdout);
	int msg = 2;//, ret = 0;
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);
	wait();
	printf("Wait!\n");fflush(stdout);
	quit();
	printf("Quit!\n");fflush(stdout);
}
typedef union {
	_pulse pulse;
	int msg;
}recv_msg;

//A separate QThread that runs the emulator.
void Frontend::run()
{
	recv_msg msg;
	int rcvid;
	int z = 5;
	int sym = -1;
	bps_event_t *event = NULL;

	initController(&_controllers[0], 0);
	initController(&_controllers[1], 1);
	initController(&_controllers[2], 2);
	initController(&_controllers[3], 3);
	create_button_mapper();
	discoverControllers();
	detectHDMI();
	setHistory(getHistory());

	while(1) {
		while(1){
			rcvid = MsgReceive(chid, &msg, sizeof(msg), 0);

			if(rcvid <= 0){
				continue;
			}

			if(msg.msg == 1){
				MsgReply(rcvid, 0, NULL, 0);
				break;
			} else if (msg.msg == 2){
				MsgReply(rcvid, 0, NULL, 0);
				return;
			}

			z = 5;
			if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z) != 0) {
				return;
			}

			screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

			while(1){
				if (BPS_SUCCESS != bps_get_event(&event, -1)) {
					fprintf(stderr, "bps_get_event failed\n");
					break;
				}

				if (event) {
					int domain = bps_event_get_domain(event);

					if (domain == screen_get_domain()) {
						screen_event_t screen_event = screen_event_get_event(event);
						int screen_val;
						screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

						if(screen_val == SCREEN_EVENT_MTOUCH_TOUCH){
							//This is touch screen event
							sym = -1;
							break;
						} else if(screen_val == SCREEN_EVENT_KEYBOARD) {
							screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_SYM, &sym);
							break;
						}
						else if (screen_val == SCREEN_EVENT_GAMEPAD || screen_val == SCREEN_EVENT_JOYSTICK)
						{
							screen_device_t device;
							screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void**)&device);
							for (int i = 0; i < 4; i++)
							{
								GameController *control = &_controllers[i];
								if (control->handle == device)
								{
									sym = NO_BUTTON;
									fprintf(stderr, "Testing Gamepad: %s\n", control->id);
									int err = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &control->buttons);
									if (err != BPS_SUCCESS)
										fprintf(stderr, "Error at SCREEN_PROPERTY_BUTTONS: %d\n", err);
									fprintf(stderr, "Testing Gamepad Button: %d\n", control->buttons);
									if (control->buttons == 0)
									{
										screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG0, control->analog0);
										if (control->analog0[2] == 255)
										{
											sym = 1 << EXT_BUTTON_L4;
										}
										screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG1, control->analog1);
										if (control->analog1[2] == 255)
										{
											sym = 1 << EXT_BUTTON_R4;
										}
									}
									else
									{
										for (int j = A_BUTTON; j < NO_BUTTON; j++)
										{
											if (control->buttons & (1 << j))
											{
												fprintf(stderr, "Gamepad Button: %d\n", j);
												sym = 1 << j;
												break;
											}
										}
									}
								}
							}
							if (sym == NO_BUTTON)
								sym = -1;
							break;
						}
					}
				}
			}

			z = -10;
			if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z) != 0) {
				return;
			}
			screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

			MsgReply(rcvid, 0, &sym, sizeof(sym));
		}

		//Cheats
		printf("CheatList: %s\n", CheatList.toAscii().constData());
		m64p->l_CheatNumList = strdup((char*)CheatList.toAscii().constData());

		//Input
		m64p->save_controller_config(0);
		m64p->save_controller_config(1);
		m64p->save_controller_config(2);
		m64p->save_controller_config(3);

		if(mAudio){
			m64p->SetConfigParameter(std::string("UI-Console[AudioPlugin]=")+"mupen64plus-audio-sdl");
		} else {
			m64p->SetConfigParameter(std::string("UI-Console[AudioPlugin]=")+"dummy");
		}

		if(!mVideoPlugin){
			m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"libmupen64plus-video-rice");
		}
		else if (mVideoPlugin == 1) {
			m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"gles2n64");
		}
		else {
			m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"mupen64plus-video-glide64mk2");
		}

		//screen resolution
		std::ostringstream s;
		s << "Video-General[ScreenWidth]=";
		s << emuWidth();
		m64p->SetConfigParameter(s.str());
		std::ostringstream s2;
		s2 << "Video-General[ScreenHeight]=";
		s2 << emuHeight();
		m64p->SetConfigParameter(s2.str());

		m_emuRunning = true;

		//m64p->print_controller_config();
		m64p->Start();
		//m64p->print_controller_config();
	}
}

void Frontend::onThumbnail()
{
	if (m_emuRunning)
	{
		int width = this->width();
		int height = this->height();
		if (height > width) {
			int temp = height;
			height = width;
			width = temp;
		}
		int x = 0;
		if (width != height)
			x = (int)(((double)(width - height)) / 2.0);
		QDateTime dt = QDateTime::currentDateTime();
		m_coverImage = "data/screens/screen" + dt.toString("ddMMyyyy_hhmmss") + ".bmp";
		bbutil_screencapture(m_coverImage.toAscii().data(), x, 0, height, height);
		QFile fl(m_coverImage);
		QFileInfo info(fl);
		m_coverImage = info.absoluteFilePath();
		if (!m_coverImage.startsWith("file://"))
			m_coverImage = "file://" + m_coverImage;
		emit coverImageChanged();
	}
}

void Frontend::showMenuFinished()
{
	m_animationLock->unlock();
}

void Frontend::onMenuOffsetChanged()
{
	bbutil_offset_menu(m_menuOffset);
}

void Frontend::swipedown()
{
	if (!m_animationLock->tryLock())
		return;
	if (bbutil_is_menu_open())
	{
		m_menuAnimation->setStartValue(175);
		m_menuAnimation->setEndValue(0);
		m_menuAnimation->start();
	}
	else
	{
		m_menuAnimation->setStartValue(0);
		m_menuAnimation->setEndValue(175);
		m_menuAnimation->start();
	}
}

QString Frontend::getValueFor(const QString &objectName, const QString &defaultValue)
{
    QSettings settings;

    // If no value has been saved, return the default value.
    if (settings.value(objectName).isNull()) {
        return defaultValue;
    }

    // Otherwise, return the value stored in the settings object.
    return settings.value(objectName).toString();
}

void Frontend::saveValueFor(const QString &objectName, const QString &inputValue)
{
    // A new value is saved to the application settings object.
    QSettings settings;
    settings.setValue(objectName, QVariant(inputValue));
}

//For input this will jsut update our controller struct. Else, we use QSettings for video.
void Frontend::saveConfigValue(const QString &section, const QString &name, const QString &value)
{
	if(!section.contains("Input-SDL-Control")) {
		if(this->getValueFor("perRom", "true") == "true"){
			this->saveValueFor(mRom.mid(mRom.lastIndexOf('/')+1) + name, value);
		} else {
			this->saveValueFor(name, value);
		}
	}

	m64p->SetConfigParameter((std::string)(section.toStdString() + "[" + name.toStdString() + "]=" + value.toStdString()));
}

//For input this will jsut update our controller struct. Else, we use QSettings for video.
QString Frontend::getConfigValue(const QString &rom, const QString &section, const QString &name, const QString &value)
{
	Q_UNUSED(rom);
	if(!section.contains("Input-SDL-Control")) {
		if(this->getValueFor("perRom", "true") == "true"){
			return this->getValueFor(mRom.mid(mRom.lastIndexOf('/')+1) + name, value);
		} else {
			return this->getValueFor(name, value);
		}
	}
	return NULL;
}

QList<Game> Frontend::getHistory()
{
	QList<Game> retval;
	if (!m_settings->value("SAVE_HISTORY", true).toBool())
		return retval;
	int size = m_settings->beginReadArray("HISTORY_LIST");
	for (int i = 0; i < size; i++)
	{
		m_settings->setArrayIndex(i);
		QVariant var = m_settings->value(QString("HISTORY"));
		Game gm = var.value<Game>();
		retval << gm;
	}
	m_settings->endArray();
	return retval;
}

void Frontend::setHistory(QList<Game> list)
{
	if (!m_settings->value("SAVE_HISTORY", true).toBool())
		return;
	int initHistCount = m_history->size();
	m_settings->beginGroup("HISTORY_LIST");
	m_settings->remove("");
	m_settings->endGroup();
	m_history->clear();
	m_settings->beginWriteArray("HISTORY_LIST");
	for (int i = 0; i < list.size(); i++)
	{
		Game gm = list[i];
		m_settings->setArrayIndex(i);
		m_settings->setValue(QString("HISTORY"), qVariantFromValue(gm));
		QVariantMap map;
		map["name"] = gm.name();
		map["time"] = gm.date();
		map["resource"] = gm.resource();
		map["location"] = gm.location();
		map["uuid"] = gm.uuid();
		(*m_history) << map;
	}
	m_settings->endArray();
	m_settings->sync();
	int endHistCount = m_history->size();
	if ((initHistCount > 0 && endHistCount == 0) || (initHistCount == 0 && endHistCount > 0))
		emit hasHistoryChanged();
}

void Frontend::addToHistory(QString title)
{
	if (!m_settings->value("SAVE_HISTORY", true).toBool())
		return;
	QList<Game> history = getHistory();
	Game gm(title);
	QString tmp = getValueFor("boxart", "true");
	bool boxart = false;
	if (QString::compare(tmp, "true", Qt::CaseInsensitive) == 0)
		boxart = true;
	for (int i = 0; i < history.size(); i++)
	{
		if (QString::compare(history[i].name(), title, Qt::CaseInsensitive) == 0)
		{
			gm = history.takeAt(i);
			break;
		}
	}
    QString selectedFile = mRom.mid(mRom.lastIndexOf('/') + 1);
    int tmp2 = selectedFile.indexOf("(");
    QString imageSource;
    if(tmp2 == -1) {
        imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + selectedFile.mid(0, selectedFile.lastIndexOf(".")).trimmed() + ".jpg";
    } else {
        imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + selectedFile.mid(0, tmp2).trimmed() + ".jpg";
    }
	if (gm.resource().length() == 0)
	{
		if (boxart)
			gm.resource(imageSource);
		else
			gm.resource("asset:///images/mupen64plus.png");
	}
	else if (boxart && QString::compare("asset:///images/mupen64plus.png", gm.resource()) == 0)
		gm.resource(imageSource);
	gm.location(mRom);
	gm.playNow();
	history.insert(0, gm);
	setHistory(history);
}

void Frontend::removeFromHistory(QString uuid)
{
	QUuid id(uuid);
	QList<Game> history = getHistory();
	for (int i = 0; i < history.size(); i++)
	{
		if (history[i] == id)
		{
			history.removeAt(i);
			setHistory(history);
			break;
		}
	}
}

void Frontend::clearHistory()
{
	if (m_history->size() == 0)
		return;
	m_settings->beginGroup("HISTORY_LIST");
	m_settings->remove("");
	m_settings->endGroup();
	m_history->clear();
	emit hasHistoryChanged();
}

void Frontend::startEmulator(bool start_now)
{
	//start();
	Q_UNUSED(start_now);
	int msg = 1;
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);
	QString goodname(m64p->l_RomName);
	addToHistory(goodname);
	return;
}

//Getters and Setters
QString Frontend::getRom()
{
    return mRom;
}

void Frontend::setRom(QString i)
{
    mRom = i;
    m64p->SetRom(mRom.toAscii().constData());
    emit romChanged(mRom);
}

void Frontend::LoadRom()
{
	m64p->LoadRom();
}

int Frontend::getVideo()
{
    return mVideoPlugin;
}

void Frontend::setVideo(int i)
{
    mVideoPlugin = i;
    emit videoChanged(mVideoPlugin);
}

int Frontend::getAudio()
{
    return mAudio;
}

void Frontend::setAudio(int i)
{
    mAudio = i;
    emit audioChanged(mAudio);
}

QSignalMapper *signalMapperToggle;
QSignalMapper *signalMapperDropDown;

void Frontend::addCheatToggle(int number){
	ToggleButton *senderButton = (ToggleButton*)signalMapperToggle->mapping(number);

	if(senderButton->isChecked()){
		CheatList.append(QString("%1,").arg(number));
	} else {
		CheatList.remove(QString("%1,").arg(number));
	}
}

void Frontend::addCheatDropDown(int number){
	DropDown *senderDropDown = (DropDown*)signalMapperDropDown->mapping(number);

	int foundLoc = -1;
	int removeUntil = 0;

	//to remove, index=cheat number. We need to see if it's already there, and remove it.
	//1,2,3-4, or 3-4,1,2

	foundLoc = CheatList.indexOf(QString("%1-").arg(number));
	if(foundLoc != -1){
		removeUntil = CheatList.indexOf(",", foundLoc);
		CheatList.remove(foundLoc, removeUntil-foundLoc+1);
	}

	if(senderDropDown->selectedIndex() > 0 ){
		CheatList.append(QString("%1-%2,").arg(number).arg(senderDropDown->selectedIndex()-1));
	}

	fflush(stdout);
}

Container * Frontend::createCheatToggle(sCheatInfo *pCur) {
	ToggleButton *toggle;

	Container *returnContainer = Container::create().top(20.0f);
	TextStyle *italicStyle = new TextStyle(SystemDefaults::TextStyles::bodyText());
	italicStyle->setFontStyle(FontStyle::Italic);

	//Container with Label and Toggle
	Container *CheatToggle = Container::create();

	StackLayout *pStackLayout = new StackLayout();
	pStackLayout->setOrientation( LayoutOrientation::LeftToRight );
	CheatToggle->setLayout(pStackLayout);

	CheatToggle->add(Label::create().text(QString(pCur->Name))
									.vertical(VerticalAlignment::Center)
									.preferredWidth(768.0f)
									.multiline(true)
									);
	toggle = ToggleButton::create().vertical(VerticalAlignment::Center);

	signalMapperToggle->setMapping(toggle, pCur->Number);
	connect(toggle, SIGNAL(checkedChanged(bool)), signalMapperToggle, SLOT(map()));

	CheatToggle->add(toggle);

	returnContainer->add(CheatToggle);

	//Container with description label
	if(pCur->Description != NULL){
		Container *CheatDescription = Container::create().bottom(20.0f);
		CheatDescription->add(Label::create().text(QString(pCur->Description))
				.vertical(VerticalAlignment::Center)
				.preferredWidth(768.0f)
				.multiline(true)
				.textStyle(*italicStyle)
				);
		returnContainer->add(CheatDescription);
	}

	returnContainer->add(Divider::create());

	return returnContainer;
}

Container * Frontend::createCheatDropDown(sCheatInfo *pCur){
	DropDown *dropDown;
	Option * tmp;

	Container *returnContainer = Container::create().top(20.0f);
	TextStyle *italicStyle = new TextStyle(SystemDefaults::TextStyles::bodyText());
	italicStyle->setFontStyle(FontStyle::Italic);

	dropDown = DropDown::create().vertical(VerticalAlignment::Center)
			                     .title(QString(pCur->Name));

	tmp = Option::create().text("Disable").value(0);
	dropDown->add(tmp);

	for (int i = 0; i < pCur->Codes[pCur->VariableLine].var_count; i++) {
		//printf("      %i: %s\n", i, pCur->Codes[pCur->VariableLine].variable_names[i]);
		tmp = Option::create().text(QString("%1").arg(pCur->Codes[pCur->VariableLine].variable_names[i]))
				              .value(i+1);
		dropDown->add(tmp);
	}


	signalMapperDropDown->setMapping(dropDown, pCur->Number);
	connect(dropDown, SIGNAL(selectedIndexChanged(int)), signalMapperDropDown, SLOT(map()));

	returnContainer->add(dropDown);

	//Container with description label
	if(pCur->Description != NULL){
		Container *CheatDescription = Container::create().bottom(20.0f);
		CheatDescription->add(Label::create().text(QString(pCur->Description))
				.vertical(VerticalAlignment::Center)
				.preferredWidth(768.0f)
				.multiline(true)
				.textStyle(*italicStyle)
				);
		returnContainer->add(CheatDescription);
	}

	returnContainer->add(Divider::create());

	return returnContainer;
}

void Frontend::createCheatsPage(){
	char numCheats = 0;
	CheatStart(CHEAT_SHOW_LIST, &numCheats);
	fflush(stdout);

	signalMapperToggle = new QSignalMapper(this);
	connect(signalMapperToggle, SIGNAL(mapped(int)), this, SLOT(addCheatToggle(int)));

	signalMapperDropDown = new QSignalMapper(this);
	connect(signalMapperDropDown, SIGNAL(mapped(int)), this, SLOT(addCheatDropDown(int)));

	mCheatsContainer->removeAll();
	mCheatsContainer->setLeftPadding(20.0f);
	mCheatsContainer->setRightPadding(20.0f);
	sCheatInfo *pCur = l_CheatList;
	while (pCur != NULL)
	{
		if (pCur->VariableLine == -1){
			if (pCur->Description == NULL){
				//printf("   %i: %s\n", pCur->Number, pCur->Name);
				mCheatsContainer->add(createCheatToggle(pCur));
			}
			else {
				//printf("   %i: %s (%s)\n", pCur->Number, pCur->Name, pCur->Description);
				mCheatsContainer->add(createCheatToggle(pCur));
			}
		//TODO: Check if this is true and make a dropdown
		} else {
			//int i;
			mCheatsContainer->add(createCheatDropDown(pCur));
			//for (i = 0; i < pCur->Codes[pCur->VariableLine].var_count; i++)
				//printf("      %i: %s\n", i, pCur->Codes[pCur->VariableLine].variable_names[i]);
		}
		pCur = pCur->Next;
	}

	CheatFreeAll();
}

int Frontend::getInputValue(int player, QString value){
	int i;

	if(value == "present") {
		return m64p->controller[player].present;
	}
	if(value == "device") {
		return m64p->controller[player].device;
	}
	if(value == "layout") {
		return m64p->controller[player].layout;
	}
	if (value == "controller") {
	}

	for(i = 0; i < 16; ++i) {
		if(button_names[i] == value) {
			return m64p->controller[player].button[i];
		}
	}

	if(value == "X Axis Left") {
		return m64p->controller[player].axis[0].a;
	} else if (value == "X Axis Right") {
		return m64p->controller[player].axis[0].b;
	} else if (value == "Y Axis Up") {
		return m64p->controller[player].axis[1].a;
	} else if (value == "Y Axis Down") {
		return m64p->controller[player].axis[1].b;
	}
	return -1;
}

QString Frontend::getMogaInputCharacter(int value)
{
	switch (value) {
	case 1 << A_BUTTON:
		return "A";
	case 1 << B_BUTTON:
		return "B";
	case 1 << C_BUTTON:
		return "C";
	case 1 << X_BUTTON:
		return "X";
	case 1 << Y_BUTTON:
		return "Y";
	case 1 << Z_BUTTON:
		return "Z";
	case 1 << MENU1_BUTTON:
		return "Menu1";
	case 1 << MENU2_BUTTON:
		return "Menu2";
	case 1 << MENU3_BUTTON:
		return "Menu3";
	case 1 << MENU4_BUTTON:
		return "Menu4";
	case 1 << L1_BUTTON:
		return "L1";
	case 1 << L2_BUTTON:
		return "L2";
	case 1 << L3_BUTTON:
		return "L3";
	case 1 << R1_BUTTON:
		return "R1";
	case 1 << R2_BUTTON:
		return "R2";
	case 1 << R3_BUTTON:
		return "R3";
	case 1 << DPAD_UP_BUTTON:
		return "Dpad Up";
	case 1 << DPAD_DOWN_BUTTON:
		return "Dpad Down";
	case 1 << DPAD_LEFT_BUTTON:
		return "Dpad Left";
	case 1 << DPAD_RIGHT_BUTTON:
		return "Dpad Right";
	case 1 << EXT_BUTTON_L4:
		return "L4";
	case 1 << EXT_BUTTON_R4:
		return "R4";
	default:
		return "Invalid";
	}
}

QString Frontend::getInputCharacter(int value)
{
	if (value < 0)
		return "Invalid";
	if (value >= KEYCODE_A && value <= KEYCODE_Z)
		return QString(QChar((char)(value - KEYCODE_A + Qt::Key_A)));
	if (value >= KEYCODE_CAPITAL_A && value <= KEYCODE_CAPITAL_Z)
		return QString(QChar((char)(value - KEYCODE_CAPITAL_A + Qt::Key_A))).toUpper();
	switch (value) {
	case KEYCODE_ESCAPE:
		return "Escape";
	case KEYCODE_RETURN:
		return "Enter";
	case KEYCODE_LEFT:
		return "Left";
	case KEYCODE_RIGHT:
		return "Right";
	case KEYCODE_UP:
		return "Up";
	case KEYCODE_DOWN:
		return "Down";
	case KEYCODE_BACKSPACE:
		return "Backspace";
	case KEYCODE_LEFT_SHIFT:
		return "Left Shift";
	case KEYCODE_RIGHT_SHIFT:
		return "Right Shift";
	case KEYCODE_LEFT_CTRL:
		return "Left Control";
	case KEYCODE_RIGHT_CTRL:
		return "Right Control";
	case KEYCODE_LEFT_ALT:
		return "Left Alt";
	case KEYCODE_RIGHT_ALT:
		return "Right Alt";
	case KEYCODE_TAB:
		return "Tab";
	case KEYCODE_INSERT:
		return "Insert";
	case KEYCODE_DELETE:
		return "Delete";
	case KEYCODE_HOME:
		return "Home";
	case KEYCODE_END:
		return "End";
	case KEYCODE_PG_UP:
		return "Page Up";
	case KEYCODE_PG_DOWN:
		return "Page Down";
	case KEYCODE_KP_PLUS:
		return "KP Plus";
	case KEYCODE_KP_MINUS:
		return "KP Minus";
	case KEYCODE_KP_MULTIPLY:
		return "KP Mul";
	case KEYCODE_KP_DIVIDE:
		return "KP Div";
	case KEYCODE_KP_ENTER:
		return "KP Ent";
	case KEYCODE_KP_HOME:
		return "KP Home";
	case KEYCODE_KP_UP:
		return "KP Up";
	case KEYCODE_KP_PG_UP:
		return "KP Pg Up";
	case KEYCODE_KP_LEFT:
		return "KP Left";
	case KEYCODE_KP_FIVE:
		return "KP 5";
	case KEYCODE_KP_RIGHT:
		return "KP Right";
	case KEYCODE_KP_END:
		return "KP End";
	case KEYCODE_KP_DOWN:
		return "KP Down";
	case KEYCODE_KP_PG_DOWN:
		return "KP Pg Dn";
	case KEYCODE_KP_INSERT:
		return "KP Insert";
	case KEYCODE_KP_DELETE:
		return "KP Delete";
	case KEYCODE_F1:
	case KEYCODE_F2:
	case KEYCODE_F3:
	case KEYCODE_F4:
	case KEYCODE_F5:
	case KEYCODE_F6:
	case KEYCODE_F7:
	case KEYCODE_F8:
	case KEYCODE_F9:
	case KEYCODE_F10:
	case KEYCODE_F11:
	case KEYCODE_F12:
		return "F" + QString::number(value - KEYCODE_F1 + 1);
	case KEYCODE_SPACE:
		return "Space";
	case KEYCODE_EXCLAM:
		return "!";
	case KEYCODE_QUOTE:
		return "\"";
	case KEYCODE_NUMBER:
		return "#";
	case KEYCODE_DOLLAR:
		return "$";
	case KEYCODE_PERCENT:
		return "%";
	case KEYCODE_AMPERSAND:
		return "&";
	case KEYCODE_APOSTROPHE:
		return "'";
	case KEYCODE_LEFT_PAREN:
		return "(";
	case KEYCODE_RIGHT_PAREN:
		return ")";
	case KEYCODE_ASTERISK:
		return "*";
	case KEYCODE_PLUS:
		return "+";
	case KEYCODE_COMMA:
		return ",";
	case KEYCODE_MINUS:
		return "-";
	case KEYCODE_PERIOD:
		return ".";
	case KEYCODE_SLASH:
		return "/";
	case KEYCODE_ZERO:
		return "0";
	case KEYCODE_ONE:
		return "1";
	case KEYCODE_TWO:
		return "2";
	case KEYCODE_THREE:
		return "3";
	case KEYCODE_FOUR:
		return "4";
	case KEYCODE_FIVE:
		return "5";
	case KEYCODE_SIX:
		return "6";
	case KEYCODE_SEVEN:
		return "7";
	case KEYCODE_EIGHT:
		return "8";
	case KEYCODE_NINE:
		return "9";
	case KEYCODE_COLON:
		return ":";
	case KEYCODE_SEMICOLON:
		return ";";
	case KEYCODE_LESS_THAN:
		return "<";
	case KEYCODE_EQUAL:
		return "=";
	case KEYCODE_GREATER_THAN:
		return ">";
	case KEYCODE_QUESTION:
		return "?";
	case KEYCODE_AT:
		return "@";
	case KEYCODE_LEFT_BRACKET:
		return "[";
	case KEYCODE_BACK_SLASH:
		return "\\";
	case KEYCODE_RIGHT_BRACKET:
		return "]";
	case KEYCODE_UNDERSCORE:
		return "_";
	case KEYCODE_LEFT_BRACE:
		return "{";
	case KEYCODE_BAR:
		return "|";
	case KEYCODE_RIGHT_BRACE:
		return "}";
	case KEYCODE_TILDE:
		return "~";
	default:
		return QString::number(value) + "?";
	}
}

void Frontend::setInputValue(int player, QString button, int value){
	int i;

	if(button == "present"){
		m64p->controller[player].present = value;
		return;
	}
	if(button == "device") {
		m64p->controller[player].device = value;
		return;
	}
	if(button == "plugin"){
		m64p->controller[player].plugin = value;
		return;
	}
	if(button == "layout"){
		m64p->controller[player].layout = value;
		return;
	}

	for(i=0; i<16; ++i){
		if(button_names[i] == button){
			m64p->controller[player].button[i] = value;
			return;
		}
	}

	if(button == "X Axis Left"){
		m64p->controller[player].axis[0].a = value;
		return;
	}
	if (button == "X Axis Right"){
		m64p->controller[player].axis[0].b = value;
		return;
	}
	if (button == "Y Axis Up"){
		m64p->controller[player].axis[1].a = value;
		return;
	}
	if (button == "Y Axis Down"){
		m64p->controller[player].axis[1].b = value;
		return;
	}
}

void Frontend::setControllerID(int player, QString value)
{
	strcpy(m64p->controller[player].gamepadId, value.left(63).toAscii().data());
	m64p->controller[player].gamepadId[63] = '\0';
}

QString Frontend::getControllerID(int player)
{
	return QString::fromAscii(m64p->controller[player].gamepadId);
}

int Frontend::getControllerIndex(int player)
{
	QString did = getControllerID(player);
	for (int i = 0; i < m_devices->size(); i++)
	{
		QString id = m_devices->value(i)["id"].toString();
		if (QString::compare(did, id) == 0)
			return i + 1;
	}
	return 0;
}

void Frontend::setControllerIndex(int player, int index)
{
	QString id = m_devices->value(index - 1)["id"].toString();
	setControllerID(player, id);
}

int vkbToMoga(int moga)
{
	switch (moga)
	{
	case 1:
		return 1;
	case 2:
		return 2;
	case 8:
		return 3;
	case 16:
		return 4;
	case 64:
		return 5;
	case 128:
		return 6;
	case 1024:
		return 7;
	case 2048:
		return 8;
	case 4096:
		return 9;
	case 8192:
		return 10;
	case 16384:
		return 11;
	case 32768:
		return 12;
	case 65536:
		return 13;
	case 131072:
		return 14;
	case 262144:
		return 15;
	case 524288:
		return 16;
	}
	return -1;
}

int mogaToVkb(int vkb)
{
	switch (vkb)
	{
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 8;
	case 4:
		return 16;
	case 5:
		return 64;
	case 6:
		return 128;
	case 7:
		return 1024;
	case 8:
		return 2048;
	case 9:
		return 4096;
	case 10:
		return 8192;
	case 11:
		return 16384;
	case 12:
		return 32768;
	case 13:
		return 65536;
	case 14:
		return 131073;
	case 15:
		return 262144;
	case 16:
		return 524288;
	}
	return -1;
}

void Frontend::setMogaInputValue(int player, QString button, int index)
{
	int vkb = mogaToVkb(index);
	if(button == "X Axis Left") {
		m64p->controller[player].axis[0].a = vkb;
		return;
	}
	if (button == "X Axis Right") {
		m64p->controller[player].axis[0].b = vkb;
		return;
	}
	if (button == "Y Axis Up") {
		m64p->controller[player].axis[1].a = vkb;
		return;
	}
	if (button == "Y Axis Down") {
		m64p->controller[player].axis[1].b = vkb;
		return;
	}
	for(int i=0; i<16; ++i)
	{
		if(button_names[i] == button) {
			m64p->controller[player].button[i] = vkb;
			return;
		}
	}
}

int Frontend::getMogaInputValue(int player, QString button)
{
	if(button == "X Axis Left")
		return vkbToMoga(m64p->controller[player].axis[0].a);
	if (button == "X Axis Right")
		return vkbToMoga(m64p->controller[player].axis[0].b);
	if (button == "Y Axis Up")
		return vkbToMoga(m64p->controller[player].axis[1].a);
	if (button == "Y Axis Down")
		return vkbToMoga(m64p->controller[player].axis[1].b);
	for(int i=0; i<16; ++i)
	{
		if(button_names[i] == button)
			return vkbToMoga(m64p->controller[player].button[i]);
	}
	return 0;
}

void Frontend::SaveState(){
	m64p->SaveState();
}

void Frontend::LoadState() {
	m64p->LoadState();
}

void Frontend::LoadTouchOverlay() {
	m64p->LoadTouchOverlay();
}

void Frontend::ExitEmulator() {
	m64p->ExitEmulator();
	m_emuRunning = false;
}

int Frontend::mapButton(){

	int msg = 0, ret = 0;

	toastButton->cancel();
	toast->setBody("Press a Button to Map");
	toast->show();

	MsgSend(coid, &msg, sizeof(msg), &ret, sizeof(ret));

	toast->cancel();
	if(ret != -1){
		toastButton->setBody("Button Pressed");
	} else {
		toastButton->setBody("Canceling");
	}
	toastButton->show();

	return ret;
}

//Getters and Setters
ImageLoader* Frontend::getBoxArt()
{
    return m_boxart;
}

bool Frontend::boxartLoaded()
{
	return m_boxartLoaded;
}

void Frontend::loadBoxArt(const QString &url)
{
	m_boxartLoaded = false;
	emit boxartLoadedChanged(m_boxartLoaded);

	TwitterRequest* request = new TwitterRequest(this);
	connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onBoxArtRecieved(QString, bool)));
	request->requestTimeline(url);
}

void Frontend::onBoxArtRecieved(const QString &info, bool success)
{
    TwitterRequest *request = qobject_cast<TwitterRequest*>(sender());

    if (success) {
    	XmlDataAccess dataAccess;
    	QString url = "";

		const QVariant variant = dataAccess.loadFromBuffer(info);

		const QVariantMap games = variant.toMap();

		//qDebug() << "URL: " << games["baseImgUrl"];
		url.append(games["baseImgUrl"].toString());

		//IF there is one game, games["game"] will be a map rather than list
		QVariantList game = games["Game"].toList();

		//TODO: Will crash with one game
		//qDebug() << "GAME: " << game;

		QVariantMap selected;
		if(!game.isEmpty()){
			selected = game[0].toMap();
		} else {
			selected = games["Game"].toMap();
		}
		QVariantMap images = selected["Images"].toMap();
		QVariantList boxart = images["boxart"].toList();

		url.append((boxart[1].toMap())[".data"].toString());

		//qDebug() << "FULL URL: " << url;

		//Could make obj once, and reuse
		if(m_boxart) {
			delete m_boxart;
		}

		m_boxart = new ImageLoader(url, mRom);
		emit boxArtChanged(m_boxart);

		m_boxart->load();

		m_boxartLoaded = true;
		emit boxartLoadedChanged(m_boxartLoaded);
    }

    request->deleteLater();
}

void Frontend::onVersionRecieved(const QString &info, bool success)
{
    TwitterRequest *request = qobject_cast<TwitterRequest*>(sender());

    if (success)
    {
    	QStringList list = info.split('.', QString::SkipEmptyParts);
    	if (list.length() == 3)
    	{
    		bool ok;
    		int maj = list[0].toInt(&ok);
    		if (ok)
    		{
    			int min = list[1].toInt(&ok);
    			if (ok)
    			{
    				int rel = list[2].toInt(&ok);
    				if (ok)
    				{
    					if (maj > VERSION_MAJOR || min > VERSION_MINOR || rel > VERSION_RELEASE)
    					{
    						SystemToast *toast = new SystemToast();
    						toast->setBody(QString("Version %1.%2.%3 of Mupen64+ BB is available to upgrade your %4.%5.%6")
    								.arg(list[0], list[1], list[2],
    										QString::number(VERSION_MAJOR), QString::number(VERSION_MINOR), QString::number(VERSION_RELEASE)));
    						toast->show();
    					}
    				}
    			}
    		}
    	}
    }

    request->deleteLater();
}

//http://supportforums.blackberry.com/t5/Native-Development-Knowledge/Gamepad-Vendor-and-Product-IDs/ta-p/2833184
static QString detect_gamepad_type(GameController* controller)
{
	if (strstr(controller->id, "057E-0306") || strstr(controller->id, "057E-0330"))
	{
		strlcpy(controller->name, "Wiimote", sizeof(controller->name));
		return "asset:///images/wii.png";
	}
	else if (strstr(controller->id, "20D6-0DAD"))
	{
		strlcpy(controller->name, "MOGA Pro", sizeof(controller->name));
		return "asset:///images/moga.png";
	}
	else if (strstr(controller->id, "25B6-0001"))
	{
		strlcpy(controller->name, "Fructel Gametel", sizeof(controller->name));
		return "asset:///images/gametel.png";
	}
	else if (strstr(controller->id, "1038-1412"))
	{
		strlcpy(controller->name, "SteelSeries Free", sizeof(controller->name));
		return "asset:///images/steelseries.png";
	}
	else if (strstr(controller->id, "046D-C21D"))
	{
		strlcpy(controller->name, "Logitech F310", sizeof(controller->name));
		return "asset:///images/logitech.png";
	}
	else if (strstr(controller->id, "045E-028E"))
	{
		strlcpy(controller->name, "Microsoft XBox 360", sizeof(controller->name));
		return "asset:///images/xbox360.png";
	}
	else if (strstr(controller->id, "045E-0291"))
	{
		strlcpy(controller->name, "Microsoft XBox 360 Wireless", sizeof(controller->name));
		return "asset:///images/xbox360w.png";
	}
	else if (strstr(controller->id, "1689-FD01"))
	{
		strlcpy(controller->name, "Razer XBox 360", sizeof(controller->name));
		return "asset:///images/razer.png";
	}
	strlcpy(controller->name, "Gamepad", sizeof(controller->name));
	return "asset:///images/ca_bluetooth.png";
}

void Frontend::discoverControllers()
{
	int deviceCount;
	screen_get_context_property_iv(screen_cxt, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount);
	fprintf(stderr, "%d connected devices\n", deviceCount);
	screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));
	screen_get_context_property_pv(screen_cxt, SCREEN_PROPERTY_DEVICES, (void**)devices);

	// Scan the list for gamepad and joystick devices.
	int i;
	int controllerIndex = 0;
	for (i = 0; i < deviceCount; i++) {
		int type;
		screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type);

		if (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK) {
			fprintf(stderr, "Found a gamepad\n");
			GameController *control = &_controllers[controllerIndex];
			control->handle = devices[i];
			screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &control->type);
			screen_get_device_property_cv(devices[i], SCREEN_PROPERTY_ID_STRING, sizeof(control->id), control->id);
			screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_BUTTON_COUNT, &control->buttonCount);
			fprintf(stderr, "Gamepad %s with %d buttons\n", control->id, control->buttonCount);
			QVariantMap map;
			map["id"] = QString::fromAscii(control->id);
			QString res = detect_gamepad_type(control);
			if (control->type == SCREEN_EVENT_GAMEPAD)
			{
				sprintf(control->deviceString, "Gamepad device id: %s", control->id);
				map["name"] = QString::fromAscii(control->name) + " (" + QString(control->id) + ")";
			}
			else
			{
				sprintf(control->deviceString, "Joystick device id: %s", control->id);
				map["name"] = "Joystick (" + QString(control->id) + ")";
			}
			QString desc = "Has " + QString::number(control->buttonCount, 10) + " buttons";
			if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG0, control->analog0)) {
				control->analogCount++;
				fprintf(stderr, "Device has analog0\n");
				if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG1, control->analog1)) {
					control->analogCount++;
					fprintf(stderr, "Device has analog1\n");
					desc += " with analog0 and analog1";
				}
				else
					desc += " with analog0";
			}
			else if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG1, control->analog1)) {
				control->analogCount++;
				fprintf(stderr, "Device has analog1\n");
				desc += " with analog1";
			}
			map["description"] = desc;
			(*m_devices) << map;
			emit createOption(QString::fromAscii(control->name), QUrl(res));
			controllerIndex++;
		}
	}

	free(devices);
	emit controllersDetected();
}

void Frontend::onCreateOption(QString name, QUrl imageSource)
{
	for (int i = 0; i < 4; i++)
	{
		DropDown* deviceDropdown = m_tab->findChild<DropDown*>("deviceList" + QString::number(i));
		if (deviceDropdown)
		{
			Option *opt = Option::create()
							.parent(deviceDropdown)
							.text(name)
							.imageSource(imageSource);
			deviceDropdown->add(opt);
		}
	}
}

void Frontend::detectHDMI()
{
	/*int hdmi = DisplayInfo::secondaryDisplayId();
	if (hdmi >= 0)
	{
		m_hdmiInfo = new DisplayInfo(hdmi, this);
		if (m_hdmiInfo->isAttached())
		{
			fprintf(stderr, "Second Display: %s\nSize: (%d, %d)\n", m_hdmiInfo->displayName().toStdString().c_str(),
					m_hdmiInfo->pixelSize().width(), m_hdmiInfo->pixelSize().height());
			emit hdmiDetected(true);
			connect(m_hdmiInfo, SIGNAL(attachedChanged(bool)), this, SIGNAL(hdmiDetected(bool)));
		}
	}*/
}

void Frontend::onInvoke(const bb::system::InvokeRequest& request)
{
	QString file = request.uri().toString();
	if (file.startsWith("file://"))
		file = file.mid(7);
	emit invoked(file);
}
