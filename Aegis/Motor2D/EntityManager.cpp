#include "EntityManager.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1Textures.h"
#include "EnemyWorm.h"
#include "j1Render.h"
#include "Player.h"
#include "EnemyTribale.h"
j1EntityManager::j1EntityManager()
{
	name.create("entities");

	//Load the document of all the properties
	pugi::xml_parse_result result = properties_doc.load_file("EntitiesConfig.xml");


}


j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake() {

	LOG("AWAKING ENTITY MANAGER");

	return true;
}

bool j1EntityManager::Start() {

	//Load textures
	player_texture	= App->tex->Load("textures/Fire_Wisp/fireSheet.png");
	worm_texture	= App->tex->Load("textures/worm_sprites.png");
	tribale_texture = App->tex->Load("textures/tribale_sprites.png");

	//Execute start() of every entity
	p2List_item<j1Entity*>*item = entities_list.start;
	for (; item != nullptr; item = item->next) {
		item->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate() {

	do_logic = false;
	return true;
}

bool j1EntityManager::Update(float dt) {

	p2List_item<j1Entity*>*item = entities_list.start;
	for (; item != nullptr; item = item->next) {
		item->data->Update(dt);
	}

	accumulated_time = 0.0f;

	return true;
}

bool j1EntityManager::CleanUp() {

	LOG("Clean Up Entity Manager");

	p2List_item<j1Entity*>* item;
	item = entities_list.start;

	while (item != nullptr) {

		RELEASE(item->data);
		item = item->next;
	}

	entities_list.clear();

	//Unload textures
	App->tex->UnLoad(player_texture);
	return true;
}

j1Entity *j1EntityManager::CreateEntity(int x, int y, ENTITY_TYPE eType) {

	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "UPDATE ENTITY TYPES");

	j1Entity* Entity = nullptr;

	switch (eType) {

	case ENTITY_TYPE::PLAYER:
		Entity = new PlayerClass(iPoint (x,y));
		break;
	
	default:
		break;
	}

	entities_list.add(Entity);
	Entity->Start();
	return Entity;
}

j1Entity* j1EntityManager::CreateEnemy(int x, int y, ENEMY_TYPE Type) {

	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "UPDATE ENEMIES");

	j1Entity* Entity = nullptr;

	switch (Type) {
	case ENEMY_TYPE::WORM:
		Entity = new EnemyWorm(iPoint(x, y));
	case ENEMY_TYPE::TRIBALE:
		Entity = new EnemyTribale(iPoint(x, y));
	}
	entities_list.add(Entity);
	Entity->Start();
	return Entity;
}

void j1EntityManager::DestroyEntity(j1Entity *Entity) {

	p2List_item<j1Entity*>*item = entities_list.start;

	while (item != nullptr) {

		if (item->data == Entity) {

			entities_list.del(item);
			RELEASE(item->data);
			break;
		}

		item = item->next;
	}
}