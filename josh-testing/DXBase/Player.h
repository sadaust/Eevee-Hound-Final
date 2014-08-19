#pragma once
#include "DXFrame.h"
#include "InputHandler.h"
#include "PhysicsSystem.h"
#include "PhysicsSystem.h"





class Player {
private: 
	D3DXVECTOR3 pos;
	D3DXVECTOR2 velocityXZ;
	float facing, 
		moving, 
		velocityY,
		speed,
		height,
		radius;
	bool onGround;
public:
	Player();
	~Player();
	void testInit(float a_x, float a_z, float a_rot);
	void Update(XINPUT_STATE a_input, float a_dt, float &a_rot, float &a_angle, PrimObj &a_ground);
	void testUpdate(float a_x, float a_z, float a_rot);
	D3DXVECTOR3 getPos();
	float getFacing();
	float getMoving();
	D3DXVECTOR3 getVelocityXZ();
	float getVelocityY();
	void setPosInts(float a_x, float a_y, float a_z);
	void setPos(D3DXVECTOR3 a_pos);
	void setFacing(float a_facing);
	void setMoving(float a_moving);
	void setVelocityInts(float a_x, float a_y, float a_z);
	void setVelocityXZ(D3DXVECTOR2 a_velocityXZ);
	void setVelocityY(float a_velocityY);
};





