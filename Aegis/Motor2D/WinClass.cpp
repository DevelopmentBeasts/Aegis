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
	

}
bool WinClass::Start() {
	bool ret = true;
	Tex = App->j1entity_manager->Win_Texture;
	WinCollider = App->collision->AddEntCollider({ position.x,position.y,20,20 }, COLLIDER_WIN, this);
	
	return ret;
}

bool WinClass::Update(float dt) {
	
	if (!Explosion) {
		current_animation = &DiamondAnim;
	}
	if (Explosion) {
		current_animation = &ExplosionAnim;
	}	

	return true;
}
void WinClass::OnCollision(Collider* c1, Collider* c2) {
	if (c2->type == COLLIDER_PLAYER) {
		Explosion = true;
	}
}