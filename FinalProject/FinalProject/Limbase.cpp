#include "Limbase.h"
#include "Player.h"
Limbase::Limbase(){
	// to be made to read from text file.
	part[0].setID(1);
	part[0].setName("Turtle");
   part[0].setType(head);
   part[1].setID(2);
   part[1].setName("Turtle");
   part[1].setType(arm);
   part[2].setID(3);
   part[2].setName("Turtle");
   part[2].setType(leg);
   part[3].setID(4);
   part[3].setName("Turtle");
   part[3].setType(body);
    part[4].setID(5);
   part[4].setName("Monkey");
   part[4].setType(head);
   part[5].setID(6);
   part[5].setName("Monkey");
   part[5].setType(arm);
   part[6].setID(7);
   part[6].setName("Monkey");
   part[6].setType(leg);
   part[7].setID(8);
   part[7].setName("Monkey");
   part[7].setType(body);
     part[8].setID(9);
   part[8].setName("Bear");
   part[8].setType(head);
   part[9].setID(10);
   part[9].setName("Bear");
   part[9].setType(arm);
   part[10].setID(11);
   part[10].setName("Bear");
   part[10].setType(leg);
   part[11].setID(12);
   part[11].setName("Bear");
   part[11].setType(body);
     part[12].setID(13);
   part[12].setName("Tiger");
   part[12].setType(head);
   part[13].setID(14);
   part[13].setName("Tiger");
   part[13].setType(arm);
   part[14].setID(15);
   part[14].setName("Tiger");
   part[14].setType(leg);
   part[15].setID(16);
   part[15].setName("Tiger");
   part[15].setType(body);
     part[16].setID(17);
   part[16].setName("Pheonix");
   part[16].setType(head);
   part[17].setID(18);
   part[17].setName("Pheonix");
   part[17].setType(arm);
   part[18].setID(19);
   part[18].setName("Pheonix");
   part[18].setType(leg);
   part[19].setID(20);
   part[19].setName("Pheonix");
   part[19].setType(body);

   bearactive = false;
}

Limb Limbase::getPartRan(){
	int num;
	num=rand()%NUMPARTS;
	return part[num];
}

Limb Limbase::getPart(int partnum){
	return part[partnum];
}

void Limbase::CaseAction(int partNumber,Player& p_data, inputState& a_state,BulletVec &testBullVec,float& a_rot,float& a_angle){
	//partNumber=18;
	switch (part[partNumber-1].getPartType()){
	case arm:
		{
			if (part[partNumber-1].getPartName()=="Pheonix"){
				//shoot fireball
				if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
					D3DXVECTOR3 tempvec = p_data.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),0,a_rot,a_angle,RangedDefaultLifeSpan, testDamage);
				}
			}
			else if (part[partNumber-1].getPartName()=="Tiger"){
				//rock throw
				if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
					D3DXVECTOR3 tempvec = p_data.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),2,a_rot,a_angle,RangedDefaultLifeSpan, testDamage);
				}
			}
			else if (part[partNumber-1].getPartName()=="Bear"){
				//bear grab
				if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
					D3DXVECTOR3 tempvec = p_data.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),3,a_rot,a_angle,RangedDefaultLifeSpan, testDamage);
				}
			}
			else if (part[partNumber-1].getPartName()=="Turtle"){
				//lazer beam
				if(a_state.buttons[binds::leftAttack]&&!a_state.buttonLast[binds::leftAttack]) { 
					D3DXVECTOR3 tempvec = p_data.getPos();
					tempvec.y += 1.5f;
					testBullVec.ActivateABullet(tempvec,D3DXVECTOR3(0,0,-BulletSpeed),1,a_rot,a_angle,RangedDefaultLifeSpan, testDamage);
				}
			}
			else break;
		}
	case leg:
		{
			if (part[partNumber-1].getPartName()=="Pheonix"){
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
			else if (part[partNumber].getPartName()=="Tiger"){
				//minor teleport
				D3DXVECTOR2 move;
				float tempfloat;
				tempfloat = a_state.lX;
				move.x = tempfloat*5.0f;
				tempfloat = a_state.lY;
				move.y = tempfloat*5.0f;
				p_data.setVelocityXZ(move);
			}
			else if (part[partNumber-1].getPartName()=="Bear"){
				//
			}
			else if (part[partNumber-1].getPartName()=="Turtle"){
				//swim speed boost
			}
			else break;
		}
	case body:  // body parts to be added to passive will not be in case in the end.
		{
			if (part[partNumber-1].getPartName()=="Pheonix"){
				//damage over time aura

			}
			else if (part[partNumber-1].getPartName()=="Tiger"){
				//chance to not receive damage
			}
			else if (part[partNumber-1].getPartName()=="Bear"){
				//HP increased
				if(bearactive==false){
					p_data.setMaxHealth(200);
					bearactive==true;
				}
			}
			else if (part[partNumber-1].getPartName()=="Turtle"){
				//damage taken reduced
			}
			else break;
		}
	case head: // headparts to be added to passive will not be in case in the end
		{
			if (part[partNumber-1].getPartName()=="Pheonix"){
				// all enemy players displayed on the map
				// lazer beam eyes
			}
			else if (part[partNumber-1].getPartName()=="Tiger"){
				//target rect on screen
			}
			else if (part[partNumber-1].getPartName()=="Bear"){
				// chance to stun enemy on hit
			}
			else if (part[partNumber-1].getPartName()=="Turtle"){
				// chance to poison enemy on attack.
			}
			else break;
		}
	}

}