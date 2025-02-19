#pragma once

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;

class ImageHelper
{
public:

	Bitmap* m_bitMap;
	HBITMAP m_hBitmap;

	ImageHelper();
	~ImageHelper();

	void Draw(HDC hdc, int x, int y, int width = -1, int height = -1);
};

