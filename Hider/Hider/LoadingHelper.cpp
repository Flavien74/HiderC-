#include "LoadingHelper.h"

LoadingHelper::LoadingHelper() :
    m_hBitmap(NULL),
    m_path(NULL)
{
}

LoadingHelper::~LoadingHelper()
{
    if (m_hBitmap) {
        DeleteObject(m_hBitmap);
    }
}

bool LoadingHelper::OpenImageFile(HWND hWnd)
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
        SetPathW(ofn.lpstrFile);
        return LoadImageFromFile(ofn.lpstrFile); // Charger l'image ici
    }
    return false;
}

bool LoadingHelper::LoadImageFromFile(LPCWSTR filename) 
{
    Bitmap bitmap(filename);

    if (bitmap.GetLastStatus() != Ok) {
        MessageBox(NULL, L"Erreur lors de la création du bitmap.", L"Erreur", MB_OK);
        return false;
    }

    if (bitmap.GetHBITMAP(Color(0, 0, 0, 0), &m_hBitmap) != Ok) {
        MessageBox(NULL, L"Erreur lors de la conversion de l'image en HBITMAP.", L"Erreur", MB_OK);
        return false;
    }

    GetObject(m_hBitmap, sizeof(BITMAP), &m_bitMap);
    return true; // Chargement réussi
}

void LoadingHelper::Draw(HDC hdc, int x, int y)
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

void LoadingHelper::SetPathW(LPWSTR path)
{
    if (m_path) {
        delete[] m_path;
    }
    // Allouer de la mémoire pour le nouveau chemin
    size_t len = wcslen(path) + 1; // +1 pour le caractère nul
    m_path = new WCHAR[len];
    wcscpy_s(m_path, len, path);
}

LPWSTR LoadingHelper::GetPathW()
{
    return m_path;
}

void LoadingHelper::SetMessageW(std::string message)
{
    m_message = message;
}

std::string LoadingHelper::GetMessageW()
{
    return m_message;
}

BITMAP LoadingHelper::GetBitMap()
{
    return m_bitMap;
}
