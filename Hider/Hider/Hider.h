#pragma once

#include "resource.h"


#define BUTTON1_ID 1
#define BUTTON2_ID 2
#define EDIT_ID 3

// Variables globales :
HINSTANCE hInst;

// Classe :

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void CreateButton(HWND, int, LPCWSTR, int, int, int, int);
HWND CreateInput(HWND, int, LPCWSTR, int, int, int, int);