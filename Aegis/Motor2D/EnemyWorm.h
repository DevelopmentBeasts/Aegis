#ifndef __ENEMY_BEE_H__
#define __ENEMY_BEE_H__

#include "Enemy.h"
#include "Animation.h"
#include "j1Collision.h"

class EnemyWorm : public Enemy
{
public:
	EnemyWorm(iPoint pos,const char* path, pugi::xml_document &EnemiesDocument);
	~EnemyWorm();
    void OnCollision(Collider *collider);
	fPoint shoot;	
	Animation example;
	SDL_Texture* Normal;

	
	void Move();
};

#endif // __ENEMY_BEE_H__
