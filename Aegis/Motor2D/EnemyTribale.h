
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
enum State {
	NONE,
	LEFT_UP,
	RIGHT_UP,
	RIGHT_DOWN,
	LEFT_DOWN,
	UP,
	DOWN,
	LEFT,
	RIGHT
	
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
	/*p2DynArray<State> wheretogo(const p2DynArray<iPoint>& path);*/
public:

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	SDL_Rect pathrect;
	SDL_Rect TribaleRect;
	MasterTimer pathfinding_recalc;
	Collider* TribaleCollider;
private:

	Animation idle;
	Animation move_left;
	Animation move_right;
	Animation attack;

    TribaleStates state = IDL;



	//PATHFINDING
	const p2DynArray<iPoint>* path = nullptr;
	
	p2DynArray<State> states;
	unsigned int i;
	bool move = false;

	iPoint PosToGo;
	
	
	
};

#endif
