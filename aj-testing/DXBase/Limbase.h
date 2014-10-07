#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Limb.h"
#include "DXFrame.h"
//#include "Player.h"

class Player;

#define NUMPARTS 20



class Limbase {
private:
	Limb part[NUMPARTS];
public:
	Limbase();
	int getPartRan();
	Limb getPart(int);
	void CaseAction(int,Player&);
	//~Limbase();
};
