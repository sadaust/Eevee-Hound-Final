#include "Limbase.h"

Limbase::Limbase()
{
	/*/ testing
	infile.open("PartList.txt");
	for (int i=0;i<NUMPARTS;i++)
	{
		getline(infile,ss, ';');
		int id;
		std::string name;
		int typehold;
		ss>>id;
		ss>>name;
		ss>>typehold;
		Limbtype type;
		if (typehold==0)
			type=head;
		if (typehold==1)
			type=arm;
		if (typehold==2)
			type=leg;
		if (typehold==3)
			type=body;
		part[i].ID=id;
		part[i].name=name;
		part[i].type=type;
	}
	*/
	// to be made to read from text file.
	part[0].setID(0);
	part[0].setName("Turtle");
   part[0].setType(head);
   part[1].setID(1);
   part[1].setName("Turtle");
   part[1].setType(arm);
   part[2].setID(2);
   part[2].setName("Turtle");
   part[2].setType(leg);
   part[3].setID(3);
   part[3].setName("Turtle");
   part[3].setType(body);
    part[4].setID(4);
   part[4].setName("Monkey");
   part[4].setType(head);
   part[5].setID(5);
   part[5].setName("Monkey");
   part[5].setType(arm);
   part[6].setID(6);
   part[6].setName("Monkey");
   part[6].setType(leg);
   part[7].setID(7);
   part[7].setName("Monkey");
   part[7].setType(body);
     part[8].setID(8);
   part[8].setName("Bear");
   part[8].setType(head);
   part[9].setID(9);
   part[9].setName("Bear");
   part[9].setType(arm);
   part[10].setID(10);
   part[10].setName("Bear");
   part[10].setType(leg);
   part[11].setID(11);
   part[11].setName("Bear");
   part[11].setType(body);
     part[12].setID(12);
   part[12].setName("Tiger");
   part[12].setType(head);
   part[13].setID(13);
   part[13].setName("Tiger");
   part[13].setType(arm);
   part[14].setID(14);
   part[14].setName("Tiger");
   part[14].setType(leg);
   part[15].setID(15);
   part[15].setName("Tiger");
   part[15].setType(body);
     part[16].setID(16);
   part[16].setName("Pheonix");
   part[16].setType(head);
   part[17].setID(17);
   part[17].setName("Pheonix");
   part[17].setType(arm);
   part[18].setID(18);
   part[18].setName("Pheonix");
   part[18].setType(leg);
   part[19].setID(19);
   part[19].setName("Pheonix");
   part[19].setType(body);
}

Limb Limbase::getPartRan()
{
	int num;
	num=rand()%NUMPARTS;
	return part[num];
}

Limb Limbase::getPart(int partnum)
{
	return part[partnum];
}

void Limbase::CaseAction(Limb part)
{
	switch (part.getPartType())
	{
	case arm:
		{
			if (part.getPartName()=="Pheonix")
			{
				//shoot fireball
			}
			else if (part.getPartName()=="Tiger")
			{
				//claw attack
			}
			else if (part.getPartName()=="Bear")
			{
				//bearpull
			}
			else if (part.getPartName()=="Turtle")
			{
				//throw slwoing bubble
			}
			else break;
		}
	case leg:
		{
			if (part.getPartName()=="Pheonix")
			{
				//tripple jump
			}
			else if (part.getPartName()=="Tiger")
			{
				//movement speed boost
			}
			else if (part.getPartName()=="Bear")
			{
				//
			}
			else if (part.getPartName()=="Turtle")
			{
				//swim speed boost
			}
			else break;
		}
	case body:  // body parts to be added to passive will not be in case in the end.
		{
			if (part.getPartName()=="Pheonix")
			{
				//damage over time aura

			}
			else if (part.getPartName()=="Tiger")
			{
				//chance to not receive damage
			}
			else if (part.getPartName()=="Bear")
			{
				//HP increased
			}
			else if (part.getPartName()=="Turtle")
			{
				//damage taken reduced
			}
			else break;
		}
	case head: // headparts to be added to passive will not be in case in the end
		{
			if (part.getPartName()=="Pheonix")
			{
				// all enemy players displayed on the map
			}
			else if (part.getPartName()=="Tiger")
			{
				//target rect on screen
			}
			else if (part.getPartName()=="Bear")
			{
				// chance to stun enemy on hit
			}
			else if (part.getPartName()=="Turtle")
			{
				// chance to poison enemy on attack.
			}
			else break;
		}
	}

}