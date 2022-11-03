#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	idleR.totalFrames = 0;
	right.totalFrames = 0;
	jump.totalFrames = 0;

	//initilize textures
	texture = app->tex->Load(texturePath);

	//idleR Anim
	for (int i = 0; i < 3; i++) {
		idleR.PushBack({ 0 + (i * 48), 336, 48, 48 });
	}
	idleR.loop = true;
	idleR.speed = 0.5f;

	//right Anim
	for (int i = 0; i < 5; i++) {
		right.PushBack({ 0 + (i * 48), 480, 48, 48 });
	}
	right.loop = true;
	right.speed = 0.5f;

	//jump Anim
	for (int i = 0; i < 3; i++) {
		jump.PushBack({ 0 + (i * 48), 384, 48, 48 });
	}
	jump.loop = true;
	jump.speed = 0.5f;

	currentAnimation = &idleR;

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);
	return true;
}

bool Player::Update()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	// L07 DONE 5: Add physics to the player - updated player position using physics

	int speed = 10; 
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);


	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speed, -GRAVITY_Y);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);
		right.Reset();
		currentAnimation = &right;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		vel = b2Vec2(0, GRAVITY_Y);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speed, GRAVITY_Y);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speed, GRAVITY_Y);
			jump.Reset();
			currentAnimation = &jump;
		}

	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	SDL_Rect textSection = { 0,0,48,48 };
	app->render->DrawTexture(texture, position.x , position.y, &textSection);

	return true;
}

bool Player::CleanUp()
{

	return true;
}