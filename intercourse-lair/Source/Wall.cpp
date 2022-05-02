
#include "Wall.h"
#include "SDLUtilities/Texture.h"

ILWall::ILWall(int x, int y, int w, int h, ILTexture* _object_texture)
{
	object_texture = _object_texture;

	location.x = x;
	location.y = y;
	object_width = w;
	object_height = h;

	simple_collider.x = x;
	simple_collider.y = y;
	simple_collider.w = w;
	simple_collider.h = h;
}

void ILWall::Render()
{
	//Show the dot
	object_texture->Render(location.x, location.y, object_width, object_height);
}