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
	testMat.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	testMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	testMat.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Emissive color reflected
	testMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Specular
	testMat.Power = 0.0f;
	testCube.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,0),0);
	testCube.primInfo = resMan.loadPrim("CuberTest",1,1,1);
	D3DXMatrixIdentity(&testCube.matrix);
	tempRen.asset = &testCube;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;
	DXVid.addRen(tempRen);

	testSprite.image = resMan.loadTexture("xboxControllerSpriteFont.tga",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),0);
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
	temp2.cam_pos.x = 0;
	temp2.cam_pos.y = 0;
	temp2.cam_pos.z = 5;
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
	testMusic = resMan.loadMusic("battle.mp3",0.100);
	sFrame.PlayStream(*testMusic,false);

	DXVid.setViewCount(4);
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
		temp2.cam_pos.x = 0;
		temp2.cam_pos.y = 0;
		temp2.cam_pos.z = 5;
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
	cTime = timeGetTime();
	dt = (float)(cTime-lTime);
	dt /= CLOCKS_PER_SEC;
	SHORT tem;
	input.update();
	sFrame.update();
	if(cTime>tTime) {
	for(int i = 0; i<numCon;++i) {
		
		input.getState(i,state);
		input.setVibration(i,(state.Gamepad.bLeftTrigger/255.0f),(state.Gamepad.bRightTrigger/255.0f));
		if(i == 0) {
			tem = state.Gamepad.sThumbLY;
			if(tem < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tem > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
				tem = 0;
			dist += (tem/32767.0f)*dt;
			D3DXMatrixTranslation(&testCube.matrix,dist,0,0);
			ss.str("");
			ss<<tem/32767.0f;
			testText.text = ss.str();
			
			tem = state.Gamepad.sThumbRX;
			if(tem < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tem > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				tem = 0;
			rot += ((tem/32767.0f)*dt)*10;
			tem = state.Gamepad.sThumbRY;
			if(tem < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tem > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				tem = 0;
			angle += ((tem/32767.0f)*dt)*10;
			if(angle >= 90)
				angle = 89.9;
			else if(angle <=-90)
				angle = -89.9;
			DXVid.rotateCam(temp,2,rot,angle);
			if(state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
				sFrame.Play(*testSound,dist,0,0,0,0,0);
		}
		tTime = cTime+(CLOCKS_PER_SEC/60);
		}
	tempProp.pos.x = temp.cam_pos.x;
	tempProp.pos.y = temp.cam_pos.y;
	tempProp.pos.z = temp.cam_pos.z;
	sFrame.setListenProp(0,tempProp);
	DXVid.setCam(1,&temp);
	lTime = cTime;
	}
	
}

void DXTest::draw() {
	RenInfo tempRen;
	if(DXVid.rendererLost()) {

	} else {
		DXVid.Render();
		//DXVid.clearRen();
	}
}

void DXTest::resize(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	DXVid.reSize(hWnd,hInst,bWindowed);
	resMan.reloadAll();
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