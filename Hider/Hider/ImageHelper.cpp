#include "ImageHelper.h"

ImageHelper::ImageHelper() :
    m_bitMap(nullptr),
    m_hBitmap(nullptr)
{

}

ImageHelper::~ImageHelper()
{
    if (m_bitMap) {
        delete m_bitMap;
        m_bitMap = nullptr;
    }
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
    }
}

void ImageHelper::Draw(HDC hdc, int x, int y, int width, int height)
{
    if (m_bitMap == nullptr) return;

    Graphics graphics(hdc);

    int imgWidth = m_bitMap->GetWidth();
    int imgHeight = m_bitMap->GetHeight();

    if (width == -1) width = imgWidth;
    if (height == -1) height = imgHeight;

    graphics.DrawImage(m_bitMap, x, y, width, height);
}