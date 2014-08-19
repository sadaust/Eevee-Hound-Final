#pragma once
#include "PhysicsSystem.h"






bool PlayerGround(D3DXVECTOR3 &a_pos, PrimStruct &a_cube) {
	if(a_pos.y <= a_cube.top) {
		if(a_pos.y >= a_cube.bottom) {
			if(a_pos.x <= a_cube.right) {
				if(a_pos.x >= a_cube.left) {
					if(a_pos.z >= a_cube.back) {
						if(a_pos.z <= a_cube.front) {
							a_pos.y = a_cube.top;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}


//float Distance2D(float a_x, float a_z, float a_x2, float a_z2) {
//
//}


bool PlayerWall(D3DXVECTOR3 &a_pos, PrimStruct &a_cube, float a_radius, float a_height) {
	float tempX = 0, tempZ = 0, tempDist = 0;

	if(a_pos.y <= a_cube.top) {
		if(a_pos.y+a_height >= a_cube.bottom) {
			for(int i = 0; i < 4; ++i) {
				if(i == 0) {
					tempX = abs(a_pos.x-a_cube.left); // left,front
					tempZ = abs(a_pos.z-a_cube.front);
				}
				else if(i == 1) {
					tempX = abs(a_pos.x-a_cube.left); // left,back
					tempZ = abs(a_pos.z-a_cube.back);
				}
				else if(i == 2) {
					tempX = abs(a_pos.x-a_cube.right); // right,back
					tempZ = abs(a_pos.z-a_cube.back);
				}
				else if(i == 3) {
					tempX = abs(a_pos.x-a_cube.right); // right,front
					tempZ = abs(a_pos.z-a_cube.front);
				}
				tempDist = tempX*tempX + tempZ*tempZ;
				if(tempDist < a_radius*a_radius)
					return true;

			}
		}
	}
	return false;
}




