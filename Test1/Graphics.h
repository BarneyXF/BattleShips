#pragma once
#include "Header.h"

class Graphics
{
public:
	Graphics(ID2D1HwndRenderTarget*);
	~Graphics();
	HRESULT DrawButton();

private:
	ID2D1HwndRenderTarget* m_pRenderTarget_Graphics;
	D2D1_SIZE_F rtSize;
};

