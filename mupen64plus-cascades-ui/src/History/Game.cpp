/*
 * Game.cpp
 *
 *  Created on: 2014-04-10
 *      Author: Travis
 */

#include "Game.hpp"

#define SERIAL_VERSION_ID 1


Game::Game(const QString& title)
	: QObject()
{
	m_name = QString(title);
	m_uid = QUuid::createUuid();
}

Game::Game(const Game &copy)
	: QObject()
{
	m_uid = QUuid(copy.m_uid);
	m_name = QString(copy.m_name);
	m_id = QString(copy.m_id);
	m_playDate = QDateTime(copy.m_playDate);
	m_resource = QString(copy.m_resource);
	m_location = QString(copy.m_location);
}

Game& Game::operator=(const Game& copy)
{
	m_uid = QUuid(copy.m_uid);
	m_name = QString(copy.m_name);
	m_id = QString(copy.m_id);
	m_playDate = QDateTime(copy.m_playDate);
	m_resource = QString(copy.m_resource);
	m_location = QString(copy.m_location);
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
	out << m_name << m_id << m_playDate << m_uid << m_location << m_resource;
}

void Game::deserialize(QDataStream& in)
{
	int version;
	in >> version;
	in >> m_name >> m_id >> m_playDate >> m_uid >> m_location >> m_resource;
}

