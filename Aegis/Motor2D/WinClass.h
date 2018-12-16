#ifndef WINCLASS_H_
#define WINCLASS_H_
#include "EntityManager.h"
#include "Entity.h"
struct Collider;

enum class PortalState{
	IDL,
	EXPLOSION,
	NONE
};

class WinClass : public j1Entity {
public:
	WinClass(iPoint pos);
	~WinClass();
	
public:
	bool Start();
	bool Update(float dt);
	void Draw();
public:
	Collider* WinCollider;
	void OnCollision(Collider *c1, Collider *c2);
public:
	Animation DiamondAnim;
	Animation ExplosionAnim;
	SDL_Texture* Tex = nullptr;
	pugi::xml_document AnimsDoc;
	pugi::xml_node AnimsNode;

	bool Explosion;
	
public:
	PortalState Portal_Curr_State = PortalState::IDL;
};

#endif
