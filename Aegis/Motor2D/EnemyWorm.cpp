#include "EnemyWorm.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"
#include "EntityManager.h"

EnemyWorm::EnemyWorm(iPoint pos): j1Enemy(pos, ENEMY_TYPE::WORM) {
	
	properties_node = App->j1entity_manager->properties_doc.child("properties").child("worm");

	idle.LoadPushbacks(properties_node.child("idle"));
	move.LoadPushbacks(properties_node.child("move"));
}

EnemyWorm::~EnemyWorm() {}

bool EnemyWorm::Start() {

	texture= App->j1entity_manager->worm_texture;
	current_animation = &idle;
	return true;
}

bool EnemyWorm::Update(float dt) {
	
	Draw();
	
	return true;
}
