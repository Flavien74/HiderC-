#pragma once
#include <Windows.h>

class ImageHelper;
class ExtensionHelper;

class LoadingHelper
{
public :
	ImageHelper* m_currentImage;
	ExtensionHelper* m_currentExtension;

	LoadingHelper();
	~LoadingHelper();

	bool OpenImageFile(HWND hWnd);
	bool LoadImageFromFile();

	bool SaveImage();
};

