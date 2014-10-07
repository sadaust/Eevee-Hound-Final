#pragma once
#include "PhysicsSystem.h"






Bullet::Bullet() {

}


Bullet::~Bullet() {
	
}


void Bullet::Init(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimStruct * a_structpoi, float a_rot, float a_angle, float a_lifespan, int a_damage) {
	pos = a_pos;
	rot = a_rot;
	angle = a_angle;
	rotate3Dvector(&a_velocity, a_rot, a_angle);
	velocity = a_velocity;
	structpoi = a_structpoi;
	prospectivePos = pos;
	speed = 5.0f;
	lifespan = a_lifespan;
	damage = a_damage;
}


void Bullet::Update(float a_dt) {
	pos.x = prospectivePos.x;
	pos.y = prospectivePos.y;
	pos.z = prospectivePos.z;
	
	prospectivePos.x += velocity.x*a_dt;
	prospectivePos.y += velocity.y*a_dt;
	prospectivePos.z += velocity.z*a_dt;
	lifespan -= a_dt;


}


D3DXVECTOR3 Bullet::getPos() {
	return pos;
}


D3DXVECTOR3 Bullet::getProspectivePos() {
	return prospectivePos;
}


int Bullet::getDamage() {
	return damage;
}


float Bullet::getLifeSpan() {
	return lifespan;
}


float Bullet::getRot() {
	return rot;
}


float Bullet::getAngle() {
	return angle;
}


//bool Bullet::getActive() {
//	return active;
//}
//
//
//void Bullet::setActive(bool a_active) {
//	active = a_active;
//}


void Bullet::HitWall() {
	
}


void Bullet::HitPlayer(Player& a_player) {
	  //////////////////////////////////////////////
	 // Add in damage and stuff here probably    //
	//////////////////////////////////////////////
	a_player.takeDamage(damage);
	
}


BulletVec::BulletVec() {

}


BulletVec::~BulletVec() {

}


void BulletVec::Init() {
	for(int i = 0; i < MAXBULLETS; ++i) {
		bullets[i].Init(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), NULL,0,0,0,0);
		bools[i] = false;
	}
}


void BulletVec::Update(float a_dt) {
	for(int i = 0; i < MAXBULLETS; ++i) {
		if(bools[i]) {
			bullets[i].Update(a_dt);
			if(bullets[i].getLifeSpan() <= 0) {
				DeactivateABullet(i);
			}
		}
	}
}


Bullet& BulletVec::GetBullet(int a_index) {
	return bullets[a_index];
}


bool BulletVec::GetActive(int a_index) {
	return bools[a_index];
}


bool BulletVec::ActivateABullet(D3DXVECTOR3 a_pos, D3DXVECTOR3 a_velocity, PrimStruct * a_structpoi, float a_rot, float a_angle, float a_lifespan, int a_damage) {
	for(int i = 0; i < MAXBULLETS; ++i) {
		if(!bools[i]) {
			bullets[i].Init(a_pos, a_velocity, a_structpoi, a_rot, a_angle, a_lifespan, a_damage);
			bools[i] = true;
			return true;
		}
	}
	return false;
}


bool BulletVec::DeactivateABullet(int a_index) {
	bools[a_index] = false;
	return true;
}


Terrain::Terrain() {
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	boundingBox.back = 0;
	boundingBox.front = 0;
	boundingBox.left = 0;
	boundingBox.right = 0;
	boundingBox.back = 0;
	boundingBox.back = 0;
}


Terrain::~Terrain() {

}


void Terrain::Init(D3DXVECTOR3 a_pos, PrimStruct * a_structpoi, TerrainType a_type) {
	pos = a_pos;
	boundingBox.back = a_structpoi->back;
	boundingBox.bottom = a_structpoi->bottom;
	boundingBox.front = a_structpoi->front;
	boundingBox.left = a_structpoi->left;
	boundingBox.right = a_structpoi->right;
	boundingBox.top = a_structpoi->top;
	type = a_type;
}


D3DXVECTOR3 Terrain::getPos() {
	return pos;
}


cube Terrain::getBound() {
	return boundingBox;
}


TerrainType Terrain::getType() {
	return type;
}


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
	if(a_player.getProspectivePos().y < a_bullet.getPos().y) { // y,y,top
		if(a_player.getProspectivePos().y > a_bullet.getPos().y-a_player.getBound().height) { //y,y,bottom
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


bool PhysicsSystem::ResolveCollision(Player& a_player, Terrain &a_terrain) {
	float closestX = 0, closestZ = 0, distX = 0, distZ = 0;
	if(a_terrain.getType() == FLOOR) {
		a_player.setProspectivePos(D3DXVECTOR3(a_player.getProspectivePos().x,a_terrain.getPos().y+a_terrain.getBound().top,a_player.getProspectivePos().z));
		a_player.toggleGrounded(true);
		a_player.setVelocityY(0);
	}
	else if(a_terrain.getType() == WALL) {
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

