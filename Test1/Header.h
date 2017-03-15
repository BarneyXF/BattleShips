#define _CRT_SECURE_NO_WARNINGS
#pragma once
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "Ole32.lib")

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <commdlg.h>
#include <d2d1.h>
#include <Objbase.h>


#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <string>



template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define FONT_SIZE 30.0f




class DemoApp
{
public:
	DemoApp();
	~DemoApp();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

	enum  Status_of_game_window {
		Menu = 0,
		Preparing,
		Playfield,
		Result
	};

	int width, height;

	Status_of_game_window Status_of_game_window_instanse = Menu ;
	

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	HRESULT CreateD2DBitmapFromFile(HWND);

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget*,
		IWICImagingFactory* ,
		PCWSTR ,
		ID2D1Bitmap** );

	// Release device-dependent resource.
	void DiscardDeviceResources();

	// Draw content.
	HRESULT OnRender();

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
		);

	void DrawField(int,int,int);			/*Рисую поле 10х10*/
	void DrawMenu(int, int);				/*рисую главное меню*/
	void DrawPlayField(int, int, int);		/*рисую все игровое поле*/
	void DrawPreparing(int, int, int);		/*рисую состояние установки кораблей*/


	void PrintText(wchar_t*, IDWriteTextFormat*, D2D1_RECT_F, ID2D1SolidColorBrush*);
	

	static bool HitInButton(float, float, D2D1_RECT_F);

	HRESULT CreatePlayWnd();
	  

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);


	

private:
	HWND m_hwnd, play_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pWriteTextFormat;
	IDWriteTextFormat* MenuTextFormat;
	
	D2D1_RECT_F menu_button;

	ID2D1Bitmap            *m_pD2DBitmap;
	IWICImagingFactory*		m_pIWICFactory;
	IWICFormatConverter    *m_pConvertedSourceBitmap;

};





