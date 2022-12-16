#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();

	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	//Money picked variable
	bool isPicked = false;

	//Money sound variable
	int pickCoinFxId;

	//Money animation
	Animation* currentAnimation = nullptr;
	Animation money;

	//Money Collision function
	void Item::OnCollision(PhysBody* physA, PhysBody* physB);

private:

	//Define money parameters
	SDL_Texture* texture;
	const char* texturePath;

	// Add a physics to an item
	PhysBody* pbody;
};

#endif // __ITEM_H__