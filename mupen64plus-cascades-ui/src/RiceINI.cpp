/*
 * RiceINI.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: Travis
 */

#include "RiceINI.hpp"

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


RiceINI::RiceINI()
{
    m_currentIndex = 0;
    bIniIsChanged = false;
}

RiceINI::RiceINI(const RiceINI& toCopy)
    : QObject()
{
    m_entries = toCopy.m_entries;
    m_currentIndex = toCopy.m_currentIndex;
    bIniIsChanged = toCopy.bIniIsChanged;
}

RiceINI& RiceINI::operator=(const RiceINI& toCopy)
{
    m_entries = toCopy.m_entries;
    m_currentIndex = toCopy.m_currentIndex;
    bIniIsChanged = toCopy.bIniIsChanged;
    return *this;
}

char * left(const char * src, int nchars)
{
    static char dst[300];
    strncpy(dst, src, nchars);
    dst[nchars] = 0;
    return dst;
}

char * right(const char *src, int nchars)
{
    static char dst[300];
    int srclen = strlen(src);
    if (nchars >= srclen)
    {
        strcpy(dst, src);
    }
    else
    {
        strncpy(dst, src + srclen - nchars, nchars);
        dst[nchars] = 0;
    }
    return dst;
}

char* tidy(char* s)
{
    char * p = s + strlen(s);

    p--;
    while (p >= s && (*p == ' ' || *p == 0xa || *p == '\r' || *p == '\n') )
    {
        *p = 0;
        p--;
    }
    return s;
}

std::ifstream & getline(std::ifstream & is, char *str)
{
    char buf[100];

    is.getline(buf, 100);
    strcpy(str, buf);
    return is;
}

void RiceINI::setCRC(const QString& crc, const QString& name)
{
    for (qint32 i = 0; i < m_entries.size(); i++)
    {
        if (QString::compare(crc, m_entries[i].crccheck, Qt::CaseInsensitive) == 0)
        {
            m_currentIndex = i;
            emit CurrentFileChanged();
            return;
        }
    }

    // Add new entry!!!
    INIEntry newsection;

    newsection.crccheck = QString(crc);
    newsection.name = QString(name);
    newsection.bDisableTextureCRC = false;
    newsection.bDisableCulling = false;
    newsection.bIncTexRectEdge = false;
    newsection.bZHack = false;
    newsection.bTextureScaleHack = false;
    newsection.bFastLoadTile = false;
    newsection.bUseSmallerTexture = false;
    newsection.bPrimaryDepthHack = false;
    newsection.bTexture1Hack = false;
    newsection.bDisableObjBG = false;
    newsection.dwVIWidth = -1;
    newsection.dwVIHeight = -1;
    newsection.dwUseCIWidthAndRatio = INIEntry::NOT_USE_CI_WIDTH_AND_RATIO;
    newsection.dwFullTMEM = 0;
    newsection.bTxtSizeMethod2 = false;
    newsection.bEnableTxtLOD = false;

    newsection.bEmulateClear = false;
    newsection.bForceScreenClear = false;
    newsection.bDisableBlender = false;
    newsection.bForceDepthBuffer = false;
    newsection.dwFastTextureCRC = 0;
    newsection.dwAccurateTextureMapping = 0;
    newsection.dwNormalBlender = 0;
    newsection.dwNormalCombiner = 0;
    newsection.dwFrameBufferOption = 0;
    newsection.dwRenderToTextureOption = 0;
    newsection.dwScreenUpdateSetting = 0;

    m_entries.append(newsection);

    bIniIsChanged = true;               // Flag to indicate we should be updated
    m_currentIndex = m_entries.size() - 1;
    connect(&m_entries[m_currentIndex], SIGNAL(entryChanged()), SLOT(iniFileChanged()));
}

void RiceINI::ReadIniFile()
{
    m_entries.clear();

    std::ifstream inifile;
    char readinfo[100];
    const char *ini_filepath = (char*)"app/native/RiceVideoLinux.ini";

    inifile.open(ini_filepath);

    if (inifile.fail())
        return;

    while (getline(inifile, readinfo))
    {
        tidy(readinfo);

        if (readinfo[0] == '/')
            continue;

        if (!strcasecmp(readinfo,"")==0)
        {
            if (readinfo[0] == '{') //if a section heading
            {
                INIEntry newsection;

                readinfo[strlen(readinfo) - 1] = '\0';
                newsection.crccheck = QString::fromUtf8(readinfo + 1);

                newsection.bDisableTextureCRC = false;
                newsection.bDisableCulling = false;
                newsection.bIncTexRectEdge = false;
                newsection.bZHack = false;
                newsection.bTextureScaleHack = false;
                newsection.bFastLoadTile = false;
                newsection.bUseSmallerTexture = false;
                newsection.bPrimaryDepthHack = false;
                newsection.bTexture1Hack = false;
                newsection.bDisableObjBG = false;
                newsection.dwVIWidth = -1;
                newsection.dwVIHeight = -1;
                newsection.dwUseCIWidthAndRatio = INIEntry::NOT_USE_CI_WIDTH_AND_RATIO;
                newsection.dwFullTMEM = 0;
                newsection.bTxtSizeMethod2 = false;
                newsection.bEnableTxtLOD = false;

                newsection.bEmulateClear = false;
                newsection.bForceScreenClear = false;
                newsection.bDisableBlender = false;
                newsection.bForceDepthBuffer = false;
                newsection.dwFastTextureCRC = 0;
                newsection.dwAccurateTextureMapping = 0;
                newsection.dwNormalBlender = 0;
                newsection.dwNormalCombiner = 0;
                newsection.dwFrameBufferOption = 0;
                newsection.dwRenderToTextureOption = 0;
                newsection.dwScreenUpdateSetting = 0;

                m_entries.append(newsection);
                connect(&m_entries[m_entries.size() - 1], SIGNAL(entryChanged()), SLOT(iniFileChanged()));
            }
            else
            {
                int sectionno = m_entries.size() - 1;

                if (strcasecmp(left(readinfo, 4), "Name") == 0)
                    m_entries[sectionno].name = QString::fromUtf8(right(readinfo, strlen(readinfo) - 5));

                if (strcasecmp(left(readinfo, 17), "DisableTextureCRC") == 0)
                    m_entries[sectionno].bDisableTextureCRC = true;

                if (strcasecmp(left(readinfo, 14), "DisableCulling") == 0)
                    m_entries[sectionno].bDisableCulling = true;

                if (strcasecmp(left(readinfo, 16), "PrimaryDepthHack") == 0)
                    m_entries[sectionno].bPrimaryDepthHack = true;

                if (strcasecmp(left(readinfo, 12), "Texture1Hack") == 0)
                    m_entries[sectionno].bTexture1Hack = true;

                if (strcasecmp(left(readinfo, 12), "FastLoadTile") == 0)
                    m_entries[sectionno].bFastLoadTile = true;

                if (strcasecmp(left(readinfo, 17), "UseSmallerTexture") == 0)
                    m_entries[sectionno].bUseSmallerTexture = true;

                if (strcasecmp(left(readinfo, 14), "IncTexRectEdge") == 0)
                    m_entries[sectionno].bIncTexRectEdge = true;

                if (strcasecmp(left(readinfo, 5), "ZHack") == 0)
                    m_entries[sectionno].bZHack = true;

                if (strcasecmp(left(readinfo, 16), "TexRectScaleHack") == 0)
                    m_entries[sectionno].bTextureScaleHack = true;

                if (strcasecmp(left(readinfo, 7), "VIWidth") == 0)
                    m_entries[sectionno].dwVIWidth = std::strtol(right(readinfo, 3), NULL, 10);

                if (strcasecmp(left(readinfo, 8), "VIHeight") == 0)
                    m_entries[sectionno].dwVIHeight = std::strtol(right(readinfo, 3), NULL, 10);

                if (strcasecmp(left(readinfo, 18), "UseCIWidthAndRatio") == 0)
                    m_entries[sectionno].dwUseCIWidthAndRatio = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 8), "FullTMEM") == 0)
                    m_entries[sectionno].dwFullTMEM = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 24), "AlternativeTxtSizeMethod") == 0)
                    m_entries[sectionno].bTxtSizeMethod2 = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 12), "EnableTxtLOD") == 0)
                    m_entries[sectionno].bEnableTxtLOD = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 12), "DisableObjBG") == 0)
                    m_entries[sectionno].bDisableObjBG = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 16), "ForceScreenClear") == 0)
                    m_entries[sectionno].bForceScreenClear = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 22), "AccurateTextureMapping") == 0)
                    m_entries[sectionno].dwAccurateTextureMapping = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 14), "FastTextureCRC") == 0)
                    m_entries[sectionno].dwFastTextureCRC = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 12), "EmulateClear") == 0)
                    m_entries[sectionno].bEmulateClear = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 18), "NormalAlphaBlender") == 0)
                    m_entries[sectionno].dwNormalBlender = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 19), "DisableAlphaBlender") == 0)
                    m_entries[sectionno].bDisableBlender = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 19), "NormalColorCombiner") == 0)
                    m_entries[sectionno].dwNormalCombiner = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 16), "ForceDepthBuffer") == 0)
                    m_entries[sectionno].bForceDepthBuffer = std::strtol(right(readinfo, 1), NULL, 10);

                if (strcasecmp(left(readinfo, 20), "FrameBufferEmulation") == 0)
                    m_entries[sectionno].dwFrameBufferOption = std::strtol(readinfo + 21, NULL, 10);

                if (strcasecmp(left(readinfo, 15), "RenderToTexture") == 0)
                    m_entries[sectionno].dwRenderToTextureOption = std::strtol(right(readinfo,1), NULL, 10);

                if (strcasecmp(left(readinfo, 19), "ScreenUpdateSetting") == 0)
                    m_entries[sectionno].dwScreenUpdateSetting = std::strtol(right(readinfo,1), NULL, 10);
            }
        }
    }
    inifile.close();
}

void RiceINI::WriteIniFile()
{
    if (!bIniIsChanged)
        return;
    qint32 i;
    FILE * fhIn;
    FILE * fhOut;

    /* get path to game-hack INI file and read it */
    const char *ini_filepath = (char*)"app/native/RiceVideoLinux.ini";
    fhIn = fopen(ini_filepath, "r");
    if (fhIn == NULL)
        return;
    fseek(fhIn, 0L, SEEK_END);
    long filelen = ftell(fhIn);
    fseek(fhIn, 0L, SEEK_SET);
    char *chIniData = (char *) malloc(filelen + 1);
    if (chIniData == NULL)
    {
        fclose(fhIn);
        return;
    }
    long bytesread = fread(chIniData, 1, filelen, fhIn);
    fclose(fhIn);
    if (bytesread != filelen)
    {
        free(chIniData);
        return;
    }
    chIniData[filelen] = 0;

    /* now try to open the file for writing */
    fhOut = fopen(ini_filepath, "w");
    if (fhOut == NULL)
    {
        free(chIniData);
        return;
    }

    // Mark all sections and needing to be written
    for (i = 0; i < m_entries.size(); i++)
    {
        m_entries[i].bOutput = false;
    }

    char *thisline = chIniData;
    while ((thisline - chIniData) < filelen)
    {
        char *nextline = strchr(thisline, '\n');
        if (nextline == NULL)
            nextline = thisline + strlen(thisline) + 1;
        else
            nextline++;
        if (thisline[0] == '{')
        {
            bool bFound = false;
            // Start of section
            tidy((char*) thisline);
            thisline[strlen(thisline) - 1] = '\0';
            for (i = 0; i < m_entries.size(); i++)
            {
                if (m_entries[i].bOutput)
                    continue;
                if (strcasecmp((char*)thisline + 1, m_entries[i].crccheck.toUtf8().constData()) == 0)
                {
                    // Output this CRC
                    OutputSectionDetails(i, fhOut);
                    m_entries[i].bOutput = true;
                    bFound = TRUE;
                    break;
                }
            }
            if (!bFound)
            {
                // Do what? This should never happen, unless the user
                // replaces the inifile while game is running!
            }
        }
        else if (thisline[0] == '/')
        {
            // Comment
            fputs((char*) thisline, fhOut);
        }
        thisline = nextline;
    }

    // Input buffer done-  process any new entries!
    for (i = 0; i < m_entries.size(); i++)
    {
        // Skip any that have not been done.
        if (m_entries[i].bOutput)
            continue;
        // Output this CRC
        OutputSectionDetails(i, fhOut);
        m_entries[i].bOutput = true;
    }

    fclose(fhOut);
    free(chIniData);

    bIniIsChanged = false;
}

void RiceINI::OutputSectionDetails(qint32 i, FILE* fh)
{
    fprintf(fh, "{%s}\n", m_entries[i].crccheck.toUtf8().constData());

    fprintf(fh, "Name=%s\n", m_entries[i].name.toUtf8().constData());

    // Tri-state variables
    if (m_entries[i].dwAccurateTextureMapping != 0)
        fprintf(fh, "AccurateTextureMapping=%d\n", m_entries[i].dwAccurateTextureMapping);

    if (m_entries[i].dwFastTextureCRC != 0)
        fprintf(fh, "FastTextureCRC=%d\n", m_entries[i].dwFastTextureCRC);

    if (m_entries[i].dwNormalBlender != 0)
        fprintf(fh, "NormalAlphaBlender=%d\n", m_entries[i].dwNormalBlender);

    if (m_entries[i].dwNormalCombiner != 0)
        fprintf(fh, "NormalColorCombiner=%d\n", m_entries[i].dwNormalCombiner);

    // Normal bi-state variables
    if (m_entries[i].bDisableTextureCRC)
        fprintf(fh, "DisableTextureCRC\n");

    if (m_entries[i].bDisableCulling)
        fprintf(fh, "DisableCulling\n");

    if (m_entries[i].bPrimaryDepthHack)
        fprintf(fh, "PrimaryDepthHack\n");

    if (m_entries[i].bTexture1Hack)
        fprintf(fh, "Texture1Hack\n");

    if (m_entries[i].bFastLoadTile)
        fprintf(fh, "FastLoadTile\n");

    if (m_entries[i].bUseSmallerTexture)
        fprintf(fh, "UseSmallerTexture\n");

    if (m_entries[i].bIncTexRectEdge)
        fprintf(fh, "IncTexRectEdge\n");

    if (m_entries[i].bZHack)
        fprintf(fh, "ZHack\n");

    if (m_entries[i].bTextureScaleHack)
        fprintf(fh, "TexRectScaleHack\n");

    if (m_entries[i].dwVIWidth > 0)
        fprintf(fh, "VIWidth=%d\n", m_entries[i].dwVIWidth);

    if (m_entries[i].dwVIHeight > 0)
        fprintf(fh, "VIHeight=%d\n", m_entries[i].dwVIHeight);

    if (m_entries[i].dwUseCIWidthAndRatio > 0)
        fprintf(fh, "UseCIWidthAndRatio=%d\n", m_entries[i].dwUseCIWidthAndRatio);

    if (m_entries[i].dwFullTMEM > 0)
        fprintf(fh, "FullTMEM=%d\n", m_entries[i].dwFullTMEM);

    if (m_entries[i].bTxtSizeMethod2 != FALSE )
        fprintf(fh, "AlternativeTxtSizeMethod=%d\n", m_entries[i].bTxtSizeMethod2);

    if (m_entries[i].bEnableTxtLOD != FALSE )
        fprintf(fh, "EnableTxtLOD=%d\n", m_entries[i].bEnableTxtLOD);

    if (m_entries[i].bDisableObjBG != 0 )
        fprintf(fh, "DisableObjBG=%d\n", m_entries[i].bDisableObjBG);

    if (m_entries[i].bForceScreenClear != 0)
        fprintf(fh, "ForceScreenClear=%d\n", m_entries[i].bForceScreenClear);

    if (m_entries[i].bEmulateClear != 0)
        fprintf(fh, "EmulateClear=%d\n", m_entries[i].bEmulateClear);

    if (m_entries[i].bDisableBlender != 0)
        fprintf(fh, "DisableAlphaBlender=%d\n", m_entries[i].bDisableBlender);

    if (m_entries[i].bForceDepthBuffer != 0)
        fprintf(fh, "ForceDepthBuffer=%d\n", m_entries[i].bForceDepthBuffer);

    if (m_entries[i].dwFrameBufferOption != 0)
        fprintf(fh, "FrameBufferEmulation=%d\n", m_entries[i].dwFrameBufferOption);

    if (m_entries[i].dwRenderToTextureOption != 0)
        fprintf(fh, "RenderToTexture=%d\n", m_entries[i].dwRenderToTextureOption);

    if (m_entries[i].dwScreenUpdateSetting != 0)
        fprintf(fh, "ScreenUpdateSetting=%d\n", m_entries[i].dwScreenUpdateSetting);

    fprintf(fh, "\n");          // Spacer
}

INIEntry::INIEntry()
{
    bOutput = false;
    crccheck = "";
    name = "";

    dwNormalCombiner = 0;
    dwNormalBlender = 0;
    dwFastTextureCRC = 0;
    dwAccurateTextureMapping = 0;
    dwFrameBufferOption = 0;
    dwRenderToTextureOption = 0;
    dwScreenUpdateSetting = 0;

    bDisableBlender = false;
    bForceScreenClear = false;
    bEmulateClear = false;
    bForceDepthBuffer = false;

    bDisableObjBG = false;
    bDisableTextureCRC = false;
    bIncTexRectEdge = false;
    bZHack = false;
    bTextureScaleHack = false;
    bFastLoadTile = false;
    bUseSmallerTexture = false;
    bPrimaryDepthHack = false;
    bTexture1Hack = false;
    bDisableCulling = false;

    dwVIWidth = -1;
    dwVIHeight = -1;
    dwUseCIWidthAndRatio = NOT_USE_CI_WIDTH_AND_RATIO;

    dwFullTMEM = 0;
    bTxtSizeMethod2 = false;
    bEnableTxtLOD = false;
}

INIEntry::INIEntry(const INIEntry& toCopy)
    : QObject()
{
    bOutput = toCopy.bOutput;
    crccheck = toCopy.crccheck;
    name = toCopy.name;

    dwNormalCombiner = toCopy.dwNormalCombiner;
    dwNormalBlender = toCopy.dwNormalBlender;
    dwFastTextureCRC = toCopy.dwFastTextureCRC;
    dwAccurateTextureMapping = toCopy.dwAccurateTextureMapping;
    dwFrameBufferOption = toCopy.dwFrameBufferOption;
    dwRenderToTextureOption = toCopy.dwRenderToTextureOption;
    dwScreenUpdateSetting = toCopy.dwScreenUpdateSetting;

    bDisableBlender = toCopy.bDisableBlender;
    bForceScreenClear = toCopy.bForceScreenClear;
    bEmulateClear = toCopy.bEmulateClear;
    bForceDepthBuffer = toCopy.bForceDepthBuffer;

    bDisableObjBG = toCopy.bDisableObjBG;
    bDisableTextureCRC = toCopy.bDisableTextureCRC;
    bIncTexRectEdge = toCopy.bIncTexRectEdge;
    bZHack = toCopy.bZHack;
    bTextureScaleHack = toCopy.bTextureScaleHack;
    bFastLoadTile = toCopy.bFastLoadTile;
    bUseSmallerTexture = toCopy.bUseSmallerTexture;
    bPrimaryDepthHack = toCopy.bPrimaryDepthHack;
    bTexture1Hack = toCopy.bTexture1Hack;
    bDisableCulling = toCopy.bDisableCulling;

    dwVIWidth = toCopy.dwVIWidth;
    dwVIHeight = toCopy.dwVIHeight;
    dwUseCIWidthAndRatio = toCopy.dwUseCIWidthAndRatio;

    dwFullTMEM = toCopy.dwFullTMEM;
    bTxtSizeMethod2 = toCopy.bTxtSizeMethod2;
    bEnableTxtLOD = toCopy.bEnableTxtLOD;
}

INIEntry& INIEntry::operator=(const INIEntry& toCopy)
{
    bOutput = toCopy.bOutput;
    crccheck = toCopy.crccheck;
    name = toCopy.name;

    dwNormalCombiner = toCopy.dwNormalCombiner;
    dwNormalBlender = toCopy.dwNormalBlender;
    dwFastTextureCRC = toCopy.dwFastTextureCRC;
    dwAccurateTextureMapping = toCopy.dwAccurateTextureMapping;
    dwFrameBufferOption = toCopy.dwFrameBufferOption;
    dwRenderToTextureOption = toCopy.dwRenderToTextureOption;
    dwScreenUpdateSetting = toCopy.dwScreenUpdateSetting;

    bDisableBlender = toCopy.bDisableBlender;
    bForceScreenClear = toCopy.bForceScreenClear;
    bEmulateClear = toCopy.bEmulateClear;
    bForceDepthBuffer = toCopy.bForceDepthBuffer;

    bDisableObjBG = toCopy.bDisableObjBG;
    bDisableTextureCRC = toCopy.bDisableTextureCRC;
    bIncTexRectEdge = toCopy.bIncTexRectEdge;
    bZHack = toCopy.bZHack;
    bTextureScaleHack = toCopy.bTextureScaleHack;
    bFastLoadTile = toCopy.bFastLoadTile;
    bUseSmallerTexture = toCopy.bUseSmallerTexture;
    bPrimaryDepthHack = toCopy.bPrimaryDepthHack;
    bTexture1Hack = toCopy.bTexture1Hack;
    bDisableCulling = toCopy.bDisableCulling;

    dwVIWidth = toCopy.dwVIWidth;
    dwVIHeight = toCopy.dwVIHeight;
    dwUseCIWidthAndRatio = toCopy.dwUseCIWidthAndRatio;

    dwFullTMEM = toCopy.dwFullTMEM;
    bTxtSizeMethod2 = toCopy.bTxtSizeMethod2;
    bEnableTxtLOD = toCopy.bEnableTxtLOD;
    return *this;
}


