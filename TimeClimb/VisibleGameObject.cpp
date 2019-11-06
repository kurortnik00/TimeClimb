#include "StdAfx.h"
#include "VisibleGameObject.h"

//Класс всех видимых объектов
VisibleGameObject::VisibleGameObject()
{
	_isLoaded = false;
	plased = false;
	finished = false;
}


VisibleGameObject::~VisibleGameObject()
{
}

void VisibleGameObject::Load(std::string filename)		//Простейшая загрузка спрайтов, загружается вся картинка целиком
{
	if (_image.loadFromFile(filename) == false)
	{
		_filename = "";
		_isLoaded = false;
	}
	else
	{
		_filename = filename;

		_image.createMaskFromColor(sf::Color(255, 255, 255));
		_texture.loadFromImage(_image);


		sf::Sprite sprite;
		_sprite.setTexture(_texture);

		_isLoaded = true;
	}
}

void VisibleGameObject::Draw(sf::RenderWindow & renderWindow)
{
	if (_isLoaded && !finished)
	{
		renderWindow.draw(_sprite);
	}
}

void VisibleGameObject::SetPosition(float x, float y)
{
	if (_isLoaded)
	{
		_sprite.setPosition(x, y);
	}
}

void VisibleGameObject::Update(sf::Event& event)
{

}


bool VisibleGameObject::getClickState()		
{
	return false;
}

sf::Sprite& VisibleGameObject::GetSprite()
{
	return _sprite;
}

sf::Vector2f VisibleGameObject::GetPosition() const
{
	if (_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}	

bool VisibleGameObject::IsLoaded() const
{
	return _isLoaded;
}

float VisibleGameObject::GetHeight() const
{
	return _sprite.getTexture()->getSize().y;
}

float VisibleGameObject::GetWidth() const
{
	return _sprite.getTexture()->getSize().x;
}

sf::Rect<float> VisibleGameObject::GetBoundingRect() const		//Прямоугольник для просчитывания взаиможействия
{
	sf::Vector2u size = _sprite.getTexture()->getSize();
	sf::Vector2f position = _sprite.getPosition();

	return sf::Rect<float>(
		position.x,
		position.y,
		size.x,
		size.y
		);
}

void VisibleGameObject::Restart() {				//Для обновления игры, переопределяется в дочерних классах

}

void VisibleGameObject::setUnbreakable(bool ans)
{

}

bool VisibleGameObject::getUnbreakable()
{
	return true;
}

void VisibleGameObject::setFinished(bool res)
{
	finished = res;
}

bool VisibleGameObject::getFinished() {
	return false;
}

void VisibleGameObject::setPlased(bool res)
{
	plased = res;
}

bool VisibleGameObject::getPlased() {
	return plased;
}

void VisibleGameObject::setClickState(bool ans)
{

}
void VisibleGameObject::reInit() {

}

sf::Vector2f VisibleGameObject::getCenter() {
	sf::IntRect rect = _sprite.getTextureRect();
	return sf::Vector2f(_sprite.getPosition().x + rect.width / 2, _sprite.getPosition().y + rect.height / 2);
}

float VisibleGameObject::getFinishedTime()
{
	return -1;
}