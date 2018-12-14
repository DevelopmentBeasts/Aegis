#include "EnemyWorm.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"
#include "EntityManager.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Input.h"

EnemyWorm::EnemyWorm(iPoint pos): j1Enemy(pos, ENEMY_TYPE::WORM) {
	
	properties_node = App->j1entity_manager->properties_doc.child("properties").child("worm");

	idle.LoadPushbacks(properties_node.child("idle"));
	moving.LoadPushbacks(properties_node.child("move"));

	WormRect.x = pos.x;
	WormRect.y = pos.y;
	WormRect.h = 20;
	WormRect.w = 20;

	position.x = pos.x;
	position.y = pos.y;



}

EnemyWorm::~EnemyWorm() {}

bool EnemyWorm::Start() {

	texture= App->j1entity_manager->worm_texture;
	debug_texture = App->j1entity_manager->debug_texture;
	current_animation = &idle;
	
	return true;
}

bool EnemyWorm::Update(float dt) {
	/*
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && (position.x - App->scene->PlayerPt->position.x < 700))
	{
		App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y));
		path = App->pathfinding->GetLastPath();
		App->pathfinding->i = 0;
		App->pathfinding->change_iterator = false;
	}
	
	iPoint to = App->scene->PlayerPt->position;

	DrawPath();

	Draw();*/

	return true;
}

void EnemyWorm::OnCollision(Collider *c1, Collider *c2) {
	if (c2->type == COLLIDER_WALL) {

		//Calculating an error margin of collision to avoid problems with colliders corners

		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + velocity.y) { //Colliding Up (falling)
				//LOG("COLLIDING UP");
				//LOG("BOOL COLLIDING UP TRUE");

			velocity.y = 0;
			position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);

		}
	}
	if (c2->type == COLLIDER_PLAYER) {
		//App->scene->PlayerPt.die
	}
}