#include "Entity.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Log.h"

j1Entity::j1Entity(iPoint pos, ENTITY_TYPE Type):position(pos), type(Type){
	
}

bool j1Entity::CleanUp() {

	if (collider != nullptr)
		collider->to_delete = true;
	
	return true;
}
void j1Entity::Draw() {
     App->render->Blit(texture, position.x - 250, position.y - 200, &current_animation.GetCurrentFrame(), 0, 0, SDL_FLIP_NONE, NULL, NULL, BlitScale);
	
}