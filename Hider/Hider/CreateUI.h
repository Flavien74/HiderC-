#pragma once
#include <windows.h>
#include "LoadingHelper.h"

class ImageHelper;

class CreateUI
{
private:
	HINSTANCE hInstance;
public:
	int baseWindowWidth = 1000;
	int baseWindowHeight = 800;

	int bigButtonWidth = 200;
	int bigButtonHeight = 50;
	int buttonWidth = 150;
	int buttonHeight = 30;
public:

	CreateUI(HINSTANCE);
	~CreateUI();

	HWND CreateButton(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateBaseWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, ImageHelper* = nullptr);
	HWND CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, ImageHelper* = nullptr);

	HWND CreateTextZone(HWND, int, LPCWSTR, int, int, int, int, long);
};

