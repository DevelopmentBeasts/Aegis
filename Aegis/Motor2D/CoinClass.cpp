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
#include "CoinClass.h"
#include "SDL_mixer/include/SDL_mixer.h"


CoinClass::CoinClass(iPoint pos) :j1Entity(pos, ENTITY_TYPE::WIN) {
	position = pos;

	pugi::xml_parse_result result = AnimsDoc.load_file("EntitiesConfig.xml");

	if (result == NULL) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml.  error: %s", result.description());
	}

	AnimsNode = AnimsDoc.child("properties").child("coin").child("idl");
	CoinAnim.LoadPushbacks(AnimsNode);

}
CoinClass::~CoinClass() {

}
bool CoinClass::Start() {
	bool ret = true;
	//Tex = App->j1entity_manager->Coin_Texture;
	texture = App->tex->Load("textures/tribale_sprites.png");
	CoinCollider = App->collision->AddEntCollider({ position.x-10,position.y-10,10,20 }, COLLIDER_COIN, this);
	current_animation = &CoinAnim;
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048);
	coindropsound = Mix_LoadWAV("audio/fx/coindrop.wav");
	
	return ret;
}

bool CoinClass::Update(float dt) {

	Draw();
	return true;
}
void CoinClass::OnCollision(Collider* c1, Collider* c2) {
	if (c2->type == COLLIDER_PLAYER) {
		//sonidito.WAV
		App->scene->PlayerPt->PlayerCoins++;
		CoinCollider->to_delete = true;
		App->tex->UnLoad(texture);
		Mix_PlayChannel(3, coindropsound, 1);
		//Mix_FreeChunk(coindropsound);
	}
}
void CoinClass::Draw() {

	App->render->Blit(texture, position.x - 10, position.y - 10, &current_animation->GetCurrentFrame(), 1.0, 0, SDL_FLIP_NONE, NULL, NULL, 1.5);
}