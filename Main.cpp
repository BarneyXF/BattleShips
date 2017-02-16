#include "Main.h"
#include <Windows.h>
// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

Main::Main()
{
}

Main::~Main()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Объявление функции окна (оконной процедуры)
HINSTANCE hInst; // Идентификатор приложения				 
LPCSTR AppName = "BattleSea";// Указатель на константную строку символов - имя программы и класса окна



// Точка входа в программу - функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd; // Уникальный идентификатор окна (handle)
	MSG msg; // Объявление структуры типа MSG, для работы с сообщениями
	hInst = hInstance; // Сохраняем идентификатор приложения	   
	WNDCLASS wc;// Заполняем структуру WNDCLASS

	// Инициализируем выделенную для структуры память нулями
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = AppName;

	RegisterClass(&wc); // Создаем и регистрируем оконный класс

	// Создаем окно программы
	hWnd = CreateWindow(
		AppName, // Имя класса окна
		AppName, // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		CW_USEDEFAULT, 0, // Горизонтальная и вертикальная позиции окна
		300, 300, // Ширина и высота окна
		NULL, // Хендл родительского окна
		NULL, // Хендл меню
		hInst, // Идентификатор приложения
		NULL); // Дополнительные данные окна

	ShowWindow(hWnd, SW_SHOW); // Отображаем окно
	UpdateWindow(hWnd); // Перерисовываем окно

	// Стандартный цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}