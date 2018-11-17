//
//#include "j1App.h"
//#include "Enemy.h"
//#include "j1Collision.h"
//#include "j1Render.h"
//#include "EnemyWorm.h"
//
//Enemy::Enemy(iPoint pos, ENEMY_TYPES type_) : Entity(EntityType::ENEMY), position(pos)
//{
//}
//Enemy::~Enemy(){}
//
//Enemy* Enemy::CreateEnemy(iPoint pos, ENEMY_TYPES enemyType, const char* path, pugi::xml_document &EnemiesDocument) {
//
//	//static_assert(ENEMY_TYPES::ENEMYERROR404 == ENEMY_TYPES(2), "UPDATE ENEMY TYPES");
//	Enemy* Enemy = nullptr;
//	switch (enemyType) {
//
//	case ENEMY_TYPES::WORM:
//		Enemy = new EnemyWorm(pos, EnemiesDocument);
//		break;
//	default:
//		break;
//	}
//
//	App->entity_manager->entities_list.add(Enemy);
//	//debug_tex = App->tex->Load("maps/path2.png");
//
//	return Enemy;
//}
//
//bool Enemy::LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument) {
//
//
//	return true;
//}
//
//void Enemy::LoadPushbacks(pugi::xml_node node, Animation &animation) {
//
//	animation.loop = node.attribute("loop").as_bool();
//	animation.speed = node.attribute("speed").as_float();
//	SDL_Rect rect;
//
//	for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {
//
//		rect.x = node.attribute("x").as_int();
//		rect.y = node.attribute("y").as_int();
//		rect.w = node.attribute("w").as_int();
//		rect.h = node.attribute("h").as_int();
//		animation.PushBack({ rect });
//	}
//}
//
//void Enemy::OnCollision(Collider *c1, Collider *c2) {
//
//	p2List_item<Entity*>* item = App->entity_manager->entities_list.start;
//
//	for (; item != nullptr; item = item->next) {
//	
//	}
//}
//
//void Enemy::DestroyEnemy(Enemy *Enemy) {
//
//	p2List_item<Entity*>*item = App->entity_manager->entities_list.start;
//
//	while (item != nullptr) {
//
//		if (item->data == Enemy) {
//
//			App->entity_manager->entities_list.del(item);
//			RELEASE(item->data);
//			break;
//		}
//
//		item = item->next;
//	}
//}
#include "Enemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "EnemyWorm.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "EntityManager.h"


j1Enemy::j1Enemy(iPoint pos, ENEMY_TYPE type_) : j1Entity(ENTITY_TYPE::ENEMY_ENT), enemy_position(pos)
{
}

j1Enemy* j1Enemy::CreateEnemy(iPoint pos, ENEMY_TYPE enemyType, const char* path, pugi::xml_document &EnemiesDocument) {

	p2SString tmp("maps\\%s", path);

	static_assert(ENEMY_TYPE::UNKNOWN == ENEMY_TYPE(2), "UPDATE ENEMY TYPES");
	j1Enemy* Enemy = nullptr;
	switch (enemyType) {

	case ENEMY_TYPE::FLYER:
		Enemy = new j1EnemyFlyer(pos, tmp.GetString(), EnemiesDocument);
		break;
	/*case ENEMY_TYPE::WALKER:
		Enemy = new j1EnemyWalker(pos, tmp.GetString(), EnemiesDocument);
		break;*/
	default:
		break;
	}
	
	App->j1entity_manager->entities_list.add(Enemy);
	//debug_tex = App->tex->Load("maps/path2.png");

	return Enemy;
}

bool j1Enemy::LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument) {


	return true;
}



void j1Enemy::OnCollision(Collider *c1, Collider *c2) {

	
}

void j1Enemy::DestroyEnemy(j1Enemy *Enemy) {

	
}