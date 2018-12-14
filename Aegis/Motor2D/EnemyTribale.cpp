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
	velocity.x = 1;
	velocity.y = 14;
	Gravity = 0;
	return true;
}

bool EnemyTribale::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && (position.x - App->scene->PlayerPt->position.x < 700))
	{
	    App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(App->scene->PlayerPt->position.x, App->scene->PlayerPt->position.y));
	    path = App->pathfinding->GetLastPath();
     	i = 2;
	    move = !move;
    }

	if(path!=nullptr)
	App->pathfinding->DrawPath(path);

	if (move) 
	{		
		//Move();
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

void EnemyTribale::Move() {

	if (path->At(i + 3) != nullptr) {
		if (path->At(i) != nullptr) {
			PosToGo.x = path->At(i+2)->x;
			PosToGo.y = path->At(i+2)->y;
		}

		PosToGo = App->map->MapToWorld(PosToGo.x, PosToGo.y);
		//position = App->map->WorldToMap(position.x, position.y);
		velocity.x = 3;
		if (velocity.x*(-1) > 0) {
			velocity.x *= -1;
		}
		velocity.y = 3;
		if (velocity.y*(-1) > 0) {
			velocity.y *= -1;
		}
		xstate;
		//LEFT
		if (PosToGo.x < position.x && !rightdone) { //si tienes que ir izquierda y no vienes de la derecha
			xstate = LEFT;
			LOG("X STATE LEFT");
		}
		if (xstate == LEFT && !leftdone) {//muevete izquierda si asi tienes que hacerlo
			position.x -= velocity.x;
		}
		if (PosToGo.x > position.x && xstate == LEFT) {
			leftdone = true;
		}
		//DOWN
		if (PosToGo.y > position.y && !updone) {//if have to go down and dont come from bot to tp
			ystate = DOWN;
			LOG("Y STATE DOWN");
		}
		if (ystate == DOWN && !downdone) {
			position.y += velocity.y;
		}
		if (PosToGo.y < position.y && ystate == DOWN) {
			downdone = true;
		}
		//RIGHT
		if (PosToGo.x > position.x && !leftdone) {//have to go right and not coming from left
			xstate = RIGHT;
			LOG("X STATE RIGHT");
		}
		if (xstate == RIGHT && !rightdone) {
			position.x += velocity.x;
		}
		if (PosToGo.x < position.x && xstate == RIGHT) {
			rightdone = true;
		}
		//UP
		if (PosToGo.y < position.y && !downdone) {//if have to go up and not down done
			ystate = UP;
			LOG("Y STATE UP");
		}
		if (ystate == UP && !updone) {
			position.y -= velocity.y;
		}
		if (PosToGo.y > position.y && ystate == UP) {
			updone = true;
		}

		if (!(PosToGo.x < position.x && !rightdone) && !(PosToGo.y > position.y && !updone) && !(PosToGo.x > position.x && !leftdone) && !(PosToGo.y < position.y && !downdone)) {
			LOG("NEXT TILE IN THE PATH");
			i++;
			leftdone = false;
			rightdone = false;
			updone = false;
			downdone = false;
		}
	}else
		move = false;
}