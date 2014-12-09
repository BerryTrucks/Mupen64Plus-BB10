/*
 * VideoSettings.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Travis
 */

#include "SettingsBase.hpp"
#include "VideoSettings.hpp"
#include "RiceVideoSettings.hpp"
#include "N64VideoSettings.hpp"
#include "GlideVideoSettings.hpp"

#include <QStringList>


QSettings SettingsBase::settings("shared/misc/n64/data/settings.ini", QSettings::IniFormat);

bool SettingsBase::GameHasSettings(const QString& gameName)
{
    int size = settings.beginReadArray("GamesWithSettings");
    bool val = false;
    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        if (QString::compare(gameName, settings.value("GameName").toString()) == 0)
        {
            val = true;
            break;
        }
    }
    settings.endArray();
    return val;
}

void SettingsBase::MakeGameHaveSettings(bool add)
{
    int size = settings.beginReadArray("GamesWithSettings");
    QStringList list;
    bool alreadyDone = false;
    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        QString value = settings.value("GameName").toString();
        bool equal = QString::compare(m_gameName, value) == 0;
        if (equal && add)
        {
            alreadyDone = true;
            break;
        }
        if (!add && equal)
            continue;
        list.append(value);
    }
    settings.endArray();
    if (alreadyDone)
        return;

    settings.beginGroup("GamesWithSettings");
    settings.remove("");
    settings.endGroup();

    list.append(m_gameName);
    settings.beginWriteArray("GamesWithSettings");
    for (int i = 0; i < list.length(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("GameName", list[i]);
    }
    settings.endArray();
}

void VideoSettings::writeSettings(Emulator *m64p)
{
    if (VideoPlugin() == 2)
        m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"mupen64plus-video-glide64mk2");
    else if (VideoPlugin() == 1)
        m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"gles2n64");
    else
        m64p->SetConfigParameter(std::string("UI-Console[VideoPlugin]=")+"libmupen64plus-video-rice");
    m64p->SetConfigParameter(std::string("Video-General[Fullscreen]=False"));
}

void RiceVideoSettings::GameNameChanged()
{
    VideoSettings::GameNameChanged();
    emit FrameskipChanged();
    emit StretchVideoChanged();
    emit FastTextureCRCChanged();
    emit AccurateTextureMappingChanged();
    emit DoubleSmallTextureSizeChanged();
    emit TextureEnhancementChanged();
}

void RiceVideoSettings::reset()
{
    Frameskip(false);
    emit FrameskipChanged();
    Fog(true);
    emit FogChanged();
    StretchVideo(true);
    emit StretchVideoChanged();
    FastTextureCRC(false);
    emit FastTextureCRCChanged();
    AccurateTextureMapping(false);
    emit AccurateTextureMappingChanged();
    DoubleSmallTextureSize(false);
    emit DoubleSmallTextureSizeChanged();
    TextureEnhancement(0);
    emit TextureEnhancementChanged();
}

void RiceVideoSettings::writeSettings(Emulator *m64p)
{
    VideoSettings::writeSettings(m64p);
    m64p->SetConfigParameter(std::string("Video-Rice[SkipFrame]=") + std::string(Frameskip() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Rice[FastTextureLoading]=") + std::string(FastTextureCRC() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Rice[AccurateTextureMapping]=") + std::string(AccurateTextureMapping() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Rice[DoubleSizeForSmallTxtrBuf]=") + std::string(DoubleSmallTextureSize() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Rice[TextureEnhancement]=") + QString::number(TextureEnhancement()).toStdString());
    m64p->SetConfigParameter(std::string("Video-Rice[FogMethod]=") + std::string(Fog() ? "1" : "0"));
}

void N64VideoSettings::GameNameChanged()
{
    VideoSettings::GameNameChanged();
    emit FrameskipChanged();
    emit FogChanged();
    emit SaiTextureFilterChanged();
    emit ForceScreenClearChanged();
    emit AlphaTestChanged();
    emit ZHackChanged();
}

void N64VideoSettings::reset()
{
    Frameskip(false);
    emit FrameskipChanged();
    MaxFrameskip(0);
    emit MaxFrameskipChanged();
    Fog(false);
    emit FogChanged();
    SaiTextureFilter(false);
    emit SaiTextureFilterChanged();
    ForceScreenClear(false);
    emit ForceScreenClearChanged();
    AlphaTest(true);
    emit AlphaTestChanged();
    ZHack(true);
    emit ZHackChanged();
}

void N64VideoSettings::writeSettings(Emulator *m64p)
{
    VideoSettings::writeSettings(m64p);
    m64p->SetConfigParameter(std::string("gles2n64[auto frameskip]=") + std::string(Frameskip() ? "True" : "False"));
    if (!Frameskip())
        m64p->SetConfigParameter(std::string("gles2n64[max frameskip]=") + QString::number(MaxFrameskip()).toStdString());
    m64p->SetConfigParameter(std::string("gles2n64[enable fog]=") + std::string(Fog() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("gles2n64[texture 2xSAI]=") + std::string(SaiTextureFilter() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("gles2n64[force screen clear]=") + std::string(ForceScreenClear() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("gles2n64[enable alpha test]=") + std::string(AlphaTest() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("gles2n64[hack z]=") + std::string(ZHack() ? "True" : "False"));
}

void GlideVideoSettings::GameNameChanged()
{
    VideoSettings::GameNameChanged();
    emit ReadEveryFrameChanged();
    emit DisplayStatsChanged();
    emit DisplayTimeChanged();
    emit DisplayTime24Changed();
    emit HiResModeChanged();
    emit AspectRatioChanged();
}

void GlideVideoSettings::reset()
{
    ReadEveryFrame(false);
    emit ReadEveryFrameChanged();
    DisplayStats(0);
    emit DisplayStatsChanged();
    DisplayTime(false);
    emit DisplayTimeChanged();
    DisplayTime24(false);
    emit DisplayTime24Changed();
    HiResMode(0);
    emit HiResModeChanged();
    AspectRatio(Stretch);
    emit AspectRatioChanged();
    AutoFrameskip(false);
    emit AutoFrameskipChanged();
    MaxFrameskip(0);
    emit MaxFrameskipChanged();
}

void GlideVideoSettings::writeSettings(Emulator *m64p)
{
    VideoSettings::writeSettings(m64p);
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[fb_read_always]=") + std::string(ReadEveryFrame() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[show_fps]=") + QString::number(DisplayStats()).toStdString());
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[clock]=") + std::string(DisplayTime() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[clock_24_hr]=") + std::string(DisplayTime24() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[fb_hires]=") + QString::number(HiResMode()).toStdString());
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[aspect]=") + QString::number(AspectRatio()).toStdString());
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[autoframeskip]=") + std::string(AutoFrameskip() ? "True" : "False"));
    m64p->SetConfigParameter(std::string("Video-Glide64mk2[maxframeskip]=") + QString::number(MaxFrameskip()).toStdString());
}
