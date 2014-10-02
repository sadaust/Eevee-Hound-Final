#pragma once
#include "Player.h"
#include "ResDefs.h"


enum TerrainType {
	FLOOR,
	WALL,
	CEILING,
	FLOORWALL,
	FLOORCEILING
};


//enum CollisionType {
//	NOTCOLLIDING,
//	
//};    THINK ABOUT THIS, ASK SAD AUST BOOM MOOB




class Bullet {
private:
	D3DXVECTOR3 pos, prospectivePos, velocity;
	PrimStruct* structpoi; // this too, if we don't want cubes. VV
	float speed; // figure out what to do with this later
public:
	Bullet();
	~Bullet();
	void Init(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimStruct * a_structpoi); // Add parameters
	void Update(float a_dt); // Parameters?
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	void HitWall();
	void HitPlayer(Player& a_player);
};

<<<<<<< HEAD
=======
class BulletVec {
private:
	Bullet bullets[MAXBULLETS];
	bool bools[MAXBULLETS];
public:
	BulletVec();
	~BulletVec();
	void Init();
	void Update(float a_dt);
	Bullet& GetBullet(int a_index);
	bool GetActive(int a_index);
	bool ActivateABullet(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimStruct * a_structpoi, float a_rot, float a_angle);
	bool DeactivateABullet(int a_index);
};



>>>>>>> origin/master

class Terrain {
private:
	D3DXVECTOR3 pos;
	PrimStruct* structpoi;
	TerrainType type;
public:
	Terrain();
	~Terrain();
	void Init(D3DXVECTOR3 a_pos, PrimStruct * a_structpoi, TerrainType a_type);
	PrimStruct* getStruct();
	D3DXVECTOR3 getPos();
	TerrainType getType();
};




class PhysicsSystem {
private:
	float closestPoint(float, float, float);
public:
	PhysicsSystem();
	~PhysicsSystem();
	bool SenseCollision(Player& a_player, Terrain &a_terrain);
	bool SenseCollision(Player& a_player, Bullet &a_bullet);
	bool SenseCollision(Terrain &a_terrain, Bullet &a_bullet);
	bool ResolveCollision(Player& a_player, Terrain &a_terrain);
	bool ResolveCollision(Player& a_player, Bullet &a_bullet);
	bool ResolveCollision(Terrain &a_terrain, Bullet &a_bullet);

};