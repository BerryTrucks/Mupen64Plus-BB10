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

#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/DataModel>
#include <bb/cascades/DropDown>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/TabbedPane>
#include <bb/system/InvokeRequest>
#include "emulator.h"
#include <QThread>
#include <QSettings>
#include <QPropertyAnimation>

#include <bb/device/DisplayInfo>

#include <bps/deviceinfo.h>

#include "imageloader.hpp"
#include "NetRequest.hpp"

using namespace bb::cascades;

#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define VERSION_RELEASE 3

/**
 * StarshipSettings Description:
 *
 * This sample application illustrates how to keep your applications current state
 * via QSettings.
 *
 * Feature summary
 * - setting up and communicating with QSettings
 */

class Frontend: public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString rom READ getRom WRITE setRom NOTIFY romChanged)
    Q_PROPERTY(QString startDirectory READ getStartDirectory WRITE setStartDirectory NOTIFY dummySignal)
    Q_PROPERTY(bool hasStartDirectory READ hasStartDirectory NOTIFY dummySignal)
    Q_PROPERTY(int video READ getVideo WRITE setVideo NOTIFY videoChanged)
    Q_PROPERTY(int audio READ getAudio WRITE setAudio NOTIFY audioChanged)
    Q_PROPERTY(ImageLoader* boxart READ getBoxArt NOTIFY boxArtChanged)
    Q_PROPERTY(bool boxartLoaded READ boxartLoaded NOTIFY boxartLoadedChanged)
    Q_PROPERTY(bool Keyboard READ hasKeyboard NOTIFY dummySignal)
    Q_PROPERTY(int colorIndex READ colorIndex NOTIFY dummySignal)
    Q_PROPERTY(int width READ width NOTIFY dummySignal)
    Q_PROPERTY(int hight READ height NOTIFY dummySignal)
    Q_PROPERTY(int emuWidth READ emuWidth NOTIFY dummySignal)
    Q_PROPERTY(int emuHeight READ emuHeight NOTIFY dummySignal)
	Q_PROPERTY(bb::cascades::DataModel* devices READ devices NOTIFY dummySignal)
	Q_PROPERTY(bb::cascades::DataModel* history READ history NOTIFY dummySignal)
	Q_PROPERTY(bool hasHistory READ hasHistory NOTIFY hasHistoryChanged)
	Q_PROPERTY(bool saveHistory READ saveHistory WRITE saveHistory NOTIFY dummySignal)
	Q_PROPERTY(int menuOffset READ menuOffset WRITE menuOffset NOTIFY menuOffsetChanged)
	Q_PROPERTY(bool debugMode READ debugMode NOTIFY dummySignal)
	Q_PROPERTY(QString coverImage READ coverImage NOTIFY coverImageChanged)
	Q_PROPERTY(QString version READ version NOTIFY dummySignal)

private:
    enum ThemeColor
    {
    	Bright = 2,
    	Dark = 1,
    	Default = 0
    };

public:
    // This is our constructor that sets up the recipe.
    Frontend(int theme);
    ~Frontend();

    /* Invokable functions that we can call from QML*/

    /**
     * This Invokable function gets a value from the QSettings,
     * if that value does not exist in the QSettings database, the default value is returned.
     *
     * @param objectName Index path to the item
     * @param defaultValue Used to create the data in the database when adding
     * @return If the objectName exists, the value of the QSettings object is returned.
     *         If the objectName doesn't exist, the default value is returned.
     */
    Q_INVOKABLE
    QString getValueFor(const QString &objectName, const QString &defaultValue);

    /**
     * This function sets a value in the QSettings database. This function should to be called
     * when a data value has been updated from QML
     *
     * @param objectName Index path to the item
     * @param inputValue new value to the QSettings database
     */
    Q_INVOKABLE
    void saveValueFor(const QString &objectName, const QString &inputValue);
    Q_INVOKABLE
    void saveConfigValue(const QString &section, const QString &name, const QString &value);
    Q_INVOKABLE
    QString getConfigValue(const QString &rom, const QString &section, const QString &name, const QString &value);
    Q_INVOKABLE
    void startEmulator(bool start);
    Q_INVOKABLE
    void createCheatsPage();
    Q_INVOKABLE
    void LoadRom();
    Q_INVOKABLE
    int getInputValue(int player, QString value);
    Q_INVOKABLE
    void setInputValue(int player, QString button, int value);
    Q_INVOKABLE
    QString getMogaInputCharacter(int value);
    Q_INVOKABLE
    QString getInputCharacter(int value);
    Q_INVOKABLE
    void setControllerID(int player, QString value);
    Q_INVOKABLE
    QString getControllerID(int player);
    Q_INVOKABLE
    int getControllerIndex(int player);
    Q_INVOKABLE
    void setControllerIndex(int player, int index);
    Q_INVOKABLE
    void setMogaInputValue(int player, QString button, int index);
    Q_INVOKABLE
    int getMogaInputValue(int player, QString button);
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
    void setBright(int index);
    Q_INVOKABLE
	void removeFromHistory(QString uuid);
    Q_INVOKABLE
	void clearHistory();
    Q_INVOKABLE
    void swipedown();

    QString getRom();
    int getVideo();
    int getAudio();
    void setRom(QString i);
    void setVideo(int i);
    void setAudio(int i);

signals:
	void romChanged(QString);
	void videoChanged(int);
	void audioChanged(int);
	void boxArtChanged(ImageLoader*);
	void boxartLoadedChanged(bool);
	void dummySignal();
	void hdmiDetected(bool hdmi);
	void hasHistoryChanged();
	void invoked(QString url);
	void menuOffsetChanged();
	void coverImageChanged();
	void createOption(QString name, QUrl imageSource);
	void controllersDetected();

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
	void onCreateOption(QString name, QUrl imageSource);
	//void emitSendCheat();
	//void handleSendCheat();

public:
    inline bool hasKeyboard() const { deviceinfo_details_t* details; deviceinfo_get_details(&details); bool retval = deviceinfo_details_get_keyboard(details) == DEVICEINFO_KEYBOARD_PRESENT; deviceinfo_free_details(&details); return retval; }

private:
    inline bool getBright() const { return m_isbright; }
    inline int colorIndex() const { return m_color == Bright ? 0 : (m_color == Default ? (m_isbright ? 0 : 1) : 1); }
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

private:
    bool m_emuRunning;
    int mVideoPlugin;
    QString mRom;
    QSettings *m_settings;
    QMutex *m_animationLock;
    int mAudio;
    Container *mCheatsContainer;
    ImageLoader* m_boxart;
    bb::cascades::TabbedPane *m_tab;
	bool m_boxartLoaded;
    bool m_isbright;
    int m_menuOffset;
    ThemeColor m_color;
    QString m_coverImage;
    bb::device::DisplayInfo *m_hdmiInfo;
    bb::cascades::QMapListDataModel* m_devices;
    bb::cascades::QMapListDataModel* m_history;
    QPropertyAnimation* m_menuAnimation;
};

#endif // ifndef STARSHIPSETTINGSAPP_H
