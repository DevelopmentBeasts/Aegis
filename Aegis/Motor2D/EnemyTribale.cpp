#include "EnemyTribale.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "Player.h"
#include "p2Defs.h"
#include "EntityManager.h"
#include "j1Scene.h"

EnemyTribale::EnemyTribale(iPoint pos) : j1Enemy(pos, ENEMY_TYPE::TRIBALE) {

	properties_node = App->j1entity_manager->properties_doc.child("properties").child("Tribale");

	idle.LoadPushbacks(properties_node.child("idle"));
	move_left.LoadPushbacks(properties_node.child("move_left"));
	move_right.LoadPushbacks(properties_node.child("move_right"));
	attack.LoadPushbacks(properties_node.child("attack"));


	TribaleRect.x = pos.x;
	TribaleRect.y = pos.y;
	TribaleRect.w = 50;
	TribaleRect.h = 87;
}

EnemyTribale::~EnemyTribale() {}

bool EnemyTribale::Start() {
	TribaleCollider = App->collision->AddEntCollider(TribaleRect, COLLIDER_ENEMY, this);
	position.x = 3000;
	position.y = 700;

	texture = App->j1entity_manager->tribale_texture;
	current_animation = &idle;
	
	return true;
}

bool EnemyTribale::Update(float dt) {

	

	if (App->framerate_cap_activated) {
		dt = 30;
	}
	if (Gravity) {
		velocity.y += GravityValue * (dt / 30);
		position.y += velocity.y*(dt / 30);
	}
	int detectdistance = 200;

	

	switch (state)
	{
	case IDL:
		//LOG("TRIBALE IDL");
		velocity.x = 0;
		if (position.x - App->scene->PlayerPt->position.x < detectdistance && position.x - App->scene->PlayerPt->position.x > 20 ) {  // SIEMPRE IRA A LA IZQUIERDA=???? NIO LO SE, MIRAR
			state = TribaleStates::RUN_LEFT;
		}
		if (position.x - App->scene->PlayerPt->position.x > (detectdistance*-1) && position.x - App->scene->PlayerPt->position.x < -20) {
			state = TribaleStates::RUN_RIGHT;
		}
		break;
	case RUN_LEFT:
		current_animation = &move_left;
		flip = SDL_FLIP_NONE;
		velocity.x = -8;
		/*LOG("TRIBALE RUN   <----------");*/
		if (position.x - App->scene->PlayerPt->position.x > (detectdistance*-1) && position.x - App->scene->PlayerPt->position.x < -20) {
			state = TribaleStates::IDL;
			flip = SDL_FLIP_NONE;
		}	
		break;
	case RUN_RIGHT:
		/*LOG("TRIBALE RUN    ---------->");*/
		current_animation = &move_right;
		velocity.x = 8;
		if (position.x - App->scene->PlayerPt->position.x < detectdistance && position.x - App->scene->PlayerPt->position.x > 20) {  // SIEMPRE IRA A LA IZQUIERDA=???? NIO LO SE, MIRAR
			state = TribaleStates::IDL;
		}
		
		break;
	default:
		break;
	}
	position.x += velocity.x/**(dt/30)*/;
	TribaleCollider->SetPos(position.x+40, position.y);
	
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
		App->scene->PlayerPt->Die();
	}
}
