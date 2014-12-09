/*
 * GeneralSettings.hpp
 *
 *  Created on: Jul 1, 2014
 *      Author: Travis
 */

#ifndef GENERALSETTINGS_HPP_
#define GENERALSETTINGS_HPP_

#include "SettingsBase.hpp"

#include <QObject>
#include <QSettings>


class GeneralSettings : public SettingsBase
{
    Q_OBJECT

    Q_PROPERTY(bool DefaultToSDCard READ DefaultToSDCard WRITE DefaultToSDCard NOTIFY DefaultToSDCardChanged)
    Q_PROPERTY(bool PerROMSettings READ PerROMSettings WRITE PerROMSettings NOTIFY PerROMSettingsChanged)
    Q_PROPERTY(bool BoxartScraping READ BoxartScraping WRITE BoxartScraping NOTIFY BoxartScrapingChanged)
    Q_PROPERTY(bool SaveHistory READ SaveHistory WRITE SaveHistory NOTIFY SaveHistoryChanged)
    Q_PROPERTY(bool ShowFPS READ ShowFPS WRITE ShowFPS NOTIFY ShowFPSChanged)
    Q_PROPERTY(int Theme READ Theme WRITE Theme NOTIFY ThemeChanged)
    Q_PROPERTY(int PrimaryColourIndex READ PrimaryColourIndex WRITE PrimaryColourIndex NOTIFY PrimaryColourIndexChanged)
    Q_PROPERTY(int PrimaryColourRed READ PrimaryColourRed WRITE PrimaryColourRed NOTIFY PrimaryColourRedChanged)
    Q_PROPERTY(int PrimaryColourGreen READ PrimaryColourGreen WRITE PrimaryColourGreen NOTIFY PrimaryColourGreenChanged)
    Q_PROPERTY(int PrimaryColourBlue READ PrimaryColourBlue WRITE PrimaryColourBlue NOTIFY PrimaryColourBlueChanged)
    Q_PROPERTY(int BaseColourIndex READ BaseColourIndex WRITE BaseColourIndex NOTIFY BaseColourIndexChanged)
    Q_PROPERTY(int BaseColourRed READ BaseColourRed WRITE BaseColourRed NOTIFY BaseColourRedChanged)
    Q_PROPERTY(int BaseColourGreen READ BaseColourGreen WRITE BaseColourGreen NOTIFY BaseColourGreenChanged)
    Q_PROPERTY(int BaseColourBlue READ BaseColourBlue WRITE BaseColourBlue NOTIFY BaseColourBlueChanged)
    Q_PROPERTY(bool CheckVersion READ CheckVersion WRITE CheckVersion NOTIFY CheckVersionChanged)
    Q_PROPERTY(bool UseGridInHistory READ UseGridInHistory WRITE UseGridInHistory NOTIFY UseGridInHistoryChanged)

signals:
    void DefaultToSDCardChanged();
    void PerROMSettingsChanged();
    void BoxartScrapingChanged();
    void SaveHistoryChanged();
    void ShowFPSChanged();
    void ThemeChanged();
    void PrimaryColourIndexChanged();
    void PrimaryColourRedChanged();
    void PrimaryColourGreenChanged();
    void PrimaryColourBlueChanged();
    void BaseColourIndexChanged();
    void BaseColourRedChanged();
    void BaseColourGreenChanged();
    void BaseColourBlueChanged();
    void CheckVersionChanged();
    void UseGridInHistoryChanged();

public:
    bool DefaultToSDCard() { return getBaseBool("GENERAL_SDCARD", false); }
    void DefaultToSDCard(bool val ) { setBaseBool("GENREAL_SDCARD", val); }

    bool PerROMSettings() { return getBaseBool("GENERAL_PERROM", true); }
    void PerROMSettings(bool val) { setBaseBool("GENERAL_PERROM", val); }

    bool BoxartScraping() { return getBaseBool("GENERAL_BOXART", true); }
    void BoxartScraping(bool val) { setBaseBool("GENERAL_BOXART", val); }

    bool SaveHistory() { return getBaseBool("GENERAL_HISTORY", true); }
    void SaveHistory(bool val) { setBaseBool("GENERAL_HISTORY", val); }

    bool ShowFPS() { return getBaseBool("GENERAL_FPS", false); }
    void ShowFPS(bool val) { setBaseBool("GENERAL_FPS", val); }

    int Theme() { return getBaseInt("GENERAL_THEME", -1); }
    void Theme(int val) { setBaseInt("GENERAL_THEME", val); }

    int PrimaryColourIndex() { return getBaseInt("GENERAL_PRIMARY_INDEX", 1); }
    void PrimaryColourIndex(int val) { setBaseInt("GENERAL_PRIMARY_INDEX", val); }

    int PrimaryColourRed() { return getBaseInt("GENERAL_PRIMARY_RED", 255); }
    void PrimaryColourRed(int val) { setBaseInt("GENERAL_PRIMARY_RED", val); }

    int PrimaryColourGreen() { return getBaseInt("GENERAL_PRIMARY_GREEN", 0); }
    void PrimaryColourGreen(int val) { setBaseInt("GENERAL_PRIMARY_GREEN", val); }

    int PrimaryColourBlue() { return getBaseInt("GENERAL_PRIMARY_BLUE", 0); }
    void PrimaryColourBlue(int val) { setBaseInt("GENERAL_PRIMARY_BLUE", val); }

    int BaseColourIndex() { return getBaseInt("GENERAL_BASE_INDEX", 0); }
    void BaseColourIndex(int val) { setBaseInt("GENERAL_BASE_INDEX", val); }

    int BaseColourRed() { return getBaseInt("GENERAL_BASE_RED", 0); }
    void BaseColourRed(int val) { return setBaseInt("GENERAL_BASE_RED", val); }

    int BaseColourGreen() { return getBaseInt("GENERAL_BASE_GREEN", 0); }
    void BaseColourGreen(int val) { setBaseInt("GENERAL_BASE_GREEN", val); }

    int BaseColourBlue() { return getBaseInt("GENERAL_BASE_BLUE", 0); }
    void BaseColourBlue(int val) { setBaseInt("GENERAL_BASE_BLUE", val); }

    bool FirstRun() { return getBool("GENERAL_FIRST_RUN", true); }
    void SecondRun() { setBool("GENERAL_FIRST_RUN", false); }

    bool CheckVersion() { return getBool("GENERAL_CHECK_VERSION", true); }
    void CheckVersion(bool val) { setBool("GENERAL_CHECK_VERSION", val); }

    bool UseGridInHistory() { return getBool("GENERAL_USE_GRID_HISTORY", false); }
    void UseGridInHistory(bool val) { setBool("GENERAL_USE_GRID_HISTORY", val); }

public:
    GeneralSettings() { }
    GeneralSettings(const GeneralSettings& toCopy) : SettingsBase() { Q_UNUSED(toCopy); }

    GeneralSettings operator=(const GeneralSettings& toCopy) { Q_UNUSED(toCopy); return *this; }

    Q_INVOKABLE virtual void reset();
    virtual void writeSettings(Emulator *m64p) { Q_UNUSED(m64p); }

protected:
    virtual void GameNameChanged() { }
};

#endif /* GENERALSETTINGS_HPP_ */
