
#pragma once

#include <SDL.h>

#include "ILPhysics.h"
#include "SDLUtilities/Texture.h"

class ILObject
{
public:

	ILObject(int x, int y, class ILTexture* object_texture);
	~ILObject();

	/**Called every frame*/
	virtual void Tick(float delta_time);

	/**Renders the object on the screen*/
	void Render(float delta_time);


	/**Take damage*/
	virtual void TakeDamage(float Damage);

	/**The death logic*/
	virtual void Die();


	/**Moves the player, use this because it handles moving the render location, coordinate location, and checks collision*/
	bool MoveObject(float delta_time);


	/**The objects movement information*/
	ObjectMovementInformation object_movement_information;


	/**Returns the simple collision box*/
	SDL_Rect* GetSimpleCollider() {return &simple_collider;};

	/**Returns the objects width*/
	int GetObjectWidth() const {return object_width;}
	int GetObjectWidth() {return object_width;}

	/**Returns the objects height*/
	int GetObjectHeight() const {return object_height;}
	int GetObjectHeight() {return object_height;}

protected:

	int object_width = 20;
	int object_height = 20;

	/**Object's mass, used for physics*/
	int object_mass = 50;


	/**Did the current animation finish*/
	bool* b_did_animation_finish;


	/**Saved to avoid a default flip*/
	SDL_RendererFlip flip;


	/**The object's simple collider*/
	SDL_Rect simple_collider;

	/**The object's landed collider (has larger height to see if object is resting on surface)*/
	SDL_Rect landed_collider;

	/**The texture of the object*/
	class ILTexture object_texture;


	/**Make sure collision is working*/
	void RecalculateCollision();
};

