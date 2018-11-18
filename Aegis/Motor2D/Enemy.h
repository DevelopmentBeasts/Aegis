#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "Entity.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2List.h"


enum class ENEMY_TYPE {

	WORM,
	DUDE,
	UNKNOWN = 2
};

class j1Enemy : public j1Entity
{

public:

	j1Enemy(iPoint pos, ENEMY_TYPE enemy_type);
	~j1Enemy() {}

	virtual bool Start() { return true; }

	virtual bool Update(float dt) override { return true; }

	void j1Enemy::OnCollision(Collider *c1, Collider *c2) override;

	void Draw();

public:

	bool falling = false;

	fPoint velocity;
	iPoint initial_velocity;

	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;

public:

	SDL_RendererFlip	flip=SDL_FLIP_NONE;
	ENEMY_TYPE			enemy_type;
	
};

#endif


