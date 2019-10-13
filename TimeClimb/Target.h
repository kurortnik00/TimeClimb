#pragma once

#include "VisibleGameObject.h"

class Target :
	public VisibleGameObject
{
public:
	Target();
	~Target();

	void Update(sf::Event& event);
	bool getState();
	bool getClickState();
	void setUnbreakable(bool ans);
	bool getUnbreakable();
	void setClickState(bool ans);
	void reInit();


private:
	bool isActive;
	bool hasClicked;
	bool Unbreakable;		//Невозможность клика пока предыдущие не кликнуты
	float spriteX, spriteY, w, h;
	void animation();
	int animationFrame;
	sf::Clock animationClock;
	bool animationStart;
	float animationTime;
	float dist2(sf::Vector2f const & p1, sf::Vector2f const & p2);
	
};
