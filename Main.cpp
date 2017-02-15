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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // ���������� ������� ���� (������� ���������)
HINSTANCE hInst; // ������������� ����������				 
LPCSTR AppName = "BattleSea";// ��������� �� ����������� ������ �������� - ��� ��������� � ������ ����



// ����� ����� � ��������� - ������� WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd; // ���������� ������������� ���� (handle)
	MSG msg; // ���������� ��������� ���� MSG, ��� ������ � �����������
	hInst = hInstance; // ��������� ������������� ����������	   
	WNDCLASS wc;// ��������� ��������� WNDCLASS

	// �������������� ���������� ��� ��������� ������ ������
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = AppName;

	RegisterClass(&wc); // ������� � ������������ ������� �����

	// ������� ���� ���������
	hWnd = CreateWindow(
		AppName, // ��� ������ ����
		AppName, // ��������� ����
		WS_OVERLAPPEDWINDOW, // ����� ����
		CW_USEDEFAULT, 0, // �������������� � ������������ ������� ����
		300, 300, // ������ � ������ ����
		NULL, // ����� ������������� ����
		NULL, // ����� ����
		hInst, // ������������� ����������
		NULL); // �������������� ������ ����

	ShowWindow(hWnd, SW_SHOW); // ���������� ����
	UpdateWindow(hWnd); // �������������� ����

	// ����������� ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// ������� ���������
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