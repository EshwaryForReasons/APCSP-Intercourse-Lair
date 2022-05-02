#pragma once

#include <vector>

#include <SDL.h>

class ILWorld
{
public:

	ILWorld();
	~ILWorld();

	/**Returns list of all objects in the game*/
	std::vector<class ILObject*> GetGlobalEntities() {return global_entities;};

	/**Returns list of all objects in the game*/
	std::vector<class ILWall*> GetGlobalWalls() {return global_walls;};


	/**Called every frame, responsible for rendering all the world objects*/
	virtual void RenderWorld(float delta_time);

	/**Send input to player*/
	virtual void HandleInput(SDL_Event& input_event);


	/**Progress level*/
	void ProgressLevel();


	/**Spawns an entity of the given characteristics*/
	template<typename T, typename... Args>
	void SpawnEntity(T t, Args... args);

	/**Adds an entity to the list*/
	void AddEntity(class ILObject* object);

	/**Removes an entity from the list*/
	void RemoveEntity(class ILObject* object);

	/**Adds an entity to the list*/
	void AddWall(class ILWall* object);

	/**Removes an entity from the list*/
	void RemoveWall(class ILWall* object);


	/**Checks whether the given rect intersects with any other in the world and returns all*/
	std::vector<class ILObject*> CheckMultipleGlobalCollision(const SDL_Rect* a, bool b_is_enemy);

	/**Checks whether the given rect intersects with any other in the world*/
	class ILWall* CheckGlobalWallCollision(const SDL_Rect* a);


	/**The player*/
	class ILPlayer* player;

	/**The current level*/
	uint8_t current_level;

protected:

	/**List of all objects in the game*/
	std::vector<class ILObject*> global_entities;

	/**List of all walls in the game*/
	std::vector<class ILWall*> global_walls;
};