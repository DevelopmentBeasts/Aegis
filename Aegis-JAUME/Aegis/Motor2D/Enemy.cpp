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
#include "EnemyTribale.h"

j1Enemy::j1Enemy(iPoint pos, ENEMY_TYPE enemy_type) : enemy_type(enemy_type), j1Entity(pos, ENTITY_TYPE::ENEMY){
	
}

void j1Enemy::OnCollision(Collider *c1, Collider *c2) {
	
	int i;
}

void j1Enemy::Draw(int extrax,int extray) {

	App->render->Blit(texture,position.x+extrax, position.y+extray, &current_animation->GetCurrentFrame(), 1/*Parallax*/, 0.0/*rotation*/, flip,NULL,NULL,1.5);
}