/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - Config_nogui.cpp                                        *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2008 Tillin9                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "Config.h"
#include "gles2N64.h"
#include "RSP.h"
#include "Textures.h"
#include "OpenGL.h"

static m64p_handle l_ConfigGles2n64 = NULL;

#include "Config.h"
#include "Common.h"


Config config;

struct Option
{
    const char* name;
    int*  data;
    const int   initial;
};


#define CONFIG_VERSION 2

Option configOptions[] =
{
    {"#gles2n64 Graphics Plugin for N64", NULL, 0},
    {"#by Orkin / glN64 developers and Adventus.", NULL, 0},

    {"config version", &config.version, 2},
    {"", NULL, 0},

    {"#Screen Settings:", NULL, 0},
    {"screen width", &config.screen.width, 1280},
    {"screen height", &config.screen.height, 768},
    {"", NULL, 0},

    {"#Window Settings:", NULL, 0},
    {"window enable x11", &config.window.enableX11, 1},
    {"window fullscreen", &config.window.fullscreen, 1},
    {"window centre", &config.window.centre, 1},
    {"window xpos", &config.window.xpos, 0},
    {"window ypos", &config.window.ypos, 0},
    {"window width", &config.window.width, 1280},
    {"window height", &config.window.height, 768},
    {"", NULL, 0},

    {"#Framebuffer Settings:",NULL,0},
//    {"framebuffer enable", &config.framebuffer.enable, 0},
    {"framebuffer bilinear", &config.framebuffer.bilinear, 0},
    {"framebuffer width", &config.framebuffer.width, 1280},
    {"framebuffer height", &config.framebuffer.height, 768},
    {"", NULL, 0},

    {"#VI Settings:", NULL, 0},
    {"video force", &config.video.force, 0},
    {"video width", &config.video.width, 320},
    {"video height", &config.video.height, 240},
    {"", NULL, 0},

    {"#Render Settings:", NULL, 0},
    {"enable fog", &config.enableFog, 0},
    {"enable primitive z", &config.enablePrimZ, 1},
    {"enable lighting", &config.enableLighting, 1},
    {"enable alpha test", &config.enableAlphaTest, 1},
    {"enable clipping", &config.enableClipping, 0},
    {"enable face culling", &config.enableFaceCulling, 1},
    {"enable noise", &config.enableNoise, 0},
    {"", NULL, 0},

    {"#Texture Settings:", NULL, 0},
    {"texture 2xSAI", &config.texture.sai2x, 0},
    {"texture force bilinear", &config.texture.forceBilinear, 0},
    {"texture max anisotropy", &config.texture.maxAnisotropy, 0},
    {"texture use IA", &config.texture.useIA, 0},
    {"texture fast CRC", &config.texture.fastCRC, 1},
    {"texture pow2", &config.texture.pow2, 1},
    {"", NULL, 0},

    {"#Frame skip:", NULL, 0},
    {"auto frameskip", &config.autoFrameSkip, 0},
    {"target FPS", &config.targetFPS, 20},
    {"frame render rate", &config.frameRenderRate, 1},
    {"vertical sync", &config.verticalSync, 0},
    {"", NULL, 0},

    {"#Other Settings:", NULL, 0},
    {"update mode", &config.updateMode, SCREEN_UPDATE_AT_VI_UPDATE },
    {"ignore offscreen rendering", &config.ignoreOffscreenRendering, 0},
    {"force screen clear", &config.forceBufferClear, 1},
    {"flip vertical", &config.screen.flipVertical, 0},
    {"", NULL, 0},

    {"#Hack Settings:", NULL, 0},
    {"hack banjo tooie", &config.hackBanjoTooie, 0},
    {"hack zelda", &config.hackZelda, 0},
    {"hack alpha", &config.hackAlpha, 0},
    {"hack z", &config.zHack, 1},

};

const int configOptionsSize = sizeof(configOptions) / sizeof(Option);

void Config_WriteConfig(const char *filename)
{
    config.version = CONFIG_VERSION;
    FILE* f = fopen(filename, "w");
    if (!f)
    {
        LOG(LOG_ERROR, "Could Not Open %s for writing\n", filename);
    }

    for(int i=0; i<configOptionsSize; i++)
    {
        Option *o = &configOptions[i];
        fprintf(f, o->name);
        if (o->data) fprintf(f,"=%i", *(o->data));
        fprintf(f, "\n");
    }


    fclose(f);
}

void Config_SetDefault()
{
    for(int i=0; i < configOptionsSize; i++)
    {
        Option *o = &configOptions[i];
        if (o->data) *(o->data) = o->initial;
    }
}

void Config_SetOption(char* line, char* val)
{
    for(int i=0; i< configOptionsSize; i++)
    {
        Option *o = &configOptions[i];
        if (strcasecmp(line, o->name) == 0)
        {
            if (o->data)
            {
                int v = atoi(val);
                *(o->data) = v;
                LOG(LOG_VERBOSE, "Config Option: %s = %i\n", o->name, v);
            }
            break;
        }
    }
}

void Config_LoadRomConfig(unsigned char* header)
{
    char line[4096];

    // get the name of the ROM
    for (int i=0; i<20; i++) config.romName[i] = header[0x20+i];
    config.romName[20] = '\0';
    while (config.romName[strlen(config.romName)-1] == ' ')
    {
        config.romName[strlen(config.romName)-1] = '\0';
    }

    switch(header[0x3e])
    {
        // PAL codes
        case 0x44:
        case 0x46:
        case 0x49:
        case 0x50:
        case 0x53:
        case 0x55:
        case 0x58:
        case 0x59:
            config.romPAL = true;
            break;

        // NTSC codes
        case 0x37:
        case 0x41:
        case 0x45:
        case 0x4a:
            config.romPAL = false;
            break;

        // Fallback for unknown codes
        default:
            config.romPAL = false;
    }

    LOG(LOG_MINIMAL, "Rom is %s\n", config.romPAL ? "PAL" : "NTSC");

    const char *filename = "shared/misc/n64/data/gles2n64rom.conf";
    FILE *f = fopen(filename,"r");
    if (!f)
    {
        LOG(LOG_MINIMAL, "Could not find %s Rom settings file, using global.\n", filename);
        return;
    }
    else
    {
        LOG(LOG_MINIMAL, "[gles2N64]: Searching %s Database for \"%s\" ROM\n", filename, config.romName);
        bool isRom = false;
        while (!feof(f))
        {
            fgets(line, 4096, f);
            if (line[0] == '\n') continue;

            if (strncmp(line,"rom name=", 9) == 0)
            {
                char* v = strchr(line, '\n');
                if (v) *v='\0';
                isRom = (strcasecmp(config.romName, line+9) == 0);
            }
            else
            {
                if (isRom)
                {
                    char* val = strchr(line, '=');
                    if (!val) continue;
                    *val++ = '\0';
                    Config_SetOption(line,val);
                }
            }
        }
    }
}

void Config_LoadConfig()
{
    FILE *f;
    char line[4096];

    // default configuration
    Config_SetDefault();

    // read configuration
    const char *filename = "shared/misc/n64/data/gles2n64.conf";
    f = fopen(filename, "r");
    if (!f)
    {
        LOG(LOG_MINIMAL, "[gles2N64]: Couldn't open config file '%s' for reading: %s\n", filename, strerror( errno ) );
        LOG(LOG_MINIMAL, "[gles2N64]: Attempting to write new Config \n");
        Config_WriteConfig(filename);
    }
    else
    {
        LOG(LOG_MINIMAL, "[gles2n64]: Loading Config from %s \n", filename);

        while (!feof( f ))
        {
            char *val;
            fgets( line, 4096, f );

            if (line[0] == '#' || line[0] == '\n')
                continue;

            val = strchr( line, '=' );
            if (!val) continue;

            *val++ = '\0';

             Config_SetOption(line,val);
        }

        if (config.version < CONFIG_VERSION)
        {
            LOG(LOG_WARNING, "[gles2N64]: Wrong config version, rewriting config with defaults\n");
            Config_SetDefault();
            Config_WriteConfig(filename);
        }

        fclose(f);
    }
}

//Adding Config settings to Mupen Core like rice
void Config_LoadConfig_Core()
{
	config.version = ConfigGetParamInt(l_ConfigGles2n64, "config version");
	config.screen.width = ConfigGetParamInt(l_ConfigGles2n64, "screen width");
	config.screen.height = ConfigGetParamInt(l_ConfigGles2n64, "screen height");
	config.window.enableX11 = ConfigGetParamInt(l_ConfigGles2n64, "window enable x11");
	config.window.fullscreen = ConfigGetParamInt(l_ConfigGles2n64, "window fullscreen");
	config.window.centre = ConfigGetParamInt(l_ConfigGles2n64, "window centre");
	config.window.xpos = ConfigGetParamInt(l_ConfigGles2n64, "window xpos");
	config.window.ypos = ConfigGetParamInt(l_ConfigGles2n64, "window ypos");
	config.window.width = ConfigGetParamInt(l_ConfigGles2n64, "window width");
	config.window.height = ConfigGetParamInt(l_ConfigGles2n64, "window height");
	config.framebuffer.bilinear = ConfigGetParamInt(l_ConfigGles2n64, "framebuffer bilinear");
	config.framebuffer.width = ConfigGetParamInt(l_ConfigGles2n64, "framebuffer width");
	config.framebuffer.height = ConfigGetParamInt(l_ConfigGles2n64, "framebuffer height");
	config.video.force = ConfigGetParamInt(l_ConfigGles2n64, "video force");
	config.video.width = ConfigGetParamInt(l_ConfigGles2n64, "video width");
	config.video.height = ConfigGetParamInt(l_ConfigGles2n64, "video height");
	config.enableFog = ConfigGetParamInt(l_ConfigGles2n64, "enable fog");
	config.enablePrimZ = ConfigGetParamInt(l_ConfigGles2n64, "enable primitive z");
	config.enableLighting = ConfigGetParamInt(l_ConfigGles2n64, "enable lighting");
	config.enableAlphaTest = ConfigGetParamInt(l_ConfigGles2n64, "enable alpha test");
	config.enableClipping = ConfigGetParamInt(l_ConfigGles2n64, "enable clipping");
	config.enableFaceCulling = ConfigGetParamInt(l_ConfigGles2n64, "enable face culling");
	config.enableNoise = ConfigGetParamInt(l_ConfigGles2n64, "enable noise");
	config.texture.sai2x = ConfigGetParamInt(l_ConfigGles2n64, "texture 2xSAI");
	config.texture.forceBilinear = ConfigGetParamInt(l_ConfigGles2n64, "texture force bilinear");
	config.texture.maxAnisotropy = ConfigGetParamInt(l_ConfigGles2n64, "texture max anisotropy");
	config.texture.useIA = ConfigGetParamInt(l_ConfigGles2n64, "texture use IA");
	config.texture.fastCRC = ConfigGetParamInt(l_ConfigGles2n64, "texture fast CRC");
	config.texture.pow2 = ConfigGetParamInt(l_ConfigGles2n64, "texture pow2");
	config.autoFrameSkip = ConfigGetParamInt(l_ConfigGles2n64, "auto frameskip");
	config.targetFPS = ConfigGetParamInt(l_ConfigGles2n64, "target FPS");
	config.frameRenderRate = ConfigGetParamInt(l_ConfigGles2n64, "frame render rate");
	config.verticalSync = ConfigGetParamInt(l_ConfigGles2n64, "vertical sync");
	config.updateMode = ConfigGetParamInt(l_ConfigGles2n64, "update mode");
	config.ignoreOffscreenRendering = ConfigGetParamInt(l_ConfigGles2n64, "ignore offscreen rendering");
	config.forceBufferClear = ConfigGetParamInt(l_ConfigGles2n64, "force screen clear");
	config.screen.flipVertical = ConfigGetParamInt(l_ConfigGles2n64, "flip vertical");
	config.hackBanjoTooie = ConfigGetParamInt(l_ConfigGles2n64, "hack banjo tooie");
	config.hackZelda = ConfigGetParamInt(l_ConfigGles2n64, "hack zelda");
	config.hackAlpha = ConfigGetParamInt(l_ConfigGles2n64, "hack alpha");
	config.zHack = ConfigGetParamInt(l_ConfigGles2n64, "hack z");
}

int InitConfiguration(void)
{
	if (ConfigOpenSection("gles2n64", &l_ConfigGles2n64) != M64ERR_SUCCESS)
	{
		//DebugMessage(M64MSG_ERROR, "Unable to open Video-Rice configuration section");
		return FALSE;
	}

	//Set the defaults
	ConfigSetDefaultInt(l_ConfigGles2n64, "config version", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "screen width", 1280, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "screen height", 768, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window enable x11", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window fullscreen", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window centre", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window xpos", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window ypos", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window width", 1280, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "window height", 768, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "framebuffer bilinear", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "framebuffer width", 1280, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "framebuffer height", 768, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "video force", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "video width", 320, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "video height", 240, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable fog", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable primitive z", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable lighting", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable alpha test", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable clipping", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable face culling", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "enable noise", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture 2xSAI", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture force bilinear", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture max anisotropy", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture use IA", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture fast CRC", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "texture pow2", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "auto frameskip", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "target FPS", 20, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "frame render rate", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "vertical sync", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "update mode", SCREEN_UPDATE_AT_VI_UPDATE, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "ignore offscreen rendering", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "force screen clear", 1, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "flip vertical", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "hack banjo tooie", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "hack zelda", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "hack alpha", 0, "test");
	ConfigSetDefaultInt(l_ConfigGles2n64, "hack z", 1, "test");
}
