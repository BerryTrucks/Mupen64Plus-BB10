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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <bb/system/InvokeManager>

using ::bb::cascades::Application;
using namespace bb::system;

Q_DECL_EXPORT int main(int argc, char **argv)
{
	struct stat buf;
	int theme = 0;
	if (!stat("data/bright.dat", &buf))
	{
		setenv("CASCADES_THEME", "bright", 1);
		theme = 2;
	}
	else if (!stat("data/dark.dat", &buf))
	{
		setenv("CASCADES_THEME", "dark", 1);
		theme = 1;
	}
	int rc;
    // Call the main application constructor.
    Application app(argc, argv);

    InvokeManager* manager = new InvokeManager();

    // Create the application.
    Frontend mainApp(theme);

    //app.setAutoExit(false);

    QObject::connect(&app, SIGNAL( aboutToQuit() ), &mainApp, SLOT( onManualExit() ));
	QObject::connect(manager, SIGNAL(invoked(const bb::system::InvokeRequest&)), &mainApp, SLOT(onInvoke(const bb::system::InvokeRequest&)));

    // We complete the transaction started in the App constructor and start the client event loop here.
    // When loop is exited the Application deletes the scene which deletes all its children (per QT rules for children).
    rc = Application::exec();

    printf("After main exec...\n");fflush(stdout);

    //delete mainApp;

    return rc;
}

