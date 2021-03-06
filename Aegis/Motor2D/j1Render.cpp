#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "player.h"
#include "j1Input.h"
#include "Brofiler/Brofiler.h"
#include "j1Scene.h"
j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	//laod borders
	pugi::xml_node borders= config.child("borders");
	top_border = borders.attribute("top").as_int();
	bot_border = borders.attribute("bot").as_int();
	left_border = borders.attribute("left").as_int();
	right_border = borders.attribute("right").as_int();

	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	BROFILER_CATEGORY("Update(); - Render;", Profiler::Color::Orchid);

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		uint x, y;
		App->win->GetWindowSize(x, y);
		LOG("Window %u %u", x, y);
		LOG("Camera %u %u ", camera.w, camera.h);

	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN && debug == false) {
		debug = !debug;

	}else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN && debug == true) {
		debug = !debug;
		find_player = true;
	}

	if (find_player)
		FindPlayer(dt);

	if (debug == true) {
		FreeMovement();
	}
	else {
		FollowPlayer(dt);
	}

	return true;
}

bool j1Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle,SDL_RendererFlip flip, int pivot_x, int pivot_y,float extrascale) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * (scale);
	rect.y = (int)(camera.y * speed) + y * (scale);

	if(section != NULL)
	{
		rect.w = section->w*extrascale;
		rect.h = section->h*extrascale;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle,0, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void j1Render::CenterCamera(){ // only called at the load level (the center camera does not really centers the camera to the player because after being used, the find player activates and makes a soft transition)
	if (App->scene->PlayerExists) {
		camera.x = left_border - (App->scene->PlayerPt->position.x);
		camera.y = top_border - (App->scene->PlayerPt->position.y);
	}
	
		
}

void j1Render::FreeMovement() {
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		camera.y += 30;

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
		camera.y -= 30;

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
		camera.x += 30;

	if (App->input->GetKey(SDL_SCANCODE_V) == KEY_REPEAT)
		camera.x -= 30;
}

void j1Render::FollowPlayer(float dt) {
	//LOG("CAMERA X = %i",camera.x);
	if (App->scene->PlayerExists) {
	
			if ((App->scene->PlayerPt->position.x) < (-camera.x + left_border) && camera.x <-15) {				//Move the camera to the left if the player is going back and behnid the left border

				camera.x -= App->scene->PlayerPt->velocity.x*(dt / 30);
		
			}
			if ((App->scene->PlayerPt->position.x) > (-camera.x + camera.w - right_border)) {					//Move the camera to the right if the player is advancing and ahead of the border

				camera.x -= App->scene->PlayerPt->velocity.x*(dt / 30);
			}
		
		camera.y = 0;
		
	}
}

void j1Render::FindPlayer(float dt) {
	
		int vel = 20;
		/*if (camera.x < -1600) {*/
			if ((App->scene->PlayerPt->position.x) > (-camera.x + camera.w - right_border))
				camera.x -= vel * (dt / 30);
		/*}*/
		/*if (camera.x > 10) {*/
			if ((App->scene->PlayerPt->position.x) < (-camera.x + left_border))
				camera.x += vel * (dt / 30);
		/*}*/

		//if ((App->scene->PlayerPt->position.y) < (-camera.y + top_border))
		//	camera.y += vel * (dt / 30);

		//if ((App->scene->PlayerPt->position.y) > (-camera.y + camera.h - bot_border))
		//	camera.y -= vel * (dt / 30);

		if (((App->scene->PlayerPt->position.x) < (-camera.x + camera.w - right_border))//If we found the player, stop looking for it
			&& ((App->scene->PlayerPt->position.x) > (-camera.x + left_border))
			/*&& ((App->scene->PlayerPt->position.y) > (-camera.y + top_border))
			&& ((App->scene->PlayerPt->position.y) < (-camera.y + camera.h - bot_border))*/)
			find_player = false;
	
	
}
