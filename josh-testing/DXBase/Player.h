#pragma once
#include "DXFrame.h"
#include "InputHandler.h"
#include "ShapeDefs.h"
#include "PlayerLimbs.h"
#include "Limb.h"
#include "Limbase.h"
#include "ItemBox.h"
#include "spawnPoint.h"
//#include "PhysicsSystem.h"

void rotate2Dvector(D3DXVECTOR2* a_vector, float a_angle);
void rotate3Dvector(D3DXVECTOR3* a_vector, float a_rot, float a_angle);


class Player {
private: 
	PlayerLimbs Limbs;
	D3DXVECTOR3 pos, prospectivePos;
	D3DXVECTOR2 velocityXZ;
	float facing, 
		moving, 
		velocityY,
		speed,
		timer;
	cylinder boundingCyl;
	bool onGround, checkItem, R_arm, alive;
	int jumpCount;

	Limb ProsteticTestLimb;

	  ///////////////////////////
	 // Game Statistics/Math  //
	///////////////////////////
	int curHealth, maxHealth;


public:
	void takeDamage(int a_damage);


	  //////////////////////////// ~~~~~~~~~~~~
	 // End of game Statistics // ~~~~~~~~~~~~
	//////////////////////////// ~~~~~~~~~~~~
private:



public:
	Player();
	~Player();
	void testInit(float a_x, float a_y, float a_z, float a_rot);
	void Update(inputState& a_input, float a_dt, float &a_rot, float &a_angle,Limbase part_list);
	void testUpdate(float a_x, float a_z, float a_rot);
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	float getFacing();
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
	void addLimb(Limb part);
	void setJumpCount(int j_count);
	int getJumpCount();
	int getHealth();
	void incrementJumpCount();
	void itemAccess(ItemBox &a_item);
	void respawn(sPoint& spawn);
	void setMaxHealth(int m_health);
	void setCurHealth(int c_health);
	int getMaxHealth();
};





