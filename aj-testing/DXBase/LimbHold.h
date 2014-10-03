#pragma once


class PlayerLimbs{
private:
	int Head;
	int R_Arm;
	int L_Arm;
	int Leg;
	int Body;
public:
	PlayerLimbs(){Head=0;R_Arm=0;L_Arm=0;Leg=0;Body=0;}
	void setHead(int h_id){Head=h_id;}
	void setRarm(int ra_id){R_Arm=ra_id;}
	void setLarm(int la_id){L_Arm=la_id;}
	void setLeg(int l_id){Leg=l_id;}
	void setBody(int b_id){Body=b_id;}
	int getHead(){return Head;}
	int getRarm(){return R_Arm;}
	int getLarm(){return L_Arm;}
	int getLeg(){return Leg;}
	int getBody(){return Body;}
	~PlayerLimbs();
};