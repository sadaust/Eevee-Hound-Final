#pragma once
#include "Terrain.h"
#include "ResourceManager.h"
#include <vector>



class Map {
private:
	std::vector<Terrain> walls;
	std::vector<Terrain> floors;
	std::vector<PrimObj> renderInfo;
	D3DMATERIAL9 mat;
public:
	Map();
	~Map();
	void LoadMap(char* fileName, ResourceManager & resMan);
	void AddWall(Terrain a_wall);
	void AddFloor(Terrain a_floor);
	Terrain& GetWall(int a_index);
	Terrain& GetFloor(int a_index);
	void render(DXFrame&frame);
	int numWalls() { return walls.size();}
	int numFloors() {return floors.size();}


};

