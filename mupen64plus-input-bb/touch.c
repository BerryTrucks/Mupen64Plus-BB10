/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-input-sdl - touch.c                                	   *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2011 Metricity                                   	   *
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

#include "touch.h"
//#include "../mupen64plus-ui-console/ui_base.h"

#include <math.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#define OVERLAY_DIR "app/native/assets/images"
#define OVERLAY_EXTERNAL_DIR "shared/misc/n64/data"

#include <stdlib.h>

TouchButton touchButtons[14];
TouchButton secondZed;
TouchStick touchStick;
TouchStick touchStick2;

int showMenu = 1;
int showController = 1;
int width = 1024, height = 600;

int useAccelerometer = 0;
int currentOverlay = 0;
int overlayCount = 0;
char** availableOverlays;
overlay_t overlay_current;

char * overlays[8];

void LoadOverlay(char* name)
{
	printf("Loading %s Overlay!\n", name);fflush(stdout);

	if(strcmp(name, "None") == 0){
		showController = 0;
		return;
	} else {
		showController = 1;
	}

	if(overlayQuad)
	{
		free(overlayQuad->text);
		delete_image(overlayQuad, 0);
		delete_image(overlayKey, 1);
		delete_image(stickQuad, 0);
		delete_image(stickQuad2, 0);
	}


	printf("\nLoading Overlay Key...\n");fflush(stdout);

	char * s_width;
	int size = strlen(OVERLAY_DIR);
	if (overlay_request == 4 || overlay_request == 5){
		printf("Width 720 detected...\n");fflush(stdout);
		s_width = strdup("720");
	}else if (overlay_request == 6){
		printf("Width 1280 detected...\n");fflush(stdout);
		s_width = strdup("1280");
		size = strlen(OVERLAY_EXTERNAL_DIR);
	}else if(width == 1280){
		printf("Width 1280 detected...\n");fflush(stdout);
		s_width = strdup("1280");
	}else{
		printf("Width 1024 detected...\n");fflush(stdout);
		s_width = strdup("1024");
	}

	char* tmp = malloc(sizeof(char) * (size + strlen(name) + strlen(s_width) + 5 + 4 + 2)); // Name+Stick+.png+NULL

	//OverlayKey is only a pix buffer, not a texture as it's not used to render
	if (overlay_request == 6)
		sprintf(tmp,"%s/%s%sKey.png",OVERLAY_EXTERNAL_DIR, name, s_width);
	else
		sprintf(tmp,"%s/%s%sKey.png",OVERLAY_DIR, name, s_width);
	overlayKey = create_image(tmp, 0, 0, 1);
	if(overlayKey != NULL) {
		overlayKey->text = strdup(name);
	}

	if (overlay_request == 6)
		sprintf(tmp,"%s/%s.png",OVERLAY_EXTERNAL_DIR,name);
	else
		sprintf(tmp,"%s/%s.png",OVERLAY_DIR,name);
	overlayQuad = create_image(tmp, 0, 0, 0);
	if(overlayQuad != NULL) {
		overlayQuad->text = strdup(name);
		if (overlay_request == 3 || overlay_request >= 6)
			overlayQuad->scale = 1.40625f;
		else
			overlayQuad->scale = 1.25f;
	}
	

	sprintf(tmp,"%s/DefaultStick.png",OVERLAY_DIR);
	stickQuad = create_image(tmp, 0, 0, 0);
	if(stickQuad != NULL) {
		stickQuad->text = strdup(name);
	}
	
	if (overlay_request == 7)
	{
		stickQuad2 = create_image(tmp, 0, 0, 0);
		if (stickQuad2 != NULL) {
			stickQuad2->text = strdup(name);
		}
	}

	free(tmp);

	//Search for centre pixel and radius pixel
	float touchCentreX=0,touchCentreY=0,touchRadiusX=100,touchRadiusY=100;
	float touchCentreX2=0,touchCentreY2=0,touchRadiusX2=100,touchRadiusY2=100;
	Uint32* p = (Uint32*)overlayKey->raw_pix;
	int x,y;
	for(y = 0;y < (int)overlayKey->textureH;y++)
	{
		for(x = 0;x < (int)overlayKey->textureW;x++)
		{
			if(*p == SDL_Swap32(0x00FFFFFF))//Centre
			{
				DebugMessage(M64MSG_VERBOSE,"Found centre %i %i",x,y);
				touchCentreX = x;
				touchCentreY = y;
			}
			else if(*p == SDL_Swap32(0xA7FFFCFF))
			{
				DebugMessage(M64MSG_VERBOSE,"Found radius %i %i",x,y);
				touchRadiusX = x;
				touchRadiusY = y;
			}
			else if (overlay_request == 7)
			{
				if (*p == SDL_Swap32(0x64FFFFFF))//Centre 2
				{
					DebugMessage(M64MSG_VERBOSE, "Found second centre %i %i", x, y);
					touchCentreX2 = x;
					touchCentreY2 = y;
				}
				else if (*p == SDL_Swap32(0x96FFFCFF))//Radius 2
				{
					DebugMessage(M64MSG_VERBOSE, "Found second radius %i %i", x, y);
					touchRadiusX2 = x;
					touchRadiusY2 = y;
				}
			}
			p++;
		}
	}

	touchStick.radius = sqrt(pow(touchCentreX-touchRadiusX,2.0)+pow(touchCentreY-touchRadiusY,2.0));;
	touchStick.xCentre = touchCentreX;
	touchStick.yCentre = touchCentreY;
	touchStick.finger = 0;
	
	if (overlay_request == 7)
	{
		touchStick2.radius = sqrt(pow(touchCentreX2 - touchRadiusX2, 2.0) + pow(touchCentreY2 - touchRadiusY2, 2.0));
		touchStick2.xCentre = touchCentreX2;
		touchStick2.yCentre = touchCentreY2;
		touchStick2.finger = 0;
	}
}

void InitTouchInput()
{
	int tmp;

	//if(initialized_n64 == 0){

		const char *env = getenv("WIDTH");

		if (0 == env) {
			perror("failed getenv for WIDTH");
		}

		width = atoi(env);

		env = getenv("HEIGHT");

		if (0 == env) {
			perror("failed getenv for HEIGHT");
		}

		height = atoi(env);

		tmp = height;
		height = width;
		width = tmp;

		compile_text_program();
		if(text_rendering_program == 0){
			printf("Error creating text program...");
		}

		printf("Width: %d, Height: %d detected...\n", width, height);fflush(stdout);

		font = bbutil_load_font("/usr/fonts/font_repository/monotype/arial.ttf", 16, 170);

		//bbutil_measure_text(font, "Game State Saved!", &tWidth, &tHeight);
		osd_save = create_text(font, 0.0f, 0.0f, "Game State Saved!");
		offset_text(osd_save, ((float)width/2) - (osd_save->textureW/2), 0.0f);

		//bbutil_measure_text(font, "Game State Loaded!", &tWidth, &tHeight);
		osd_load = create_text(font, 0.0f, 0.0f, "Game State Loaded!");
		offset_text(osd_load, ((float)width/2) - (osd_save->textureW/2), 0.0f);

		overlays[0] = "None";
		overlays[1] = "Default";
		overlays[2] = "Alternate";
		overlays[3] = "Fullscreen";
		overlays[4] = "Keyboard";
		overlays[5] = "KeyboardFlip";
		overlays[6] = "custom";
		overlays[7] = "Dual";

		LoadOverlay(overlays[overlay_request]);
		overlay_current = overlay_request;

		initialized_n64 = 1;
	//}

	memset(touchButtons, 0, sizeof(TouchButton) * 9);
	//keyColor is RGBA
	touchButtons[0].button = START_BUTTON;
	touchButtons[0].keyColour = SDL_Swap32(0xFF0000FF);

	touchButtons[1].button = Z_TRIG;
	touchButtons[1].keyColour = SDL_Swap32(0xBDD9FFFF);

	touchButtons[2].button = A_BUTTON;
	touchButtons[2].keyColour = SDL_Swap32(0x2A00FFFF);

	touchButtons[3].button = B_BUTTON;
	touchButtons[3].keyColour = SDL_Swap32(0x00FF37FF);

	touchButtons[4].button = L_TRIG;
	touchButtons[4].keyColour = SDL_Swap32(0x009CFFFF);

	touchButtons[5].button = R_TRIG;
	touchButtons[5].keyColour = SDL_Swap32(0xFF00C6FF);

	touchButtons[6].button = U_DPAD;
	touchButtons[6].keyColour = SDL_Swap32(0x2B2B2BFF);

	touchButtons[7].button = D_DPAD;
	touchButtons[7].keyColour = SDL_Swap32(0x7F7F7FFF);

	touchButtons[8].button = L_DPAD;
	touchButtons[8].keyColour = SDL_Swap32(0xFFFFFFFF);

	touchButtons[9].button = R_DPAD;
	touchButtons[9].keyColour = SDL_Swap32(0xC4C3C3FF);

	touchButtons[10].button = U_CBUTTON;
	touchButtons[10].keyColour = SDL_Swap32(0xFF9C00FF);

	touchButtons[11].button = D_CBUTTON;
	touchButtons[11].keyColour = SDL_Swap32(0xFFC600FF);

	touchButtons[12].button = L_CBUTTON;
	touchButtons[12].keyColour = SDL_Swap32(0xF6FF00FF);

	touchButtons[13].button = R_CBUTTON;
	touchButtons[13].keyColour = SDL_Swap32(0xFFEA00FF);
	
	secondZed.button = Z_TRIG;
	secondZed.keyColour = SDL_Swap32(0xBDFFFFFF);
	secondZed.finger = 0;
	
	if (overlay_current == 7)
	{
		if (gamepadId[0][0] == '0')
			controller[2].control->Present = 1;
		else
			controller[1].control->Present = 1;
	}

	printf("InitTouchInput Complete...\n");fflush(stdout);
}

void ProcessTouchEvent(screen_event_t *event,SController* controller,unsigned short* button_bits)
{
	int x = 0,y = 0;
	Uint8 finger = 0;
	int type;
	int contactId;
	int pos[2];
	//int  z = -10;
	double dist;
	int buttonHit = 0;
	TouchButton* whichButton = NULL;

	screen_get_event_property_iv(*event, SCREEN_PROPERTY_TYPE, &type);
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_TOUCH_ID, (int*)&contactId);
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_SOURCE_POSITION, pos);

	x = pos[0];
	y = height-pos[1];
	finger = contactId + 1;

	if(x < 0){
		x = 0;
	}
	if(y < 0){
		y = 0;
	}

	if(overlayKey && x < overlayKey->textureW && y < overlayKey->textureH)
	{
		uint32_t pixel = ((Uint32*)overlayKey->raw_pix)[(y * (int)overlayKey->textureW) + x];

		int i;

		//printf("pixel: %x", pixel);fflush(stdout);
		for(i=0;i < (sizeof(touchButtons)/sizeof(touchButtons[0]));i++)
		{
			if(touchButtons[i].finger == finger)
				whichButton = &touchButtons[i];

			if(touchButtons[i].keyColour == pixel && finger != touchStick.finger)
			{
				//printf("Button Pressed! Colour: %x\n", touchButtons[i].keyColour);fflush(stdout);
				touchButtons[i].finger = finger;
				buttonHit = 1;
				break;
			}
		}
		if (overlay_current == 7)
		{
			if (secondZed.finger == finger)
				whichButton = &secondZed;
			if (secondZed.keyColour == pixel && finger != touchStick2.finger) 
			{
				secondZed.finger = finger;
				buttonHit = 1;
			}
		}

		dist = sqrt(pow((double)x-touchStick.xCentre,2.0)+pow((double)y-touchStick.yCentre,2.0));

		if(dist < touchStick.radius && touchStick.finger == 0)
		{
			touchStick.finger = finger;
		}
		else
		{
			dist = sqrt(pow((double)x - touchStick2.xCentre, 2.0) + pow((double)y - touchStick2.yCentre, 2.0));
			if (overlay_current == 7 && dist < touchStick2.radius && touchStick2.finger == 0)
				touchStick2.finger = finger;
		}
	}

	if((buttonHit == 0 || type == SCREEN_EVENT_MTOUCH_RELEASE) && whichButton) //if no button release button for correct finger (which)
	{
		whichButton->finger = 0;
	}


	//on mouseup turn stick off
	if(type == SCREEN_EVENT_MTOUCH_RELEASE)
	{
		if (finger == touchStick.finger)
		{
			//DebugMessage(M64MSG_VERBOSE,  "Turned stick off");
			touchStick.finger = 0;
			touchStick.xPos = touchStick.yPos = 0;
		}
		else if (overlay_current == 7 && finger == touchStick2.finger)
		{
			touchStick2.finger = 0;
			touchStick2.xPos = touchStick2.yPos = 0;
		}
	}

	//update stick position
	if(finger == touchStick.finger)
	{
		touchStick.xPos = SDL_min(SDL_max(((x - touchStick.xCentre) / touchStick.radius),-1.0f),1.0f) * 80.0;
		touchStick.yPos = SDL_min(SDL_max(((touchStick.yCentre - y) / touchStick.radius),-1.0f),1.0f) * 80.0;
	}
	else if (overlay_current == 7 && finger == touchStick2.finger)
	{
		touchStick2.xPos = SDL_min(SDL_max(((x - touchStick2.xCentre) / touchStick2.radius), -1.0f), 1.0f) * 80.0;
		touchStick2.yPos = SDL_min(SDL_max(((touchStick2.yCentre - y) / touchStick2.radius), -1.0f), 1.0f) * 80.0;
	}
}

int UseAccelerometer()
{
	return useAccelerometer;
}

void ApplyTouchInput(SController* controller,unsigned short* button_bits)
{
	int j;
	for (j = 0; j < 4; j++)
	{
		if (!controller[j].control->Present || (controller[j].device != -3 && controller[j].device != -5)) {
			continue;
		}
		if (overlay_current == 7)
		{
			if (gamepadId[0][0] == '0' && j == 2)
				continue;
			else if (j == 1)
				continue;
		}
		if(touchStick.finger != 0) //Apply touch input
		{
			//printf("Apply stick...\n");
			controller[j].buttons.X_AXIS = (int)touchStick.xPos;
			controller[j].buttons.Y_AXIS = -(int)touchStick.yPos;
		}
		/*else //Read accelerometer input for display
		{
			touchStick.xPos = controller[j].buttons.X_AXIS;
			touchStick.yPos = controller[j].buttons.Y_AXIS;
		}*/

		int i;
		for(i=0;i < (sizeof(touchButtons)/sizeof(touchButtons[0]));i++)
		{
			if(touchButtons[i].finger != 0)
			{
				//printf("Apply Button...\n");
				controller[j].buttons.Value |= button_bits[touchButtons[i].button];
			}
		}
	}
	
	if (overlay_current == 7)
	{
		if (gamepadId[0][0] == '0')
		{
			if (secondZed.finger != 0)
			{
				controller[2].buttons.Value |= button_bits[secondZed.button];
			}
			if (touchStick2.finger != 0)
			{
				controller[2].buttons.X_AXIS = (int)touchStick2.xPos;
				controller[2].buttons.Y_AXIS = -(int)touchStick2.yPos;
			}
		}
		else
		{
			if (secondZed.finger != 0)
			{
				controller[1].buttons.Value |= button_bits[secondZed.button];
			}
			if (touchStick2.finger != 0)
			{
				controller[1].buttons.X_AXIS = (int)touchStick2.xPos;
				controller[1].buttons.Y_AXIS = -(int)touchStick2.yPos;
			}
		}
	}
	fflush(stdout);
}

void DrawController()
{
	if(overlayQuad == NULL){
		printf("NULL OVERLAY!!!!\n");fflush(stdout);
	}

	render_image(overlayQuad);

	//TIP: Don't mix different coordinate systems.
	stickQuad->x = UIPixelToViewportX(touchStick.xCentre + touchStick.xPos) - (stickQuad->w/2.0f);// + touchStick.xPos + (stickQuad->w/2.0f));// -(stickQuad->w/2.0f));
	stickQuad->y = UIPixelToViewportY(touchStick.yCentre - touchStick.yPos) + (stickQuad->h/2.0f);// -touchStick.yPos + (stickQuad->h/2.0f));// -(stickQuad->h/2.0f));

	render_image(stickQuad);
	
	if (overlay_current == 7)
	{
		stickQuad2->x = UIPixelToViewportX(touchStick2.xCentre + touchStick2.xPos) - (stickQuad2->w/2.0f);
		stickQuad2->y = UIPixelToViewportY(touchStick2.yCentre - touchStick2.yPos) + (stickQuad2->h/2.0f);
		
		render_image(stickQuad2);
	}
}

void DrawMenu()
{

	if(save > 0){
		osd_save->alpha = save;
		render_text(font, osd_save);
		save -= 0.02f;

	}
	if(load > 0){
		osd_load->alpha = load;
		render_text(font, osd_load);
		load -= 0.02f;
	}
}

void touch_video_plugin_render_callback(void)
{
    GLint currentProgram,currentTexture,currentActiveTexture,currentBlendSRC,currentBlendDST,currentVertexAttrib,currentTexcoorAttrib;
    GLboolean currentDepth,currentCull,currentScissor,currentBlend,currentDepthMask;

    GLint currentVertexSize, currentTexSize;
    GLsizei currentVertexStride, currentTexStride;
    void *currentVertexPtr;
    void *currentTexPtr;

    if(overlay_request != overlay_current){
		LoadOverlay(overlays[overlay_request]);
		overlay_current = overlay_request;
	}

    if(!showController && !showMenu){
		return;
	}

    //Save settings

    glGetIntegerv(GL_CURRENT_PROGRAM,&currentProgram);
    glGetIntegerv(GL_TEXTURE_BINDING_2D,&currentTexture);
    glGetIntegerv(GL_BLEND_SRC_ALPHA,&currentBlendSRC);
    glGetIntegerv(GL_BLEND_DST_ALPHA,&currentBlendDST);
    glGetIntegerv(GL_ACTIVE_TEXTURE,&currentActiveTexture);

    glGetVertexAttribiv(0,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&currentVertexAttrib);
    glGetVertexAttribiv(0,GL_VERTEX_ATTRIB_ARRAY_SIZE,&currentVertexSize);
    glGetVertexAttribiv(0,GL_VERTEX_ATTRIB_ARRAY_STRIDE,&currentVertexStride);
    glGetVertexAttribPointerv(0,GL_VERTEX_ATTRIB_ARRAY_POINTER,&currentVertexPtr);


    glGetVertexAttribiv(1,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&currentTexcoorAttrib);
    glGetVertexAttribiv(1,GL_VERTEX_ATTRIB_ARRAY_SIZE,&currentTexSize);
    glGetVertexAttribiv(1,GL_VERTEX_ATTRIB_ARRAY_STRIDE,&currentTexStride);
    glGetVertexAttribPointerv(1,GL_VERTEX_ATTRIB_ARRAY_POINTER,&currentTexPtr);

    currentDepth = glIsEnabled(GL_DEPTH_TEST);
    currentCull = glIsEnabled(GL_CULL_FACE);
    currentScissor= glIsEnabled(GL_SCISSOR_TEST);
    currentBlend = glIsEnabled(GL_BLEND);

    glViewport(0,0,width,height);

    glGetBooleanv(GL_DEPTH_WRITEMASK,&currentDepthMask);

	glUseProgram(text_rendering_program);
	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texcoordLoc);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	if(showController){
		DrawController();
	}

	if(showMenu){
    	DrawMenu();
    }

    //Restore
    if(currentTexcoorAttrib)
    {
    	glVertexAttribPointer(1, currentTexSize, GL_FLOAT, GL_FALSE, currentTexStride, currentTexPtr);
    }
    else
    {
    	glDisableVertexAttribArray( 1 );
    }
    if(currentVertexAttrib)
    {
    	glVertexAttribPointer(0, currentVertexSize, GL_FLOAT, GL_FALSE, currentVertexStride, currentVertexPtr);
    }
    else
    {
    	glDisableVertexAttribArray( 0 );
    }

//    glDepthMask(GL_TRUE);
//    glClearDepthf(1.0f);
//    glClear(GL_DEPTH_BUFFER_BIT);

	glBlendFunc(currentBlendSRC,currentBlendDST);
	if(!currentBlend)
		glDisable(GL_BLEND);
	if(currentScissor)
		glEnable(GL_SCISSOR_TEST);
	if(currentCull)
		glEnable(GL_CULL_FACE);
	if(currentDepth)
		glEnable(GL_DEPTH_TEST);

	glDepthMask(currentDepthMask);

	glActiveTexture(currentActiveTexture);
	glBindTexture(GL_TEXTURE_2D,currentTexture);
    glUseProgram(currentProgram);
}
