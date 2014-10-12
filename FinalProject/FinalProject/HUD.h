#pragma once
#include "Player.h"
//#include "DXBase.h"

class HUD {
private:
	TextStruct hp;
	//TextStruct timer;
public:
	void drawHud(Player& player, DXFrame& frame,int camNum);
};