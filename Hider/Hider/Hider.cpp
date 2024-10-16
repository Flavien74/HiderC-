#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include "CreateUI.h"
#include "ImageHelper.h"
#include "ExtensionHelper.h"
#include "LoadingHelper.h"
#include "Stenography.h"

#include <vector>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	createUI = new CreateUI(hInstance);
	steno = new Stenography();

	firstWindow = createUI->CreateAWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hbutton1, hbutton2, hbutton3, hEdit, hStatic1, hStatic2, hStatic3;

    static bool isTextCleared = false;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message)
	{
	case WM_CREATE:
	{
		//hbutton1 = createUI->CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", (createUI->baseWindowWidth / 2) - (createUI->buttonWidth / 2), (createUI->baseWindowWidth / 8), createUI->buttonWidth, createUI->buttonHeight);
		hStatic1 = createUI->CreateTextZone(hWnd, TEXT_ID, L"", (createUI->baseWindowWidth / 2) + (200 / 2), (createUI->baseWindowWidth / 5), 25, 15);
		hEdit = createUI->CreateInput(hWnd, EDIT_ID, L"Message a cacher", (createUI->baseWindowWidth / 2) - (250 / 2), (createUI->baseWindowWidth / 4.5), 250, 50);

		//hbutton2 = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", (createUI->baseWindowWidth / 2) - (350 / 2), (createUI->baseWindowWidth / 3), 350, 50);
		hbutton2 = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", (createUI->baseWindowWidth / 2) - (createUI->bigButtonWidth / 2), (createUI->baseWindowWidth / 3), createUI->bigButtonWidth, createUI->bigButtonHeight);
		hbutton3 = createUI->CreateButton(hWnd, BUTTON3_ID, L"Reveler le message", (createUI->baseWindowWidth / 2) - (createUI->bigButtonWidth / 2), (createUI->baseWindowWidth / 2.4), createUI->bigButtonWidth, createUI->bigButtonHeight);

		hStatic2 = createUI->CreateTextZone(hWnd, TEXT3_ID, L"Message cache : ", (createUI->baseWindowWidth / 2) - (200 / 2), (createUI->baseWindowWidth / 1.8), 200, 20);
		hStatic3 = createUI->CreateTextZone(hWnd, TEXT2_ID, L"", (createUI->baseWindowWidth / 2) - (200 / 2), (createUI->baseWindowWidth / 1.75), 200, 20);

		break;
	}
	case WM_SIZE:
	{
		if (LOWORD(lParam) < (createUI->baseWindowWidth - 50))
		{
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			int posY = (screenHeight / 2) - (createUI->baseWindowHeight / 2);
			int posX = (screenWidth / 2) - (createUI->baseWindowWidth / 2);

			MoveWindow(firstWindow, posX, posY, createUI->baseWindowWidth - 50, createUI->baseWindowHeight, TRUE);
		}
		else if (HIWORD(lParam) < (createUI->baseWindowHeight - 50))
		{
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			int posY = (screenHeight / 2) - (createUI->baseWindowHeight / 2);
			int posX = (screenWidth / 2) - (createUI->baseWindowWidth / 2);

			MoveWindow(firstWindow, posX, posY, createUI->baseWindowWidth, createUI->baseWindowHeight - 50, TRUE);
		}
		else {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			//MoveWindow(hbutton1, (width / 2) - (createUI->buttonWidth / 2), (height / 8), createUI->buttonWidth, createUI->buttonHeight, TRUE);
			MoveWindow(hStatic1, (width / 2) + (200 / 2), (height / 5), 25, 15, TRUE);
			MoveWindow(hEdit, (width / 2) - (250 / 2), (height / 4.5), 250, 50, TRUE);
			//MoveWindow(hbutton2, (width / 2) - (350 / 2), (height / 3), 350, 50, TRUE);
			MoveWindow(hbutton2, (width / 2) - (createUI->bigButtonWidth / 2), (height / 3), createUI->bigButtonWidth, createUI->bigButtonHeight, TRUE);
			MoveWindow(hbutton3, (width / 2) - (createUI->bigButtonWidth / 2), (height / 2.4), createUI->bigButtonWidth, createUI->bigButtonHeight, TRUE);
			MoveWindow(hStatic2, (width / 2) - (200 / 2), (height / 1.8), 200, 20, TRUE);
			MoveWindow(hStatic3, (width / 2) - (200 / 2), (height / 1.72), 200, 20, TRUE);
		}
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) // Check which control sent the WM_COMMAND message
		{
		case EDIT_ID:
		{
			if (HIWORD(wParam) == EN_SETFOCUS && !isTextCleared)
			{
				// Efface le texte au focus s'il n'a pas déjà été effacé
				SetWindowText(hEdit, TEXT(""));
				isTextCleared = true;
			}
			if (HIWORD(wParam) == EN_CHANGE)
			{
				nbCurrentCharacter = GetWindowTextLength(hEdit);
				if (nbCurrentCharacter > nbLastCharacter) 
				{
					if (nbCharacterPossible > 0)
					{
						nbCharacterPossible--;
					}
					else
					{
						SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)1, 0);  // Bloquer les entrées supplémentaires
					}
				}
				if (nbCurrentCharacter < nbLastCharacter)
				{
					nbCharacterPossible++;
				}
				nbLastCharacter = nbCurrentCharacter;
				swprintf(buffernumber, 50, L"%d", nbCharacterPossible);

				SetWindowText(hStatic1, buffernumber);
			}
			break;
		}
		case BUTTON1_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd))
			{
				createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper->m_currentImage);
				DestroyLoadingHelper(hWnd);
				break;
			}
		}
		case BUTTON2_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd))
			{
				GetWindowText(hEdit, bufferMessage, 255);
				steno->LSBEncode(loadingHelper->m_currentImage->m_bitMap, bufferMessage);

				loadingHelper->SaveImage(loadingHelper->m_currentExtension->GetNewCompletePath());

				createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture_Encrypte", PictureWndProc, loadingHelper->m_currentImage);
				DestroyLoadingHelper(hWnd);
			}
			break;
		}
		case BUTTON3_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd)) 
			{
				std::wstring newMessage = steno->LSBDecode(loadingHelper->m_currentImage->m_bitMap);

				LPCWSTR lpcwstr = newMessage.c_str();
				SetWindowText(hStatic3, lpcwstr);

				DestroyLoadingHelper(hWnd);
			}
			break;
		}
		}
		break;
	}

	case WM_DESTROY:

		delete loadingHelper;
		loadingHelper = nullptr;
		delete createUI;
		createUI = nullptr;
		delete steno;
		steno = nullptr;
		GdiplusShutdown(gdiplusToken);

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImageHelper* imageHelper = (ImageHelper*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	PAINTSTRUCT ps;

    switch (message)
    {
	case WM_PAINT:
	{
		int x, y = 0;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		int windowWidth = clientRect.right - clientRect.left;
		int windowHeight = clientRect.bottom - clientRect.top;
		
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
		HGDIOBJ hOldBitmap = SelectObject(hdcMem, hbmMem);

		HBRUSH hBrush = (HBRUSH)(COLOR_WINDOW + 1);
		FillRect(hdcMem, &clientRect, hBrush);

		if (imageHelper && imageHelper->m_bitMap)
		{
			int imgWidth = imageHelper->m_bitMap->GetWidth();
			int imgHeight = imageHelper->m_bitMap->GetHeight();

			float aspectRatio = (float)imgWidth / (float)imgHeight;

			int newWidth = windowWidth;
			int newHeight = static_cast<int>(newWidth / aspectRatio);

			if (newHeight > windowHeight) {
				newHeight = windowHeight;
				newWidth = static_cast<int>(newHeight * aspectRatio);
			}

			x = (windowWidth - newWidth) / 2;
			y = (windowHeight - newHeight) / 2;

			imageHelper->Draw(hdcMem, x, y, newWidth, newHeight);
		}

		BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOldBitmap);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_DESTROY:

		delete loadingHelper;
		loadingHelper = nullptr;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool CheckAndCreateLoadingHelper(HWND hWnd)
{
	if (loadingHelper == nullptr)
	{
		loadingHelper = new LoadingHelper();
		if (!loadingHelper->OpenImageFile(hWnd))
		{
			delete loadingHelper;
			loadingHelper = nullptr;
			return false;
		}
	}
	return true;
}

void DestroyLoadingHelper(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, TRUE);

	delete loadingHelper;
	loadingHelper = nullptr;
}
