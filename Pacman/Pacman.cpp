#include "Pacman.h"
#include <iostream>
#include <sstream>

using namespace std;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanFrameTime(250), _cPacmanSpeed(0.1f)
{
	_pacmanStruct = new player();
	//_Menus = new Menu();
	_pacmanStruct->_frameCount = 0;
	_Menus->_paused = false;
	_Menus->_mainMenuPause = true;
	_pacmanStruct->_pacmanCurrentFrameTime = 0;
	_pacmanStruct->_pacmanFrame = 0;
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanStruct->_pacmanTexture;
	delete _pacmanStruct->_pacmanSourceRect;
	delete _pacmanStruct->_munchieBlueTexture;
	delete _pacmanStruct->_munchieInvertedTexture;
	delete _pacmanStruct->_munchieRect;
	delete _pacmanStruct;
	delete _Menus;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanStruct->_pacmanTexture = new Texture2D();
	_pacmanStruct->_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanStruct->_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanStruct->_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	_pacmanStruct->_offset = 10;
	// Load Munchie
	_pacmanStruct->_munchieBlueTexture = new Texture2D();
	_pacmanStruct->_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_pacmanStruct->_munchieInvertedTexture = new Texture2D();
	_pacmanStruct->_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_pacmanStruct->_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	//Load Menu
	_Menus->_menuBackground = new Texture2D();
	_Menus->_menuBackground->Load("Textures/Transparency.png", false);
	_Menus->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_Menus->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	
	//Loads Main Menu
	_Menus->_mainMenuBackground = new Texture2D();
	_Menus->_mainMenuBackground->Load("Textures/Transparency.png", false);
	_Menus->_mainMenuRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_Menus->_mainMenuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Set string position
	_pacmanStruct->_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	//makes sure the start menu runs and dismisses it when it space is pressed
	if (_Menus->_started == true)
	{
		if (keyboardState->IsKeyDown(Input::Keys::SPACE) && _Menus->_spKeyDown == true)

			_Menus->_started = true;
		_Menus->_pKeyDown = true;
		_Menus->_mainMenuPause = false;
	}
	else
	{
		checkPaused(keyboardState, Input::Keys::P, elapsedTime);
	}

}
//processes the movement of pacman
void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	_pacmanStruct->_frameCount++;
	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_pacmanStruct->_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacmanStruct->_pacmanDirection = 0;
		_pacmanStruct->_pacmanCurrentFrameTime += elapsedTime;
		_pacmanStruct->_pacmanSourceRect->X = _pacmanStruct->_pacmanSourceRect->Width * _pacmanStruct->_pacmanFrame; // creates animations
		_pacmanStruct->_pacmanSourceRect->Y = _pacmanStruct->_pacmanSourceRect->Height * _pacmanStruct->_pacmanDirection; //Changes the Pacman's Sprite Direction
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_pacmanStruct->_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacmanStruct->_pacmanDirection = 2;
		_pacmanStruct->_pacmanSourceRect->X = _pacmanStruct->_pacmanSourceRect->Width * _pacmanStruct->_pacmanFrame; //makes animations run
		_pacmanStruct->_pacmanSourceRect->Y = _pacmanStruct->_pacmanSourceRect->Height * _pacmanStruct->_pacmanDirection; //Changes the Pacman's Sprite Direction
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacmanStruct->_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
		_pacmanStruct->_pacmanDirection = 1;
		_pacmanStruct->_pacmanSourceRect->Y = _pacmanStruct->_pacmanSourceRect->Height * _pacmanStruct->_pacmanDirection; //Changes the Pacman's Sprite Direction
		_pacmanStruct->_pacmanSourceRect->X = _pacmanStruct->_pacmanSourceRect->Width * _pacmanStruct->_pacmanFrame;
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_pacmanStruct->_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacmanStruct->_pacmanDirection = 3;
		_pacmanStruct->_pacmanSourceRect->Y = _pacmanStruct->_pacmanSourceRect->Height * _pacmanStruct->_pacmanDirection; //Changes the Pacman's Sprite Direction
		_pacmanStruct->_pacmanSourceRect->X = _pacmanStruct->_pacmanSourceRect->Width * _pacmanStruct->_pacmanFrame;
	}
	//sees if pacman is trying to escape the world
	if (_pacmanStruct->_pacmanPosition->X + _pacmanStruct->_pacmanSourceRect->Width > Graphics::GetViewportWidth()) //uses openGl to define the width
	{
		//keeps pacman inside the world along the X axis by wrapping him
		_pacmanStruct->_pacmanPosition = new Vector2(0, _pacmanStruct->_pacmanPosition->Y);
	}
	//sees if pacman is trying to escape the world
	if (_pacmanStruct->_pacmanPosition->X < 0 - _pacmanStruct->_offset)
	{
		//keeps pacman inside the world along the X axis by wrapping him
		_pacmanStruct->_pacmanPosition = new Vector2(1024, _pacmanStruct->_pacmanPosition->Y);
	}
	//                    32                                     768                        10
	if (_pacmanStruct->_pacmanPosition->Y + _pacmanStruct->_pacmanSourceRect->Height > Graphics::GetViewportHeight() + _pacmanStruct->_offset) //this uses a pointer so always use -> when refering to Y
	{
		//keeps pacman inside the world along the Y axis by wrapping him
		_pacmanStruct->_pacmanPosition = new Vector2(_pacmanStruct->_pacmanPosition->X, 0);
	}
	if (_pacmanStruct->_pacmanPosition->Y < 0 - _pacmanStruct->_offset) //this uses a pointer so always use -> when refering to Y
	{
		//keeps pacman inside the world along the Y axis by wrapping him
		_pacmanStruct->_pacmanPosition = new Vector2(_pacmanStruct->_pacmanPosition->X, Graphics::GetViewportHeight() - _pacmanStruct->_offset);
	}
	_pacmanStruct->_pacmanCurrentFrameTime += elapsedTime;

	//draws pacmans next frame 
	if (_pacmanStruct->_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanStruct->_pacmanFrame++;
		if (_pacmanStruct->_pacmanFrame >= 2)
		{
			_pacmanStruct->_pacmanFrame = 0;
		}
		_pacmanStruct->_pacmanCurrentFrameTime = 0;
	}
	
}

void Pacman::checkPaused(Input::KeyboardState* state, Input::Keys pauseKey, int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	//makes sure the start menu runs and dismisses it when it space is pressed

	if (_Menus->_mainMenuPause)
		if (keyboardState->IsKeyDown(Input::Keys::P) && _Menus->_pKeyDown == true)
		{
			_pacmanStruct->_pacmanDirection = 0;
			_Menus->_pKeyDown = true;
			_Menus->_paused = _Menus->_paused == true;

		}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		if (_Menus->_paused == true)
		{
			Input(elapsedTime, keyboardState);
			updateMunchie(elapsedTime);
			checkViewportCollision();
			updatePacman(elapsedTime);
		}

}

void Pacman::updatePacman(int elapsedTime)

{

	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanStruct->_pacmanPosition->X << " Y: " << _pacmanStruct->_pacmanPosition->Y;
	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanStruct->_pacmanTexture, _pacmanStruct->_pacmanPosition, _pacmanStruct->_pacmanSourceRect); // Draws Pacman

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _pacmanStruct->_stringPosition, Color::Green);

	//Draws Main Menu
		// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _pacmanStruct->_stringPosition, Color::Green);
}
void Pacman::updateMunchie(int elapsedTime)
{
	if (_pacmanStruct->_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_pacmanStruct->_munchieInvertedTexture, _pacmanStruct->_munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_pacmanStruct->_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_pacmanStruct->_munchieBlueTexture, _pacmanStruct->_munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_pacmanStruct->_frameCount++;

		if (_pacmanStruct->_frameCount >= 60)
			_pacmanStruct->_frameCount = 0;
	}
}

void Pacman::checkViewportCollision()
{
	return;
}

void Pacman::Draw(int elapsedTime)
{
	if (_Menus->_mainMenuPause)
	{
		stringstream mainMenuStream;
		mainMenuStream << "Press space to play the game!";
		SpriteBatch::Draw(_Menus->_mainMenuBackground, _Menus->_mainMenuRect, nullptr);
		SpriteBatch::DrawString(mainMenuStream.str().c_str(), _Menus->_mainMenuStringPosition, Color::Red);
	}



	//Draws Menu
	if (_Menus->_paused)
	{
		stringstream menuStream;
		menuStream << "Paused!";
		SpriteBatch::Draw(_Menus->_menuBackground, _Menus->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _Menus->_menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}
