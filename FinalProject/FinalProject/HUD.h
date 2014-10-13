#pragma once
#include "Player.h"
#include "GameMode.h"
//#include "DXBase.h"

class HUD {
private:
	TextStruct hp;
	TextStruct timer;
	TextStruct item;
public:
	void drawHud(Player& player, DXFrame& frame, GameMode& gMode,int camNum);
};