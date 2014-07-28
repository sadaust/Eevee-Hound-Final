#include "ResourceManager.h"
#include "InputHandler.h"
#include "SoundFrame.h"
#include <sstream>

class DXTest {
private:
	cam temp,temp2,temp3,temp4;
	ResourceManager resMan;
	DXFrame DXVid;
	SoundFrame sFrame;
	PrimObj testCube;
	SpriteObj testSprite;
	TextStruct testText;
	D3DMATERIAL9 testMat;
	InputHandler input;
	listenProp tempProp;
	std::stringstream ss;
	SoundStruct* testSound;
	MusicStruct* testMusic;
	int numCon;
	DWORD cTime,lTime,tTime;
	float dist, rot, angle;
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