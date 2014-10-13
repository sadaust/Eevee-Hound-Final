#pragma once
#include "DXFrame.h"
#include "InputHandler.h"
#include "ShapeDefs.h"
#include "PlayerLimbs.h"
#include "Limb.h"
#include "Limbase.h"
#include "ItemBox.h"
#include "spawnPoint.h"
#include "Bullet.h"
#include "Map.h"
//#include "PhysicsSystem.h"

class Limbase;
class ItemBox;
class BulletVec;

void rotate2Dvector(D3DXVECTOR2* a_vector, float a_angle);
void rotate3Dvector(D3DXVECTOR3* a_vector, float a_rot, float a_angle);


const int numPlayerPrims = 6;
const int PrimTypes = 3;
const int MAXPLAYERS = 16;

const float jumpHeight  = 13.0f;
const float gravity = 20.0f;

class Player {
private: 
	PlayerLimbs Limbs;
	PrimObj playerPrims[numPlayerPrims]; // 0 is body, 1 is head, 2 and 3 are arms, 4 and 5 are legs
	D3DXVECTOR3 pos, prospectivePos;
	D3DXVECTOR2 velocityXZ;
	float facing,
		angle,
		moving, 
		speed,
		timer;
	double velocityY;
	cylinder boundingCyl;
	bool onGround, checkItem, R_arm, alive, spectator;
	int jumpCount;

	Limb ProsteticTestLimb;

	  ///////////////////////////
	 // Game Statistics/Math  //
	///////////////////////////
	int curHealth, maxHealth;


public:
	void takeDamage(int a_damage);
	int getHealth();
	void setMaxHealth(int m_health);
	void setCurHealth(int c_health);
	int getMaxHealth();


	  //////////////////////////// ~~~~~~~~~~~~
	 // End of game Statistics // ~~~~~~~~~~~~
	//////////////////////////// ~~~~~~~~~~~~
private:



public:
	Player();
	~Player();
	void Init(sPoint& spawn, PrimObj a_primDefs[]);
	void Update(inputState& a_input, double a_dt,Limbase& part_list,BulletVec &a_bulvec);
	void Render(DXFrame& DXVid);
	void testUpdate(float a_x, float a_z, float a_rot);
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	float getFacing();
	float getAngle();
	float getMoving();
	float getTimer(){return timer;}
	D3DXVECTOR2 getVelocityXZ();
	float getVelocityY();
	cylinder getBound();
	void setBound(float a_h, float a_r);
	void setPosInts(float a_x, float a_y, float a_z);
	void setPos(D3DXVECTOR3 a_pos);
	void setProspectivePos(D3DXVECTOR3 a_prospos);
	void setFacing(float a_facing);
	void setAngle(float a_angle);
	void setMoving(float a_moving);
	void setVelocityInts(float a_x, float a_y, float a_z);
	void setVelocityXZ(D3DXVECTOR2 a_velocityXZ);
	void setVelocityY(float a_velocityY);
	void toggleGrounded(bool a_ground);
	void toggleRarm(bool right_a);
	void togglecheckItem(bool item);
	bool getGrounded();
	bool getcheckItem();
	bool isAlive();
	bool isSpectator();
	void setSpec(bool);
	void addLimb(Limb part);
	void setJumpCount(int j_count);
	int getJumpCount();

	void incrementJumpCount();
	void itemAccess(ItemBox &a_item);
	void respawn(sPoint& spawn);

	PrimObj getPrimObj(int a_index);
	void setPrimObj(PrimObj a_prim, int a_index);


};





class PlayerVec {
private:
	Player players[MAXPLAYERS];
	bool active[MAXPLAYERS];
	int numPlayers;
	PrimObj playerDefaultPrims[numPlayerPrims]; // 0 is body, 1 is head, 2 and 3 are arms, 4 and 5 are legs
	D3DMATERIAL9  PlayMat; // used for the player primobj's
public:
	PlayerVec();
	~PlayerVec();
	void Init(Map& a_man, ResourceManager& resMan);
	void Update(inputState& a_input, double a_dt, Limbase part_list,BulletVec &a_bulvec);
	void Render(DXFrame& DXVid);
	void ActivateAPlayer(Map& a_map);
	void DeactivateAPlayer(int a_index);
	Player& GetPlayer(int a_index);
	int GetNumPlayers();
};






