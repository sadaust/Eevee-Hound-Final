#include "ResourceManager.h"
#include "InputHandler.h"
#include "SoundFrame.h"
#include "Player.h"
#include "PhysicsSystem.h"
#include <sstream>
#include "Limbase.h"

class DXTest {
private:
	cam temp,temp2,temp3,temp4;
	ResourceManager resMan;
	DXFrame DXVid;
	SoundFrame sFrame;
	PrimObj testCube,testCube2, testCube3, testCube4;
	PrimObj testPrimObjs[MAXBULLETS];
	SpriteObj testSprite;
	TextStruct testText;
	ModelObj testMod;
	D3DMATERIAL9 testMat;
	InputHandler input;
	listenProp tempProp;
	D3DLIGHT9 m_Light;
	std::stringstream ss;
	SoundStruct* testSound;
	MusicStruct* testMusic;
	Player testPlayer;
	int numCon;
	DWORD cTime,lTime,tTime;
	float dist, distX, distZ, rot, angle;
	Terrain testTerrain, testTerrain2;
	PhysicsSystem testPhys;
	Bullet testBullet;
	BulletVec testBullVec;
	Limbase PartList;
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