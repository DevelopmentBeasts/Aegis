
#ifndef _J1_ENEMY_FLYER_
#define _J1_ENEMY_FLYER_

#include "Enemy.h"
#include "Animation.h"

class j1EnemyFlyer : public j1Enemy
{

public:

	j1EnemyFlyer(iPoint pos, const char* path, pugi::xml_document &EnemiesDocument);
	~j1EnemyFlyer();

	bool Update(float dt) override;

public:

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	SDL_Rect pathrect;
	MasterTimer pathfinding_recalc;

private:

	Animation* current_animation = nullptr;
	Animation Idle;
	Animation Run;
	Animation Dead;

};

#endif
