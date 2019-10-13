#include "StdAfx.h"
#include "Target.h"
#include "Game.h"

//Класс для примера, объекты класса спрайты которые уничтожаются по нажатию стрелки "влево"

Target::Target()
{
	Load("images/target2.png");
	assert(IsLoaded());
	isActive = true;
	hasClicked = false;
	Unbreakable = true;		//Невозможность клика пока предыдущие не кликнуты
	spriteX = 0;
	spriteY = 0;
	w = 256;
	h = 256;
	VisibleGameObject::GetSprite().setTextureRect(sf::IntRect(spriteX, spriteY, w, h));


	animationFrame = 0;
	animationStart = false;
	animationClock.restart();
	animationTime = 0;

}

Target::~Target()
{
}

bool Target::getState()
{
	return isActive;
}

bool Target::getClickState()
{
	return hasClicked;
}



void Target::setUnbreakable(bool ans)	
{
	Unbreakable = ans;
}

bool Target::getUnbreakable()
{
	return Unbreakable;
}

void Target::setClickState(bool ans)
{
	hasClicked = ans;
}

void Target::Update(sf::Event& event) {

	if (dist2(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), sf::Vector2f(VisibleGameObject::getCenter()) ) < 75*75
		&& Unbreakable == false)
	{
		if (!hasClicked) {
			hasClicked = true;
			animationStart = true;
		}
	}
	animation();
}

void Target::reInit() {
	isActive = true;
	hasClicked = false;
	Unbreakable = true;		//Невозможность клика пока предыдущие не кликнуты
	spriteX = 0;
	spriteY = 0;
	w = 256;
	h = 256;
	VisibleGameObject::GetSprite().setTextureRect(sf::IntRect(spriteX, spriteY, w, h));
	VisibleGameObject::setFinished(false);
}


void Target::animation() {
	if (!animationStart) {
		return;
	}
	if (animationFrame == 0)
	{
		spriteX = 0;
		spriteY = 768;
	}
	animationTime = animationClock.getElapsedTime().asMilliseconds();
	if (animationTime > 30) {				//animation speed
		spriteX = spriteX + 256;
		animationClock.restart();
		animationFrame++;
	}

	if (animationFrame == 4) {
		spriteX = 0;
		spriteY = 1024;
	}
	if (animationFrame == 7) {

		animationFrame = 0;
		animationStart = false;

	}
	VisibleGameObject::GetSprite().setTextureRect(sf::IntRect(spriteX, spriteY, w, h));


}

float Target::dist2(sf::Vector2f const & p1, sf::Vector2f const & p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}