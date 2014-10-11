#pragma once
#include "ResDefs.h"
#include "Player.h"
#include "ResourceManager.h"

class Player;

const float RangedDefaultLifeSpan = 20.0f;
const float MeleeDefaultLifeSpan = 0.2f;
const int MAXBULLETS = 256;
const float BulletSpeed = 10.1f;
const int testDamage = 1;
const int bulletTypes= 4;


enum BulletType {
	defaultbullet,
	laser,
	boulder,
	grab
};



class Bullet {
private:
	D3DXVECTOR3 pos, prospectivePos, velocity, startpos;
	PrimObj structpoi; // this too, if we don't want cubes. VV
	float speed; // figure out what to do with this later
	int damage;
	float lifespan;
	float rot;
	float angle;
	BulletType a_bultype;
	//bool active;
public:
	Bullet();
	~Bullet();
	void Init(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimObj  a_structpoi, float a_rot, float a_angle, float a_lifespan, int a_damage); // Add parameters
	void Update(float a_dt); // Parameters?
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	int getDamage();
	float getLifeSpan();
	float getRot();
	float getAngle();
	PrimObj& getPrimObj();
	void setPrimObj(PrimObj a_prim);
	D3DXVECTOR3 getStartpos(){return startpos;}
	void setStartpos(D3DXVECTOR3 a_startpos){startpos=a_startpos;}
	void setBullType(BulletType a_typeofbul){a_bultype=a_typeofbul;}
	BulletType getBullType(){return a_bultype;}
	//bool getActive();
	//void setActive(bool a_active); moving these into bulletvec
	void HitWall();
	void HitPlayer(Player& a_player);
};


class BulletVec {
private:
	PrimObj b_render[bulletTypes];
	Bullet bullets[MAXBULLETS];
	bool bools[MAXBULLETS];
	D3DMATERIAL9 testMat;
public:
	BulletVec();
	~BulletVec();
	void Init(ResourceManager& resMan);
	void Render(DXFrame& DXVid);
	void Update(float a_dt);
	Bullet& GetBullet(int a_index);
	bool GetActive(int a_index);
	bool ActivateABullet(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, int a_type, float a_rot, float a_angle, float a_lifespan, int a_damage);
	bool DeactivateABullet(int a_index);
};
