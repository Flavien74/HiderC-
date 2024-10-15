#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include "Stenography.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CreateAWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	static bool isTextCleared = false;
	static LoadingHelper* loadingHelper = nullptr;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message)
	{
	case WM_CREATE:
	{
		CreateButton(hWnd, BUTTON1_ID, L"Charger une image", 650, 50, 150, 50);
		CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", 550, 250, 350, 50);
		hEdit = CreateInput(hWnd, EDIT_ID, L"Message a cacher", 600, 150, 250, 50);
		break;
	}

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
			break;
		}
		case BUTTON1_ID: // Gérer le chargement d'une image si le bouton est cliqué
		{
			if (loadingHelper) {
				delete loadingHelper; // Nettoyer l'ancienne instance
			}
			loadingHelper = new LoadingHelper();

			// Appel à la fonction pour ouvrir un fichier
			if (!loadingHelper->OpenImageFile(hWnd, loadingHelper)) {
				delete loadingHelper; // Nettoyage si le chargement échoue
				loadingHelper = nullptr; // Eviter d'utiliser un pointeur nul
			}
			InvalidateRect(hWnd, NULL, TRUE); // Demande un nouveau dessin après le chargement
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

void CreateButton(HWND hWnd, int button_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
{
	HICON hIcon = (HICON)LoadImage(NULL, L"picture.ico", IMAGE_ICON, 5, 5, LR_LOADFROMFILE);

	//if (hIcon == NULL)
	//{
	//    MessageBox(hWnd, L"Erreur : l'icône n'a pas pu être chargée.", L"Erreur de chargement", MB_ICONERROR);
	//}

	HWND hButton = CreateWindow(
		L"BUTTON",  // Précise le type de la fenêtre
		message, // Texte du bouton
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
		posX,         // Position X
		posY,         // Position Y
		largeur,        // Largeur
		longueur,         // Hauteur
		hWnd,       // Handle de la fenêtre parente
		(HMENU)button_id, // Identifiant du bouton
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pas de données supplémentaires

	SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);
}

HWND CreateInput(HWND hWnd, int input_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
{
	return CreateWindow(
		L"EDIT",            // Type de contrôle
		message,                // Texte initial
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, // Styles
		posX,             // Position X
		posY,             // Position Y
		largeur,          // Largeur
		longueur,         // Hauteur
		hWnd,              // Handle de la fenêtre parente
		(HMENU)input_id,    // Identifiant du champ d'édition
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);             // Pas de données supplémentaires
}

void CreateAWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR ClassName, LPCWSTR WindowName)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;

	RegisterClass(&wc);

	HWND hWnd = CreateWindowEx(
		0,
		ClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
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

