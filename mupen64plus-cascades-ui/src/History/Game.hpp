/*
 * Game.hpp
 *
 *  Created on: 2014-04-10
 *      Author: Travis
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include <QUuid>
#include <QList>
#include <QObject>
#include <QMetaType>
#include <QDateTime>

class Game : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString name READ name NOTIFY nameChanged)
	Q_PROPERTY(QString id READ id NOTIFY dummySignal)
	Q_PROPERTY(QString date READ date NOTIFY dateChanged)
	Q_PROPERTY(QString uuid READ uuid NOTIFY dummySignal)

signals:
	void nameChanged();

public:
	Game() { m_pinned = false; }
	Game(const QString& title);
	Game(const Game &copy);
	virtual ~Game();

public:
	Game& operator=(const Game& copy);
	bool operator==(const Game& other);
	bool operator!=(const Game& other) { return !(*this == other); }
	bool operator==(const QUuid& uuid);
	bool operator!=(const QUuid& uuid) { return !(*this == uuid); }

public:
	void serialize(QDataStream& out) const;
	void deserialize(QDataStream& in);

signals:
	void dummySignal();
	void dateChanged();

public:
	inline QString name() const { if (m_name.isNull() || m_name.isEmpty()) return m_basename; return m_name; }
	inline QString baseName() const { if (m_basename.isNull() || m_basename.isEmpty()) return m_name; return m_basename; }
	inline QString id() const { return m_id; }
	inline QString date() const { return m_playDate.toString("h:mmap d MMMM yyyy"); }
	inline QString uuid() const { return m_uid.toString(); }
	inline QString location() const { return m_location; }
	inline QString resource() const { return m_resource; }
	inline bool isPinned() const { return m_pinned; }
	inline void setPinned(bool val) { m_pinned = val; }
	inline void playNow() { m_playDate = QDateTime::currentDateTime(); }
	inline void location(const QString& loc) { m_location = QString(loc); }
	inline void resource(const QString& res) { m_resource = QString(res); }
	inline void rename(const QString& name) { m_name = QString(name); emit nameChanged(); }

private:
	QString m_name;
	QString m_basename;
	QString m_id;
	QString m_location;
	QString m_resource;
	QDateTime m_playDate;
	QUuid m_uid;
	bool m_pinned;
};
Q_DECLARE_METATYPE(Game)
Q_DECLARE_METATYPE(QList<Game>)

#endif /* GAME_HPP_ */
