#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "j1App.h"

#define CURSOR_WIDTH 2

enum class ButtonFunction
{
	LOAD_LEVEL1,
	NONE
};

enum class UiType {

	LABEL,
	IMAGE,
	BUTTON,
	CHECKBOX,
	INTEXT,
	WINDOW,
	NONE
};

enum class ButtonSize {
	BIG,
	SMALL,
	NONE
};

class UiElement
{
public:

	//Constructor
	UiElement(iPoint position);

	//Print the element on the screen
	virtual void Draw() {}

	//CleanUp
	virtual void CleanUp() {}

public:

	//Elemenent to which it this is nested
	UiElement* parent = nullptr;

	//Position of the element
	iPoint position;

	//Type of ui element
	UiType type;

	//Indicates if an element is visible and we can interact with it
	bool active;
};

class UiImage : public UiElement
{
public:

	//Constructor
	UiImage(iPoint position, SDL_Rect section);

	void Draw();

	void CleanUp();



private:

	//Atlas texture
	SDL_Texture* atlas;

	//Section we want to draw from the atlas
	SDL_Rect section;

};

class UiLabel : public UiElement
{
public:

	//Constructors
	UiLabel(iPoint position, char* label, _TTF_Font* font = App->fonts->default);

	void CleanUp();

	void Draw();

private:

	//Text we want to print
	p2SString text;

	//Font
	_TTF_Font* font;

};

class UiActiveElement : public UiElement
{
public:

	UiActiveElement(iPoint position);

	virtual void Start() {}

	virtual void Update() {}

	void Draw() {}

	virtual void CleanUp() {}

	virtual void Act() {}

	bool MouseOnTop() const;

protected:

	//Atlas
	SDL_Texture* atlas;

	//Sections to draw
	SDL_Rect section_idle;
	SDL_Rect section_selected;

	//Pointer to the section we want to print
	SDL_Rect* current_section = nullptr;

	//Area in which the mouse can activate the element
	SDL_Rect action_area;

};

class UiButton : public UiActiveElement
{
public:

	//Constructor
	UiButton(iPoint position, ButtonSize size, j1Module* callback);

	void Start();
	void Update();

	void Act();

	//Draw
	void Draw();

	UiLabel* NestLabel(iPoint label_position, char* text);
	UiImage* NestImage(iPoint image_position, SDL_Rect section);

public:

	//Callback to make an action when the button is pressed
	j1Module* callback = nullptr;

	//Ui label or Ui image, depending on what
	//   !!!CANT ADD AN ACTIVE ELEMENT OR A WINDOW!!!!
	UiElement* son_element=nullptr;
};

class UiCheckBox : public UiActiveElement
{
public:

	//Constructor
	UiCheckBox(iPoint position, bool &boolean, char* label);

	void Start();
	void Update();

	//Draw
	void Draw();

	void Act();


private:

	//Label
	UiLabel* label;

	//Tick
	UiImage* tick;

	//Text to put into the label
	char* label_text;

	//Value we want to switch
	bool &bool_ptr;

};

class UiWindow:public UiElement
{

public:

	UiWindow(iPoint position);

	void CleanUp();

	void Draw();

	void NestImage(iPoint image_position, SDL_Rect section);
	void NestLabel(iPoint label_position, char* text = nullptr, _TTF_Font* font = App->fonts->default);
	UiButton* NestButton(iPoint button_position, ButtonSize size, j1Module* callback = nullptr);

public:



private:

	SDL_Rect section;

	SDL_Texture* atlas=nullptr;

	p2List<UiElement*> element_list;

};


// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UiImage*	AddImage(iPoint position, SDL_Rect section);
	UiLabel*	AddLabel(iPoint position, char* label, _TTF_Font* font = App->fonts->default);
	UiButton*	AddButton(iPoint position, ButtonSize size, j1Module* callback = nullptr);
	UiCheckBox*	AddCheckBox(iPoint position, bool * boolean, char* label);
	UiWindow*	AddWindow(iPoint position);

	//Call Draw() function of all the UiElements
	void DrawUi() const;

	SDL_Texture* GetAtlas() const;

private:

	//List with all the UI elements
	p2List<UiElement*> ui_elements;

	//List with all the elements we can interact with
	p2List<UiActiveElement*> active_elements;

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__
