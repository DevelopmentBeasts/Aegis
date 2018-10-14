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


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
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

	LoadLevel(level1);
	
	
	current_map = App->map;
	
	current_map->DrawColliders();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)			//Save game
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)			//Save game
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {//Load game
	/*	LoadLevel(level1);*/
		LoadLevel1NOW = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)	//Load game
		LoadLevel2NOW = true;
		
	

	if (LoadLevel1NOW && App->player->jumping) {
		LoadLevel(level1);
		LoadLevel1NOW = false;
	}
	if (LoadLevel2NOW && App->player->jumping) {
		LoadLevel(level2);
		LoadLevel2NOW = false;
	}

	

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)			//Load game
	{
		App->player->data.xpos = App->map->data.start_position.x;
		App->player->data.ypos = App->map->data.start_position.y;

		App->render->CenterCamera();
	}


	//Draw the map
	current_map->Draw();				

	//Draw the title
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("AEGIS Version 0.2"); /*Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);*/

	App->win->SetTitle(title.GetString());
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
	
	LoadLevel(data.attribute("level").as_string());

	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{

	data.append_attribute("level") = current_level.GetString();

	return true;
}

void j1Scene::LoadLevel(const char* leveltoload) {
		
		App->map->CleanUp();
		App->map->Load(leveltoload);
		App->map->DrawColliders();
		current_level = leveltoload;

		App->player->data.xpos = App->map->data.start_position.x;
		App->player->data.ypos = App->map->data.start_position.y;

		App->render->CenterCamera();

}
