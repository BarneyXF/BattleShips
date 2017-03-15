#include "Header.h"

template <typename T>
inline void SafeRelease(T *&p)
{
	if (nullptr != p)
	{
		p->Release();
		p = nullptr;
	}
}

DemoApp::DemoApp() :


	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)
{
	
};

DemoApp::~DemoApp()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);

};


void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = "D2DDemoApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		//WS_OVERLAPPED|WS_CAPTION| WS_SYSMENU |WS_THICKFRAME |WS_MINIMIZEBOX 

		// Create the window.
		m_hwnd = CreateWindow(
			"D2DDemoApp",
			"Admiral Myrzabeck",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
			);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

HRESULT DemoApp::CreatePlayWnd()
{
	FLOAT dpiX, dpiY;
	HRESULT hr;

	// The factory returns the current system DPI. This is also the value it will use
	// to create its own windows.
	m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	play_hwnd = CreateWindow(
		"D2DDemoApp",
		"Admiral Myrzabeck",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
		static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
		);
	hr = play_hwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(play_hwnd, SW_SHOWNORMAL);
		UpdateWindow(play_hwnd);
	}
	return hr;
};

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(								//Создаю фабрику для рисования
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pWriteFactory)
			);
	};


	return hr;
}


HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
			);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
			);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
				);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
				);
		}

		if (SUCCEEDED(hr))
		{

			hr = m_pWriteFactory->CreateTextFormat(					// Здесь задаю шрифт и параметры для текста
				L"Comic Sans",
				NULL,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				80.0f,
				L"en-us",
				&m_pWriteTextFormat
				);
		}

		if (SUCCEEDED(hr))
		{
			/* Создаю шрифт для надписи на кнопке */
			hr = m_pWriteFactory->CreateTextFormat(
				L"Algerian",
				NULL,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				FONT_SIZE,
				L"en-us",
				&MenuTextFormat
				);
		}
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}


/*ЭТА ШУТКА ОТСЛЕЖИВАЕТ ВСЕ СОБЫТИЯ, ПРОИСХОДЯЩИЕ С ОКНОМ, КАК ТО: СОЗДАНИЕ, ЗАКРЫТИЕ, РЕСАЙЗ, НАЖАТИЕ КЛАВИШ ИТД ИТП			*/	
/*CALLBACK - эту функцию вызывает сама ОС, а мы задаем то, что хотим получить от программы*/
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
			);

		

		result = 1;
	}
	else
	{
		DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
				)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->CreateD2DBitmapFromFile(pDemoApp->m_hwnd);

				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDemoApp->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			/*ЗДЕСЬ ОТСЛЕЖИВАЮТСЯ НАЖАТИЯ ЛЕВОЙ КЛАВИШИ МЫШКИ,  И ВСЕ ПЕРЕХОДЫ ТОЖЕ ТУТ*/
			case WM_LBUTTONDOWN:
			{
				float x, y;
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				  
				switch (pDemoApp->Status_of_game_window_instanse)
				{
					case Menu:
					{
						if (HitInButton(x, y, pDemoApp->menu_button))
						{
							pDemoApp->Status_of_game_window_instanse = Preparing;
							pDemoApp->OnRender();
							ValidateRect(hwnd, NULL);
						}
						break;
					}

					case Preparing:
					{
						
						break;
					}

					case Playfield:
					{
						
						break;
					}
				}


				
				break;
			}
			result = 0;
			wasHandled = true;
			break;


			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Draw a grid background.
		width = static_cast<int>(rtSize.width);
		height = static_cast<int>(rtSize.height);
		int x_center = width / 2;
		int y_center = height / 2;
		int width_of_cell =20;

		switch (Status_of_game_window_instanse)
		{
			case Menu:
			{
				DrawMenu(width, height);
				break;

			}
			
			case Preparing:
			{
				DrawPreparing(x_center, y_center, width_of_cell);
				break;
			}

			case Playfield:
			{
				DrawPlayField(x_center, y_center, width_of_cell);
				break;
			}


		}


		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void  DemoApp::DrawField(int x_center, int y_center, int width_of_cell)
{

	HRESULT hr = S_OK;


	ID2D1SolidColorBrush* black;

	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&black
		);

	/*===================================РИСУЮ КВАДРАТНОЕ ПОЛЕ 10Х10 ==========================================*/
	for (int i = -5; i < 6; i++)
	{

		/*Рисую гориз линии*/
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(x_center - 5 * width_of_cell, static_cast<FLOAT>(y_center + i*width_of_cell)),
			D2D1::Point2F(x_center + 5 * width_of_cell, static_cast<FLOAT>(y_center + i*width_of_cell)),
			black,
			0.5f
			);

		/*Рисую верт линии*/
		m_pRenderTarget->DrawLine(
			D2D1::Point2F(x_center + i * width_of_cell, static_cast<FLOAT>(y_center + 5 * width_of_cell)),
			D2D1::Point2F(x_center + i * width_of_cell, static_cast<FLOAT>(y_center - 5 * width_of_cell)),
			black,
			0.5f
			);
	}


	/*
*/
};

void DemoApp::DrawMenu(int width, int height)
{
	// Прямоугольник главной кнопки меню
	 menu_button= D2D1::RectF(		
		0.2*width, 0.45*height, 0.8*width, 0.55*height
		);

	 // рисую кнопку (прямоуг)
	m_pRenderTarget->DrawRectangle(
		&menu_button,
		m_pLightSlateGrayBrush
		);

	// Это текст в кнопке
	wchar_t * wch = L"New game";

	PrintText(wch, MenuTextFormat, menu_button, m_pCornflowerBlueBrush);
	

};

void DemoApp::PrintText(wchar_t* text, IDWriteTextFormat* text_format, D2D1_RECT_F rect_for_text, ID2D1SolidColorBrush* brush)
{

	int text_lenght = std::wcslen(text);

								/* рисую сам текст, */
	m_pRenderTarget->DrawTextA(
		text,					/*текстовая переменная*/
		text_lenght,			/*ее длина*/
		text_format,			/*ранее созданный стиль текста*/
		&rect_for_text,			/*ссылка на прямогульник, в котором разместиться текст*/
		brush					/*экземпляр кисти определенного цвета, хз какого*/
		);
}


bool DemoApp::HitInButton(float x, float y, D2D1_RECT_F rect)
{
	return (x > rect.left && x < rect.right && y<rect.bottom && y > rect.top);	/*банальная проверка на вхождение координат*/
}



/*==============ЭТА ВЕЩЬ НЕ РАБОТАЕТ И НЕ ИСПОЛЬЗУЕТСЯ, НО Я ЕЕ НЕ УДАЛЯЮ, ПОТОМУ ЧТО ТАК НАДО!=================*/
HRESULT DemoApp::CreateD2DBitmapFromFile(HWND hWnd)
{
	HRESULT hr = S_OK;

	WCHAR szFileName[MAX_PATH]=L"1.jpg";

	// Step 1: Create the open dialog box and locate the image file

	//if (LocateImageFile(hWnd, szFileName, ARRAYSIZE(szFileName)))
	//{

		// Step 2: Decode the source image

		// Create a decoder
		IWICBitmapDecoder *pDecoder = nullptr;

		hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

		hr = CoCreateInstance(
			_uuidof(IWICImagingFactory)
			/*CLSID_WICImagingFactory*/,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pIWICFactory)
			);
		/*
		hr = m_pIWICFactory->CreateDecoderFromFilename(
			szFileName,                      // Image to be decoded
			NULL,                         // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&pDecoder                        // Pointer to the decoder
			);*/

		// Retrieve the first frame of the image from the decoder
		IWICBitmapFrameDecode *pFrame = nullptr;

		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
		}

		//Step 3: Format convert the frame to 32bppPBGRA
		if (SUCCEEDED(hr))
		{
			SafeRelease(m_pConvertedSourceBitmap);
			hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pConvertedSourceBitmap->Initialize(
				pFrame,                          // Input bitmap to convert
				GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
				WICBitmapDitherTypeNone,         // Specified dither pattern
				nullptr,                         // Specify a particular palette 
				0.f,                             // Alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
				);
		}

		//Step 4: Create render target and D2D bitmap from IWICBitmapSource
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources();
		}

		if (SUCCEEDED(hr))
		{
			// Need to release the previous D2DBitmap if there is one
			SafeRelease(m_pD2DBitmap);
			hr = m_pRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, nullptr, &m_pD2DBitmap);
		}

		SafeRelease(pDecoder);
		SafeRelease(pFrame);
	

	return hr;
}


HRESULT DemoApp::LoadBitmapFromFile(
	ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,
	PCWSTR uri,/*
	UINT destinationWidth,
	UINT destinationHeight,*/
	ID2D1Bitmap **ppBitmap
	)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}


	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
			);

	}
	if (SUCCEEDED(hr))
	{

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
			);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}


void DemoApp::DrawPlayField(int x_center, int y_center, int width_of_cell)
{
	DrawField(x_center / 2, y_center, width_of_cell);
	DrawField(x_center * 3 / 2, y_center, width_of_cell);
}

void DemoApp::DrawPreparing(int x_center, int y_center, int width_of_cell)
{
	
	D2D1_RECT_F TitleRect = D2D1::RectF(0.2*width, 0.1*height, 0.8*width, 0.3*height);
	
	PrintText(L"Расположите корабли", MenuTextFormat, TitleRect, m_pCornflowerBlueBrush);
	
	
	DrawField(x_center * 3 / 2, y_center, width_of_cell);

}





/*==================================ЗДЕСЬ Я ПЫТАЮСЬ ВСТАВИТЬ КАРТИНКУ В ОКНО=================================================================*/


/*	if (SUCCEEDED(hr))
{
hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(),
nullptr,
0,
D2D1::BitmapProperties(),
&m_pBitmap
);
}


if (SUCCEEDED(hr))
{
CoCreateInstance(
CLSID_WICImagingFactory,
nullptr,
CLSCTX_INPROC_SERVER,
IID_PPV_ARGS(&m_pImagingFactory)
);
}
PCWSTR pict = L"1.jpg";

if (SUCCEEDED(hr))
{
LoadBitmapFromFile(m_pRenderTarget, m_pImagingFactory, pict, &m_pBitmap);
}
if (SUCCEEDED(hr))
{
m_pRenderTarget->DrawBitmap(
m_pBitmap, &rectangle2, 50.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &rectangle2
);
}*/
/*==================================================================================================================================================*/





/*
for (int x = 0; x < width; x += 10)
{
c
}

for (int y = 0; y < height; y += 10)
{
m_pRenderTarget->DrawLine(
D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
m_pLightSlateGrayBrush,
0.5f
);
}

// Draw two rectangles.
D2D1_RECT_F rectangle1 = D2D1::RectF(
rtSize.width / 2 - 50.0f,
rtSize.height / 2 - 50.0f,
rtSize.width / 2 + 50.0f,
rtSize.height / 2 + 50.0f
);

D2D1_RECT_F rectangle2 = D2D1::RectF(
rtSize.width / 2 - 100.0f,
rtSize.height / 2 - 100.0f,
rtSize.width / 2 + 100.0f,
rtSize.height / 2 + 100.0f
);

// Draw a filled rectangle.
m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

// Draw the outline of a rectangle.
m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);


D2D1_RECT_F rectangle1 = D2D1::RectF(
rtSize.width / 2 - 50.0f,
rtSize.height / 2 - 50.0f,
rtSize.width / 2 + 50.0f,
rtSize.height / 2 + 50.0f
);

m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

*/





