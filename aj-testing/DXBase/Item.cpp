#include "Item.h"


ItemBox::ItemBox(){
	itemBox.mat=0;
	D3DXMatrixIdentity(&itemBox.matrix);
	itemBox.primInfo=0;
	itemBox.Tex=0;
	partID=0;
	pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

void ItemBox::ItemBoxInit(float a_x, float a_y, float a_z) {
	pos.x = a_x;
	pos.y = a_y;
	pos.z = a_z;
	prospectivePos.x = pos.x;
	prospectivePos.y = pos.y;
	prospectivePos.z = pos.z;
	bounding.height = 1;
	bounding.radius = 0.5f;
}

D3DXVECTOR3 ItemBox::getPos() {
	return pos;
}

D3DXVECTOR3 ItemBox::getProspectivePos() {
	return prospectivePos;
}

cylinder ItemBox::getBound() {
	return bounding;
}

void ItemBox::setBound(float a_h, float a_r) {
	bounding.height = a_h;
	bounding.radius = a_r;
}

void ItemBox::setPosInts(float a_x, float a_y, float a_z) {
	pos = D3DXVECTOR3(a_x,a_y,a_z);
}

void ItemBox::setPos(D3DXVECTOR3 a_pos) {
	pos = a_pos;
}

void ItemBox::setProspectivePos(D3DXVECTOR3 a_prospos) {
	prospectivePos = a_prospos;
}