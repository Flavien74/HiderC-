#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include "CreateUI.h"
#include "ImageHelper.h"

#include <vector>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CreateUI createUI;
	createUI.CreateAWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit, hStatic, hStatic2;
    static bool isTextCleared = false;

    LoadingHelper* loadingHelper = nullptr;
	CreateUI createUI;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message)
	{
	case WM_CREATE:
		createUI.CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", 650, 50, 150, 30);

		hStatic = createUI.CreateTextZone(hWnd, TEXT_ID, L"", 825, 130, 25, 15);
		hEdit = createUI.CreateInput(hWnd, EDIT_ID, L"Message a cacher", 600, 150, 250, 50);

		createUI.CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", 550, 250, 350, 50);
		createUI.CreateButton(hWnd, BUTTON3_ID, L"Reveler le message", 650, 330, 150, 30);

		hStatic2 = createUI.CreateTextZone(hWnd, TEXT2_ID,L"", 600, 400, 200, 20);
		createUI.CreateTextZone(hWnd, TEXT3_ID,L"Message cache : ", 600, 380, 125, 20);
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
				swprintf(bufferMessage, 50, L"%d", nbCharacterPossible);

				SetWindowText(hStatic, bufferMessage);
			}
			break;
		}
		case BUTTON1_ID:
		{
			if (loadingHelper) {
				delete loadingHelper;
			}
			loadingHelper = new LoadingHelper();

			if (!loadingHelper->OpenImageFile(hWnd)) 
			{
				delete loadingHelper; 
				loadingHelper = nullptr;
			}
			else {
				createUI.CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper->m_currentImage);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case BUTTON2_ID:
		{
			//Faire truc
			break;
		}
		}
		break;
	}

	case WM_DESTROY:

		delete loadingHelper; // Nettoyer le chargeur d'image
		loadingHelper = nullptr;
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