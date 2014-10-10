#pragma once
#include "Terrain.h"
#include <vector>



class Map {
private:
	std::vector<Terrain> walls;
	std::vector<Terrain> floors;
public:
	Map();
	~Map();
	void LoadMap(/* Add parameters maybe */);
	void AddWall(Terrain a_wall);
	void AddFloor(Terrain a_floor);
	Terrain& GetWall(int a_index);
	Terrain& GetFloor(int a_index);



};

