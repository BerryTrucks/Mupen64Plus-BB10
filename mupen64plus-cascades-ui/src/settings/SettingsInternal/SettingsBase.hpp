/*
 * SettingsBase.hpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Travis
 */

#ifndef SETTINGSBASE_HPP_
#define SETTINGSBASE_HPP_

#include "../../emulator.h"

#include <QObject>
#include <QSettings>


class SettingsBase : public QObject
{
    Q_OBJECT

public:
    SettingsBase() { }

public:
    QString GameName() const { return m_gameName; }
    void GameName(const QString &name) { m_gameName = QString(name); MakeGameHaveSettings(); GameNameChanged(); }

    static bool GameHasSettings(const QString& gameName);

private:
    bool getBoolSimple(const QString &name, bool def) { return settings.value(name, def).toBool(); }
    void setBoolSimple(const QString &name, bool val) { settings.setValue(name, val); settings.sync(); }

    int getIntSimple(const QString &name, int def) { return settings.value(name, def).toInt(); }
    void setIntSimple(const QString &name, int val) { settings.setValue(name, val); settings.sync(); }

    QString getStringSimple(const QString &name, const QString &def) { return settings.value(name, def).toString(); }
    void setStringSimple(const QString &name, const QString &val) { settings.setValue(name, val); settings.sync(); }

    void clearValSimple(const QString &name) { settings.remove(name); }

    bool enterGroup() { if (m_gameName.isEmpty()) return false; settings.beginGroup(m_gameName); return true; }
    void leaveGroup() { if (!m_gameName.isEmpty()) settings.endGroup(); }

    void MakeGameHaveSettings(bool add = true);

protected:
    virtual void GameNameChanged() = 0;

    bool getBaseBool(const QString& name, bool def) { return getBoolSimple(name, def); }
    void setBaseBool(const QString& name, bool val) { setBoolSimple(name, val); }

    virtual bool getBool(const QString &name, bool def) { bool val = getBoolSimple(name, def); if (enterGroup()) { val = getBoolSimple(name, val); leaveGroup(); } return val; }
    virtual void setBool(const QString &name, bool val) { enterGroup(); setBoolSimple(name, val); leaveGroup(); }

    virtual QString getString(const QString &name, const QString &def) { QString val = getStringSimple(name, def); if (enterGroup()) { val = getStringSimple(name, val); leaveGroup(); } return val; }
    virtual void setString(const QString &name, const QString &val) { enterGroup(); setStringSimple(name, val); leaveGroup(); }

    int getBaseInt(const QString& name, int def) { return getIntSimple(name, def); }
    void setBaseInt(const QString& name, int val) { setIntSimple(name, val); }

    virtual int getInt(const QString &name, int def) { int val = getIntSimple(name, def); if (enterGroup()) { val = getIntSimple(name, val); leaveGroup(); } return val; }
    virtual void setInt(const QString &name, int val) { enterGroup(); setIntSimple(name, val); leaveGroup(); }

    virtual void clearVal(const QString &name) { if (enterGroup()) { clearValSimple(name); leaveGroup(); } }

public:
    Q_INVOKABLE virtual void reset() = 0;
    virtual void writeSettings(Emulator *m64p) = 0;

    static inline void sync() { settings.sync(); }

protected:
    QString m_gameName;

    static QSettings settings;
};

#endif /* SETTINGSBASE_HPP_ */
