#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Limb.h"
#include "DXFrame.h"
#include "InputHandler.h"
//#include "Player.h"

class Player;

#define NUMPARTS 20



class Limbase {
private:
	Limb part[NUMPARTS];
	bool bearactive;
public:
	bool activebear(){return bearactive;}
	void toggleBear(bool a_bear);
	Limbase();
	Limb getPartRan();
	Limb getPart(int);
	void CaseAction(int,Player&,inputState&);
	//~Limbase();
};
