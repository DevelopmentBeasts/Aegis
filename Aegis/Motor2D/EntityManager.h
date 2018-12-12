#ifndef _J1_ENTITY_MANAGER
#define _J1_ENTITY_MANAGER

#include "j1Module.h"
#include "Entity.h"
#include "p2List.h"
#include "Enemy.h"
#include "PugiXml\src\pugixml.hpp"


enum class ENTITY_TYPE {

	ENEMY,
	PLAYER,
	WIN,
	UNKNOWN = 2

};


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	~j1EntityManager();

	bool Awake();
	bool Start();				//Load textures for each entity

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	j1Entity *CreateEntity(int x , int y, ENTITY_TYPE eType);
	j1Entity *CreateEnemy(int x, int y, ENEMY_TYPE );
	void DestroyEntity(j1Entity *Entity);

	p2List<j1Entity*> entities_list;

	float accumulated_time;
	float update_ms_cycle;

public:

	bool do_logic;

	//Document with the properties of all the entities
	pugi::xml_document properties_doc;

	//Textures for the entities
	SDL_Texture* player_texture;
	SDL_Texture* worm_texture;
	SDL_Texture* tribale_texture;
	SDL_Texture* debug_texture;
	SDL_Texture* Win_Texture;

};

#endif