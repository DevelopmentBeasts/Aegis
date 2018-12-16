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
#include "WinClass.h"

WinClass::WinClass(iPoint pos):j1Entity(pos, ENTITY_TYPE::WIN){
	position = pos;

	pugi::xml_parse_result result = AnimsDoc.load_file("EntitiesConfig.xml");

	if (result == NULL) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml.  error: %s", result.description());
	}
	
	AnimsNode = AnimsDoc.child("properties").child("Win").child("idl");
	DiamondAnim.LoadPushbacks(AnimsNode);
	AnimsNode = AnimsDoc.child("properties").child("Win").child("Explosion");
	ExplosionAnim.LoadPushbacks(AnimsNode);
	texture = App->tex->Load("textures/explosions.png");

}
bool WinClass::Start() {
	bool ret = true;
	Tex = App->j1entity_manager->Win_Texture;
	WinCollider = App->collision->AddEntCollider({ position.x,position.y,20,30 }, COLLIDER_WIN, this);
	
	current_animation = &DiamondAnim;
	win_explosion = &ExplosionAnim;
	return ret;
}

bool WinClass::Update(float dt) {
	Draw();
	if (Explosion && win_explosion->Finished()) {
		Explosion = false;
		CleanUp();
		App->scene->LoadLevel(App->scene->level2);
	}
	return true;
}
void WinClass::OnCollision(Collider* c1, Collider* c2) {
	if (c2->type == COLLIDER_PLAYER && App->scene->PlayerPt->PlayerCoins != 0) {
		Explosion = true;
		
	}
}
void WinClass::Draw() {
	App->render->Blit(texture, position.x - 10, position.y - 10, &current_animation->GetCurrentFrame(), 1.0, 0, SDL_FLIP_NONE, NULL, NULL, 0.5);
	if (Explosion) {
		App->render->Blit(texture, position.x - 140, position.y - 150, &win_explosion->GetCurrentFrame(), 1.0, 0, SDL_FLIP_NONE, NULL, NULL, 3);
	}
}