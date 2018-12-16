#ifndef __P2ANIMATION_H__
#define __P2ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "Brofiler/Brofiler.h"
#define MAX_FRAMES 100

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}


	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished()
	{
		if (loops > 0) {
			loops = 0;
			return true;
		}
		else
			return false;
	}

	void Reset()
	{
		current_frame = 0;
	}

 	void LoadPushbacks(pugi::xml_node node) {
		BROFILER_CATEGORY("LoadPushBacks();", Profiler::Color::Aquamarine);
		loop = node.attribute("loop").as_bool();
		speed = node.attribute("speed").as_float();
	
		SDL_Rect rect;
		for (node = node.child("PushBack"); node; node = node.next_sibling("PushBack")) {
			rect.x = node.attribute("x").as_int();
			rect.y = node.attribute("y").as_int();
			rect.w = node.attribute("w").as_int();
			rect.h = node.attribute("h").as_int();
			this->PushBack({ rect });
		}

	}

};

#endif // __P2ANIMATION_H__ 