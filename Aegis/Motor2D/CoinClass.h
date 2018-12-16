#ifndef COINCLASS_H_
#define COINCLASS_H_
#include "EntityManager.h"
#include "Entity.h"
struct Collider;



class CoinClass : public j1Entity {
public:
	CoinClass(iPoint pos);
	~CoinClass();

public:
	bool Start();
	bool Update(float dt);
	void Draw();
	bool CleanUp();
public:
	
	void OnCollision(Collider *c1, Collider *c2);
public:
	Animation CoinAnim;
	
	SDL_Texture* Tex = nullptr;

	pugi::xml_document AnimsDoc;
	pugi::xml_node AnimsNode;

	bool Explosion;

public:
	
};

#endif

