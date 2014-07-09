/*
 * BpsEventHandler.hpp
 *
 *  Created on: Jul 4, 2014
 *      Author: Travis
 */

#ifndef BPSEVENTHANDLER_HPP_
#define BPSEVENTHANDLER_HPP_

#include <QObject>

#include <bb/AbstractBpsEventHandler>

#include <bps/bps.h>
#include <bps/button.h>

class BpsEventHandler : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT

public:
    BpsEventHandler();
    virtual ~BpsEventHandler();

signals:
    void PlayPressed();
    void PlayReleased();

public:
    virtual void event(bps_event_t* event);

private:
    bool m_pressed;
};


#endif /* BPSEVENTHANDLER_HPP_ */
