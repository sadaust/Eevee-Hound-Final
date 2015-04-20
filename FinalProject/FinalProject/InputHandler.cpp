#include "InputHandler.h"

InputHandler::InputHandler() {
	mouseOffSet = 256;
	m_pDIObj = 0;
	m_pDIKeyboard = 0;
	m_pDIMouse = 0;
	//trigger sensitivity
	fireTriggerPoint = 100;
	//set binds
	setBind(binds::jump,DIK_SPACE);
	setBind(binds::legPower,DIK_C);
	setBind(binds::use,DIK_E);
	setBind(binds::headPower,DIK_Q);
	setBind(binds::pause,DIK_ESCAPE);
	setBind(binds::leftAttack,mouseOffSet);
	setBind(binds::leftAltAttack,DIK_R);
	setBind(binds::rightAttack,mouseOffSet+1);
	setBind(binds::rightAltAttack,DIK_F);
	setBind(binds::forward,DIK_W);
	setBind(binds::back,DIK_S);
	setBind(binds::left,DIK_A);
	setBind(binds::right,DIK_D);
	setBind(binds::sprint,DIK_LSHIFT);
	setBind(binds::leftSelect,DIK_Z);
	setBind(binds::rightSelect,DIK_X);
	setBind(binds::leave,DIK_BACKSPACE);
	//set sensitivity
	for(int i = 0;i<4;++i) {
		sens[i].xSens = 2;
		sens[i].ySens = 2;
	}
	sens[4].xSens = 1;
	sens[4].ySens = -1;
}

bool InputHandler::setSens(int pNum, float xSens, float ySens) {
	if(pNum >= 0&&pNum<5) {
		sens[pNum].xSens = xSens;
		sens[pNum].ySens = ySens;
		return true;
	}
	return false;
}

void InputHandler::setBind(binds::bindList bin, int key) {
	binds[bin] = key;
}

bool InputHandler::getBoundKey(int num) {
	if(num < 256)
		return (buffer[num]&0x80);
	else {
		num -= 256;
		return (mouseState.rgbButtons[num]&0x80);
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

	for(int i = 0; i < 5;++i) {
		for(int z = 0; z < binds::last; ++z) {
			state[i].buttonLast[z] = false;
			state[i].buttons[z] = false;
		}
	}
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
	//clear input
	for(int i = 0; i < binds::last;++i)
		out.buttons[i] = false;
	out.lX = 0;
	out.lY = 0;
	out.rX = 0;
	out.rY = 0;
	for(int i = 0; i < binds::last; ++i) {
		out.buttonLast[i] = state[controllerNum].buttons[i];
	}
	//get input
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
			temp *= sens[controllerNum].xSens;
			out.rX = temp;
			//right thump stick y
			temp = xInputState.Gamepad.sThumbRY;
			if(temp < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&temp > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				temp = 0;
			temp /= MAXSHORT;
			temp *= sens[controllerNum].ySens;
			out.rY = temp;
			//set buttons
			//get jump
			out.buttons[binds::jump] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_A;
			//get legpower
			out.buttons[binds::legPower] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_B;
			//get use
			out.buttons[binds::use] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_X;
			//get Head power
			out.buttons[binds::headPower] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_Y;
			//get Pause
			out.buttons[binds::pause] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_START;
			//get left attack
			out.buttons[binds::leftAttack] = xInputState.Gamepad.bLeftTrigger >fireTriggerPoint;
			//get left alt attack
			out.buttons[binds::leftAltAttack] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER;
			//get right attack
			out.buttons[binds::rightAttack] = xInputState.Gamepad.bRightTrigger > fireTriggerPoint;
			//get right alt attack
			out.buttons[binds::rightAltAttack] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER;
			//get left select
			out.buttons[binds::leftSelect] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT;
			//get right select
			out.buttons[binds::rightSelect] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT;
			//get leave
			out.buttons[binds::leave] = xInputState.Gamepad.wButtons&XINPUT_GAMEPAD_BACK;
			state[controllerNum] = out;
			return true;
		}
		else
			return false;
	} else if(controllerNum == 4) {
		temp = mouseState.lX;
		temp *= sens[4].xSens;
		out.rX = temp;
		temp = mouseState.lY;
		temp *= sens[4].ySens;
		out.rY = temp;
		//check all binds
		if(getBoundKey(binds[binds::sprint]))
			temp = 0.5f;
		else
			temp = 1.0f;
		//
		if(getBoundKey(binds[binds::forward])) {
			out.lY = temp;
		} else if(getBoundKey(binds[binds::back])) {
			out.lY = -temp;
		}
		//
		if(getBoundKey(binds[binds::right])) {
			out.lX = temp;
		}
		else if(getBoundKey(binds[binds::left])) {
			out.lX = -temp;
		}
		for(int i = 0; i < binds::last; ++i) {
			out.buttons[i] = getBoundKey(binds[i]);
		}
		state[controllerNum] = out;
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