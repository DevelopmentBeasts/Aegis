#ifndef _J1_ENTITY_
#define _J1_ENTITY_

#include "p2SString.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"
#include "Animation.h"
#include "j1Collision.h"

enum EntityDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_RIGHT,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT,
	NO_DIRECTION
};
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
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp();

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) { return true; }

	//virtual void OnCollision(Collider* c1, Collider* c2) {}
	virtual void OnCollision(Collider*, Collider*) {}

public:

	// x & y position
	iPoint		position;
	
	//Type of entity
	ENTITY_TYPE type;

	//Collider
	Collider*	collider = nullptr;
	fPoint velocity = { 0,-3.0 };

	//Node of the document with the properties
	pugi::xml_node		properties_node;

public:

	Animation idle;							///In all the animations the character is facing the left side 
	Animation move;
	Animation death;

	//Texture
	SDL_Texture* texture = nullptr;
	
	//Animation being shown
	Animation* current_animation=nullptr;
	Animation* win_explosion = nullptr;
	void Die();
	bool die = false;

};

#endif