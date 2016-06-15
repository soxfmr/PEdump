#include <stdio.h>
#include "Colorama.h"

Colorama::Colorama()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	this->m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if (INVALID_HANDLE_VALUE == NULL)
	{
		printf("[-] WARNING: Cannot open the standard output handle.");
		return;
	}

	if (! GetConsoleScreenBufferInfo(this->m_hConsole, &csbi))
	{
		return;
	}

	this->m_DefaultAttr = csbi.wAttributes;
}


Colorama::~Colorama()
{
	if (this->m_hConsole != NULL && this->m_hConsole != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hConsole);
	}
}

void Colorama::SetColor(WORD color)
{
	if (this->m_hConsole == NULL)
		return;

	SetConsoleTextAttribute(this->m_hConsole, color);
}

void Colorama::Reset()
{
	if (this->m_hConsole == NULL)
		return;

	SetConsoleTextAttribute(m_hConsole, this->m_DefaultAttr);
}

void Colorama::Output(WORD color, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	
	this->SetColor(color);
	
	vprintf(format, args);

	this->Reset();

	va_end(args);
}

