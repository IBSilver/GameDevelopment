#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneLogo : public Module
{
public:
	//Constructor
	SceneLogo();

	//Destructor
	~SceneLogo();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;

	int timer;
	bool scape;

};

#endif
