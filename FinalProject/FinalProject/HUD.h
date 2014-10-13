#pragma once
#include "Player.h"
#include "GameMode.h"
//#include "DXBase.h"

class HUD {
private:
	TextStruct hp;
	TextStruct timer;
public:
	void drawHud(Player& player, DXFrame& frame, GameMode& gMode,int camNum);
};