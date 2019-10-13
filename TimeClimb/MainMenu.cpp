#include "stdafx.h"
#include "MainMenu.h"


MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window)
{

	//Load menu image from file
	sf::Image image;
	image.loadFromFile("images/menu.jpg");
	sf::Texture texture;//создаем объект Texture (текстура)
	texture.loadFromImage(image);//передаем в него объект Image (изображения)

	sf::Sprite sprite;//создаем объект Sprite(спрайт)
	sprite.setTexture(texture);//передаём в него объект Texture (текстуры)

	//Setup clickable regions

	//Play menu item coordinates
	MenuItem playButton;
	playButton.rect.top = 145;
	playButton.rect.width = 1023;
	playButton.rect.left = 0;
	playButton.rect.height = 200;
	playButton.action = Play;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 0;
	exitButton.rect.width = 1023;
	exitButton.rect.top = 345;
	exitButton.rect.height = 200;
	exitButton.action = Exit;

	_menuItems.push_back(playButton);
	_menuItems.push_back(exitButton);

	window.clear(sf::Color(0, 0, 0));
	window.draw(sprite);
	window.display();


	return GetMenuResponse(window);
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y)				//Проверка куда кликнули мышью
{
	std::list<MenuItem>::iterator it;

	for (it = _menuItems.begin(); it != _menuItems.end(); it++)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if (menuItemRect.height + menuItemRect.top > y
			&& menuItemRect.top < y
			&& menuItemRect.left < x
			&& menuItemRect.width + menuItemRect.left> x)
		{
			return (*it).action;										//Возврацает номер "сцены" (состояния)
		}
	}

	return Nothing;
}

MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window)		
{
	sf::Event menuEvent;

	while (true)
	{

		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return HandleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}
			if (menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}