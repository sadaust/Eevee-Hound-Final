#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Limb.h"

#define NUMPARTS 20



class Limbase {
private:
	Limb part[NUMPARTS];
	//std::ifstream infile; //still testing
	std::stringstream ss;
public:
	Limbase();
	Limb getPartRan();
	Limb getPart(int);
	void CaseAction(Limb);
	~Limbase();
};
