/*
 * GameInfo.hpp
 *
 *  Created on: Sep 20, 2014
 *      Author: Travis
 */

#ifndef GAMEINFO_HPP_
#define GAMEINFO_HPP_

#include "GameImage.hpp"

#include <QObject>
#include <QString>

#include <bb/cascades/QListDataModel>

class GameInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString BaseImageURL READ BaseImageURL NOTIFY BaseImageUrlChanged)
    Q_PROPERTY(int Id READ Id NOTIFY IdChanged)
    Q_PROPERTY(QString Title READ Title NOTIFY TitleChanged)
    Q_PROPERTY(int PlatformId READ PlatformId NOTIFY PlatformIdChanged)
    Q_PROPERTY(QString Platform READ Platform NOTIFY PlatformChanged)
    Q_PROPERTY(QString ReleaseDate READ ReleaseDate NOTIFY ReleaseDateChanged)
    Q_PROPERTY(QString Overview READ Overview NOTIFY OverviewChanged)
    Q_PROPERTY(QString ESRB READ ESRB NOTIFY ESRBChanged)
    Q_PROPERTY(QString Genres READ Genres NOTIFY GenresChanged)
    Q_PROPERTY(int Players READ Players NOTIFY PlayersChanged)
    Q_PROPERTY(bool CoOp READ CoOp NOTIFY CoopChanged)
    Q_PROPERTY(QString YouTube READ YouTube NOTIFY YoutubeChanged)
    Q_PROPERTY(QString Publisher READ Publisher NOTIFY PublisherChanged)
    Q_PROPERTY(QString Developer READ Developer NOTIFY DeveloperChanged)
    Q_PROPERTY(float Rating READ Rating NOTIFY RatingChanged)
    Q_PROPERTY(GameImage* ClearIcon READ ClearIcon NOTIFY ClearIconChanged)
    Q_PROPERTY(GameImage* BannerIcon READ BannerIcon NOTIFY BannerIconChanged)
    Q_PROPERTY(bb::cascades::DataModel* Screenshots READ Screenshots NOTIFY ScreenshotsChanged)
    Q_PROPERTY(int ScreenshotCount READ ScreenshotCount NOTIFY ScreenshotCountChanged)
    Q_PROPERTY(QString Boxart READ Boxart CONSTANT)

signals:
    void BaseImageUrlChanged();
    void IdChanged();
    void TitleChanged();
    void PlatformIdChanged();
    void PlatformChanged();
    void ReleaseDateChanged();
    void OverviewChanged();
    void ESRBChanged();
    void GenresChanged();
    void PlayersChanged();
    void CoopChanged();
    void YoutubeChanged();
    void PublisherChanged();
    void DeveloperChanged();
    void RatingChanged();
    void BannerIconChanged();
    void ClearIconChanged();
    void ScreenshotsChanged();
    void ScreenshotCountChanged();

public slots:
    void ScreenshotLoaded();

public:
    inline QString BaseImageURL() const { return m_baseImageUrl; }
    inline int Id() const { return m_id; }
    inline QString Title() const { return m_title; }
    inline int PlatformId() const { return m_platformId; }
    inline QString Platform() const { return m_platform; }
    inline QString ReleaseDate() const { return m_releaseDate; }
    inline QString Overview() const { return m_overview; }
    inline QString ESRB() const { return m_esrb; }
    inline QString Genres() const { return m_genres; }
    inline int Players() const { return m_players; }
    inline bool CoOp() const { return m_coop; }
    inline QString YouTube() const { return m_youtube; }
    inline QString Publisher() const { return m_publisher; }
    inline QString Developer() const { return m_developer; }
    inline float Rating() const { return m_rating; }
    inline GameImage* ClearIcon() { foreach (GameImage* img, m_images) { if (img->ImageType() == GameImage::CLEARLOGO) return img; } return &m_empty; }
    inline GameImage* BannerIcon() { foreach (GameImage* img, m_images) { if (img->ImageType() == GameImage::BANNER) return img; } return &m_empty; }
    inline bb::cascades::DataModel* Screenshots() const { return m_screenshots; }
    inline int ScreenshotCount() const { return m_screenshots->size(); }

    inline void BaseImageURL(const QString& url) { m_baseImageUrl = QString(url); }
    inline void Id(int id) { m_id = id; }
    inline void Title(const QString& title) { m_title = QString(title); }
    inline void PlatformId(int id) { m_platformId = id; }
    inline void Platform(const QString& platform) { m_platform = QString(platform); }
    inline void ReleaseDate(const QString& date) { m_releaseDate = QString(date); }
    inline void Overview(const QString& overview) { m_overview = QString(overview); }
    inline void ESRB(const QString& esrb) { m_esrb = QString(esrb); }
    inline void Genres(const QString& genres) { m_genres = QString(genres); }
    inline void Player(int players) { m_players = players; }
    inline void CoOp(bool coop) { m_coop = coop; }
    inline void YouTube(const QString& youtube) { m_youtube = QString(youtube); }
    inline void Publisher(const QString& publisher) { m_publisher = QString(publisher); }
    inline void Developer(const QString& developer) { m_developer = QString(developer); }
    inline void Rating(float rating) { m_rating = rating; }

    inline void AddImage(GameImage* image) { m_images.append(image); }
    inline void ClearImages() { m_images.clear(); m_screenshots->clear(); }

    QString Boxart();

public:
    GameInfo();
    GameInfo& operator=(const GameInfo& toCopy);
    GameInfo(const GameInfo& toCopy);
    virtual ~GameInfo();

    void parse(const QString& xml);
    void refresh();

private:
    QString m_baseImageUrl;
    int m_id;
    QString m_title;
    int m_platformId;
    QString m_platform;
    QString m_releaseDate;
    QString m_overview;
    QString m_esrb;
    QString m_genres;
    int m_players;
    bool m_coop;
    QString m_youtube;
    QString m_publisher;
    QString m_developer;
    float m_rating;
    QList<GameImage*> m_images;
    GameImage m_empty;
    bb::cascades::QMapListDataModel* m_screenshots;
};

Q_DECLARE_METATYPE(GameInfo)

#endif /* GAMEINFO_HPP_ */
