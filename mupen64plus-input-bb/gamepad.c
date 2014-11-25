#include "gamepad.h"
#include "input.h"

#include <stdio.h>

enum GP_GamePadButton{
   GP_A_BUTTON=0,
   GP_B_BUTTON,
   GP_C_BUTTON,
   GP_X_BUTTON,
   GP_Y_BUTTON,
   GP_Z_BUTTON,
   GP_MENU1_BUTTON,
   GP_MENU2_BUTTON,
   GP_MENU3_BUTTON,
   GP_MENU4_BUTTON,
   GP_L1_BUTTON,
   GP_L2_BUTTON,
   GP_L3_BUTTON,
   GP_R1_BUTTON,
   GP_R2_BUTTON,
   GP_R3_BUTTON,
   GP_DPAD_UP_BUTTON,
   GP_DPAD_DOWN_BUTTON,
   GP_DPAD_LEFT_BUTTON,
   GP_DPAD_RIGHT_BUTTON,
   GP_NO_BUTTON,
   GP_EXT_BUTTON_L4,
   GP_EXT_BUTTON_R4,
   GP_BUTTON_MAX=23
 };

int buttons;
int analog0[3], analog1[3];
int pressed[GP_BUTTON_MAX];
char gamepadId[4][256];
screen_device_t usedDevices[4];

extern unsigned int inputButtons[4][16];
extern InputStick inputStick[4];

void ProcessGamepadEvent(screen_event_t *event,SController* controller,unsigned short* button_bits)
{
	screen_device_t device;
	screen_get_event_property_pv(*event, SCREEN_PROPERTY_DEVICE, (void**)&device);
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_BUTTONS, &buttons);
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_ANALOG0, analog0);
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_ANALOG1, analog1);
	/*screen_get_device_property_iv(device, SCREEN_PROPERTY_BUTTONS, &buttons);
	screen_get_device_property_iv(device, SCREEN_PROPERTY_ANALOG0, analog0);
	screen_get_device_property_iv(device, SCREEN_PROPERTY_ANALOG1, analog1);*/
	char id[64];
	screen_get_device_property_cv(device, SCREEN_PROPERTY_ID_STRING, sizeof(id), id);
	//printf("Gamepad event with buttons %d\n", buttons);fflush(stdout);
	//printf("Gamepad event with analog0 (%d, %d, %d)\n", analog0[0], analog0[1], analog0[2]);fflush(stdout);

	int i;
	for (i = GP_A_BUTTON; i < GP_NO_BUTTON; i++)
	{
		if (buttons & (1 << i))
			pressed[i] = 1;
		else
			pressed[i] = 0;
	}
	if (analog0[2] == 255)
		pressed[GP_EXT_BUTTON_L4] = 1;
	else
		pressed[GP_EXT_BUTTON_L4] = 0;
	if (analog1[2] == 255)
		pressed[GP_EXT_BUTTON_R4] = 1;
	else
		pressed[GP_EXT_BUTTON_R4] = 0;
		
	//TODO remove me; debug
	//int used = 0;
	
	int d, c, b, axis_val, axis_max_val;
	axis_max_val = 80;
	
	for (c = 0; c < 4; c++)
	{
		if (strcmp(id, gamepadId[c]))
			continue;
		//used++;
		//printf("Check user %d for controller event (has controller %s)\n", c, gamepadId[c]);
		for (b = 0; b < 16; b++)
		{
			if(((int)controller[c].button[b].key) < 0)
				continue;
			for (d = GP_A_BUTTON; d < GP_BUTTON_MAX; d++)
			{
				if (((int)controller[c].button[b].key) == (1 << d))
				{
					if (pressed[d])
						inputButtons[c][b] = 1;
					else
						inputButtons[c][b] = 0;
				}
			}
		}
		if (controller[c].axis[0].key_a == -2)
		{
			double x = (double)analog0[0];
			double y = (double)analog0[1];
			inputStick[c].x = x * ((x < 0) ? 0.625f : 0.6299212598425196f);
			inputStick[c].y = y * ((y < 0) ? 0.625f : 0.6299212598425196f);
		}
		else if (controller[c].axis[0].key_a == -3)
		{
			double x = (double)analog1[0];
			double y = (double)analog1[1];
			inputStick[c].x = x * ((x < 0) ? 0.625f : 0.6299212598425196f);
			inputStick[c].y = y * ((y < 0) ? 0.625f : 0.6299212598425196f);
		}
		else
		{
			for(b = 0; b < 2; b++)
			{
				// from the N64 func ref: The 3D Stick data is of type signed char and in
				// the range between 80 and -80. (32768 / 409 = ~80.1)
				if(b == 0)
					axis_val = inputStick[c].x;
				else
					axis_val = inputStick[c].y;

				if(((int) controller[c].axis[b].key_a) > 0)
				{
					for (d = GP_A_BUTTON; d < GP_BUTTON_MAX; d++)
					{
						if(controller[c].axis[b].key_a == (1 << d)) {
							if(pressed[d]) {
								axis_val = -axis_max_val;
							}
							else {
								axis_val = 0;
							}
						}
					}
				}
				if (axis_val == 0)
				{
					if(((int)controller[c].axis[b].key_b) > 0)
					{
						for (d = GP_A_BUTTON; d < GP_BUTTON_MAX; d++)
						{
							if(controller[c].axis[b].key_b == (1 << d)) {
								if(pressed[d]) {
									axis_val = axis_max_val;
								}
								else {
									axis_val = 0;
								}
							}
						}
					}
				}

				if(b == 0) {
					inputStick[c].x = axis_val;
				}
				else {
					inputStick[c].y = axis_val;
				}
			}
		}
	}
	
	//TODO remove me; debug
	/*if (!used)
	{
		printf("Control %d not used for controller %s\n", buttons, id);
		fflush(stdout);
	}*/
}

void initGamePad()
{
    if (screen_request_events(screen_cxt) != BPS_SUCCESS)
    {
        printf("Error getting screen events in mupen64plus-intput-bb gamepad\n");fflush(stdout);
    }
	usedDevices[0] = 0;
	usedDevices[1] = 0;
	usedDevices[2] = 0;
	usedDevices[3] = 0;
	memset(pressed, 0, GP_BUTTON_MAX);
	analog0[0] = 0;
	analog0[1] = 0;
	analog0[2] = 0;
	analog1[0] = 0;
	analog1[1] = 0;
	analog1[2] = 0;
	/*int deviceCount;
	screen_get_context_property_iv(screen_cxt, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount);
	fprintf(stderr, "Initializing %d Gamepads\n", deviceCount);
	if (deviceCount > 0)
	{
		screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));
		screen_get_context_property_pv(screen_cxt, SCREEN_PROPERTY_DEVICES, (void**)devices);
		int i;
		for (i = 0; i < deviceCount; i++)
		{
			int type;
			screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type);
			fprintf(stderr, "Checking device type: %d\n", type);

			if (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)
			{
				char id[64];
				screen_get_device_property_cv(devices[i], SCREEN_PROPERTY_ID_STRING, sizeof(id), id);
				fprintf(stderr, "Checking device: %s\n", id);
				int j;
				for (j = 0; j < 4; j++)
				{
					fprintf(stderr, "\tagainst: %s\n", gamepadId[j]);
					if (strcmp(id, gamepadId[j]) == 0)
					{
						usedDevices[j] = devices[i];
						fprintf(stderr, "Found a device for player %d: %s\n", j + 1, id);
						break;
					}
				}
			}
		}

		free(devices);
	}*/
}