#pragma once
#include <windows.h>
#include "LoadingHelper.h"

class ImageHelper;

class CreateUI
{
private:
	HBRUSH hBrushBG;
	HINSTANCE hInstance;
public:
	int baseWindowWidth = 1000;
	int baseWindowHeight = 800;

	int bigButtonWidth = 150;
	int bigButtonHeight = 30;
	int buttonWidth = 150;
	int buttonHeight = 30;
public:

	CreateUI(HINSTANCE);
	~CreateUI();

	HWND CreateButton(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, ImageHelper* = nullptr);
	HWND CreateTextZone(HWND, int, LPCWSTR, int, int, int, int);
};

