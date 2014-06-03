#include "input.h"
#include "touch.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/keycodes.h>

static SDL_keysym Playbook_Keycodes[256];
static SDLKey *Playbook_specialsyms;

unsigned int inputButtons[4][16];

InputStick inputStick[4];

int joyX, joyY;

//TODO: remove SDL
//TODO: add https://github.com/blackberry/NDK-Samples/tree/master/Gamepad
void ProcessKeyboardEvent(screen_event_t *event,SController* controller,unsigned short* button_bits) {
	static const int KEYBOARD_TYPE_MASK = 0x20;
	int sym = 0;
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_KEY_SYM, &sym);
	int modifiers = 0;
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_KEY_MODIFIERS, &modifiers);
	int flags = 0;
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_KEY_FLAGS, &flags);
	int scan = 0;
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_KEY_SCAN, &scan);
	int cap = 0;
	screen_get_event_property_iv(*event, SCREEN_PROPERTY_KEY_CAP, &cap);
	
	SDL_keysym keysym;
	if ((flags & KEYBOARD_TYPE_MASK)) {
		if (!TranslateBluetoothKeyboard(sym, modifiers, flags, scan, cap, &keysym))
		{
			return; // No translation
		}
	}

	//printf("Keyboard Translated: sym:%d, state:%d\n", keysym.sym, flags & KEY_DOWN);fflush(stdout);

	int c, b, axis_val, axis_max_val;

	axis_max_val = 80;

	//TODO: Re-enable walking by detecting shift and going to half max axis
	//if (keystate[SDLK_RCTRL])
		//axis_max_val -= 40;
	//if (keystate[SDLK_RSHIFT])
		//axis_max_val -= 20;

	for( c = 0; c < 4; c++ )
	{
		for( b = 0; b < 16; b++ )
		{
			if( controller[c].button[b].key == SDLK_UNKNOWN || ((int) controller[c].button[b].key) < 0)
				continue;
			if( controller[c].button[b].key == keysym.sym ){
				if(flags & KEY_DOWN){
					inputButtons[c][b] = 1;
				} else {
					inputButtons[c][b] = 0;
				}
			}
				//controller[c].buttons.Value |= button_bits[b];
		}
		for( b = 0; b < 2; b++ )
		{
			// from the N64 func ref: The 3D Stick data is of type signed char and in
			// the range between 80 and -80. (32768 / 409 = ~80.1)
			if( b == 0 )
				axis_val = inputStick[c].x;//axis_val = controller[c].buttons.X_AXIS;
			else
				axis_val = inputStick[c].y;//axis_val = -controller[c].buttons.Y_AXIS;

			if( controller[c].axis[b].key_a != SDLK_UNKNOWN && ((int) controller[c].axis[b].key_a) > 0)
				if( controller[c].axis[b].key_a == keysym.sym){
					if(flags & KEY_DOWN){
						axis_val = -axis_max_val;
						if (b == 0)
							joyX = axis_val;
						else
							joyY = axis_val;
					} else {
						axis_val = 0;
						if (b == 0)
							joyX = 0;
						else
							joyY = 0;
					}
				}
			if( controller[c].axis[b].key_b != SDLK_UNKNOWN && ((int) controller[c].axis[b].key_b) > 0)
				if( controller[c].axis[b].key_b == keysym.sym ){
					if(flags & KEY_DOWN){
						axis_val = axis_max_val;
						if (b == 0)
							joyX = axis_val;
						else
							joyY = axis_val;
					} else {
						axis_val = 0;
						if (b == 0)
							joyX = 0;
						else
							joyY = 0;
					}
				}

			if( b == 0 ){
				inputStick[c].x = axis_val;
			}
			else{
				inputStick[c].y = axis_val;
			}
		}
		{
			int x_axis, y_axis;
			x_axis = inputStick[c].x;
			y_axis = inputStick[c].y;
			if (controller[c].diagonals[0] == keysym.sym) {
				if (flags & KEY_DOWN)
				{
					x_axis = -axis_max_val;
					y_axis = -axis_max_val;
				}
				else
				{
					x_axis = joyX;
					y_axis = joyY;
				}
			}
			else if (controller[c].diagonals[1] == keysym.sym) {
				if (flags & KEY_DOWN)
				{
					x_axis = axis_max_val;
					y_axis = -axis_max_val;
				}
				else
				{
					x_axis = joyX;
					y_axis = joyY;
				}
			}
			else if (controller[c].diagonals[2] == keysym.sym) {
				if (flags & KEY_DOWN)
				{
					x_axis = -axis_max_val;
					y_axis = axis_max_val;
				}
				else
				{
					x_axis = joyX;
					y_axis = joyY;
				}
			}
			else if (controller[c].diagonals[3] == keysym.sym) {
				if (flags & KEY_DOWN)
				{
					x_axis = axis_max_val;
					y_axis = axis_max_val;
				}
				else
				{
					x_axis = joyX;
					y_axis = joyY;
				}
			}
			inputStick[c].x = x_axis;
			inputStick[c].y = y_axis;
		}
	}
}

void InitKeyboard(){
	// We match perfectly from 32 to 64
	int i = 32, j = 0;
	for (; i<65; i++)
	{
		Playbook_Keycodes[i].sym = i;
	}
	// Capital letters
	for (; i<91; i++)
	{
		Playbook_Keycodes[i].sym = i+32;
		Playbook_Keycodes[i].mod = KMOD_LSHIFT;
	}
	// Perfect matches again from 91 to 122
	for (; i<123; i++)
	{
		Playbook_Keycodes[i].sym = i;
	}

	// Handle tilde
	//Playbook_Keycodes[126].sym = SDLK_PB_TILDE;

	Playbook_specialsyms = (SDLKey *)malloc(256 * sizeof(SDLKey));
	Playbook_specialsyms[SDLK_BACKSPACE] = SDLK_BACKSPACE;
	Playbook_specialsyms[SDLK_TAB] = SDLK_TAB;
	Playbook_specialsyms[SDLK_RETURN] = SDLK_RETURN;
	Playbook_specialsyms[SDLK_PAUSE] = SDLK_PAUSE;
	Playbook_specialsyms[SDLK_ESCAPE] = SDLK_ESCAPE;
	Playbook_specialsyms[0xff] = SDLK_DELETE;
	Playbook_specialsyms[0x52] = SDLK_UP;
	Playbook_specialsyms[0x54] = SDLK_DOWN;
	Playbook_specialsyms[0x53] = SDLK_RIGHT;
	Playbook_specialsyms[0x51] = SDLK_LEFT;
	Playbook_specialsyms[0x63] = SDLK_INSERT;
	Playbook_specialsyms[0x50] = SDLK_HOME;
	Playbook_specialsyms[0x57] = SDLK_END;
	Playbook_specialsyms[0x55] = SDLK_PAGEUP;
	Playbook_specialsyms[0x56] = SDLK_PAGEDOWN;
	Playbook_specialsyms[0xbe] = SDLK_F1;
	Playbook_specialsyms[0xbf] = SDLK_F2;
	Playbook_specialsyms[0xc0] = SDLK_F3;
	Playbook_specialsyms[0xc1] = SDLK_F4;
	Playbook_specialsyms[0xc2] = SDLK_F5;
	Playbook_specialsyms[0xc3] = SDLK_F6;
	Playbook_specialsyms[0xc4] = SDLK_F7;
	Playbook_specialsyms[0xc5] = SDLK_F8;
	Playbook_specialsyms[0xc6] = SDLK_F9;
	Playbook_specialsyms[0xc7] = SDLK_F10;
	Playbook_specialsyms[0xc8] = SDLK_F11;
	Playbook_specialsyms[0xc9] = SDLK_F12;
	Playbook_specialsyms[0xe5] = SDLK_CAPSLOCK;
	Playbook_specialsyms[0x14] = SDLK_SCROLLOCK;
	Playbook_specialsyms[0xe2] = SDLK_RSHIFT;
	Playbook_specialsyms[0xe1] = SDLK_LSHIFT;
	Playbook_specialsyms[0xe4] = SDLK_RCTRL;
	Playbook_specialsyms[0xe3] = SDLK_LCTRL;
	Playbook_specialsyms[0xe8] = SDLK_RALT;
	Playbook_specialsyms[0xe9] = SDLK_LALT;
	Playbook_specialsyms[0xbe] = SDLK_MENU;
	Playbook_specialsyms[0x61] = SDLK_SYSREQ;
	Playbook_specialsyms[0x6b] = SDLK_BREAK;

	for(i=0;i<4;++i){
		for(j=0;j<14;++j){
			inputButtons[i][j] = 0;
		}
	}
	
	joyX = 0;
	joyY = 0;
}

int TranslateBluetoothKeyboard(int sym, int mods, int flags, int scan, int cap, SDL_keysym *keysym)
{
	if (flags == 32) {
		return 0; // No translation for this - this is an addition message sent
		// with arrow keys, right ctrl, right ctrl and pause
	}
	// FIXME: Figure out how to separate left and right modifiers
	if (scan > 128)
		scan -= 128; // Keyup events have the high bit set, but we want to have the same scan for down and up.
	keysym->scancode = scan;
	keysym->mod = 0;
	if (mods & (0x1))
		keysym->mod |= KMOD_LSHIFT;
	if (mods & (0x2))
		keysym->mod |= KMOD_LCTRL;
	if (mods & (0x4))
		keysym->mod |= KMOD_LALT;
	if (mods & (0x10000))
		keysym->mod |= KMOD_CAPS;
	if (mods & (0x20000)) // FIXME: guessing
		keysym->mod |= KMOD_NUM;
	//if (mods & (0x40000))
		//keysym.mod |= SCROLL LOCK; // SDL has no scroll lock

	if (sym & 0xf000) {
		sym = sym & 0xff;
		keysym->sym = Playbook_specialsyms[sym];
	} else {
		keysym->sym = Playbook_Keycodes[sym].sym;
	}

	return 1;
}

void ApplyInputButtons(SController* controller,unsigned short* button_bits){

	int i, j;
	for(j=0;j<4;++j){
		if (!controller[j].control->Present || (controller[j].device != -2 && controller[j].device != -5))
			continue;

		if(controller[j].device != -5 && (j == 0) && (touchStick.finger != 0)){
			return;
		}

		if (controller[j].device != -5) {
			controller[j].buttons.X_AXIS = inputStick[j].x;
			controller[j].buttons.Y_AXIS = -inputStick[j].y;
		}

		for(i=0;i<16;i++)
		{
			if(inputButtons[j][i] != 0)
			{
				controller[j].buttons.Value |= button_bits[i];
			}
		}
	}
}
