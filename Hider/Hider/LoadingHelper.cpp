#include "LoadingHelper.h"

LoadingHelper::LoadingHelper() : 
    m_hBitmap(nullptr) 
{
}

LoadingHelper::~LoadingHelper()
{
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

bool LoadingHelper::OpenImageFile(HWND hWnd, LoadingHelper* loadingHelper)
{
    OPENFILENAME ofn;
    wchar_t szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    //ofn.lpstrFilter = L"Images (*.png;*.bmp;*.jpg;*.jpeg)\0*.png;*.bmp;*.jpg;*.jpeg\0All Files (*.*)\0*.*\0";
    ofn.lpstrFilter = L"Images (*.png;)\0*.png;\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = L"Sélectionnez une image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetOpenFileName(&ofn)) 
    {                
        //LOAD IMAGE
        return true;
    }
    return false;
}

void LoadingHelper::Draw(HDC hdc, int x, int y)
{
    if (m_hBitmap) {
        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
        BITMAP bitmap;
        GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);
        BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}
