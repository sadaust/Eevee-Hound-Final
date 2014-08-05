#pragma once

#include <windows.h>
#include <XInput.h>
#include <dinput.h>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

enum binds {
	jump,
	leftAttack,
	leftAltAttack,
	rightAttack,
	rightAltAttack,
	
	last
};

struct inputState {
	float lX;
	float lY;
	float rX;
	float rY;

};

class InputHandler {
private:
	IDirectInput8* m_pDIObj;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	unsigned char binds[4]; 
	//keyboard state
	char buffer[256];
	//mouse state
	DIMOUSESTATE2 mouseState;
public:
	InputHandler();
	void init(HWND& hWnd,HINSTANCE& hInst);
	int numGamePads();
	bool getState(int padNum,XINPUT_STATE& state);
	void setVibration(int padNum,XINPUT_VIBRATION& state);
	void setVibration(int padNum,float leftSpeed,float rightSpeed);
	void setVibration(int padNum,WORD leftSpeed,WORD rightSpeed);
	void stopVibration(int padNum);
	void update();
	//0-3 controllers 4 keyboard, returns true if it works
	bool getState(int controllerNum,inputState& out);
	void shutdown();
	~InputHandler();
};