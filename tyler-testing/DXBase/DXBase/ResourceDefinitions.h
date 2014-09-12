#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define Texture IDirect3DTexture9*
#define TexInfo D3DXIMAGE_INFO

struct PrimObj
{
	IDirect3DVertexBuffer9* obj;
	IDirect3DIndexBuffer9* objInd;
	IDirect3DVertexDeclaration9* objDec;
	D3DMATERIAL9* objMat;
	Texture objTex;
	D3DXMATRIX matrix;
	int numVerts;
	int numPrim;
};

struct TextureStruct
{
	LPCWSTR name;
	Texture objTex;
	TexInfo texInfo;
};

struct SpriteStruct
{
	Texture tex;
	TexInfo texinfo;
	D3DXMATRIX matrix;
};

struct TextStruct
{
	RECT rec;
	LPCSTR text;
	D3DCOLOR textColor;
};

struct Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};