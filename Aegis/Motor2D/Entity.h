#ifndef ENTITY_H___
#define ENTITY_H___
#include "PugiXml/src/pugixml.hpp"

enum EntityType {
	NONVALID,
	PLAYER,
    ENEMY
};

class Entity {
public:
	Entity(EntityType Type):Etype(Type) {}
	~Entity(){}

	EntityType Etype;

public:
	virtual void Start(){}
	virtual void CleanUp(){}
	virtual void Awake(){}
	virtual void Update(float dt){}
	virtual void Draw(){}

public:
	/*virtual bool Load(pugi::xml_node& node){}
	virtual bool Save(pugi::xml_node& node){}*/
};

#endif