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
#include "GameMode.h"


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
	ItemVec itemVec;
	Map mapSys;
	PhysicsSystem physSys;
	cam camera[4];
	cam defCam;
	HUD hud[4];
	GameMode gameRules;
	inputState iState[MAXPLAYERS];
	MusicStruct *backMusic;
	SpriteObj logo;
	SpriteObj smallLogo;
	//keep track of number of players
	int players, lPlayers;
	int pNum[5];
	//time
	double dt;
	int updatesPerSec;
	DWORD cTime,lTime,tTime, lastTime4Timer;
	int timer;
public:
	bool devLost();
	void resetDev(HWND& hWnd,HINSTANCE& hInsts);
	void init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	bool update();
	void draw();
	void shutDown();
	~Game(){shutDown();}
};