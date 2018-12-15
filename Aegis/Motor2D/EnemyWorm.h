
#ifndef _J1_ENEMY_FLYER_
#define _J1_ENEMY_FLYER_

#include "Enemy.h"
#include "Animation.h"


class EnemyWorm: public j1Enemy
{

public:

	EnemyWorm(iPoint pos);
	~EnemyWorm();

	bool Start() override;

	bool Update(float dt) override;
	void OnCollision(Collider *c1, Collider *c2);
	void Move(const p2DynArray<iPoint>&path, float dt);
	EntityDirection NewMovement(const p2DynArray<iPoint>*EntityPath);
	bool DetectThePlayer();
public:

	SDL_Rect WormRect;

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	MasterTimer pathfinding_recalc;

private:

	SDL_Texture* debug_texture;

	Animation idle;
	Animation moving;



	//PATHFINDING
	p2DynArray<iPoint>* path = nullptr;

	//p2DynArray<State> states;

	bool move = false;

	int i = 0;
	bool change_iterator = false;

	EntityDirection curr_direction;
};

#endif
