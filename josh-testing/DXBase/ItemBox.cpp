#include "ItemBox.h"

const float terminalVelocity = 10.0f;

ItemBox::ItemBox(){
	//itemBox.mat=0;
	//D3DXMatrixIdentity(&itemBox.matrix);
	//itemBox.primInfo=0;
	//itemBox.Tex=0;
	part.LimbInit(); // for testing
	pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	bounding.height = 0.5f;
	bounding.radius = 0.5f;
	velocityY=0;
	onGround=false;
	active=true;
}

void ItemBox::ItemBoxInit(float a_x, float a_y, float a_z) {
	pos.x = a_x;
	pos.y = a_y;
	pos.z = a_z;
	prospectivePos.x = pos.x;
	prospectivePos.y = pos.y;
	prospectivePos.z = pos.z;
	bounding.height = 0.5f;
	bounding.radius = 0.5f;
	part.LimbInit(); //for testing
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

void ItemBox::setVelocityY(float a_velocityY) {
	velocityY = a_velocityY;
}

void ItemBox::toggleGrounded(bool a_grounded) {
	onGround = a_grounded;
}

void ItemBox::Update(float a_dt) {
	float tempfloat;

	pos.x = prospectivePos.x;
	pos.y = prospectivePos.y;
	pos.z = prospectivePos.z;

	velocityY -= 0.3f*a_dt;
	if(velocityY < -terminalVelocity)
		velocityY = -terminalVelocity;
	if(velocityY > terminalVelocity)
		velocityY = terminalVelocity;

	prospectivePos.y += velocityY;

}

void ItemBox::toggleActive(bool is_active){
	active=is_active;
}