
//#include "EnemyWorm.h"
//#include "p2Point.h"
//#include "p2Log.h"
//#include <math.h>
//#include "p2Defs.h"
//
//
//EnemyWorm::EnemyWorm(iPoint pos, pugi::xml_document &EnemiesDocument) : Enemy(pos, ENEMY_TYPES::WORM)
//{
//	position.x = pos.x;
//	position.y = pos.y;	
//	
//	LOG("CREATING WORM IN POS %i %i", pos.x, pos.y);
//	//doc = EnemiesDocument;
//	//collider = App->collision->AddCollider({ 0, 0, 48, 45 }, COLLIDER_TYPE::COLLIDER_ENEMY);
//}
//
//void EnemyWorm::Move() {
//
//
//}
//
//void EnemyWorm::OnCollision(Collider *collider) {
//	
//}
#include "EnemyWorm.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"



j1EnemyFlyer::j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument) : j1Enemy(enemy_position, ENEMY_TYPE::FLYER) {
	int i = 1;
	

	
}

j1EnemyFlyer::~j1EnemyFlyer() {}

void j1EnemyFlyer::Update(float dt) {

	
}