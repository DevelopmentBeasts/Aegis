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
	velocity.x = 8;
	velocity.y = 8;
	return true;
}

bool EnemyWorm::Update(float dt) {
	
	//if (App->input->GetKey(SDL_SCANCODE_O) == j1KeyState::KEY_DOWN && (position.x - App->scene->PlayerPt->position.x < 700))
	//{
	//	path = App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y));
	//	//path = App->pathfinding->GetLastPath();

	//	i = 0;
	//	change_iterator = false;
	//}

 //	if (path != nullptr) {
	//	App->pathfinding->DrawPath(path);
	//	Move(*path, dt);
	//}

	

	Draw(0,0);

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
void EnemyWorm::Move(const p2DynArray<iPoint>&path, float dt) {

	//const p2DynArray<iPoint>* Path = path;

	curr_direction = NewMovement(&path);

	switch (curr_direction) {

	case UP_RIGHT:
		position.x += velocity.x;
		position.y -= velocity.y;
		break;
	case UP_LEFT:
		position.x -= velocity.x;
		position.y -= velocity.y;
		break;
	case DOWN_RIGHT:
		position.x += velocity.x;
		position.y += velocity.y;
		break;
	case DOWN_LEFT:
		position.x -= velocity.x;
		position.y += velocity.y;
		break;
	case RIGHT:
		position.x += velocity.x;
		break;
	case LEFT:
		position.x -= velocity.x;
		break;
	case UP:
		position.y -= velocity.y;
		break;
	case DOWN:
		position.y += velocity.y;
		break;
	case NO_DIRECTION:
		LOG("NOT MOVING");
	}
}
EntityDirection EnemyWorm::NewMovement(const p2DynArray<iPoint>*EntityPath) {

	if (EntityPath->Count() >= 2) {

		if (EntityPath->At(i + 2) != nullptr) {
			iPoint current_Tile;
			current_Tile.x = EntityPath->At(i)->x;
			current_Tile.y = EntityPath->At(i)->y;
			iPoint next_Tile;
			next_Tile.x = EntityPath->At(i + 1)->x;
			next_Tile.y = EntityPath->At(i + 1)->y;
			iPoint Direction_comp;

			Direction_comp.x = next_Tile.x - current_Tile.x;
			Direction_comp.y = next_Tile.y - current_Tile.y;
			if (change_iterator) {
				i++;
			}

			change_iterator = !change_iterator;


			if (Direction_comp.x == 1 && Direction_comp.y == 1) {
				return DOWN_RIGHT;

			}
			else if (Direction_comp.x == -1 && Direction_comp.y == 1) {
				return DOWN_LEFT;

			}
			else if (Direction_comp.x == 1 && Direction_comp.y == -1) {
				return UP_RIGHT;
			}
			else if (Direction_comp.x == 1 && Direction_comp.y == -1) {
				return UP_LEFT;

			}
			else if (Direction_comp.x == 1) {
				return RIGHT;

			}
			else if (Direction_comp.x == -1) {
				return LEFT;

			}
			else if (Direction_comp.y == 1) {
				return DOWN;

			}
			else if (Direction_comp.y == -1) {
				return UP;

			}


		}
		else return NO_DIRECTION;
	}
}
bool EnemyWorm::DetectThePlayer() {
	iPoint player;
	player.x = App->scene->PlayerPt->position.x;
	player.y = App->scene->PlayerPt->position.y;
	int xdistance = player.x - position.x;
	if (xdistance*-1 > 0) {
		xdistance *= -1;
	}
	int ydistance = player.y - position.y;
	if (ydistance*-1 > 0) {
		ydistance *= -1;
	}
	if (xdistance < 3 && ydistance < 3) {
		return true;
	}
}