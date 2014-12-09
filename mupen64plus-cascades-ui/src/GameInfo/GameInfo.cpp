/*
 * GameInfo.cpp
 *
 *  Created on: Sep 20, 2014
 *      Author: Travis
 */

#include "GameInfo.hpp"

#include <bb/data/XmlDataAccess>

using namespace bb::cascades;
using namespace bb::data;

GameInfo::GameInfo()
{
    m_baseImageUrl = "";
    m_id = 0;
    m_title = "";
    m_platformId = 0;
    m_platform = "";
    m_releaseDate = "";
    m_overview = "";
    m_esrb = "";
    m_genres = "";
    m_players = 1;
    m_coop = false;
    m_youtube = "";
    m_publisher = "";
    m_developer = "";
    m_rating = 0;
    m_screenshots = new QMapListDataModel;
}

GameInfo::GameInfo(const GameInfo& toCopy)
    : QObject()
{
    m_baseImageUrl = toCopy.m_baseImageUrl;
    m_id = toCopy.m_id;
    m_title = QString(toCopy.m_title);
    m_platformId = toCopy.m_platformId;
    m_platform = QString(toCopy.m_platform);
    m_releaseDate = QString(toCopy.m_releaseDate);
    m_overview = QString(toCopy.m_overview);
    m_esrb = QString(toCopy.m_esrb);
    m_genres = QString(toCopy.m_genres);
    m_players = toCopy.m_players;
    m_coop = toCopy.m_coop;
    m_youtube = QString(toCopy.m_youtube);
    m_publisher = QString(toCopy.m_publisher);
    m_developer = QString(toCopy.m_developer);
    m_rating = toCopy.m_rating;
    m_screenshots = new QMapListDataModel;
    foreach (GameImage* img, toCopy.m_images)
    {
        GameImage* newImage = new GameImage(*img);
        m_images.append(newImage);
        if (newImage->ImageType() == GameImage::SCREENSHOT)
        {
            QVariantMap map;
            map["image"] = QVariant::fromValue(newImage);
            (*m_screenshots) << map;
        }
    }
}

GameInfo& GameInfo::operator=(const GameInfo& toCopy)
{
    m_baseImageUrl = toCopy.m_baseImageUrl;
    m_id = toCopy.m_id;
    m_title = QString(toCopy.m_title);
    m_platformId = toCopy.m_platformId;
    m_platform = QString(toCopy.m_platform);
    m_releaseDate = QString(toCopy.m_releaseDate);
    m_overview = QString(toCopy.m_overview);
    m_esrb = QString(toCopy.m_esrb);
    m_genres = QString(toCopy.m_genres);
    m_players = toCopy.m_players;
    m_coop = toCopy.m_coop;
    m_youtube = QString(toCopy.m_youtube);
    m_publisher = QString(toCopy.m_publisher);
    m_developer = QString(toCopy.m_developer);
    m_rating = toCopy.m_rating;
    foreach (GameImage* img, toCopy.m_images)
    {
        GameImage* newImage = new GameImage(*img);
        m_images.append(newImage);
        if (newImage->ImageType() == GameImage::SCREENSHOT)
        {
            QVariantMap map;
            map["image"] = QVariant::fromValue(newImage);
            (*m_screenshots) << map;
        }
    }
    return *this;
}

GameInfo::~GameInfo()
{
    int count = m_images.count();
    for (int i = 0; i < count; i++)
        delete m_images.takeAt(0);
}

void GameInfo::parse(const QString& xml)
{
    ClearImages();
    XmlDataAccess access;
    QVariantMap root = access.loadFromBuffer(xml).toMap();
    QVariantMap game;
    QVariantList lst = root["Game"].toList();
    if (lst.length() > 0)
    {
        game = lst.at(0).toMap();
    }
    else
        game = root["Game"].toMap();
    BaseImageURL(root["baseImgUrl"].toString());
    Id(game["id"].toInt());
    Title(game["GameTitle"].toString());
    PlatformId(game["PlatformId"].toInt());
    Platform(game["Platform"].toString());
    ReleaseDate(game["ReleaseDate"].toString());
    Overview(game["Overview"].toString());
    ESRB(game["ESRB"].toString());
    QVariantMap genres = game["Genres"].toMap();
    QVariant g = genres["genre"];
    QVariantList list = g.toList();
    if (list.length() > 0)
    {
        QString genreString = "";
        foreach (QVariant val, list)
        {
            genreString += val.toString() + ",";
        }
        if (genreString.endsWith(","))
            genreString = genreString.left(genreString.length() - 1);
        Genres(genreString);
    }
    else
    {
        Genres(g.toString());
    }
    Player(game["Players"].toString().left(1).toInt());
    if (QString::compare(game["Co-op"].toString(), "No") == 0)
        CoOp(false);
    else
        CoOp(true);
    YouTube(game["Youtube"].toString());
    Publisher(game["Publisher"].toString());
    Developer(game["Developer"].toString());
    Rating(game["Rating"].toFloat());

    QVariantMap images = game["Images"].toMap();
    QVariantList l = images["fanart"].toList();
    if (l.size() > 0)
    {
        foreach (QVariant fanart, l)
        {
            QVariantMap map = fanart.toMap();
            GameImage* img = new GameImage;
            img->ImageType(GameImage::FANART);
            QVariantMap original = map["original"].toMap();
            img->Width(original["width"].toInt());
            img->Height(original["height"].toInt());
            img->Location(BaseImageURL() + original[".data"].toString());
            img->ThumbLocation(BaseImageURL() + map["thumb"].toString());
            AddImage(img);
        }
    }
    else
    {
        QVariantMap fanart = images["fanart"].toMap();
        if (!fanart.isEmpty())
        {
            GameImage* img = new GameImage;
            img->ImageType(GameImage::FANART);
            QVariantMap original = fanart["original"].toMap();
            img->Width(original["width"].toInt());
            img->Height(original["height"].toInt());
            img->Location(BaseImageURL() + original[".data"].toString());
            img->ThumbLocation(BaseImageURL() + fanart["thumb"].toString());
            AddImage(img);
        }
    }
    l = images["boxart"].toList();
    if (l.size() > 0)
    {
        foreach (QVariant boxart, l)
        {
            QVariantMap map = boxart.toMap();
            GameImage* img = new GameImage;
            img->ImageType(GameImage::BOXART);
            QString side = map["side"].toString();
            if (QString::compare(side, "back") == 0)
                img->Side(GameImage::BACK);
            else
                img->Side(GameImage::FRONT);
            img->Width(map["width"].toInt());
            img->Height(map["height"].toInt());
            img->ThumbLocation(BaseImageURL() + map["thumb"].toString());
            img->Location(BaseImageURL() + map[".data"].toString());
            AddImage(img);
        }
    }
    else
    {
        QVariantMap boxart = images["boxart"].toMap();
        if (!boxart.isEmpty())
        {
            GameImage* img = new GameImage;
            img->ImageType(GameImage::BOXART);
            QString side = boxart["side"].toString();
            if (QString::compare(side, "back") == 0)
                img->Side(GameImage::BACK);
            else
                img->Side(GameImage::FRONT);
            img->Width(boxart["width"].toInt());
            img->Height(boxart["height"].toInt());
            img->ThumbLocation(BaseImageURL() + boxart["thumb"].toString());
            img->Location(BaseImageURL() + boxart[".data"].toString());
            AddImage(img);
        }
    }
    l = images["banner"].toList();
    if (l.size() > 0)
    {
        foreach (QVariant banner, l)
        {
            QVariantMap map = banner.toMap();
            GameImage* img = new GameImage;
            img->ImageType(GameImage::BANNER);
            img->Width(map["width"].toInt());
            img->Height(map["height"].toInt());
            img->Location(BaseImageURL() + map[".data"].toString());
            AddImage(img);
        }
    }
    else
    {
        QVariantMap banner = images["banner"].toMap();
        if (!banner.isEmpty())
        {
            GameImage* img = new GameImage;
            img->ImageType(GameImage::BANNER);
            img->Width(banner["width"].toInt());
            img->Height(banner["height"].toInt());
            img->Location(BaseImageURL() + banner[".data"].toString());
            AddImage(img);
        }
    }
    emit BannerIconChanged();
    BannerIcon()->DownloadImage(BaseImageURL());
    l = images["screenshot"].toList();
    if (l.size() > 0)
    {
        foreach (QVariant screen, l)
        {
            QVariantMap map = screen.toMap();
            GameImage* img = new GameImage;
            img->ImageType(GameImage::SCREENSHOT);
            QVariantMap original = map["original"].toMap();
            img->Width(original["width"].toInt());
            img->Height(original["height"].toInt());
            img->Location(BaseImageURL() + original[".data"].toString());
            img->ThumbLocation(BaseImageURL() + map["thumb"].toString());
            AddImage(img);
        }
    }
    else
    {
        QVariantMap screen = images["screenshot"].toMap();
        if (!screen.isEmpty())
        {
            GameImage* img = new GameImage;
            img->ImageType(GameImage::SCREENSHOT);
            QVariantMap original = screen["original"].toMap();
            img->Width(original["width"].toInt());
            img->Height(original["height"].toInt());
            img->Location(BaseImageURL() + original[".data"].toString());
            img->ThumbLocation(BaseImageURL() + screen["thumb"].toString());
            AddImage(img);
        }
    }
    emit ScreenshotsChanged();
    QVariantMap clearLogo = images["clearlogo"].toMap();
    GameImage* img = new GameImage;
    img->ImageType(GameImage::CLEARLOGO);
    img->Width(clearLogo["width"].toInt());
    img->Height(clearLogo["height"].toInt());
    img->Location(BaseImageURL() + clearLogo[".data"].toString());
    AddImage(img);
    emit ClearIconChanged();

    emit BaseImageUrlChanged();
    emit IdChanged();
    emit TitleChanged();
    emit PlatformIdChanged();
    emit PlatformChanged();
    emit ReleaseDateChanged();
    emit OverviewChanged();
    emit ESRBChanged();
    emit GenresChanged();
    emit PlayersChanged();
    emit CoopChanged();
    emit YoutubeChanged();
    emit PublisherChanged();
    emit DeveloperChanged();
    emit RatingChanged();
}

void GameInfo::ScreenshotLoaded()
{
    GameImage* image = qobject_cast<GameImage*>(sender());
    QVariantMap newVal;
    newVal["image"] = QVariant::fromValue(image->Picture());
    (*m_screenshots) << newVal;
    emit ScreenshotCountChanged();
}

QString GameInfo::Boxart()
{
    foreach (GameImage* img, m_images)
    {
        if (img->ImageType() == GameImage::BOXART && img->Side() == GameImage::FRONT)
            return img->Location();
    }
    return "asset:///images/mupen64plus.png";
}

