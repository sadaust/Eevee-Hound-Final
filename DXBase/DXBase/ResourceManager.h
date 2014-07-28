#pragma once

#include "ResDefs.h"
#include "DXFrame.h"
#include "SoundFrame.h"
#include <vector>
#include <list>

enum resType {
	null,
	prim,
	texture,
	material,
	soundEffect,
	music
};

struct resStruct {
	LPCSTR name;
	resType type;
	void* res;
};

class ResourceManager {
private:
	DXFrame* loadDev;
	SoundFrame* soundDev; 
	std::list<PrimStruct> primList;
	std::list<TextureStruct> textureList;
	std::list<D3DMATERIAL9> matList;
	std::list<SoundStruct> soundList;
	std::list<MusicStruct> musicList;
	std::vector<resStruct> resources;
public:
	ResourceManager();
	ResourceManager(DXFrame*,SoundFrame*);
	void clear();
	void changeDevice(DXFrame*);
	void changeDevice(SoundFrame*);
	void reloadAll();
	void reload(LPSTR name);
	bool isResLoaded(LPSTR name);
	resStruct* getRes(LPSTR name);
	TextureStruct* loadTexture(LPCSTR name,UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,PALETTEENTRY *pPalette);
	PrimStruct* loadPrim(LPCSTR name,float height,float width,float depth);
	SoundStruct* loadSound(LPCSTR name,float minDist, float maxDist,float volume);
	MusicStruct* loadMusic(LPCSTR name,float volume);
	~ResourceManager();
};