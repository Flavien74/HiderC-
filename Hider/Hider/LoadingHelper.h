#pragma once
#include <Windows.h>
#include <string>

class ImageHelper;
class ExtensionHelper;

class LoadingHelper
{
public :
	ImageHelper* m_currentImage;
	ExtensionHelper* m_currentExtension;

	LoadingHelper();
	~LoadingHelper();

	bool Init(HWND hWnd);
	bool OpenImageFile(HWND hWnd);
	bool LoadImageFromFile();

	bool SaveImage(std::wstring newPathName);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};