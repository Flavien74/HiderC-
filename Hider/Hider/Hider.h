#pragma once

#include "resource.h"


#define MAX_LOADSTRING 100
#define BUTTON_ID 1
#define EDIT_ID 2

// Variables globales :
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// Classe :

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);