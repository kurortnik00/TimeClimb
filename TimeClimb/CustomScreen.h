#pragma once
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "GameObjectManager.h"
#include "stdafx.h"
#include "windows.h"
#include "Target.h"
class CustomScreen
{
public:
	CustomScreen();
	~CustomScreen();

	sf::Vector2f Show(sf::RenderWindow& window, GameObjectManager& gameObjectManager);

private:
	//sf::Vector2f HandleClick(int x, int y);
	sf::Sprite  _sprite;
	sf::Texture _texture;
	sf::Image _image;

};
