#include "Entity.h"
#include "j1Render.h"
#include "Player.h"

j1Entity::j1Entity(iPoint pos, ENTITY_TYPE Type):position(pos), type(Type){

}

bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;
	
	return true;
}
