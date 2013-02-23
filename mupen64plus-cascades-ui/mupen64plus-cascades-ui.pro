TEMPLATE = app
TARGET = mupen64plus-cascades-ui

CONFIG += qt warn_on debug_and_release cascades

INCLUDEPATH += ../src ../../mupen64plus-core-richard42/api ../../bbutil/
SOURCES += ../src/*.cpp
SOURCES += ../src/*.c
HEADERS += ../src/*.h
HEADERS += ../src/*.hpp 

LIBS += -L../lib -lscreen -lGLESv2 -lbbutil -lSDL12 -ltco -lpng -lz -lbps

LIBS += -lbbcascadespickers -lbbsystem -lbbdata
INCLUDEPATH += ${QNX_TARGET}/usr/include/bb/cascades/pickers

device {
	CONFIG(release, debug|release) {		
		DESTDIR = o.le-v7		
		TEMPLATE=lib
		QMAKE_CXXFLAGS += -fvisibility=hidden
	} 
	CONFIG(debug, debug|release) {
		DESTDIR = o.le-v7-g
	}
}

simulator {
	CONFIG(release, debug|release) {
		DESTDIR = o
	} 
	CONFIG(debug, debug|release) {
		DESTDIR = o-g
	}
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

