#pragma once
#include <windows.h>

// Include GDI+
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
// End Include

#include <string>

using namespace Gdiplus;

class LoadingHelper
{
public :

	LoadingHelper();
	~LoadingHelper();

	bool LoadImageFromFile(const std::wstring& filePath);
	void Draw(HWND hdc, int x, int y);

private : 
	Image* m_image = nullptr;
	ULONG_PTR m_gdiplusToken;
};

