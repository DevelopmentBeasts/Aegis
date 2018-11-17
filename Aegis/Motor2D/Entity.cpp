#include "Entity.h"
#include "j1Render.h"

j1Entity::j1Entity(int x, int y, ENTITY_TYPE Type):position(x,y), type(Type){}

bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;
	
	return true;
}

void j1Entity::Draw() {

	if (current_animation!=nullptr)
		App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame());
}