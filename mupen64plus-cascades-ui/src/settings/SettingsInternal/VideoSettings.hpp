/*
 * VideoSettings.hpp
 *
 *  Created on: Jun 12, 2014
 *      Author: Travis
 */

#ifndef VIDEOSETTINGS_HPP_
#define VIDEOSETTINGS_HPP_

#include "SettingsBase.hpp"

#include <stdio.h>

#include <QObject>
#include <QSettings>


class VideoSettings : public SettingsBase
{
    Q_OBJECT

    Q_PROPERTY(int VideoPlugin READ VideoPlugin WRITE VideoPlugin NOTIFY VideoPluginChanged)

signals:
    void VideoPluginChanged();

public:
    int VideoPlugin() { return getInt("VIDEO_PLUGIN", 0); }
    void VideoPlugin(int val) { setInt("VIDEO_PLUGIN", val); }

public:
    VideoSettings() { }
    VideoSettings(const VideoSettings& toCopy) : SettingsBase() { Q_UNUSED(toCopy); }

    VideoSettings operator=(const VideoSettings& toCopy) { Q_UNUSED(toCopy); return *this; }

    Q_INVOKABLE virtual void reset() { VideoPlugin(0); emit VideoPluginChanged(); }
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged() { emit VideoPluginChanged(); }
};


#endif /* VIDEOSETTINGS_HPP_ */
