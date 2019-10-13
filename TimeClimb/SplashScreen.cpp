#include "stdafx.h"
#include "SplashScreen.h"


void SplashScreen::Show(sf::RenderWindow & renderWindow)
{
	sf::Image image;
	if (image.loadFromFile("images/maxresdefault.jpg") != true)
	{
		return;
	}

	sf::Texture logotexture;//������� ������ Texture (��������)
	logotexture.loadFromImage(image);//�������� � ���� ������ Image (�����������)

	sf::Sprite logosprite;//������� ������ Sprite(������)
	logosprite.setTexture(logotexture);//������� � ���� ������ Texture (��������)


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