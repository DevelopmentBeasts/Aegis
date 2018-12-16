#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Enemy.h"
#include "EnemyWorm.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "SDL_mixer/include/SDL_mixer.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	intro = "Main_menu.tmx";
	level1 = "MAGIC_CAVES.tmx";
	level2 = "MAGIC_CAVES_2.tmx";

	
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{	
	App->render->camera.x = -1000;

	fade_step = FadeStep::fade_none;
	fade_time = 2000;
	fade_rect.w = App->render->camera.w;
	fade_rect.h = App->render->camera.h;

	current_map = App->map;

	PlayerPt = App->j1entity_manager->CreateEntity(App->map->data.start_position.x, App->map->data.start_position.y, ENTITY_TYPE::PLAYER);

	EnemyTribale1 = App->j1entity_manager->CreateEnemy(600, 100, ENEMY_TYPE::TRIBALE);
	//EnemyWorm1 = App->j1entity_manager->CreateEnemy(1400, 250, ENEMY_TYPE::WORM);
	CoinPt = App->j1entity_manager->CreateEntity(300, 300, ENTITY_TYPE::COIN);
	if (PlayerPt != nullptr) {
		PlayerExists = true;
	}	

	SceneMusic = Mix_LoadMUS("audio\music\gamemusic.wav");
	
	Mix_PlayMusic(SceneMusic, -1);
	LoadLevel(intro);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	fade_rect.x = -App->render->camera.x;
	fade_rect.y = -App->render->camera.y;

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	
	if (SceneLoaded) {
		PlayerPt->position.x = App->map->data.start_position.x;
		PlayerPt->position.y = App->map->data.start_position.y;
		SceneLoaded = false;
		PlayerExists = true;//no hace falta pero por si acaso
	}
	
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)			//Save game
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)			//Save game
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 		//Load game
		FadeToBlack(level1);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)		//Load game
		FadeToBlack(level2);
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)			//Load game
		FadeToBlack(*current_level);

	if (PlayerPt->position.x >= App->map->data.wincondition) {

		LoadLevel(level2);
		//App->render->FindPlayer(dt);
	}

	if (current_level == &intro)
		App->render->camera.x -= 1 * dt / 15;


	//Draw the map
	current_map->Draw();

	//Fade
	UpdateFade();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// Load Game State
bool j1Scene::Load(pugi::xml_node& data)
{
	
	LoadLevel(p2SString(data.attribute("level").as_string()));

	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{

	data.append_attribute("level") = current_level->GetString();

	return true;
}

void j1Scene::LoadLevel(p2SString &level_to_load) {
	
	if (&level_to_load != current_level)
	{
		App->map->CleanUp();
		App->map->Load(level_to_load.GetString());
		current_level = &level_to_load;

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

	}

		PlayerPt->position.x = App->map->data.start_position.x;
		PlayerPt->position.y = App->map->data.start_position.y;
		
		App->render->CenterCamera();
		
		SceneLoaded = true;
		
		App->render->camera.x = -100;
		
		App->render->find_player = true;
		

}

void j1Scene::UpdateFade() 
{
	if (fade_step == FadeStep::fade_none)
		return;

	static uint phase_fade_time = fade_time/2;

	Uint32 now = SDL_GetTicks() - fade_start_time;
	float normalized = MIN(1.0f, (float)now / (float)fade_time);

	switch (fade_step)
	{
	case FadeStep::fade_to_black:
		if (now >= fade_time)
		{
			App->render->DrawQuad(fade_rect, 0, 0, 0, (Uint8)(normalized * 255.0f));
			LoadLevel(*level_to_load);
			fade_start_time = SDL_GetTicks();
			fade_step = FadeStep::fade_from_black;
		}
		break;
	case FadeStep::fade_from_black:

		normalized = 1.0f - normalized;
		
		if (now > fade_time)
		{
			fade_step = FadeStep::fade_none;
		}
		break;
	}

	App->render->DrawQuad(fade_rect, 0, 0, 0, (Uint8)(normalized * 255.0f));

}

void j1Scene::FadeToBlack(p2SString &leveltoload)
{
	if (fade_step == FadeStep::fade_none)
	{
		fade_start_time = SDL_GetTicks();
		fade_step = FadeStep::fade_to_black;
		level_to_load = &leveltoload;
	}

}

void j1Scene::ButtonAction(UiButton* button)
{
	int i = 0;
}
