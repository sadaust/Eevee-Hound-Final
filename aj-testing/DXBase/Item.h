#pragma once
#include "ShapeDefs.h"
#include "ResDefs.h"
#include "DXFrame.h"

class ItemBox{
private:
	cylinder bounding;
	D3DXVECTOR3 pos, prospectivePos;
	PrimObj itemBox;
	int partID;

public:
	ItemBox();
	void ItemBoxInit(float a_x, float a_y, float a_z);
	D3DXVECTOR3 getPos();
	D3DXVECTOR3 getProspectivePos();
	cylinder getBound();
	void setBound(float a_h, float a_r);
	void setPosInts(float a_x, float a_y, float a_z);
	void setPos(D3DXVECTOR3 a_pos);
	void setProspectivePos(D3DXVECTOR3 a_prospos);
	int getPartID(){return partID;}
	void setPartID(int p_ID){partID=p_ID;}
	void setPrim(PrimObj object){itemBox=object;}
	PrimObj getPrim(){return itemBox;}

};
