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
#ifndef FRONTEND_H
#define FRONTEND_H

#include <History/Game.hpp>

#include "settings/M64PSettings.hpp"
#include "GameInfo/GameInfo.hpp"
#include "RiceINI.hpp"

#include "emulator.h"
#include "bbutil.h"

#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/DataModel>
#include <bb/cascades/DropDown>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/ActionBarPlacement>
#include <bb/system/InvokeRequest>
#include <bb/system/SystemUiResult>

#include <QDir>
#include <QThread>
#include <QSettings>
#include <QTimer>
#include <QPropertyAnimation>
#include <QAtomicInt>
#include <QPoint>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <bb/device/DisplayInfo>

#include <bps/deviceinfo.h>

#include "imageloader.hpp"
#include "NetRequest.hpp"

using namespace bb::cascades;

extern QSettings *m_settings;


class Frontend: public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString rom READ getRom WRITE setRom NOTIFY romChanged)
    Q_PROPERTY(QString startDirectory READ getStartDirectory WRITE setStartDirectory NOTIFY dummySignal)
    Q_PROPERTY(bool hasStartDirectory READ hasStartDirectory CONSTANT)
    Q_PROPERTY(ImageLoader* boxart READ getBoxArt NOTIFY boxArtChanged)
    Q_PROPERTY(bool boxartLoaded READ boxartLoaded NOTIFY boxartLoadedChanged)
    Q_PROPERTY(bool Keyboard READ hasKeyboard CONSTANT)
    Q_PROPERTY(bool useHdmi READ useHdmi WRITE useHdmi NOTIFY dummySignal)
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int hight READ height CONSTANT)
    Q_PROPERTY(int emuWidth READ emuWidth CONSTANT)
    Q_PROPERTY(int emuHeight READ emuHeight CONSTANT)
	Q_PROPERTY(bb::cascades::DataModel* devices READ devices CONSTANT)
	Q_PROPERTY(bb::cascades::DataModel* history READ history CONSTANT)
	Q_PROPERTY(bool hasHistory READ hasHistory CONSTANT)
	Q_PROPERTY(int menuOffset READ menuOffset WRITE menuOffset NOTIFY menuOffsetChanged)
	Q_PROPERTY(bool debugMode READ debugMode CONSTANT)
	Q_PROPERTY(QString coverImage READ coverImage NOTIFY coverImageChanged)
	Q_PROPERTY(QString version READ version CONSTANT)
	Q_PROPERTY(QString compileDate READ compileDate CONSTANT)
	Q_PROPERTY(QString currentROM READ currentROM NOTIFY currentROMChanged)
	Q_PROPERTY(bool isOSThree READ isOSThree CONSTANT)
	Q_PROPERTY(bb::cascades::ActionBarPlacement::Type playPlacement READ playPlacement CONSTANT)
	Q_PROPERTY(bool hdmi1080 READ hdmi1080 WRITE hdmi1080 NOTIFY dummySignal)
	Q_PROPERTY(int hdmiResolution READ hdmiResolution WRITE hdmiResolution NOTIFY dummySignal)
	Q_PROPERTY(int rotateQ10 READ rotateQ10 NOTIFY rotationChanged)
	Q_PROPERTY(int numMenuItems READ numMenuItems NOTIFY numMenuItemsChanged)
	Q_PROPERTY(bool noTouchScreenControllers READ noTouchScreenControllers NOTIFY touchScreenControllerCountChanged)
	Q_PROPERTY(int SaveStateSlot READ SaveStateSlot WRITE SaveStateSlot NOTIFY SaveStateSlotChanged)
	Q_PROPERTY(bool UseForeignWindowControl READ UseForeignWindowControl NOTIFY UseForeignWindowControlChanged)
	Q_PROPERTY(QString ImageName READ ImageName CONSTANT)
    Q_PROPERTY(RiceINI* RiceINI READ getRiceINI CONSTANT)
    Q_PROPERTY(GameInfo* GameInfo READ getInfo CONSTANT)

	Q_PROPERTY(QString info_goodname READ info_goodname CONSTANT)
	Q_PROPERTY(QString info_name READ info_name CONSTANT)
	Q_PROPERTY(QString info_md5 READ info_md5 CONSTANT)
	Q_PROPERTY(QString info_crc READ info_crc CONSTANT)
	Q_PROPERTY(QString info_imagetype READ info_imagetype CONSTANT)
	Q_PROPERTY(QString info_romsize READ info_romsize CONSTANT)
	Q_PROPERTY(QString info_version READ info_version CONSTANT)
	Q_PROPERTY(QString info_manufacturer READ info_manufacturer CONSTANT)
	Q_PROPERTY(QString info_country READ info_country CONSTANT)

private:
    enum ThemeColor
    {
    	Bright = 2,
    	Dark = 1,
    	Default = 0
    };

public:
    // This is our constructor that sets up the recipe.
    Frontend();
    ~Frontend();

    /* Invokable functions that we can call from QML*/
    Q_INVOKABLE
    void invokeLoadHistoryROM(const QString& rom) { m_historyROM = QString(rom); QTimer::singleShot(0, this, SLOT(onLoadHistoryROM())); }
    Q_INVOKABLE
    void invokePlayHistoryROM(const QString& rom) { m_historyROM = QString(rom); QTimer::singleShot(0, this, SLOT(onPlayHistoryROM())); }
    Q_INVOKABLE
    void startEmulator(bool start);
    Q_INVOKABLE
    void createCheatsPage();
    Q_INVOKABLE
    void LoadRom();
    Q_INVOKABLE
    QString getMogaInputCharacter(int value);
    Q_INVOKABLE
    QString getInputCharacter(int value);
    Q_INVOKABLE
    int getControllerIndex(const QString &id);
    Q_INVOKABLE
    void SaveState();
    Q_INVOKABLE
    void LoadState();
    Q_INVOKABLE
    void LoadTouchOverlay();
    Q_INVOKABLE
    int mapButton();
    Q_INVOKABLE
    void ExitEmulator();
    Q_INVOKABLE
	void loadBoxArt(const QString &url);
    Q_INVOKABLE
	void removeFromHistory(QString uuid);
    Q_INVOKABLE
    void changeHistoryName(QString uuid);
    Q_INVOKABLE
    void pinHistory(QString uuid);
    Q_INVOKABLE
    void unpinHistory(QString uuid);
    Q_INVOKABLE
	void clearHistory();
    Q_INVOKABLE
    void swipedown();
    Q_INVOKABLE
    bool createShortcut(const QString& name, const QString& icon, const QString& location, bool run);
    Q_INVOKABLE
    bool isValidFilename(const QString &filename);
    Q_INVOKABLE
    inline void addMupenMenuItem() { m_numMenuItems.ref(); emit numMenuItemsChanged(); }
    Q_INVOKABLE
    inline void removeMupenMenuItem() { m_numMenuItems.deref(); emit numMenuItemsChanged(); }
    Q_INVOKABLE
    void pressGameshark();
    Q_INVOKABLE
    void selectState(bool save);
    Q_INVOKABLE
    void getGameInfo();
    Q_INVOKABLE
    bool fileExists(const QString& filename);

    //invokables for creating custom overlays
public:
    Q_INVOKABLE
    void createLayout();
    Q_INVOKABLE
    void placeStart(int x, int y);
    Q_INVOKABLE
    void placeLeft(int x, int y);
    Q_INVOKABLE
    void placeRight(int x, int y);
    Q_INVOKABLE
    void placeAnalog(int x, int y);
    Q_INVOKABLE
    void placeDPad(int x, int y);
    Q_INVOKABLE
    void placeCPad(int x, int y);
    Q_INVOKABLE
    void placeZ(int x, int y);
    Q_INVOKABLE
    void placeA(int x, int y);
    Q_INVOKABLE
    void placeB(int x, int y);
    Q_INVOKABLE
    void backup();
    Q_INVOKABLE
    void restore();

    QString getRom();
    void setRom(QString i);

signals:
	void romChanged(QString);
	void boxArtChanged(ImageLoader*);
	void boxartLoadedChanged(bool);
	void dummySignal();
	void hdmiDetected(bool hdmi);
	void hasHistoryChanged();
	void invoked(QString url, bool runnow);
	void menuOffsetChanged();
	void coverImageChanged();
	void createOption(QString name, QString value, QUrl imageSource);
	void controllersDetected();
	void loadHistoryROM(QString rom);
	void playHistoryROM(QString rom);
	void currentROMChanged();
	void rotationChanged();
	void numMenuItemsChanged();
	void touchScreenControllerCountChanged();
	void SaveStateSlotChanged();
	void ROMLoaded();
	void FocusForeignWindow();
	void UseForeignWindowControlChanged();
	void overlayCreationComplete();
	void restoreComplete();
	void backupComplete();
    void restoreCanceled();
    void gameHasId(bool has);

public slots:
	void addCheatToggle(int);
	void addCheatDropDown(int);
	void onManualExit();
	void onThumbnail();
	void onBoxArtRecieved(const QString &info, bool success);
	void onVersionRecieved(const QString &info, bool success);
	void onInvoke(const bb::system::InvokeRequest& req);
	void showMenuFinished();
	void onMenuOffsetChanged();
	void onCreateOption(QString name, QString value, QUrl imageSource);
	void onLoadHistoryROM() { emit loadHistoryROM(m_historyROM); }
	void onPlayHistoryROM() { emit playHistoryROM(m_historyROM); }
	void startEmulatorInternal();
	void releaseGameshark();
	void pressFastforward();
	void releaseFastforward();
	void saveStateSlotSelected(bb::system::SystemUiResult::Type result);
	void saveStateFileSelected(const QStringList &files);
    void createVisual();
    void create1024();
    void create1280();
    void backupFileSelected(const QStringList& list);
    void backupCanceled();
    void restoreFileSelected(const QStringList& list);
    void onRestoreAccepted(bb::system::SystemUiResult::Type type);
    void historyRenameSelected(bb::system::SystemUiResult::Type);
    void boxartDownloaded();
    void onIdDiscovered(const QString& info);
	//void emitSendCheat();
	//void handleSendCheat();
    void onPlayReleased();
    void onCapturePressed();
    void saveScreenShot();
    void swipedown_external() { swipedown(); }

public:
    inline bool hasKeyboard() const { deviceinfo_details_t* details; deviceinfo_get_details(&details); bool retval = deviceinfo_details_get_keyboard(details) == DEVICEINFO_KEYBOARD_PRESENT; deviceinfo_free_details(&details); return retval; }
    inline QString compileDate() const { return QString(__DATE__).simplified(); }
    inline void focus() { emit FocusForeignWindow(); }
#ifdef BB103
    static inline bool isOSThree() const { return true; }
#else
    static bool isOSThree();
#endif

private:
    inline int width() const { bb::device::DisplayInfo info; return info.pixelSize().width(); }
    inline int height() const { bb::device::DisplayInfo info; return info.pixelSize().height(); }
    inline int emuWidth() const { int w = width(); int h = height(); if (h > w) return h; return w; }
    inline int emuHeight() const { int w = width(); int h = height(); if (h > w) return w; return h; }
    inline QString getStartDirectory() const { return m_settings->value("StartDirectory", "").toString(); }
    inline void setStartDirectory(const QString& dir) { m_settings->setValue("StartDirectory", dir); }
    inline bool hasStartDirectory() const { return (getStartDirectory()).length() > 0; }
    inline bb::cascades::DataModel* devices() const { return m_devices; }
    inline bb::cascades::DataModel* history() const { return m_history; }
    inline bool hasHistory() const { return m_history->size() > 0; }
    inline bool saveHistory() const { return m_settings->value("SAVE_HISTORY", true).toBool(); }
    inline void saveHistory(bool value) { m_settings->setValue("SAVE_HISTORY", value); clearHistory(); }
    inline int menuOffset() const { return m_menuOffset; }
    inline QString coverImage() const { return m_coverImage; }
    inline void menuOffset(int offset) { m_menuOffset = offset; emit menuOffsetChanged(); }
    inline QString version() const { return QString("%1.%2.%3").arg(QString::number(VERSION_MAJOR),
    		QString::number(VERSION_MINOR), QString::number(VERSION_RELEASE)); }
    inline QString lastROM() const { if (!hasHistory()) return ""; return m_history->value(0)["name"].toString(); }
    inline QString currentROM() const { return m_currentROM; }
    inline bool useHdmi() const { return m_useHdmi; }
    inline void useHdmi(bool use) { m_useHdmi = use; }
    inline bool hdmi1080() const { return m_settings->value("HDMI_1080", false).toBool(); }
    inline void hdmi1080(bool use) { m_settings->setValue("HDMI_1080", use); }
    inline int hdmiResolution() const { return m_settings->value("HDMI_RESOLUTION", 4).toInt(); }
    inline void hdmiResolution(int val) { m_settings->setValue("HDMI_RESOLUTION", val); }
    inline int rotateQ10() const { return q10_rotate; }
    inline int numMenuItems() const { return m_numMenuItems; }
    inline bool noTouchScreenControllers() const { return m_noTouchScreenControllers; }
    inline bool UseForeignWindowControl() const { return use_gamepad; }
    inline QString workingDir() const { return QDir::currentPath(); }
    inline QString ImageName() const { return "file://" + workingDir() + "/" + m_imageName; }
    inline bool loadingGame() const { return m_loadingGame; }
    inline RiceINI* getRiceINI() { return &m_riceini; }
    inline GameInfo* getInfo() { return &m_gameInfo; }
#ifdef BB103
    inline bool isOSThreeCompiled() const { return true; }
    inline bb::cascades::ActionBarPlacement::Type playPlacement() const { return bb::cascades::ActionBarPlacement::Signature; }
#else
    inline bool isOSThreeCompiled() const { return false; }
    inline bb::cascades::ActionBarPlacement::Type playPlacement() const { if (isOSThree()) return (bb::cascades::ActionBarPlacement::Type)3; return bb::cascades::ActionBarPlacement::OnBar; }
#endif

    QString info_goodname();
    QString info_name();
    QString info_md5();
    QString info_crc();
    QString info_imagetype();
    QString info_romsize();
    QString info_version();
    QString info_manufacturer();
    QString info_country();

    int SaveStateSlot();
    void SaveStateSlot(int slot);
    void run();
    Container *createCheatToggle(sCheatInfo *pCur);
    Container *createCheatDropDown(sCheatInfo *pCur);
    void create_button_mapper();
    bool boxartLoaded();
	ImageLoader* getBoxArt();
	void discoverControllers();
	void detectHDMI();
	QList<Game> getHistory();
	void setHistory(QList<Game> list);
	void addToHistory(QString title);
	bool debugMode();
	void setupGlide();

private:
    void refreshColours();
    void refreshTheme();

    void discoverBluetoothDevices();

private:
    bool m_emuRunning;
    QString mRom;
    QMutex *m_animationLock;
    Container *mCheatsContainer;
    ImageLoader* m_boxart;
    bb::cascades::TabbedPane *m_tab;
	bool m_boxartLoaded;
    int m_menuOffset;
    QString m_coverImage;
    QString m_currentROM;
    QString m_historyROM;
    int VERSION_MAJOR;
    int VERSION_MINOR;
    int VERSION_RELEASE;
    bb::device::DisplayInfo *m_hdmiInfo;
    bb::cascades::QMapListDataModel* m_devices;
    bb::cascades::QMapListDataModel* m_history;
    QPropertyAnimation* m_menuAnimation;
    bool m_useHdmi;
    QAtomicInt m_numMenuItems;
    bool m_noTouchScreenControllers;
    M64PSettings* m_gameSettings;
    bool m_selectStateSaving;
    bool m_loadingGame;
    GameInfo m_gameInfo;
    RiceINI m_riceini;

public:
    static QQueue<QPair<int, unsigned char*> > s_screenQueue;
    static int s_isOsThree;

    //variables for creating custom overlays
private:
    QPoint m_start;
    QPoint m_left;
    QPoint m_right;
    QPoint m_analog;
    QPoint m_dpad;
    QPoint m_cpad;
    QPoint m_ztrigg;
    QPoint m_a;
    QPoint m_b;
    QString m_imageName;
};

#endif // ifndef STARSHIPSETTINGSAPP_H
