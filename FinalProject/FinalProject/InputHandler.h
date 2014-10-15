#pragma once

#include <windows.h>
#include <XInput.h>
#include <dinput.h>
#include <vector>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace binds {
	//BYTE fireTriggerPoint = 100;
	enum bindList {
		jump,
		legPower,
		use,
		headPower,
		pause,
		leftAttack,
		leftAltAttack,
		rightAttack,
		rightAltAttack,
		forward,
		back,
		left,
		right,
		sprint,
		leftSelect,
		rightSelect,
		leave,
		last
	};
};

struct inputState {
	float lX;
	float lY;
	float rX;
	float rY;
	bool buttons[binds::last];
	bool buttonLast[binds::last];
};

struct sensitivity {
	float xSens;
	float ySens;
};

class InputHandler {
private:
	IDirectInput8* m_pDIObj;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	inputState state[5];
	int binds[binds::last]; 
	int mouseOffSet;
	//keyboard state
	char buffer[256];
	//mouse state
	DIMOUSESTATE2 mouseState;
	sensitivity sens[5];
	bool getBoundKey(int num);
	BYTE fireTriggerPoint;
public:
	InputHandler();
	void init(HWND& hWnd,HINSTANCE& hInst);
	int numGamePads();
	bool getState(int padNum,XINPUT_STATE& state);
	void setVibration(int padNum,XINPUT_VIBRATION& state);
	void setVibration(int padNum,float leftSpeed,float rightSpeed);
	void setVibration(int padNum,WORD leftSpeed,WORD rightSpeed);
	void stopVibration(int padNum);
	void setBind(binds::bindList,int keyNum);
	DIMOUSESTATE2 getMouse();
	void getKeyboard(char[]);
	void update();
	bool setSens(int pNum, float xSens, float ySens);
	//0-3 controllers 4 keyboard, returns true if it works
	bool getState(int controllerNum,inputState& out);
	void shutdown();
	~InputHandler();
};