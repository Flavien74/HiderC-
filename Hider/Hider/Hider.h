#pragma once

#include "resource.h"
#include <vector>
#include "ButtonUI.h"

#define BUTTON1_ID 1
#define BUTTON2_ID 2
#define BUTTON3_ID 3
#define EDIT_ID 4
#define TEXT_ID 5
#define TEXT2_ID 6
#define TEXT3_ID 7

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
std::vector<ObjectUI>* UIObject = nullptr;

HBRUSH hBrushTransparent = NULL;

HWND firstWindow;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ResizeWindow(int width, int height);

int ReturnIndexObject(int id);

bool CheckAndCreateLoadingHelper(HWND hWnd);
void DestroyLoadingHelper(HWND hWnd);
