#include "InputHandler.h"

InputHandler::InputHandler() {
	m_pDIObj = 0;
	m_pDIKeyboard = 0;
	m_pDIMouse = 0;
}

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
	char buffer[256];
	DIMOUSESTATE2 mouseState;
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