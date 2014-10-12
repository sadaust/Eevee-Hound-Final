#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define Texture IDirect3DTexture9*
#define TexInfo D3DXIMAGE_INFO

#include <fmod.hpp>
#pragma comment(lib,"Fmodex_vc.lib")
//sound for sound system
#define Sound FMOD::Sound*

struct PrimStruct {
	IDirect3DVertexBuffer9* obj;
	IDirect3DIndexBuffer9* objInd;
	IDirect3DVertexDeclaration9* objDec;
	int numVerts;
	int numPrim;

	float bottom;
	float top;
	float left;
	float right;
	float front;
	float back;
};

struct TextureStruct {
	Texture objTex;
	TexInfo texInfo;

	D3DCOLOR mask;
};

struct ModelStruct {
	ID3DXMesh* mesh;
	ID3DXBuffer* adj;
	ID3DXBuffer* matPoint;
	D3DXMATERIAL* mats;
	IDirect3DVertexDeclaration9* dec;
	DWORD numMats;
	TextureStruct** textures; 
};

struct SoundStruct {
	Sound sound;
	float minDist;
	float maxDist;
	float volume;
};

struct MusicStruct {
	Sound sound;
	float volume;
};

struct PrimObj {
	PrimStruct* primInfo;
	TextureStruct* Tex;
	D3DMATERIAL9* mat;
	D3DXMATRIX matrix;
};

struct ModelObj {
	ModelStruct* mod;
	D3DXMATRIX matrix;
};

struct SpriteObj {
	TextureStruct* image;
	float posX, posY;
	float scalX, scalY;
};

struct frect {
	float top;
	float bottom;
	float left;
	float right;
};

struct TextStruct {
	frect rec;
	std::string text;
	D3DCOLOR textColor;
};

struct Vertex {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};