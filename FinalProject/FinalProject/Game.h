#pragma once

#include "ResourceManager.h"
#include "InputHandler.h"
#include "Map.h"
#include "SoundFrame.h"
#include "PhysicsSystem.h"
#include "Limbase.h"
#include "ItemBox.h"
#include "HUD.h"
#include "Map.h"
#include "Menu.h"

enum gameState {
	mainMenu,
	server,
	client
};

class Game{ 
private:
	gameState curState;
	ResourceManager resMan;
	DXFrame DXVid;
	InputHandler input;
	SoundFrame sFrame;
	Limbase partList;
	D3DLIGHT9 m_Light;

	//time
	float dt;
	int updatesPerSec;
	DWORD cTime,lTime,tTime;
public:
	bool devLost();
	void resetDev(HWND& hWnd,HINSTANCE& hInsts);
	void init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	bool update();
	void draw();
	void shutDown();
	~Game(){shutDown();}
};