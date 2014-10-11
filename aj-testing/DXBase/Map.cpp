#include "Map.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>



Map::Map() {
	mat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	mat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	mat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Emissive color reflected
	mat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// Specular
	mat.Power = 0.0f;
}


Map::~Map() {

}


void Map::LoadMap(char* fileName, ResourceManager& resMan) {
	////////////////////////////////////////////////////////////////FILE I/O STUFF
	std::stringstream ss;
	std::ifstream file;
	char lineType;
	Terrain tempTerrain;
	D3DVECTOR pos, corner1, corner2, size;
	float rot;
	PrimStruct *tempPrim;
	PrimObj tempObj;
	sPoint tempSpawn;
	tempObj.mat = &mat;
	tempObj.Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	file.open(fileName);
	if(file.is_open()) {
		while(!file.eof()) {
			lineType = ' ';
			file>>lineType;
			if(lineType == '#') {
				//comment line
				file.ignore(256,'\n');
			} else if(lineType == 'f'||lineType == 'F') {
				//floor
				//get top right
				file>>corner1.x;
				file>>corner1.y;
				file>>corner1.z;
				//get bottom left
				file>>corner2.x;
				file>>corner2.y;
				file>>corner2.z;
				//
				size.x = corner2.x - corner1.x;
				size.y = corner2.y - corner1.y;
				size.z = corner2.z - corner1.z;
				//pos
				pos.x = size.x/2;
				pos.y = size.y/2;
				pos.z = size.z/2;

				pos.x += corner1.x;
				pos.y += corner1.y;
				pos.z += corner1.z;

				ss<<"Floor"<<size.x<<"x"<<size.y<<"x"<<size.z;

				tempPrim = resMan.loadPrim(ss.str().c_str(),size.y,size.x,size.z);
				D3DXMatrixIdentity(&tempObj.matrix);
				D3DXMatrixTranslation(&tempObj.matrix,pos.x,pos.y,pos.z);
				tempObj.primInfo = tempPrim;
				renderInfo.push_back(tempObj);

				tempTerrain.Init(pos,tempPrim,FLOOR);

				AddFloor(tempTerrain);

				file.ignore();
			} else if(lineType == 'w' || lineType == 'W') {
				//walls
				//get top left
				file>>corner1.x;
				file>>corner1.y;
				file>>corner1.z;
				//get bottom right
				file>>corner2.x;
				file>>corner2.y;
				file>>corner2.z;
				//
				size.x = corner2.x - corner1.x;
				size.y = corner2.y - corner1.y;
				size.z = corner2.z - corner1.z;
				//pos
				pos.x = size.x/2;
				pos.y = size.y/2;
				pos.z = size.z/2;

				pos.x += corner1.x;
				pos.y += corner1.y;
				pos.z += corner1.z;

				ss<<"Wall"<<size.x<<"x"<<size.y<<"x"<<size.z;

				tempPrim = resMan.loadPrim(ss.str().c_str(),size.y,size.x,size.z);
				D3DXMatrixIdentity(&tempObj.matrix);
				D3DXMatrixTranslation(&tempObj.matrix,pos.x,pos.y,pos.z);
				tempObj.primInfo = tempPrim;
				renderInfo.push_back(tempObj);

				tempTerrain.Init(pos,tempPrim,WALL);

				AddWall(tempTerrain);

				file.ignore();
			} else if(lineType == 's' || lineType == 'S'){
				file>>pos.x;
				file>>pos.y;
				file>>pos.z;

				file>>rot;

				tempSpawn.setPoint(pos,rot);
				
				AddSpawn(tempSpawn);
			}
		}
		file.close();
	}
}


void Map::AddWall(Terrain a_wall) {
	walls.push_back(a_wall);
}


void Map::AddFloor(Terrain a_floor) {
	floors.push_back(a_floor);
}

void Map::AddSpawn(sPoint a_spawn) {
	spawnPoints.push_back(a_spawn);
}


Terrain& Map::GetWall(int a_index) {
	return walls[a_index];
}


Terrain& Map::GetFloor(int a_index) {
	return floors[a_index];
}

sPoint& Map::GetSpawn(int a_index) {
	return spawnPoints[a_index];
}

void Map::render(DXFrame& frame) {
	RenInfo tempRen;
	tempRen.locCamNum = 0;
	tempRen.type = primitive;
	for(int i = 0; i < renderInfo.size();++i) {
		tempRen.asset = &renderInfo[i];
		frame.addRen(tempRen);
	}
}





