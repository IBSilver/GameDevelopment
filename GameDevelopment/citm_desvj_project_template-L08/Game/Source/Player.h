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
	
	// Add physics to the player - declare a Physics body
	PhysBody* pbody;

public:

	Animation* currentAnimation = nullptr;
	
	// Set of animations
	Animation idleR;
	Animation idleL;
	Animation right;
	Animation left;
	Animation jumpR;
	Animation jumpL;
	Animation jumpR2;
	Animation jumpL2;
	Animation death;
	Animation win;

	// Save last player direction (left or right)
	bool dir;

	// Jump timer
	int jumpTimer;

	// Character variables
	bool dead = false;
	bool onair = false;
	bool winner = false;
	bool GodMode = false;

	// Calculate if player moves in y
	int posInicialY;

	// Desclare sounds parameters
	int jumpFx;
	int deathFx;
	int winFx;

	// Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

private:

	// Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// Declare scenes parameters
	SDL_Texture* logo;
	SDL_Texture* title;
	SDL_Texture* gameover;

};

#endif // __PLAYER_H__