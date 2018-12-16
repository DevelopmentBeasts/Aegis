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
	Attack.LoadPushbacks(properties_node.child("attack"));

	WormRect.x = pos.x;
	WormRect.y = pos.y;
	WormRect.h = 30;
	WormRect.w = 60;

	position.x = pos.x;
	position.y = pos.y;

	RightWormColliderSensorRect.x = WormRect.x-30 + WormRect.w;
	RightWormColliderSensorRect.y = WormRect.y;
	RightWormColliderSensorRect.w = 40+30;
	RightWormColliderSensorRect.h = 70;

	LeftWormColliderSensorRect.x = WormRect.x - 40;
	LeftWormColliderSensorRect.y = WormRect.y;
	LeftWormColliderSensorRect.w = 40+30;
	LeftWormColliderSensorRect.h = 70;

}

EnemyWorm::~EnemyWorm() {}

bool EnemyWorm::Start() {

	texture= App->j1entity_manager->worm_texture;
	debug_texture = App->j1entity_manager->debug_texture;
	current_animation = &idle;
	velocity.x = 8;
	velocity.y = 8;
	if (provisional = 1) {
		WormCollider = App->collision->AddEntCollider(WormRect, COLLIDER_ENEMY, this);
		RightWormColliderSensor = App->collision->AddEntCollider(RightWormColliderSensorRect, COLLIDER_ENEMY_SENSOR, this);
		LeftWormColliderSensor = App->collision->AddEntCollider(LeftWormColliderSensorRect, COLLIDER_ENEMY_SENSOR, this);
	}
	provisional++;

	return true;
}

bool EnemyWorm::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_F9) == j1KeyState::KEY_DOWN) {
		DebugDraw = !DebugDraw;
	}

	if (current_animation == &Attack) {
		if (current_animation->Finished()) {
			current_animation = &idle;
		}
	}
	int ActiveTarget = position.x - App->scene->PlayerPt->position.x;
	if (ActiveTarget*-1 > 0) {
		ActiveTarget *= -1;
	}
	if ((ActiveTarget<500))
	{
		iPoint initial_pos = App->map->WorldToMap(position.x, (position.y + 30));
		iPoint final_pos = App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y);

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos) /*&& App->input->GetKey(SDL_SCANCODE_O) == j1KeyState::KEY_REPEAT*/) {

			path = App->pathfinding->CreatePath(initial_pos, final_pos);
			//LOG("WORM PATHFINDING");

			
			//if (position.y - App->scene->PlayerPt->position.y < 100)
				
		}
		i = 0;
		change_iterator = false;
	}

 	if (path != nullptr) {
		if(DebugDraw)
			App->pathfinding->DrawPath(path);
		Move(*path, dt);
		//path->Clear();
	}

	DetectThePlayer();

	//WormCollider->SetPos(position.x, position.y);
	RightWormColliderSensor->SetPos(position.x + WormCollider->rect.w-30, position.y);
	LeftWormColliderSensor->SetPos(position.x - LeftWormColliderSensor->rect.w, position.y);


	if (!(App->framerate_cap_activated)) {
		idle.speed *= (dt / 30);
		moving.speed *= (dt / 30);
		Attack.speed *= (dt / 30);
	}
	if (velocity.y == 0)
		LOG("Y NOT WORKING");

	Draw(0,0);

	//App->render->Blit(texture, position.x, position.y, &current_animation->GetCurrentFrame(), 1/*Parallax*/, 0.0/*rotation*/, flip, NULL, NULL, 1.5);

	return true;
}

void EnemyWorm::OnCollision(Collider *c1, Collider *c2) {
	if (c1->type == COLLIDER_ENEMY_SENSOR && c2->type == COLLIDER_PLAYER) {
		current_animation = &Attack;

		if (current_animation->GetCurrentFrame().x == 200 && App->scene->PlayerPt->die != true) {
			if(current_animation->GetCurrentFrame().y == 80)
			LOG("DIE MOTHERFUCKER!");
			if (!(App->scene->PlayerPt->godmode_activated)) {
				App->scene->PlayerPt->die = true;
				App->scene->PlayerPt->velocity.x = 0;
				App->scene->PlayerPt->velocity.y = 0;
			}
		}
	}
	if (c2->type == COLLIDER_WALL && c1->type != COLLIDER_ENEMY_SENSOR) {

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
void EnemyWorm::Move(const p2DynArray<iPoint>&path_, float dt) {

	//const p2DynArray<iPoint>* Path = path;

	curr_direction = NewMovement(&path_);

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
	//case NO_DIRECTION:
		//LOG("NOT MOVING");
	}

	/*if (NewMovement(&path_) == NO_DIRECTION)
		path->Clear();*/

}
EntityDirection EnemyWorm::NewMovement(const p2DynArray<iPoint>*EntityPath) {
	velocity.y = 8;
	if (EntityPath->Count() >= 4) {
		int o = 3;
		if (SDL_FLIP_HORIZONTAL)
			o = 6;
		if (EntityPath->At(i + o) != nullptr) {
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
	iPoint Position;
	player.x = App->scene->PlayerPt->position.x;
	player.y = App->scene->PlayerPt->position.y;
	player = App->map->WorldToMap(player.x, player.y);
	Position = App->map->WorldToMap(position.x, position.y);
	int xdistance = player.x - Position.x;
	if (player.x - Position.x < 2) {
		flip = SDL_FLIP_NONE;
	}
	else if (player.x - Position.x > 2)
		flip = SDL_FLIP_HORIZONTAL;

	return true;

}