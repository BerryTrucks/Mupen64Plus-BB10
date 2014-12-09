APP_NAME = Mupen64Plus_cascades_ui

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../../../BB101Emulators/Mupen64Plus-BB10-master/mupen64plus-core-richard42/src/api
INCLUDEPATH += ../../bbutil/src ${QNX_TARGET}/usr/include/qt4/QtNetwork
INCLUDEPATH += ${QNX_TARGET}/usr/include/qt4/QtGui ../../Unzip/src

LIBS += -L../lib1.7 -lscreen -lGLESv2 -lbbutil -lSDL12 -ltco -lpng -lz -lc -lbps
LIBS += -lbbcascadespickers -lbbsystem -lbbdata -lbb -lbbdevice -lUnzip -lbbplatform

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
