#include "Bullet.h"


Bullet::Bullet() {

}


Bullet::~Bullet() {
	
}


void Bullet::Init(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimObj  a_structpoi, float a_rot, float a_angle, float a_lifespan, int a_damage) {
	pos = a_pos;
	rot = a_rot;
	angle = a_angle;
	rotate3Dvector(&a_velocity, a_rot, a_angle);
	velocity = a_velocity;
	setPrimObj(a_structpoi);
	prospectivePos = pos;
	speed = 5.0f;
	lifespan = a_lifespan;
	damage = a_damage;
}


void Bullet::Update(double a_dt) {
	pos.x = prospectivePos.x;
	pos.y = prospectivePos.y;
	pos.z = prospectivePos.z;
	if(a_bultype==boulder){
		prospectivePos.y -= a_dt*4.55f;
	}
	prospectivePos.x += velocity.x*a_dt;
	prospectivePos.y += velocity.y*a_dt;
	prospectivePos.z += velocity.z*a_dt;
	lifespan -= a_dt;


}


D3DXVECTOR3 Bullet::getPos() {
	return pos;
}


D3DXVECTOR3 Bullet::getProspectivePos() {
	return prospectivePos;
}


int Bullet::getDamage() {
	return damage;
}


float Bullet::getLifeSpan() {
	return lifespan;
}


float Bullet::getRot() {
	return rot;
}


float Bullet::getAngle() {
	return angle;
}


PrimObj& Bullet::getPrimObj() {
	return structpoi;
}


void Bullet::setPrimObj(PrimObj a_prim) {
	structpoi.mat = a_prim.mat;
	structpoi.matrix = a_prim.matrix;
	structpoi.primInfo = a_prim.primInfo;
	structpoi.Tex = a_prim.Tex;
}


//bool Bullet::getActive() {
//	return active;
//}
//
//
//void Bullet::setActive(bool a_active) {
//	active = a_active;
//}


void Bullet::HitWall() {
	
}


void Bullet::HitPlayer(Player& a_player) {
	  //////////////////////////////////////////////
	 // Add in damage and stuff here probably    //
	//////////////////////////////////////////////
	a_player.takeDamage(damage);
	
}


BulletVec::BulletVec() {

}


BulletVec::~BulletVec() {

}


void BulletVec::Init(ResourceManager& resMan) {
	PrimObj tempObj;
	for(int i = 0; i < MAXBULLETS; ++i) {
		bullets[i].Init(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), tempObj,0,0,0,0); //////////////////////////////////////////
		bools[i] = false;
	}
	
	//regular bullet
	testMat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	testMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	testMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// Emissive color reflected
	testMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// Specular
	testMat.Power = 0.0f;
	b_render[0].mat = &testMat;
	b_render[0].Tex = resMan.loadTexture("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,255),0);
	b_render[0].primInfo = resMan.loadPrim("bulletCube",0.1f,0.1f,0.1f);
	D3DXMatrixIdentity(&b_render[0].matrix);
	//lazer bullet
	b_render[1]=b_render[0];
	b_render[1].primInfo = resMan.loadPrim("LazerBeamz",0.25,0.25,10);
	//rock bullet
	b_render[2]=b_render[0];
	b_render[2].primInfo = resMan.loadPrim("RockSolid",1.5,1.5,1.5);
	//grab bullet
	b_render[3]=b_render[0];
	b_render[3].primInfo = resMan.loadPrim("Grabby",0.5,0.5,0.5);

}


void BulletVec::Render(DXFrame& DXVid) {
	RenInfo tempRen;
	D3DXMATRIX TransMat, RotMat;
	tempRen.type = primitive;
	tempRen.locCamNum = 0;
	for(int i = 0; i < MAXBULLETS; ++i) {
		if(GetActive(i)) {
			//bullets[i].setPrimObj(b_render[1]);
			D3DXMatrixIdentity(&TransMat);
			D3DXMatrixIdentity(&RotMat);
			D3DXMatrixRotationYawPitchRoll(&RotMat, D3DXToRadian(bullets[i].getRot()), D3DXToRadian(bullets[i].getAngle()), 0);
			D3DXMatrixTranslation(&TransMat, bullets[i].getPos().x, bullets[i].getPos().y-.5f, bullets[i].getPos().z);
			D3DXMatrixMultiply(&bullets[i].getPrimObj().matrix, &RotMat, &TransMat);
			tempRen.asset = &bullets[i].getPrimObj();
			DXVid.addRen(tempRen);
		}
	}

}


void BulletVec::Update(double a_dt) {
	for(int i = 0; i < MAXBULLETS; ++i) {
		if(bools[i]) {
			bullets[i].Update(a_dt);
			if(bullets[i].getLifeSpan() <= 0) {
				DeactivateABullet(i);
			}
		}
	}
}


Bullet& BulletVec::GetBullet(int a_index) {
	return bullets[a_index];
}


bool BulletVec::GetActive(int a_index) {
	return bools[a_index];
}


bool BulletVec::ActivateABullet(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, int a_type, float a_rot, float a_angle, float a_lifespan, int a_damage) {
	for(int i = 0; i < MAXBULLETS; ++i) {
		if(!bools[i]) {
			if(a_type==defaultbullet){
				bullets[i].setBullType(defaultbullet);
			}
			if(a_type==laser){
				bullets[i].setBullType(laser);
			}
			if(a_type==boulder){
				bullets[i].setBullType(boulder);
			}
			if(a_type==grab){
				bullets[i].setBullType(grab);
				bullets[i].setStartpos(a_pos);
			}
			bullets[i].Init(a_pos, a_velocity, b_render[a_type], a_rot, a_angle, a_lifespan, a_damage);
			bools[i] = true;
			return true;

		}
	}
	return false;
}


bool BulletVec::DeactivateABullet(int a_index) {
	bools[a_index] = false;
	return true;
}