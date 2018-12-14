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
     	i = 0;
	    move = true;
    }

	if(path!=nullptr)
	App->pathfinding->DrawPath(path);


	if (move) 
	{
		/*PosToGo.x = path->At(i)->x;
		PosToGo.y = path->At(i)->y;*/



		i++;
	}

	//if (move) {
		//iPoint EnemyPos = position;

		//EnemyPos = App->map->WorldToMap(position.x, position.y);
		//PosToGo.x = path->At(iTile)->x;
		//PosToGo.y = path->At(iTile)->y;

		//states = wheretogo(*path);

		//for (int i = 0; i < states.Count()-1; i++) {
		//	LOG("%i", i);
		//}
		////NextPosToGo = App->map->MapToWorld(path->At(iTile + 1)->x, path->At(iTile + 1)->y);

	//}


	if (App->framerate_cap_activated) {
		dt = 30;
	}
	if (Gravity) {
		velocity.y += GravityValue * (dt / 30);
		position.y += velocity.y*(dt / 30);
	}

	TribaleCollider->SetPos(position.x+65, position.y);
	
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
//p2DynArray<State> EnemyTribale::wheretogo(const p2DynArray<iPoint>& path) {
//	p2DynArray<State> StatesArray;
//	for (int i = 0; i < path.Count(); i++) {
//
//		iPoint Tile;
//		Tile.x = path.At(i)->x;
//		Tile.y = path.At(i)->y;
//		iPoint NextTile;
//		NextTile.x = path.At(i+1)->x;
//		NextTile.y = path.At(i+1)->y;
//
//		int translationX = NextTile.x - Tile.x;
//		int translationY = NextTile.y - Tile.y;
//
//		if (translationX == 1 && translationY == 1) {
//			StatesArray[i] = RIGHT_DOWN;
//		}
//		else if (translationX == 1 && translationY == -1) {
//			StatesArray[i] = RIGHT_UP;
//		}
//		else if (translationX == -1 && translationY == 1) {
//			StatesArray[i] = LEFT_DOWN;
//		}
//		else if (translationX == -1 && translationY == -1) {
//			StatesArray[i] = LEFT_UP;
//		}
//		else if (translationX == 1) {
//			StatesArray[i] = RIGHT;
//		}
//		else if (translationX == -1) {
//			StatesArray.PushBack(LEFT);
//		}
//		else if (translationX == 1) {
//			StatesArray[i] = DOWN;
//		}
//		else if (translationX == -1) {
//			StatesArray[i] = UP;
//		}
//	}
//
//	return  StatesArray;
//}
