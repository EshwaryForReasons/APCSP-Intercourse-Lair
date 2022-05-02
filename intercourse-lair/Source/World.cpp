
#include "World.h"
#include "main.h"
#include "ILObject.h"
#include "Player.h"
#include "Resources.h"
#include "Wall.h"
#include "Enemies/ILEnemyBase.h"


ILWorld::ILWorld()
{
	current_level = 1;
}

ILWorld::~ILWorld()
{
	for (ILObject* x : global_entities)
	{
		RemoveEntity(x);
	}

	for (ILWall* x : global_walls)
	{
		RemoveWall(x);
	}
}

void ILWorld::RenderWorld(float delta_time)
{
	for (ILObject* x : global_entities)
	{
		if (x)
		{
			x->Render(delta_time);
		}
	}

	for (ILWall* x : global_walls)
	{
		x->Render();
	}
}

void ILWorld::HandleInput(SDL_Event& input_event)
{
	if (player)
	{
		player->HandleInput(input_event);
	}

	if ((!player || player->b_is_dead) && input_event.key.keysym.sym == SDLK_SPACE)
	{
		Main::GetInstance()->StartGame();
	}
}

template<typename T, typename... Args>
void ILWorld::SpawnEntity(T t, Args... args)
{
	T* new_entity = new T(args);
	AddEntity(new_entity);
}

void ILWorld::AddEntity(class ILObject* object)
{
	global_entities.push_back(object);
}

void ILWorld::RemoveEntity(class ILObject* object)
{
	if (object)
	{
		global_entities.erase(std::remove(global_entities.begin(), global_entities.end(), object), global_entities.end());
	}
}

void ILWorld::AddWall(class ILWall* wall)
{
	global_walls.push_back(wall);
}

void ILWorld::RemoveWall(class ILWall* wall)
{
	global_walls.erase(std::remove(global_walls.begin(), global_walls.end(), wall), global_walls.end());
}


void ILWorld::ProgressLevel()
{
	current_level++;

	for (int i = 0; i < current_level; i++)
	{
		AddEntity(new ILEnemyBase(clamp(10 + (50 * i), 10, SCREEN_WIDTH - 50), 200, current_level, &Resources::death_texture));
	}
}


std::vector<ILObject*> ILWorld::CheckMultipleGlobalCollision(const SDL_Rect* a, bool b_is_enemy)
{
	std::vector<ILObject*> final = {};

	if (!b_is_enemy)
	{
		for (int i = 0; i < global_entities.size(); i++)
		{
			ILObject* other = global_entities[i];

			if (a != other->GetSimpleCollider() && SDL_HasIntersection(a, other->GetSimpleCollider()))
			{
				final.push_back(other);
			}
		}
	}
	else
	{
		for (int i = 0; i < global_entities.size(); i++)
		{
			ILObject* other = global_entities[i];

			if (a != other->GetSimpleCollider() && SDL_HasIntersection(a, other->GetSimpleCollider()) && other->GetSimpleCollider() == player->GetSimpleCollider())
			{
				final.push_back(other);

				break;
			}
		}
	}
	

	return final;
}

ILWall* ILWorld::CheckGlobalWallCollision(const SDL_Rect* a)
{
	ILWall* final = nullptr;

	for (int i = 0; i < global_walls.size(); i++)
	{
		ILWall* other = global_walls[i];

		if (other && a != other->GetSimpleCollider() && SDL_HasIntersection(a, other->GetSimpleCollider()))
		{
			final = other;

			break;
		}
	}

	return final;
}