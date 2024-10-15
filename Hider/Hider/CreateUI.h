#pragma once
#include <windows.h>
#include "LoadingHelper.h"

class ImageHelper;

class CreateUI
{
public:
	CreateUI();
	~CreateUI();

	void CreateButton(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);
	void CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, ImageHelper* = nullptr);
	HWND CreateTextZone(HWND, int, LPCWSTR, int, int, int, int);
};

