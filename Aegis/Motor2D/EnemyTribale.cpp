#include "EnemyTribale.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"
#include "EntityManager.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "Player.h"
#include "p2DynArray.h"
#include "j1Input.h"

EnemyTribale::EnemyTribale(iPoint pos) : j1Enemy(pos, ENEMY_TYPE::TRIBALE) {

	properties_node = App->j1entity_manager->properties_doc.child("properties").child("Tribale");

	idle.LoadPushbacks(properties_node.child("idle"));
	move_left.LoadPushbacks(properties_node.child("move_left"));
	move_right.LoadPushbacks(properties_node.child("move_right"));
	attack.LoadPushbacks(properties_node.child("attack"));


	TribaleRect.x = pos.x;
	TribaleRect.y = pos.y;
	position.x = pos.x;
	position.y = pos.y;
	TribaleRect.w = 25;
	TribaleRect.h = 43;
}

EnemyTribale::~EnemyTribale() {}

bool EnemyTribale::Start() {
	TribaleCollider = App->collision->AddEntCollider(TribaleRect, COLLIDER_ENEMY, this);
	

	texture = App->j1entity_manager->tribale_texture;
	current_animation = &idle;
	velocity.x = 8;
	velocity.y = 8;
	Gravity = 0;
	return true;
}

bool EnemyTribale::Update(float dt) {

 	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && (position.x - App->scene->PlayerPt->position.x < 700))
	{
 	    App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y));
	    path = App->pathfinding->GetLastPath();	
		i = 0;
		change_iterator = false;
    }

	if (path != nullptr) {
		App->pathfinding->DrawPath(path);
		Move(path, dt);
	}
		

	if (App->framerate_cap_activated) {
		dt = 30;
	}
	if (Gravity) {
		velocity.y += GravityValue * (dt / 30);
		position.y += velocity.y*(dt / 30);
	}

	//TribaleCollider->SetPos(position.x+65, position.y);
	
	Draw();

	return true;
}
void EnemyTribale::OnCollision(Collider *c1, Collider *c2) {
	
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

void EnemyTribale::Move(const p2DynArray<iPoint>*path, float dt) {

	const p2DynArray<iPoint>* Path = path;

	curr_direction = NewMovement(Path);

	

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
bool EnemyTribale::DetectThePlayer() {
	iPoint player;
	player.x = App->scene->PlayerPt->position.x;
	player.y = App->scene->PlayerPt->position.y;
	int xdistance = player.x - position.x;
	if (xdistance*-1 > 0) {
		xdistance*=-1;
	}
	int ydistance = player.y - position.y;
	if (ydistance*-1 > 0) {
		ydistance *= -1;
	}
	if (xdistance < 3 && ydistance < 3) {
		return true;
	}
}
EntityDirection EnemyTribale::NewMovement(const p2DynArray<iPoint>*EntityPath) {

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