#pragma once

#include "resource.h"
#include <vector>
#include "ButtonUI.h"

#define BUTTON_LOAD_1 1
#define BUTTON_LOAD_2 11
#define BUTTON2_ID 2
#define BUTTON3_ID 3
#define EDIT_ID_1 4
#define EDIT_ID_2 4
#define TEXT_RESTANT_1 5
#define TEXT2_ID 6
#define TEXT3_ID 7
#define CLEAR_IMAGE 8

class ImageHelper;
class CreateUI;
class Stenography;
class LoadingHelper;

//Variable :
int nbCharacterPossible = 0;
int nbCurrentCharacter = 0;
int nbLastCharacter = 0;
wchar_t bufferMessage[256];
wchar_t bufferEncryptMessage[256];
wchar_t buffernumber[255];
static bool isTextCleared = false;
CreateUI* createUI = nullptr;
Stenography* steno = nullptr;
LoadingHelper* loadingHelperDecrytpe = nullptr;
LoadingHelper* loadingHelperEncrypte = nullptr;
std::vector<ObjectUI>* UIObject = nullptr;

HBRUSH hBrushTransparent = NULL;

HWND firstWindow;
static HWND buttonLoadCase, buttonLoadCase2, buttonSteno, ButtonReveal, hEdit, hEdit2, TextCharRestant, TextMessageLabel, TextMessageReturn;
HBITMAP hbmMem;
HDC hdcMem;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ResizeWindow(int width, int height);
int ReturnIndexObject(int id);
bool CheckAndCreateLoadingHelper(HWND hWnd);
void DestroyLoadingHelper(HWND hWnd, bool isEncrypt = false);
void CreateMemoryDC(HWND hWnd);
void HandleWM_PAINT(HWND hWnd);
void CleanupMemoryDC();
void DrawScaledImage(HDC hdc, ImageHelper* image, int x, int y, int maxWidth, int availableHeight);
