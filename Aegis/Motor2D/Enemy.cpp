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
#include "Player.h"
#include "j1Collision.h"


j1Enemy::j1Enemy(iPoint pos, ENEMY_TYPE enemy_type) : enemy_type(enemy_type), j1Entity(pos, ENTITY_TYPE::ENEMY){
	
}

void j1Enemy::OnCollision(Collider *c1, Collider *c2) {
	
	int i;
}

void j1Enemy::Draw() {
	App->render->Blit(texture, App->player->position.x, App->player->position.y, &current_animation->GetCurrentFrame(), 1.0/*Parallax*/, 0.0/*rotation*/, flip);
}