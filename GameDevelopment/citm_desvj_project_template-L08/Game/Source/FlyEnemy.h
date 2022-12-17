#ifndef __FLY_ENEMY_H__
#define __FLY_ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class FlyEnemy : public Entity
{
public:

	FlyEnemy();

	virtual ~FlyEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// Add physics to the fly enemy - declare a Physics body
	PhysBody* pbody;

public:

	//Enemy destroyed variable
	bool destroyed = false;

	//Enemy animations
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation move;
	Animation death;

	// Save last fly enemy direction (left or right)
	//bool dir;

	// Fly enemy variables
	//bool dead = false;

	// Declare sounds parameters
	//int deathFx;

	// Define OnCollision function for thefly enemy. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

private:

	// Declare fly enemy parameters
	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __PLAYER_H__
