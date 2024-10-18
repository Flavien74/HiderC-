#pragma once

#include "resource.h"
#include <vector>
#include "ButtonUI.h"

#define BUTTON_LOAD_1 1
#define BUTTON_LOAD_2 11
#define BUTTON_STENO 2
#define BUTTON_REVEAL 3
#define EDIT_ID_1 4
#define EDIT_ID_2 4
#define TEXT_RESTANT_1 5
#define TEXT2_ID 6
#define TEXT3_ID 7
#define CLEAR_IMAGE 8

ACCEL accelerators[] = {
    { FVIRTKEY | FCONTROL, 'L', 1001 }, // Ctrl+L
    { FVIRTKEY | FCONTROL, 'D', 1002 },  // Ctrl+D
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
wchar_t bufferEncryptMessage[256];
wchar_t buffernumber[255];
static bool isTextCleared = false;
static bool isFirstChange = true;
CreateUI* createUI = nullptr;
Stenography* steno = nullptr;
LoadingHelper* loadingHelperDecrytpe = nullptr;
LoadingHelper* loadingHelperEncrypte = nullptr;
std::vector<ObjectUI*>* UIObject = nullptr;

//HBRUSH hBrushTransparent = (HBRUSH)GetStockObject(WHITE_BRUSH);
HBRUSH hBrushTransparent = (HBRUSH)GetStockObject(DKGRAY_BRUSH);

HWND firstWindow;
static HWND buttonLoadCase, buttonLoadCase2, buttonSteno, buttonClear, ButtonReveal, hEdit, hEdit2, TextCharRestant;
HBITMAP hbmMem;
HDC hdcMem;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void InitializeUI(HWND* hWnd);

void HandleCommand(HWND* hWnd, WPARAM wParam, LPARAM lParam);

HBRUSH HandleCtlColorStatic(WPARAM wParam, LPARAM lParam, HBRUSH hBrushTransparent, HWND);

void HandleStenography(HWND* hWnd, HWND* hEdit, wchar_t* bufferMessage);
void HandleRevealMessage(HWND* hWnd, HWND* hEdit2);

void HandleClearImage(HWND* hWnd, HWND* hEdit, HWND* hEdit2);

void HandleAccelerators(HWND* hWnd, WPARAM wParam);

void HandleTextEdit(HWND* hWnd, WPARAM wParam, LPARAM lParam);
void UpdateTextRemaining(HWND* hWnd, HWND* hEdit, HWND TextCharRestant, int& nbCharacterPossible, int& nbLastCharacter);

void HandleLoadButton1(HWND* hWnd);
void HandleLoadButton2(HWND* hWnd);
void HandleWM_PAINT(HWND* hWnd);

void HandleResize(HWND* hWnd, LPARAM lParam);
void ResizeWindow(int width, int height);

void Cleanup(HWND* hWnd, ULONG_PTR gdiplusToken);
void DestroyLoadingHelper(HWND* hWnd, bool isEncrypt = false);

void CreateMemoryDC(HWND* hWnd);
void DrawScaledImage(HDC hdc, ImageHelper* image, int x, int y, int maxWidth, int availableHeight);
void CleanupMemoryDC();

int ReturnIndexObject(int id);
