#pragma once

#include <SDL_ttf.h>

#include "SDLUtilities/Texture.h"

struct Resources
{
public:

	/**Loads media*/
	static bool LoadMedia();

	/**Frees all media*/
	static void FreeMedia();


	/**The font to use throughout the game*/
	inline static TTF_Font* global_font_title;

	/**The font to use throughout the game*/
	inline static TTF_Font* global_font_normal;

	/**The font to use throughout the game*/
	inline static TTF_Font* global_font_small;


	/**The texture of the background*/
	inline static ILTexture background_texture;

	/**The texture of the player*/
	inline static ILTexture player_texture;

	/**The texture of the player*/
	inline static ILTexture death_texture;


	/**The texture of the bricks*/
	inline static ILTexture ground_texture;
};