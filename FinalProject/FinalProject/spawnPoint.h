#pragma once
#include "ResDefs.h"
class sPoint {
private:
	D3DXVECTOR3 pos;
	float rot;
	bool active;
public:
	sPoint() {active = true;pos.x = 0; pos.y = 0;pos.z = 0;rot = 0;}
	bool isActive(){return active;}
	D3DXVECTOR3& getPos(){return pos;}
	float getRot(){return rot;}
	void setPoint(D3DXVECTOR3 a_pos,float a_rot) {pos = a_pos; rot = a_rot;}
	void toggle(){active = !active;}
};