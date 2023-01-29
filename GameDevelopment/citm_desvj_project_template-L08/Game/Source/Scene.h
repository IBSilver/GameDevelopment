#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include "EnemyFlying.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	bool Logo = true;
	bool Title = false;
	int timer = 0;
	bool transition = false;

	bool limitFPS = true;
public:

	// Declare the entities attribute
	Player* player;
	Enemy* enemy;
	EnemyFlying* enemyFlying;

private:
	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	iPoint originF;
	bool originSelected = false;
	bool showPath = false;

	iPoint pos;
};

#endif // __SCENE_H__