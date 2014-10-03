#include "Player.h"

const float turnspeedX = 90.0f;
const float turnspeedY = 45.0f;
const float maxSpeed   = 20.0f;
const float terminalVelocity = 10.0f;


Player::Player() {
	pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	velocityXZ = D3DXVECTOR2(0.0f,0.0f);
	facing = 0;
	velocityY = 0;
	moving = 0;
	boundingCyl.height = 1;
	boundingCyl.radius = 0.5f;
	onGround = false;
}


Player::~Player() {

}


void Player::testInit(float a_x, float a_y, float a_z, float a_rot) {
	pos.x = a_x;
	pos.y = a_y;
	pos.z = a_z;
	prospectivePos.x = pos.x;
	prospectivePos.y = pos.y;
	prospectivePos.z = pos.z;
	facing = a_rot;
	speed = 5.0f;
	boundingCyl.height = 1;
	boundingCyl.radius = 0.5f;
}


void rotate2Dvector(D3DXVECTOR2* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->x - sinf(angle)*pV2->y;
	float Vy1 = sinf(angle)*pV2->x + cosf(angle)*pV2->y;
	// store results thru the pointer
	pV2->x = Vx1;
	pV2->y = Vy1;

	return;
}

void Player::Update(XINPUT_STATE a_state, float a_dt, float &a_rot, float &a_angle) {
	int tempint;

	
	pos.x = prospectivePos.x;
	pos.y = prospectivePos.y;
	pos.z = prospectivePos.z;


	  ////////////////////////////////////////
	 // Takes inputs, sets prospective pos //
	////////////////////////////////////////



	tempint = a_state.Gamepad.sThumbLX;
	if(tempint < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tempint > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		tempint = 0;
	//pos.x += (tempint/32767.0f)*a_dt;
	velocityXZ.x = (tempint/32767.0f)*a_dt*speed;

	tempint = a_state.Gamepad.sThumbLY;
	if(tempint < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&tempint > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		tempint = 0;
	//pos.z += (tempint/32767.0f)*a_dt;
	velocityXZ.y = (tempint/32767.0f)*a_dt*speed;

	tempint = a_state.Gamepad.sThumbRX;
	if(tempint < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tempint > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		tempint = 0;
	a_rot += ((tempint/32767.0f)*a_dt)*turnspeedX;
	tempint = a_state.Gamepad.sThumbRY;
	if(tempint < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&& tempint > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		tempint = 0;
	a_angle -= ((tempint/32767.0f)*a_dt)*turnspeedY; // minus to uninvert it
	if(a_angle >= 90)
		a_angle = 89.9f;
	else if(a_angle <=-90)
		a_angle = -89.9f;
	
	velocityY -= 0.3f*a_dt;
	if(velocityY < -terminalVelocity)
		velocityY = -terminalVelocity;
	if(velocityY > terminalVelocity)
		velocityY = terminalVelocity;
	//if(pos.y <= 0 && velocityY <= 0)
	//	velocityY = 0;
	if(a_state.Gamepad.wButtons&XINPUT_GAMEPAD_A && onGround) {
		velocityY = 0.2f;
		onGround = false;
	}
	
	
	facing = D3DXToRadian(a_rot);

	//D3DXMATRIX matrixlovetemptest;
	//D3DXVECTOR2 tempveloc = velocityXZ;
	//float templength = D3DXVec2Length(&velocityXZ);
	//D3DXVec2Normalize(&velocityXZ,&velocityXZ);
	//D3DXMatrixTransformation2D(&matrixlovetemptest,NULL,NULL,NULL,&D3DXVECTOR2(pos.x,pos.z),facing,&velocityXZ);
	//D3DXVECTOR4 matrixtemplove4;
	//D3DXVec2Transform(&matrixtemplove4, &velocityXZ, &matrixlovetemptest);

	//velocityXZ.x = matrixtemplove4.x;
	//velocityXZ.y = matrixtemplove4.y;
	//D3DXVECTOR2 tempveloc = velocityXZ;
	//if(D3DXVec2Length(&velocityXZ) >= maxSpeed)
	//	velocityXZ = tempveloc;
	rotate2Dvector(&velocityXZ,-facing);

	prospectivePos.x += velocityXZ.x;
	prospectivePos.y += velocityY;
	prospectivePos.z += velocityXZ.y;


	//if() {
	//	onGround = true;
	//	velocityY = 0;
	//}
	//else {
	//	onGround = false;
	//}





}


void Player::testUpdate(float a_x, float a_z, float a_rot) {
	pos.x = a_x;
	pos.z = a_z;
	facing = D3DXToRadian(a_rot);
}


D3DXVECTOR3 Player::getPos() {
	return pos;
}


D3DXVECTOR3 Player::getProspectivePos() {
	return prospectivePos;
}


float Player::getFacing() {
	return facing;
}


float Player::getMoving() {
	return moving;
}


D3DXVECTOR2 Player::getVelocityXZ() {
	return velocityXZ;
}


float Player::getVelocityY() {
	return velocityY;
}


cylinder Player::getBound() {
	return boundingCyl;
}


void Player::setBound(float a_h, float a_r) {
	boundingCyl.height = a_h;
	boundingCyl.radius = a_r;
}


void Player::setPosInts(float a_x, float a_y, float a_z) {
	pos = D3DXVECTOR3(a_x,a_y,a_z);
}


void Player::setPos(D3DXVECTOR3 a_pos) {
	pos = a_pos;
}


void Player::setProspectivePos(D3DXVECTOR3 a_prospos) {
	prospectivePos = a_prospos;
}

	
void Player::setFacing(float a_facing) {
	facing = a_facing;
}


void Player::setMoving(float a_moving) {
	moving = a_moving;
}


void Player::setVelocityInts(float a_x, float a_y, float a_z) {
	velocityXZ = D3DXVECTOR2(a_x,a_z);
	velocityY = a_y;
}


void Player::setVelocityXZ(D3DXVECTOR2 a_velocityXZ) {
	velocityXZ = a_velocityXZ;
}


void Player::setVelocityY(float a_velocityY) {
	velocityY = a_velocityY;
}


void Player::toggleGrounded(bool a_grounded) {
	onGround = a_grounded;
}

void Player::addLimb(Limb part){
	if(part.getPartType()==head){
		Limbs.setHead(part.getPartID());
	}
	if(part.getPartType()==arm&&R_arm==true){
		Limbs.setRarm(part.getPartID());
	}
	if(part.getPartType()==arm&&R_arm==false){
		Limbs.setLarm(part.getPartID());
	}
	if(part.getPartType()==leg){
		Limbs.setLeg(part.getPartID());
	}
	if(part.getPartType()==body){
		Limbs.setBody(part.getPartID());
	}
}