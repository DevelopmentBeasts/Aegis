#ifndef _J1_ENTITY_
#define _J1_ENTITY_

#include "p2SString.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"
#include "Animation.h"
#include "j1Collision.h"


class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE;

class j1Entity {


public:

	j1Entity(iPoint pos, ENTITY_TYPE Type);

	~j1Entity() {}

	virtual bool Start() { return true; }

	virtual bool Update(float dt) { return true; }

	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) { return true; }

	virtual void OnCollision(Collider* c1, Collider* c2) {}

public:

	iPoint		position;
	ENTITY_TYPE type;
	Collider*	collider;

public:

	SDL_Texture *texture;
	Animation   *current_animation;

};

#endif