#include "LoadingHelper.h"
#include "ImageHelper.h"
#include "ExtensionHelper.h"
#include <vector>

LoadingHelper::LoadingHelper() :
    m_currentImage(nullptr),
    m_currentExtension(nullptr)
{
}

LoadingHelper::~LoadingHelper()
{
}

bool LoadingHelper::OpenImageFile(HWND hWnd)
{
    m_currentImage = new ImageHelper();
    m_currentExtension = new ExtensionHelper();
    OPENFILENAME ofn;
    wchar_t szFile[260];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Images (*.png;*.bmp;*.jpg;*.jpeg)\0*.png;*.bmp;*.jpg;*.jpeg\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = L"Sélectionnez une image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetOpenFileName(&ofn)) 
    {
        if (m_currentExtension->SetExtensionByPath(ofn.lpstrFile)
            && LoadImageFromFile())
        {
            return true;
        }
    }
    return false;
}

bool LoadingHelper::LoadImageFromFile()
{
    LPWSTR newStr = nullptr;
    std::wstring completePath = m_currentExtension->GetCompletePath();
    std::vector<wchar_t> buffer(completePath.begin(), completePath.end());
    buffer.push_back(L'\0');
    newStr = buffer.data();
    m_currentImage->m_bitMap = new Bitmap(newStr);

    if (m_currentImage->m_bitMap->GetLastStatus() != Ok) {
        MessageBox(NULL, L"Erreur lors de la création du bitmap.", L"Erreur", MB_OK);
        return false;
    }

    if (m_currentImage->m_bitMap->GetHBITMAP(Color(0, 0, 0, 0), &m_currentImage->m_hBitmap) != Ok) {
        MessageBox(NULL, L"Erreur lors de la conversion de l'image en HBITMAP.", L"Erreur", MB_OK);
        return false;
    }

    GetObject(m_currentImage->m_hBitmap, sizeof(Bitmap), &m_currentImage->m_bitMap);
    return true;
}

bool LoadingHelper::SaveImage(std::wstring newPathName)
{
    if (m_currentImage->m_bitMap->GetLastStatus() != Ok) {
        return false;
    }

    const WCHAR* mimeType = nullptr;
    if (m_currentExtension->m_extension == L".png") {
        mimeType = L"image/png";
    }
    else if (m_currentExtension->m_extension == L".jpg" || m_currentExtension->m_extension == L".jpeg") {
        mimeType = L"image/jpeg";
    }
    else if (m_currentExtension->m_extension == L".bmp") {
        mimeType = L"image/bmp";
    }
    else if (m_currentExtension->m_extension == L".gif") {
        mimeType = L"image/gif";
    }
    else {
        return false;  // Unsupported format
    }

    // Get the CLSID for the desired format
    CLSID clsid;
    if (GetEncoderClsid(mimeType, &clsid) == -1) {
        return false;  // Could not get CLSID
    }

    // Save the image
    Status status = m_currentImage->m_bitMap->Save(newPathName.c_str(), &clsid, nullptr);

    return (status == Ok);  // Return true if save is successful
}

// Helper function to get the CLSID of an image encoder (e.g., PNG, JPEG, BMP)
int LoadingHelper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;          // Number of image encoders
    UINT size = 0;         // Size of the image encoder array in bytes

    // Get the number and size of the image encoders
    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;  // Failure

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == nullptr) return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT i = 0; i < num; ++i)
    {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return i;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}
