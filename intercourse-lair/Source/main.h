
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>

#include <SDL.h>

#ifdef _DEBUG
#ifndef SI_LOG
static std::ofstream myfile;
#define SI_LOG(LogInfo) myfile.open("log.txt", std::fstream::app); myfile << LogInfo << std::endl; myfile.close();
#endif

static void InitializeLogFile()
{
	time_t t = time(NULL);
	struct tm tPtr;
	localtime_s(&tPtr, &t);

	std::ostringstream new_log_file_name;
	new_log_file_name << "log_backup_" << tPtr.tm_mday << "." << (tPtr.tm_mon + 1) << "." << (tPtr.tm_year + 1900) << "_" << tPtr.tm_hour << "." << tPtr.tm_min << "." << tPtr.tm_sec << ".txt";

	std::string rename_command("rename log.txt \"" + new_log_file_name.str() + "\"");
	std::string move_command("move \"" + new_log_file_name.str() + "\" .\\BackupLogs");

	//Move old log file
	system(rename_command.c_str());
	system(move_command.c_str());

	//Create new log file
	system("type nul > log.txt");
}

#elif NDEBUG
#ifndef SI_LOG
#define SI_LOG(x)
#endif
#endif

void InsertNewError(std::string Error);

template<class T>
const T& clamp(const T& x, const T& lower, const T& upper)
{
	return std::min(upper, std::max(x, lower));
}

static SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color col = { r,g,b,a };
	return col;
}


static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 600;

class Main
{
public:

	Main();
	~Main();

	static Main* GetInstance();
	static class ILWorld* GetWorld() {return current_world;};


	/**The ticker object for main, responsible for all game logic*/
	void MainTick();


	/**Changes the game mode*/
	void StartGame();


	/**The SDL window we will be rendering to*/
	SDL_Window* global_window;

	/**The window renderer*/
	SDL_Renderer* global_renderer;


	/**Should the game keep running*/
	bool quit = false;

	/**Has the game started*/
	bool b_has_started;


	/**Starts up SDL and created window*/
	bool InitializeSDL();

	/**Frees media and shuts down SDL*/
	void Close();

private:

	/**This is the world that will be opened*/
	inline static class ILWorld* current_world;
};