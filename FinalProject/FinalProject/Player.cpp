#include "Player.h"

const float turnspeedX = 90.0f;
const float turnspeedY = 45.0f;
const float maxSpeed   = 20.0f;
const float terminalVelocity = 50.0f;


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

}


Player::~Player() {

}


void Player::Init(sPoint& spawn, PrimObj a_primDefs[],SoundFrame *sound) {
	sFrame = sound;
	maxHealth = 100;
	curHealth = 100;
	velocityXZ = D3DXVECTOR2(0.0f,0.0f);
	velocityY = 0;
	boundingCyl.height = 1;
	boundingCyl.radius = 0.5f;
	onGround = false;
	checkItem= false;
	R_arm= false;
	alive = false;
	timer = 0;
	jumpCount=0;
	respawn(spawn);
	speed = 5.0f;
	boundingCyl.height = 1;
	boundingCyl.radius = 0.5f;
	setPrimObj(a_primDefs[0],0);
	setPrimObj(a_primDefs[1],1);
	setPrimObj(a_primDefs[2],2);
	setPrimObj(a_primDefs[2],3);
	setPrimObj(a_primDefs[2],4);
	setPrimObj(a_primDefs[2],5);
	facing = 0;
	angle = 0;
	spectator = false;
	ProsteticTestLimb.setName("None");
	effectprop.pos.x=pos.x;
	effectprop.pos.y=pos.y;
	effectprop.pos.z=pos.z;
	effectprop.vel.x=velocityXZ.x;
	effectprop.vel.y=velocityXZ.y;
	effectprop.up.y=velocityY;
	
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


void Player::Update(inputState& a_state, double a_dt, Limbase &part_list,BulletVec &a_bulvec, ItemVec &a_items,ResourceManager& resMan,SoundFrame* sFrame) {
	float tempfloat;
	tempfloat = a_state.rX;
	facing += (tempfloat*a_dt)*turnspeedX;
	tempfloat = a_state.rY;
	angle -= (tempfloat*a_dt)*turnspeedY; // minus to uninvert it
	if(angle >= 90)
		angle = 89.9f;
	else if(angle <=-90)
		angle = -89.9f;
	if(spectator) {
		curHealth = maxHealth;
		//last player
		if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
			--maxHealth;
		}
		//next player
		if(a_state.buttons[binds::rightAttack]&&!a_state.buttonLast[binds::rightAttack]) { 
			++maxHealth;
		}
	} else {
		//check alive
		if(alive) {
			//do if alive
			pos.x = prospectivePos.x;
			pos.y = prospectivePos.y;
			pos.z = prospectivePos.z;
			effectprop.pos.x=pos.x;
			effectprop.pos.y=pos.y;
			effectprop.pos.z=pos.z;


			////////////////////////////////////////
			// Takes inputs, sets prospective pos //
			////////////////////////////////////////



			tempfloat = a_state.lX;
			velocityXZ.x = tempfloat*a_dt*speed;
			effectprop.vel.x=velocityXZ.x;

			tempfloat = a_state.lY;
			velocityXZ.y = tempfloat*a_dt*speed;
			effectprop.vel.z=velocityXZ.y;

			velocityY -= gravity*a_dt;
			if(velocityY < -terminalVelocity)
				velocityY = -terminalVelocity;
			if(velocityY > terminalVelocity)
				velocityY = terminalVelocity;
			effectprop.up.y=velocityY;

			//if(pos.y <= 0 && velocityY <= 0)
			//	velocityY = 0;
			/////////////////////////////////////
			//////////PLAYER CONTROLS////////////
			/////////////////////////////////////

			//selecting item for left and right arm
			if(a_state.buttons[binds::leftSelect]&&!a_state.buttonLast[binds::leftSelect]) {
				toggleRarm(false);
			}
			if(a_state.buttons[binds::rightSelect]&&!a_state.buttonLast[binds::rightSelect]) {
				toggleRarm(true);
			}

			// need to deal with object for bullet 
			//left attack Left Trigger
			if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
				if (!Limbs.getLarm()==0){
					part_list.CaseAction(Limbs.getLarm(),*this,a_state,a_bulvec,facing,angle,resMan,sFrame);
				}
			}
			//right attack Right Trigger
			if(a_state.buttons[binds::rightAttack]&&!a_state.buttonLast[binds::rightAttack]) { 
				if(Limbs.getRarm()==0){
					D3DXVECTOR2 tempvec;
					tempvec.y = boundingCyl.radius; // z
					tempvec.x = boundingCyl.radius; // x
					rotate2Dvector(&tempvec, D3DXToRadian(-facing));
					a_bulvec.ActivateABullet(D3DXVECTOR3(tempvec.x + pos.x, 1.4f + pos.y, tempvec.y +pos.z),D3DXVECTOR3(0,0,-BulletSpeed),0,facing,angle,MeleeDefaultLifeSpan, testDamage);
					
					sFrame->setListenProp(0,effectprop);
					soundeffect = resMan.loadSound("Bullet.mp3",1,5,1);
					sFrame->Play(*soundeffect,getPos().x,getPos().y,getPos().z,getVelocityXZ().x,getVelocityY(),getVelocityXZ().y);
				}
				else if(!Limbs.getRarm()==0){
					part_list.CaseAction(Limbs.getRarm(),*this,a_state,a_bulvec,facing,angle,resMan,sFrame);
				}
			}
			// jump  A
			if(a_state.buttons[binds::jump] && onGround && !a_state.buttonLast[binds::jump]) {
				velocityY = jumpHeight;
				onGround = false;
				sFrame->setListenProp(0,effectprop);
				soundeffect = resMan.loadSound("Jump.mp3",1,5,1);
				sFrame->Play(*soundeffect,getPos().x,getPos().y,getPos().z,getVelocityXZ().x,getVelocityY(),getVelocityXZ().y);
			}
			//leg power B
			if(a_state.buttons[binds::legPower]&&!a_state.buttonLast[binds::legPower]) {
				//get part !0 
				if (!Limbs.getLeg()==0){
					part_list.CaseAction(Limbs.getLeg(),*this,a_state,a_bulvec,facing,angle,resMan,sFrame);
					// find that part 
					// execute that function
				}
			}
			if(a_state.buttons[binds::use]) {
				//if item is coliding with character
				if(getcheckItem()){
					addLimb(ProsteticTestLimb);
					togglecheckItem(false);
					ProsteticTestLimb.setName("None");
				}
				// make bool switch here.
				//
				//bring menue up 
				// 
			}





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
			rotate2Dvector(&velocityXZ,-D3DXToRadian(facing));

			prospectivePos.x += velocityXZ.x;
			prospectivePos.y += velocityY*a_dt;;
			prospectivePos.z += velocityXZ.y;

			//if() {
			//	onGround = true;
			//	velocityY = 0;
			//}
			//else {
			//	onGround = false;
			//}
			if(pos.y < -40) {
				alive = false;
				timer = 3;
			}
			if(curHealth <= 0) {
				alive = false;
				timer = 3;
				a_items.ActivateAItem(*this,part_list);
			}
		} else {
			//do if dead
			if(timer>=0) {
				timer -= a_dt;
			}
		}
	}
}


void Player::Render(DXFrame& DXVid,ResourceManager &resMan) {
	RenInfo tempRen;
	D3DXMATRIX TransMat, RotMat, locTransMat;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;
	int tempint = 0;

	// body
	tempint=Limbs.getBody();
	settexture(tempint,playerPrims[0],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixTranslation(&locTransMat, 0, .75f, 0);
	D3DXMatrixMultiply(&playerPrims[0].matrix, &locTransMat, &RotMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[0].matrix, &playerPrims[0].matrix, &TransMat);
	tempRen.asset = &playerPrims[0];
	DXVid.addRen(tempRen);
	// head
	tempint=Limbs.getHead();
	settexture(tempint,playerPrims[1],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixTranslation(&locTransMat, 0, 1, 0);
	D3DXMatrixMultiply(&playerPrims[1].matrix, &locTransMat, &RotMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[1].matrix, &playerPrims[1].matrix, &TransMat);
	tempRen.asset = &playerPrims[1];
	DXVid.addRen(tempRen);
	// left arm
	tempint=Limbs.getLarm();
	settexture(tempint,playerPrims[2],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixTranslation(&locTransMat, -.3f, 1.0f, 0);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixMultiply(&playerPrims[2].matrix, &locTransMat, &RotMat);
	//D3DXMatrixRotationY(&RotMat, D3DXToRadian(90));
	//D3DXMatrixMultiply(&playerPrims[2].matrix, &RotMat, &playerPrims[2].matrix);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[2].matrix, &playerPrims[2].matrix, &TransMat);
	tempRen.asset = &playerPrims[2];
	DXVid.addRen(tempRen);
	// right arm
	tempint=Limbs.getRarm();
	settexture(tempint,playerPrims[3],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixTranslation(&locTransMat, .3f, 1.0f, 0);
	D3DXMatrixMultiply(&playerPrims[3].matrix, &locTransMat, &RotMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[3].matrix, &playerPrims[3].matrix, &TransMat);
	tempRen.asset = &playerPrims[3];
	DXVid.addRen(tempRen);
	// left leg
	tempint=Limbs.getLeg();
	settexture(tempint,playerPrims[4],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixTranslation(&locTransMat, .15f, .5f, 0);
	D3DXMatrixMultiply(&playerPrims[4].matrix, &locTransMat, &RotMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[4].matrix, &playerPrims[4].matrix, &TransMat);
	tempRen.asset = &playerPrims[4];
	DXVid.addRen(tempRen);
	// right leg
	tempint=Limbs.getLeg();
	settexture(tempint,playerPrims[5],resMan);
	D3DXMatrixIdentity(&TransMat);
	D3DXMatrixIdentity(&RotMat);
	D3DXMatrixIdentity(&locTransMat);
	D3DXMatrixRotationY(&RotMat, D3DXToRadian(facing));
	D3DXMatrixTranslation(&locTransMat, -.15f, .5f, 0);
	D3DXMatrixMultiply(&playerPrims[5].matrix, &locTransMat, &RotMat);
	D3DXMatrixTranslation(&TransMat, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&playerPrims[5].matrix, &playerPrims[5].matrix, &TransMat);
	tempRen.asset = &playerPrims[5];
	DXVid.addRen(tempRen);

}
void Player::settexture(int part,PrimObj &texture,ResourceManager &resMan){
//tech fair jankyness to fix properly later
//	2 turtle arm
//	6 bear arm
//	10 tiger arm
//	11 tiger leg
//	14  pheonix arm
//	15 pheonix leg
	if(part==2){
		texture.Tex = resMan.loadTexture("Turtle.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	}
	else if(part==6){
		texture.Tex = resMan.loadTexture("Bear.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	}
	else if(part==10||part==11){
		texture.Tex = resMan.loadTexture("Tiger.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	}
	else if(part==14||part==15){
		texture.Tex = resMan.loadTexture("Pheonix.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	}
	else{
		texture.Tex = resMan.loadTexture("Default.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	}
}


void Player::respawn(sPoint& spawn) {
	if(spawn.isActive()) {
		alive = true;
		maxHealth = 100;
		curHealth = maxHealth;
		pos = spawn.getPos();
		prospectivePos = pos;

		velocityY = 0;


		Limbs.setBody(0);
		Limbs.setHead(0);
		Limbs.setLarm(0);
		Limbs.setRarm(0);
		Limbs.setLeg(0);
		spawn.toggle();
	}
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


float Player::getAngle() {
	return angle;
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


void Player::setAngle(float a_angle) {
	angle = a_angle;
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


void Player::togglecheckItem(bool a_item) {
	checkItem=a_item;
}


bool Player::getGrounded() {
	return onGround;
}


bool Player::getcheckItem() {
	return checkItem;
}


bool Player::isAlive() {
	return alive;
}

void Player::toggleRarm(bool right_a){
	R_arm=right_a;
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


void Player::setJumpCount(int j_count) {
	jumpCount=j_count;
}


int Player::getJumpCount() {
	return jumpCount;
}


int Player::getHealth() {
	return curHealth;
}


void Player::incrementJumpCount() {
	jumpCount++;
}


void Player::itemAccess(ItemBox &a_item){
	// should handle bringing up itme swap menue etc in the future \
	// to be changed to list of accessable parts (maybe in area)
	//for now just passes test item to player
	ProsteticTestLimb=a_item.getPart();
	togglecheckItem(true);
}


void Player::setMaxHealth(int m_health) {
	maxHealth=m_health;
}


void Player::setCurHealth(int c_health) {
	curHealth=c_health;
}

void Player::setSpec(bool set) {
	spectator = set;
}

int Player::getMaxHealth() {
	return maxHealth;
}


PrimObj Player::getPrimObj(int a_index) {
	return playerPrims[a_index];
}


void Player::setPrimObj(PrimObj a_prim, int a_index) {
	playerPrims[a_index].mat = a_prim.mat;
	playerPrims[a_index].matrix = a_prim.matrix;
	playerPrims[a_index].primInfo = a_prim.primInfo;
	playerPrims[a_index].Tex = a_prim.Tex;
}

bool Player::isSpectator() {
	return spectator;
}



PlayerVec::PlayerVec() {

}


PlayerVec::~PlayerVec() {

}


void PlayerVec::Init(Map& a_map, ResourceManager& resMan) {
	for(int i = 0; i < MAXPLAYERS; ++i) {
		active[i] = false;
	}
	numPlayers = 0;


	//regular bullet
	PlayMat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	PlayMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	PlayMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Emissive color reflected
	PlayMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// Specular
	PlayMat.Power = 0.0f;
	playerDefaultPrims[0].mat = &PlayMat;
	playerDefaultPrims[0].Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	// body
	playerDefaultPrims[0].primInfo = resMan.loadPrim("Body",0.5f,0.5f,0.5f);
	D3DXMatrixIdentity(&playerDefaultPrims[0].matrix);
	// head
	playerDefaultPrims[1]=playerDefaultPrims[0];
	playerDefaultPrims[1].primInfo = resMan.loadPrim("Head",0.25f, 0.25f, .25f);
	// limb
	playerDefaultPrims[2]=playerDefaultPrims[0];
	playerDefaultPrims[2].primInfo = resMan.loadPrim("Limb",-.5f,0,-0.0625f,0.0625f,0.0625f,-0.0625f);



}


void PlayerVec::Update(inputState* a_input, double a_dt, Limbase part_list,BulletVec &a_bulvec,ItemVec &a_items,ResourceManager& resMan,SoundFrame* sFrame) {
	int temp, maxHp, curHp;
	temp = maxHp = curHp = 0;
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(active[i]) {
			players[i].Update(a_input[i], a_dt, part_list, a_bulvec,a_items,resMan,sFrame);
			if(players[i].isSpectator()) {
				if(players[i].getMaxHealth() != players[i].getHealth()) {
					maxHp = players[i].getMaxHealth();
					curHp = players[i].getHealth();

					if(maxHp>curHp) {
						temp = 1;
					} else {
						temp = -1;
					}
					if(maxHp<0)
						maxHp = MAXPLAYERS-1;
					else if(maxHp >= MAXPLAYERS)
						maxHp = 0;
					while(maxHp != curHp) {
						if(players[maxHp].isAlive()&&active[maxHp]) {
							curHp = maxHp;
						} else {
							maxHp += temp;
							if(maxHp<0)
								maxHp = MAXPLAYERS-1;
							else if(maxHp >= MAXPLAYERS)
								maxHp = 0;
						}
					}
					players[i].setCurHealth(curHp);
					players[i].setMaxHealth(maxHp);
				}
				players[i].setPos(players[players[i].getHealth()].getPos());
				players[i].setProspectivePos(players[i].getPos());
			}
		}
	}
}


void PlayerVec::Render(DXFrame& DXVid,ResourceManager &resMan) {

	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(active[i]&&players[i].isAlive()) {
			players[i].Render(DXVid,resMan);
		}
	}
}


bool PlayerVec::GetActive(int a_index) {
	return active[a_index];
}


int PlayerVec::ActivateAPlayer(Map& a_map) {
	int random = rand() % a_map.numSpawn();
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(!active[i]) {
			players[i].Init(a_map.GetSpawn(random),playerDefaultPrims,sFrame);
			active[i] = true;
			++numPlayers;
			return i;
		}
	}
}


void PlayerVec::DeactivateAPlayer(int a_index) {
	active[a_index] = false;
	--numPlayers;
}


Player& PlayerVec::GetPlayer(int a_index) {
	return players[a_index];
}


int PlayerVec::GetNumPlayers() {
	return numPlayers;
}





