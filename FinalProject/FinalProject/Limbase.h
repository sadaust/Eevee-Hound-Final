#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Limb.h"
#include "DXFrame.h"
#include "InputHandler.h"
#include "Bullet.h"
#include "SoundFrame.h"
#include "ResourceManager.h"
//#include "Player.h"

class Player;
class BulletVec;

#define NUMPARTS 16
#define TECHFAIR 6


class Limbase {
private:
	Limb part[NUMPARTS];
	bool bearactive;;
	SoundStruct *soundeffect;
public:
	bool activebear(){return bearactive;}
	void toggleBear(bool a_bear);
	Limbase();
	Limb getPartRan();
	Limb getPart(int);
	void CaseAction(int,Player&,inputState&,BulletVec&,float& a_rot,float& a_angle,ResourceManager& resMan,SoundFrame* sFrame);
	void armfire(Player&,inputState&,BulletVec&,float& a_rot,float& a_angle,int a_type,int a_damage,ResourceManager& resMan,SoundFrame* sFrame);
	~Limbase();
};
