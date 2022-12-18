#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// Add physics to the enemy - declare a Physics body
	PhysBody* pbody;

public:

	//Enemy destroyed variable
	bool destroyed = false;

	//Enemy animations
	Animation* currentAnimation = nullptr;
	Animation idleR;
	Animation idleL;
	Animation right;
	Animation left;
	Animation death;

	// Save last player direction (left or right)
	//bool dir;

	// Character variables
	//bool dead = false;

	// Desclare sounds parameters
	//int deathFx;

	// Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

private:

	// Declare enemy parameters
	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __PLAYER_H__