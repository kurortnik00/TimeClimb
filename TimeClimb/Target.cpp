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

	tracking_Type tP = mainPointAvarage;
	//for shapes need change functions in Update

	//_kinectControl set true or false in Game Init
	if (_kinectControl) {

		int joint_Count = 0;

		switch (tP)
		{
		case Target::allJoints:
			joint_Count = JointType_Count;
			kinectUpdateActions(joint_Count, tP);
			break;
		case Target::mainPointAvarage:
			joint_Count = 4;
			kinectUpdateActions(joint_Count, tP);
			break;
		case Target::allJointsTimeAvarage:
			joint_Count = JointType_Count;
			kinectUpdateActions(joint_Count, tP);
		case Target::mainPointTimeAvarage:
			joint_Count = 4;
			kinectUpdateActions(joint_Count, tP);
		default:
			break;
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

void Target::Draw(sf::RenderWindow & window)
{
	sf::CircleShape _shape1;
	float _radius = 30;
	_shape1.setFillColor(sf::Color(0, 0, 0));
	_shape1.setRadius(_radius);
	_shape1.setOutlineThickness(10);
	_shape1.setOutlineColor(sf::Color(250, 50, 100));
	float x = kinectTranform_X_Cordinates(kinectApplication.arms_legs_pointAveraged_PointsXY(CBodyBasics::RIGHT_ARM).x);
	float y = kinectTranform_Y_Cordinates(kinectApplication.arms_legs_pointAveraged_PointsXY(CBodyBasics::RIGHT_ARM).y);
	_shape1.setPosition(sf::Vector2f(x, y));

	sf::CircleShape _shape2;
	_shape2.setFillColor(sf::Color(0, 0, 0));
	_shape2.setRadius(_radius);
	_shape2.setOutlineThickness(10);
	_shape2.setOutlineColor(sf::Color(250, 150, 100));
	x = kinectTranform_X_Cordinates(kinectApplication.arms_legs_pointAveraged_PointsXY(CBodyBasics::LEFT_ARM).x);
	y = kinectTranform_Y_Cordinates(kinectApplication.arms_legs_pointAveraged_PointsXY(CBodyBasics::LEFT_ARM).y);
	_shape2.setPosition(sf::Vector2f(x, y));


	window.draw(_shape1);
	window.draw(_shape2);

	std::cout << kinectApplication.arms_legs_timeAveraged_DepthPoints(CBodyBasics::LEFT_ARM) << "   " << kinectApplication.arms_legs_timeAveraged_DepthPoints(CBodyBasics::RIGHT_ARM) << "\n";
	//std::cout << kinectApplication.DepthSkeletonPoints(HANDLEFT) << "   " << kinectApplication.DepthSkeletonPoints(HANDRIGHT) << "\n";

	VisibleGameObject::Draw(window);

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
	animationStart = false;
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


void Target::kinectUpdateActions(int joint_Count, tracking_Type tP)
{
	for (int i = 0; i < joint_Count; i++) {

		switch (tP)
		{
		case Target::allJoints:
			joint_xy = sf::Vector2f(kinectApplication.SkeletPointsXY(i).x, kinectApplication.SkeletPointsXY(i).y);
			joint_z = kinectApplication.DepthSkeletonPoints(i);
			break;
		case Target::mainPointAvarage:
			joint_xy = kinectApplication.arms_legs_pointAveraged_PointsXY(i);
			joint_z = kinectApplication.arms_legs_pointAveraged_DepthPoints(i);
			break;
		case Target::allJointsTimeAvarage:
			joint_xy = kinectApplication.allJoints_timeAveraged_PointsXY(i);
			joint_z = kinectApplication.allJoints_timeAveraged_DepthPoints(i);
			break;
		case Target::mainPointTimeAvarage:
			joint_xy = kinectApplication.arms_legs_timeAveraged_PointsXY(i);
			joint_z = kinectApplication.arms_legs_timeAveraged_DepthPoints(i);
		default:
			break;
		}


		joint_xy.x = kinectTranform_X_Cordinates(joint_xy.x); //translate to pixel
		joint_xy.y = kinectTranform_Y_Cordinates(joint_xy.y); //same



		if (joint_z >= _trashHold) {
			if (animationClock.getElapsedTime().asMilliseconds() > 100) {						//need instad (event.type == sf::Event::MouseButtonPressed) to avoid mass click to target
				if ((dist2(VisibleGameObject::getCenter(), joint_xy) < 8100))
				{
					if (!hasClicked) {
						hasClicked = true;
						animationStart = true;
					}
				}
			}
		}
	}
}

void Target::setKinectControl(bool kinectCOontrol) {

	_kinectControl = kinectCOontrol;
}

float Target::kinectTranform_X_Cordinates(float x)
{
	return ((1920 - x * 1920 / 640) - 510)*4.9 / 2.4;
}

float Target::kinectTranform_Y_Cordinates(float y)
{
	return (y * 1200 / 280 - 430) * 4 / 1.4;
}

