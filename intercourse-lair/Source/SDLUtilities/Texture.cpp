//Original form lazyfoo.net. File has been modified extensively.

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

#include "Source/main.h"
#include "Texture.h"
#include "Resources.h"

ILTexture::ILTexture()
{
	main_texture = NULL;
	width = 0;
	height = 0;
	render_rect.w = width;
	render_rect.h = height;

	b_is_animated = false;
	ILAnimation* a = new ILAnimation();
	a->frame_count = 0;
	a->total_frames = 1;
	a->animation_duration = 0;
	animations.emplace("default", a);
	current_animation = "default";
	total_animations = animations.size();
}

ILTexture::ILTexture(int frames, float _animation_duration)
{
	main_texture = NULL;
	width = 0;
	height = 0;
	render_rect.w = width;
	render_rect.h = height;

	b_is_animated = true;
	ILAnimation* a = new ILAnimation();
	a->frame_count = 0;
	a->total_frames = frames;
	a->animation_duration = _animation_duration;
	animations.emplace("default", a);
	current_animation = "default";
	total_animations = animations.size();
}

ILTexture::ILTexture(AnimationList animation_list, float _scale)
{
	main_texture = NULL;
	width = 0;
	height = 0;
	render_rect.w = width;
	render_rect.h = height;
	scale = _scale;

	b_is_animated = true;
	animations = animation_list;
	current_animation = "default";
	total_animations = animation_list.size();
}

ILTexture::~ILTexture()
{
	Free();
}


bool ILTexture::LoadFromFile(const char* path)
{
	//Get rid of preexisting texture
	Free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface)
	{
		//Color key image

		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Main::GetInstance()->global_renderer, loadedSurface);

		if (newTexture)
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
			frame_height = height / total_animations;
			SetAnimation("idle");
		}
		else
		{
			SI_LOG("Uanable to create texture from " << path << ". SDL Error: " << SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	else
	{
		SI_LOG("Uanable to load image " << path << ". SDL_image Error: " << IMG_GetError());
	}

	//Return success
	main_texture = newTexture;
	return main_texture != NULL;
}

bool ILTexture::LoadFromRenderedText(std::string textureText, SDL_Color textColor, const EFontSize& font_size)
{
	//Get rid of preexisting texture
	Free();

	TTF_Font* font = nullptr;

	switch (font_size)
	{
	case EFontSize::FS_TITLE:
		font = Resources::global_font_title;
		break;
	case EFontSize::FS_NORMAL:
		font = Resources::global_font_normal;
		break;
	case EFontSize::FS_SMALL:
		font = Resources::global_font_small;
		break;
	}

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);

	if (textSurface == NULL)
	{
		SI_LOG("Unable to render text surface: " << textureText.c_str() << ". SDL_ttf Error: " << TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		main_texture = SDL_CreateTextureFromSurface(Main::GetInstance()->global_renderer, textSurface);

		if (main_texture == NULL)
		{
			SI_LOG("Uanable to create texture from rendered text: SDL Error: " << SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return main_texture != NULL;
}


void ILTexture::Free()
{
	//Free texture if it exists
	if (main_texture != NULL)
	{
		SDL_DestroyTexture(main_texture);
		main_texture = NULL;
		width = 0;
		height = 0;
	}
}

void ILTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(main_texture, red, green, blue);
}

void ILTexture::SetBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(main_texture, blending);
}

void ILTexture::SetAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(main_texture, alpha);
}

void ILTexture::SetAnimation(const char* animation_name, bool* b_is_animating)
{
	b_is_currently_animating = b_is_animating;

	if (current_animation != animation_name && animations.contains(animation_name))
	{
		current_animation = animation_name;
		animations[current_animation]->frame_width = width / animations[current_animation]->total_frames;
		render_rect.w = animations[current_animation]->frame_width;
		render_rect.h = frame_height;
		render_rect.x = 0;
		render_rect.y = animations[current_animation]->animation_index * frame_height;
	}
}

void ILTexture::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, animations[current_animation]->frame_width, frame_height};

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	if (b_is_animated)
	{
		if (animations[current_animation]->animation_duration / animations[current_animation]->total_frames == animations[current_animation]->frame_count)
		{
			render_rect.x += animations[current_animation]->frame_width;

			if (render_rect.x >= width)
				render_rect.x = 0;
		}

		if (animations[current_animation]->frame_count >= animations[current_animation]->animation_duration)
			animations[current_animation]->frame_count = 0;

		animations[current_animation]->frame_count++;
	}

	//Render to screen
	SDL_RenderCopyEx(Main::GetInstance()->global_renderer, main_texture, &render_rect, &renderQuad, angle, center, flip);
}

void ILTexture::Render(float delta_time, int x, int y, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, animations[current_animation]->frame_width, frame_height};

	if (b_is_animated)
	{
		if (animations[current_animation]->frame_count >= (animations[current_animation]->animation_duration / animations[current_animation]->total_frames) * animations[current_animation]->current_frame)
		{
			render_rect.x += animations[current_animation]->frame_width;
			animations[current_animation]->current_frame++;
		}

		if (render_rect.x >= width - animations[current_animation]->frame_width && b_is_currently_animating != nullptr)
			*b_is_currently_animating = false;

		if (animations[current_animation]->frame_count >= animations[current_animation]->animation_duration * 60 || render_rect.x >= width)
		{
			render_rect.x = 0;
			animations[current_animation]->current_frame = 1;
			animations[current_animation]->frame_count = 0;
		}

		//Increment frame, based on 60 fps
		animations[current_animation]->frame_count += delta_time;
	}

	//Render to screen
	SDL_RenderCopyEx(Main::GetInstance()->global_renderer, main_texture, &render_rect, &renderQuad, 0.0f, NULL, flip);
}

void ILTexture::Render(int x, int y, int w, int h)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, w, h};

	//Render to screen
	SDL_RenderCopyEx(Main::GetInstance()->global_renderer, main_texture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}