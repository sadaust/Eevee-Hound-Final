#include "ResourceManager.h"
#include "SoundFrame.h"

ResourceManager::ResourceManager() {
	loadDev = 0;
}

ResourceManager::ResourceManager(DXFrame* frame,SoundFrame* sFrame) {
	loadDev = frame;
	soundDev = sFrame;
}

ResourceManager::~ResourceManager() {
	clear();
}

void ResourceManager::clear() {
	//clear all textures
	TextureStruct* tempText;
	while(textureList.size() > 0) {
		tempText = &textureList.back();
		tempText->objTex->Release();
		tempText->objTex = 0;
		textureList.pop_back();
	}
	//clear all primitives
	PrimStruct* tempPrim;
	while(primList.size() > 0) {
		tempPrim = &primList.back();
		tempPrim->obj->Release();
		tempPrim->obj = 0;
		tempPrim->objDec->Release();
		tempPrim->obj = 0;
		tempPrim->objInd->Release();
		tempPrim->objInd = 0;
		primList.pop_back();
	}
	//clear all sounds
	SoundStruct* tempSound;
	while(soundList.size() > 0) {
		tempSound = &soundList.back();
		tempSound->sound->release();
		tempSound->sound = 0;
		soundList.pop_back();
	}
	//clear all music
	MusicStruct* tempMusic;
	while(musicList.size() > 0) {
		tempMusic = &musicList.back();
		tempMusic->sound->release();
		musicList.pop_back();
	}
	//clear all materials
	matList.clear();
	//clear resources stucts
	resources.clear();
}

void ResourceManager::reloadAll() {
	TextureStruct* tempTex;
	PrimStruct* tempPrim;
	for(int i = 0;i<resources.size();++i) {
		switch(resources[i].type) {
		case prim:
			tempPrim = (PrimStruct*)resources[i].res;
			//release
			tempPrim->obj->Release();
			tempPrim->obj = 0;
			tempPrim->objInd->Release();
			tempPrim->objInd = 0;
			tempPrim->objDec->Release();
			tempPrim->objDec = 0;
			//reload
			loadDev->CreateUVCube(*tempPrim,tempPrim->height,tempPrim->width,tempPrim->depth);
			break;
		case texture:
			tempTex = (TextureStruct*)resources[i].res;
			//release
			tempTex->objTex->Release();
			tempTex->objTex = 0;
			//reload
			loadDev->Load2D(resources[i].name,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,tempTex->mask,&tempTex->texInfo,0,&tempTex->objTex);
			break;
		default:
			break;
		}
	}
}

void ResourceManager::reload(LPSTR name) {
	resStruct* tempRes;
	TextureStruct* tempTex;
	PrimStruct* tempPrim;
	tempRes = getRes(name);
	if(tempRes) {
		switch(tempRes->type) {
		case prim:
			tempPrim = (PrimStruct*) tempRes->res;
			//release
			tempPrim->obj->Release();
			tempPrim->obj = 0;
			tempPrim->objDec->Release();
			tempPrim->obj = 0;
			tempPrim->objInd->Release();
			tempPrim->objInd = 0;
			//reload
			loadDev->CreateUVCube(*tempPrim,tempPrim->height,tempPrim->width,tempPrim->depth);
			break;
		case texture:
			tempTex = (TextureStruct*) tempRes->res;
			//release
			tempTex->objTex->Release();
			tempTex->objTex = 0;
			//reload
			loadDev->Load2D(tempRes->name,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,tempTex->mask,&tempTex->texInfo,0,&tempTex->objTex);
			break;
		default:
			break;
		}
	}
}

void ResourceManager::changeDevice(DXFrame* frame) {
	loadDev = frame;
}

void ResourceManager::changeDevice(SoundFrame* sFrame) {
	soundDev = sFrame;
}

bool ResourceManager::isResLoaded(LPSTR name) {
	for(int i = 0; i < resources.size();++i) {
		if(name == resources[i].name)
			return true;
	}
	return false;
}

resStruct* ResourceManager::getRes(LPSTR name) {
	for(int i = 0; i < resources.size();++i) {
		if(name == resources[i].name)
			return &resources[i];
	}
	return 0;
}

TextureStruct* ResourceManager::loadTexture(LPCSTR name,UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,DWORD Filter,DWORD MipFilter,D3DCOLOR ColorKey,PALETTEENTRY *pPalette) {
	TextureStruct tempTex;
	resStruct tempRes;
	tempRes.type = texture;
	tempRes.name = name;
	loadDev->Load2D(name,Width,Height,MipLevels,Usage,Format,Pool,Filter,MipFilter,ColorKey,&tempTex.texInfo,pPalette,&tempTex.objTex);
	tempTex.mask = ColorKey;
	textureList.push_back(tempTex);
	tempRes.res = &textureList.back();
	resources.push_back(tempRes);
	return (TextureStruct*)tempRes.res;
}

PrimStruct* ResourceManager::loadPrim(LPCSTR name,float height,float width,float depth) {
	PrimStruct tempPrim;
	resStruct tempRes;
	tempRes.type = prim;
	tempRes.name = name;
	loadDev->CreateUVCube(tempPrim,height,width,depth);
	tempPrim.height = height;
	tempPrim.width = width;
	tempPrim.depth = depth;
	primList.push_back(tempPrim);
	tempRes.res = &primList.back();
	resources.push_back(tempRes);
	return (PrimStruct*)tempRes.res;
}

SoundStruct* ResourceManager::loadSound(LPCSTR name,float minDist, float maxDist,float volume) {
	SoundStruct tempSound;
	resStruct tempRes;
	tempRes.type = soundEffect;
	tempRes.name = name;
	soundDev->load(name,&tempSound.sound);
	tempSound.minDist = minDist;
	tempSound.maxDist = maxDist;
	tempSound.volume = volume;
	soundList.push_back(tempSound);
	tempRes.res = &soundList.back();
	resources.push_back(tempRes);
	return (SoundStruct*)tempRes.res;
}

MusicStruct* ResourceManager::loadMusic(LPCSTR name,float volume) {
	MusicStruct tempMusic;
	resStruct tempRes;
	tempRes.type = music;
	tempRes.name = name;
	soundDev->loadStream(name,&tempMusic.sound);
	tempMusic.volume = volume;
	musicList.push_back(tempMusic);
	tempRes.res = &musicList.back();
	resources.push_back(tempRes);
	return (MusicStruct*)tempRes.res;
}