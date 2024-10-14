#pragma once

#include "resource.h"


#define BUTTON1_ID 1
#define BUTTON2_ID 2
#define EDIT_ID 3

// Classe :

class LoadingHelper;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateButton(HWND, int, LPCWSTR, int, int, int, int);
HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);
void CreateAWindow(HINSTANCE, int, LPCWSTR, LPCWSTR, WNDPROC, LoadingHelper* = nullptr);