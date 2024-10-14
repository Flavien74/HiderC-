#include "LoadingHelper.h"

LoadingHelper::LoadingHelper()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

LoadingHelper::~LoadingHelper()
{
    // Nettoyage de GDI+
    GdiplusShutdown(m_gdiplusToken);
}

bool LoadingHelper::LoadImageFromFile(const std::wstring& filePath)
{
    m_image = new Gdiplus::Image(filePath.c_str());
    if (m_image->GetLastStatus() != Gdiplus::Ok) {
        delete m_image;
        m_image = nullptr;
        return false;
    }
    return true;
}

void LoadingHelper::Draw(HWND hwnd, int x, int y)
{
    if (m_image) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics graphics(hdc);
        graphics.DrawImage(m_image, x, y);
        EndPaint(hwnd, &ps); // Terminer le dessin
    }
}
