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

	_kinectControl = false;
	_trashHold = 2;
	kinectApplication.Run();

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


	//_kinectControl set true or false in Game Init
	if (_kinectControl) {
		for (int i = 0; i < JointType_Count; i++) {

			joint_xy = sf::Vector2f(kinectApplication.SkeletPointsXY(i).x, kinectApplication.SkeletPointsXY(i).y);
			joint_z = kinectApplication.DepthSkeletonPoints(i);

			/*HANDRIGHT_xy = sf::Vector2f((kinectApplication.SkeletPointsXY(HANDRIGHT).x + kinectApplication.SkeletPointsXY(WRISTRIGHT).x + kinectApplication.SkeletPointsXY(HANDTIPRIGHT).x + kinectApplication.SkeletPointsXY(THUMBRIGHT).x) / 4,
				(kinectApplication.SkeletPointsXY(HANDRIGHT).y + kinectApplication.SkeletPointsXY(WRISTRIGHT).y + kinectApplication.SkeletPointsXY(HANDTIPRIGHT).y + kinectApplication.SkeletPointsXY(THUMBRIGHT).y) / 4);
			HANDRIGHT_z = (kinectApplication.DepthSkeletonPoints(HANDRIGHT) + kinectApplication.DepthSkeletonPoints(WRISTRIGHT) + kinectApplication.DepthSkeletonPoints(HANDTIPRIGHT) + kinectApplication.DepthSkeletonPoints(THUMBRIGHT) + kinectApplication.DepthSkeletonPoints(ELBOWRIGHT)) / 5;
*/

			//HANDRIGHT_xy.x = HANDRIGHT_xy.x * 1900 / 640 * 1 / 1; //translate to pixel
			//HANDRIGHT_xy.y = HANDRIGHT_xy.y * 1080 / 280 * 1 / 1;//same

			joint_xy.x = joint_xy.x * 1900 / 640 * 1 / 1; //translate to pixel
			joint_xy.y = joint_xy.y * 1080 / 280 * 1 / 1;//same


			if (joint_z >= _trashHold) {
				if (animationClock.getElapsedTime().asMilliseconds() > 100) {						//need instad (event.type == sf::Event::MouseButtonPressed) to avoid mass click to target
					if ((dist2(VisibleGameObject::getCenter(), joint_xy) < 6400))
						if (!hasClicked) {
							hasClicked = true;
							animationStart = true;
						}
				}
			}
		}
	}


	else {


		if (dist2(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), sf::Vector2f(VisibleGameObject::getCenter())) < 75 * 75
			&& Unbreakable == false)
		{
			if (!hasClicked) {
				hasClicked = true;
				animationStart = true;
			}
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