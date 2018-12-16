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

//#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

	mainmenu = "Main_menu.tmx";
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
	fade_step = FadeStep::fade_none;
	fade_time = 1000;
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

	LoadLevel(mainmenu);
	
	App->audio->PlayMusic("audio/music/Audionautix_SportsAction.ogg");

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
	//This should be in Start(), but for some reason it doesn't work
	//So I call it here
	static bool create_menu= ui_main_menu.Create();
	static bool create_pause_menu = CreatePauseWindow();
	static bool create_settings_window = CreateSettingsWindow();
	
	if (close_app)
		return false;
	
	if (SceneLoaded) {
		PlayerPt->position.x = App->map->data.start_position.x;
		PlayerPt->position.y = App->map->data.start_position.y;
		SceneLoaded = false;
		PlayerExists = true;//no hace falta pero por si acaso
	}
	
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)			//Save game
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) 
	{		//Save game
		App->SaveGame("save_game.xml");
		game_saved = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 		//Load game
		FadeToBlack(level1);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)		//Load game
		FadeToBlack(level2);
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)		//Load game
		FadeToBlack(current_level.GetString());

	if (current_level == mainmenu)
		App->render->camera.x -= 1 * dt / 30;


	//Draw the map
	current_map->Draw();
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	//Fade
	UpdateFade();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (current_level != mainmenu)
			ui_pause_window->ChangeState();
	}
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
	const char* load = data.attribute("level").as_string();

	FadeToBlack(load);

	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data)const 
{
	const char* level = current_level.GetString();

	data.append_attribute("level") = level;

	return true;
}

void j1Scene::LoadLevel(const char* level_to_load) {
	
	
	if (p2SString(level_to_load) != current_level)
	{
			App->map->CleanUp();
			App->map->Load(level_to_load);
			current_level = level_to_load;

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

	}
	

	if (PlayerPt!=nullptr)
	{
		PlayerPt->position.x = App->map->data.start_position.x;
		PlayerPt->position.y = App->map->data.start_position.y;
		
		App->render->CenterCamera();
		App->render->find_player = true;
		SceneLoaded = true;
	}
	
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
			LoadLevel(level_to_load.GetString());
			fade_start_time = SDL_GetTicks();
			fade_step = FadeStep::fade_from_black;

			//UI
			if (level_to_load != mainmenu)
				ui_main_menu.Hide();
			else
			{
				ui_pause_window->ChangeState();
				ui_main_menu.Show();
			}

			if (ui_settings_window->active)
				ui_settings_window->ChangeState();
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

void j1Scene::FadeToBlack(const char* leveltoload)
{
	if (fade_step == FadeStep::fade_none)
	{
		fade_start_time = SDL_GetTicks();
		fade_step = FadeStep::fade_to_black;
		level_to_load = leveltoload;
	}

}

void j1Scene::ButtonAction(UiButton* button)
{
	switch (button->function)
	{
	case ButtonFunction::CLOSE_APP:

		close_app = true;
		break;

	case ButtonFunction::HIDE_SETTINGS:
		ui_settings_window->ChangeState();
		break;

	case ButtonFunction::HIDE_PAUSE:
		ui_pause_window->ChangeState();
		break;

	case ButtonFunction::LOAD_MAIN_MENU:

		FadeToBlack(mainmenu);
		break;

	case ButtonFunction::LOAD_LEVEL1:

		FadeToBlack(level1);
		break;

	case ButtonFunction::SAVE_GAME:
		App->SaveGame("save_game.xml");
		game_saved = true;
		break;

	case ButtonFunction::LOAD_GAME:
		App->LoadGame("save_game.xml");
		break;

	case ButtonFunction::OPEN_SETTINGS:
		ui_settings_window->ChangeState();
		break;

	case ButtonFunction::OPEN_GITHUB:

		ShellExecuteA(NULL, "open", "https://github.com/DevelopmentBeasts/Aegis", NULL, NULL, SW_SHOWNORMAL);
		break;

	}
}

bool j1Scene::CreatePauseWindow()
{
	ui_pause_window = App->gui->AddWindow({ 650,50 });

	//Close window
	UiButton* exit = ui_pause_window->NestButton({0,0}, ButtonSize::SMALL, App->scene, ButtonFunction::HIDE_PAUSE);
	exit->NestImage({ 0, 0 }, App->gui->exit);
	
	//go to main menu
	UiButton* main_menu = ui_pause_window->NestButton({170,700}, ButtonSize::EXTRA, App->scene, ButtonFunction::LOAD_MAIN_MENU);
	main_menu->NestLabel({ 70,30 }, "MENU");

	//load game
	UiButton* load = ui_pause_window->NestButton({200,330}, ButtonSize::BIG,App->scene, ButtonFunction::LOAD_GAME);
	load->NestLabel({55,30},"LOAD");

	//save game
	UiButton* save = ui_pause_window->NestButton({200,150}, ButtonSize::BIG, App->scene, ButtonFunction::SAVE_GAME);
	save->NestLabel({55,30}, "SAVE");

	//settings
	UiButton* settings = ui_pause_window->NestButton({170,500}, ButtonSize::EXTRA, App->scene, ButtonFunction::OPEN_SETTINGS);
	settings->NestLabel({15,30},"SETTINGS");

	ui_pause_window->ChangeState();

	return true;
}

bool UiMainMenu::Create()
{

	//play button
	play = App->gui->AddButton({ 850,500 }, ButtonSize::BIG, App->scene, ButtonFunction::LOAD_LEVEL1);
	play->NestLabel({ 55,30 }, "PLAY");

	//exit button
	exit = App->gui->AddButton({ 850,875 }, ButtonSize::BIG, App->scene, ButtonFunction::CLOSE_APP);
	exit->NestLabel({ 55,30 }, "EXIT");

	//settings button
	settings = App->gui->AddButton({ 820,750 }, ButtonSize::EXTRA, App->scene, ButtonFunction::OPEN_SETTINGS);
	settings->NestLabel({ 16,30 }, "SETTINGS");

	//continue button | laod game
	load = App->gui->AddButton({ 820,625 }, ButtonSize::EXTRA, App->scene, ButtonFunction::LOAD_GAME);
	load->NestLabel({ 16, 30 }, "CONTINUE");
	load->active = false;

	//github
	github = App->gui->AddButton({ 150, 775 }, ButtonSize::BIG, App->scene, ButtonFunction::OPEN_GITHUB);
	github->NestLabel({20,30},"GITHUB");

	return true;
}

void UiMainMenu::Hide()
{
	play->active = false;
	exit->active = false;
	settings->active = false;
	load->active = false;
	github->active = false;
}

void UiMainMenu::Show()
{
	play->active = true;
	exit->active = true;
	settings->active = true;
	github->active = true;

	if (App->scene->game_saved)
		load->active = true;
}

bool j1Scene::CreateSettingsWindow()
{

	ui_settings_window = App->gui->AddWindow({1200,50});

	//exit
	UiButton* window_exit = ui_settings_window->NestButton({ 0,0 }, ButtonSize::SMALL, App->scene, ButtonFunction::HIDE_SETTINGS);
	window_exit->NestImage({ 0,0 }, App->gui->exit);

	//music bar
	music_bar = ui_settings_window->NestBar({ 300,200 });
	ui_settings_window->NestLabel({50,200},"MUSIC");

	//sound bar
	fx_bar= ui_settings_window->NestBar({ 300,450 });
	ui_settings_window->NestLabel({50, 450 }, "FX");

	//volume label
	ui_settings_window->NestLabel({330,100},"VOLUME");

	ui_settings_window->ChangeState();

	return true;
}

float j1Scene::GetFxVolume()
{
	if (fx_bar != nullptr)
		return fx_bar->GetValue();

	else
		return 1.0;
}

float j1Scene::GetMusicVolume()
{
	if (music_bar != nullptr)
		return music_bar->GetValue();

	else 
		return 1.0;
}