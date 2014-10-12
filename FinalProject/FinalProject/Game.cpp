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

	curState = server;

	cTime = timeGetTime();
	lTime = cTime;
	tTime = cTime;
	dt = 0;
	updatesPerSec = 600;

	mapSys.LoadMap("TestMap.txt",resMan);
	bullVec.Init(resMan);
	playVec.Init(mapSys,resMan);

	for(int i = 0; i < 4; ++i) {
		camera[i].drawDist = 200.0f;
		camera[i].fov_deg = 90.0f;
	}
	camera[0].cam_look_pos.x = 0;
	camera[0].cam_look_pos.y = 0;
	camera[0].cam_look_pos.z = 0;
	camera[0].cam_pos.x = 5;
	camera[0].cam_pos.y = 0;
	camera[0].cam_pos.z = 0;
	camera[0].cam_up_vec.x = 0;
	camera[0].cam_up_vec.y = 1;
	camera[0].cam_up_vec.z = 0;
	camera[1] = camera[0];
	camera[2] = camera[0];
	camera[3] = camera[0];

	/////////////////////////////////////////

	playVec.ActivateAPlayer(mapSys);


	/////////////////////////////////////////


}

bool Game::update() {
	//update dt
	cTime = timeGetTime();
	
	input.update();
	inputState iState;
	if(cTime>=tTime) {
		dt = (float)(cTime-lTime);
		dt /= CLOCKS_PER_SEC;
		lTime = cTime;
		if(curState == mainMenu) {
			//main menu
		} else if(curState == client) {
			//playing as client
		} else if(curState == server) {
			//player as host
			input.getState(0,iState);
			playVec.Update(iState, dt, partList, bullVec);
			bullVec.Update(dt);
			physSys.DoCollisions(playVec,bullVec,mapSys);
			

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
		
		playVec.Render(DXVid);	// draws players
		bullVec.Render(DXVid);	// draws bullets
		mapSys.render(DXVid);	// draws map
		for(int i = 0; i < 1; ++i) {
			camera[i].cam_look_pos = playVec.GetPlayer(i).getPos();
			DXVid.rotateCam(camera[i], 2,playVec.GetPlayer(i).getFacing(),playVec.GetPlayer(i).getAngle());
			DXVid.setCam(i+1,&camera[i]);
			hud[i].drawHud(playVec.GetPlayer(i),DXVid,i+1);
		}
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