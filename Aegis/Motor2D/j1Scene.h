#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

#include "j1Module.h"
#include "Enemy.h"

struct SDL_Texture;
class j1Map;


enum class FadeStep {
	fade_none,
	fade_to_black,
	fade_from_black
};

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
	void LoadLevel(p2SString &level_to_load);

	void ButtonAction(UiButton* button);

private:
	
	//Usually does nothing, unless we call the function FadeToBlack()
	//Draws the fade 
	void UpdateFade();

	//Call this function in order to start a fade to load a level
	void FadeToBlack(p2SString &leveltoload);


public:

	p2SString	intro  ;
	p2SString	level1 ;		//Level 1
	p2SString	level2 ;		//Level 2

	////Pointer to the current map
	j1Map* current_map=nullptr;

	float playerinitx;
	float playerinity;

	float screeninitx;
	float screeninity;

	pugi::xml_document doc;
public:
	bool LoadLevel1NOW = false;
	bool LoadLevel2NOW = false;
	bool SceneLoaded = false;
	bool PlayerExists = false;
	bool findplayer = false;
public:

	//Pointer to the player entity
	j1Entity * PlayerPt;
	j1Entity * EnemyWorm1;
	j1Entity * EnemyTribale1;
	j1Entity * MagicRock1;
private:

	//Fade
	uint fade_time;				//Time for each phase of the fade
	uint fade_start_time;		//Time at which we start the fade
	SDL_Rect fade_rect;			//Square
	FadeStep fade_step;			//Fading vs unfading
	p2SString* level_to_load;	//Level we want to load

	//Level that is loaded at the moment
	p2SString* current_level = nullptr;

};


#endif // __j1SCENE_H__