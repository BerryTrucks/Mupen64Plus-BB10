/*
 * GameImage.hpp
 *
 *  Created on: Sep 20, 2014
 *      Author: Travis
 */

#ifndef GAMEIMAGE_HPP_
#define GAMEIMAGE_HPP_

#include <QObject>
#include <QString>

#include <QNetworkAccessManager>

#include <bb/cascades/Image>

class GameImage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int ImageType READ ImageType CONSTANT)
    Q_PROPERTY(int Width READ Width CONSTANT)
    Q_PROPERTY(int Height READ Height CONSTANT)
    Q_PROPERTY(QString Location READ Location CONSTANT)
    Q_PROPERTY(QString ThumbLocation READ ThumbLocation CONSTANT)
    Q_PROPERTY(int Side READ Side CONSTANT)
    Q_PROPERTY(bb::cascades::Image Picture READ Picture NOTIFY PictureChanged)
    Q_CLASSINFO("DefaultProperty", "Picture");

public:
    enum Type {
        FANART,
        BOXART,
        BANNER,
        SCREENSHOT,
        CLEARLOGO
    };

    enum BoxSide {
        FRONT,
        BACK
    };

signals:
    void PictureChanged();

public slots:
    void ImageDownloaded();
    void ImageDownloadSave();

public:
    inline int ImageType() const { return m_type; }
    inline int Width() const { return m_width; }
    inline int Height() const { return m_height; }
    inline QString Location() const { return m_location; }
    inline QString ThumbLocation() const { return m_thumbLocation; }
    inline int Side() const { return m_side; }
    inline bb::cascades::Image Picture() const { return m_picture; }

    inline void ImageType(Type tp) { m_type = tp; }
    inline void Width(int val) { m_width = val; }
    inline void Height(int val) { m_height = val; }
    inline void Location(const QString& loc) { m_location = loc; }
    inline void ThumbLocation(const QString& loc) { m_thumbLocation = loc; }
    inline void Side(BoxSide val) { m_side = val; }

public:
    GameImage();
    GameImage(const GameImage& image);
    GameImage& operator=(const GameImage& image);
    virtual ~GameImage();

    void DownloadImage(const QString& baseLocation);
    void DownloadThumbnail(const QString& baseLocation);
    void SaveImage(const QString& location);

private:
    Type m_type;
    int m_width;
    int m_height;
    QString m_location;
    QString m_thumbLocation;
    BoxSide m_side;
    bb::cascades::Image m_picture;
    QNetworkAccessManager m_manager;
    QString m_savelocation;
};

Q_DECLARE_METATYPE(GameImage);
Q_DECLARE_METATYPE(GameImage*);

#endif /* GAMEIMAGE_HPP_ */
