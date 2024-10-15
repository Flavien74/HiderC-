#pragma once

#include "resource.h"

#define BUTTON1_ID 1
#define BUTTON2_ID 2
#define BUTTON3_ID 2
#define EDIT_ID 3
#define TEXT_ID 4
#define TEXT2_ID 5
#define TEXT3_ID 6

//Variable :
int nbCharacterPossible = 50;
int nbCurrentCharacter = 0;
int nbLastCharacter = 0;
wchar_t bufferMessage[255];
static bool isTextCleared = false;

class ImageHelper;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
