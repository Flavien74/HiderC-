#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include "CreateUI.h"
#include "ImageHelper.h"
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
		hbutton1 = createUI->CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", (createUI->baseWindowWidth / 2) - (createUI->buttonWidth / 2), (createUI->baseWindowWidth / 8), createUI->buttonWidth, createUI->buttonHeight);
		hStatic1 = createUI->CreateTextZone(hWnd, TEXT_ID, L"", (createUI->baseWindowWidth / 2) + (200 / 2), (createUI->baseWindowWidth / 5), 25, 15);
		hEdit = createUI->CreateInput(hWnd, EDIT_ID, L"Message a cacher", (createUI->baseWindowWidth / 2) - (250 / 2), (createUI->baseWindowWidth / 4.5), 250, 50);

		hbutton2 = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", (createUI->baseWindowWidth / 2) - (350 / 2), (createUI->baseWindowWidth / 3), 350, 50);
		hbutton3 = createUI->CreateButton(hWnd, BUTTON3_ID, L"Reveler le message", (createUI->baseWindowWidth / 2) - (createUI->buttonWidth / 2), (createUI->baseWindowWidth / 2.4), createUI->buttonWidth, createUI->buttonHeight);

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

			MoveWindow(hbutton1, (width / 2) - (createUI->buttonWidth / 2), (height / 8), createUI->buttonWidth, createUI->buttonHeight, TRUE);
			MoveWindow(hStatic1, (width / 2) + (200 / 2), (height / 5), 25, 15, TRUE);
			MoveWindow(hEdit, (width / 2) - (250 / 2), (height / 4.5), 250, 50, TRUE);
			MoveWindow(hbutton2, (width / 2) - (350 / 2), (height / 3), 350, 50, TRUE);
			MoveWindow(hbutton3, (width / 2) - (createUI->buttonWidth / 2), (height / 2.4), createUI->buttonWidth, createUI->buttonHeight, TRUE);
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
			loadingHelper = new LoadingHelper();

			if (!loadingHelper->OpenImageFile(hWnd)) 
			{
				delete loadingHelper; 
				loadingHelper = nullptr;
			}
			else {
				createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper->m_currentImage);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case BUTTON2_ID:
		{
			//NE PAS FERMER IMAGE POUR QUE CA PASSE
			GetWindowText(hEdit, bufferMessage, 255);
			steno->LSBEncode(loadingHelper->m_currentImage->m_bitMap,bufferMessage);
			createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture_Encrypte", PictureWndProc, loadingHelper->m_currentImage);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case BUTTON3_ID:
		{
			std::string newMessage = steno->LSBDecode(loadingHelper->m_currentImage->m_bitMap);
			//SetWindowText(hStatic1, newMessage);
			//Faire truc
			break;
		}
		}
		break;
	}

	case WM_DESTROY:

		delete loadingHelper; // Nettoyer le chargeur d'image
		loadingHelper = nullptr;
		delete createUI; // Nettoyer le chargeur d'image
		createUI = nullptr;
		delete steno; // Nettoyer le chargeur d'image
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

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (imageHelper) 
        {
            imageHelper->Draw(hdc, 0, 0); 
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:

        delete imageHelper; 
        imageHelper = nullptr;
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