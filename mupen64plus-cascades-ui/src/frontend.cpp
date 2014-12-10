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
#include "BPS/BpsEventHandler.hpp"
#include "History/Game.hpp"
#include "main.h"
#include "bbutil.h"
#include "m64p_types.h"
#include "core_interface.h"
#include "GameInfo/GameImage.hpp"

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <climits>
#include <utime.h>

#include <QSettings>
#include <QSignalMapper>

#include <bb/cascades/QmlDocument>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/DropDown>
#include <bb/cascades/Divider>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/Label>
#include <bb/cascades/FontStyle>
#include <bb/cascades/TextStyle>
#include <bb/cascades/StackLayout>
#include <bb/cascades/DockLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/Theme>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/VisualStyle>
#include <bb/cascades/Option>
#include <bb/cascades/Window>

#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FileType>

#include <bb/data/XmlDataAccess>

#include <bb/platform/HomeScreen>
#include <bb/platform/PlatformInfo>

#include <bb/system/SystemToast>
#include <bb/system/SystemListDialog>
#include <bb/system/SystemDialog>
#include <bb/system/SystemPrompt>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>

#include <bb/device/Led>

#include <bb/Packageinfo>

#include <bps/bps.h>
#include <bps/screen.h>
#include <bps/event.h>
#include <bps/navigator_invoke.h>

#include <sys/keycodes.h>
#include <sys/neutrino.h>

#include <zip.h>
#include <unzip.h>

using namespace bb::cascades::pickers;
using namespace bb::cascades;
using namespace bb::platform;
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
extern bool first_on_new_settings;
extern BpsEventHandler *s_handler;

extern Frontend *mainApp;

QQueue<QPair<int, unsigned char*> > Frontend::s_screenQueue;
int Frontend::s_isOsThree = -1;

#define COLORIZE(color) ((float)(((double)(color)) / 255.0))

enum GamePadButton
{
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

struct GameController
{
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

void touch_close_menu()
{
    m64p_emit_touch = false;
    QTimer::singleShot(0, mainApp, SLOT(swipedown_external()));
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
	if (screen_request_events(screen_cxt) != BPS_SUCCESS)
	{
	    printf("Error getting screen events\n");fflush(stdout);
	}
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

Frontend::Frontend()
{
    m_gameSettings = new M64PSettings();
    if (m_gameSettings->Settings()->Theme() < 0)
    {
        ThemeSupport* themeSupport = Application::instance()->themeSupport();
        Theme* currentTheme = themeSupport->theme();
        ColorTheme* colorTheme = currentTheme->colorTheme();
        VisualStyle::Type style = colorTheme->style();
        switch (style)
        {
        case VisualStyle::Bright:
            m_gameSettings->Settings()->Theme(0);
            break;
        case VisualStyle::Dark:
            m_gameSettings->Settings()->Theme(1);
            break;
        }
    }
	qmlRegisterType<bb::cascades::pickers::FilePicker>("bb.cascades.pickers", 1, 0, "FilePicker");
	qmlRegisterUncreatableType<bb::cascades::pickers::FileType>("bb.cascades.pickers", 1, 0, "FileType", "");
	qmlRegisterType<ImageLoader>();
    qmlRegisterType<GameImage>("travis.lib", 1, 0, "GameImage");
    qmlRegisterType<GameInfo>("travis.lib", 1, 0, "GameInfo");
	qRegisterMetaType<Game>("Game");
	qRegisterMetaTypeStreamOperators<Game>("Game");
	qmlRegisterType<INIEntry>("travis.lib", 1, 0, "INIEntry");
	qmlRegisterType<RiceINI>("travis.lib", 1, 0, "RiceINI");

    // Set the application organization and name, which is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Mupen64Plus-BB");

	bb::PackageInfo pacInfo;
	QStringList version = pacInfo.version().split('.');
	bool ok = false;
	if (version.length() == 4)
	{
	    VERSION_MAJOR = version[0].toInt(&ok);
	    if (!ok) goto NOT_OK;
	    VERSION_MINOR = version[1].toInt(&ok);
        if (!ok) goto NOT_OK;
	    VERSION_RELEASE = version[2].toInt(&ok);
	}
NOT_OK:
	if (!ok)
	{
	    VERSION_MAJOR = INT_MAX;
	    VERSION_MINOR = INT_MAX;
	    VERSION_RELEASE = INT_MAX;
	}

	//Set up a
	m_hdmiInfo = NULL;
	m_useHdmi = false;
	m_emuRunning = false;
	m_boxart = 0;
	m_boxartLoaded = false;
	m_menuOffset = 0;
	m_devices = new QMapListDataModel();
	m_history = new QMapListDataModel();
	m_menuAnimation = new QPropertyAnimation(this, "menuOffset");
	m_animationLock = new QMutex();
	m_menuAnimation->setDuration(250);
	m_coverImage = "asset:///images/mupen64plus.png";
	m_numMenuItems = 0;
	m_noTouchScreenControllers = true;
	m_selectStateSaving = false;
	m_loadingGame = true;
	m_riceini.ReadIniFile();

	touch_callback = &touch_close_menu;

	QTime now = QTime::currentTime();
	qsrand(now.msec());

	m_imageName = m_settings->value("IMAGE_RESOURCE_NAME", "").toString();

	if(access("shared/misc/n64/", F_OK) != 0)
		mkdir("shared/misc/n64/", S_IRWXU | S_IRWXG);

	if (access("shared/misc/n64/data", F_OK) != 0)
		mkdir("shared/misc/n64/data", S_IRWXU | S_IRWXG);
    if (access("shared/misc/n64/roms", F_OK) != 0)
		mkdir("shared/misc/n64/roms", S_IRWXU | S_IRWXG);
    if (access("shared/misc/n64/.boxart", F_OK) != 0)
		mkdir("shared/misc/n64/.boxart", S_IRWXU | S_IRWXG);
    if (access("shared/misc/n64/save", F_OK) != 0)
        mkdir("shared/misc/n64/save", S_IRWXU | S_IRWXG);
    if (access("shared/misc/n64/hires_texture", F_OK) != 0)
        mkdir("shared/misc/n64/hires_texture", S_IRWXU | S_IRWXG);

    if (!QFile::exists("shared/misc/n64/data/mupen64plus.cfg"))
    {
		char buf[8192];
		size_t size;

		FILE* source = fopen("app/native/mupen64plus.cfg.2", "rb");
		FILE* dest = fopen("shared/misc/n64/data/mupen64plus.cfg", "wb");

		// clean and more secure
		// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

		while ((size = fread(buf, 1, 8192, source)))
		{
			fwrite(buf, 1, size, dest);
		}

		fclose(source);
		fclose(dest);
	}

	m64p = new Emulator((char *)Application::instance()->mainWindow()->groupId().toAscii().constData(), (char*)QString("emulator_m64p").toAscii().constData());
	connect(m64p, SIGNAL(focusScreen()), SIGNAL(FocusForeignWindow()));
	//m64p->print_controller_config();

	if (first_on_new_settings)
	{
	    QSettings settings;
	    QVariant val = settings.value("gles2n64");
	    if (!val.isNull())
	    {
	        if (QString::compare(val.toString(), "true", Qt::CaseInsensitive) == 0)
	            m_gameSettings->VideoPlugin(1);
	    }
	    for (int i = 0; i < 4; i++)
	    {
	        if (m64p->controller[i].present)
	        {
	            if (m64p->controller[i].device == -2)
	            {
	                m_gameSettings->PlayerKeyboardSettings(i + 1)->DPadR(m64p->controller[i].button[0]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->DPadL(m64p->controller[i].button[1]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->DPadU(m64p->controller[i].button[2]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->DPadD(m64p->controller[i].button[3]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->Start(m64p->controller[i].button[4]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->ZTrigger(m64p->controller[i].button[5]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->BButton(m64p->controller[i].button[6]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->AButton(m64p->controller[i].button[7]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->CButtonR(m64p->controller[i].button[8]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->CButtonL(m64p->controller[i].button[9]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->CButtonD(m64p->controller[i].button[10]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->CButtonU(m64p->controller[i].button[11]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->RTrigger(m64p->controller[i].button[12]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->LTrigger(m64p->controller[i].button[13]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->XAxisLeft(m64p->controller[i].axis[0].a);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->XAxisRight(m64p->controller[i].axis[0].b);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->YAxisUp(m64p->controller[i].axis[1].a);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->YAxisDown(m64p->controller[i].axis[1].b);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->UpLeft(m64p->controller[i].diagonals[0]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->UpRight(m64p->controller[i].diagonals[1]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->DownLeft(m64p->controller[i].diagonals[2]);
                    m_gameSettings->PlayerKeyboardSettings(i + 1)->DownRight(m64p->controller[i].diagonals[3]);
                    if (m64p->controller[i].plugin == 5)
                        m_gameSettings->PlayerKeyboardSettings(i + 1)->RumblePak(true);
	            }
	            else if (m64p->controller[i].device == -3)
	            {
	                m_gameSettings->PlayerTouchscreenSettings(i + 1)->ControllerLayout(m64p->controller[i].layout);
                    if (m64p->controller[i].plugin == 5)
                        m_gameSettings->PlayerTouchscreenSettings(i + 1)->RumblePak(true);
	            }
	            else if (m64p->controller[i].device == -4)
	            {
                    m_gameSettings->PlayerGamepadSettings(i + 1)->DPadR(m64p->controller[i].button[0]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->DPadL(m64p->controller[i].button[1]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->DPadU(m64p->controller[i].button[2]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->DPadD(m64p->controller[i].button[3]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->Start(m64p->controller[i].button[4]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->ZTrigger(m64p->controller[i].button[5]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->BButton(m64p->controller[i].button[6]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->AButton(m64p->controller[i].button[7]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->CButtonR(m64p->controller[i].button[8]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->CButtonL(m64p->controller[i].button[9]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->CButtonD(m64p->controller[i].button[10]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->CButtonU(m64p->controller[i].button[11]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->RTrigger(m64p->controller[i].button[12]);
                    m_gameSettings->PlayerGamepadSettings(i + 1)->LTrigger(m64p->controller[i].button[13]);
                    if (m64p->controller[i].axis[0].a == -2)
                    {
                        m_gameSettings->PlayerGamepadSettings(i + 1)->AnalogStickType(1);
                    }
                    else if (m64p->controller[i].axis[0].a == -3)
                    {
                        m_gameSettings->PlayerGamepadSettings(i + 1)->AnalogStickType(2);
                    }
                    else
                    {
                        m_gameSettings->PlayerGamepadSettings(i + 1)->AnalogStickType(0);
                        m_gameSettings->PlayerGamepadSettings(i + 1)->XAxisLeft(m64p->controller[i].axis[0].a);
                        m_gameSettings->PlayerGamepadSettings(i + 1)->XAxisRight(m64p->controller[i].axis[0].b);
                        m_gameSettings->PlayerGamepadSettings(i + 1)->YAxisUp(m64p->controller[i].axis[1].a);
                        m_gameSettings->PlayerGamepadSettings(i + 1)->YAxisDown(m64p->controller[i].axis[1].b);
                    }
	            }
	            else if (m64p->controller[i].device == -5)
	            {
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->DPadR(m64p->controller[i].button[0]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->DPadL(m64p->controller[i].button[1]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->DPadU(m64p->controller[i].button[2]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->DPadD(m64p->controller[i].button[3]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->ZTrigger(m64p->controller[i].button[5]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->BButton(m64p->controller[i].button[6]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->AButton(m64p->controller[i].button[7]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->CButtonR(m64p->controller[i].button[8]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->CButtonL(m64p->controller[i].button[9]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->CButtonD(m64p->controller[i].button[10]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->CButtonU(m64p->controller[i].button[11]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->RTrigger(m64p->controller[i].button[12]);
                    m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->LTrigger(m64p->controller[i].button[13]);
                    if (m64p->controller[i].plugin == 5)
                        m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->RumblePak(true);
	            }
	        }
	    }
	}

	connect(m_menuAnimation, SIGNAL(finished()), SLOT(showMenuFinished()));
	connect(this, SIGNAL(menuOffsetChanged()), SLOT(onMenuOffsetChanged()));
	connect(this, SIGNAL(createOption(QString, QString, QUrl)), SLOT(onCreateOption(QString, QString, QUrl)));
    connect(s_handler, SIGNAL(PlayReleased()), SLOT(onPlayReleased()));
    connect(s_handler, SIGNAL(CapturePressed()), SLOT(onCapturePressed()));

	toast = new SystemToast(this);
	toastButton = new SystemToast(this);

	chid = ChannelCreate(0);
	coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);

    // Then we load the application.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_frontend", this);
    qml->setContextProperty("_settings", m_gameSettings);
    qml->setContextProperty("_game", &m_gameInfo);

    if (!qml->hasErrors())
    {
        if (debug_mode)
        {
            printf("qml has no errors, hooray\n");
            fflush(stdout);
        }
        m_tab = qml->createRootObject<TabbedPane>();
        if (m_tab)
        {
        	mCheatsContainer = m_tab->findChild<Container*>("cheats");

            Application::instance()->setScene(m_tab);

            if (m_gameSettings->Settings()->CheckVersion())
            {
                TwitterRequest* request = new TwitterRequest(this);
                connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onVersionRecieved(QString, bool)));
                request->requestVersion();
            }

            start();
        }
    }
    else
    {
        printf("qml has errors\n"); fflush(stdout);
    }
}

bool Frontend::isOSThree()
{
    if (s_isOsThree < 0)
    {
#ifdef BB103
        s_isOsThree = 1;
#else
        s_isOsThree = 0;
        PlatformInfo info;
        QString os = info.osVersion();
        QStringList oss = os.split('.', QString::SkipEmptyParts);
        if (oss.length() > 1)
        {
            bool ok;
            int min = oss[1].toInt(&ok);
            if (min > 2)
                s_isOsThree = 1;
        }
#endif
    }
    return s_isOsThree == 1;
}

Frontend::~Frontend()
{
    if (debug_mode) { printf("Saving Controllers!\n");fflush(stdout); }
	m64p->save_controller_config(0);
	m64p->save_controller_config(1);
	m64p->save_controller_config(2);
	m64p->save_controller_config(3);
	if (debug_mode) { printf("Finished Saving Controllers!\n");fflush(stdout); }
	delete m_settings;
	delete m_devices;
	delete m_history;
	delete m_menuAnimation;
	delete m_animationLock;
	delete m_gameSettings;
	if (m_hdmiInfo)
		delete m_hdmiInfo;
}

void Frontend::refreshColours()
{
#ifdef BB103
    Color primary = Color::fromRGBA(COLORIZE(m_gameSettings->Settings()->PrimaryColourRed()),
            COLORIZE(m_gameSettings->Settings()->PrimaryColourGreen()), COLORIZE(m_gameSettings->Settings()->PrimaryColourBlue()));
    Color base = Color::fromRGBA(COLORIZE(m_gameSettings->Settings()->BaseColourRed()),
            COLORIZE(m_gameSettings->Settings()->BaseColourGreen()), COLORIZE(m_gameSettings->Settings()->BaseColourBlue()));
    if (m_gameSettings->Settings()->PrimaryColourIndex() == 0)
        Application::instance()->themeSupport()->setPrimaryColor(bb::cascades::Color());
    else if (m_gameSettings->Settings()->BaseColourIndex() == 0)
        Application::instance()->themeSupport()->setPrimaryColor(primary);
    else
        Application::instance()->themeSupport()->setPrimaryColor(primary, base);
#endif
}

void Frontend::refreshTheme()
{
#ifdef BB103
    if (themeIndex())
        Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Dark);
    else
        Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
#endif
}

bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);
    printf("Attempting to remove directory: %s\n", dirName.toAscii().constData());

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
            {
                result = removeDir(info.absoluteFilePath());
            }
            else
            {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result)
            {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    else
    {
        printf("Directory doesn't exist\n");fflush(stdout);
    }
    return result;
}

static Led led;
void Frontend::saveScreenShot()
{
    QDir dir;
    removeDir(dir.absolutePath() + "/data/gif");
    mkdir("data/gif", 0777);
    printf("Saving %d images\n", s_screenQueue.size());fflush(stdout);
    while (s_screenQueue.size() > 0)
    {
        QPair<int, unsigned char*> working = s_screenQueue.takeFirst();
        QString name = dir.absolutePath() + QString::fromLatin1("/data/gif/screen");
        name += QString::number(working.first);
        name += QString::fromLatin1(".png");
        QImage img(working.second, emuWidth(), emuHeight(), QImage::Format_ARGB32);
        img = img.rgbSwapped();
        img = img.mirrored(false, true);
        if (!img.save(name, "PNG"))
            printf("Failed to save image %s\n", name.toAscii().constData());
        else
            printf("Saved image %s, %d left\n", name.toAscii().constData(), s_screenQueue.size());
        fflush(stdout);
        delete working.second;
    }
    led.setColor(LedColor::Magenta);
    led.flash(1);
}

void captureScreen(int id, unsigned char* buffer, int size)
{
    unsigned char* buffer2 = new unsigned char[size];
    memcpy(buffer2, buffer, size);
    Frontend::s_screenQueue.append(QPair<int, unsigned char*>(id, buffer2));
    if (id == 0)
    {
        QTimer::singleShot(0, mainApp, SLOT(saveScreenShot()));
    }
}

void Frontend::onCapturePressed()
{
    //capture_callback = captureScreen;
    //begin_capture();
}

bool Frontend::debugMode()
{
	return debug_mode;
}

void Frontend::onManualExit()
{
	QDir dir("data/screens/");
	dir.setNameFilters(QStringList() << "*.*");
	dir.setFilter(QDir::Files);
	foreach (QString dirFile, dir.entryList())
	{
		dir.remove(dirFile);
	}
	if (debug_mode) { printf("OnManualExit!\n");fflush(stdout); }
	int msg = 2;//, ret = 0;
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);
	wait();
	if (debug_mode) { printf("Wait!\n");fflush(stdout); }
	quit();
	if (debug_mode) { printf("Quit!\n");fflush(stdout); }
}

typedef union
{
	_pulse pulse;
	int msg;
} recv_msg;

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
	discoverBluetoothDevices();
	detectHDMI();
	setHistory(getHistory());

	while(1)
	{
		while(1)
		{
			rcvid = MsgReceive(chid, &msg, sizeof(msg), 0);

			if(rcvid <= 0)
				continue;

			if(msg.msg == 1)
			{
				MsgReply(rcvid, 0, NULL, 0);
				break;
			}
			else if (msg.msg == 2)
			{
				MsgReply(rcvid, 0, NULL, 0);
				return;
			}

			z = 5;
			if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z) != 0)
				return;

			screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

			while(1)
			{
				if (BPS_SUCCESS != bps_get_event(&event, -1))
				{
					if (debug_mode)
					    fprintf(stderr, "bps_get_event failed\n");
					break;
				}

				if (event)
				{
					int domain = bps_event_get_domain(event);

					if (domain == screen_get_domain())
					{
						screen_event_t screen_event = screen_event_get_event(event);
						int screen_val;
						screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

						if(screen_val == SCREEN_EVENT_MTOUCH_TOUCH)
						{
							//This is touch screen event
							sym = -1;
							break;
						}
						else if(screen_val == SCREEN_EVENT_KEYBOARD)
						{
							screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_SYM, &sym);
							break;
						}
						else if (screen_val == SCREEN_EVENT_GAMEPAD || screen_val == SCREEN_EVENT_JOYSTICK)
						{
							int buttons;
							int analog0[3];
							int analog1[3];
                            sym = NO_BUTTON;
                            int err = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &buttons);
                            if (err != BPS_SUCCESS && debug_mode)
                                fprintf(stderr, "Error at SCREEN_PROPERTY_BUTTONS: %d\n", err);
                            if (buttons == 0)
                            {
                                screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG0, analog0);
                                if (analog0[2] == 255)
                                {
                                    sym = 1 << EXT_BUTTON_L4;
                                }
                                else
                                {
                                    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG1, analog1);
                                    if (analog1[2] == 255)
                                    {
                                        sym = 1 << EXT_BUTTON_R4;
                                    }
                                }
                            }
                            else
                            {
                                for (int j = A_BUTTON; j < NO_BUTTON; j++)
                                {
                                    if (buttons & (1 << j))
                                    {
                                        if (debug_mode)
                                            fprintf(stderr, "Gamepad Button: %d\n", j);
                                        sym = 1 << j;
                                        break;
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
			if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z) != 0)
				return;
			screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

			MsgReply(rcvid, 0, &sym, sizeof(sym));
		}

		//Cheats
		if (debug_mode) printf("CheatList: %s\n", CheatList.toAscii().constData());
		m64p->l_CheatNumList = strdup((char*)CheatList.toAscii().constData());

        dbg_fps = false;
        if (m_gameSettings->GlideSettings()->VideoPlugin() == 2 &&
                m_gameSettings->GlideSettings()->DisplayStats() == 0 && m_gameSettings->Settings()->ShowFPS())
            m64p->SetConfigParameter(std::string("Video-Glide64mk2[show_fps]=1"));
        else if (m_gameSettings->GlideSettings()->VideoPlugin() != 2 && m_gameSettings->Settings()->ShowFPS())
            dbg_fps = true;

		bool stretch = m_gameSettings->RiceSettings()->StretchVideo();
        int w = emuWidth();
        int h = emuHeight();
        use_overlay = false;
        use_hdmi = false;
        q10_rotate = 0;
        use_gamepad = true;
        if (m_hdmiInfo && m_hdmiInfo->isAttached())
        {
            if (m_useHdmi)
            {
                if (hdmi1080())
                {
                    w = m_hdmiInfo->pixelSize().width();
                    h = m_hdmiInfo->pixelSize().height();
                }
                else
                {
                    switch (hdmiResolution())
                    {
                    case 1:
                        w = 320;
                        h = 240;
                        break;
                    case 2:
                        w = 640;
                        h = 480;
                        break;
                    case 3:
                        w = 720;
                        h = 720;
                        break;
                    case 4:
                        w = 960;
                        h = 720;
                        break;
                    case 5:
                        w = 1280;
                        h = 720;
                        break;
                    case 6:
                        w = 1280;
                        h = 768;
                        break;
                    default:
                        w = 256;
                        h = 224;
                        break;
                    }
                }
                hdmi_width = w;
                hdmi_height = h;
                use_hdmi = true;
                stretch = true;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            int input = m_gameSettings->PlayerKeyboardSettings(i + 1)->Input();
            if (input != 0)
            {
                if (input == -3)
                {
                    if (i == 0)
                    {
                        if (m_gameSettings->Player1TouchscreenSettings()->ControllerLayout() != 5)
                            m_noTouchScreenControllers = false;
                    }
                    else
                        m_noTouchScreenControllers = false;
                    if (emuWidth() != emuHeight())
                    {
                        if (m_useHdmi && m_hdmiInfo && m_hdmiInfo->isAttached())
                            use_overlay = true;
                        else
                            stretch = true;
                    }
                }
                else if (input == -5)
                {
                    if (emuWidth() == emuHeight())
                    {
                        if (m_useHdmi && m_hdmiInfo && m_hdmiInfo->isAttached())
                            use_overlay = true;
                        else
                            stretch = true;
                        q10_rotate = -((m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->RotationDirection() << 1) - 1);
                    }
                    use_gamepad = false;
                }
                else if (input == -2)
                {
                    if (hasKeyboard())
                        use_gamepad = false;
                }
            }
        }
        emit UseForeignWindowControlChanged();
        emit touchScreenControllerCountChanged();
		//screen resolution
		std::ostringstream s, s3, s5;
		s << "Video-General[ScreenWidth]=";
        s3 << "gles2n64[screen width]=";
        s5 << "gles2n64[framebuffer width]=";
		if (m_gameSettings->RiceSettings()->VideoPlugin() == 0 && !stretch)
        {
            if (w == h)
            {
                s << w;
                s3 << w;
                s5 << w;
                margin_left = 0;
            }
            else
            {
                int wdth = ((double)h) * 1.3333333333333;
                s << (int)wdth;
                s3 << (int)wdth;
                s5 << (int)wdth;
                margin_left = (int)((((double)w) - wdth) / 2.0);
                if (debug_mode) printf("margin_left = %d\n", margin_left);
            }
        }
		else
        {
            margin_left = 0;
            s << w;
            s3 << w;
            s5 << w;
        }
		m64p->SetConfigParameter(s.str());
        m64p->SetConfigParameter(s3.str());
        m64p->SetConfigParameter(s5.str());
		std::ostringstream s2, s4, s6;
		s2 << "Video-General[ScreenHeight]=";
        s4 << "gles2n64[screen height]=";
        s6 << "gles2n64[framebuffer height]=";
		if (m_gameSettings->RiceSettings()->VideoPlugin() == 0 && !stretch)
		{
            if (w == h)
            {
                double hght = ((double)w) * 0.75;
                s2 << (int)hght;
                s4 << (int)hght;
                s6 << (int)hght;
                margin_bottom = (int)((((double)h) - hght) / 2.0);
            }
            else
            {
                s2 << h;
                s4 << h;
                s6 << h;
                margin_bottom = 0;
            }
		}
		else
		{
            margin_bottom = 0;
		    s2 << h;
            s4 << h;
            s6 << h;
		}
		m64p->SetConfigParameter(s2.str());
        m64p->SetConfigParameter(s4.str());
        m64p->SetConfigParameter(s6.str());

        m_gameSettings->GameAudioSettings()->writeSettings(m64p);
        m_gameSettings->RiceSettings()->writeSettings(m64p);
        m_gameSettings->N64Settings()->writeSettings(m64p);
        m_gameSettings->GlideSettings()->writeSettings(m64p);
        m_gameSettings->Hardware()->writeSettings(m64p);
        for (int i = 0; i < 4; i++)
        {
            m_gameSettings->PlayerKeyboardSettings(i + 1)->writeSettings(m64p);
            m_gameSettings->PlayerTouchscreenSettings(i + 1)->writeSettings(m64p);
            m_gameSettings->PlayerTouchKeyboardSettings(i + 1)->writeSettings(m64p);
            m_gameSettings->PlayerGamepadSettings(i + 1)->writeSettings(m64p);
            m64p->save_controller_config(i);
        }
        m64p->SetConfigParameter("CoreEvents[Kbd Mapping Gameshark]=103");
        m64p->SetConfigParameter("CoreEvents[Kbd Mapping Fast Forward]=102");

        int retval = m_riceini.WriteIniFile();
        if (retval == 13)
        {
            SystemToast* toast = new SystemToast;
            toast->setBody(tr("Unable to save advanced settings: permissions"));
            toast->show();
        }
        fflush(stdout);

        if (m_gameSettings->VideoPlugin() == M64PSettings::GLIDE)
        {
            setupGlide();
        }

        emit rotationChanged();

		m_emuRunning = true;

		//m64p->print_controller_config();
		m64p->Start();
		//m64p->print_controller_config();
	}
}

void Frontend::setupGlide()
{
    int screenwidth = emuWidth();
    int screenheight = emuHeight();
    int ratio = m_gameSettings->GlideSettings()->AspectRatio();
    egl_letterbox = false;
    egl_pillarbox = false;
    if (ratio != GlideVideoSettings::Stretch)
    {
        double widthreference;
        double heightreference;
        if (ratio == GlideVideoSettings::Original || ratio == GlideVideoSettings::GameDefault || ratio == GlideVideoSettings::FourToThree)
        {
            widthreference = 4.0;
            heightreference = 3.0;
        }
        else
        {
            widthreference = 16.0;
            heightreference = 9.0;
            //don't pillar box or letter box a 16:9 ratio on a 16:9 device
            if (screenwidth == 720 && screenheight == 1280)
                return;
        }
        //keyboard devices with 1:1 screens always letterbox
        if (screenwidth == screenheight)
        {
            egl_letterbox = true;
            egl_width = screenwidth;
            int windowheight = (int)(((double)screenwidth) * heightreference / widthreference);
            egl_height = (int)(((double)(screenwidth - windowheight)) * 0.5);
            egl_bottom = egl_height + windowheight;
        }
        else
        {
            //4:3 on either 16:9 or 15:9 devices need pillar boxing
            if (widthreference == 4.0)
            {
                egl_pillarbox = true;
                egl_height = screenheight;
                int windowwidth = (int)(((double)screenheight) * widthreference / heightreference);
                egl_width = (int)(((double)(screenwidth - windowwidth)) * 0.5);
                egl_left = egl_width + windowwidth;
            }
            //16:9 on a 15:9 device needs letter boxing
            else
            {
                egl_letterbox = true;
                egl_width = screenwidth;
                int windowheight = (int)(((double)screenwidth) * heightreference / widthreference);
                egl_height = (int)(((double)(screenheight - windowheight)) * 0.5);
                egl_bottom = egl_height + windowheight;
            }
        }
    }
}

void Frontend::onThumbnail()
{
	if (m_emuRunning)
	{
		int width = hdmi_width;
		int height = hdmi_height;
		if (width < 0)
		    width = this->width();
		if (height < 0)
		    height = this->height();
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
    if (menuOffset() < 10 && use_gamepad)
        focus();
	m_animationLock->unlock();
}

void Frontend::onMenuOffsetChanged()
{
    if (!use_gamepad)
        bbutil_offset_menu(m_menuOffset);
}

void Frontend::swipedown()
{
    if (!m_animationLock->tryLock())
        return;
    if (menuOffset() > 0)
    {
        m64p_emit_touch = false;
        m_menuAnimation->setStartValue(175);
        m_menuAnimation->setEndValue(0);
        m_menuAnimation->start();
    }
    else
    {
        m64p_emit_touch = true;
        m_menuAnimation->setStartValue(0);
        m_menuAnimation->setEndValue(175);
        m_menuAnimation->start();
    }
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
		QString clearRes = gm.resource();
		int index = clearRes.lastIndexOf('.');
		clearRes = clearRes.left(index);
		clearRes += "_clear.png";
		map["clear"] = clearRes;
		if (QFile::exists(clearRes.mid(7)))
		    map["hasclear"] = true;
		else
		    map["hasclear"] = false;
		map["location"] = gm.location();
		map["uuid"] = gm.uuid();
		map["pinned"] = gm.isPinned();
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
	bool boxart = m_gameSettings->Settings()->BoxartScraping();
	int i;
	for (i = 0; i < history.size(); i++)
	{
		if (QString::compare(history[i].baseName(), title, Qt::CaseInsensitive) == 0)
		{
			gm = history.at(i);
			break;
		}
	}
	if (gm.isPinned())
	{
	    gm.playNow();
	    setHistory(history);
	    return;
	}
	else if (i != history.size())
	{
	    gm = history.takeAt(i);
	}
    QString selectedFile = mRom.mid(mRom.lastIndexOf('/') + 1);
    int tmp2 = selectedFile.indexOf("(");
    QString imageSource;
    if(tmp2 == -1)
    {
        imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + selectedFile.mid(0, selectedFile.lastIndexOf(".")).trimmed() + ".jpg";
    }
    else
    {
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
	int j;
	for (j = 0; j < history.size(); j++)
	{
	    if (!history[j].isPinned())
	        break;
	}
	history.insert(j, gm);
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

void Frontend::changeHistoryName(QString uuid)
{
    QUuid id(uuid);
    QList<Game> hist = getHistory();
    for (int i = 0; i < hist.size(); i++)
    {
        if (hist[i] == id)
        {
            SystemPrompt *prompt = new SystemPrompt();
            prompt->setTitle(tr("Enter a new name"));
            prompt->setDismissAutomatically(true);
            prompt->inputField()->setDefaultText(hist[i].name());
            prompt->inputField()->setEmptyText(tr("Enter a new name"));
            prompt->setProperty("EDITUUID", uuid);
            connect(prompt, SIGNAL(finished(bb::system::SystemUiResult::Type)), SLOT(historyRenameSelected(bb::system::SystemUiResult::Type)));
            prompt->show();
            break;
        }
    }
}

void Frontend::pinHistory(QString uuid)
{
    QUuid id(uuid);
    QList<Game> hist = getHistory();
    for (int i = 0; i < hist.size(); i++)
    {
        if (hist[i] == id)
        {
            Game g = hist.takeAt(i);
            g.setPinned(true);
            int j;
            for (j = 0; j < hist.size(); j++)
            {
                if (!hist[j].isPinned())
                    break;
            }
            hist.insert(j, g);
            setHistory(hist);
            break;
        }
    }
}

void Frontend::unpinHistory(QString uuid)
{
    QUuid id(uuid);
    QList<Game> hist = getHistory();
    for (int i = 0; i < hist.size(); i++)
    {
        if (hist[i] == id)
        {
            Game g = hist.takeAt(i);
            g.setPinned(false);
            int j;
            for (j = 0; j < hist.size(); j++)
            {
                if (!hist[j].isPinned())
                    break;
            }
            hist.insert(j, g);
            setHistory(hist);
            break;
        }
    }
}

void Frontend::historyRenameSelected(bb::system::SystemUiResult::Type result)
{
    if (result == SystemUiResult::ConfirmButtonSelection)
    {
        SystemPrompt *prompt = qobject_cast<SystemPrompt*>(sender());
        QString uuid = prompt->property("EDITUUID").toString();
        QUuid id(uuid);
        QList<Game> hist = getHistory();
        for (int i = 0; i < hist.size(); i++)
        {
            if (hist[i] == id)
            {
                hist[i].rename(prompt->inputFieldTextEntry());
                setHistory(hist);
                break;
            }
        }
        prompt->deleteLater();
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

void Frontend::startEmulatorInternal()
{
    s_handler->startEmulating();
	int msg = 1;
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);
	m_currentROM = QString(m64p->l_RomName);
	emit currentROMChanged();
	addToHistory(m_currentROM);
}

void Frontend::startEmulator(bool start_now)
{
	Q_UNUSED(start_now);
	QTimer::singleShot(0, this, SLOT(startEmulatorInternal()));
}

//Getters and Setters
QString Frontend::getRom()
{
    return mRom;
}

//TODO working
void Frontend::onPlayReleased()
{
    /*screen_event_t sevent;
    int err = screen_create_event(&sevent);
    printf("screen_create_event: %d\n", err);fflush(stdout);
    int param[] = { SCREEN_EVENT_GAMEPAD };
    screen_set_event_property_iv(sevent, SCREEN_PROPERTY_TYPE, param);
    int buttons[] = { 0x1 };
    screen_set_event_property_iv(sevent, SCREEN_PROPERTY_BUTTONS, buttons);
    int analog0[] = { 0, 100, 0 };
    screen_set_event_property_iv(sevent, SCREEN_PROPERTY_ANALOG0, analog0);
    screen_set_event_property_iv(sevent, SCREEN_PROPERTY_ANALOG1, analog0);
    err = screen_send_event(screen_cxt, sevent, getpid());
    printf("screen_send_event: %d\n", err);fflush(stdout);*/
}

void Frontend::setRom(QString i)
{
    mRom = i;
    m64p->SetRom(mRom.toAscii().constData());
    m_gameSettings->setGameName(mRom);
    emit romChanged(mRom);
}

void ROM_GetRomNameFromHeader(char * szName, unsigned char* szStartName)
{
    char * p;

    memcpy(szName, szStartName, 20);
    szName[20] = '\0';

    p = szName + (strlen((char*)szName) -1);        // -1 to skip null
    while (p >= szName && *p == ' ')
    {
        *p = 0;
        p--;
    }
}

void Frontend::LoadRom()
{
	m64p->LoadRom();
	emit gameHasId(false);
    TwitterRequest* request = new TwitterRequest(this);
    connect(request, SIGNAL(idDiscovered(const QString&)), this, SLOT(onIdDiscovered(const QString&)));
    QString romname = mRom.right(mRom.length() - mRom.lastIndexOf('/') - 1);
    printf("Requested Game: %s\n", romname.toAscii().constData());
    fflush(stdout);
    request->requestId(romname);
	emit ROMLoaded();
	QString crc = info_crc();
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) == M64ERR_SUCCESS)
    {
        char RomSection[51];
        sprintf(RomSection, "%08x%08x-%02x", RomHeader.CRC1, RomHeader.CRC2, RomHeader.Country_code);
        printf("Loading Rice settings for CRC: %s\n", RomSection);
        char name[256];
        ROM_GetRomNameFromHeader(name, RomHeader.Name);
        printf("ROM Name: %s\n", name);fflush(stdout);
        m_riceini.setCRC(QString::fromUtf8(RomSection), QString::fromUtf8(name));
    }
}

void Frontend::onIdDiscovered(const QString& info)
{
    //printf("ID Discovered: %s\n", info.toAscii().constData());fflush(stdout);
    m_gameInfo.parse(info);
    if (m_gameInfo.Id() <= 0)
        emit gameHasId(false);
    else
    {
        emit gameHasId(true);
        QString filename = mRom.right(mRom.length() - mRom.lastIndexOf('/') - 1);
        int tmp = filename.indexOf("(");
        if (tmp >= 0)
            filename = filename.left(tmp) + "_clear.png";
        else
            filename = filename.left(filename.lastIndexOf('.')) + "_clear.png";
        if (!QFile::exists("/accounts/1000/shared/misc/n64/.boxart/" + filename))
            m_gameInfo.ClearIcon()->SaveImage("/accounts/1000/shared/misc/n64/.boxart/" + filename);
    }
}

bool Frontend::fileExists(const QString& filename)
{
    QString name = QString(filename);
    if (name.startsWith("file://"))
        name = name.right(name.length() - 7);
    QFileInfo info(name);
    return info.exists();
}

void Frontend::pressGameshark()
{
    m64p->SendKeyDown(103, 0);
    QTimer::singleShot(250, this, SLOT(releaseGameshark()));
}

void Frontend::releaseGameshark()
{
    m64p->SendKeyUp(103, 0);
}

void Frontend::pressFastforward()
{
    m64p->SendKeyDown(102, 0);
}

void Frontend::releaseFastforward()
{
    m64p->SendKeyUp(102, 0);
}

int Frontend::SaveStateSlot()
{
    return m64p->getSaveStateSlot();
}

void Frontend::SaveStateSlot(int slot)
{
    m64p->setSaveStateSlot(slot);
}

void Frontend::selectState(bool save)
{
    m_selectStateSaving = save;
    SystemListDialog *listDialog = new SystemListDialog(tr("Set"), tr("Cancel"));
    listDialog->setTitle(tr("Select Save State Slot"));
    listDialog->appendItem(tr("Save to File"));
    int sel = SaveStateSlot();
    if (sel == 0)
        listDialog->appendItem("0", true, true);
    else
        listDialog->appendItem("0");
    if (sel == 1)
        listDialog->appendItem("1", true, true);
    else
        listDialog->appendItem("1");
    if (sel == 2)
        listDialog->appendItem("2", true, true);
    else
        listDialog->appendItem("2");
    if (sel == 3)
        listDialog->appendItem("3", true, true);
    else
        listDialog->appendItem("3");
    if (sel == 4)
        listDialog->appendItem("4", true, true);
    else
        listDialog->appendItem("4");
    if (sel == 5)
        listDialog->appendItem("5", true, true);
    else
        listDialog->appendItem("5");
    if (sel == 6)
        listDialog->appendItem("6", true, true);
    else
        listDialog->appendItem("6");
    if (sel == 7)
        listDialog->appendItem("7", true, true);
    else
        listDialog->appendItem("7");
    if (sel == 8)
        listDialog->appendItem("8", true, true);
    else
        listDialog->appendItem("8");
    if (sel == 9)
        listDialog->appendItem("9", true, true);
    else
        listDialog->appendItem("9");

    connect(listDialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), SLOT(saveStateSlotSelected(bb::system::SystemUiResult::Type)));

    listDialog->show();
}

void Frontend::saveStateSlotSelected(bb::system::SystemUiResult::Type result)
{
    Q_UNUSED(result);
    SystemListDialog *listDialog = qobject_cast<SystemListDialog*>(sender());
    if (listDialog->buttonSelection() == listDialog->confirmButton())
    {
        int index = listDialog->selectedIndices()[0];
        if (index == 0)
        {
            FilePicker* filePicker = new FilePicker();
            filePicker->setType(FileType::Other);
            if (m_selectStateSaving)
            {
                filePicker->setTitle(tr("Save State"));
                filePicker->setMode(FilePickerMode::Saver);
            }
            else
            {
                filePicker->setTitle(tr("Load State"));
                filePicker->setMode(FilePickerMode::Picker);
            }
            QStringList filter;
            filter << "*.st";
            filePicker->setFilter(filter);

            filePicker->open();

            connect(filePicker, SIGNAL(fileSelected(const QStringList&)), SLOT(saveStateFileSelected(const QStringList&)));
        }
        else
        {
            m64p->setSaveStateSlot(index - 1);
            //m64p->setSaveStateSlot(index);
            if (m_selectStateSaving)
                m64p->SaveState();
            else
                m64p->LoadState();
            emit SaveStateSlotChanged();
        }
    }
    listDialog->deleteLater();
}

void Frontend::saveStateFileSelected(const QStringList &files)
{
    if (files.length() < 1)
        return;
    QString file = files[0];
    if (!file.endsWith(".st"))
        file += ".st";
    if (m_selectStateSaving)
        m64p->SaveState(file.toAscii().constData());
    else
        m64p->LoadState(file.toAscii().constData());
}

QSignalMapper *signalMapperToggle;
QSignalMapper *signalMapperDropDown;

void Frontend::addCheatToggle(int number)
{
	ToggleButton *senderButton = (ToggleButton*)signalMapperToggle->mapping(number);

	if(senderButton->isChecked())
		CheatList.append(QString("%1,").arg(number));
	else
		CheatList.remove(QString("%1,").arg(number));
}

void Frontend::addCheatDropDown(int number)
{
	DropDown *senderDropDown = (DropDown*)signalMapperDropDown->mapping(number);

	int foundLoc = -1;
	int removeUntil = 0;

	//to remove, index=cheat number. We need to see if it's already there, and remove it.
	//1,2,3-4, or 3-4,1,2

	foundLoc = CheatList.indexOf(QString("%1-").arg(number));
	if(foundLoc != -1)
	{
		removeUntil = CheatList.indexOf(",", foundLoc);
		CheatList.remove(foundLoc, removeUntil-foundLoc+1);
	}

	if(senderDropDown->selectedIndex() > 0 )
	{
		CheatList.append(QString("%1-%2,").arg(number).arg(senderDropDown->selectedIndex()-1));
	}

	fflush(stdout);
}

Container * Frontend::createCheatToggle(sCheatInfo *pCur)
{
	ToggleButton *toggle;

	Container *returnContainer = Container::create().top(20.0f);
	TextStyle *italicStyle = new TextStyle(SystemDefaults::TextStyles::bodyText());
	italicStyle->setFontStyle(FontStyle::Italic);

	//Container with Label and Toggle
	Container *CheatToggle = Container::create().left(15.0f).right(15.0f).horizontal(HorizontalAlignment::Fill);

	DockLayout *pDockLayout = new DockLayout();
	CheatToggle->setLayout(pDockLayout);

	Container *nameContainer = Container::create().right(200);
	nameContainer->add(Label::create().text(QString(pCur->Name))
									.vertical(VerticalAlignment::Center)
									.multiline(true)
									);
	CheatToggle->add(nameContainer);
	toggle = ToggleButton::create().vertical(VerticalAlignment::Center)
	                               .horizontal(HorizontalAlignment::Right);

	signalMapperToggle->setMapping(toggle, pCur->Number);
	connect(toggle, SIGNAL(checkedChanged(bool)), signalMapperToggle, SLOT(map()));

	CheatToggle->add(toggle);

	returnContainer->add(CheatToggle);

	//Container with description label
	if(pCur->Description != NULL)
	{
		Container *CheatDescription = Container::create().bottom(20.0f).left(15.0f).right(15.0f);
		CheatDescription->add(Label::create().text(QString(pCur->Description))
				.vertical(VerticalAlignment::Center)
				.multiline(true)
				.textStyle(*italicStyle)
				.visible(false)
				);
		returnContainer->add(CheatDescription);
	}

	returnContainer->add(Divider::create());

	return returnContainer;
}

Container * Frontend::createCheatDropDown(sCheatInfo *pCur)
{
	DropDown *dropDown;
	Option * tmp;

	Container *returnContainer = Container::create().top(20.0f);
	TextStyle *italicStyle = new TextStyle(SystemDefaults::TextStyles::bodyText());
	italicStyle->setFontStyle(FontStyle::Italic);

	dropDown = DropDown::create().title(QString(pCur->Name));

	tmp = Option::create().text(tr("Disable")).value(0);
	dropDown->add(tmp);

	for (int i = 0; i < pCur->Codes[pCur->VariableLine].var_count; i++)
	{
		tmp = Option::create().text(QString("%1").arg(pCur->Codes[pCur->VariableLine].variable_names[i]))
				              .value(i+1);
		dropDown->add(tmp);
	}

	signalMapperDropDown->setMapping(dropDown, pCur->Number);
	connect(dropDown, SIGNAL(selectedIndexChanged(int)), signalMapperDropDown, SLOT(map()));

	Container *dropDownCont = Container::create().left(15.0f).right(15.0f)
        .vertical(VerticalAlignment::Center);
	dropDownCont->add(dropDown);

	returnContainer->add(dropDownCont);

	//Container with description label
	if(pCur->Description != NULL){
		Container *CheatDescription = Container::create().bottom(20.0f).left(15.0f).right(15.0f);
		CheatDescription->add(Label::create().text(QString(pCur->Description))
				.vertical(VerticalAlignment::Center)
				.multiline(true)
				.textStyle(*italicStyle)
				);
		returnContainer->add(CheatDescription);
	}

	returnContainer->add(Divider::create());

	return returnContainer;
}

void Frontend::createCheatsPage()
{
	char numCheats = 0;
	CheatStart(CHEAT_SHOW_LIST, &numCheats);
	fflush(stdout);

	signalMapperToggle = new QSignalMapper(this);
	connect(signalMapperToggle, SIGNAL(mapped(int)), this, SLOT(addCheatToggle(int)));

	signalMapperDropDown = new QSignalMapper(this);
	connect(signalMapperDropDown, SIGNAL(mapped(int)), this, SLOT(addCheatDropDown(int)));

	mCheatsContainer->removeAll();
	sCheatInfo *pCur = l_CheatList;
	while (pCur != NULL)
	{
		if (pCur->VariableLine == -1)
		{
			mCheatsContainer->add(createCheatToggle(pCur));
		}
		else
		{
			mCheatsContainer->add(createCheatDropDown(pCur));
		}
		pCur = pCur->Next;
	}

	CheatFreeAll();
}

QString Frontend::info_goodname()
{
    m64p_rom_settings RomSettings;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(RomSettings), &RomSettings) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM settings information from core library\n");
        return "";
    }
    return QString::fromAscii(RomSettings.goodname);
}

QString Frontend::info_name()
{
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM header information from core library\n");
        return "";
    }
    return QString::fromAscii((char*)RomHeader.Name);
}

QString Frontend::info_md5()
{
    m64p_rom_settings RomSettings;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(RomSettings), &RomSettings) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM settings information from core library\n");
        return "";
    }
    return RomSettings.MD5;
}

QString Frontend::info_crc()
{
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM header information from core library\n");
        return "";
    }
    char RomSection[24];
    sprintf(RomSection, "%08X %08X", sl(RomHeader.CRC1), sl(RomHeader.CRC2));
    return QString::fromAscii(RomSection);
}

QString Frontend::info_imagetype()
{
    QFileInfo inf(mRom);
    QString ext = inf.suffix();
    if (QString::compare(ext, "z64") == 0)
        return ".z64 (native)";
    if (QString::compare(ext, "v64") == 0)
        return ".v64 (byteswapped)";
    if (QString::compare(ext, "n64") == 0)
        return ".n64 (wordswapped)";
    if (QString::compare(ext, "zip") == 0)
    {
        unzFile uf = unzOpen(mRom.toAscii().constData());
        if (!uf)
        {
            printf("Error: couldn't open ROM file '%s' for reading.\n", mRom.toAscii().constData());
            return "";
        }
        unz_global_info gi;
        unzGetGlobalInfo(uf, &gi);
        for (uLong i = 0; i < gi.number_entry; i++)
        {
            char filename_inzip[256];
            unz_file_info file_info;
            unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
            printf("Looking at file %s\n", filename_inzip);
            QString filename(filename_inzip);
            inf = QFileInfo(filename);
            ext = inf.suffix();
            if (QString::compare(ext, "z64") == 0)
            {
                unzClose(uf);
                return ".z64 (native)";
            }
            if (QString::compare(ext, "v64") == 0)
            {
                unzClose(uf);
                return ".v64 (byteswapped)";
            }
            if (QString::compare(ext, "n64") == 0)
            {
                unzClose(uf);
                return ".n64 (wordswapped)";
            }
            if ((i + 1) < gi.number_entry)
                unzGoToNextFile(uf);
        }
        unzClose(uf);
    }
    return "";
}

QString Frontend::info_romsize()
{
    QFileInfo inf(mRom);
    if (inf.exists())
    {
        if (QString::compare(inf.suffix(), "zip", Qt::CaseInsensitive) != 0)
        {
            qint64 size = inf.size();
            return QString::number(size) + " bytes (or " + QString::number(size / 1024 / 1024) + " Mb or " + QString::number(size / 1024 / 1024 * 8) + " Megabits";
        }
        else
        {
            unzFile uf = unzOpen(mRom.toAscii().constData());
            if (!uf)
            {
                printf("Error: couldn't open ROM file '%s' for reading.\n", mRom.toAscii().constData());
                return "";
            }
            unz_global_info gi;
            unzGetGlobalInfo(uf, &gi);
            uLong size = 0;
            for (uLong i = 0; i < gi.number_entry; i++)
            {
                char filename_inzip[256];
                unz_file_info file_info;
                unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
                size += file_info.uncompressed_size;
                if ((i + 1) < gi.number_entry)
                    unzGoToNextFile(uf);
            }
            unzClose(uf);
            return QString::number(size) + " bytes (or " + QString::number(size / 1024 / 1024) + " Mb or " + QString::number(size / 1024 / 1024 * 8) + " Megabits)";
        }
    }
    return "";
}

QString Frontend::info_version()
{
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM header information from core library\n");
        return "";
    }
    return QString::number(sl(RomHeader.Release), 16);
}

QString Frontend::info_manufacturer()
{
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM header information from core library\n");
        return "";
    }
    if (sl(RomHeader.Manufacturer_ID) == 'N')
        return tr("Nintendo");
    return "0x" + QString::number(sl(RomHeader.Manufacturer_ID), 16);
}

QString countrycodestring(unsigned short countrycode)
{
    switch (countrycode)
    {
    case 0:    /* Demo */
        return "Demo";
        break;

    case '7':  /* Beta */
        return "Beta";
        break;

    case 0x41: /* Japan / USA */
        return "USA/Japan";
        break;

    case 0x44: /* Germany */
        return "Germany";
        break;

    case 0x45: /* USA */
        return "USA";
        break;

    case 0x46: /* France */
        return "France";
        break;

    case 'I':  /* Italy */
        return "Italy";
        break;

    case 0x4A: /* Japan */
        return "Japan";
        break;

    case 'S':  /* Spain */
        return "Spain";
        break;

    case 0x55: case 0x59:  /* Australia */
        return "Australia (0x" + QString::number(countrycode, 16) + ")";
        break;

    case 0x50: case 0x58: case 0x20:
    case 0x21: case 0x38: case 0x70:
        return "Europe (0x" + QString::number(countrycode, 16) + ")";
        break;

    default:
        return "Unknown (0x" + QString::number(countrycode, 16) + ")";
        break;
    }
}

QString Frontend::info_country()
{
    m64p_rom_header RomHeader;
    if ((*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(RomHeader), &RomHeader) != M64ERR_SUCCESS)
    {
        printf("UI-Console: couldn't get ROM header information from core library\n");
        return "";
    }
    return countrycodestring(RomHeader.Country_code);
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
		return tr("Not Mapped");
	}
}

QString Frontend::getInputCharacter(int value)
{
	if (value < 0)
		return tr("Not Mapped");
	if (value >= KEYCODE_A && value <= KEYCODE_Z)
		return QString(QChar((char)(value - KEYCODE_A + Qt::Key_A)));
	if (value >= KEYCODE_CAPITAL_A && value <= KEYCODE_CAPITAL_Z)
		return QString(QChar((char)(value - KEYCODE_CAPITAL_A + Qt::Key_A))).toUpper();
	switch (value) {
	case KEYCODE_ESCAPE:
		return tr("Escape");
	case KEYCODE_RETURN:
		return tr("Enter");
	case KEYCODE_LEFT:
		return tr("Left");
	case KEYCODE_RIGHT:
		return tr("Right");
	case KEYCODE_UP:
		return tr("Up");
	case KEYCODE_DOWN:
		return tr("Down");
	case KEYCODE_BACKSPACE:
		return tr("Backspace");
	case KEYCODE_LEFT_SHIFT:
		return tr("Left Shift");
	case KEYCODE_RIGHT_SHIFT:
		return tr("Right Shift");
	case KEYCODE_LEFT_CTRL:
		return tr("Left Control");
	case KEYCODE_RIGHT_CTRL:
		return tr("Right Control");
	case KEYCODE_LEFT_ALT:
		return tr("Left Alt");
	case KEYCODE_RIGHT_ALT:
		return tr("Right Alt");
	case KEYCODE_TAB:
		return tr("Tab");
	case KEYCODE_INSERT:
		return tr("Insert");
	case KEYCODE_DELETE:
		return tr("Delete");
	case KEYCODE_HOME:
		return tr("Home");
	case KEYCODE_END:
		return tr("End");
	case KEYCODE_PG_UP:
		return tr("Page Up");
	case KEYCODE_PG_DOWN:
		return tr("Page Down");
	case KEYCODE_KP_PLUS:
		return tr("Keypad Plus");
	case KEYCODE_KP_MINUS:
		return tr("Keypad Minus");
	case KEYCODE_KP_MULTIPLY:
		return tr("Keypad Multiply");
	case KEYCODE_KP_DIVIDE:
		return tr("Keypad Divide");
	case KEYCODE_KP_ENTER:
		return tr("Keypad Enter");
	case KEYCODE_KP_HOME:
		return tr("Keypad Home");
	case KEYCODE_KP_UP:
		return tr("Keypad Up");
	case KEYCODE_KP_PG_UP:
		return tr("Keypad Page Up");
	case KEYCODE_KP_LEFT:
		return tr("Keypad Left");
	case KEYCODE_KP_FIVE:
		return tr("Keypad 5");
	case KEYCODE_KP_RIGHT:
		return tr("Keypad Right");
	case KEYCODE_KP_END:
		return tr("Keypad End");
	case KEYCODE_KP_DOWN:
		return tr("Keypad Down");
	case KEYCODE_KP_PG_DOWN:
		return tr("Keypad Page Down");
	case KEYCODE_KP_INSERT:
		return tr("Keypad Insert");
	case KEYCODE_KP_DELETE:
		return tr("Keypad Delete");
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

int Frontend::getControllerIndex(const QString &id)
{
	for (int i = 0; i < m_devices->size(); i++)
	{
		QString cid = m_devices->value(i)["id"].toString();
		if (QString::compare(cid, id) == 0)
			return i + 1;
	}
	return 0;
}

void Frontend::SaveState()
{
	m64p->SaveState();
}

void Frontend::LoadState()
{
	m64p->LoadState();
}

void Frontend::LoadTouchOverlay()
{
	m64p->LoadTouchOverlay();
}

void Frontend::ExitEmulator()
{
	m64p->ExitEmulator();
	m_emuRunning = false;
}

int Frontend::mapButton()
{
	int msg = 0, ret = 0;

	toastButton->cancel();
	toast->setBody(tr("Press a Button to Map"));
	toast->show();

	MsgSend(coid, &msg, sizeof(msg), &ret, sizeof(ret));

	toast->cancel();
	if(ret != -1)
	{
	}
	else
	{
		toastButton->setBody(tr("Cancelling"));
	    toastButton->show();
	}

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
    if (url.contains("nhl", Qt::CaseInsensitive) && url.contains("99") && !url.contains("breakaway", Qt::CaseInsensitive)
            && !url.contains("blades", Qt::CaseInsensitive))
    {
        QString info = "<Data>\n" \
                       "  <baseImgUrl>http://thegamesdb.net/banners/</baseImgUrl>\n" \
                       "  <Game>\n" \
                       "    <id>1169</id>\n" \
                       "    <GameTitle>NHL 99</GameTitle>\n" \
                       "    <PlatformId>3</PlatformId>\n" \
                       "    <ReleaseDate>10/01/1998</ReleaseDate>\n" \
                       "    <Overview>\n" \
                       "      The most celebrated hockey game comes to the Nintendo 64. Battle along the boards, feed the " \
                       "      open man, patrol the ice! Game Features: *Beginner lever - pick up and play!; *Coaching " \
                       "      strategies from Stanley Cup Winner Marc Crawford; *Commentary by Bill Clement; *5 game " \
                       "      modes; *18 top international teams; *Updated 1998 roster-expansion Nashville Predators.\n" \
                       "    </Overview>\n" \
                       "    <ESRB>E</ESRB>\n" \
                       "    <Genres>\n" \
                       "      <genre>Sports</genre>\n" \
                       "    </Genres>\n" \
                       "    <Players>2</Players>\n" \
                       "    <Co-op>No</Co-op>\n" \
                       "    <Publisher>Electronics Arts</Publisher>\n" \
                       "    <Developer>Electronics Arts</Developer>\n" \
                       "    <Rating>6</Rating>\n" \
                       "    <Images>\n" \
                       "      <boxart side=\"back\" width=\"2100\" height=\"1532\" thumb=\"boxart/thumb/original/back/1169-1.jpg\">boxart/original/back/1169-1.jpg</boxart>\n" \
                       "      <boxart side=\"front\" width=\"2100\" height=\"1532\" thum=\"boxart/thumb/original/front/1169-1.jpg\">boxart/original/front/1169-1.jpg</boxart>\n" \
                       "      <banner width=\"760\" height=\"140\">graphical/1169-g.jpg</banner>\n" \
                       "    </Images>\n" \
                       "  </Game>\n" \
                       "</Data>";
        onBoxArtRecieved(info, true);
    }
    else
    {
        m_boxartLoaded = false;
        emit boxartLoadedChanged(m_boxartLoaded);

        TwitterRequest* request = new TwitterRequest(this);
        connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onBoxArtRecieved(QString, bool)));
        request->requestTimeline(url);
    }
}

void Frontend::onBoxArtRecieved(const QString &info, bool success)
{
    TwitterRequest *request = qobject_cast<TwitterRequest*>(sender());

    if (success)
    {
    	XmlDataAccess dataAccess;
    	QString url = "";

		const QVariant variant = dataAccess.loadFromBuffer(info);

		const QVariantMap games = variant.toMap();

		//qDebug() << "URL: " << games["baseImgUrl"];
		url.append(games["baseImgUrl"].toString());

		if (!games.contains("Game"))
		{
	        m_boxartLoaded = true;
	        emit boxartLoadedChanged(m_boxartLoaded);
		}
		else
		{
            //IF there is one game, games["game"] will be a map rather than list
            QVariantList game = games["Game"].toList();

            //TODO: Will crash with one game
            //qDebug() << "GAME: " << game;

            QVariantMap selected;
            if(!game.isEmpty())
                selected = game[0].toMap();
            else
                selected = games["Game"].toMap();
            QVariantMap images = selected["Images"].toMap();
            QVariantList boxart = images["boxart"].toList();

            if (boxart.length() == 0)
            {
                m_boxartLoaded = true;
                emit boxartLoadedChanged(m_boxartLoaded);
            }
            else
            {
                url.append((boxart[1].toMap())[".data"].toString());

                //qDebug() << "FULL URL: " << url;

                //Could make obj once, and reuse
                if(m_boxart)
                    delete m_boxart;

                m_boxart = new ImageLoader(url, mRom);
                emit boxArtChanged(m_boxart);
                connect(m_boxart, SIGNAL(imageChanged()), SLOT(boxartDownloaded()));

                m_boxart->load();
            }
		}
    }

    if (request)
        request->deleteLater();
}

void Frontend::boxartDownloaded()
{
    m_boxartLoaded = true;
    emit boxartLoadedChanged(m_boxartLoaded);
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
    						toast->setBody(tr("Version %1.%2.%3 of Mupen64+ BB is available to upgrade your %4.%5.%6")
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
		strlcpy(controller->name, QObject::tr("Wiimote").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/wii.png";
	}
	else if (strstr(controller->id, "20D6-0DAD"))
	{
		strlcpy(controller->name, QObject::tr("MOGA Pro").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/moga.png";
	}
	else if (strstr(controller->id, "25B6-0001"))
	{
		strlcpy(controller->name, QObject::tr("Fructel Gametel").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/gametel.png";
	}
	else if (strstr(controller->id, "1038-1412"))
	{
		strlcpy(controller->name, QObject::tr("SteelSeries Free").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/steelseries.png";
	}
	else if (strstr(controller->id, "046D-C21D"))
	{
		strlcpy(controller->name, QObject::tr("Logitech F310").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/logitech.png";
	}
	else if (strstr(controller->id, "045E-028E"))
	{
		strlcpy(controller->name, QObject::tr("Microsoft XBox 360").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/xbox360.png";
	}
	else if (strstr(controller->id, "045E-0291"))
	{
		strlcpy(controller->name, QObject::tr("Microsoft XBox 360 Wireless").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/xbox360w.png";
	}
	else if (strstr(controller->id, "1689-FD01"))
	{
		strlcpy(controller->name, QObject::tr("Razer XBox 360").toAscii().constData(), sizeof(controller->name));
		return "asset:///images/razer.png";
	}
	strlcpy(controller->name, QObject::tr("Unknown Gamepad").toAscii().constData(), sizeof(controller->name));
	return "asset:///images/ca_bluetooth.png";
}

void Frontend::discoverControllers()
{
	int deviceCount;
	screen_get_context_property_iv(screen_cxt, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount);
	if (debug_mode)
	    fprintf(stderr, "%d connected devices\n", deviceCount);
	screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));
	screen_get_context_property_pv(screen_cxt, SCREEN_PROPERTY_DEVICES, (void**)devices);

	// Scan the list for gamepad and joystick devices.
	int i;
	int controllerIndex = 0;
	for (i = 0; i < deviceCount; i++)
	{
		int type;
		screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type);

		if (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)
		{
		    if (debug_mode)
		        fprintf(stderr, "Found a gamepad\n");
			GameController *control = &_controllers[controllerIndex];
			control->handle = devices[i];
			screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &control->type);
			screen_get_device_property_cv(devices[i], SCREEN_PROPERTY_ID_STRING, sizeof(control->id), control->id);
			screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_BUTTON_COUNT, &control->buttonCount);
            if (debug_mode)
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
			if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG0, control->analog0))
			{
				control->analogCount++;
	            if (debug_mode)
	                fprintf(stderr, "Device has analog0\n");
				if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG1, control->analog1))
				{
					control->analogCount++;
		            if (debug_mode)
		                fprintf(stderr, "Device has analog1\n");
					desc += " with analog0 and analog1";
				}
				else
					desc += " with analog0";
			}
			else if (!screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_ANALOG1, control->analog1))
			{
				control->analogCount++;
	            if (debug_mode)
	                fprintf(stderr, "Device has analog1\n");
				desc += " with analog1";
			}
			map["description"] = desc;
			(*m_devices) << map;
			emit createOption(QString::fromAscii(control->name), QString::fromAscii(control->id), QUrl(res));
			controllerIndex++;
		}
	}

	free(devices);
	emit controllersDetected();
}

//TODO bluetooth
void Frontend::discoverBluetoothDevices()
{
}

void Frontend::onCreateOption(QString name, QString value, QUrl imageSource)
{
	for (int i = 0; i < 4; i++)
	{
		DropDown* deviceDropdown = m_tab->findChild<DropDown*>("deviceList" + QString::number(i));
		if (deviceDropdown)
		{
			Option *opt = Option::create()
							.parent(deviceDropdown)
							.text(name)
							.value(value)
							.imageSource(imageSource);
			deviceDropdown->add(opt);
		}
		else
		{
		    printf("Error getting device list dropdown\n");fflush(stdout);
		}
	}
}

void Frontend::detectHDMI()
{
	int hdmi = DisplayInfo::secondaryDisplayId();
	if (hdmi >= 0)
	{
		m_hdmiInfo = new DisplayInfo(hdmi, this);
		if (m_hdmiInfo->isAttached())
		{
		    if (debug_mode)
		        fprintf(stderr, "Second Display: %s\nSize: (%d, %d)\n", m_hdmiInfo->displayName().toStdString().c_str(),
					m_hdmiInfo->pixelSize().width(), m_hdmiInfo->pixelSize().height());
			emit hdmiDetected(true);
			connect(m_hdmiInfo, SIGNAL(attachedChanged(bool)), this, SIGNAL(hdmiDetected(bool)));
		}
	}
}

void Frontend::onInvoke(const bb::system::InvokeRequest& request)
{
	QString target = request.target();
	if (QString::compare(target, "com.emulator.mupen64p.application") == 0)
	{
		QString file = request.uri().toString();
		if (file.startsWith("file://"))
			file = file.mid(7);
		emit invoked(file, false);
	}
	else if (QString::compare(target, "com.emulator.mupen64p.runnow") == 0)
	{
		QString file = request.uri().toString();
		if (file.startsWith("n64://"))
			file = file.mid(6);
		emit invoked(file, true);
	}
	else if (QString::compare(target, "com.emulator.mupen64p.load") == 0)
	{
		QString file = request.uri().toString();
		if (file.startsWith("m64://"))
			file = file.mid(6);
		emit invoked(file, false);
	}
	else if (QString::compare(target, "com.emulator.mupen64p.search") == 0)
	{
	    QString game = QString::fromUtf8(request.data());
	    QList<Game> history = getHistory();
	    bool found = false;
	    foreach (Game g, history)
	    {
	        if (g.baseName().contains(game, Qt::CaseInsensitive))
	        {
	            emit invoked(g.location(), false);
	            found = true;
	            break;
	        }
	    }
	    if (!found)
	    {
	        SystemToast *toast = new SystemToast;
	        toast->setBody(tr("Could not find a ROM corresponding to the search string \"") + game + "\"");
	        toast->show();
	    }
	}
}

bool Frontend::createShortcut(const QString& name, const QString& icon, const QString& location, bool run)
{
    //if (debug_mode)
        fprintf(stderr, "Shortcut: %s, %s, %s\n", name.toAscii().data(), (icon.right(icon.length() - 7)).toAscii().data(), location.toAscii().data());
    QFileInfo iconInfo(icon.right(icon.length() - 7));
    bool result = false;
    if (iconInfo.exists())
    {
        QImage img = QImage(icon.right(icon.length() - 7));
        QImage newimg = img.scaled(158, 158, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QImage newimg2(158, 158, QImage::Format_ARGB32);
        QPainter painter(&newimg2);
        int x, y;
        x = (int)(((double)(158 - newimg.width())) / 2.0);
        y = (int)(((double)(158 - newimg.height())) / 2.0);
        painter.drawImage(x, y, newimg, 0, 0, newimg.width(), newimg.height());
        newimg2.save("data/temp.png", "PNG");
        HomeScreen homeScreen;
        QString temp;
        if (run)
            temp = "n64://";
        else
            temp = "m64://";
        result = homeScreen.addShortcut(QUrl("file:data/temp.png"), name, QUrl(temp + location));
    }
	if (!result)
	{
		SystemToast *toast = new SystemToast();
		toast->setBody(tr("Error creating shortcut"));
		toast->show();
	}
	return result;
}

bool Frontend::isValidFilename(const QString &filename)
{
	static QRegExp reg("([^A-Za-z0-9_\\-\\.\\s\\+\\(\\)\\[\\]])");
	if (filename.length() == 0 || filename.length() > 20)
		return false;
	return reg.indexIn(filename) < 0;
}

void Frontend::backup()
{
    FilePicker* filePicker = new FilePicker();
    filePicker->setType(FileType::Other);
    filePicker->setTitle(tr("Backup"));
    filePicker->setMode(FilePickerMode::Saver);
    QStringList filter;
    filter << "*.zip";
    filePicker->setFilter(filter);

    filePicker->open();

    connect(filePicker, SIGNAL(fileSelected(const QStringList&)), SLOT(backupFileSelected(const QStringList&)));
    connect(filePicker, SIGNAL(canceled()), SLOT(backupCanceled()));
}

#define MAXFILENAME (256)

uLong filetime(const char* f, tm_zip *tmzip)
{
    int ret = 0;
    struct stat s;
    struct tm* filedate;
    time_t tm_t = 0;

    if (strcmp(f, "-") != 0)
    {
        char name[MAXFILENAME+1];
        int len = strlen(f);
        if (len > MAXFILENAME)
            len = MAXFILENAME;

        strncpy(name, f, MAXFILENAME - 1);
        name[MAXFILENAME] = '\0';
        if (name[len - 1] == '/')
            name[len - 1] = '\0';
        if (stat(name, &s) == 0)
        {
            tm_t = s.st_mtime;
            ret = 1;
        }
    }
    filedate = localtime(&tm_t);
    tmzip->tm_sec = filedate->tm_sec;
    tmzip->tm_min = filedate->tm_min;
    tmzip->tm_hour = filedate->tm_hour;
    tmzip->tm_mday = filedate->tm_mday;
    tmzip->tm_mon = filedate->tm_mon;
    tmzip->tm_year = filedate->tm_year;

    return ret;
}

void change_file_date(const char* filename, tm_unz tmu_date)
{
    struct utimbuf ut;
    struct tm newdate;
    newdate.tm_sec = tmu_date.tm_sec;
    newdate.tm_min = tmu_date.tm_min;
    newdate.tm_hour = tmu_date.tm_hour;
    newdate.tm_mday = tmu_date.tm_mday;
    newdate.tm_mon = tmu_date.tm_mon;
    if (tmu_date.tm_year > 1900)
        newdate.tm_year = tmu_date.tm_year - 1900;
    else
        newdate.tm_year = tmu_date.tm_year;
    newdate.tm_isdst = -1;
    ut.actime = ut.modtime = mktime(&newdate);
    utime(filename, &ut);
}

void writeFileToZip(QString filepath, zipFile zf, QString dirPrefix)
{
    FILE *fin;
    zip_fileinfo zi;
    int size_read;
    void *buf = (void*)malloc(16384);
    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
            zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    filetime(filepath.toAscii().constData(), &zi.tmz_date);
    int err = zipOpenNewFileInZip3(zf, (dirPrefix + QFileInfo(filepath).fileName()).toAscii().constData(),
            &zi, NULL, 0, NULL, 0, NULL, 0, Z_DEFAULT_COMPRESSION, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL, 0);
    fin = fopen(filepath.toAscii().constData(), "rb");
    if (fin == NULL)
    {
        zipCloseFileInZip(zf);
        printf("Error openning file %s\n", filepath.toAscii().constData());
        return;
    }

    do
    {
        err = ZIP_OK;
        size_read = (int)fread(buf, 1, 16384, fin);
        if (size_read < 16384 && feof(fin) == 0)
        {
            err = ZIP_ERRNO;
        }
        if (size_read > 0)
        {
            err = zipWriteInFileInZip(zf, buf, size_read);
            if (err < 0)
                break;
        }
    }
    while ((err == ZIP_OK) && (size_read > 0));

    if (fin)
        fclose(fin);
    zipCloseFileInZip(zf);
    free(buf);
}

int extract_current_file(unzFile uf)
{
    unz_file_info file_info;
    char* p;
    char* filename_withoutpath;
    char filename_inzip[256];
    FILE *fout=NULL;
    int err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

    if (err != UNZ_OK)
    {
        printf("Unable to get file info for current file\n");
        return err;
    }
    uInt size_buf = 16384;
    void* buf = (void*)malloc(size_buf);

    printf("Extracting %s\n", filename_inzip);
    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p) == '/') || ((*p) == '\\'))
            filename_withoutpath = p + 1;
        p++;
    }
    err = unzOpenCurrentFile(uf);
    if (err != UNZ_OK)
    {
        printf("Unable to open file\n");
        return err;
    }

    QString writepath;
    if (strstr(filename_inzip, "data/"))
        writepath = "shared/misc/n64/data/";
    else if (strstr(filename_inzip, "save/"))
        writepath = "shared/misc/n64/save/";
    else if (strcmp(filename_withoutpath, "Trolltech.conf") == 0)
        writepath = "data/Settings/";
    else if (strcmp(filename_withoutpath, "mupen64p.conf"))
        writepath = "data/Settings/emulators/";
    else
    {
        printf("Unknown file\n");
        unzCloseCurrentFile(uf);
        return UNZ_ERRNO;
    }
    writepath = writepath + filename_withoutpath;
    fout = fopen(writepath.toAscii().constData(), "wb");

    do
    {
        err = unzReadCurrentFile(uf, buf, size_buf);
        if (err < 0)
        {
            printf("Error reading from file\n");
            break;
        }
        if (err > 0)
        {
            if (fwrite(buf, err, 1, fout) != 1)
            {
                printf("Error writing to file\n");
                err = UNZ_ERRNO;
                break;
            }
        }
    }
    while (err > 0);

    if (fout)
        fclose(fout);
    if (err == 0)
        change_file_date(writepath.toAscii().constData(), file_info.tmu_date);
    unzCloseCurrentFile(uf);
    free(buf);
    return err;
}

void Frontend::backupFileSelected(const QStringList& list)
{
    if (list.length() == 0)
        return;
    QString filename = list[0];
    if (!filename.endsWith(".zip"))
        filename = filename + ".zip";
    zipFile file = zipOpen(filename.toAscii().constData(), APPEND_STATUS_CREATE);
    SystemToast *toast = new SystemToast;
    if (!file)
    {
        toast->setBody(tr("Error creating zip file"));
        toast->show();
        return;
    }
    QDir dir("shared/misc/n64/data");
    dir.setFilter(QDir::Files);
    foreach (QString name, dir.entryList())
    {
        printf("Archiving %s\n", name.toAscii().constData());
        writeFileToZip("shared/misc/n64/data/" + name, file, "data/");
    }
    dir = QDir("shared/misc/n64/save");
    dir.setFilter(QDir::Files);
    foreach (QString name, dir.entryList())
    {
        printf("Archiving %s\n", name.toAscii().constData());
        writeFileToZip("shared/misc/n64/save/" + name, file, "save/");
    }
    printf("Archiving Trolltech.conf\n");
    writeFileToZip("data/Settings/Trolltech.conf", file, "app/");
    printf("Archiving mupen64p.conf\n");
    writeFileToZip("data/Settings/emulators/mupen64p.conf", file, "app/");
    zipClose(file, NULL);
    toast->setBody(tr("Backup created"));
    toast->show();
    fflush(stdout);
    emit backupComplete();
}

void Frontend::backupCanceled()
{
    emit backupComplete();
}

void Frontend::restore()
{
    SystemDialog *dialog = new SystemDialog(tr("Continue"), tr("Cancel"));
    dialog->setTitle(tr("Restore Data"));
    dialog->setBody(tr("Restoring data will cause all current settings and saves to be lost. Are you sure you want to continue?"));
    connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), SLOT(onRestoreAccepted(bb::system::SystemUiResult::Type)));
    dialog->show();
}

void Frontend::onRestoreAccepted(SystemUiResult::Type type)
{
    SystemDialog *dialog = qobject_cast<SystemDialog*>(sender());
    if (type == SystemUiResult::ConfirmButtonSelection)
    {
        FilePicker* filePicker = new FilePicker();
        filePicker->setType(FileType::Other);
        filePicker->setTitle(tr("Restore"));
        filePicker->setMode(FilePickerMode::Picker);
        QStringList filter;
        filter << "*.zip";
        filePicker->setFilter(filter);

        filePicker->open();

        connect(filePicker, SIGNAL(fileSelected(const QStringList&)), SLOT(restoreFileSelected(const QStringList&)));
        connect(filePicker, SIGNAL(canceled()), SIGNAL(restoreCanceled()));
    }
    else
        emit restoreCanceled();
    dialog->deleteLater();
}

void Frontend::restoreFileSelected(const QStringList& list)
{
    if (list.length() == 0)
        return;
    QString filename = list[0];
    SystemToast *toast = new SystemToast;
    if (!filename.endsWith(".zip"))
    {
        toast->setBody(tr("Invalid file selected"));
        toast->show();
        emit restoreCanceled();
        return;
    }
    Q_UNUSED(list);
    m_settings->sync();
    m_gameSettings->sync();
    QSettings().sync();

    unzFile uf = unzOpen(filename.toAscii().constData());
    if (uf == NULL)
    {
        toast->setBody(tr("Couldn't open the selected file"));
        toast->show();
        emit restoreCanceled();
        return;
    }

    unz_global_info gi;
    int err = unzGetGlobalInfo(uf, &gi);

    for (uLong i = 0; i < gi.number_entry; i++)
    {
        if (extract_current_file(uf) != UNZ_OK)
        {
            toast->setBody(tr("Error extracting files"));
            toast->show();
            break;
        }
        if ((i + 1) < gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK)
            {
                toast->setBody(tr("Error iterating zip file"));
                toast->show();
                break;
            }
        }
    }
    unzClose(uf);

    if (err == UNZ_OK)
        emit restoreComplete();
    else
        emit restoreCanceled();
}

void Frontend::getGameInfo()
{
    navigator_invoke_invocation_t *invoke;
    navigator_invoke_invocation_create(&invoke);
    navigator_invoke_invocation_set_action(invoke, "bb.action.OPEN");
    navigator_invoke_invocation_set_uri(invoke, ("http://thegamesdb.net/game/" + QString::number(m_gameInfo.Id())).toAscii().constData());
    navigator_invoke_invocation_send(invoke);
    navigator_invoke_invocation_destroy(invoke);
}
