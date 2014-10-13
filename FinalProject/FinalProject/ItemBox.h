#pragma once
#include "ShapeDefs.h"
#include "ResDefs.h"
#include "DXFrame.h"
#include "Limbase.h"
#include "spawnPoint.h"
#include "Map.h"

class Limbase;

const int MAXITEMS = 30;

class ItemBox{
private:
	PrimObj a_box;
	cylinder bounding;
	D3DXVECTOR3 pos, prospectivePos;
	Limb part;
	bool onGround;
	float velocityY;
	D3DXVECTOR2 velocityXZ;
	bool active;

public:
	ItemBox();
	void ItemBoxInit(sPoint& spawn, PrimObj a_primObj);
		//(float a_x, float a_y, float a_z);
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
	void Render(DXFrame& DXVid,float dt);
	~ItemBox();

};



class ItemVec {
private:
	ItemBox a_itembox[MAXITEMS];
	bool active[MAXITEMS];
	int numItems;
	PrimObj itemObj;
	D3DMATERIAL9  itemMat;
public:
	ItemVec();
	void Init(Map& a_man, ResourceManager& resMan);
	void Update(inputState& a_input, double a_dt, Limbase part_list);
	void Render(DXFrame& DXVid,float dt);
	void ActivateAItem(Map& a_map);
	void DeactivateAItem(int a_index);
	ItemBox& GetItem(int a_index);
	int GetNumItems();
	~ItemVec();
};
