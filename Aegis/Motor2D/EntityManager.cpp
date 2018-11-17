#include "EntityManager.h"
#include "p2Log.h"
#include "Entity.h"
#include "j1Scene.h"



EntityManager::EntityManager()
{
	name.create("entities");
}


EntityManager::~EntityManager() {}

bool EntityManager::Awake() {

	LOG("AWAKING ENTITY MANAGER");
	times_per_sec = TIMES_PER_SEC; //Read this with an XML
	update_ms_cycle = 1.0f / (float)times_per_sec;

	return true;
}

bool EntityManager::Start() {

	p2List_item<Entity*>* item;
	item = entities_list.start;

	while (item != nullptr) {

		item->data->Start();
		item = item->next;
	}

	return true;
}

bool EntityManager::PreUpdate() {

	do_logic = false;
	return true;
}

bool EntityManager::Update(float dt) {

	/*accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	if (do_logic == true) {*/
	p2List_item<Entity*>*item = entities_list.start;
	for (; item != nullptr; item = item->next)
		item->data->Update(dt);

	accumulated_time = 0.0f;
	//}

	return true;
}

bool EntityManager::CleanUp() {

	LOG("Clean Up Entity Manager");

	p2List_item<Entity*>* item;
	item = entities_list.start;

	while (item != nullptr) {

		RELEASE(item->data);
		item = item->next;
	}

	entities_list.clear();

	return true;
}

Entity *EntityManager::CreateEntity(EntityType eType) {

	//static_assert(EntityType::NONVALID == EntityType(3), "UPDATE ENTITY TYPES");

	Entity* Entity_ = nullptr;

	switch (eType) {

	case EntityType::ENEMY:
		Entity_ = new Entity(EntityType::ENEMY);
		break;
	case EntityType::PLAYER:
		Entity_ = new Entity(EntityType::PLAYER);
		break;
	default:
		break;

	}

	entities_list.add(Entity_);
	return Entity_;
}


void EntityManager::DestroyEntity(Entity *Entity) {

	/*p2List_item<Entity*>*item = entities_list.start;

	while (item != nullptr) {

		if (item->data == Entity) {

			entities_list.del(item);
			RELEASE(item->data);
			break;
		}

		item = item->next;
	}*/
}