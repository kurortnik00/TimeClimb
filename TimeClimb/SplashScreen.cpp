#include "stdafx.h"
#include "SplashScreen.h"


void SplashScreen::Show(sf::RenderWindow & renderWindow)
{
	sf::Image image;
	if (image.loadFromFile("images/maxresdefault.jpg") != true)
	{
		return;
	}

	sf::Texture logotexture;//создаем объект Texture (текстура)
	logotexture.loadFromImage(image);//передаем в него объект Image (изображения)

	sf::Sprite logosprite;//создаем объект Sprite(спрайт)
	logosprite.setTexture(logotexture);//передаём в него объект Texture (текстуры)


	renderWindow.draw(logosprite);
	renderWindow.display();

	sf::Event event;
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed)
			{
				return;
			}
		}
	}
}