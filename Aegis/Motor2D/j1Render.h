#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, SDL_RendererFlip flip=SDL_FLIP_NONE, int pivot_x = INT_MAX, int pivot_y = INT_MAX,float EstraScale = 1) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	
	//Instantly moves the camera to the player position
	void CenterCamera();	
	
	//Allows the camera to move freely
	void FreeMovement();

	//Moves the camera depending on where the player goes
	void FollowPlayer();

	//Moves the camera to the player position in a short period of time
	//Works like FollowPlayer() but faster
	void FindPlayer();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Returns true if something is in the camera range
	bool InScreen(int x,int y,uint w, uint h, float parallax_speed) const;

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;


	//We make this true in order to use the function FindPlayer()
	bool find_player=false;

private:

	//Borders to move the camera depending on the player position
	int top_border;
	int bot_border;
	int left_border;
	int right_border;

	bool debug=false;	//In debug mode we will be able to move the camera as we want
};

#endif // __j1RENDER_H__