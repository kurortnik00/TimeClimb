#include "Timer.h"
#include <sstream>

//Класс для отрисовки таймера и засекания времени

Timer::Timer() :
	text("", font, 150)
{

	gameTimeClock.restart();
	text.setStyle(sf::Text::Bold);
	text.setPosition(50, 400);
	finished = false;
	finishedTime = 0;
}

Timer::~Timer() {

}

void Timer::Load(std::string filename) {
	font.loadFromFile(filename);
}

sf::Time Timer::GetTime() {
	return gameTimeClock.getElapsedTime();
}

void Timer::Draw(sf::RenderWindow & renderWindow) {

	float gameTime = gameTimeClock.getElapsedTime().asMilliseconds() / 1000.0;

	if (finished) {
		gameTime = finishedTime;
	}

	std::ostringstream timerStr;
	timerStr << gameTime;
	text.setString(timerStr.str());
	renderWindow.draw(text);
}

void Timer::Restart() {
	gameTimeClock.restart();
}

void Timer::setFinished(bool res)
{
	finished = res;
	finishedTime = gameTimeClock.getElapsedTime().asMilliseconds() / 1000.0;
	text.setCharacterSize(250);
	text.setPosition(500, 200);
}
bool Timer::getFinished() {
	return finished;
}

void Timer::reInit() {
	text.setCharacterSize(150);
	gameTimeClock.restart();
	text.setStyle(sf::Text::Bold);
	text.setPosition(50, 400);
	finished = false;
}
	