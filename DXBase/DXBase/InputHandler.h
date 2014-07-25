#pragma once

#include <windows.h>
#include <XInput.h>
#include <dinput.h>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputHandler {
private:
	IDirectInput8* m_pDIObj;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
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
	void shutdown();
	~InputHandler();
};