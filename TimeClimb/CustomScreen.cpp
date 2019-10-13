#include "CustomScreen.h"

CustomScreen::CustomScreen(){

}
CustomScreen::~CustomScreen() {

}

sf::Vector2f CustomScreen::Show(sf::RenderWindow& window, GameObjectManager& gameObjectManager)				//Проверка куда кликнули мышью
{
	

	_image.loadFromFile("images/target2.png");
	_image.createMaskFromColor(sf::Color(255, 255, 255));
	
	_texture.loadFromImage(_image);
	sf::Sprite sprite;
	_sprite.setTexture(_texture);
	float spriteX = 0;
	float spriteY = 768;
	float w = 256;
	float h = 256;
	_sprite.setTextureRect(sf::IntRect(spriteX, spriteY, w, h));

	sf::Event placeTarget;
	for (int i = 0; i < 3; i++) {
		int k = 0;
		
		VisibleGameObject* obj = gameObjectManager.Get(std::to_string(i));
		while (k == 0)
		{
			window.clear(sf::Color(0, 0, 0));
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
			_sprite.setPosition(mousePos);
			window.draw(_sprite);
			gameObjectManager.DrawAll(window);
			window.display();

			
			window.pollEvent(placeTarget);
			if (placeTarget.type == sf::Event::MouseButtonPressed)
			{
				obj->SetPosition(placeTarget.mouseButton.x, placeTarget.mouseButton.y);
				k = 1;
				Sleep(200);
				obj->setPlased(true);
			}
		}
		

	}
	
	sf::Vector2f vec1(0, 0);

	return vec1;
}


