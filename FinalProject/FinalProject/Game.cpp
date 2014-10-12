#include "Game.h"
#include <ctime>
void Game::init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	DXVid.init(hWnd,hInst,bWindowed);
	sFrame.Init();
	resMan.changeDevice(&DXVid);
	resMan.changeDevice(&sFrame);
	input.init(hWnd,hInst);

	// Ambient light color emitted from this light
	m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Diffuse light color emitted from this light
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Specular light color emitted from this light
	m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// Light Type (Point) Requires: Position, Range, Attenuation
	m_Light.Type = D3DLIGHT_POINT;	// Point, alternatively D3DLIGHT_DIRECTIONAL or D3DLIGHT_SPOT
	// Light Position
	m_Light.Position = D3DXVECTOR3(30.0f, 10.0f, -10.0f); 
	// Range of Light
	m_Light.Range = 100.0f;
	// Light Attenuation
	m_Light.Attenuation0 = 0.0f;	// Constant
	m_Light.Attenuation1 = 0.05f;	// Linear
	m_Light.Attenuation2 = 0.0f;	// Quadratic

	// Set Light
	DXVid.setLight(0, &m_Light);
	//turn on light
	DXVid.setLightActive(0,true);

	curState = mainMenu;

	cTime = timeGetTime();
	lTime = cTime;
	tTime = cTime;
	dt = 0;
	updatesPerSec = 100;
}

bool Game::update() {
	//update dt
	cTime = timeGetTime();
	dt = (float)(cTime-lTime);
	dt /= CLOCKS_PER_SEC;
	lTime = cTime;
	if(cTime>=tTime) {
		if(curState == mainMenu) {
			//main menu
		} else if(curState == client) {
			//playing as client
		} else if(curState == server) {
			//player as host
		}
		draw();
		tTime = cTime+(CLOCKS_PER_SEC/updatesPerSec);
	}
	return true;
}

void Game::draw() {
	if(!DXVid.rendererLost()) {
		DXVid.clearRen();
		//add render code

		DXVid.Render();
	}
}

bool Game::devLost() {
	return DXVid.rendererLost();
}

void Game::resetDev(HWND& hWnd,HINSTANCE& hInsts) {
	DXVid.resetDev(hWnd,hInsts);
	resMan.reloadVideo();
}

void Game::shutDown() {
	input.shutdown();
	resMan.clear();
	sFrame.ShutDown();
	DXVid.Shutdown();
}