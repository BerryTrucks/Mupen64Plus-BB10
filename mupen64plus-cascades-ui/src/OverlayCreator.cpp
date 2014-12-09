/*
 * OverlayCreator.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Travis
 */

#include "frontend.h"

#include <QPainter>
#include <QImage>
#include <QSettings>
#include <QTimer>

extern QSettings *m_settings;

static QString dir;


void Frontend::placeStart(int x, int y)
{
    m_start.setX(x);
    m_start.setY(y);
}

void Frontend::placeLeft(int x, int y)
{
    m_left.setX(x);
    m_left.setY(y);
}

void Frontend::placeRight(int x, int y)
{
    m_right.setX(x);
    m_right.setY(y);
}

void Frontend::placeAnalog(int x, int y)
{
    m_analog.setX(x);
    m_analog.setY(y);
}

void Frontend::placeDPad(int x, int y)
{
    m_dpad.setX(x);
    m_dpad.setY(y);
}

void Frontend::placeCPad(int x, int y)
{
    m_cpad.setX(x);
    m_cpad.setY(y);
}

void Frontend::placeZ(int x, int y)
{
    m_ztrigg.setX(x);
    m_ztrigg.setY(y);
}

void Frontend::placeA(int x, int y)
{
    m_a.setX(x);
    m_a.setY(y);
}

void Frontend::placeB(int x, int y)
{
    m_b.setX(x);
    m_b.setY(y);
}

void Frontend::createLayout()
{
    QFile::remove(m_imageName);
    dir = "shared/misc/n64/data/";
    int var = qrand() % 1000;
    m_imageName = dir + "display" + QString::number(var) + ".png";
    m_settings->setValue("IMAGE_RESOURCE_NAME", m_imageName);

    QTimer::singleShot(0, this, SLOT(createVisual()));
}

void Frontend::createVisual()
{
    QString filename = dir + "custom.png";
    QImage visual(1440, 720, QImage::Format_ARGB32);
    visual.fill(Qt::transparent);
    QPainter painter(&visual);
    QImage l("app/native/assets/images/overlay/lgood.png", "PNG");
    QImage r("app/native/assets/images/overlay/rgood.png", "PNG");
    QImage z("app/native/assets/images/overlay/z.png", "PNG");
    QImage a("app/native/assets/images/overlay/a.png", "PNG");
    QImage b("app/native/assets/images/overlay/b.png", "PNG");
    QImage analog("app/native/assets/images/overlay/analog.png", "PNG");
    QImage dpad("app/native/assets/images/overlay/dpad.png", "PNG");
    QImage start("app/native/assets/images/overlay/start.png", "PNG");
    QImage cpad("app/native/assets/images/overlay/cpad.png", "PNG");
    painter.drawImage(m_left, l);
    painter.drawImage(m_right, r);
    painter.drawImage(m_ztrigg, z);
    painter.drawImage(m_a, a);
    painter.drawImage(m_b, b);
    painter.drawImage(m_analog, analog);
    painter.drawImage(m_dpad, dpad);
    painter.drawImage(m_start, start);
    painter.drawImage(m_cpad, cpad);
    visual.save(filename, "PNG");
    visual.save(m_imageName, "PNG");
    QTimer::singleShot(0, this, SLOT(create1024()));
}

#define SCALE_WIDTH(val) ((int)((((double)(val))*1024.0) / 1280.0))
#define SCALE_HEIGHT(val) ((int)((((double)(val))*512.0) / 720.0))

void Frontend::create1024()
{
    QString filename = dir + "custom1024Key.png";
    QImage visual(1024, 512, QImage::Format_ARGB32);
    visual.fill(Qt::transparent);
    QPainter painter(&visual);
    QImage l("app/native/assets/images/overlay/creator/l.png", "PNG");
    QImage r("app/native/assets/images/overlay/creator/r.png", "PNG");
    QImage z("app/native/assets/images/overlay/creator/z.png", "PNG");
    QImage a("app/native/assets/images/overlay/creator/a.png", "PNG");
    QImage b("app/native/assets/images/overlay/creator/b.png", "PNG");
    QImage analog("app/native/assets/images/overlay/analog.png", "PNG");
    QImage dpad("app/native/assets/images/overlay/creator/dpad.png", "PNG");
    QImage start("app/native/assets/images/overlay/creator/start.png", "PNG");
    QImage cpad("app/native/assets/images/overlay/creator/cpad.png", "PNG");
    QPoint loc;
    loc.setX(SCALE_WIDTH(m_left.x()));
    loc.setY(SCALE_HEIGHT(m_left.y()));
    painter.drawImage(loc, l);
    loc.setX(SCALE_WIDTH(m_right.x()));
    loc.setY(SCALE_HEIGHT(m_right.y()));
    painter.drawImage(loc, r);
    loc.setX(SCALE_WIDTH(m_ztrigg.x()));
    loc.setY(SCALE_HEIGHT(m_ztrigg.y()));
    painter.drawImage(loc, z);
    loc.setX(SCALE_WIDTH(m_a.x()));
    loc.setY(SCALE_HEIGHT(m_a.y()));
    painter.drawImage(loc, a);
    loc.setX(SCALE_WIDTH(m_b.x()));
    loc.setY(SCALE_HEIGHT(m_b.y()));
    painter.drawImage(loc, b);
    loc.setX(SCALE_WIDTH(m_dpad.x()));
    loc.setY(SCALE_HEIGHT(m_dpad.y()));
    painter.drawImage(loc, dpad);
    loc.setX(SCALE_WIDTH(m_start.x()));
    loc.setY(SCALE_HEIGHT(m_start.y()));
    painter.drawImage(loc, start);
    loc.setX(SCALE_WIDTH(m_cpad.x()));
    loc.setY(SCALE_HEIGHT(m_cpad.y()));
    painter.drawImage(loc, cpad);
    visual.setPixel(SCALE_WIDTH(m_analog.x() + (analog.width() / 2.0)), SCALE_HEIGHT(m_analog.y() + (analog.height() / 2.0)), qRgb(0, 255, 255));
    visual.setPixel(SCALE_WIDTH(m_analog.x() + (analog.width() / 2.0)), SCALE_HEIGHT(m_analog.y()), qRgb(167, 255, 252));
    visual.save(filename, "PNG");
    QTimer::singleShot(0, this, SLOT(create1280()));
}

void Frontend::create1280()
{
    QString filename = dir + "custom1280Key.png";
    QImage visual(1280, 720, QImage::Format_ARGB32);
    visual.fill(Qt::transparent);
    QPainter painter(&visual);
    QImage l("app/native/assets/images/overlay/creator/l.png", "PNG");
    QImage r("app/native/assets/images/overlay/creator/r.png", "PNG");
    QImage z("app/native/assets/images/overlay/creator/z.png", "PNG");
    QImage a("app/native/assets/images/overlay/creator/a.png", "PNG");
    QImage b("app/native/assets/images/overlay/creator/b.png", "PNG");
    QImage analog("app/native/assets/images/overlay/analog.png", "PNG");
    QImage dpad("app/native/assets/images/overlay/creator/dpad.png", "PNG");
    QImage start("app/native/assets/images/overlay/creator/start.png", "PNG");
    QImage cpad("app/native/assets/images/overlay/creator/cpad.png", "PNG");
    painter.drawImage(m_left, l);
    painter.drawImage(m_right, r);
    painter.drawImage(m_ztrigg, z);
    painter.drawImage(m_a, a);
    painter.drawImage(m_b, b);
    painter.drawImage(m_dpad, dpad);
    painter.drawImage(m_start, start);
    painter.drawImage(m_cpad, cpad);
    visual.setPixel(m_analog.x() + (analog.width() / 2.0), m_analog.y() + (analog.height() / 2.0), qRgb(0, 255, 255));
    visual.setPixel(m_analog.x() + (analog.width() / 2.0), m_analog.y(), qRgb(167, 255, 252));
    visual.save(filename, "PNG");

    emit overlayCreationComplete();
}


