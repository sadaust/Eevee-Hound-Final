#pragma once

#pragma comment(lib, "winmm.lib")

//Direct 3D 9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


//render info vector data
#include "RenVector.h"
#include "ResDefs.h"

struct cam
{
	D3DXVECTOR3 cam_pos;
	D3DXVECTOR3 cam_look_pos;
	D3DXVECTOR3 cam_up_vec;
	float fov_deg;
	float drawDist;
};

struct view
{
	cam camera;
	D3DVIEWPORT9 viewPort;
};

class DXFrame
{
private:
	//window stuff
	HWND m_hWnd;
	HINSTANCE m_hInts;
	bool m_bVsync;
	bool showFPS;
	bool m_bFullScreen;
	bool m_bLost;
	bool m_bSplitScreen;
	bool m_vSplit;
	int w_width;
	int w_height;
	D3DXVECTOR2 vLine[2];
	D3DXVECTOR2 hLine[2];
	int fps;
	int waitTime;
	int numViewPorts;
	float dt,ltime,cTime,framCount;
	//render vector
	RenVector renVec;
	//DX stuff
	IDirect3D9* m_pD3DObject;
	IDirect3DDevice9* m_pD3DDevice;
	D3DCAPS9 m_D3DCaps;
	
	//font handler
	ID3DXFont* m_pD3DFont;
	//2D object handler
	ID3DXSprite* m_pD3DSprite;
	//lines
	LPD3DXLINE m_pD3DLine;
	//default viewport
	D3DVIEWPORT9 defaultView;
	//viewport array
	view* viewPorts;
public:
	DXFrame();
	~DXFrame();
	void rotateCam(cam& camra,float distance,float rot,float angle);
	void init(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	void reSize(HWND& hWnd, HINSTANCE& hInst,bool bWindowed);
	void setLight(int index, const D3DLIGHT9* light){if(m_pD3DDevice)m_pD3DDevice->SetLight(index,light);}
	void setLightActive(int index, bool active){if(m_pD3DDevice)m_pD3DDevice->LightEnable(index,active);}
	void Load2D(LPCSTR FileName,UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,D3DXIMAGE_INFO *SrcInfo,PALETTEENTRY *pPalette,LPDIRECT3DTEXTURE9 *Tex);
	void loadDX(LPCTSTR name,LPD3DXBUFFER* buffer,DWORD& numMat, LPD3DXMESH* mesh);
	void addRen(RenInfo&);
	void clearRen();
	void Shutdown();
	void Render();
	void toggleSS();
	void setSSvSplit(bool);
	void displayFPS(bool show) {showFPS = show;}
	void resetDev(HWND& hWnd,HINSTANCE& hInsts);
	void CreateUVCube(PrimStruct& obj,float height,float width,float depth);
	void loadXFile(LPCSTR name,ModelStruct& obj);
	void uncenterdUVCube(PrimStruct& obj,float bottom,float top,float left,float right,float front, float back);
	bool rendererLost(){return m_bLost;}
	bool setCam(int camNum,cam* camInfo);
	void zeroWaitTime(){waitTime = 0;}
	int getWaitTime(){return waitTime;}
	bool setViewCount(int numView);
};