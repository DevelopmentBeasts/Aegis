#ifndef FIREBALL_H_
#define FIREBALL_H_
#include "EntityManager.h"
#include "Entity.h"
#include "j1Collision.h"

struct Collider;

class FireBallClass : public j1Entity {
public:
	FireBallClass(iPoint pos);
	~FireBallClass();

public:
	bool Start();
	bool Update(float dt);
	void Draw();
public:
	
	void OnCollision(Collider *c1, Collider *c2);
public:

	Collider* FireBallCollider = nullptr;
	Animation movementanim;
	SDL_Texture* Tex = nullptr;
	pugi::xml_document AnimsDoc;
	pugi::xml_node AnimsNode;

	

public:
	
};

#endif
