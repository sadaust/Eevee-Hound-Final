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

	float height;
	float width;
	float depth;
};

struct TextureStruct {
	Texture objTex;
	TexInfo texInfo;

	D3DCOLOR mask;
};

struct PrimObj {
	PrimStruct* primInfo;
	TextureStruct* Tex;
	D3DMATERIAL9* mat;
	D3DXMATRIX matrix;
};

struct SpriteObj {
	TextureStruct* image;
	float posX, posY;
	float scalX, scalY;
};

struct TextStruct {
	RECT rec;
	std::string text;
	D3DCOLOR textColor;
};

struct Vertex {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};