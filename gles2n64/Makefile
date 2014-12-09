#/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# *   Mupen64plus - Makefile                                                *
# *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
# *   Copyright (C) 2007-2008 DarkJeztr Tillin9 Richard42                   *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# Makefile for glN64 in Mupen64plus.

ARCH = ARM
OS = LINUX
#ARCH = X86
#OS = WIN32

CFLAGS  = -Wall -Os
LDFLAGS =

SO_EXTENSION = so
#CC_OVERRIDE = C:/bbndk/host_10_0_9_52/win32/x86/usr/bin/qcc -V4.6.3,gcc_ntoarmv7le_cpp -w1 -shared
#CXX = C:/bbndk-2.1.0-beta1/host/win32/x86/usr/bin/qcc -V4.4.2,gcc_ntoarmv7le_cpp -w1 -shared
CXX := J:/bbndk10.2-Gold/host_10_1_0_238/win32/x86/usr/bin/qcc -V4.6.3,gcc_ntoarmv7le_cpp

CXX += -w1 -shared
INCLUDE = ..
CFLAGS  += -I$(INCLUDE)/blackberry-SDL/include
CFLAGS  += -I../mupen64plus-core-richard42/api -I../bbutil -I../mupen64plus-video-glide64mk2-take2/src
CFLAGS  += -march=armv7-a -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ffast-math #\
                  #-fsingle-precision-constant  -ftree-vectorize -fexpensive-optimizations -fomit-frame-pointer

#CFLAGS  += -findirect-inlining
#CFLAGS  += -ftree-switch-conversion

CFLAGS  += -DARM_ASM -D__NEON_OPT -DUSE_SDL -D__CRC_OPT -D__HASHMAP_OPT -D__TRIBUFFER_OPT -D__VEC4_OPT
#CFLAGS += -D__PACKVERTEX_OPT

CFLAGS += -Wall -fPIC
LDFLAGS += -lGLESv2 -L../blackberry-SDL/Device-Debug -lSDL12 -L../mupen64plus-cascades-ui/lib -lbbutil


#CFLAGS += -DPROFILE_GBI
#CFLAGS += -DSHADER_TEST

OBJECTS =

# list of object files to generate
OBJECTS = 2xSAI.o \
        3DMath.o \
        Config.o \
        CRC.o \
        DepthBuffer.o \
        F3D.o \
        F3DCBFD.o \
        F3DDKR.o \
        F3DEX.o \
        F3DEX2.o \
        F3DPD.o \
        F3DWRUS.o \
        FrameSkipper.o \
        GBI.o \
        gDP.o \
	    gles2n64.o \
        gSP.o \
        L3D.o \
        L3DEX.o \
        L3DEX2.o \
        N64.o \
        OpenGL.o \
        RDP.o \
        RSP.o \
        S2DEX.o \
        S2DEX2.o \
        ShaderCombiner.o \
        Textures.o \
        ticks.o \
        VI.o \
        gSPNeon.o \
        3DMathNeon.o

# build targets
all: ../mupen64plus-cascades-ui/lib/gles2n64.$(SO_EXTENSION)

clean:
	rm -f *.o *.s *.ii *.$(SO_EXTENSION) ui_gln64config.*

# build rules
.cpp.o:
	$(CXX) -o $@ $(CFLAGS) -c $<

.c.o:
	$(CXX) -o $@ $(CFLAGS) -c $<

ui_gln64config.h: gln64config.ui
	$(UIC) $< -o $@

../mupen64plus-cascades-ui/lib/gles2n64.$(SO_EXTENSION): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) $(SDL_LIBS) $(LIBGL_LIBS) -o $@

gles2n64.o: gles2N64.cpp
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -DMAINDEF -c -o $@ $<

Config_qt4.o: Config_qt4.cpp ui_gln64config.h
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -c -o $@ $<

Config_gtk2.o: Config_gtk2.cpp
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -c -o $@ $<

GBI.o: GBI.cpp
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -c -o $@ $<
