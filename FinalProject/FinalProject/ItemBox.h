#pragma once
#include "ShapeDefs.h"
#include "ResDefs.h"
#include "DXFrame.h"
#include "Limbase.h"

class ItemBox{
private:
	cylinder bounding;
	D3DXVECTOR3 pos, prospectivePos;
	Limb part;
	bool onGround;
	float velocityY;
	bool active;

public:
	ItemBox();
	void ItemBoxInit(float a_x, float a_y, float a_z);
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	cylinder getBound();
	void Update(float a_dt);
	void setBound(float a_h, float a_r);
	void setPosInts(float a_x, float a_y, float a_z);
	void setPos(D3DXVECTOR3 a_pos);
	void setProspectivePos(D3DXVECTOR3 a_prospos);
	Limb getPart(){return part;}
	void setPart(Limb a_part){part=a_part;}
	void toggleGrounded(bool a_ground);
	void setVelocityY(float a_velocityY);
	void toggleActive(bool is_active);
	bool getActive(){return active;}

};
