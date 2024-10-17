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

	void Copy(LoadingHelper* toCopy);

	bool Init(HWND hWnd, LPCWSTR title);
	bool OpenImageFile(HWND hWnd, LPCWSTR& title);
	bool LoadImageFromFile();

	bool SaveImage(std::wstring newPathName);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};