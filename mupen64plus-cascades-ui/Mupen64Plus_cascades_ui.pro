APP_NAME = Mupen64Plus_cascades_ui

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../../mupen64plus-core-richard42/api ../../bbutil/src ${QNX_TARGET}/usr/include/qt4/QtNetwork ${QNX_TARGET}/usr/include/qt4/QtGui ../../Unzip/src

LIBS += -L../lib -lscreen -lGLESv2 -lbbutil -lSDL12 -ltco -lpng -lz -lc -lbps -lbbcascadespickers -lbbsystem -lbbdata -lbb -lbbdevice -lUnzip

DEFINES += HAVE_BZIP2

device {
	CONFIG(release, debug|release) {
		LIBS += -L../../bbutil/Device-Release -L../../Unzip/Device-Release
	}
	CONFIG(debug, debug|release) {
		LIBS += -L../../bbutil/Device-Debug -L../../Unzip/Device-Debug
	}
}

simulator {
	CONFIG(debug, debug|release) {
		LIBS += -L../../bbutil/Simulator-Debug -L../../Unzip/Simulator-Debug
	}
}

include(config.pri)
