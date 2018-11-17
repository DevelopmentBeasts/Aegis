#ifndef ENEMY_H_
#define ENEMY_H_

#include "p2Point.h"
#include "Animation.h"
#include "j1Textures.h"
#include "EntityManager.h"
#include "Entity.h"
struct SDL_Textures;
struct Collider;

enum ENEMY_TYPES {

	ENEMYERROR404,
	WORM,
	REPTIL

};
struct EnemyInfo {

	ENEMY_TYPES type = ENEMY_TYPES::ENEMYERROR404;
	iPoint position;

};

class Enemy :public Entity {
public:
	Enemy(iPoint pos,ENEMY_TYPES type);
	~Enemy();
protected:
	
public:

	Animation *animation = nullptr;
	iPoint position;
	SDL_Texture *sprites;
	
	const Collider* GetCollider() const;

	ENEMY_TYPES type;
	
public:

	 void Move() {};
	 void Draw(SDL_Texture *sprites);
	 void OnCollision(Collider *collider1,Collider *collider2);
public:
	Enemy* CreateEnemy(iPoint pos, ENEMY_TYPES enemyType, const char* path, pugi::xml_document &EnemiesDocument);

	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);
	void LoadPushbacks(pugi::xml_node node, Animation &animation);
	void DestroyEnemy(Enemy *Enemy);
};


#endif


