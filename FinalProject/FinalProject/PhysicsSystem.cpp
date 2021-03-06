#pragma once
#include "PhysicsSystem.h"




PhysicsSystem::PhysicsSystem() {

}


PhysicsSystem::~PhysicsSystem() {

}


float PhysicsSystem::closestPoint(float a_point, float a_1, float a_2) {
	if(abs(a_point-a_1) <= abs(a_point-a_2)) {
		return a_1; // returns VVVVVVVVVVVVVVVVVVVV
	}
	else {
		return a_2; // returns point that is closest
	}
}


void PhysicsSystem::DoCollisions(PlayerVec& a_playerVec, BulletVec& a_bulletVec, Map& a_map, ItemVec& a_itemVec) {

	// all player stuff but bullets
	for(int i = 0; i < MAXPLAYERS; ++i) {
		if(a_playerVec.GetActive(i)) {
			if(a_playerVec.GetPlayer(i).isAlive()) { // if player A is alive
				for(int g = 0; g < a_playerVec.GetNumPlayers(); ++g) { // PLAYER PLAYER COLLISION
					if(i != g && a_playerVec.GetPlayer(g).isAlive()&&a_playerVec.GetActive(g)) { // if target player is alive and isn't player A
						if(SenseCollision(a_playerVec.GetPlayer(i), a_playerVec.GetPlayer(g))) { // if they're colliding
							ResolveCollision(a_playerVec.GetPlayer(i), a_playerVec.GetPlayer(g)); // resolve the collision
						}
					}
				}
				// END OF PLAYER PLAYER

				for(int g = 0; g < a_map.numWalls(); ++g) { // PLAYER WALLS
					if(SenseCollision(a_playerVec.GetPlayer(i), a_map.GetWall(g))) { // if they're colliding
						ResolveCollision(a_playerVec.GetPlayer(i), a_map.GetWall(g)); // resolve the collision
					}
				}
				for(int g = 0; g < a_map.numFloors(); ++g) { // PLAYER FLOORS
					if(SenseCollision(a_playerVec.GetPlayer(i), a_map.GetFloor(g))) { // if they're colliding
						ResolveCollision(a_playerVec.GetPlayer(i), a_map.GetFloor(g)); // resolve the collision
					}
				}

				// END OF MAP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ADD ANOTHER FOR LOOP AND ANOTHER VECTOR FOR FLOORWALLS, MAYBE









			}
		}
	}



	// all bullet stuff
	for(int i = 0; i < MAXBULLETS; ++i) { // START OF BULLETS
		if(a_bulletVec.GetActive(i)) { // FOR EVERY ACTIVE BULLET
			for(int g = 0; g < a_playerVec.GetNumPlayers(); ++g) { // for every player
				if(a_playerVec.GetPlayer(g).isAlive()) {					// if the player is alive
					if(SenseCollision(a_playerVec.GetPlayer(g), a_bulletVec.GetBullet(i))) { // if they're colliding
						ResolveCollision(a_playerVec.GetPlayer(g),a_bulletVec.GetBullet(i)); // resolve the collision
						a_bulletVec.DeactivateABullet(i);
					}
				}
			}
			for(int g = 0; g < a_map.numWalls(); ++g) { // BULLET WALLS
				if(SenseCollision(a_map.GetWall(g), a_bulletVec.GetBullet(i))) { // if they're colliding
					ResolveCollision(a_map.GetWall(g), a_bulletVec.GetBullet(i)); // resolve the collision
					a_bulletVec.DeactivateABullet(i);
				}
			}
			for(int g = 0; g < a_map.numFloors(); ++g) { // BULLET FLOORS
				if(SenseCollision(a_map.GetFloor(g), a_bulletVec.GetBullet(i))) { // if they're colliding
					ResolveCollision(a_map.GetFloor(g), a_bulletVec.GetBullet(i)); // resolve the collision
					a_bulletVec.DeactivateABullet(i);
				}
			}








		}
	}
	
	for(int i = 0; i < MAXITEMS; ++i) { // 
		if(a_itemVec.GetActive(i)) {
			for(int g = 0; g < MAXPLAYERS; ++g) {
				if(a_playerVec.GetActive(g)) {
					if(a_playerVec.GetPlayer(g).isAlive()) {
						if(SenseCollision(a_playerVec.GetPlayer(g), a_itemVec.GetItem(i))) { // if they're colliding
							a_playerVec.GetPlayer(g).itemAccess(a_itemVec.GetItem(i));
							a_itemVec.DeactivateAItem(i);     //Aj: I find this ammusing keeping it here.//Josh: AJ I DON'T KNOW WHAT YOU WANT FROM ME BUT HERE IS THE SENSE COLLISION
						}
					}
				}
			}
			for(int g = 0; g < a_map.numWalls(); ++g) {
				if(SenseCollision(a_itemVec.GetItem(i), a_map.GetWall(g))) {
					ResolveCollision(a_itemVec.GetItem(i), a_map.GetWall(g));
				}

			}
			for(int g = 0; g < a_map.numFloors(); ++g) {
				if(SenseCollision(a_itemVec.GetItem(i), a_map.GetFloor(g))) {
					ResolveCollision(a_itemVec.GetItem(i), a_map.GetFloor(g));
				}

			}
		}
	}
}


bool PhysicsSystem::SenseCollision(ItemBox &a_item, Terrain &a_terrain) {
	float distX = 0.0f;
	float distZ = 0.0f;
	float closestX = 0.0f;
	float closestZ = 0.0f;
	float tempDist = 0.0f;
	if(a_item.getProspectivePos().y < a_terrain.getPos().y+a_terrain.getBound().top) { // y,y,top
		if(a_item.getProspectivePos().y > a_terrain.getPos().y+a_terrain.getBound().bottom-a_item.getBound().height) { //y,y,bottom
			////////////////////////
			// POSITION INSIDE    //
			////////////////////////
			if(a_item.getProspectivePos().x < a_terrain.getPos().x+a_terrain.getBound().right+a_item.getBound().radius) {//x,x,right
				if(a_item.getProspectivePos().x > a_terrain.getPos().x+a_terrain.getBound().left-a_item.getBound().radius) {//x,x,left
					if(a_item.getProspectivePos().z > a_terrain.getPos().z+a_terrain.getBound().back-a_item.getBound().radius) {//z,z,back
						if(a_item.getProspectivePos().z < a_terrain.getPos().z+a_terrain.getBound().front+a_item.getBound().radius) {//z,z,front
							//a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_item.getProspectivePos().z));
							//a_item.setVelocityY(0.0f);
							//a_item.toggleGrounded(true);
							return true;
							//////////////////////////
							// Player is inside     //
							//////////////////////////
						}
					}
				}
			} 




			/////////////////
			// CORNER      //
			/////////////////
			closestX = closestPoint(a_item.getProspectivePos().x,a_terrain.getPos().x+a_terrain.getBound().left,a_terrain.getPos().x+a_terrain.getBound().right);
			closestZ = closestPoint(a_item.getProspectivePos().z,a_terrain.getPos().z+a_terrain.getBound().back,a_terrain.getPos().z+a_terrain.getBound().front);
			// Finds the closest point
			distX = a_item.getProspectivePos().x-closestX;
			distZ = a_item.getProspectivePos().z-closestZ;
			// Finds distances in straight lines between the points

			tempDist = (distX*distX)+(distZ*distZ);


			if(tempDist < (a_item.getBound().radius*a_item.getBound().radius)) {
				//a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_item.getProspectivePos().z));
				//a_item.toggleGrounded(true);
				//a_item.setVelocityY(0);
				return true;
			}
			else {
				return false;
			}





		}
	}


	//a_item.toggleGrounded(false);
	return false;
}

bool PhysicsSystem::SenseCollision(Player &a_player, Terrain &a_terrain) {
	float distX = 0.0f;
	float distZ = 0.0f;
	float closestX = 0.0f;
	float closestZ = 0.0f;
	float tempDist = 0.0f;
	if(a_player.getProspectivePos().y < a_terrain.getPos().y+a_terrain.getBound().top) { // y,y,top
		if(a_player.getProspectivePos().y > a_terrain.getPos().y+a_terrain.getBound().bottom-a_player.getBound().height) { //y,y,bottom
			////////////////////////
			// POSITION INSIDE    //
			////////////////////////
			if(a_player.getProspectivePos().x < a_terrain.getPos().x+a_terrain.getBound().right+a_player.getBound().radius) {//x,x,right
				if(a_player.getProspectivePos().x > a_terrain.getPos().x+a_terrain.getBound().left-a_player.getBound().radius) {//x,x,left
					if(a_player.getProspectivePos().z > a_terrain.getPos().z+a_terrain.getBound().back-a_player.getBound().radius) {//z,z,back
						if(a_player.getProspectivePos().z < a_terrain.getPos().z+a_terrain.getBound().front+a_player.getBound().radius) {//z,z,front
							//a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_player.getProspectivePos().z));
							//a_player.setVelocityY(0.0f);
							//a_player.toggleGrounded(true);
							return true;
							//////////////////////////
							// Player is inside     //
							//////////////////////////
						}
					}
				}
			} 




			/////////////////
			// CORNER      //
			/////////////////
			closestX = closestPoint(a_player.getProspectivePos().x,a_terrain.getPos().x+a_terrain.getBound().left,a_terrain.getPos().x+a_terrain.getBound().right);
			closestZ = closestPoint(a_player.getProspectivePos().z,a_terrain.getPos().z+a_terrain.getBound().back,a_terrain.getPos().z+a_terrain.getBound().front);
			// Finds the closest point
			distX = a_player.getProspectivePos().x-closestX;
			distZ = a_player.getProspectivePos().z-closestZ;
			// Finds distances in straight lines between the points

			tempDist = (distX*distX)+(distZ*distZ);


			if(tempDist < (a_player.getBound().radius*a_player.getBound().radius)) {
				//a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_player.getProspectivePos().z));
				//a_player.toggleGrounded(true);
				//a_player.setVelocityY(0);
				return true;
			}
			else {
				return false;
			}





		}
	}


	//a_player.toggleGrounded(false);
	return false;
}


bool PhysicsSystem::SenseCollision(Player& a_player, Bullet &a_bullet) {
	if(a_player.getProspectivePos().y < a_bullet.getPos().y - (.5f*a_player.getBound().height)) { // y,y,top
		if(a_player.getProspectivePos().y > a_bullet.getPos().y-(1.5f*a_player.getBound().height)) { //y,y,bottom
			float distX = 0;
			float distZ = 0;
			float tempDist = 0;
			distX = a_bullet.getProspectivePos().x-a_player.getProspectivePos().x;
			distZ = a_bullet.getProspectivePos().z-a_player.getProspectivePos().z;
			// Finds distances in straight lines between the points
			tempDist = (distX*distX)+(distZ*distZ);
			if(tempDist < (a_player.getBound().radius*a_player.getBound().radius)) {
				//a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_player.getProspectivePos().z));
				//a_player.toggleGrounded(true);
				//a_player.setVelocityY(0);
				a_bullet.HitPlayer(a_player);
				return true;
			}
		}
	}
	return false;
}


bool PhysicsSystem::SenseCollision(Terrain &a_terrain, Bullet &a_bullet) {
	if(a_bullet.getProspectivePos().y < a_terrain.getPos().y+a_terrain.getBound().top) { // y,y,top
		if(a_bullet.getProspectivePos().y > a_terrain.getPos().y+a_terrain.getBound().bottom) { //y,y,bottom
			if(a_bullet.getProspectivePos().x < a_terrain.getPos().x+a_terrain.getBound().right) {//x,x,right
				if(a_bullet.getProspectivePos().x > a_terrain.getPos().x+a_terrain.getBound().left) {//x,x,left
					if(a_bullet.getProspectivePos().z > a_terrain.getPos().z+a_terrain.getBound().back) {//z,z,back
						if(a_bullet.getProspectivePos().z < a_terrain.getPos().z+a_terrain.getBound().front) {//z,z,front
							//a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getStruct()->top,a_player.getProspectivePos().z));
							//a_player.setVelocityY(0.0f);
							//a_player.toggleGrounded(true);
							return true;
							//////////////////////////
							// Player is inside     //
							//////////////////////////
						}
					}
				}
			} 
		}
	}


	return false;
}


bool PhysicsSystem::SenseCollision(Player& a_player, ItemBox &a_item) {
	if(a_player.getProspectivePos().y < a_item.getPos().y+a_item.getBound().height) { // y,y,top
		if(a_player.getProspectivePos().y > a_item.getPos().y-a_player.getBound().height) { //y,y,bottom
			float distX = 0;
			float distZ = 0;
			float tempDist = 0;
			distX = a_item.getProspectivePos().x-a_player.getProspectivePos().x;
			distZ = a_item.getProspectivePos().z-a_player.getProspectivePos().z;
			// Finds distances in straight lines between the points
			tempDist = (distX*distX)+(distZ*distZ);
			if(tempDist <= a_item.getBound().radius + a_player.getBound().radius) {
				return true;
			}
		}
	}
	return false;
}



bool PhysicsSystem::SenseCollision(Player& a_player, Player& a_player2) {
	if(a_player.getProspectivePos().y < a_player2.getPos().y+a_player2.getBound().height) { // y,y,top
		if(a_player.getProspectivePos().y > a_player2.getPos().y-a_player.getBound().height) { //y,y,bottom
			float distX = 0;
			float distZ = 0;
			float tempDist = 0;
			distX = a_player2.getProspectivePos().x-a_player.getProspectivePos().x;
			distZ = a_player2.getProspectivePos().z-a_player.getProspectivePos().z;
			// Finds distances in straight lines between the points
			tempDist = (distX*distX)+(distZ*distZ);
			if(tempDist <= a_player2.getBound().radius + a_player.getBound().radius) {
				return true;
			}
		}
	}
	return false;
}


bool  PhysicsSystem::ResolveCollision(Player& a_player, Player& a_player2){

	a_player.setProspectivePos(a_player.getPos());
	a_player.setJumpCount(0);
	a_player.toggleGrounded(true);
	a_player2.setJumpCount(0);
	a_player2.toggleGrounded(true);
	return true;
}

bool PhysicsSystem::ResolveCollision(Player& a_player, Terrain &a_terrain) {
	float closestX = 0, closestZ = 0, distX = 0, distZ = 0;

	// player is falling into the top
	if(a_player.getPos().y >= a_terrain.getPos().y+a_terrain.getBound().top) { // PLAYER WAS ABOVE
		
		a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getBound().top,a_player.getProspectivePos().z));
		a_player.toggleGrounded(true);
		a_player.setVelocityY(0);
		return true;
	}


	// player is jumping into the bottom
	if(a_player.getPos().y <= a_terrain.getPos().y+a_terrain.getBound().bottom-a_player.getBound().height) { // PLAYER WAS BELOW
		a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getBound().bottom-a_player.getBound().height,a_player.getProspectivePos().z));
		a_player.setVelocityY(0);
		return true;
	}


	// player is running into the side
	if(a_player.getPos().y <= a_terrain.getPos().y+a_terrain.getBound().top) { // y,y,top
		if(a_player.getPos().y >= a_terrain.getPos().y+a_terrain.getBound().bottom-a_player.getBound().height) { //y,y,bottom  YOU WERE ALREADY ON WITHIN Y, AND THEREFORE HAD TO BE OUTSIDE
		//if(a_player.getProspectivePos().x-(a_terrain.getPos().x+a_terrain.getStruct()->right)
		closestX = closestPoint(a_player.getProspectivePos().x,a_terrain.getPos().x+a_terrain.getBound().left,a_terrain.getPos().x+a_terrain.getBound().right);
		closestZ = closestPoint(a_player.getProspectivePos().z,a_terrain.getPos().z+a_terrain.getBound().back,a_terrain.getPos().z+a_terrain.getBound().front);
		//distX = a_player.getProspectivePos().x-closestX;
		//distZ = a_player.getProspectivePos().z-closestZ;


		if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // RIGHT and FRONT are closest
			distX = closestX-a_player.getProspectivePos().x; // closest minus pos for right
			distZ = closestZ-a_player.getProspectivePos().z; // closest minus pos for front
			if(distX < 0 && distX < distZ) {
				a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_player.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // RIGHT and BACK are closest
			distX = closestX-a_player.getProspectivePos().x; // closest minus pos for right
			distZ = a_player.getProspectivePos().z-closestZ; // pos minus closest for back
			if(distX < 0 && distX < distZ) {
				a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_player.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // LEFT and BACK are closest
			distX = a_player.getProspectivePos().x-closestX; // pos minus closest for left
			distZ = a_player.getProspectivePos().z-closestZ; // pos minus closest for back
			if(distX < 0 && distX < distZ) {
				a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_player.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // LEFT and FRONT are closest
			distX = a_player.getProspectivePos().x-closestX; // pos minus closest for left
			distZ = closestZ-a_player.getProspectivePos().z; // clsest minus pos for front
			if(distX < 0 && distX < distZ) {
				a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_player.getBound().radius));
			}
			}
		}
		return true;

	}





	


	

	






	//if(a_terrain.getType() == FLOOR) {
	//	a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getBound().top,a_player.getProspectivePos().z));
	//	a_player.toggleGrounded(true);
	//	a_player.setVelocityY(0);
	//}
	//else if(a_terrain.getType() == WALL) {
	//	//if(a_player.getProspectivePos().x-(a_terrain.getPos().x+a_terrain.getStruct()->right)
	//	closestX = closestPoint(a_player.getProspectivePos().x,a_terrain.getPos().x+a_terrain.getBound().left,a_terrain.getPos().x+a_terrain.getBound().right);
	//	closestZ = closestPoint(a_player.getProspectivePos().z,a_terrain.getPos().z+a_terrain.getBound().back,a_terrain.getPos().z+a_terrain.getBound().front);
	//	//distX = a_player.getProspectivePos().x-closestX;
	//	//distZ = a_player.getProspectivePos().z-closestZ;


	//	if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // RIGHT and FRONT are closest
	//		distX = closestX-a_player.getProspectivePos().x; // closest minus pos for right
	//		distZ = closestZ-a_player.getProspectivePos().z; // closest minus pos for front
	//		if(distX < 0 && distX < distZ) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
	//		}
	//		if(distZ < 0 && distZ < distX) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_player.getBound().radius));
	//		}
	//	}
	//	else if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // RIGHT and BACK are closest
	//		distX = closestX-a_player.getProspectivePos().x; // closest minus pos for right
	//		distZ = a_player.getProspectivePos().z-closestZ; // pos minus closest for back
	//		if(distX < 0 && distX < distZ) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
	//		}
	//		if(distZ < 0 && distZ < distX) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_player.getBound().radius));
	//		}
	//	}
	//	else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // LEFT and BACK are closest
	//		distX = a_player.getProspectivePos().x-closestX; // pos minus closest for left
	//		distZ = a_player.getProspectivePos().z-closestZ; // pos minus closest for back
	//		if(distX < 0 && distX < distZ) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
	//		}
	//		if(distZ < 0 && distZ < distX) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_player.getBound().radius));
	//		}
	//	}
	//	else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // LEFT and FRONT are closest
	//		distX = a_player.getProspectivePos().x-closestX; // pos minus closest for left
	//		distZ = closestZ-a_player.getProspectivePos().z; // clsest minus pos for front
	//		if(distX < 0 && distX < distZ) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_player.getBound().radius, a_player.getProspectivePos().y, a_player.getProspectivePos().z));
	//		}
	//		if(distZ < 0 && distZ < distX) {
	//			a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_player.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_player.getBound().radius));
	//		}
	//	}
	//}
	//else if(a_terrain.getType() == CEILING) {

	//}
	//else if(a_terrain.getType() == FLOORWALL) {

	//}
	//else if(a_terrain.getType() == FLOORCEILING) {

	//}








	return true;
}


bool PhysicsSystem::ResolveCollision(ItemBox& a_item, Terrain &a_terrain) {
	float closestX = 0, closestZ = 0, distX = 0, distZ = 0;
	if(a_terrain.getType() == FLOOR) {
		a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getBound().top,a_item.getProspectivePos().z));
		a_item.toggleGrounded(true);
		a_item.setVelocityY(0);
	}
	else if(a_terrain.getType() == WALL) {
		//if(a_item.getProspectivePos().x-(a_terrain.getPos().x+a_terrain.getStruct()->right)
		closestX = closestPoint(a_item.getProspectivePos().x,a_terrain.getPos().x+a_terrain.getBound().left,a_terrain.getPos().x+a_terrain.getBound().right);
		closestZ = closestPoint(a_item.getProspectivePos().z,a_terrain.getPos().z+a_terrain.getBound().back,a_terrain.getPos().z+a_terrain.getBound().front);
		//distX = a_item.getProspectivePos().x-closestX;
		//distZ = a_item.getProspectivePos().z-closestZ;


		if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // RIGHT and FRONT are closest
			distX = closestX-a_item.getProspectivePos().x; // closest minus pos for right
			distZ = closestZ-a_item.getProspectivePos().z; // closest minus pos for front
			if(distX < 0 && distX < distZ) {
				a_item.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_item.getBound().radius, a_item.getProspectivePos().y, a_item.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_item.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_item.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().right && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // RIGHT and BACK are closest
			distX = closestX-a_item.getProspectivePos().x; // closest minus pos for right
			distZ = a_item.getProspectivePos().z-closestZ; // pos minus closest for back
			if(distX < 0 && distX < distZ) {
				a_item.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().right + a_item.getBound().radius, a_item.getProspectivePos().y, a_item.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_item.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_item.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().back) { // LEFT and BACK are closest
			distX = a_item.getProspectivePos().x-closestX; // pos minus closest for left
			distZ = a_item.getProspectivePos().z-closestZ; // pos minus closest for back
			if(distX < 0 && distX < distZ) {
				a_item.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_item.getBound().radius, a_item.getProspectivePos().y, a_item.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_item.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().back - a_item.getBound().radius));
			}
		}
		else if(closestX == a_terrain.getPos().x + a_terrain.getBound().left && closestZ == a_terrain.getPos().z + a_terrain.getBound().front) { // LEFT and FRONT are closest
			distX = a_item.getProspectivePos().x-closestX; // pos minus closest for left
			distZ = closestZ-a_item.getProspectivePos().z; // clsest minus pos for front
			if(distX < 0 && distX < distZ) {
				a_item.setProspectivePos(D3DXVECTOR3(a_terrain.getPos().x + a_terrain.getBound().left - a_item.getBound().radius, a_item.getProspectivePos().y, a_item.getProspectivePos().z));
			}
			if(distZ < 0 && distZ < distX) {
				a_item.setProspectivePos(D3DXVECTOR3(a_item.getProspectivePos().x,a_item.getProspectivePos().y,a_terrain.getPos().z+a_terrain.getBound().front + a_item.getBound().radius));
			}
		}
	}
	else if(a_terrain.getType() == CEILING) {

	}
	else if(a_terrain.getType() == FLOORWALL) {

	}
	else if(a_terrain.getType() == FLOORCEILING) {

	}








	return true;
}

bool PhysicsSystem::ResolveCollision(Player& a_player, Bullet &a_bullet) {
	a_bullet.HitPlayer(a_player);
	if(a_bullet.getBullType()==grab){
		a_player.setPos(a_bullet.getStartpos());
		a_player.setProspectivePos(a_bullet.getStartpos());
	}
	return true;
}


bool PhysicsSystem::ResolveCollision(Terrain &a_terrain, Bullet &a_bullet) {
	a_bullet.HitWall();

	return true;
}



















//float Distance2D(float a_x, float a_z, float a_x2, float a_z2) {
//
//}


//bool PlayerWall(D3DXVECTOR3 &a_pos, PrimStruct &a_cube, float a_radius, float a_height) {
//	float tempX = 0, tempZ = 0, tempDist = 0;
//
//	if(a_pos.y <= a_cube.top) {
//		if(a_pos.y+a_height >= a_cube.bottom) {
//			for(int i = 0; i < 4; ++i) {
//				if(i == 0) {
//					tempX = abs(a_pos.x-a_cube.left); // left,front
//					tempZ = abs(a_pos.z-a_cube.front);
//				}
//				else if(i == 1) {
//					tempX = abs(a_pos.x-a_cube.left); // left,back
//					tempZ = abs(a_pos.z-a_cube.back);
//				}
//				else if(i == 2) {
//					tempX = abs(a_pos.x-a_cube.right); // right,back
//					tempZ = abs(a_pos.z-a_cube.back);
//				}
//				else if(i == 3) {
//					tempX = abs(a_pos.x-a_cube.right); // right,front
//					tempZ = abs(a_pos.z-a_cube.front);
//				}
//				tempDist = tempX*tempX + tempZ*tempZ;
//				if(tempDist < a_radius*a_radius)
//					return true;
//
//			}
//		}
//	}
//	return false;
//}
//
//
//

