
#ifndef ENEMYTRIBALE_H_
#define ENEMYTRIBALE_H_

#include "Enemy.h"
#include "Animation.h"
enum TribaleStates {
	IDL,
	RUN_LEFT,
	RUN_RIGHT,
	ATTACK_RIGHT,
	ATTACK_LEFT,
	DIE,
	ERROR404
};

class EnemyTribale : public j1Enemy
{

public:

	EnemyTribale(iPoint pos);
	~EnemyTribale();

	bool Start() override;

	bool Update(float dt) override;
	void OnCollision(Collider *c1, Collider *c2);

	bool Gravity = true;

public:

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	SDL_Rect pathrect;
	SDL_Rect TribaleRect;
	MasterTimer pathfinding_recalc;
	Collider* TribaleCollider;
private:

	Animation idle;
	Animation move;
	Animation attack;

    TribaleStates state = IDL;

};

#endif
