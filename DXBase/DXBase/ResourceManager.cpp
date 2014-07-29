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
	//clear all models
	ModelStruct* tempMod;
	while(modelList.size() > 0) {
		tempMod = &modelList.back();
		tempMod->adj->Release();
		tempMod->adj = 0;
		tempMod->matPoint->Release();
		tempMod->matPoint = 0;
		tempMod->mesh->Release();
		tempMod->mesh = 0;
		delete [] tempMod->textures;
		tempMod->numMats = 0;
		modelList.pop_back();
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
	resStruct* res;
	TextureStruct* tempTex;
	PrimStruct* tempPrim;
	ModelStruct* tempMod;
	SoundStruct* tempSound;
	MusicStruct* tempMusic;
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
			loadDev->uncenterdUVCube(*tempPrim,tempPrim->bottom,tempPrim->top,tempPrim->left,tempPrim->right,tempPrim->front,tempPrim->back);
			break;
		case texture:
			tempTex = (TextureStruct*)resources[i].res;
			//release
			tempTex->objTex->Release();
			tempTex->objTex = 0;
			//reload
			loadDev->Load2D(resources[i].name,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,tempTex->mask,&tempTex->texInfo,0,&tempTex->objTex);
			break;
		case xModel:
			tempMod = (ModelStruct*)resources[i].res;
			//release
			tempMod->adj->Release();
			tempMod->adj = 0;
			tempMod->matPoint->Release();
			tempMod->matPoint = 0;
			tempMod->mesh->Release();
			tempMod->mesh = 0;
			delete [] tempMod->textures;
			tempMod->numMats = 0;
			//reload
			loadDev->loadXFile(resources[i].name,*tempMod);
			for(int i = 0;i<tempMod->numMats;++i) {
				res = getRes(tempMod->mats[i].pTextureFilename);
				if(res == 0) {
					res->res = loadTexture(tempMod->mats[i].pTextureFilename,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(0,0,0),0);
				}
				tempMod->textures[i] = (TextureStruct*)res->res;
			}
			break;
		case soundEffect:
			tempSound = (SoundStruct*) resources[i].res;
			//release
			tempSound->sound->release();
			tempSound->sound = 0;
			//reload
			soundDev->load(resources[i].name,&tempSound->sound);
			break;
		case music:
			tempMusic = (MusicStruct*) resources[i].res;
			//release
			tempMusic->sound->release();
			tempMusic->sound = 0;
			//reload
			soundDev->loadStream(resources[i].name,&tempMusic->sound);
			break;
		default:
			break;
		}
	}
}

void ResourceManager::reloadVideo() {
	resStruct* res;
	TextureStruct* tempTex;
	PrimStruct* tempPrim;
	ModelStruct* tempMod;
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
			loadDev->uncenterdUVCube(*tempPrim,tempPrim->bottom,tempPrim->top,tempPrim->left,tempPrim->right,tempPrim->front,tempPrim->back);
			break;
		case texture:
			tempTex = (TextureStruct*)resources[i].res;
			//release
			tempTex->objTex->Release();
			tempTex->objTex = 0;
			//reload
			loadDev->Load2D(resources[i].name,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,tempTex->mask,&tempTex->texInfo,0,&tempTex->objTex);
			break;
		case xModel:
			tempMod = (ModelStruct*)resources[i].res;
			//release
			tempMod->adj->Release();
			tempMod->adj = 0;
			tempMod->matPoint->Release();
			tempMod->matPoint = 0;
			tempMod->mesh->Release();
			tempMod->mesh = 0;
			delete [] tempMod->textures;
			tempMod->numMats = 0;
			//reload
			loadDev->loadXFile(resources[i].name,*tempMod);
			for(int i = 0;i<tempMod->numMats;++i) {
				res = getRes(tempMod->mats[i].pTextureFilename);
				if(res == 0) {
					res->res = loadTexture(tempMod->mats[i].pTextureFilename,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),0);
				}
				tempMod->textures[i] = (TextureStruct*)res->res;
			}
			break;
		default:
			break;
		}
	}
}

void ResourceManager::reloadSound() {
	SoundStruct* tempSound;
	MusicStruct* tempMusic;
	for(int i = 0;i<resources.size();++i) {
		switch(resources[i].type) {
		case soundEffect:
			tempSound = (SoundStruct*) resources[i].res;
			//release
			tempSound->sound->release();
			tempSound->sound = 0;
			//reload
			soundDev->load(resources[i].name,&tempSound->sound);
			break;
		case music:
			tempMusic = (MusicStruct*) resources[i].res;
			//release
			tempMusic->sound->release();
			tempMusic->sound = 0;
			//reload
			soundDev->loadStream(resources[i].name,&tempMusic->sound);
			break;
		default:
			break;
		}
	}
}

void ResourceManager::reload(LPCSTR name) {
	resStruct* tempRes, *res;
	TextureStruct* tempTex;
	PrimStruct* tempPrim;
	ModelStruct* tempMod;
	SoundStruct* tempSound;
	MusicStruct* tempMusic;
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
			loadDev->uncenterdUVCube(*tempPrim,tempPrim->bottom,tempPrim->top,tempPrim->left,tempPrim->right,tempPrim->front,tempPrim->back);
			break;
		case texture:
			tempTex = (TextureStruct*) tempRes->res;
			//release
			tempTex->objTex->Release();
			tempTex->objTex = 0;
			//reload
			loadDev->Load2D(tempRes->name,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,tempTex->mask,&tempTex->texInfo,0,&tempTex->objTex);
			break;
		case xModel:
			tempMod = (ModelStruct*)tempRes->res;
			//release
			tempMod->adj->Release();
			tempMod->adj = 0;
			tempMod->matPoint->Release();
			tempMod->matPoint = 0;
			tempMod->mesh->Release();
			tempMod->mesh = 0;
			tempMod->dec->Release();
			tempMod->dec = 0;
			delete [] tempMod->textures;
			tempMod->numMats = 0;
			//reload
			loadDev->loadXFile(tempRes->name,*tempMod);
			for(int i = 0;i<tempMod->numMats;++i) {
				res = getRes(tempMod->mats[i].pTextureFilename);
				if(res == 0) {
					res->res = loadTexture(tempMod->mats[i].pTextureFilename,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),0);
				}
				tempMod->textures[i] = (TextureStruct*)res->res;
			}
			break;
		case soundEffect:
			tempSound = (SoundStruct*) tempRes->res;
			//release
			tempSound->sound->release();
			tempSound->sound = 0;
			//reload
			soundDev->load(tempRes->name,&tempSound->sound);
			break;
		case music:
			tempMusic = (MusicStruct*) tempRes->res;
			//release
			tempMusic->sound->release();
			tempMusic->sound = 0;
			//reload
			soundDev->loadStream(tempRes->name,&tempMusic->sound);
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

bool ResourceManager::isResLoaded(LPCSTR name) {
	for(int i = 0; i < resources.size();++i) {
		if(name == resources[i].name)
			return true;
	}
	return false;
}

resStruct* ResourceManager::getRes(LPCSTR name) {
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
	primList.push_back(tempPrim);
	tempRes.res = &primList.back();
	resources.push_back(tempRes);
	return (PrimStruct*)tempRes.res;
}

//use this to create cubes with the center point off center
PrimStruct* ResourceManager::loadPrim(LPCSTR name,float bottom,float top,float left,float right,float front, float back) {
	PrimStruct tempPrim;
	resStruct tempRes;
	tempRes.type = prim;
	tempRes.name = name;
	loadDev->uncenterdUVCube(tempPrim,bottom,top,left,right,front,back);
	primList.push_back(tempPrim);
	tempRes.res = &primList.back();
	resources.push_back(tempRes);
	return (PrimStruct*)tempRes.res;
}

ModelStruct* ResourceManager::loadXFile(LPCSTR name) {
	ModelStruct tempMod;
	resStruct tempRes;
	resStruct* res;
	tempRes.type = xModel;
	tempRes.name = name;
	loadDev->loadXFile(name,tempMod);
	for(int i = 0;i<tempMod.numMats;++i) {
		res = getRes(tempMod.mats[i].pTextureFilename);
		if(res == 0) {
			res = &tempRes;
			res->res = loadTexture(tempMod.mats[i].pTextureFilename,D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,255,255),0);
		}
		tempMod.textures[i] = (TextureStruct*)res->res;
	}
	modelList.push_back(tempMod);
	tempRes.res = &modelList.back();
	resources.push_back(tempRes);
	return (ModelStruct*)tempRes.res;
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