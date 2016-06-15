#pragma once
#include "Windows.h"

#define FORE_DARK_BLUE 1
#define FORE_DARK_GREEN 2
#define FORE_DARK_RED 4
#define FORE_LIGHT_BLUE 9

class Colorama
{
public:
	Colorama();
	~Colorama();

	void SetColor(WORD color);
	void Reset();

	void Output(WORD color, const char* format, ...);
private:
	HANDLE m_hConsole;
	WORD m_DefaultAttr;
};

