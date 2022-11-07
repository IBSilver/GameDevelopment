#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();
	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
public:

	Animation* currentAnimation = nullptr;

	//Set of animations
	Animation idleR;
	Animation idleL;
	Animation right;
	Animation left;
	Animation jumpR;
	Animation jumpL;

	bool dir;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __PLAYER_H__