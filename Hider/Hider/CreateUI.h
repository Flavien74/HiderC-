#pragma once
#include <windows.h>
#include "LoadingHelper.h"
#include "TransformUI.h"

class ImageHelper;

class CreateUI
{
private:
	HINSTANCE hInstance;
public:
	TransformUI* m_transformWindow = new TransformUI(0, 0, 1000, 800, 0, 0);

	CreateUI(HINSTANCE);
	~CreateUI();

	HWND CreateButton(HWND hWnd, int input_id, LPCWSTR message, TransformUI* transform);
	HWND CreateInput(HWND hWnd, int input_id, LPCWSTR message, TransformUI* transform);
	HWND CreateBaseWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC);
	HWND CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, ImageHelper* = nullptr);

	HWND CreateTextZone(HWND hWnd, int input_id, LPCWSTR message, TransformUI* transform, long Align);
};

