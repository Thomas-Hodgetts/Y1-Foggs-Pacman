#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;


struct player
{
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;
	int _pacmanDirection;
	// Data to represent Pacman Animation
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	// Position for String
	Vector2* _stringPosition;
	int _offset;
};

//a stucture that holds all of the Menu data
struct Menu
{
	//Data For the Pause menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;

	//Data For the Main menu
	Texture2D* _mainMenuBackground;
	Rect* _mainMenuRect;
	Vector2* _mainMenuStringPosition;
	bool _mainMenuPause;
	bool _spKeyDown;
	bool _started;
};

player* _pacmanStruct;
Menu* _Menus;
// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	//input methods
	void Input(int elapsedTime, Input::KeyboardState* state);

	//check methods
	void checkPaused(Input::KeyboardState* state, Input::Keys pauseKey, int elapsedTime);
	void checkViewportCollision();

	//Update methods
	void updatePacman(int elapsedTime);
	void updateMunchie(int elaspsedTime);
	player* _pacman1;

	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	
public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};