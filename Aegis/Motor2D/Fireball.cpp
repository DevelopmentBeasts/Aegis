#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "EntityManager.h"
#include "Player.h"
#include "j1Collision.h"
#include "Fireball.h"

FireBallClass::FireBallClass(iPoint pos) :j1Entity(pos, ENTITY_TYPE::WIN) {
	position = pos;

	pugi::xml_parse_result result = AnimsDoc.load_file("EntitiesConfig.xml");

	if (result == NULL) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml.  error: %s", result.description());
	}

	AnimsNode = AnimsDoc.child("properties").child("Win").child("idl");
	
	AnimsNode = AnimsDoc.child("properties").child("Win").child("Explosion");
	movementanim.LoadPushbacks(AnimsNode);
	texture = App->tex->Load("textures/explosions.png");

}
bool FireBallClass::Start() {

	FireBallCollider = App->collision->AddEntCollider({ position.x,position.y,20,30 }, COLLIDER_WIN, this);

	return true;
}

bool FireBallClass::Update(float dt) {
	//FRAMERATE CONTROL
 	if (App->framerate_cap_activated) {
		dt = 30;
	}
	velocity.x = 10 * (dt / 30);



	Draw();
	
	FireBallCollider->rect.x *= velocity.x;
	return true;
}
void FireBallClass::OnCollision(Collider* c1, Collider* c2) {
	
}
void FireBallClass::Draw() {
	App->render->Blit(texture, position.x - 10, position.y - 10, &current_animation->GetCurrentFrame(), 1.0, 0, SDL_FLIP_NONE, NULL, NULL, 0.5);
}