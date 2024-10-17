#include "CreateUI.h"
#include "ImageHelper.h"
#include "TransformUI.h"

CreateUI::CreateUI(HINSTANCE _hInstance)
{
	hInstance = _hInstance;
}

CreateUI::~CreateUI()
{
}

HWND CreateUI::CreateButton(HWND hWnd, int button_id, LPCWSTR message, TransformUI* transformPtr)
{
	// Get button position and size from the TransformUI object
	int x = transformPtr->getPositionX();
	int y = transformPtr->getPositionY();
	int width = transformPtr->getWidth();
	int height = transformPtr->getHeight();

	// Create the button control
	HWND hwndButton = CreateWindow(
		L"BUTTON",               // Predefined class for buttons
		message,                 // Button text (label)
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // Styles for visibility, child, and push button
		x,                       // X position
		y,                       // Y position
		width,                   // Button width
		height,                  // Button height
		hWnd,                    // Parent window
		(HMENU)button_id,        // Button ID for handling events
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Instance handle
		NULL                     // No additional data
	);

	// Check if button creation succeeded
	if (hwndButton == NULL)
	{
		MessageBox(NULL, L"Button Creation Failed!", L"Error", MB_OK | MB_ICONERROR);
		return NULL;
	}

	return hwndButton; // Return handle to the button
}

HWND CreateUI::CreateInput(HWND hWnd, int input_id, LPCWSTR message, TransformUI* transform)
{
	HWND hwnd = CreateWindow(
		L"EDIT",
		message,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		transform->getPositionX(),
		transform->getPositionY(),
		transform->getWidth(),
		transform->getHeight(),
		hWnd,              
		(HMENU)input_id,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);

	return hwnd;
}

HWND CreateUI::CreateBaseWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR ClassName, LPCWSTR WindowName, WNDPROC func)
{
	HBRUSH hBrushBG = CreateSolidBrush(RGB(173, 216, 230));

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = func;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;
	wc.hbrBackground = hBrushBG;

	RegisterClass(&wc);

	LONG longueur = m_transformWindow->getWidth();
	LONG largeur = m_transformWindow->getHeight();

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth / 2) - (m_transformWindow->getWidth() / 2);
	int posY = (screenHeight / 2) - (m_transformWindow->getHeight() / 2);

	HWND hWnd = CreateWindowEx(
		0,
		ClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, m_transformWindow->getWidth(), m_transformWindow->getHeight(),
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

HWND CreateUI::CreateAWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR ClassName, LPCWSTR WindowName, WNDPROC func, ImageHelper* helper)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = func;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;

	RegisterClass(&wc);

	LONG longueur = helper == nullptr ? CW_USEDEFAULT : helper->m_bitMap->GetWidth();
	LONG largeur = helper == nullptr ? CW_USEDEFAULT : helper->m_bitMap->GetHeight();

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

HWND CreateUI::CreateTextZone(HWND hWnd, int input_id, LPCWSTR message, TransformUI* transform, long Align)
{
	HWND hwnd = CreateWindow(
		L"STATIC",
		message,
		WS_CHILD | WS_VISIBLE | Align,
		transform->getPositionX(),
		transform->getPositionY(),
		transform->getWidth(),
		transform->getHeight(),
		hWnd,
		(HMENU)input_id,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);

	return hwnd;
}
