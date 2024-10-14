#pragma once
#include <windows.h>
#include <string>

#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

class LoadingHelper
{
public :

	LoadingHelper();
	~LoadingHelper();

	bool OpenImageFile(HWND hWnd);
	bool LoadImageFromFile(LPCWSTR filename);
	void Draw(HDC hdc, int x, int y);
	void SetPathW(LPWSTR path);
	LPWSTR GetPathW();
	void SetMessageW(std::string message);
	std::string GetMessageW();
	BITMAP GetBitMap();

private :

	BITMAP m_bitMap;
	HBITMAP m_hBitmap;
	LPWSTR m_path;
	std::string m_message;
};

