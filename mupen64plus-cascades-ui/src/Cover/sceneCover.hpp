/*
 * scenceCover.hpp
 *
 *  Created on: 2013-07-05
 *      Author: Travis
 */

#ifndef SCENCECOVER_HPP_
#define SCENCECOVER_HPP_

#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Label>

#include <QObject>

#include "../frontend.h"

class ActiveFrame : public bb::cascades::SceneCover
{
	Q_OBJECT

public:
	ActiveFrame(Frontend* ui);
	virtual ~ActiveFrame();

private:
	bb::cascades::Container* mMainContainer;
	bb::cascades::QmlDocument* qml;
};


#endif /* SCENCECOVER_HPP_ */
