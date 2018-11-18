
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

	void DrawPath();
public:

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	SDL_Rect pathrect;
	MasterTimer pathfinding_recalc;

private:

	const p2DynArray<iPoint>* path;

	SDL_Texture* debug_texture;

	Animation idle;
	Animation move;

};

#endif
