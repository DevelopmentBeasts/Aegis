#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class j1Map;


class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Called when we want the change the level
	void LoadLevel(const char* leveltoload);

private:
	
	const char* level1="AEGIS_RUN.tmx";	//Level 1
	const char* level2="hello2.tmx";			//Level 2
	
	//Level that is loaded at the moment
	p2SString current_level;

public:

	////Pointer to the current map
	j1Map* current_map=nullptr;

	float playerinitx;
	float playerinity;
	float screeninitx;
	float screeninity;
};

#endif // __j1SCENE_H__