/*
 * GameImage.cpp
 *
 *  Created on: Sep 20, 2014
 *      Author: Travis
 */

#include "GameImage.hpp"

#include <QNetworkRequest>
#include <QNetworkReply>

#include <stdio.h>

using namespace bb::cascades;

GameImage::GameImage()
{
    m_type = FANART;
    m_width = 0;
    m_height = 0;
    m_location = "";
    m_thumbLocation = "";
    m_side = FRONT;
}

GameImage::GameImage(const GameImage& image)
    : QObject()
{
    this->m_type = image.m_type;
    this->m_width = image.m_width;
    this->m_height = image.m_height;
    this->m_location = QString(image.m_location);
    this->m_thumbLocation = QString(image.m_thumbLocation);
    this->m_side = image.m_side;
}

GameImage& GameImage::operator=(const GameImage& image)
{
    this->m_type = image.m_type;
    this->m_width = image.m_width;
    this->m_height = image.m_height;
    this->m_location = QString(image.m_location);
    this->m_thumbLocation = QString(image.m_thumbLocation);
    this->m_side = image.m_side;
    return *this;
}

GameImage::~GameImage()
{
}

void GameImage::DownloadImage(const QString& baseLocation)
{
    QString queryUri = baseLocation + m_location;
    fflush(stdout);

    QNetworkRequest request(queryUri);
    QNetworkReply* reply = m_manager.get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(ImageDownloaded()));
}

void GameImage::SaveImage(const QString& location)
{
    QFileInfo info(location);
    if (info.exists())
        return;
    m_savelocation = location;
    printf("Download from: %s\nAnd Save to: %s\n", m_location.toAscii().constData(), m_savelocation.toAscii().constData());fflush(stdout);
    QString queryUri = m_location;
    fflush(stdout);

    QNetworkRequest request(queryUri);
    QNetworkReply* reply = m_manager.get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(ImageDownloadSave()));
}

void GameImage::DownloadThumbnail(const QString& baseLocation)
{
    QString queryUri = baseLocation + m_thumbLocation;
    fflush(stdout);

    QNetworkRequest request(queryUri);
    QNetworkReply* reply = m_manager.get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(ImageDownloaded()));
}

void GameImage::ImageDownloaded()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QImage img;
    img.loadFromData(reply->readAll());
    QByteArray arr;
    QBuffer buf(&arr);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "PNG");
    m_picture = Image(arr);
    emit PictureChanged();
    reply->deleteLater();
}

void GameImage::ImageDownloadSave()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QImage img;
    printf("Save image %s with %lld bytes of data\n", m_savelocation.toAscii().constData(), reply->size());fflush(stdout);
    img.loadFromData(reply->readAll());
    img.save(m_savelocation, "PNG");
    reply->deleteLater();
}

