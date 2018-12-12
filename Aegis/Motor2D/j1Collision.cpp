#include "j1Collision.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "Entity.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;
	matrix[COLLIDER_NONE][COLLIDER_WALL] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_ENEMY] = false;
	//matrix[COLLIDER_NONE][COLLIDER_SENSOR] = false;

	matrix[COLLIDER_WALL][COLLIDER_WALL]=false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_NONE] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	//matrix[COLLIDER_WALL][COLLIDER_SENSOR] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	//matrix[COLLIDER_PLAYER][COLLIDER_SENSOR] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_NONE] = false;
	//matrix[COLLIDER_ENEMY][COLLIDER_SENSOR] = false;

	matrix[COLLIDER_SENSOR][COLLIDER_SENSOR] = false;
	matrix[COLLIDER_SENSOR][COLLIDER_WALL] = true;
	matrix[COLLIDER_SENSOR][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_SENSOR][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_SENSOR][COLLIDER_NONE] = false;
	matrix[COLLIDER_SENSOR][COLLIDER_WIN] = false;

	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_SENSOR] = false;
	matrix[COLLIDER_WIN][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_SENSOR][COLLIDER_DEATH] = false;
}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

				if (matrix[c1->type][c2->type] && c1->entity_callback)
					c1->entity_callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->entity_callback)
					c2->entity_callback->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	
	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{

	if (App->input->GetKey(SDL_SCANCODE_F9) == j1KeyState::KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // blue
			App->render->DrawQuad(colliders[i]->rect, 255, 30, 12, alpha);
			break;
		case COLLIDER_ENEMY: // pink
			App->render->DrawQuad(colliders[i]->rect, 200, 0, 200, alpha);
			break;
		case COLLIDER_SENSOR: //yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: //Red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}
Collider* j1Collision::AddEntCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}




bool Collider::CheckCollision(const SDL_Rect& r) const
{
	
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (rect.x + rect.w < r.x || r.x + r.w < rect.x || rect.y + rect.h < r.y || r.y + r.h < rect.y)
		{
			return false;
		}
	}
	return true;
}