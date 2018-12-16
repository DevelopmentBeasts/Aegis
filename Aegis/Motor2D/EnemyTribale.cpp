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
	attackleft.LoadPushbacks(properties_node.child("attackleft"));
	jump.LoadPushbacks(properties_node.child("jump"));

	TribaleRect.x = pos.x;
	TribaleRect.y = pos.y;
	position.x = pos.x+34;
	position.y = pos.y+22;
	TribaleRect.w = 25;
	TribaleRect.h = 43;

	RightTribaleColliderSensorRect.x = pos.x + TribaleRect.w;
	RightTribaleColliderSensorRect.y = pos.y ;
	RightTribaleColliderSensorRect.w = 65;
	RightTribaleColliderSensorRect.h = 20;

	LeftTribaleColliderSensorRect.x = pos.x -65;
	LeftTribaleColliderSensorRect.y = pos.y ;
	LeftTribaleColliderSensorRect.w = 65;
	LeftTribaleColliderSensorRect.h = 20;
}

EnemyTribale::~EnemyTribale() {}

bool EnemyTribale::Start() {
	TribaleCollider = App->collision->AddEntCollider(TribaleRect, COLLIDER_ENEMY, this);
	RightTribaleColliderSensor = App->collision->AddEntCollider(RightTribaleColliderSensorRect, COLLIDER_ENEMY_SENSOR, this);
	LeftTribaleColliderSensor = App->collision->AddEntCollider(LeftTribaleColliderSensorRect, COLLIDER_ENEMY_SENSOR, this);

	texture = App->j1entity_manager->tribale_texture;
	current_animation = &idle;
	velocity.x = 8;
	velocity.y = 12;
	
	

	Gravity = false;
	return true;
}

bool EnemyTribale::Update(float dt) {
	
	DT += dt;
	if (App->input->GetKey(SDL_SCANCODE_F9) == j1KeyState::KEY_DOWN) {
		DebugDraw = !DebugDraw;
	}
	if (App->input->GetKey(SDL_SCANCODE_G) == j1KeyState::KEY_DOWN) {
		Gravity = true;
	}
	
	if (current_animation == &move_left)
		current_animation = &idle;
	if(current_animation == &jump)
		current_animation = &idle;
	if (current_animation == &attackleft && current_animation->Finished()) {
		current_animation->Reset();
		current_animation = &idle;
	}

	if (firstiteration) {
		current_animation = &idle;
		firstiteration = false;
	}
	int ActiveTarget = position.x - App->scene->PlayerPt->position.x;
	if (ActiveTarget*-1 > 0) {
		ActiveTarget *= -1;
	}

 	if (/*App->input->GetKey(SDL_SCANCODE_O) == j1KeyState::KEY_REPEAT &&*/Gravity && ActiveTarget<400)
	{
		DT = 0;
		iPoint initial_pos = App->map->WorldToMap(position.x, (position.y - 15));
		iPoint final_pos = App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y-25);

		if (App->pathfinding->IsWalkable(initial_pos) && App->pathfinding->IsWalkable(final_pos) /*&& App->input->GetKey(SDL_SCANCODE_O) == j1KeyState::KEY_REPEAT*/) {

			Path = App->pathfinding->CreatePath(initial_pos, final_pos);
			//LOG("TRIBALE PATHFINDING");
		}
		
		i = 0;
		change_iterator = false;
    }

	if ( Path !=nullptr) {
		if (DebugDraw) {
			App->pathfinding->DrawPath(Path);
		}
		
		if (position.y - App->scene->PlayerPt->position.y < 150)
			Move(*Path, dt);
		Path->Clear();
	
	}
	
	DetectThePlayer();
	
	if (App->framerate_cap_activated) {
		dt = 30;
	}
	if (Gravity) {
		velocity.y += GravityValue * (dt / 30);
		position.y += velocity.y*(dt / 30);
	}
	if (!(App->framerate_cap_activated)) {
		idle.speed *= (dt / 30);
		move_left.speed *= (dt / 30);
		attackleft.speed *= (dt / 30);
	}
	TribaleCollider->SetPos(position.x+65, position.y);
	RightTribaleColliderSensor->SetPos(TribaleCollider->rect.x+TribaleCollider->rect.w, position.y);
	LeftTribaleColliderSensor->SetPos(position.x, position.y);
	Draw(10,-50);

	//directly connected with the enemy jump, change enemy jump to 8 if this is commented or erased
	if (velocity.y != 0 & current_animation == &jump) {
		velocity.y += 0.25;
	}
	//App->render->Blit(texture, position.x + 10, position.y + -50, &current_animation->GetCurrentFrame(), 1/*Parallax*/, 0.0/*rotation*/, flip, NULL, NULL, 1.5);

	return true;
}
void EnemyTribale::OnCollision(Collider *c1, Collider *c2) {
	
	if (c1->type == COLLIDER_ENEMY_SENSOR && c2->type == COLLIDER_WALL) {
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Colliding Left (going right)
			//LOG(" sensor COLLIDING FROM LEFT");

			//position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 8;
			if (velocity.y == 0 && velocity.x != 0)
				velocity.y -=11;
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - ((velocity.x*-1) - velocity.x)) { //Colliding Right (going left)
			//LOG("sensor COLLIDING FROM RIGHT");

			//position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 8;
			if (velocity.y == 0 && velocity.x != 0)
				velocity.y -= 11;
		}
	}
	if (c1->type == COLLIDER_ENEMY_SENSOR && c2->type == COLLIDER_PLAYER) {
			current_animation = &attackleft;
			if (current_animation->GetCurrentFrame().x == 200 && App->scene->PlayerPt->die != true) {
				LOG("DIE MOTHERFUCKER!");
				if(!(App->scene->PlayerPt->godmode_activated))
					App->scene->PlayerPt->die = true;
				
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
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Colliding Left (going right)
				//	LOG("COLLIDING FROM LEFT");
			//velocity.y = 0;
			position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 8;
			
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - ((velocity.x*-1) - velocity.x)) { //Colliding Right (going left)
				//LOG("COLLIDING FROM RIGHT");
			//velocity.y = 0;
			position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 8;
			
		}
	}
	if (c2->type == COLLIDER_PLAYER) {
		//App->scene->PlayerPt.die
	}
}

void EnemyTribale::Move(const p2DynArray<iPoint>&path, float dt) {

	fromright = false;
	fromleft = false;
	//const p2DynArray<iPoint>* Path = path;
	int ydistance = App->scene->PlayerPt->position.y - position.y;
	if (App->scene->PlayerPt->position.y - position.y*-1 > 0) {
		ydistance *= -1;
	}
	if (ydistance < 100) {
		curr_direction = NewMovement(&path);
		switch (curr_direction) {

		case UP_RIGHT:
			position.x += velocity.x*(dt / 30);
			//position.y -= velocity.y*(dt / 30);
			break;
		case UP_LEFT:
			position.x -= velocity.x*(dt / 30);
			//position.y -= velocity.y*(dt / 30);
			break;
		case DOWN_RIGHT:
			position.x += velocity.x*(dt / 30);
		//	position.y += velocity.y*(dt / 30);
			break;
		case DOWN_LEFT:
			position.x -= velocity.x*(dt / 30);
		//	position.y += velocity.y*(dt / 30);
			break;
		case RIGHT:
			//LOG("MOVING RIGHT");
			position.x += velocity.x*(dt / 30);
			current_animation = &move_left;
			fromleft = false;
			fromright = true;
			break;
		case LEFT:
			//LOG("MOVING LEFT");
			position.x -= velocity.x*(dt / 30);
			current_animation = &move_left;
			fromleft = true;
			fromright = false;
			break;
		case UP:
			position.y -= velocity.y*(dt / 30);
			break;
		case DOWN:
			position.y += velocity.y*(dt / 30);
			break;
		case NO_DIRECTION:
			//LOG("NOT MOVING");
			break;
			//current_animation = &idle;
		}
	}
	//else
	//	Path->Clear();
}
bool EnemyTribale::DetectThePlayer() {
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
	else if(player.x-Position.x > 3)
		flip = SDL_FLIP_HORIZONTAL;
	
	if (velocity.y != 0) {
		current_animation = &jump;
	}	
		return true;
	    
}
EntityDirection EnemyTribale::NewMovement(const p2DynArray<iPoint>*EntityPath) {
	int themagicnumber;
	if (flip == SDL_FLIP_HORIZONTAL) {
		themagicnumber = 5;
	}
	else
		themagicnumber = 1;
	if (EntityPath->Count() >= 3) {
		if (EntityPath->At(i + themagicnumber) != nullptr)
		{
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
				//return DOWN_RIGHT;
				return RIGHT;

			}
			else if (Direction_comp.x == -1 && Direction_comp.y == 1) {
				//return DOWN_LEFT;
				return LEFT;
			}
			else if (Direction_comp.x == 1 && Direction_comp.y == -1) {
				//return UP_RIGHT;
				return RIGHT;
			}
			else if (Direction_comp.x == 1 && Direction_comp.y == -1) {
				//return UP_LEFT;
				return LEFT;
			}
			else if (Direction_comp.x == 1) {
				return RIGHT;

			}
			else if (Direction_comp.x == -1) {
				return LEFT;

			}
			else if (Direction_comp.y == 1) {
				//return DOWN;
				return NO_DIRECTION;

			}
			else if (Direction_comp.y == -1) {
				//return UP;
				return NO_DIRECTION;

			}


		}
		else return NO_DIRECTION; // SI NO ENCAJA DENTO DE "i+2"
	}
	

	

}