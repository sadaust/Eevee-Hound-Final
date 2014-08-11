#include "InputHandler.h"

InputHandler::InputHandler() {
	m_pDIObj = 0;
	m_pDIKeyboard = 0;
	m_pDIMouse = 0;
}

char InputHandler::getBoundKey(int num) {
	if(num < 256)
		return buffer[num];
	else {
		num -= 256;
		return mouseState.rgbButtons[num];
	}
}

//get the mouse state
DIMOUSESTATE2 InputHandler::getMouse() {return mouseState;}

//get the keyboard state
void InputHandler::getKeyboard(char keyboard[]) {
	for(int i = 0; i < 256; ++i)
		keyboard[i] = buffer[i];
}

//run this to start input or to reset when changing window
void InputHandler::init(HWND& hWnd,HINSTANCE& hInst) {
	if(m_pDIMouse) {
		m_pDIMouse->Release();
		m_pDIMouse = 0;
	}

	if(m_pDIKeyboard) {
		m_pDIKeyboard->Release();
		m_pDIKeyboard = 0;
	}

	if(m_pDIObj) {
		m_pDIObj->Release();
		m_pDIObj = 0;
	}

	DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_pDIObj,NULL);

	m_pDIObj->CreateDevice(GUID_SysKeyboard,&m_pDIKeyboard,NULL);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 
	m_pDIKeyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	m_pDIObj->CreateDevice(GUID_SysMouse,&m_pDIMouse,NULL);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);
	m_pDIMouse->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_EXCLUSIVE);
}

void InputHandler::shutdown() {
	if(m_pDIMouse) {
		m_pDIMouse->Release();
		m_pDIMouse = 0;
	}

	if(m_pDIKeyboard){
		m_pDIKeyboard->Release();
		m_pDIKeyboard = 0;
	}

	if(m_pDIObj) {
		m_pDIObj->Release();
		m_pDIObj = 0;
	}
}

int InputHandler::numGamePads() {
	DWORD numPads = 0;
	XINPUT_STATE xinputState;
	for(int i = 0;i<4;++i) {
		if(XInputGetState(i,&xinputState)==ERROR_SUCCESS) {
			++numPads;
		}
	}
	return numPads;
}
//return true if succeded
bool InputHandler::getState(int controllerNum,inputState& out) {
	XINPUT_STATE xInputState;
	float temp;
	if(controllerNum >= 0 && controllerNum < 4) {
		if(XInputGetState(controllerNum,&xInputState) == ERROR_SUCCESS) {
			//left thumb stick x
			temp = xInputState.Gamepad.sThumbLX;
			if(temp < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&temp > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				temp = 0;
			temp /= MAXSHORT;
			out.lX = temp;
			//left thumb stick y
			temp = xInputState.Gamepad.sThumbLY;
			if(temp < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&temp > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				temp = 0;
			temp /= MAXSHORT;
			out.lY = temp;
			//right thumb stick x
			temp = xInputState.Gamepad.sThumbRX;
			if(temp < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&temp > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				temp = 0;
			temp /= MAXSHORT;
			out.rX = temp;
			//right thump stick y
			temp = xInputState.Gamepad.sThumbRY;
			if(temp < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&temp > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				temp = 0;
			temp /= MAXSHORT;
			out.rY = temp;
			return true;
		}
		else
			return false;
	} else if(controllerNum == 4) {
		temp = mouseState.lX;
		temp /= MAXLONG;
		out.rX = temp;
		temp = mouseState.lY;
		temp /= MAXLONG;
		out.rY = temp;
		//check all binds
		if(getBoundKey(binds[binds::sprint]))
			temp = 1;
		else
			temp = 0.5f;
		//
		if(getBoundKey(binds[binds::forward])&0x80) {
			out.lY = temp;
		} else if(getBoundKey(binds[binds::back])&0x80) {
			out.lY = -temp;
		}
		//
		if(getBoundKey(binds[binds::right])&0x80) {
			out.lX = temp;
		}
		else if(getBoundKey(binds[binds::left])&0x80) {
			out.lX = -temp;
		}

		return true;
	}
	return false;
}

//retun false if failed
bool InputHandler::getState(int padNum,XINPUT_STATE& state) {
	if(XInputGetState(padNum,&state)==ERROR_SUCCESS) {
		return true;
	}
	return false;
}

void InputHandler::setVibration(int padNum,XINPUT_VIBRATION& state) {
	XInputSetState(padNum,&state);
}

void InputHandler::setVibration(int padNum,float leftSpeed,float rightSpeed) {
	XINPUT_VIBRATION state;
	state.wLeftMotorSpeed = 65535 * leftSpeed;
	state.wRightMotorSpeed = 65535 * rightSpeed;
	XInputSetState(padNum,&state);
}

void InputHandler::setVibration(int padNum,WORD leftSpeed,WORD rightSpeed) {
	XINPUT_VIBRATION state;
	state.wLeftMotorSpeed = leftSpeed;
	state.wRightMotorSpeed = rightSpeed;
	XInputSetState(padNum,&state);
}

void InputHandler::stopVibration(int padNum) {
	XINPUT_VIBRATION clear;
	clear.wLeftMotorSpeed = 0;
	clear.wRightMotorSpeed = 0;
	XInputSetState(padNum,&clear);
}

void InputHandler::update() {
	ZeroMemory(buffer,sizeof(buffer));
	ZeroMemory(&mouseState,sizeof(mouseState));
	if(m_pDIKeyboard->Acquire() != DIERR_INPUTLOST)
		m_pDIKeyboard->GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
	if(m_pDIMouse->Acquire()!= DIERR_INPUTLOST)
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2),&mouseState);
}

InputHandler::~InputHandler() {
	shutdown();
}