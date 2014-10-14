#include "GameMode.h"
#include <cmath>

GameMode::GameMode() {
	warmUpMins = 2;
	roundMins = 15;
	playersAlive = 0;
	light = 70;
}

void GameMode::intit(Map* map,DXFrame* DXVid,int warmUp,int round) {
	level = map;
	render = DXVid;
	warmUpMins = warmUp;
	roundMins = round;
}

void GameMode::update(double dt,PlayerVec& playVec) {
	Player* tempPlay;
	int ran;
	level->update(dt);
	if(curState != battle && curState != transition) {
		timeLeft -= dt;
		for(int i = 0; i < MAXPLAYERS; ++i) {
			tempPlay = &playVec.GetPlayer(i);
			if(playVec.GetActive(i)&&!tempPlay->isAlive()) {
				if(tempPlay->getTimer()<=0) {
					ran = rand()%level->numSpawn();
					tempPlay->respawn(level->GetSpawn(ran));
				}	
			}
		}
		if(timeLeft <= 0) {
			if(curState == warmUp) {
				curState = mainRound;
				timeLeft = roundMins*60;
				//respawn all players
				for(int  i = 0; i < MAXPLAYERS; ++i) {
					if(playVec.GetActive(i)) {
						playVec.GetPlayer(i).setSpec(false);
						ran = rand()%level->numSpawn();
						playVec.GetPlayer(i).respawn(level->GetSpawn(ran));
					}
				}
			} else if(curState == mainRound) {
				curState = transition;
				timeLeft = 60;
				//count living players
				playersAlive = 0;
				for(int i = 0; i < MAXPLAYERS;++i) {
					if(playVec.GetActive(i)&&playVec.GetPlayer(i).isAlive())
						++playersAlive;
				}
			} else if(curState == end) {
				Restart(playVec);
				light = 70;
			}
		}
	} else if(curState == battle||curState == transition) {
		for(int i = 0; i < MAXPLAYERS; ++i) {
			tempPlay = &playVec.GetPlayer(i);
			if(playVec.GetActive(i)&&!tempPlay->isSpectator()) {
				if(playVec.GetActive(i)&&!tempPlay->isAlive()) {
					--playersAlive;
					tempPlay->setSpec(true);
					tempPlay->setCurHealth(0);
					tempPlay->setMaxHealth(1);
				}
			}
		}
		if(curState == transition) {
			timeLeft -= dt;
			light -= dt;
			if(timeLeft <= 0) {
				light = 10;
				curState = battle;
				timeLeft = 0;
			}
		}
		if(playersAlive <= 1) {
			curState = end;
			light = 70;
			timeLeft = 30;
		}
	} 
}

gameModeState GameMode::getState() {
	return curState;
}

double GameMode::getTime() {
	return timeLeft;
}

void GameMode::Start() {
	curState = warmUp;
	timeLeft = warmUpMins*60;
}

int GameMode::getAliveCount() {
	return playersAlive;
}

void GameMode::Restart(PlayerVec &playVec) {
	int ran;
	for(int  i = 0; i < playVec.GetNumPlayers(); ++i) {
		if(playVec.GetActive(i)) {
			playVec.GetPlayer(i).setSpec(false);
			ran = rand()%level->numSpawn();
			playVec.GetPlayer(i).respawn(level->GetSpawn(ran));
		}
	}
	Start();
}