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
#include "Player.h"
#include "Bullet.h"


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
	PlayerVec playVec;
	BulletVec bullVec;
	Map mapSys;
	PhysicsSystem physSys;
	cam camera[4];

	//time
	double dt;
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