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
	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	
	//Set of animations
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

	//Guarda direccion personaje
	bool dir;
	//Timer salto
	int jumpTimer;
	//Variables del personaje
	bool dead = false;
	bool onair = false;
	bool winner = false;
	bool GodMode = false;
	//para calcular si el player se mueve en y
	int posInicialY;

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;


	int jumpFx;
	int pickCoinFxId;

};

#endif // __PLAYER_H__