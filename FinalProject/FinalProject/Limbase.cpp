#include "Limbase.h"
#include "Player.h"
Limbase::Limbase(){
	// to be made to read from text file.
	part[0].setID(1);
	part[0].setName("Turtle Head");
   part[0].setType(head);
   part[1].setID(2);
   part[1].setName("Turtle Arm");
   part[1].setType(arm);
   part[2].setID(3);
   part[2].setName("Turtle Leg");
   part[2].setType(leg);
   part[3].setID(4);
   part[3].setName("Turtle Body");
   part[3].setType(body);
     part[4].setID(5);
   part[4].setName("Bear Head");
   part[4].setType(head);
   part[5].setID(6);
   part[5].setName("Bear Arm");
   part[5].setType(arm);
   part[6].setID(7);
   part[6].setName("Bear Leg");
   part[6].setType(leg);
   part[7].setID(8);
   part[7].setName("Bear Body");
   part[7].setType(body);
     part[8].setID(9);
   part[8].setName("Tiger Head");
   part[8].setType(head);
   part[9].setID(10);
   part[9].setName("Tiger Arm");
   part[9].setType(arm);
   part[10].setID(11);
   part[10].setName("Tiger Leg");
   part[10].setType(leg);
   part[11].setID(12);
   part[11].setName("Tiger Body");
   part[11].setType(body);
     part[12].setID(13);
   part[12].setName("Pheonix Head");
   part[12].setType(head);
   part[13].setID(14);
   part[13].setName("Pheonix Arm");
   part[13].setType(arm);
   part[14].setID(15);
   part[14].setName("Pheonix Leg");
   part[14].setType(leg);
   part[15].setID(16);
   part[15].setName("Pheonix Body");
   part[15].setType(body);

   bearactive = false;
}

Limbase::~Limbase(){

}

Limb Limbase::getPartRan(){
	//int num;
	//num=rand()%NUMPARTS;
	int num[]={1,5,9,13,14,10};
	int num2;
	num2=rand()%TECHFAIR;
	return part[num[num2]];
}

Limb Limbase::getPart(int partnum){
	return part[partnum];
}

void Limbase::CaseAction(int partNumber,Player& p_data, inputState& a_state,BulletVec &testBullVec,float& a_rot,float& a_angle){
	//partNumber=18;
	int tempInt = 0;
	int tempdamage=0;
	switch (part[partNumber-1].getPartType()){
	case arm:
		{
			if (part[partNumber-1].getPartName()=="Pheonix Arm"){
				//shoot fireball
				tempInt = 0;
				tempdamage=2;
				armfire(p_data,a_state,testBullVec,a_rot,a_angle,tempInt,tempdamage);
			}
			else if (part[partNumber-1].getPartName()=="Tiger Arm"){
				//rock throw
				tempInt = 2;
				tempdamage=15;
				armfire(p_data,a_state,testBullVec,a_rot,a_angle,tempInt,tempdamage);
			}
			else if (part[partNumber-1].getPartName()=="Bear Arm"){
				//bear grab
				tempInt = 3;
				tempdamage=5;
				armfire(p_data,a_state,testBullVec,a_rot,a_angle,tempInt,tempdamage);
			}
			else if (part[partNumber-1].getPartName()=="Turtle Arm"){
				//lazer beam
				tempInt = 1;
				tempdamage=20;
				armfire(p_data,a_state,testBullVec,a_rot,a_angle,tempInt,tempdamage);
			}
			else break;
		}
	case leg:
		{
			if (part[partNumber-1].getPartName()=="Pheonix Leg"){
				//tripple jump
				//increase jump count function to be made
				if(p_data.getGrounded()==false&&p_data.getJumpCount()<2){
					p_data.setVelocityY(jumpHeight);
					p_data.incrementJumpCount();
					//if(p_data.getJumpCount()==2){
						//p_data.setJumpCount(0);
					//}
				}
				
			}
			else if (part[partNumber-1].getPartName()=="Tiger Leg"){
				//minor teleport
				D3DXVECTOR2 move;
				float tempfloat;
				tempfloat = a_state.lX;
				move.x = tempfloat*5.0f;
				tempfloat = a_state.lY;
				move.y = tempfloat*5.0f;
				p_data.setVelocityXZ(move);
			}
			else if (part[partNumber-1].getPartName()=="Bear Leg"){
				//
			}
			else if (part[partNumber-1].getPartName()=="Turtle Leg"){
				//swim speed boost
			}
			else break;
		}
	case body:  // body parts to be added to passive will not be in case in the end.
		{
			if (part[partNumber-1].getPartName()=="Pheonix Body"){
				//damage over time aura

			}
			else if (part[partNumber-1].getPartName()=="Tiger Body"){
				//chance to not receive damage
			}
			else if (part[partNumber-1].getPartName()=="Bear Body"){
				//HP increased
				if(bearactive==false){
					p_data.setMaxHealth(200);
					bearactive==true;
				}
			}
			else if (part[partNumber-1].getPartName()=="Turtle Body"){
				//damage taken reduced
			}
			else break;
		}
	case head: // headparts to be added to passive will not be in case in the end
		{
			if (part[partNumber-1].getPartName()=="Pheonix Head"){
				// all enemy players displayed on the map
				// lazer beam eyes
			}
			else if (part[partNumber-1].getPartName()=="Tiger Head"){
				//target rect on screen
			}
			else if (part[partNumber-1].getPartName()=="Bear Head"){
				// chance to stun enemy on hit
			}
			else if (part[partNumber-1].getPartName()=="Turtle Head"){
				// chance to poison enemy on attack.
			}
			else break;
		}
	}

}

void Limbase::armfire(Player& p_data, inputState& a_state,BulletVec &testBullVec,float& a_rot,float& a_angle,int a_type,int a_damage){
	if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
		//D3DXVECTOR3 tempvec = p_data.getPos();
		//tempvec.y += 1.5f;
		D3DXVECTOR2 tempvec;
		tempvec.y = p_data.getBound().radius; // z
		tempvec.x = -p_data.getBound().radius; // x
		rotate2Dvector(&tempvec, D3DXToRadian(-p_data.getFacing()));
		testBullVec.ActivateABullet(D3DXVECTOR3(tempvec.x + p_data.getPos().x, 1.4f + p_data.getPos().y, 
			tempvec.y + p_data.getPos().z),D3DXVECTOR3(0,0,-BulletSpeed),a_type,a_rot,a_angle,RangedDefaultLifeSpan,a_damage);
		}
	else if(a_state.buttons[binds::rightAttack]&&!a_state.buttonLast[binds::rightAttack]) { 
		//D3DXVECTOR3 tempvec = p_data.getPos();
		//tempvec.y += 1.5f;
		D3DXVECTOR2 tempvec;
		tempvec.y = p_data.getBound().radius; // z
		tempvec.x = +p_data.getBound().radius; // x
		rotate2Dvector(&tempvec, D3DXToRadian(-p_data.getFacing()));
		testBullVec.ActivateABullet(D3DXVECTOR3(tempvec.x + p_data.getPos().x, 1.4f + p_data.getPos().y,
			tempvec.y + p_data.getPos().z),D3DXVECTOR3(0,0,-BulletSpeed),a_type,a_rot,a_angle,RangedDefaultLifeSpan,a_damage);
		}
}