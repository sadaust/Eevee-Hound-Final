#include "HUD.h"
#include <sstream>

void HUD::drawHud(Player& player, DXFrame& frame,GameMode& gMode,int camNum) {
	RenInfo ren;
	std::stringstream ss;
	int temp;
	ren.locCamNum = camNum;
	ren.type = text;
	hp.textColor = D3DCOLOR(0xFFFFFFFF);
	timer.textColor = D3DCOLOR(0xFFFFFFFF);
	if(!player.isSpectator()){
		if(player.isAlive()) {
			ss<<player.getHealth();
			hp.text = ss.str();
			hp.rec.bottom = .99f;
			hp.rec.top = .95f;
			hp.rec.left = .01f;
			hp.rec.right = .1f;

			ss.str("");
			ss<<player.getProsteticTestLimb().getPartName();
			item.text = ss.str();
			item.rec.bottom = 0.05f;
			item.rec.top = 0.9f;
			item.rec.left = 0.49f;
			item.rec.right = 0.51f;
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
		ren.asset = &item;
		frame.addRen(ren);
	}

	ss.str("");
	timer.rec.top = 0.01;
	timer.rec.bottom = 0.08;
	timer.rec.left = 0.49;
	timer.rec.right = 0.51;
	if(gMode.getState() != battle && gMode.getState()!= transition) {
		if(gMode.getState() == warmUp) {
			ss<<"Round Start In: ";
		} else if(gMode.getState() == mainRound) {
			ss<<"Time Left: ";
		} else {
			ss<<"New Game In: ";
		}
		temp = (int)((int)gMode.getTime())/60;
		ss<<temp<<':';
		temp = ((int)gMode.getTime())%60;
		if(temp < 10)
			ss<<'0';
		ss<<temp;
	} else {
		ss<<"Players Alive: "<<gMode.getAliveCount()<<'\n';
		if(gMode.getState() == transition) {
			temp = ((int)gMode.getTime())%60;
			ss<<"Get To The Arena: 0:";
			if(temp<10)
				ss<<'0';
			ss<<temp;
		}
	}
	timer.text = ss.str();
	ren.asset = &timer;
	frame.addRen(ren);

}
