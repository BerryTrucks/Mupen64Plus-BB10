#include "bbutil.h"
#include <bps/event.h>
#include "plugin.h"

typedef struct {
	int x, y;
}InputStick;

void ProcessKeyboardEvent(screen_event_t *event,SController* controller,unsigned short* button_bits);
int TranslateBluetoothKeyboard(int sym, int mods, int flags, int scan, int cap, SDL_keysym *keysym);
void InitKeyboard();
