//Original form lazyfoo.net. File has been modified extensively.

#pragma once

#include <string>
#include <unordered_map>

#include <SDL.h>

typedef std::unordered_map<const char*, class ILAnimation*> AnimationList;

enum class EFontSize
{
	FS_TITLE,
	FS_NORMAL,
	FS_SMALL
};

struct ILAnimation
{
	/**The total number of frames*/
	uint8_t total_frames;

	/**The total number of frames*/
	uint8_t current_frame;

	/**The width of each frame*/
	uint16_t frame_width;

	/**The position of this animation vertically*/
	uint8_t animation_index;

	/**The frame rate to animate at, normalized to 60*/
	float animation_duration;

	/**The current frame number (out of total number)*/
	float frame_count;


	SDL_Rect GetRenderClip()
	{

	}

	ILAnimation() {};
	ILAnimation(uint8_t _total_frames, uint8_t _animation_index, float _animation_duration)
		: total_frames(_total_frames)
		, animation_index(_animation_index)
		, animation_duration(_animation_duration)
		, frame_count(0.0f)
		, current_frame(1)
	{}
};

class ILTexture
{
public:
	//Initializes variables
	ILTexture();

	ILTexture(int frames, float animation_duration);

	ILTexture(AnimationList animation_list, float scale = 1.0f);

	//Deallocates memory
	~ILTexture();


	/**Loads image at specified path*/
	bool LoadFromFile(const char* path);

	/**Creates image from font string*/
	bool LoadFromRenderedText(std::string textureText, SDL_Color textColor, const EFontSize& font_size);

	/**Deallocates texture*/
	void Free();


	/**Set color modulation*/
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);

	/**Set blending*/
	void SetBlendMode(SDL_BlendMode blending);

	/**Set alpha modulation*/
	void SetAlpha(Uint8 alpha);


	/**Renders texture at given point*/
	void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	/**Renders texture at given point*/
	void Render(float delta_time, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE);

	/**Renders to a specific width and height*/
	void Render(int x, int y, int w, int h);


	/**Set the animation to play*/
	void SetAnimation(const char* animation_name, bool* b_is_animating = nullptr);


	/**Returns the image width*/
	int GetWidth() {return width;};

	/**Returns the width of every frame (better for collision)*/
	int GetFrameWidth() {return animations[current_animation]->frame_width;}

	/**Returns the image height*/
	int GetHeight() {return height;};

	/**Returns the height of every frame (better for collision)*/
	int GetFrameHeight() {return frame_height;}

private:

	//The actual hardware texture
	SDL_Texture* main_texture;

	/**The part of the image to render (for sprite sheets)*/
	SDL_Rect render_rect;


	/**Did the current animation finish*/
	bool* b_is_currently_animating;


	/**The height of each frame*/
	uint16_t frame_height;

	/**Is this a sprite sheet*/
	bool b_is_animated;

	/**The total number of animations*/
	uint8_t total_animations;

	/**The current animation name*/
	const char* current_animation;

	/**Map of animation name to animation*/
	AnimationList animations;


	/**Texxture scaling amount*/
	float scale;


	//Image dimensions
	int width;
	int height;
};

