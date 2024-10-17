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
	loadingHelper = new LoadingHelper();
	firstWindow = createUI->CreateBaseWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

	HACCEL hAccelTable = CreateAcceleratorTable(accelerators, sizeof(accelerators) / sizeof(ACCEL));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateAccelerator(firstWindow, hAccelTable, &msg); // Traite les accélérateurs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyAcceleratorTable(hAccelTable);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hbutton1, hbutton2, hbutton3, hEdit, hStatic1, hStatic2, hStatic3;

	static bool isTextCleared = false;
	static bool isFirstChange = true;
	static bool isEditing = false;;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message)
	{
	case WM_CREATE:
	{
		hBrushTransparent = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		hbutton1 = createUI->CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", (createUI->baseWindowWidth / 2) - (createUI->buttonWidth / 2), (createUI->baseWindowWidth / 8), createUI->buttonWidth, createUI->buttonHeight);
		hStatic1 = createUI->CreateTextZone(hWnd, TEXT_ID, L"", (createUI->baseWindowWidth / 2) + (200 / 2), (createUI->baseWindowWidth / 5), 25, 100, ES_RIGHT);
		hEdit = createUI->CreateInput(hWnd, EDIT_ID, L"Message a cacher", (createUI->baseWindowWidth / 2) - (250 / 2), (createUI->baseWindowWidth / 4.5), 250, 50);

		//hbutton2 = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", (createUI->baseWindowWidth / 2) - (350 / 2), (createUI->baseWindowWidth / 3), 350, 50);
		hbutton2 = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier un message", (createUI->baseWindowWidth / 2) - (createUI->bigButtonWidth / 2), (createUI->baseWindowWidth / 3), createUI->bigButtonWidth, createUI->bigButtonHeight);
		hbutton3 = createUI->CreateButton(hWnd, BUTTON3_ID, L"Reveler un message", (createUI->baseWindowWidth / 2) - (createUI->bigButtonWidth / 2), (createUI->baseWindowWidth / 2.4), createUI->bigButtonWidth, createUI->bigButtonHeight);

		hStatic2 = createUI->CreateTextZone(hWnd, TEXT3_ID, L"Message cache : ", (createUI->baseWindowWidth / 2) - (200 / 2), (createUI->baseWindowWidth / 1.8), 200, 20, ES_LEFT);
		hStatic3 = createUI->CreateTextZone(hWnd, TEXT2_ID, L"", (createUI->baseWindowWidth / 2) - (200 / 2), (createUI->baseWindowWidth / 1.75), 200, 20, ES_LEFT);

		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		HWND hStatic = (HWND)lParam;

		SetBkMode(hdcStatic, TRANSPARENT);

		if (GetDlgCtrlID(hStatic) == TEXT_ID) {
			COLORREF textColor = RGB(255, 0, 0);
			SetTextColor(hdcStatic, textColor);
		}

		return (INT_PTR)hBrushTransparent;
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

			MoveWindow(hbutton1, (width / 2) - (createUI->buttonWidth / 2), (height / 6), createUI->buttonWidth, createUI->buttonHeight, TRUE);
			MoveWindow(hStatic1, (width / 2) - (200 / 2), (height / 3.3), 220, 15, TRUE);
			MoveWindow(hEdit, (width / 2) - (250 / 2), (height / 3), 250, 50, TRUE);
			MoveWindow(hbutton2, (width / 2) - (350 / 2), (height / 2), 350, 50, TRUE);
			MoveWindow(hbutton3, (width / 2) - (createUI->buttonWidth / 2), (height / 1.5), createUI->buttonWidth, createUI->buttonHeight, TRUE);
			MoveWindow(hStatic2, (width / 2) - (200 / 2), (height / 1.25), 200, 20, TRUE);
			MoveWindow(hStatic3, (width / 2) - (200 / 2), (height / 1.2), 200, 20, TRUE);
		}
	}
	break;
	case WM_COMMAND:
	{
		if (!isEditing) {
			isEditing = true;
			break;
		}
		isEditing = true;
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
				if (isFirstChange)
				{
					isFirstChange = false; // Ignore le premier changement
					break; // Sortie pour ne pas exécuter le reste du code
				}
				if (nbCharacterPossible == 0)
				{
					isEditing = false;
					MessageBox(hWnd, L"Renseigne une image avant de pouvoir renseigner ton message !", L"image missing", MB_ICONERROR | MB_OK);
				}
				else {
					SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)nbCharacterPossible, 0);
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
					else if (nbCurrentCharacter < nbLastCharacter)
					{
						nbCharacterPossible++;
					}
					nbLastCharacter = nbCurrentCharacter;
					swprintf(buffernumber, nbCharacterPossible, L"%d", nbCharacterPossible);
					SetWindowText(hStatic1, L"");
					InvalidateRect(hWnd, NULL, TRUE);
					SetWindowText(hStatic1, buffernumber);
				}
			}
			break;
		}
		case BUTTON1_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd))
			{
				nbCharacterPossible = loadingHelper->m_currentImage->m_bitMap->GetHeight() * loadingHelper->m_currentImage->m_bitMap->GetWidth() * 3;
				swprintf(buffernumber, nbCharacterPossible, L"%d", nbCharacterPossible);

				SetWindowText(hStatic1, buffernumber);

				createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper->m_currentImage);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case BUTTON2_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd))
			{
				GetWindowText(hEdit, bufferMessage, 255);
				if (bufferMessage[0] == L'\0') {
					MessageBox(hWnd, L"Ecris un message à cacher dans l'image !", L"message missing", MB_ICONERROR | MB_OK);
					break;
				}
				if (!loadingHelper->m_currentImage) {
					MessageBox(hWnd, L"Choisis un fichier comme image !", L"iamge missing", MB_ICONERROR | MB_OK);
					break;
				}
				steno->LSBEncode(loadingHelper->m_currentImage->m_bitMap, bufferMessage);

				loadingHelper->SaveImage(loadingHelper->m_currentExtension->GetCompletePath(L"_out"));

				createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture_Encrypte", PictureWndProc, loadingHelper->m_currentImage);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case BUTTON3_ID:
		{
			if (CheckAndCreateLoadingHelper(hWnd)) 
			{
				std::wstring newMessage = steno->LSBDecode(loadingHelper->m_currentImage->m_bitMap);

				LPCWSTR lpcwstr = newMessage.c_str();
				SetWindowText(hStatic3, L"");
				InvalidateRect(hWnd, NULL, TRUE);
				SetWindowText(hStatic3, lpcwstr);

				DestroyLoadingHelper(hWnd);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case 1001:  // ID of the accelerator (Ctrl+L)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON1_ID);
			if (hButton)
			{
				PostMessage(hButton, BM_CLICK, 0, 0);
			}
			break;
		}
		case 1002:  // ID of the accelerator (Ctrl+M)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON2_ID);
			if (hButton)
			{
				PostMessage(hButton, BM_CLICK, 0, 0);
			}
			break;
		}
		case 1003:  // ID of the accelerator (Ctrl+E)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON3_ID);
			if (hButton)
			{
				PostMessage(hButton, BM_CLICK, 0, 0);
			}
			break;
		}
		}
		break;
	}

	case WM_DESTROY:

		DestroyLoadingHelper(hWnd);
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
	PAINTSTRUCT ps;

    switch (message)
    {
	case WM_PAINT:
	{
		if (!loadingHelper) 
		{
			break;
		}
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

		if (loadingHelper->m_currentImage && loadingHelper->m_currentImage->m_bitMap)
		{
			int imgWidth = loadingHelper->m_currentImage->m_bitMap->GetWidth();
			int imgHeight = loadingHelper->m_currentImage->m_bitMap->GetHeight();

			float aspectRatio = (float)imgWidth / (float)imgHeight;

			int newWidth = windowWidth;
			int newHeight = static_cast<int>(newWidth / aspectRatio);

			if (newHeight > windowHeight) {
				newHeight = windowHeight;
				newWidth = static_cast<int>(newHeight * aspectRatio);
			}

			x = (windowWidth - newWidth) / 2;
			y = (windowHeight - newHeight) / 2;

			loadingHelper->m_currentImage->Draw(hdcMem, x, y, newWidth, newHeight);
		}

		BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hOldBitmap);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_SIZE:
	{
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_DESTROY:
		DestroyLoadingHelper(hWnd);
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
	if (loadingHelper == nullptr || loadingHelper->m_currentImage == nullptr || loadingHelper->m_currentExtension == nullptr)
	{
		loadingHelper = new LoadingHelper();
		if (loadingHelper->Init(hWnd)) {
			return true;
		}
		else{
			DestroyLoadingHelper(hWnd);
			return false;
		}
	}
	return true;
}

void DestroyLoadingHelper(HWND hWnd)
{
	if (loadingHelper) 
	{
		loadingHelper->~LoadingHelper();
		delete loadingHelper;
		loadingHelper = nullptr;
	}
}
