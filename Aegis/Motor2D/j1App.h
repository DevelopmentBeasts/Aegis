#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "MasterTimer.h"
#include "Timer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Collision;
class j1EntityManager;
class j1EntityManeger;
class j1PathFinding;
class j1Fonts;
class j1Gui;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win			= nullptr;
	j1Input*			input		= nullptr;
	j1Render*			render		= nullptr;
	j1Textures*			tex			= nullptr;
	j1Audio*			audio		= nullptr;
	j1Scene*			scene		= nullptr;
	j1Map*				map			= nullptr;
	j1Collision*		collision	= nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			fonts		= nullptr;
	j1Gui*				gui			= nullptr;
	
	j1EntityManager*    j1entity_manager	= nullptr;

public:
	bool                framerate_cap_activated;

private:

	p2List<j1Module*>	modules;
	double              frame_count = 0;
	float               last_frame_ms;
	float               last_frame_ms_;
	uint				frames;
	uint                framerate_cap;
	
	MasterTimer			Mtimer;
	MasterTimer         Mtimerdt;
	Timer               frame_time;
	Timer               startup_time;
	Timer				last_sec_frame_time;
	float				last_sec_frame_count = 0;
	float				prev_last_sec_frame_count = 0;
	float				dt;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

public:

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif