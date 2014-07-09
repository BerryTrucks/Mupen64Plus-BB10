#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include <bb/cascades/Application>
#include <QObject>

/*#ifdef __cplusplus
extern "C" {
#endif*/
#include "cheat.h"
/*#ifdef __cplusplus
}
#endif*/

typedef struct {
	int a;
	int b;
} Axis;

typedef struct {
	int present;
	int plugin;
	int device;
	int mouse;
	int layout;

	int button[16];

	Axis axis[2];

	int analogDeadZone[2];
	int analogPeak[2];
	int diagonals[4];
	char gamepadId[64];
} Controller;

extern const char *button_names[];

class Emulator : public QObject
{
    Q_OBJECT

signals:
    void focusScreen();

public:
	Emulator(char * groupId, char * windowId);
	~Emulator();
	int SetConfigParameter(std::string ParamSpec);
	int Start();
	void SetRom(const char * rom );
	int LoadRom();
	char * l_CheatNumList;
	char l_RomName[256];
	Controller controller[4];
	void save_controller_config(int iCtrlIdx);
	void SaveState(const char* filename = 0);
	void LoadState(const char* filename = 0);
	void SendKeyDown(int key, int mod);
    void SendKeyUp(int key, int mod);
	void LoadTouchOverlay();
	void ExitEmulator();
	int print_controller_config();
	int getSaveStateSlot();
	void setSaveStateSlot(int slot);

private:
	int load_controller_config(const char *SectionName, int i);
	int init_controller_config();
	unsigned char* loadFromFile(int* error, size_t* size);
	unsigned char* loadFromZip(int* error, size_t* size);
};

#endif // ifndef EMUALTOR_H
