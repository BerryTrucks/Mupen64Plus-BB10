/*
 * Game.cpp
 *
 *  Created on: 2014-04-10
 *      Author: Travis
 */

#include "Game.hpp"

#include <stdio.h>

#define SERIAL_VERSION_ID 2


Game::Game(const QString& title)
	: QObject()
{
	m_basename = QString(title);
	m_uid = QUuid::createUuid();
	m_name = "";
	m_pinned = false;
}

Game::Game(const Game &copy)
	: QObject()
{
	m_uid = QUuid(copy.m_uid);
	m_name = QString(copy.m_name);
	m_basename = QString(copy.m_basename);
	m_id = QString(copy.m_id);
	m_playDate = QDateTime(copy.m_playDate);
	m_resource = QString(copy.m_resource);
	m_location = QString(copy.m_location);
	m_pinned = copy.m_pinned;
}

Game& Game::operator=(const Game& copy)
{
	m_uid = QUuid(copy.m_uid);
	m_name = QString(copy.m_name);
	m_basename = QString(copy.m_basename);
	m_id = QString(copy.m_id);
	m_playDate = QDateTime(copy.m_playDate);
	m_resource = QString(copy.m_resource);
	m_location = QString(copy.m_location);
	m_pinned = copy.m_pinned;
	return *this;
}

bool Game::operator==(const Game& other)
{
	return this->m_uid == other.m_uid;
}

bool Game::operator==(const QUuid& uuid)
{
	return this->m_uid == uuid;
}

Game::~Game()
{
}

void Game::serialize(QDataStream& out) const
{
	out << SERIAL_VERSION_ID;
	out << m_name << m_id << m_playDate << m_uid << m_location << m_resource << m_basename << m_pinned;
}

void Game::deserialize(QDataStream& in)
{
	int version;
	in >> version;
	if (version > 1) {
	    in >> m_name >> m_id >> m_playDate >> m_uid >> m_location >> m_resource >> m_basename >> m_pinned;
	    if (m_basename.isEmpty())
	        m_basename = QString(m_name);
	    else if (m_name.isEmpty())
	        m_name = QString(m_basename);
	}
	else
	{
        in >> m_basename >> m_id >> m_playDate >> m_uid >> m_location >> m_resource >> m_basename >> m_pinned;
	    m_name = QString(m_basename);
	    m_pinned = false;
	}
	//printf("Game loaded %s (%s)\n", m_name.toAscii().constData(), m_basename.toAscii().constData());
}

