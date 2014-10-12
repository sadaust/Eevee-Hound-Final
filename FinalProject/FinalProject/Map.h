#pragma once
#include "Terrain.h"
#include "spawnPoint.h"
#include "ResourceManager.h"
#include <vector>



class Map {
private:
	std::vector<Terrain> walls;
	std::vector<int> wallZone;
	std::vector<Terrain> floors;
	std::vector<int> floorZone;
	std::vector<sPoint> spawnPoints;
	std::vector<PrimObj> renderInfo;
	D3DMATERIAL9 mat;
public:
	Map();
	~Map();
	void LoadMap(char* fileName, ResourceManager & resMan);
	void AddWall(Terrain a_wall);
	void AddFloor(Terrain a_floor);
	void AddSpawn(sPoint a_spawn);
	Terrain& GetWall(int a_index);
	Terrain& GetFloor(int a_index);
	sPoint& GetSpawn(int a_index);
	void render(DXFrame&frame);
	int numWalls() { return walls.size();}
	int numFloors() {return floors.size();}
	int numSpawn() {return spawnPoints.size();}

};

