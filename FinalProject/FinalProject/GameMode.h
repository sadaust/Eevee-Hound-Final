#pragma once

#include "Map.h"
#include "Player.h"

enum gameModeState {
	warmUp,
	mainRound,
	transition,
	battle,
	end
};

class GameMode {
private:
	gameModeState curState;
	Map* level;
	DXFrame* render;
	double timeLeft;
	double light;
	int warmUpMins;
	int roundMins;
	int playersAlive;
public:
	GameMode();
	void intit(Map* map,DXFrame* DXVid,int warmUp,int round);
	void update(double dt,PlayerVec& playVec);
	gameModeState getState();
	double getTime();
	int getAliveCount();
	void Start();
	double getLight() {return light;}
	void Restart(PlayerVec& playVec);
};