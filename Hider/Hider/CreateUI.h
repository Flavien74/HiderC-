#pragma once
#include <windows.h>
#include "LoadingHelper.h"

class CreateUI
{
public:
	CreateUI();
	~CreateUI();

	void CreateButton(HWND, int, LPCWSTR, int, int, int, int);
	HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);
	void CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, LoadingHelper* = nullptr);
	HWND CreateTextZone(HWND, int, LPCWSTR, int, int, int, int);
};

