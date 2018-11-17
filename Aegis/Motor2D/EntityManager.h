//#ifndef ENTITYMANAGER_H__
//#define ENTITYMANAGER_H__
//
//
//#include "j1Module.h"
//#include "SDL/include/SDL.h"
////#include "Player.h"
////ENTITY MANAGER
//
//#include "Entity.h"
//#include "p2List.h"
//#include "Enemy.h"
//#include "PugiXml\src\pugixml.hpp"
//
//#define TIMES_PER_SEC 5
//
//class EntityManager : public j1Module
//{
//public:
//
//	EntityManager();
//	~EntityManager();
//
//public:
//
//	bool Awake();
//	bool Start();
//	bool PreUpdate();
//	bool Update(float dt);
//	bool CleanUp();
//
//
//public:
//
//	Entity *CreateEntity(EntityType eType);
//	void DestroyEntity(Entity *Entity);
//
//public:
//
//	p2List<Entity*> entities_list;
//
//	float accumulated_time;
//	float update_ms_cycle;
//	int times_per_sec;
//
//	bool do_logic;
//
//};
//
//#endif


#ifndef _J1_ENTITY_MANAGER
#define _J1_ENTITY_MANAGER

#include "j1Module.h"
#include "Entity.h"
#include "p2List.h"
#include "Enemy.h"
#include "PugiXml\src\pugixml.hpp"

#define TIMES_PER_SEC 5

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

public:

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();


public:

	j1Entity *CreateEntity(ENTITY_TYPE eType);
	void DestroyEntity(j1Entity *Entity);

public:

	p2List<j1Entity*> entities_list;

	float accumulated_time;
	float update_ms_cycle;
	int times_per_sec;

	bool do_logic;

};

#endif