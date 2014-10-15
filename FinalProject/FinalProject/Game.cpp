#include "Game.h"
#include <ctime>
void Game::init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	srand(timeGetTime());
	DXVid.init(hWnd,hInst,bWindowed);
	sFrame.Init();
	resMan.changeDevice(&DXVid);
	resMan.changeDevice(&sFrame);
	input.init(hWnd,hInst);

	//init small logo for filling 4th inactive screen
	smallLogo.image = resMan.loadTexture("el.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,255,255),0);
	
	smallLogo.scalX = 0.5f;
	smallLogo.scalY = 0.5f;

	smallLogo.posX = 0.2f;
	smallLogo.posY = 0.1f;
	//init full logo for filling screen when no players

	logo.image = resMan.loadTexture("Elimbination.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,255,255),0);
	
	logo.scalX = 0.3f;
	logo.scalY = 0.3f;

	logo.posX = 0.1f;
	logo.posY = 0.2f;
	// Ambient light color emitted from this light
	m_Light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	// Diffuse light color emitted from this light
	m_Light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	// Specular light color emitted from this light
	m_Light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	// Light Type (Point) Requires: Position, Range, Attenuation
	m_Light.Type = D3DLIGHT_SPOT;	// Point, alternatively D3DLIGHT_DIRECTIONAL or D3DLIGHT_SPOT
	// Light Position
	m_Light.Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f); 
	m_Light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0);
	// Range of Light
	m_Light.Range = 1000.0f;
	// Light Attenuation
	m_Light.Attenuation0 = 0.0f;	// Constant
	m_Light.Attenuation1 = 0.05f;	// Linear
	m_Light.Attenuation2 = 0.0f;	// Quadratic
	//
	m_Light.Phi = D3DXToRadian(90.0f);
	m_Light.Theta = m_Light.Phi;
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
	itemVec.Init(mapSys,resMan);

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
	defCam = camera[0];

	//tempProp.pos.x = temp.cam_pos.x;
	//tempProp.pos.y = temp.cam_pos.y;
	//tempProp.pos.z = temp.cam_pos.z;
	//tempProp.up.x = temp.cam_up_vec.x;
	//tempProp.up.y = temp.cam_up_vec.y;
	//tempProp.up.z = temp.cam_up_vec.z;
	//tempProp.vel.x = tempProp.vel.y = tempProp.vel.z = 0;
	//tempProp.forward.x = tempProp.forward.y = tempProp.forward.z = 0;
	//sFrame.setListenProp(0,tempProp);
	//testSound = resMan.loadSound("plinkhit.wav",1,5,1);
	backMusic = resMan.loadMusic("battle.mp3",0.10f);
	sFrame.PlayStream(*backMusic,false);

	/////////////////////////////////////////
	players = 0;
	for(int i = 0; i < 5; ++i) {
		pNum[i] = -1;
	}
	/*
	for(int i = 0; i < 5; ++i) {
		if(input.getState(i,iState[i])&&players<4) {
			pNum[i] = players;
			++players;
			playVec.ActivateAPlayer(mapSys);
		}
	}*/
	for(int i = 0; i < numStartingItems; ++i)
		itemVec.ActivateAItem(mapSys,partList);


	/////////////////////////////////////////
	DXVid.setViewCount(players);
	DXVid.toggleSS();
	gameRules.intit(&mapSys,&DXVid,1,1);
	gameRules.Start();
	DXVid.displayFPS(false);
	lastTime4Timer = timeGetTime();
	timer = time2Item;
}

bool Game::update() {
	inputState iTemp;
	int firstCon = 0;
	//update dt
	cTime = timeGetTime();
	
	input.update();


	if(cTime >= lastTime4Timer+1000) {
		--timer;
		lastTime4Timer = timeGetTime();
	}
	if(timer <= 0) {
		timer = time2Item;
		itemVec.ActivateAItem(mapSys, partList);
	}



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
			//clear inputs
			for(int i = 0; i < MAXPLAYERS; ++i) {
				input.getState(-1,iState[i]);
			}
			//set inputs
			for(int i = 0; i < 5; ++i) {
				if(pNum[i] >= 0) {
					if(!input.getState(i,iState[pNum[i]])||iState[pNum[i]].buttons[binds::leave]) {
						playVec.DeactivateAPlayer(pNum[i]);
						pNum[i] = -1;
						--players;
						if(players<=0)
							gameRules.Restart(playVec);
					}
				} else if(players<4&&input.getState(i,iTemp)){
					if(iTemp.buttons[binds::pause]) {
						for(int z = 0; z < 5; ++z) {
							if(pNum[z] == firstCon) {
								++firstCon;
								z = 0;
							}
						}
						pNum[i] = firstCon;
						playVec.ActivateAPlayer(mapSys);
						++players;
					}
				}
			}
			if(players > 0) {
			playVec.Update(iState, dt, partList, bullVec,itemVec,resMan,&sFrame);
			bullVec.Update(dt);
			itemVec.Update(iState,dt,partList);
			physSys.DoCollisions(playVec,bullVec,mapSys, itemVec);
			gameRules.update(dt,playVec);
			}
			sFrame.update();
		}
		draw();
		tTime = cTime+(CLOCKS_PER_SEC/updatesPerSec);
	}
	return true;
}

void Game::draw() {
	int renNum = 0;
	RenInfo tempRen;
	if(!DXVid.rendererLost()) {
		DXVid.clearRen();
		//add render code
		switch(players) {
		case 0:
			tempRen.asset = &logo;
			tempRen.type = sprite;
			tempRen.locCamNum = 1;
			DXVid.setCam(1,&defCam);
			DXVid.addRen(tempRen);
		case 1:
			DXVid.setViewCount(1);

			break;
		case 2:
			DXVid.setViewCount(2);
			break;
		case 3:
			tempRen.asset = &smallLogo;
			tempRen.type = sprite;
			tempRen.locCamNum = 4;
			DXVid.addRen(tempRen);
		case 4:
			DXVid.setViewCount(4);
			break;
		}
		if(players > 0) {
		m_Light.Phi = D3DXToRadian(gameRules.getLight());
		m_Light.Theta = D3DXToRadian(gameRules.getLight()+20);
		DXVid.setLight(0,&m_Light);
		playVec.Render(DXVid,resMan);	// draws players
		bullVec.Render(DXVid);	// draws bullets
		mapSys.render(DXVid);	// draws map
		itemVec.Render(DXVid);  // draws items
		for(int i = 0; i < 5; ++i) {
			if(pNum[i] >= 0) {
				camera[renNum].cam_look_pos = playVec.GetPlayer(pNum[i]).getPos();
				camera[renNum].cam_look_pos.y = playVec.GetPlayer(pNum[i]).getPos().y+1;
				DXVid.rotateCam(camera[renNum], 2,playVec.GetPlayer(pNum[i]).getFacing(),playVec.GetPlayer(pNum[i]).getAngle());
				DXVid.setCam(renNum+1,&camera[renNum]);
				hud[renNum].drawHud(playVec.GetPlayer(pNum[i]),DXVid,gameRules,renNum+1);
				++renNum;
			}
		}
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