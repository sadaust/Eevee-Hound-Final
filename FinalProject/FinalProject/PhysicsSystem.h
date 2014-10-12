#pragma once
#include "Player.h"
#include "ResDefs.h"
#include "ShapeDefs.h"
#include "ItemBox.h"
#include "Bullet.h"
#include "Terrain.h"
#include "Map.h"



class PhysicsSystem {
private:
	float closestPoint(float, float, float);
public:
	PhysicsSystem();
	~PhysicsSystem();
	void DoCollisions(PlayerVec& a_playerVec, BulletVec& a_bulletVec, Map& a_map);
	bool SenseCollision(Player& a_player, Terrain &a_terrain);
	bool SenseCollision(ItemBox& a_item, Terrain &a_terrain);
	bool SenseCollision(Player& a_player, Bullet &a_bullet);
	bool SenseCollision(Terrain &a_terrain, Bullet &a_bullet);
	bool SenseCollision(Player& a_player, ItemBox &a_item);
	bool SenseCollision(Player& a_player, Player& a_player2);
	bool ResolveCollision(Player& a_player, Terrain &a_terrain);
	bool ResolveCollision(Player& a_player, Bullet &a_bullet);
	bool ResolveCollision(Terrain &a_terrain, Bullet &a_bullet);
	bool ResolveCollision(Player& a_player, Player& a_player2);
	bool ResolveCollision(ItemBox& a_item, Terrain &a_terrain);
};