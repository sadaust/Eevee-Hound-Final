#include "DXTest.h"
#include <sstream>
#include <ctime>

DXTest::DXTest() {
	resMan.changeDevice(&DXVid);
	resMan.changeDevice(&sFrame);
}

void DXTest::init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed) {
	dist = 2;
	dist2 = 2;
	distX = 2;
	distZ = 0;
	rot = 0;
	angle = 0;
	dist2 = 2;
	angle2 = 0;
	distX2 = 2;
	distZ2 = 0;
	//second player stuff
	distX3=4;
	distZ3=0;
	rot2=0;
	angle2=0;
	


	input.init(hWnd,hInst);
	sFrame.Init();
	tTime = cTime = lTime = GetTickCount();
	numCon = input.numGamePads();
	RenInfo tempRen;
	ss<<numCon;
	testText.text = ss.str();
	testText.textColor = D3DCOLOR(0xFFFFFFFF);
	testText.rec.top = 700;
	testText.rec.left = 50;
	testText.rec.right = 100;
	testText.rec.bottom = 800;
	testText2 = testText;
	tempRen.asset = &testText;
	tempRen.locCamNum = 5;
	tempRen.type = text;
	Limbase PartList;
	
	DXVid.init(hWnd,hInst,bWindowed);
	DXVid.addRen(tempRen);
	testCube.mat = &testMat;
	testCube3.mat = &testMat;
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

	testCube.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	testCube3.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	testCube.primInfo = resMan.loadPrim("CuberTest",1,1,1);
	testCube3.primInfo = resMan.loadPrim("FloorTest",1,20,20);
	D3DXMatrixIdentity(&testCube.matrix);
	testCube2 = testCube;
	testCube4 = testCube;
	testCube5 = testCube;
	testCube6 = testCube;
	testCube2.primInfo = resMan.loadPrim("WallTest",20,1,1);
	testCube4.primInfo = resMan.loadPrim("BulletTest",0.1f,0.1f,0.1f);
	testCube5.primInfo = resMan.loadPrim("ItemTest",0.5f,0.5f,0.5f);
	testCube6.primInfo = resMan.loadPrim("CuberTest2",1,1,1);

	for(int i = 0; i < MAXBULLETS; ++i)
		testPrimObjs[i] = testCube4;
	tempRen.asset = &testCube;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube2;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube3;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube4;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube5;
	DXVid.addRen(tempRen);
	tempRen.asset = &testCube6;
	DXVid.addRen(tempRen);
	for(int i = 0; i < 256; ++i) {
		tempRen.asset = &testPrimObjs[i];
		DXVid.addRen(tempRen);
	}


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

	/*temp2.cam_look_pos.x = 0;
	temp2.cam_look_pos.y = 0;
	temp2.cam_look_pos.z = 0;
	temp2.cam_pos.x = 0;
	temp2.cam_pos.y = 5;
	temp2.cam_pos.z = 0;
	temp2.cam_up_vec.x = 0;
	temp2.cam_up_vec.y = 0;
	temp2.cam_up_vec.z = 1;
	temp2.drawDist = 200.0f;
	temp2.fov_deg = 90.0f;*/

	temp2 = temp3 = temp4 = temp;
	
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
	DXVid.setSSvSplit(true);
	DXVid.setCam(1,&temp);
	DXVid.setCam(2,&temp2);
	DXVid.setCam(3,&temp3);
	DXVid.setCam(4,&temp4);
	testPlayer.testInit(distX,0,distZ,rot);
	testPlayer2.testInit(distX3,0,distZ3,rot2);
	itemDrop.ItemBoxInit(distX2,5,distZ2);
	testTerrain.Init(D3DXVECTOR3(0,0,0),testCube3.primInfo, FLOOR);
	testTerrain2.Init(D3DXVECTOR3(2,5,2),testCube2.primInfo, WALL);
	testBullet.Init(D3DXVECTOR3(20,5,2),D3DXVECTOR3(-3,0,0),testCube4,0,0,RangedDefaultLifeSpan,1);
	testBullVec.Init(resMan);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4,0,0, RangedDefaultLifeSpan,1);
	//testBullVec.ActivateABullet(D3DXVECTOR3(10,4,2),D3DXVECTOR3(-2,0,0),testCube4,0,0, RangedDefaultLifeSpan,1);
	//testBullVec.ActivateABullet(D3DXVECTOR3(37,4,3),D3DXVECTOR3(-7,0,0),testCube4,0,0, RangedDefaultLifeSpan,1);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,-.2,0),testCube4,0,0, RangedDefaultLifeSpan,1);
	//testBullVec.ActivateABullet(D3DXVECTOR3(0,5,0),D3DXVECTOR3(1,-1,1),testCube4,0,0, RangedDefaultLifeSpan,1);

	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);
	//testBullVec.ActivateABullet(D3DXVECTOR3(19,5,2),D3DXVECTOR3(-3,0,0),testCube4.primInfo);

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

		/*temp2.cam_look_pos.x = 0;
		temp2.cam_look_pos.y = 0;
		temp2.cam_look_pos.z = 0;
		temp2.cam_pos.x = 5;
		temp2.cam_pos.y = 0;
		temp2.cam_pos.z = 0;
		temp2.cam_up_vec.x = 0;
		temp2.cam_up_vec.y = 1;
		temp2.cam_up_vec.z = 0;
		temp2.drawDist = 200.0f;
		temp2.fov_deg = 90.0f;*/

		temp2 = temp3 = temp4 = temp;
	
		temp3.cam_pos.x = 0;
		temp3.cam_pos.z = -2;
			
		temp4.cam_pos.x = -2;
		resMan.reloadAll();
		DXVid.setViewCount(2);
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
	SHORT tem = 0;
	SHORT tem2 = 0;
	input.update();
	sFrame.update();
	if(cTime>tTime) {
	for(int i = 0; i<numCon;++i) {
		input.getState(i,iState);
		//input.setVibration(i,(iState.buttons[binds::leftAttack]),(iState.buttons[binds::leftAttack])));
		//input.getState(4,iState);
		if(i == 0) {
			
			//tem = state.Gamepad.sThumbLY;
			//if(tem < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tem > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			//	tem = 0;
				
			dist += iState.lY*dt;
			//D3DXMatrixTranslation(&testCube.matrix,0,0,dist);
			
			tem = iState.lX;
			rot += (tem*dt)*10;
			tem = iState.lY;
			angle += (tem*dt)*10;
			if(angle >= 90)
				angle = 89.9f;
			else if(angle <=-90)
				angle = -89.9f;
			DXVid.rotateCam(temp,2,rot,angle);
			
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
			//	//sFrame.Play(*testSound,dist,0,0,0,0,0);
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_B)
			//	DXVid.setSSvSplit(true);
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_Y)
			//	DXVid.setSSvSplit(false);

			  ////////////////////////////////
			 // Player test stuff ~~~ Josh //
			////////////////////////////////
			testPlayer.Update(iState, dt,rot, angle,PartList);
			//testPlayer2.Update(iState, dt, rot2, angle, PartList);
			itemDrop.Update(dt);
			testBullet.Update(dt);
			testBullVec.Update(dt);
			if(testPhys.SenseCollision(testPlayer,testTerrain)) {
				testPhys.ResolveCollision(testPlayer,testTerrain);
			}
			if(testPhys.SenseCollision(testPlayer,testTerrain2)) {
				testPhys.ResolveCollision(testPlayer,testTerrain2);
			}
			if(testPhys.SenseCollision(testTerrain,testBullet)) {
				testPhys.ResolveCollision(testTerrain,testBullet);
			}
			if(testPhys.SenseCollision(testTerrain2,testBullet)) {
				testPhys.ResolveCollision(testTerrain2,testBullet);
			}
			if(testPhys.SenseCollision(testPlayer,testBullet)) {
				testPhys.ResolveCollision(testPlayer,testBullet);
			}
			if(testPhys.SenseCollision(itemDrop,testTerrain)) {
				testPhys.ResolveCollision(itemDrop,testTerrain);
			}
			if(testPhys.SenseCollision(itemDrop,testTerrain2)) {
				testPhys.ResolveCollision(itemDrop,testTerrain2);
			}
			if(testPhys.SenseCollision(testPlayer,itemDrop)) {
				//setplayer item collision to true
				testPlayer.togglecheckItem(true);
				//pass colliding item to player test to see if passing works.
				testPlayer.itemAccess(itemDrop);
			 
			}
			if(!testPhys.SenseCollision(testPlayer,itemDrop)){
				//setplayer item collision to false
				testPlayer.togglecheckItem(false);
			}
			

			for(int i = 0; i < MAXBULLETS; ++i) {
				if(testBullVec.GetActive(i)) {
					if(testPhys.SenseCollision(testTerrain,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testTerrain, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
					if(testPhys.SenseCollision(testTerrain2,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testTerrain2, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
					if(testPhys.SenseCollision(testPlayer,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testPlayer, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
					if(testPhys.SenseCollision(testPlayer2,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testPlayer2, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
				}
			}
			//temp.cam_look_pos.x = 0;
			//temp.cam_look_pos.y = 0;
			
			
			



			///*D3DXMatrixTranslation(&testCube.matrix,0,0,distX);*/
			
			temp.cam_look_pos.x = testPlayer.getPos().x;
			temp.cam_look_pos.y = testPlayer.getPos().y;
			temp.cam_look_pos.z = testPlayer.getPos().z;
			DXVid.rotateCam(temp,2,rot,angle);
			////////////////////////////////
			if(testPlayer.isAlive())  {
			if(iState.buttons[binds::leftAttack]&&!iState.buttonLast[binds::leftAttack]) { 
				D3DXVECTOR3 tempvec = testPlayer.getPos();
				tempvec.y += 1.5f;
				testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),0,rot,angle,RangedDefaultLifeSpan, testDamage);
			}
			if(iState.buttons[binds::rightAttack]&&!iState.buttonLast[binds::rightAttack]) { 
				D3DXVECTOR3 tempvec = testPlayer.getPos();
				tempvec.y += 1.5f;
				testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),0,rot,angle,MeleeDefaultLifeSpan, testDamage);
			}
			
			} else {
				if(testPlayer.getTimer()<=0) {
					testPlayer.respawn(spawn);
					rot = spawn.getRot();
				}
			}
		}
		if(i == 1) {
			
			//tem = state.Gamepad.sThumbLY;
			//if(tem < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tem > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			//	tem = 0;
				
			dist2 += iState.lY*dt;
			//D3DXMatrixTranslation(&testCube.matrix,0,0,dist);

			tem2 = iState.lX;
			rot2 += (tem2*dt)*10;
			tem2 = iState.lY;
			angle2 += (tem2*dt)*10;
			if(angle2 >= 90)
				angle2 = 89.9f;
			else if(angle2 <=-90)
				angle2 = -89.9f;
			DXVid.rotateCam(temp2,2,rot2,angle2);
			
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
			//	//sFrame.Play(*testSound,dist,0,0,0,0,0);
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_B)
			//	DXVid.setSSvSplit(true);
			//if(state.Gamepad.wButtons&XINPUT_GAMEPAD_Y)
			//	DXVid.setSSvSplit(false);

			  ////////////////////////////////
			 // Player test stuff ~~~ Josh //
			////////////////////////////////
			testPlayer2.Update(iState, dt,rot2, angle2,PartList);
			//testPlayer2.Update(iState, dt, rot2, angle, PartList);
			itemDrop.Update(dt);
			testBullet.Update(dt);
			testBullVec.Update(dt);
			if(testPhys.SenseCollision(testPlayer2,testTerrain)) {
				testPhys.ResolveCollision(testPlayer2,testTerrain);
			}
			if(testPhys.SenseCollision(testPlayer2,testTerrain2)) {
				testPhys.ResolveCollision(testPlayer2,testTerrain2);
			}
			if(testPhys.SenseCollision(testTerrain,testBullet)) {
				testPhys.ResolveCollision(testTerrain,testBullet);
			}
			if(testPhys.SenseCollision(testTerrain2,testBullet)) {
				testPhys.ResolveCollision(testTerrain2,testBullet);
			}
			if(testPhys.SenseCollision(testPlayer2,testBullet)) {
				testPhys.ResolveCollision(testPlayer2,testBullet);
			}
			if(testPhys.SenseCollision(itemDrop,testTerrain)) {
				testPhys.ResolveCollision(itemDrop,testTerrain);
			}
			if(testPhys.SenseCollision(itemDrop,testTerrain2)) {
				testPhys.ResolveCollision(itemDrop,testTerrain2);
			}
			if(testPhys.SenseCollision(testPlayer2,itemDrop)) {
				//setplayer item collision to true
				testPlayer2.togglecheckItem(true);
				//pass colliding item to player test to see if passing works.
				testPlayer.itemAccess(itemDrop);
			 
			}
			if(!testPhys.SenseCollision(testPlayer2,itemDrop)){
				//setplayer item collision to false
				testPlayer2.togglecheckItem(false);
			}
			

			for(int i = 0; i < MAXBULLETS; ++i) {
				if(testBullVec.GetActive(i)) {
					if(testPhys.SenseCollision(testTerrain,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testTerrain, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
					if(testPhys.SenseCollision(testTerrain2,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testTerrain2, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
					if(testPhys.SenseCollision(testPlayer,testBullVec.GetBullet(i))) {
						testPhys.ResolveCollision(testPlayer, testBullVec.GetBullet(i));
						testBullVec.DeactivateABullet(i);
					}
				}
			}
			//temp.cam_look_pos.x = 0;
			//temp.cam_look_pos.y = 0;
			
			
			



			///*D3DXMatrixTranslation(&testCube.matrix,0,0,distX);*/

			temp2.cam_look_pos.x = testPlayer2.getPos().x;
			temp2.cam_look_pos.y = testPlayer2.getPos().y;
			temp2.cam_look_pos.z = testPlayer2.getPos().z;
			DXVid.rotateCam(temp2,2,rot2,angle2);
			////////////////////////////////
			if(testPlayer2.isAlive()) {
				if(iState.buttons[binds::leftAttack]&&!iState.buttonLast[binds::leftAttack]) { 
					D3DXVECTOR3 tempvec = testPlayer2.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),0,rot2,angle2,RangedDefaultLifeSpan, testDamage);
				}
				if(iState.buttons[binds::rightAttack]&&!iState.buttonLast[binds::rightAttack]) { 
					D3DXVECTOR3 tempvec = testPlayer2.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),0,rot2,angle2,MeleeDefaultLifeSpan, testDamage);
				}
			} else {
				if(testPlayer2.getTimer()<=0) {
					testPlayer2.respawn(spawn);
					rot2 = spawn.getRot();
				}
			}
		}
		tTime = cTime+(CLOCKS_PER_SEC/60);
		}
	tempProp.pos.x = temp.cam_pos.x;
	tempProp.pos.y = temp.cam_pos.y;
	tempProp.pos.z = temp.cam_pos.z;
	sFrame.setListenProp(0,tempProp);
	DXVid.setCam(1,&temp);
	DXVid.setCam(2,&temp2);
	lTime = cTime;
	}
	
}

void DXTest::draw() {
	RenInfo tempRen;
	D3DXMATRIX TransMat, RotMat;
	if(DXVid.rendererLost()) {

	} else {
		DXVid.clearRen();
		

		  /////////////////////////////////////////////////////// ~~~~~~~~~~~~~~~~~~
		 // START OF PRIMITIVE DRAWING                        // ~~~~~~~~~~~~~~~~~~  A lot of this is pretty temporary until we just have all of the 
		/////////////////////////////////////////////////////// ~~~~~~~~~~~~~~~~~~   players in a for loop or a draw function, along with all the bullets, and the map, etc.


		TransMat = testCube.matrix;
		RotMat = testCube.matrix; // translates and rotates the testCube which is what draws where the player is
		D3DXMatrixRotationY(&RotMat,testPlayer.getFacing());
		D3DXMatrixTranslation(&TransMat,testPlayer.getPos().x,testPlayer.getPos().y,testPlayer.getPos().z);
		D3DXMatrixMultiply(&testCube.matrix,&RotMat, &TransMat);

		TransMat = testCube6.matrix;
		RotMat = testCube6.matrix; // translates and rotates the testCube which is what draws where the player is
		D3DXMatrixRotationY(&RotMat,testPlayer2.getFacing());
		D3DXMatrixTranslation(&TransMat,testPlayer2.getPos().x,testPlayer2.getPos().y,testPlayer2.getPos().z);
		D3DXMatrixMultiply(&testCube6.matrix,&RotMat, &TransMat);
		

		// translates the three cubes(testcube2,3,4) that draw where the floor is, the giant pillar is, and where the first test bullet is
		D3DXMatrixTranslation(&testCube2.matrix,testTerrain2.getPos().x,testTerrain2.getPos().y-0.5f,testTerrain2.getPos().z);
		D3DXMatrixTranslation(&testCube3.matrix,testTerrain.getPos().x,testTerrain.getPos().y-0.5f,testTerrain.getPos().z);
		D3DXMatrixTranslation(&testCube4.matrix,testBullet.getPos().x,testBullet.getPos().y-0.5f,testBullet.getPos().z);
		D3DXMatrixTranslation(&testCube5.matrix,itemDrop.getPos().x,itemDrop.getPos().y-0.25f,itemDrop.getPos().z);





		// adds all the prims
		tempRen.type = primitive;
		tempRen.locCamNum = 0;
		//p1
		if(testPlayer.isAlive()){
			tempRen.asset = &testCube;
			DXVid.addRen(tempRen);
		}
		tempRen.asset = &testCube2;
		DXVid.addRen(tempRen);
		tempRen.asset = &testCube3;
		DXVid.addRen(tempRen);
		tempRen.asset = &testCube4;
		DXVid.addRen(tempRen);
		//p2
		if(testPlayer2.isAlive()) {
			tempRen.asset = &testCube6;
			DXVid.addRen(tempRen);
		}

		if(itemDrop.getActive()){
			tempRen.asset = &testCube5;
			DXVid.addRen(tempRen);
		}


		  ////////////////////////////////////////////////////////////////////////////
		 // draws(translates and adds) the whole bullvec only if they're active    //
		////////////////////////////////////////////////////////////////////////////
		testBullVec.Render(DXVid);
		  ///////////////////////////////////////////////////////  ~~~~~~~~~~~~~~~~~~~~~~
		 // done drawing primitives                           //  ~~~~~~~~~~~~~~~~~~~~~~
		///////////////////////////////////////////////////////  ~~~~~~~~~~~~~~~~~~~~~~
		tempRen.asset = &testSprite;
		tempRen.type = sprite;
		DXVid.addRen(tempRen);

		tempRen.asset = &testMod;
		tempRen.type = model;
		DXVid.addRen(tempRen);
		
		//text
		testText.textColor = D3DCOLOR(0xFFFFFFFF);
		testText.rec.top = 500;
		testText.rec.left = 50;
		testText.rec.right = 100;
		testText.rec.bottom = 600;
		testText2 = testText;

		ss.str("");
		ss<<testPlayer.getHealth();
		testText.text = ss.str();
		tempRen.asset = &testText;
		tempRen.type = text;
		tempRen.locCamNum = 1;
		DXVid.addRen(tempRen);

		ss.str("");
		ss<<testPlayer2.getHealth();
		testText2.text = ss.str();
		tempRen.asset = &testText2;
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