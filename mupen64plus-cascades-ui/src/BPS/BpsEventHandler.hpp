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
    void UpPressed();
    void UpReleased();
    void DownPressed();
    void DownReleased();
    void CapturePressed();

protected slots:
    void timeUp();

public:
    virtual void event(bps_event_t* event);
    void startEmulating();

private:
    bool m_playPressed;
    bool m_upPressed;
    bool m_downPressed;
    bool m_emuRunning;
};


#endif /* BPSEVENTHANDLER_HPP_ */
