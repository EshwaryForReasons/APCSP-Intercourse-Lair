
#pragma once

#include <SDL.h>

#include "ILPhysics.h"

class ILWall
{
public:

	ILWall(int x, int y, int w, int h, class ILTexture* object_texture);

	/**Renders the object on the screen*/
	void Render();


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

	Vector location;

	/**The object's simple collider*/
	SDL_Rect simple_collider;

	/**The texture of the object*/
	class ILTexture* object_texture;
};

