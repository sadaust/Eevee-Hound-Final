#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>

#define NUMPARTS 20

enum Limbtype {
	head,
	arm,
	leg,
	body
};

class Limb{
private:
	int ID;
	Limbtype type;
	std::string name;
public:
	void setID(int x){ID=x;}
	void setType(Limbtype l_type){type=l_type;}
	void setName(std::string P_name){name=P_name;}
	int getPartID(){return ID;}
	Limbtype getPartType(){return type;}
	std::string getPartName(){return name;}
};


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
