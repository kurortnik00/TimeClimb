#pragma once
#pragma once
#include "stdafx.h"

class MyKeyboard : public sf::Keyboard
{
public:
	MyKeyboard();
	~MyKeyboard();

	static char getChar();

private:

};
