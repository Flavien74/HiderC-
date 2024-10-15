#include "ImageHelper.h"

ImageHelper::ImageHelper() :
    m_hBitmap(NULL)
{

}

ImageHelper::~ImageHelper()
{
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

void ImageHelper::Draw(HDC hdc, int x, int y)
{
    if (m_hBitmap) {
        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
        GetObject(m_hBitmap, sizeof(BITMAP), &m_bitMap);
        BitBlt(hdc, x, y, m_bitMap.bmWidth, m_bitMap.bmHeight, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}
