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
#include "settings/M64PSettings.hpp"
#include "BPS/BpsEventHandler.hpp"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include <bps/navigator.h>

#include <bb/system/InvokeManager>

#include <QFile>

using ::bb::cascades::Application;
using namespace bb::system;

static BpsEventHandler *s_handler;
bool debug_mode = false;
bool first_on_new_settings = false;
ActiveFrame* sceneCover;
QSettings *m_settings;

void myMessageOutput(QtMsgType type, const char* msg)
{
    Q_UNUSED(type);
    fprintf(stderr, "qml> %s\n", msg);
}

Q_DECL_EXPORT int main(int argc, char **argv)
{
    printf("Starting up\n");fflush(stdout);
    m_settings = new QSettings("emulators", "mupen64p");
    M64PSettings settings;
    first_on_new_settings = settings.Settings()->FirstRun();
    if (first_on_new_settings)
    {
        settings.Settings()->SecondRun();
        settings.Settings()->Theme(m_settings->value("THEME", -1).toInt());
        settings.Settings()->PrimaryColourIndex(m_settings->value("PRIMARY_COLOUR_INDEX", 1).toInt());
        settings.Settings()->PrimaryColourRed(m_settings->value("PRIMARY_COLOUR_R", 255).toInt());
        settings.Settings()->PrimaryColourGreen(m_settings->value("PRIMARY_COLOUR_G", 0).toInt());
        settings.Settings()->PrimaryColourBlue(m_settings->value("PRIMARY_COLOUR_B", 0).toInt());
        settings.Settings()->BaseColourIndex(m_settings->value("BASE_COLOUR_INDEX", 0).toInt());
        settings.Settings()->BaseColourRed(m_settings->value("BASE_COLOUR_R", 0).toInt());
        settings.Settings()->BaseColourGreen(m_settings->value("BASE_COLOUR_G", 0).toInt());
        settings.Settings()->BaseColourBlue(m_settings->value("BASE_COLOUR_B", 0).toInt());
    }
    QString themeString;
    int id = settings.Settings()->Theme();
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
    if (settings.Settings()->PrimaryColourIndex())
    {
        themeString += "?primaryColor=0x";
        themeString += ("00" + QString::number(settings.Settings()->PrimaryColourRed() & 0xFF, 16).toUpper()).right(2);
        themeString += ("00" + QString::number(settings.Settings()->PrimaryColourGreen() & 0xFF, 16).toUpper()).right(2);
        themeString += ("00" + QString::number(settings.Settings()->PrimaryColourBlue() & 0xFF, 16).toUpper()).right(2);
        if (settings.Settings()->BaseColourIndex())
        {
            themeString += "&amp;primaryBase=0x";
            themeString += ("00" + QString::number(settings.Settings()->BaseColourRed() & 0xFF, 16).toUpper()).right(2);
            themeString += ("00" + QString::number(settings.Settings()->BaseColourGreen() & 0xFF, 16).toUpper()).right(2);
            themeString += ("00" + QString::number(settings.Settings()->BaseColourBlue() & 0xFF, 16).toUpper()).right(2);
        }
    }
    setenv("CASCADES_THEME", themeString.toAscii().data(), 1);
	int rc;
    // Call the main application constructor.
    Application app(argc, argv);

    if (first_on_new_settings || !QFile::exists("shared/misc/n64/data/gln64.conf"))
    {
        const char *env = getenv("WIDTH");
        int width = atoi(env);
        env = getenv("HEIGHT");
        int height = atoi(env);
        if (width == height)
        {
            if (width == 720)
                QFile::copy("app/native/data/gln64.conf.Q10", "shared/misc/n64/data/gln64.conf");
            else if (width == 1440)
                QFile::copy("app/native/data/gln64.conf.psp", "shared/misc/n64/data/gln64.conf");
        }
        else
        {
            if (width == 768)
                QFile::copy("app/native/data/gln64.conf.Z10", "shared/misc/n64/data/gln64.conf");
            else
                QFile::copy("app/native/data/gln64.conf.Z30", "shared/misc/n64/data/gln64.conf");
        }
    }

    // localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("Mupen64Plus_cascades_ui_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    FILE* fl = 0;
    if (debug_mode)
    {
    	fl = freopen("/accounts/1000/shared/misc/n64/debug.txt", "w", stderr);
    	dup2(fileno(stderr), fileno(stdout));
    	setvbuf(stdout, NULL, _IONBF, 0);
        qInstallMsgHandler(myMessageOutput);
    }

	mkdir("data/screens/", S_IRWXU | S_IRWXG);

    InvokeManager* manager = new InvokeManager();
    s_handler = new BpsEventHandler();

    // Create the application.
    Frontend *mainApp = new Frontend;
    QObject::connect(s_handler, SIGNAL(PlayPressed()), mainApp, SLOT(pressFastforward()));
    QObject::connect(s_handler, SIGNAL(PlayReleased()), mainApp, SLOT(releaseFastforward()));

    sceneCover = new ActiveFrame(mainApp);
    app.setCover(sceneCover);

    //app.setAutoExit(false);

    QObject::connect(&app, SIGNAL( aboutToQuit() ), mainApp, SLOT( onManualExit() ));
    QObject::connect(&app, SIGNAL( thumbnail() ), mainApp, SLOT( onThumbnail() ));
	QObject::connect(manager, SIGNAL(invoked(const bb::system::InvokeRequest&)), mainApp, SLOT(onInvoke(const bb::system::InvokeRequest&)));

    // We complete the transaction started in the App constructor and start the client event loop here.
    // When loop is exited the Application deletes the scene which deletes all its children (per QT rules for children).
    rc = Application::exec();

    printf("After main exec...\n");fflush(stdout);

    if (debug_mode && fl)
    	fclose(stderr);

    delete mainApp;
    delete s_handler;

    return rc;
}

