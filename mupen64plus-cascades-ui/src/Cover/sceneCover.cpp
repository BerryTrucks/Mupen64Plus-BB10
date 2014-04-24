/*
 * sceneCover.cpp
 *
 *  Created on: 2013-07-05
 *      Author: Travis
 */

#include "sceneCover.hpp"

#include <bb/cascades/Application>

using namespace bb::cascades;

ActiveFrame::ActiveFrame(Frontend* ui)
	: SceneCover(bb::cascades::Application::instance())
{
	qml = QmlDocument::create("asset:///cover.qml")
		.parent(this);
	qml->setContextProperty("_frontend", ui);

	mMainContainer = qml->createRootObject<Container>();
	setContent(mMainContainer);
}

ActiveFrame::~ActiveFrame()
{
}




