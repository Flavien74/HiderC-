#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include "CreateUI.h"
#include "ImageHelper.h"
#include "ExtensionHelper.h"
#include "LoadingHelper.h"
#include "Stenography.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	createUI = new CreateUI(hInstance);
	steno = new Stenography();
	loadingHelperDecrytpe = new LoadingHelper();
	loadingHelperEncrypte = new LoadingHelper();
	UIObject = new std::vector<ObjectUI*>();

	firstWindow = createUI->CreateBaseWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

	MSG msg;
	HACCEL hAccelTable = CreateAcceleratorTable(accelerators, sizeof(accelerators) / sizeof(ACCEL));

	while (GetMessage(&msg, nullptr, 0, 0)) {
		// Translate the accelerator keys
		if (!TranslateAccelerator(firstWindow, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isEditing = false;
	HWND hStatic = (HWND)lParam;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	switch (message)
	{
	case WM_CREATE:
		InitializeUI(&hWnd);  // Move UI creation to a separate function
		break;

	case WM_CTLCOLORSTATIC:
		if (hStatic == TextCharRestant) {
			return (INT_PTR)HandleCtlColorStatic(wParam, lParam, hBrushTransparent, hStatic);
		}
		break;

	case WM_SIZE:
		HandleResize(&hWnd, lParam);  // Handle resizing logic
		break;

	case WM_COMMAND:
		HandleCommand(&hWnd, wParam, lParam);  // Handle button clicks and text editing logic
		break;

	case WM_PAINT:
		HandleWM_PAINT(&hWnd);  // Handle painting logic
		break;

	case WM_DESTROY:
		Cleanup(&hWnd, gdiplusToken);  // Cleanup resources on destroy
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

INT_PTR CALLBACK About(HWND* hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(*hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void InitializeUI(HWND* hWnd)
{
	ObjectUI* object;
	TransformUI transformBase;

	transformBase = TransformUI(2, 10, (createUI->m_transformWindow->getWidth() / 2) - 2, 50, 0, 0);
	buttonLoadCase = createUI->CreateButton(*hWnd, BUTTON_LOAD_1, L"Ctrl+L : Choisir un fichier", &transformBase);
	object = new ObjectUI(BUTTON_LOAD_1, "BtnDecrypte1", transformBase, &buttonLoadCase);
	UIObject->push_back(object);

	transformBase = TransformUI((createUI->m_transformWindow->getWidth() / 2) + 1, 10, (createUI->m_transformWindow->getWidth() / 2) - 2, 50, 0, 0);
	buttonLoadCase2 = createUI->CreateButton(*hWnd, BUTTON_LOAD_2, L"Choisir un fichier", &transformBase);
	object = new ObjectUI(BUTTON_LOAD_2, "BtnEncrypte1", transformBase, &buttonLoadCase2);
	UIObject->push_back(object);

	transformBase = TransformUI((createUI->m_transformWindow->getWidth() / 2) - (createUI->m_transformWindow->getWidth() / 4), createUI->m_transformWindow->getHeight() - 220, createUI->m_transformWindow->getWidth() / 2, 30, 0, 0);
	buttonClear = createUI->CreateButton(*hWnd, CLEAR_IMAGE, L"Nettoyer les images", &transformBase);
	object = new ObjectUI(CLEAR_IMAGE, "Clear", transformBase, &buttonClear);
	UIObject->push_back(object);

	transformBase = TransformUI(2, createUI->m_transformWindow->getHeight() - 140, (createUI->m_transformWindow->getWidth() / 2) - 2, 20, 0, 0);
	TextCharRestant = createUI->CreateTextZone(*hWnd, TEXT_RESTANT_1, L"", &transformBase, ES_RIGHT);
	object = new ObjectUI(TEXT_RESTANT_1, "TextRestant", transformBase, &TextCharRestant);
	UIObject->push_back(object);

	transformBase = TransformUI(2, createUI->m_transformWindow->getHeight() - 120, (createUI->m_transformWindow->getWidth() / 2) - 2, 110, 0, 0);
	hEdit = createUI->CreateInput(*hWnd, EDIT_ID_1, L"Message a cacher :", &transformBase);
	object = new ObjectUI(EDIT_ID_1, "Edit", transformBase, &hEdit);
	UIObject->push_back(object);

	transformBase = TransformUI(2, createUI->m_transformWindow->getHeight() - 180, (createUI->m_transformWindow->getWidth() / 2) - 2, 40, 0, 0);
	buttonSteno = createUI->CreateButton(*hWnd, BUTTON_STENO, L"Ctrl+D : Stenographier un message", &transformBase);
	object = new ObjectUI(BUTTON_STENO, "BtnSteno", transformBase, &buttonSteno);
	UIObject->push_back(object);

	transformBase = TransformUI((createUI->m_transformWindow->getWidth() / 2) + 1, createUI->m_transformWindow->getHeight() - 180, (createUI->m_transformWindow->getWidth() / 2) - 2, 40, 0, 0);
	ButtonReveal = createUI->CreateButton(*hWnd, BUTTON_REVEAL, L"Ctrl+E : Reveler un message", &transformBase);
	object = new ObjectUI(BUTTON_REVEAL, "BtnReveal", transformBase, &ButtonReveal);
	UIObject->push_back(object);

	transformBase = TransformUI((createUI->m_transformWindow->getWidth() / 2) + 1, createUI->m_transformWindow->getHeight() - 120, (createUI->m_transformWindow->getWidth() / 2) - 2, 110, 0, 0);
	hEdit2 = createUI->CreateTextZone(*hWnd, EDIT_ID_2, L"Message retourne :", &transformBase, ES_LEFT);
	object = new ObjectUI(EDIT_ID_2, "TextReturn", transformBase, &hEdit2);
	UIObject->push_back(object);
}


void HandleCommand(HWND* hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case EDIT_ID_1:
		HandleTextEdit(hWnd, wParam, lParam);  // Text editing logic for EDIT_ID_1
		break;

	case BUTTON_LOAD_1:
		HandleLoadButton1(hWnd);  // Handle loading a file (Button 1)
		break;

	case BUTTON_LOAD_2:
		HandleLoadButton2(hWnd);  // Handle loading a file (Button 2)
		break;

	case BUTTON_STENO:
		HandleStenography(hWnd, &hEdit, bufferMessage);
		break;

	case BUTTON_REVEAL:
		HandleRevealMessage(hWnd, &hEdit2);
		break;

	case CLEAR_IMAGE:
		HandleClearImage(hWnd, &hEdit, &hEdit2);
		break;

	case 1001:  // Ctrl+L accelerator
	case 1002:  // Ctrl+D accelerator
	case 1003:  // Ctrl+E accelerator
		HandleAccelerators(hWnd, LOWORD(wParam));  // Handle Ctrl+L/D/E shortcuts
		break;

	default:
		break;
	}
}


HBRUSH HandleCtlColorStatic(WPARAM wParam, LPARAM lParam, HBRUSH hBrushTransparent,HWND hStatic) {
	HDC hdcStatic = (HDC)wParam;
	
		SetBkMode(hdcStatic, TRANSPARENT);

		if (GetDlgCtrlID(hStatic) == TEXT_RESTANT_1) {
			COLORREF textColor = RGB(255, 0, 0); // Set text color to red
			SetTextColor(hdcStatic, textColor);
		}

	return hBrushTransparent;
}


void HandleStenography(HWND* hWnd, HWND* hEdit, wchar_t* bufferMessage) {

	if (!loadingHelperDecrytpe || !loadingHelperDecrytpe->m_currentImage) {
		MessageBox(*hWnd, L"Choisis un fichier comme image avant de la stenographier !", L"Missing image", MB_ICONERROR | MB_OK);
		HandleLoadButton1(hWnd);
		return;
	}

	GetWindowText(*hEdit, bufferMessage, 255);
	if (bufferMessage[0] == L'\0') {
		MessageBox(*hWnd, L"Ecris un message a cacher dans l'image avant de la stenographier !", L"Missing image", MB_ICONERROR | MB_OK);
		return;
	}

	steno->LSBEncode(loadingHelperDecrytpe->m_currentImage->m_bitMap, bufferMessage);

	if (loadingHelperDecrytpe->SaveImage(loadingHelperDecrytpe->m_currentExtension->GetCompletePath(L"_out"))) {
		MessageBox(*hWnd, L"Fichier bien enregistre !", L"Saving image", MB_ICONINFORMATION | MB_OK);
	}

	if (!loadingHelperEncrypte) {
		loadingHelperEncrypte = new LoadingHelper();
	}
	loadingHelperEncrypte = loadingHelperDecrytpe;
	loadingHelperDecrytpe = nullptr;

	SetWindowText(GetDlgItem(*hWnd, TEXT_RESTANT_1), L"\0");
	SetWindowText(*hEdit, L"Message a cacher :");
	isTextCleared = false;

	InvalidateRect(*hWnd, NULL, TRUE);
}

void HandleRevealMessage(HWND* hWnd, HWND* hEdit2) {
	if (!loadingHelperEncrypte || !loadingHelperEncrypte->m_currentImage) {
		MessageBox(*hWnd, L"Choisis un fichier steganographie comme image !", L"Missing image", MB_ICONERROR | MB_OK);
		HandleLoadButton2(hWnd);
		return;
	}

	std::wstring newMessage = steno->LSBDecode(loadingHelperEncrypte->m_currentImage->m_bitMap);
	if (!newMessage.empty()) 
	{
		LPCWSTR lpcwstr = newMessage.c_str();
		SetWindowText(*hEdit2, lpcwstr); // Display the revealed message
	}
	else
	{
		SetWindowText(*hEdit2, L"Aucun message dans l'image."); // Display the revealed message
	}

	SetWindowText(GetDlgItem(*hWnd, TEXT_RESTANT_1), L"\0"); // Clear character count
	InvalidateRect(*hWnd, NULL, TRUE);
}


void HandleClearImage(HWND* hWnd, HWND* hEdit, HWND* hEdit2) 
{

	SetWindowText(*hEdit2, L"\0");
	SetWindowText(GetDlgItem(*hWnd, TEXT_RESTANT_1), L"\0");
	nbCharacterPossible = 0;
	SetWindowText(*hEdit, L"Message a cacher :");
	isTextCleared = false;

	InvalidateRect(*hWnd, NULL, TRUE);

	if (!loadingHelperEncrypte && !loadingHelperDecrytpe) {
		MessageBox(*hWnd, L"Toutes les images sont nettoyer !", L"image missing", MB_ICONERROR | MB_OK);
		return;
	}

	DestroyLoadingHelper(hWnd);
	DestroyLoadingHelper(hWnd, true);

	InvalidateRect(*hWnd, NULL, TRUE);
}


void HandleAccelerators(HWND* hWnd, WPARAM wParam) {
	HWND hButton = nullptr;

	switch (LOWORD(wParam)) {
	case 1001:  // Ctrl+L
		hButton = GetDlgItem(*hWnd, BUTTON_LOAD_1);
		break;
	case 1002:  // Ctrl+D
		hButton = GetDlgItem(*hWnd, BUTTON_STENO);
		break;
	case 1003:  // Ctrl+E
		hButton = GetDlgItem(*hWnd, BUTTON_REVEAL);
		break;
	}

	if (hButton) {
		PostMessage(hButton, BM_CLICK, 0, 0);
	}
	InvalidateRect(*hWnd, NULL, TRUE);
}


void HandleTextEdit(HWND* hWnd, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == EN_SETFOCUS && !isTextCleared)
	{
		SetWindowText(hEdit, TEXT(""));
		isTextCleared = true;
	}

	if (HIWORD(wParam) == EN_CHANGE)
	{
		if (isFirstChange)
		{
			isFirstChange = false; // Ignore the first change
			return;
		}

		UpdateTextRemaining(hWnd, &hEdit, TextCharRestant, nbCharacterPossible, nbLastCharacter);
	}
}

void UpdateTextRemaining(HWND* hWnd, HWND* hEdit, HWND TextCharRestant, int& nbCharacterPossible, int& nbLastCharacter)
{
	int nbCurrentCharacter = GetWindowTextLength(*hEdit);
	wchar_t buffernumber[10];

	//if ((!loadingHelperDecrytpe || !loadingHelperDecrytpe->m_currentImage) && nbCurrentCharacter == 0) {
	//	MessageBox(*hWnd, L"Choisis un fichier comme image avant d'ecrire ton texte !", L"Missing image", MB_ICONERROR | MB_OK);
	//	SetWindowText(*hEdit, L"Message a cacher :");
	//	isTextCleared = false;
	//	return;
	//}

	if (nbCurrentCharacter > nbLastCharacter) {

		if (nbCharacterPossible > 0) {

			nbCharacterPossible--;
		}
		else {
			SendMessage(*hEdit, EM_SETLIMITTEXT, (WPARAM)1, 0);
		}

		InvalidateRect(*hWnd, NULL, TRUE);
	}
	else if (nbCurrentCharacter < nbLastCharacter) {
		nbCharacterPossible++;
	}

	nbLastCharacter = nbCurrentCharacter;

	swprintf(buffernumber, 10, L"%d", nbCharacterPossible);
	SetWindowText(TextCharRestant, buffernumber);
	SendMessage(*hEdit, EM_SETLIMITTEXT, (WPARAM)nbCharacterPossible, 0); // Définir la limite de texte
}


void HandleLoadButton1(HWND* hWnd)
{
	DestroyLoadingHelper(hWnd);
	loadingHelperDecrytpe = new LoadingHelper();

	if (loadingHelperDecrytpe->Init(*hWnd, L"Sélectionnez une image"))
	{
		nbCharacterPossible = loadingHelperDecrytpe->m_currentImage->m_bitMap->GetHeight() * loadingHelperDecrytpe->m_currentImage->m_bitMap->GetWidth() * 3;
		swprintf(buffernumber, nbCharacterPossible, L"%d", nbCharacterPossible);

		SetWindowText(hEdit2, TEXT(""));
		SetWindowText(TextCharRestant, buffernumber);
		InvalidateRect(*hWnd, NULL, TRUE);
	}
	else
	{
		DestroyLoadingHelper(hWnd);
	}
}

void HandleLoadButton2(HWND* hWnd)
{
	DestroyLoadingHelper(hWnd, true);
	loadingHelperEncrypte = new LoadingHelper();

	if (loadingHelperEncrypte->Init(*hWnd, L"Sélectionnez une image steganographiee"))
	{
		InvalidateRect(*hWnd, NULL, TRUE);
	}
	else
	{
		DestroyLoadingHelper(hWnd, true);
	}
}

void HandleWM_PAINT(HWND* hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(*hWnd, &ps);
	CreateMemoryDC(hWnd);

	FillRect(hdcMem, &ps.rcPaint, hBrushTransparent);

	int ymin = (*UIObject)[ReturnIndexObject(BUTTON_LOAD_1)]->m_transformResize.getPositionY() +
		(*UIObject)[ReturnIndexObject(BUTTON_LOAD_1)]->m_transformResize.getHeight();
	int ymax = (*UIObject)[ReturnIndexObject(CLEAR_IMAGE)]->m_transformResize.getPositionY();
	int availableHeight = ymax - ymin;

	int imageAreaWidth = (ps.rcPaint.right - ps.rcPaint.left) / 2;

	if (loadingHelperDecrytpe && loadingHelperDecrytpe->m_currentImage && loadingHelperDecrytpe->m_currentImage->m_bitMap) {
		int xPosition1 = ((ps.rcPaint.right / 2) - imageAreaWidth) / 2;
		DrawScaledImage(hdcMem, loadingHelperDecrytpe->m_currentImage, xPosition1, ymin, imageAreaWidth, availableHeight);
	}

	if (loadingHelperEncrypte && loadingHelperEncrypte->m_currentImage && loadingHelperEncrypte->m_currentImage->m_bitMap) {
		int xPosition2 = ((ps.rcPaint.right / 2) + imageAreaWidth) / 2;
		DrawScaledImage(hdcMem, loadingHelperEncrypte->m_currentImage, xPosition2, ymin, imageAreaWidth, availableHeight);
	}

	BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
		hdcMem, 0, 0, SRCCOPY);

	CleanupMemoryDC();
	EndPaint(*hWnd, &ps);
}


void HandleResize(HWND* hWnd, LPARAM lParam)
{
	int width = LOWORD(lParam);
	int height = HIWORD(lParam);

	ResizeWindow(width, height);
	InvalidateRect(*hWnd, NULL, TRUE);
}

void ResizeWindow(int width, int height)
{
	for (size_t i = 0; i < (*UIObject).size(); i++)
	{
		ObjectUI* objectUI = (*UIObject)[i];
		float ratioX = objectUI->m_transform.getWidth() / createUI->m_transformWindow->getWidth();
		float ratioY = objectUI->m_transform.getHeight() / createUI->m_transformWindow->getHeight();

		float newX = objectUI->m_transform.getPositionX();
		float newY = objectUI->m_transform.getPositionY();

		if (newX != 0)
		{
			newX = (objectUI->m_transform.getPositionX() / createUI->m_transformWindow->getWidth()) * width;
		}
		if (newY != 0)
		{
			newY = (objectUI->m_transform.getPositionY() / createUI->m_transformWindow->getHeight()) * height;
		}

		objectUI->m_transformResize = TransformUI(newX, newY, ratioX * width, ratioY * height, 0, 0);
		MoveWindow(*objectUI->m_hwnd, newX, newY, ratioX * width, ratioY * height, TRUE);
	}
}


void Cleanup(HWND* hWnd, ULONG_PTR gdiplusToken)
{
	DestroyLoadingHelper(hWnd);
	DestroyLoadingHelper(hWnd, true);

	if (createUI) {
		delete createUI;
		createUI = nullptr;
	}
	if (steno) {
		delete steno;
		steno = nullptr;
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	PostQuitMessage(0);
}

void DestroyLoadingHelper(HWND* hWnd, bool isEncrypt)
{
	if (loadingHelperDecrytpe && !isEncrypt)
	{
		loadingHelperDecrytpe->~LoadingHelper();
		loadingHelperDecrytpe = nullptr;
	}
	if (loadingHelperEncrypte && isEncrypt)
	{
		loadingHelperEncrypte->~LoadingHelper();
		loadingHelperEncrypte = nullptr;
	}
}


void CreateMemoryDC(HWND* hWnd) {
	HDC hdc = GetDC(*hWnd);
	RECT clientRect;
	GetClientRect(*hWnd, &clientRect);
	int windowWidth = clientRect.right - clientRect.left;
	int windowHeight = clientRect.bottom - clientRect.top;

	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	SelectObject(hdcMem, hbmMem);

	ReleaseDC(*hWnd, hdc);
}

void DrawScaledImage(HDC hdc, ImageHelper* image, int x, int y, int maxWidth, int availableHeight) {
	if (!image || !image->m_bitMap) {
		return;
	}

	int imgWidth = image->m_bitMap->GetWidth();
	int imgHeight = image->m_bitMap->GetHeight();

	if (imgWidth <= 0 || imgHeight <= 0) {
		return;
	}

	float aspectRatio = static_cast<float>(imgWidth) / imgHeight;
	int newWidth = maxWidth;
	int newHeight = static_cast<int>(newWidth / aspectRatio);

	if (newHeight > availableHeight) {
		newHeight = availableHeight;
		newWidth = static_cast<int>(newHeight * aspectRatio);
		x = x + (maxWidth - newWidth) / 2;
	}
	y = y + ((availableHeight - newHeight) / 2);

	Gdiplus::Graphics graphics(hdc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.DrawImage(image->m_bitMap, x, y, newWidth, newHeight);
}

void CleanupMemoryDC() {
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}


int ReturnIndexObject(int id)
{
	for (int i = 0; i < UIObject->size(); i++)
	{
		if ((*UIObject)[i]->m_id == id) {
			return i;
		}
	}
	return -1;
}