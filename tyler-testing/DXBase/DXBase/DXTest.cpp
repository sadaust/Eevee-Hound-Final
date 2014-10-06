#include "DXTest.h"
#include <sstream>
#include <ctime>

DXTest::DXTest() {
	resMan.changeDevice(&DXVid);
	resMan.changeDevice(&sFrame);
}

void DXTest::init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	dist = 2;
	rot = 0;
	angle = 0;

	dist2 = 2;
	rot2 = 0;
	angle2 = 0;

	input.init(hWnd,hInst);
	sFrame.Init();
	tTime = cTime = lTime = GetTickCount();
	numCon = input.numGamePads();
	RenInfo tempRen;
	ss<<numCon;
	testText.text = ss.str();
	testText.textColor = D3DCOLOR(0xFFFFFFFF);
	testText.rec.top = 0;
	testText.rec.left = 0;
	testText.rec.right = 100;
	testText.rec.bottom = 100;
	tempRen.asset = &testText;
	tempRen.locCamNum = 5;
	tempRen.type = text;

	DXVid.init(hWnd,hInst,bWindowed);
	DXVid.addRen(tempRen);
	testCube.mat = &testMat;
	testMat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	testMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	testMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Emissive color reflected
	testMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// Specular
	testMat.Power = 0.0f;

	testMod.matrix = testCube.matrix;
	D3DXMatrixScaling(&testMod.matrix,0.01,0.01,0.01);
	testMod.mod = resMan.loadXFile("tiny.x");
	for(int i = 0;i< testMod.mod->numMats;++i) {
		testMod.mod->mats[i].MatD3D = testMat;
	}
	tempRen.type = model;
	tempRen.asset = &testMod;
	tempRen.locCamNum = 0;
	DXVid.addRen(tempRen);

	testCube.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(0,0,0),0);
	testCube.primInfo = resMan.loadPrim("CuberTest",1,1,1);
	D3DXMatrixIdentity(&testCube.matrix);
	testCube2 = testCube;
	tempRen.asset = &testCube;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube2;
	DXVid.addRen(tempRen);

	testSprite.image = resMan.loadTexture("xboxControllerSpriteFont.tga",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,0);
	testSprite.posX = 0;
	testSprite.posY = 0;
	testSprite.scalX = 0.1f;
	testSprite.scalY = 0.1f;
	tempRen.asset = &testSprite;
	tempRen.type = sprite;
	tempRen.locCamNum = 0;
	DXVid.addRen(tempRen);

	temp.cam_look_pos.x = 0;
	temp.cam_look_pos.y = 0;
	temp.cam_look_pos.z = 0;
	temp.cam_pos.x = 5;
	temp.cam_pos.y = 0;
	temp.cam_pos.z = 0;
	temp.cam_up_vec.x = 0;
	temp.cam_up_vec.y = 1;
	temp.cam_up_vec.z = 0;
	temp.drawDist = 200.0f;
	temp.fov_deg = 90.0f;

	temp2.cam_look_pos.x = 0;
	temp2.cam_look_pos.y = 0;
	temp2.cam_look_pos.z = 0;
	temp2.cam_pos.x = 5;
	temp2.cam_pos.y = 0;
	temp2.cam_pos.z = 0;
	temp2.cam_up_vec.x = 0;
	temp2.cam_up_vec.y = 1;
	temp2.cam_up_vec.z = 0;
	temp2.drawDist = 200.0f;
	temp2.fov_deg = 90.0f;

	temp3 = temp4 = temp;

	temp3.cam_pos.x = 0;
	temp3.cam_pos.z = -2;

	temp4.cam_pos.x = -2;

	tempProp.pos.x = temp.cam_pos.x;
	tempProp.pos.y = temp.cam_pos.y;
	tempProp.pos.z = temp.cam_pos.z;
	tempProp.up.x = temp.cam_up_vec.x;
	tempProp.up.y = temp.cam_up_vec.y;
	tempProp.up.z = temp.cam_up_vec.z;
	tempProp.vel.x = tempProp.vel.y = tempProp.vel.z = 0;
	tempProp.forward.x = tempProp.forward.y = tempProp.forward.z = 0;
	sFrame.setListenProp(0,tempProp);
	testSound = resMan.loadSound("plinkhit.wav",1,5,1);
	testMusic = resMan.loadMusic("battle.mp3",0.10f);
	sFrame.PlayStream(*testMusic,false);


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

	DXVid.setViewCount(2);
	DXVid.toggleSS();
	DXVid.setCam(1,&temp);
	DXVid.setCam(2,&temp2);
	DXVid.setCam(3,&temp3);
	DXVid.setCam(4,&temp4);
}

bool DXTest::devLost() {
	return DXVid.rendererLost();
}

void DXTest::resetDev(HWND& hWnd,HINSTANCE& hInsts) {
	DXVid.Shutdown();
	DXVid.init(hWnd,hInsts,true);
	if(!DXVid.rendererLost()) {
		temp.cam_look_pos.x = 0;
		temp.cam_look_pos.y = 0;
		temp.cam_look_pos.z = 0;
		temp.cam_pos.x = 5;
		temp.cam_pos.y = 0;
		temp.cam_pos.z = 0;
		temp.cam_up_vec.x = 0;
		temp.cam_up_vec.y = 1;
		temp.cam_up_vec.z = 0;
		temp.drawDist = 200.0f;
		temp.fov_deg = 90.0f;

		temp2.cam_look_pos.x = 0;
		temp2.cam_look_pos.y = 0;
		temp2.cam_look_pos.z = 0;
		temp2.cam_pos.x = 5;
		temp2.cam_pos.y = 0;
		temp2.cam_pos.z = 0;
		temp2.cam_up_vec.x = 0;
		temp2.cam_up_vec.y = 1;
		temp2.cam_up_vec.z = 0;
		temp2.drawDist = 200.0f;
		temp2.fov_deg = 90.0f;

		temp3 = temp4 = temp;

		temp3.cam_pos.x = 0;
		temp3.cam_pos.z = -2;

		temp4.cam_pos.x = -2;
		resMan.reloadAll();
		//DXVid.setViewCount(1);
		//DXVid.setViewCount(4);
		//DXVid.toggleSS();
		///DXVid.setCam(1,&temp);
		//DXVid.setCam(2,&temp2);
		//DXVid.setCam(3,&temp3);
		//DXVid.setCam(4,&temp4);
		//DXVid.clearRen();
	}
}

void DXTest::update() {
	XINPUT_STATE state;
	inputState iState;
	cTime = timeGetTime();
	dt = (float)(cTime-lTime);
	dt /= CLOCKS_PER_SEC;
	SHORT tem;
	input.update();
	sFrame.update();
	if(cTime>tTime) {
		for(int i = 0; i<numCon;++i) {
			input.getState(i,state);
			input.getState(i,iState);
			input.setVibration(i,(state.Gamepad.bLeftTrigger/255.0f),(state.Gamepad.bRightTrigger/255.0f));
			if(i == 0) {
				
				tem = state.Gamepad.sThumbLY;
				if(tem < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tem > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					tem = 0;
				
				dist += iState.lY*dt;
				D3DXMatrixTranslation(&testCube.matrix,0,0,dist);
				ss.str("");
				ss<<tem/32767.0f;
				testText.text = ss.str();
				/*
				tem = iState.rX;
				if(tem < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tem > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					tem = 0;*/
				rot += (iState.rX*dt)*10;
				tem = state.Gamepad.sThumbRY;
				if(tem < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tem > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					tem = 0;
				angle += (iState.rY*dt)*10;
				if(angle >= 90)
					angle = 89.9f;
				else if(angle <=-90)
					angle = -89.9f;
				DXVid.rotateCam(temp,2,rot,angle);
				if(iState.buttons[binds::jump])
					sFrame.Play(*testSound,dist,0,0,0,0,0);
				if(state.Gamepad.wButtons&XINPUT_GAMEPAD_B)
					DXVid.setSSvSplit(true);
				if(state.Gamepad.wButtons&XINPUT_GAMEPAD_Y)
					DXVid.setSSvSplit(false);
			}
			

		}
		DXVid.setCam(1,&temp);
		tem = 0;
		if(input.getState(4,iState)) {
			dist2 -= iState.lY*dt;
			tem = iState.rY;
			angle2 += iState.rY*dt;
			rot2 += iState.rX*dt;
			if(angle2 >= 90)
				angle2 = 89.9f;
			else if(angle2 <=-90)
				angle2 = -89.9f;
			DXVid.rotateCam(temp2,dist2,rot2,angle2);
			DXVid.setCam(2,&temp2);
		}
		
		tTime = cTime+(CLOCKS_PER_SEC/60);
		tempProp.pos.x = temp.cam_pos.x;
		tempProp.pos.y = temp.cam_pos.y;
		tempProp.pos.z = temp.cam_pos.z;
		sFrame.setListenProp(0,tempProp);
		lTime = cTime;
	}

}

void DXTest::draw() {
	RenInfo tempRen;
	if(DXVid.rendererLost()) {

	} else {
		DXVid.clearRen();

		tempRen.asset = &testCube;
		tempRen.locCamNum = 0;
		tempRen.type = primitive;

		DXVid.addRen(tempRen);
		tempRen.asset = &testCube2;
		DXVid.addRen(tempRen);

		tempRen.asset = &testSprite;
		tempRen.type = sprite;
		DXVid.addRen(tempRen);

		tempRen.asset = &testMod;
		tempRen.type = model;
		DXVid.addRen(tempRen);
		
		tempRen.asset = &testText;
		tempRen.type = text;
		tempRen.locCamNum = 2;
		DXVid.addRen(tempRen);

		DXVid.Render();
		//DXVid.clearRen();
	}
}

void DXTest::resize(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	DXVid.reSize(hWnd,hInst,bWindowed);
	resMan.reloadVideo();
}

void DXTest::shutDown() {
	input.shutdown();
	resMan.clear();
	sFrame.ShutDown();
	DXVid.Shutdown();
	for(int i = 0; i<numCon;++i) {
		input.stopVibration(i);
	}
}

DXTest::~DXTest() {
	shutDown();
}