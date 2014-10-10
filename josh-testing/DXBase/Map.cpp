#include "Map.h"






Map::Map() {

}


Map::~Map() {

}


void Map::LoadMap(/* Add parameters maybe */) {
	////////////////////////////////////////////////////////////////FILE I/O STUFF


}


void Map::AddWall(Terrain a_wall) {
	walls.push_back(a_wall);
}


void Map::AddFloor(Terrain a_floor) {
	floors.push_back(a_floor);
}


Terrain& Map::GetWall(int a_index) {
	return walls[a_index];
}


Terrain& Map::GetFloor(int a_index) {
	return floors[a_index];
}





