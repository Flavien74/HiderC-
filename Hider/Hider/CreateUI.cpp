#include "CreateUI.h"
#include "ImageHelper.h"

CreateUI::CreateUI(HINSTANCE _hInstance)
{
	hInstance = _hInstance;
}

CreateUI::~CreateUI()
{
}

HWND CreateUI::CreateButton(HWND hWnd, int button_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
{
	/*HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BUTTON), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	if (hBitmap == NULL)
	{
		MessageBox(NULL, L"Erreur de chargement de l'image.", L"Erreur", MB_OK | MB_ICONERROR);
		return 0;
	}*/

	return CreateWindow(
		L"BUTTON",
		message,
		SS_LEFT | WS_VISIBLE | WS_CHILD,
		posX,
		posY,
		largeur,
		longueur,
		hWnd,
		(HMENU)button_id,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);
}

HWND CreateUI::CreateInput(HWND hWnd, int input_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
{
	return CreateWindow(
		L"EDIT",
		message,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		posX,
		posY,
		largeur,    
		longueur,
		hWnd,              
		(HMENU)input_id,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);
}

HWND CreateUI::CreateAWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR ClassName, LPCWSTR WindowName, WNDPROC func, ImageHelper* helper)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = func;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;

	RegisterClass(&wc);

	LONG longueur = helper == nullptr ? CW_USEDEFAULT : helper->m_bitMap.bmWidth;
	LONG largeur = helper == nullptr ? CW_USEDEFAULT : helper->m_bitMap.bmHeight;

	HWND hWnd = CreateWindowEx(
		0,
		ClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, longueur, largeur,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (helper != nullptr)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)helper);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

HWND CreateUI::CreateTextZone(HWND hWnd, int input_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
{
	return CreateWindow(
		L"STATIC",
		message,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		posX,
		posY,
		largeur,
		longueur,
		hWnd,
		(HMENU)input_id,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);
}
