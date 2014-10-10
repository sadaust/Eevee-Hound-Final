#include "ResourceManager.h"
#include "InputHandler.h"
#include "SoundFrame.h"
#include "Player.h"
#include "PhysicsSystem.h"
#include <sstream>
#include "Limbase.h"
#include "ItemBox.h"
#include "spawnPoint.h"
#include "HUD.h"

class DXTest {
private:
	HUD hud[4];
	cam temp,temp2,temp3,temp4;
	ResourceManager resMan;
	DXFrame DXVid;
	SoundFrame sFrame;
	PrimObj testCube,testCube2, testCube3, testCube4, testCube5,testCube6;
	PrimObj testPrimObjs[MAXBULLETS];
	SpriteObj testSprite;
	TextStruct testText,testText2;
	ModelObj testMod;
	D3DMATERIAL9 testMat;
	InputHandler input;
	listenProp tempProp;
	D3DLIGHT9 m_Light;
	std::stringstream ss,ss2;
	SoundStruct* testSound;
	MusicStruct* testMusic;
	Player testPlayer,testPlayer2;
	int numCon;
	DWORD cTime,lTime,tTime;
	float dist, distX, distZ, rot, angle, distX2, distZ2, distX3, distZ3;
	Terrain testTerrain, testTerrain2;
	PhysicsSystem testPhys;
	Bullet testBullet;
	BulletVec testBullVec;
	Limbase PartList;
	ItemBox itemDrop;
	sPoint spawn;
	float dist2, rot2, angle2;
	//dt in seconds
	float dt;
public:
	DXTest();
	bool devLost();
	void resetDev(HWND& hWnd,HINSTANCE& hInsts);
	void init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	void update();
	void draw();
	void resize(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	void shutDown();
	~DXTest();
};