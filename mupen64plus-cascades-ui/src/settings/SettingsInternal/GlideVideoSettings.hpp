/*
 * GlideVideoSettings.hpp
 *
 *  Created on: Jun 26, 2014
 *      Author: Travis
 */

#ifndef GLIDEVIDEOSETTINGS_HPP_
#define GLIDEVIDEOSETTINGS_HPP_

#include "VideoSettings.hpp"

#include <QObject>
#include <QSettings>


class GlideVideoSettings : public VideoSettings
{
    Q_OBJECT

    Q_PROPERTY(bool ReadEveryFrame READ ReadEveryFrame WRITE ReadEveryFrame NOTIFY ReadEveryFrameChanged)
    Q_PROPERTY(int DisplayStats READ DisplayStats WRITE DisplayStats NOTIFY DisplayStatsChanged)
    Q_PROPERTY(bool DisplayTime READ DisplayTime WRITE DisplayTime NOTIFY DisplayTimeChanged)
    Q_PROPERTY(bool DisplayTime24 READ DisplayTime24 WRITE DisplayTime24 NOTIFY DisplayTime24Changed)
    Q_PROPERTY(int HiResMode READ HiResMode WRITE HiResMode NOTIFY HiResModeChanged)
    Q_PROPERTY(int AspectRatio READ AspectRatio WRITE AspectRatio NOTIFY AspectRatioChanged)

signals:
    void ReadEveryFrameChanged();
    void DisplayStatsChanged();
    void DisplayTimeChanged();
    void DisplayTime24Changed();
    void HiResModeChanged();
    void AspectRatioChanged();

public:
    bool ReadEveryFrame() { return getBool("GLIDE_READEVERYFRAME", false); }
    void ReadEveryFrame(bool val) { setBool("GLIDE_READEVERYFRAME", val); }

    int DisplayStats() { return getInt("GLIDE_STATS", 0); }
    void DisplayStats(int val) { setInt("GLIDE_STATS", val); }

    bool DisplayTime() { return getBool("GLIDE_TIME", false); }
    void DisplayTime(bool val) { setBool("GLIDE_TIME", val); }

    bool DisplayTime24() { return getBool("GLIDE_TIME24", false); }
    void DisplayTime24(bool val) { setBool("GLIDE_TIME24", val); }

    int HiResMode() { return getInt("GLIDE_HIRES", 0); }
    void HiResMode(int val) { setInt("GLIDE_HIRES", val); }

    int AspectRatio() { return getInt("GLIDE_ASPECT", 0); }
    void AspectRatio(int val) { setInt("GLIDE_ASPECT", val); }

public:
    GlideVideoSettings() { }
    GlideVideoSettings(const GlideVideoSettings& toCopy) : VideoSettings(toCopy) { }

    GlideVideoSettings operator=(const GlideVideoSettings& toCopy) { VideoSettings::operator=(toCopy); return *this; }

public:
    Q_INVOKABLE virtual void reset();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};


#endif /* GLIDEVIDEOSETTINGS_HPP_ */
