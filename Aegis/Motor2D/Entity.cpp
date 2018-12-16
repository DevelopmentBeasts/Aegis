#include "Entity.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Animation.h"
#include "Player.h"

j1Entity::j1Entity(iPoint pos, ENTITY_TYPE Type):position(pos), type(Type){
	
}

bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;
	
	return true;
}
void j1Entity::Die() {
	
	App->scene->PlayerPt->die = true;;
	
}
