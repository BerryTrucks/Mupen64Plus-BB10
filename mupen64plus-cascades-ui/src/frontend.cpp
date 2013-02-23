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
#include <QSettings>
#include <bb/cascades/Window>
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FileType>
#include <bb/cascades/pickers/ViewMode>
#include <QSignalMapper>
#include <bb/data/XmlDataAccess>

#include <bb/system/SystemToast>
#include <bps/bps.h>
#include <bps/screen.h>

#include "main.h"
#include "bbutil.h"

#include <sys/neutrino.h>

using namespace bb::cascades;
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

void Frontend::create_button_mapper() {
	const int usage2 = SCREEN_USAGE_NATIVE | SCREEN_USAGE_WRITE | SCREEN_USAGE_READ;
	int rc;

	screen_create_context(&screen_cxt, SCREEN_APPLICATION_CONTEXT);
	screen_create_window_type(&screen_win_map, screen_cxt, SCREEN_CHILD_WINDOW);

	screen_join_window_group(screen_win_map, Application::instance()->mainWindow()->groupId().toAscii().constData());
	int format = SCREEN_FORMAT_RGBA8888;
	screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_FORMAT, &format);

	screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_USAGE, &usage2);

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

Frontend::Frontend()
{
	qmlRegisterType<bb::cascades::pickers::FilePicker>("bb.cascades.pickers", 1, 0, "FilePicker");
	qmlRegisterUncreatableType<bb::cascades::pickers::FileType>("bb.cascades.pickers", 1, 0, "FileType", "");
	qmlRegisterType<ImageLoader>();

	//Set up a
	mStartEmu = false;
	m_boxart = 0;

	if(access("shared/misc/n64/", F_OK) != 0){
		mkdir("shared/misc/n64/", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/data", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/roms", S_IRWXU | S_IRWXG);
		mkdir("shared/misc/n64/.boxart", S_IRWXU | S_IRWXG);

		char buf[8192];
		size_t size;

		FILE* source = fopen("app/native/mupen64plus.cfg", "rb");
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
        TabbedPane *tab = qml->createRootObject<TabbedPane>();
        if (tab) {
        	mCheatsContainer = tab->findChild<Container*>("cheats");

            Application::instance()->setScene(tab);

            start();
        }
    }
}

Frontend::~Frontend()
{
	printf("Saving COntrollers!\n");fflush(stdout);
	m64p->save_controller_config(0);
	m64p->save_controller_config(1);
	m64p->save_controller_config(2);
	m64p->save_controller_config(3);
	printf("Finished Saving Controllers!\n");fflush(stdout);
}

void Frontend::onManualExit(){
	printf("OnManualExit!\n");fflush(stdout);
	int msg = 2, ret = 0;
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

	create_button_mapper();

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
					}
				}
			}

			z = -10;
			if (screen_set_window_property_iv(screen_win_map, SCREEN_PROPERTY_ZORDER, &z) != 0) {
				return;
			}
			screen_post_window(screen_win_map, screen_buf[0], 1, rect, 0);

			int ret = sym&0xff;
			MsgReply(rcvid, 0, &ret, sizeof(ret));
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
		} else {
			m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"gles2n64");
		}

		m64p->Start();
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
	if(!section.contains("Input-SDL-Control")) {
		if(this->getValueFor("perRom", "true") == "true"){
			return this->getValueFor(mRom.mid(mRom.lastIndexOf('/')+1) + name, value);
		} else {
			return this->getValueFor(name, value);
		}
	}
	return NULL;
}

void Frontend::startEmulator(bool start_now)
{
	//start();
	int msg = 1;
	int ret = 0;
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);
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
			int i;
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

	if(value == "present"){
		return m64p->controller[player].present;
	}
	if(value == "device") {
		return m64p->controller[player].device;
	}
	if(value == "layout") {
		return m64p->controller[player].layout;
	}

	for(i=0; i<16; ++i){
		if(button_names[i] == value){
			return m64p->controller[player].button[i];
		}
	}

	if(value == "X Axis Left"){
		return m64p->controller[player].axis[0].a;
	} else if (value == "X Axis Right"){
		return m64p->controller[player].axis[0].b;
	} else if (value == "Y Axis Up"){
		return m64p->controller[player].axis[1].a;
	} else if (value == "Y Axis Down"){
		return m64p->controller[player].axis[1].b;
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
