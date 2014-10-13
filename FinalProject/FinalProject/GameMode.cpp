#include "GameMode.h"
#include <cmath>

GameMode::GameMode() {
	warmUpMins = 2;
	roundMins = 15;
	playersAlive = 0;
}

void GameMode::intit(Map* map,DXFrame* DXVid,int warmUp,int round) {
	level = map;
	render = DXVid;
}

void GameMode::update(double dt,PlayerVec& playVec) {
	Player* tempPlay;
	int ran;
	if(curState != battle) {
		timeLeft -= dt;
		for(int i = 0; i < playVec.GetNumPlayers(); ++i) {
			tempPlay = &playVec.GetPlayer(i);
			if(!tempPlay->isAlive()) {
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
				for(int  i = 0; i < playVec.GetNumPlayers(); ++i) {
					playVec.GetPlayer(i).setSpec(false);
					ran = rand()%level->numSpawn();
					playVec.GetPlayer(i).respawn(level->GetSpawn(ran));
				}
			} else if(curState == mainRound) {
				curState = battle;
				timeLeft = 0;
				//count living players
				playersAlive = 0;
				for(int i = 0; i < playVec.GetNumPlayers();++i) {
					if(playVec.GetPlayer(i).isAlive())
						++playersAlive;
				}
			} else if(curState == end) {
				Restart(playVec);
			}
		}
	} else {
		for(int i = 0; i < playVec.GetNumPlayers(); ++i) {
			tempPlay = &playVec.GetPlayer(i);
			if(!tempPlay->isSpectator()) {
				if(!tempPlay->isAlive()) {
					--playersAlive;
					tempPlay->setSpec(true);
				}
			}
		}
		if(playersAlive <= 1) {
			curState = end;
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
		playVec.GetPlayer(i).setSpec(false);
		ran = rand()%level->numSpawn();
		playVec.GetPlayer(i).respawn(level->GetSpawn(ran));
	}
	Start();
}