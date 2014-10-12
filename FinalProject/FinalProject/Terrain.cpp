#include "Terrain.h"




Terrain::Terrain() {
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	boundingBox.back = 0;
	boundingBox.front = 0;
	boundingBox.left = 0;
	boundingBox.right = 0;
	boundingBox.back = 0;
	boundingBox.back = 0;
}


Terrain::~Terrain() {

}


void Terrain::Init(D3DXVECTOR3 a_pos, PrimStruct * a_structpoi, TerrainType a_type) {
	pos = a_pos;
	boundingBox.back = a_structpoi->back;
	boundingBox.bottom = a_structpoi->bottom;
	boundingBox.front = a_structpoi->front;
	boundingBox.left = a_structpoi->left;
	boundingBox.right = a_structpoi->right;
	boundingBox.top = a_structpoi->top;
	type = a_type;
}


D3DXVECTOR3 Terrain::getPos() {
	return pos;
}


cube Terrain::getBound() {
	return boundingBox;
}


TerrainType Terrain::getType() {
	return type;
}