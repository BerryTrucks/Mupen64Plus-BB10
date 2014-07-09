/*
 * N64VideoSettings.hpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Travis
 */

#ifndef N64VIDEOSETTINGS_HPP_
#define N64VIDEOSETTINGS_HPP_

#include "VideoSettings.hpp"

#include <QObject>
#include <QSettings>


class N64VideoSettings : public VideoSettings
{
    Q_OBJECT

    Q_PROPERTY(bool Frameskip READ Frameskip WRITE Frameskip NOTIFY FrameskipChanged)
    Q_PROPERTY(bool Fog READ Fog WRITE Fog NOTIFY FogChanged)
    Q_PROPERTY(bool SaiTextureFilter READ SaiTextureFilter WRITE SaiTextureFilter NOTIFY SaiTextureFilterChanged)
    Q_PROPERTY(bool ForceScreenClear READ ForceScreenClear WRITE ForceScreenClear NOTIFY ForceScreenClearChanged)
    Q_PROPERTY(bool AlphaTest READ AlphaTest WRITE AlphaTest NOTIFY AlphaTestChanged)
    Q_PROPERTY(bool ZHack READ ZHack WRITE ZHack NOTIFY ZHackChanged)

signals:
    void FrameskipChanged();
    void FogChanged();
    void SaiTextureFilterChanged();
    void ForceScreenClearChanged();
    void AlphaTestChanged();
    void ZHackChanged();

public:
    bool Frameskip() { return getBool("N64_FRAMESKIP", false); }
    void Frameskip(bool val) { setBool("N64_FRAMESKIP", val); }

    bool Fog() { return getBool("N64_FOG", false); }
    void Fog(bool val) { setBool("N64_FOG", val); }

    bool SaiTextureFilter() { return getBool("N64_SAITEXTFILT", false); }
    void SaiTextureFilter(bool val) { setBool("N64_SAITEXTFILT", val); }

    bool ForceScreenClear() { return getBool("N64_FORCECLEAR", false); }
    void ForceScreenClear(bool val) { setBool("N64_FORCECLEAR", val); }

    bool AlphaTest() { return getBool("N64_ALPHATEST", true); }
    void AlphaTest(bool val) { setBool("N64_ALPHATEST", val); }

    bool ZHack() { return getBool("N64_ZHACK", true); }
    void ZHack(bool val) { setBool("N64_ZHACK", val); }

public:
    N64VideoSettings() { }
    N64VideoSettings(const N64VideoSettings& toCopy) : VideoSettings(toCopy) { }

    N64VideoSettings operator=(const N64VideoSettings& toCopy) { VideoSettings::operator=(toCopy); return *this; }

public:
    Q_INVOKABLE virtual void reset();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};

#endif /* N64VIDEOSETTINGS_HPP_ */
