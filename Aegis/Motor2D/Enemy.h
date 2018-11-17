#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "Entity.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2List.h"

enum Enemy_direction {

	EN_RIGHT,
	EN_LEFT
};

class j1Enemy : public j1Entity
{

public:

	j1Enemy(iPoint pos);
	~j1Enemy() {}

	void j1Enemy::OnCollision(Collider *c1, Collider *c2) override;

public:

	bool falling = false;;

	Enemy_direction Direction = EN_RIGHT;

	iPoint enemy_position;
	fPoint enemy_velocity;
	iPoint initial_velocity;

	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;

	SDL_Texture* texture;

	//Path texture
	SDL_Texture *path_tex;

public:

	ENTITY_TYPE type;

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;
};

#endif


