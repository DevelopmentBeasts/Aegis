#include "EnemyWorm.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"
#include "EntityManager.h"



EnemyWorm::EnemyWorm(iPoint pos): j1Enemy(pos, ENEMY_TYPE::WORM) {
	
	current_animation = &idle;
	idle.PushBack({0,0,500,500});
}

EnemyWorm::~EnemyWorm() {}

bool EnemyWorm::Start() {

	texture= App->j1entity_manager->worm_texture;
	
	return true;
}

bool EnemyWorm::Update(float dt) {
	

	//texture = App->j1entity_manager->worm_texture;
	Draw();
	return true;
}
