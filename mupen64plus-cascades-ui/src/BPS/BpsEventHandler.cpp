/*
 * BpsEventHandler.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: Travis
 */

#include "BpsEventHandler.hpp"

#include <stdio.h>

using namespace bb;


BpsEventHandler::BpsEventHandler()
    : AbstractBpsEventHandler()
{
    subscribe(button_get_domain());
    bps_initialize();
    button_request_events(0);
    m_pressed = false;
}

BpsEventHandler::~BpsEventHandler()
{
    unsubscribe(button_get_domain());
}

void BpsEventHandler::event(bps_event_t* event)
{
    if (bps_event_get_domain(event) == button_get_domain())
    {
        int but = button_event_get_button(event);
        if (but == BUTTON_PLAYPAUSE)
        {
            m_pressed = !m_pressed;
            if (m_pressed)
                emit PlayPressed();
            else
                emit PlayReleased();
        }
    }
}


