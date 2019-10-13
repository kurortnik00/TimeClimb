#include "stdafx.h"
#include "Game.h"
#include "MainMenu.h"
#include "SplashScreen.h"
#include "Target.h"
#include "Timer.h"
#include "CustomScreen.h"


void Game::Start(void)	//инициализация объектов
{
	if (_gameState != Uninitialized) return;

	_mainWindow.create(sf::VideoMode(1200, 800), "Pang!");

	Game::Init(targetCount);


	_gameState = Game::Custom;		//Начинаем с заставки

	while (!IsExiting())
	{
		GameLoop();
	}

	Game::_mainWindow.close();
}

bool Game::IsExiting()//для остановки GameLoop()
{
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

sf::RenderWindow& Game::GetWindow()
{
	return Game::_mainWindow;
}

void Game::GameLoop()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	
	switch (_gameState)
	{
		
		case Game::ShowingMenu:
		{
			ShowMenu();		
			break;
		}
		case Game::ShowingSplash:
		{
		
			ShowSplashScreen();
			break;
		}
		case Game::Custom:
		{

			ShowCustomScreen();
			break;
		}
		case Game::Playing:
		{
		
			
			_mainWindow.clear(sf::Color(0, 0, 0));
			
			_gameObjectManager.setAllPlased();
			
			_gameObjectManager.UpdateAll(currentEvent);		//Обновляет все объекты
			_gameObjectManager.DrawAll(_mainWindow);		//Рисует все объекты


			if ((_gameObjectManager.Get(std::to_string(targetCount - 1))->getClickState()) && !(_gameObjectManager.Get("timer1")->getFinished())) {
				_gameObjectManager.Get("timer1")->setFinished(true);
				for (int i = 0; i < targetCount; i++) {
					_gameObjectManager.Get(std::to_string(i))->setFinished(true);
				}

			}

			_mainWindow.display();

			if (currentEvent.type == sf::Event::Closed) _gameState = Game::Exiting;

			if (currentEvent.type == sf::Event::KeyPressed)
			{
				if (currentEvent.key.code == sf::Keyboard::Escape)		//Выход из сцены игры в меню по клавише ESC
				{
					//_gameObjectManager.RemoveAll();
					_gameState = Game::ShowingMenu;
				}
			}
			break;
		}
	}
}

void Game::ShowSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);					//Внутри бесконечный цикл прерываемый по нажатию любой клавиши
													
	_gameState = Game::ShowingMenu;
}

void Game::ShowCustomScreen()
{
	CustomScreen customScreen;
	customScreen.Show(_mainWindow, _gameObjectManager);					//Внутри бесконечный цикл прерываемый по нажатию любой клавиши

	_gameState = Game::ShowingMenu;
}

void Game::ShowMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(_mainWindow);   //Возврацает значение нажатой "кнопки", т.е. что делать дальше: Играть или выйти из игры
																//Внутри бесконечный цикл прерываемый по нажатию одной из "кнопок" или закрытию окна
																	

	switch (result)
	{
		case MainMenu::Exit:
			_gameState = Game::Exiting;
			break;
		case MainMenu::Play:
			_gameState = Game::Playing;
			Game::reInit(targetCount);
			break;
	}
}

void Game::Init(int targ_count) {

	for (int i = 0; i < targ_count; i++) {
		Target *tar = new Target();
		tar->Load("images/target2.png");
		tar->SetPosition(200 + 200*i, 500);
		_gameObjectManager.Add(std::to_string(i), tar);
	}

	Timer *time1 = new Timer();
	time1->Load("font/11583.ttf");
	_gameObjectManager.Add("timer1", time1);
}

void Game::reInit(int targ_count)
{
	for (int i = 0; i < targ_count; i++) {
		_gameObjectManager.Get(std::to_string(i))->reInit();
	}
	_gameObjectManager.Get("timer1")->reInit();
}


GameObjectManager Game::_gameObjectManager;
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
int Game::targetCount = 3;




