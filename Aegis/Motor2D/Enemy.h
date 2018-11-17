//#ifndef ENEMY_H_
//#define ENEMY_H_
//
//#include "p2Point.h"
//#include "Animation.h"
//#include "j1Textures.h"
//#include "EntityManager.h"
//#include "Entity.h"
//struct SDL_Textures;
//struct Collider;
//
//enum ENEMY_TYPES {
//
//	ENEMYERROR404,
//	WORM,
//	REPTIL
//
//};
//struct EnemyInfo {
//
//	ENEMY_TYPES type = ENEMY_TYPES::ENEMYERROR404;
//	iPoint position;
//
//};
//
//class Enemy :public Entity {
//public:
//	Enemy(iPoint pos,ENEMY_TYPES type);
//	~Enemy();
//protected:
//	
//public:
//
//	Animation *animation = nullptr;
//	iPoint position;
//	SDL_Texture *sprites;
//	
//	const Collider* GetCollider() const;
//
//	ENEMY_TYPES type;
//	
//public:
//
//	 void Move() {};
//	 void Draw(SDL_Texture *sprites);
//	 void OnCollision(Collider *collider1,Collider *collider2);
//public:
//	Enemy* CreateEnemy(iPoint pos, ENEMY_TYPES enemyType, const char* path, pugi::xml_document &EnemiesDocument);
//
//	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);
//	void LoadPushbacks(pugi::xml_node node, Animation &animation);
//	void DestroyEnemy(Enemy *Enemy);
//};
//
//
//#endif

#ifndef _J1_ENEMY_
#define _J1_ENEMY_

#include "Entity.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2List.h"

enum ENEMY_TYPE {

	NON = -1,
	FLYER,
	WALKER,
	UNKNOWN = 2
};

enum Enemy_direction {

	EN_RIGHT,
	EN_LEFT
};

class j1Enemy : public j1Entity
{

public:

	j1Enemy(iPoint pos, ENEMY_TYPE type_);
	~j1Enemy() {}

	void j1Enemy::OnCollision(Collider *c1, Collider *c2) override;

public:

	//Create/Destroy an Enemy
	j1Enemy* CreateEnemy(iPoint pos, ENEMY_TYPE enemyType, const char* path, pugi::xml_document &EnemiesDocument); //Remember to put maps/ at path
	void DestroyEnemy(j1Enemy *Enemy);

	//Load Enemy
	bool LoadEnemy(const char*file_name, pugi::xml_document &EnemiesDocument);
	//Pushbacks loading
	

public:

	bool falling = false;;

	ENEMY_TYPE type;
	Enemy_direction Direction = EN_RIGHT;

	iPoint enemy_position;
	fPoint enemy_velocity;
	iPoint initial_velocity;

	SDL_Texture *Enemy_tex = nullptr;
	SDL_Rect enemy_rect;
	SDL_Rect enemy_Collider_rect;

	//Path texture
	SDL_Texture *debug_tex;

public:

	pugi::xml_node		EnemySettings;
	pugi::xml_node		Animation_node;
};

#endif


