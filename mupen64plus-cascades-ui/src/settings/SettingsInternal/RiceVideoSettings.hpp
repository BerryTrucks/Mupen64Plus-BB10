/*
 * RiceVideoSettings.hpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Travis
 */

#ifndef RICEVIDEOSETTINGS_HPP_
#define RICEVIDEOSETTINGS_HPP_

#include "VideoSettings.hpp"

#include <QObject>
#include <QSettings>


class RiceVideoSettings : public VideoSettings
{
    Q_OBJECT

    Q_PROPERTY(bool Frameskip READ Frameskip WRITE Frameskip NOTIFY FrameskipChanged)
    Q_PROPERTY(bool StretchVideo READ StretchVideo WRITE StretchVideo NOTIFY StretchVideoChanged)
    Q_PROPERTY(bool FastTextureCRC READ FastTextureCRC WRITE FastTextureCRC NOTIFY FastTextureCRCChanged)
    Q_PROPERTY(bool AccurateTextureMapping READ AccurateTextureMapping WRITE AccurateTextureMapping NOTIFY AccurateTextureMappingChanged)
    Q_PROPERTY(bool DoubleSmallTextureSize READ DoubleSmallTextureSize WRITE DoubleSmallTextureSize NOTIFY DoubleSmallTextureSizeChanged)
    Q_PROPERTY(int TextureEnhancement READ TextureEnhancement WRITE TextureEnhancement NOTIFY TextureEnhancementChanged)

signals:
    void FrameskipChanged();
    void StretchVideoChanged();
    void FastTextureCRCChanged();
    void AccurateTextureMappingChanged();
    void DoubleSmallTextureSizeChanged();
    void TextureEnhancementChanged();

public:
    enum TEXTURE_ENHANCEMENT
    {
        None = 0,
        DoubleSize = 1,
        TwoxSai = 2,
        HQ2x = 3,
        LQ2x = 4
    };

public:
    bool Frameskip() { return getBool("RICE_FRAMESKIP", false); }
    void Frameskip(bool val) { setBool("RICE_FRAMESKIP", val); }

    bool StretchVideo() { return getBool("RICE_STRETCH", true); }
    void StretchVideo(bool val) { setBool("RICE_STRETCH", val); }

    bool FastTextureCRC() { return getBool("RICE_FASTTEXTCRC", false); }
    void FastTextureCRC(bool val) { setBool("RICE_FASTTEXTCRC", val); }

    bool AccurateTextureMapping() { return getBool("RICE_ACCTEXTMAP", false); }
    void AccurateTextureMapping(bool val) { setBool("RICE_ACCTEXTMAP", val); }

    bool DoubleSmallTextureSize() { return getBool("RICE_DBLSMALLTEXTSIZE", false); }
    void DoubleSmallTextureSize(bool val) { setBool("RICE_DBLSMALLTEXTSIZE", val); }

    int TextureEnhancement() { return getInt("RICE_TEXTENH", 0); }
    void TextureEnhancement(int val) { setInt("RICE_TEXTENH", val); }

public:
    RiceVideoSettings() { }
    RiceVideoSettings(const RiceVideoSettings& toCopy) : VideoSettings(toCopy) { }

    RiceVideoSettings operator=(const RiceVideoSettings& toCopy) { VideoSettings::operator=(toCopy); return *this; }

public:
    Q_INVOKABLE virtual void reset();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};


#endif /* RICEVIDEOSETTINGS_HPP_ */
