#include "Graphics.h"


HRESULT Graphics::DrawButton()
{

	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - 50.0f,
		rtSize.height / 2 - 50.0f,
		rtSize.width / 2 + 50.0f,
		rtSize.height / 2 + 50.0f
		);

	ID2D1SolidColorBrush* brush;
	m_pRenderTarget_Graphics->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &brush);
	m_pRenderTarget_Graphics->FillRectangle(&rectangle1, brush);
}


Graphics::Graphics(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	m_pRenderTarget_Graphics = m_pRenderTarget;

	m_pRenderTarget_Graphics->BeginDraw();

	m_pRenderTarget_Graphics->SetTransform(D2D1::Matrix3x2F::Identity());

	m_pRenderTarget_Graphics->Clear(D2D1::ColorF(D2D1::ColorF::White));

	rtSize = m_pRenderTarget_Graphics->GetSize();

	// Draw a grid background.
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

}


Graphics::~Graphics()
{
}
