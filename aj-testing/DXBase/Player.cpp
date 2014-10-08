#include "Player.h"

const float turnspeedX = 90.0f;
const float turnspeedY = 45.0f;
const float maxSpeed   = 20.0f;
const float terminalVelocity = 10.0f;


///////////////////////////
// Game Statistics/Math  //
///////////////////////////



void Player::takeDamage(int a_damage) {
	curHealth -= a_damage;
	if(curHealth < 0)
		curHealth = 0;
}



//////////////////////////// ~~~~~~~~~~~~
// End of game Statistics // ~~~~~~~~~~~~
//////////////////////////// ~~~~~~~~~~~~



Player::Player() {
	maxHealth = 100;
	curHealth = 100;
	pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	velocityXZ = D3DXVECTOR2(0.0f,0.0f);
	facing = 0;
	velocityY = 0;
	moving = 0;
	boundingCyl.height = 1;
	boundingCyl.radius = 0.5f;
	onGround = false;
	checkItem= false;
	R_arm= false;
	alive = true;
	timer = 0;
	jumpCount=0;
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


void rotate2Dvector(D3DXVECTOR2* a_vector, float a_angle) {
	float Y = sinf(a_angle) * a_vector->x + cosf(a_angle) * a_vector->y;
	float X = cosf(a_angle) * a_vector->x - sinf(a_angle) * a_vector->y;
	a_vector->x = X;
	a_vector->y = Y;
}


void rotate3Dvector(D3DXVECTOR3* a_vector, float a_rot, float a_angle) {
	D3DXVECTOR2 Yaw, Pitch;
	a_rot = D3DXToRadian(a_rot);
	a_angle = D3DXToRadian(a_angle);
	Pitch.x = a_vector->z;				
	Pitch.y = a_vector->y;				// the final Y
	rotate2Dvector(&Pitch, a_angle);	// XY rotation 
	Yaw.x = a_vector->x;				// the final X
	Yaw.y = Pitch.x;					// the final Z
	rotate2Dvector(&Yaw,a_rot);			// XZ rotation

	a_vector->x = Yaw.x; // X
	a_vector->y = Pitch.y; // Y
	a_vector->z = -(Yaw.y); // Z
}


void Player::Update(inputState& a_state, float a_dt, float &a_rot, float &a_angle,Limbase part_list) {
	float tempfloat;

	//check alive
	if(alive) {
		//do if alive
		pos.x = prospectivePos.x;
		pos.y = prospectivePos.y;
		pos.z = prospectivePos.z;


		////////////////////////////////////////
		// Takes inputs, sets prospective pos //
		////////////////////////////////////////



		tempfloat = a_state.lX;
		velocityXZ.x = tempfloat*a_dt*speed;

		tempfloat = a_state.lY;
		velocityXZ.y = tempfloat*a_dt*speed;

		tempfloat = a_state.rX;
		a_rot += (tempfloat*a_dt)*turnspeedX;
		tempfloat = a_state.rY;
		a_angle -= (tempfloat*a_dt)*turnspeedY; // minus to uninvert it
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
		if(a_state.buttons[binds::jump] && onGround) {
			velocityY = 0.2f;
			onGround = false;
		}
		if(a_state.buttons[binds::legPower]&&!a_state.buttonLast[binds::legPower]) {
			//get part !0 
			if (!Limbs.getLeg()==0){
				part_list.CaseAction(Limbs.getLeg(),*this);
				// find that part 
				// execute that function
			}
		}
		if(a_state.buttons[binds::use]) {
			//if item is coliding with character
			if(getcheckItem()){
				addLimb(ProsteticTestLimb);
			}
			// make bool switch here.
			//
			//bring menue up 
			// 
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
		if(curHealth <= 0) {
			alive = false;
			timer = 3;
		}
	} else {
		//do if dead
		if(timer>=0) {
			timer -= a_dt;
		}
	}

}

void Player::respawn(sPoint& spawn) {
	alive = true;
	curHealth = maxHealth;
	pos = spawn.getPos();
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
	setJumpCount(0);
}

void Player::togglecheckItem(bool a_item){
	checkItem=a_item;
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

void Player::itemAccess(ItemBox &a_item){
	// should handle bringing up itme swap menue etc in the future \
	// to be changed to list of accessable parts (maybe in area)
	//for now just passes test item to player
	ProsteticTestLimb=a_item.getPart();
	a_item.toggleActive(false);
}