#pragma once

#include <windows.h>
#include "resource.h"

#define BUTTON1_ID 1
#define BUTTON2_ID 2
#define BUTTON3_ID 3
#define EDIT_ID 4
#define TEXT_ID 5
#define TEXT2_ID 6
#define TEXT3_ID 7

// Déclaration des accélérateurs
ACCEL accelerators[] = {
    { FVIRTKEY | FCONTROL, 'L', 1001 }, // Ctrl+L
    { FVIRTKEY | FCONTROL, 'M', 1002 }, // Ctrl+M
    { FVIRTKEY | FCONTROL, 'E', 1003 }  // Ctrl+E
};

class ImageHelper;
class CreateUI;
class Stenography;
class LoadingHelper;

//Variable :
int nbCharacterPossible = 0;
int nbCurrentCharacter = 0;
int nbLastCharacter = 0;
wchar_t bufferMessage[256];
wchar_t buffernumber[255];
static bool isTextCleared = false;
CreateUI* createUI = nullptr;
Stenography* steno = nullptr;
LoadingHelper* loadingHelper = nullptr;

HBRUSH hBrushTransparent = NULL;

HWND firstWindow;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool CheckAndCreateLoadingHelper(HWND hWnd);
void DestroyLoadingHelper(HWND hWnd);
