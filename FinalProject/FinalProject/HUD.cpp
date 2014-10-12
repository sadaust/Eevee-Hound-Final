#include "HUD.h"
#include <sstream>

void HUD::drawHud(Player& player, DXFrame& frame,int camNum) {
	RenInfo ren;
	std::stringstream ss;
	ren.locCamNum = camNum;
	ren.type = text;
	hp.textColor = D3DCOLOR(0xFFFFFFFF);
	if(player.isAlive()) {
	ss<<player.getHealth();
	hp.text = ss.str();
	hp.rec.bottom = .99f;
	hp.rec.top = .95f;
	hp.rec.left = .01f;
	hp.rec.right = .1f;
	} else {
		ss<<"RESPAWN IN "<<(int)player.getTimer();
		hp.text = ss.str();
		hp.rec.bottom = .52f;
		hp.rec.top = .48f;
		hp.rec.left = .4f;
		hp.rec.right = .4f;
	}
	ren.asset = &hp;
	frame.addRen(ren);
}
