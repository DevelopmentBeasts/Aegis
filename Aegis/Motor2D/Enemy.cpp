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


j1Enemy::j1Enemy(iPoint pos) : j1Entity(pos.x, pos.y, ENTITY_TYPE::ENEMY){}

void j1Enemy::OnCollision(Collider *c1, Collider *c2) {
	 


}
