/*
 * GeneralSettings.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: Travis
 */

#include "GeneralSettings.hpp"


void GeneralSettings::reset()
{
    Theme(-1);
    PrimaryColourIndex(1);
    PrimaryColourRed(255);
    PrimaryColourGreen(0);
    PrimaryColourBlue(0);
    BaseColourIndex(0);
    BaseColourRed(0);
    BaseColourGreen(0);
    BaseColourBlue(0);
    CheckVersion(true);
    emit ThemeChanged();
    emit PrimaryColourIndexChanged();
    emit PrimaryColourRedChanged();
    emit PrimaryColourGreenChanged();
    emit PrimaryColourBlueChanged();
    emit BaseColourIndexChanged();
    emit BaseColourRedChanged();
    emit BaseColourGreenChanged();
    emit BaseColourBlueChanged();
    emit CheckVersionChanged();
}


