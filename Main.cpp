#include "Main.h"
#include <Windows.h>

Main::Main()
{
}

Main::~Main()
{
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, "���������� ���������!", "WinAPI App", 0); //���������
	
	WM_QUIT;
}