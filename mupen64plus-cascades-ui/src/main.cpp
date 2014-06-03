/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "frontend.h"
#include "Cover/sceneCover.hpp"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <bps/navigator.h>

#include <bb/system/InvokeManager>

using ::bb::cascades::Application;
using namespace bb::system;

bool debug_mode = false;
ActiveFrame* sceneCover;
QSettings *m_settings;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    m_settings = new QSettings("emulators", "mupen64p");
    QString themeString;
    int id = m_settings->value("THEME", -1).toInt();
    if (id == 1)
        themeString = "dark";
    else if (id == 0)
        themeString = "bright";
    else
    {
        struct stat buf;
        if (!stat("data/bright.dat", &buf))
        {
            m_settings->setValue("THEME", 0);
            remove("data/bright.dat");
            themeString = "bright";
        }
        else if (!stat("data/dark.dat", &buf))
        {
            m_settings->setValue("THEME", 1);
            remove("data/dark.dat");
            themeString = "dark";
        }
        else
            themeString = "default";
    }
    if (m_settings->value("PRIMARY_COLOUR_INDEX", 1).toInt())
    {
        themeString += "?primaryColor=0x";
        themeString += ("00" + QString::number(m_settings->value("PRIMARY_COLOUR_R", 255).toInt() & 0xFF, 16).toUpper()).right(2);
        themeString += ("00" + QString::number(m_settings->value("PRIMARY_COLOUR_G", 0).toInt() & 0xFF, 16).toUpper()).right(2);
        themeString += ("00" + QString::number(m_settings->value("PRIMARY_COLOUR_B", 0).toInt() & 0xFF, 16).toUpper()).right(2);
        if (m_settings->value("BASE_COLOUR_INDEX", 0).toInt())
        {
            themeString += "&amp;primaryBase=0x";
            themeString += ("00" + QString::number(m_settings->value("BASE_COLOUR_R", 0).toInt() & 0xFF, 16).toUpper()).right(2);
            themeString += ("00" + QString::number(m_settings->value("BASE_COLOUR_G", 0).toInt() & 0xFF, 16).toUpper()).right(2);
            themeString += ("00" + QString::number(m_settings->value("BASE_COLOUR_B", 255).toInt() & 0xFF, 16).toUpper()).right(2);
        }
    }
    setenv("CASCADES_THEME", themeString.toAscii().data(), 1);
	int rc;
    // Call the main application constructor.
    Application app(argc, argv);

    FILE* fl = 0;
    if (debug_mode)
    {
    	fl = freopen("/accounts/1000/shared/misc/n64/debug.txt", "w", stderr);
    	dup2(fileno(stderr), fileno(stdout));
    	setvbuf(stdout, NULL, _IONBF, 0);
    }

	mkdir("data/screens/", S_IRWXU | S_IRWXG);

    InvokeManager* manager = new InvokeManager();

    // Create the application.
    Frontend mainApp;
    sceneCover = new ActiveFrame(&mainApp);
    app.setCover(sceneCover);

    //app.setAutoExit(false);

    QObject::connect(&app, SIGNAL( aboutToQuit() ), &mainApp, SLOT( onManualExit() ));
    QObject::connect(&app, SIGNAL( thumbnail() ), &mainApp, SLOT( onThumbnail() ));
	QObject::connect(manager, SIGNAL(invoked(const bb::system::InvokeRequest&)), &mainApp, SLOT(onInvoke(const bb::system::InvokeRequest&)));

    // We complete the transaction started in the App constructor and start the client event loop here.
    // When loop is exited the Application deletes the scene which deletes all its children (per QT rules for children).
    rc = Application::exec();

    printf("After main exec...\n");fflush(stdout);

    if (debug_mode && fl)
    	fclose(stderr);

    //delete mainApp;

    return rc;
}

