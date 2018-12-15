
#ifndef ENEMYTRIBALE_H_
#define ENEMYTRIBALE_H_

#include "Enemy.h"
#include "Animation.h"
#include "j1Pathfinding.h"
enum TribaleStates {
	IDL,
	RUN_LEFT,
	RUN_RIGHT,
	ATTACK_RIGHT,
	ATTACK_LEFT,
	DIE,
	ERROR404
};
//enum State {
//	NONE,
//	LEFT_UP,
//	RIGHT_UP,
//	RIGHT_DOWN,
//	LEFT_DOWN,
//	UP,
//	DOWN,
//	LEFT,
//	RIGHT
//	
//};

class EnemyTribale : public j1Enemy
{

public:

	EnemyTribale(iPoint pos);
	~EnemyTribale();

	bool Start() override;

	bool Update(float dt) override;
	void OnCollision(Collider *c1, Collider *c2);

	bool Gravity = true;
	void Move(const p2DynArray<iPoint>&path, float dt);
	bool DetectThePlayer();
	EntityDirection NewMovement(const p2DynArray<iPoint>*EntityPath);
public:

	p2DynArray<iPoint>* enemy_path;
	p2DynArray<iPoint>* last_enemy_path;

	SDL_Rect pathrect;
	SDL_Rect TribaleRect;
	SDL_Rect RightTribaleColliderSensorRect;
	SDL_Rect LeftTribaleColliderSensorRect;

	MasterTimer pathfinding_recalc;
	Collider* TribaleCollider;
	Collider* RightTribaleColliderSensor;
	Collider* LeftTribaleColliderSensor;
private:

	Animation idle;
	Animation move_left;
	Animation move_right;
	Animation attackleft;
	Animation attackright;
	Animation jump;
	bool fromleft = false;
	bool fromright = false;
	bool firstiteration = true;
	bool playerinrange = false;
    TribaleStates state = IDL;



	//PATHFINDING
	 p2DynArray<iPoint>* Path = nullptr;

	//p2DynArray<State> states;

	bool move = false;

	int i = 0;
	bool change_iterator = false;

	EntityDirection curr_direction;
};

#endif
