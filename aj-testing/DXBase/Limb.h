#pragma once

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
	Limb(){ID=2;type=arm;name="Turtle";} // initialzed for testing
	void LimbInit(){ID=2;type=arm;name="Turtle";} // ""
	void setID(int x){ID=x;}
	void setType(Limbtype l_type){type=l_type;}
	void setName(std::string P_name){name=P_name;}
	int getPartID(){return ID;}
	Limbtype getPartType(){return type;}
	std::string getPartName(){return name;}
};
