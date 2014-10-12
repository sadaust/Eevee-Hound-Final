#pragma once
#include "ResDefs.h"
#include "ShapeDefs.h"


enum TerrainType {
	FLOOR,
	WALL,
	CEILING,
	FLOORWALL,
	FLOORCEILING
};





class Terrain {
private:
	D3DXVECTOR3 pos;
	cube boundingBox;
	TerrainType type;
public:
	Terrain();
	~Terrain();
	void Init(D3DXVECTOR3 a_pos, PrimStruct * a_structpoi, TerrainType a_type);
	cube getBound();
	D3DXVECTOR3 getPos();
	TerrainType getType();
};
