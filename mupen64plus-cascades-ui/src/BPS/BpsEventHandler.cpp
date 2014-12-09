/*
 * BpsEventHandler.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: Travis
 */

#include "BpsEventHandler.hpp"

#include <stdio.h>
#include <bps/virtualkeyboard.h>

#include <QTimer>

using namespace bb;


BpsEventHandler::BpsEventHandler()
    : AbstractBpsEventHandler()
{
    subscribe(button_get_domain());
    subscribe(virtualkeyboard_get_domain());
    bps_initialize();
    button_request_events(0);
    virtualkeyboard_request_events(0);
    m_playPressed = false;
    m_upPressed = false;
    m_downPressed = false;
    m_emuRunning = false;
}

BpsEventHandler::~BpsEventHandler()
{
    unsubscribe(button_get_domain());
    unsubscribe(virtualkeyboard_get_domain());
}

void BpsEventHandler::startEmulating()
{
    virtualkeyboard_hide();
    m_emuRunning = true;
}

void BpsEventHandler::timeUp()
{
    if (m_playPressed)
        emit PlayPressed();
    else if (m_upPressed)
        emit UpPressed();
    else
        emit DownPressed();
}

void BpsEventHandler::event(bps_event_t* event)
{
    if (bps_event_get_domain(event) == button_get_domain())
    {
        int but = button_event_get_button(event);
        if (but == BUTTON_MINUS)
        {
            m_downPressed = !m_downPressed;
            if (m_upPressed)
            {
                emit CapturePressed();
            }
            else
            {
                if (!m_downPressed)
                    emit DownReleased();
                else
                    QTimer::singleShot(250, this, SLOT(timeUp()));
            }
        }
        else if (but == BUTTON_PLUS)
        {
            m_upPressed = !m_upPressed;
            if (m_downPressed)
            {
                emit CapturePressed();
            }
            else
            {
                if (!m_upPressed)
                    emit UpReleased();
                else
                    QTimer::singleShot(250, this, SLOT(timeUp()));
            }
        }
        else if (but == BUTTON_PLAYPAUSE)
        {
            m_playPressed = !m_playPressed;
            if (!m_playPressed)
                emit PlayReleased();
            else
                QTimer::singleShot(250, this, SLOT(timeUp()));
        }
    }
    else if (bps_event_get_domain(event) == virtualkeyboard_get_domain())
    {
        if (m_emuRunning)
            virtualkeyboard_hide();
    }
}


