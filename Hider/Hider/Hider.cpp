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
	loadingHelper = new LoadingHelper();
	UIObject = new std::vector<ObjectUI>();

	firstWindow = createUI->CreateBaseWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND buttonLoadCase, buttonSteno, ButtonReveal, hEdit, TextCharRestant, TextMessageLabel, TextMessageReturn;

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
		ObjectUI object;
		TransformUI transformBase;

		hBrushTransparent = (HBRUSH)GetStockObject(HOLLOW_BRUSH);  // Transparent background

		transformBase = TransformUI(0, 0, createUI->m_transformWindow->getWidth() / 2, 50, 0, 0);
		buttonLoadCase = createUI->CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", &transformBase);
		if (buttonLoadCase != NULL) {
			object = ObjectUI(BUTTON1_ID, "Btn1", transformBase, &buttonLoadCase);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create buttonLoadCase!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(createUI->m_transformWindow->getWidth() / 2, transformBase.getPositionY());
		TextCharRestant = createUI->CreateTextZone(hWnd, TEXT_ID, L"", &transformBase, ES_LEFT);
		if (TextCharRestant != NULL) {
			object = ObjectUI(TEXT_ID, "Text1", transformBase, &TextCharRestant);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create TextCharRestant!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(0, transformBase.getPositionY() + transformBase.getHeight());
		buttonSteno = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier un message", &transformBase);
		if (buttonSteno != NULL) {
			object = ObjectUI(BUTTON2_ID, "Btn2", transformBase, &buttonSteno);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create buttonSteno!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(createUI->m_transformWindow->getWidth() / 2, transformBase.getPositionY());
		hEdit = createUI->CreateInput(hWnd, EDIT_ID, L"Message a cacher", &transformBase);
		if (hEdit != NULL) {
			object = ObjectUI(EDIT_ID, "Edit", transformBase, &hEdit);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create hEdit input box!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(0, transformBase.getPositionY() + transformBase.getHeight());
		ButtonReveal = createUI->CreateButton(hWnd, BUTTON3_ID, L"Reveler un message", &transformBase);
		if (ButtonReveal != NULL) {
			object = ObjectUI(BUTTON3_ID, "Btn3", transformBase, &ButtonReveal);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create ButtonReveal!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(createUI->m_transformWindow->getWidth() / 2, transformBase.getPositionY());
		TextMessageLabel = createUI->CreateTextZone(hWnd, TEXT3_ID, L"Message cache : ", &transformBase, ES_LEFT);
		if (TextMessageLabel != NULL) {
			object = ObjectUI(TEXT3_ID, "Text3", transformBase, &TextMessageLabel);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create TextMessageLabel!", L"Error", MB_OK | MB_ICONERROR);
		}

		transformBase.setPosition(0, transformBase.getPositionY() + transformBase.getHeight());
		transformBase.setSize(createUI->m_transformWindow->getWidth(), transformBase.getHeight());

		TextMessageReturn = createUI->CreateTextZone(hWnd, TEXT2_ID, L"", &transformBase, ES_LEFT);
		if (TextMessageReturn != NULL) {
			object = ObjectUI(TEXT2_ID, "Text2", transformBase, &TextMessageReturn);
			UIObject->push_back(object);
		}
		else {
			MessageBox(hWnd, L"Failed to create TextMessageReturn!", L"Error", MB_OK | MB_ICONERROR);
		}
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
		if (LOWORD(lParam) < (createUI->m_transformWindow->getWidth() - 50))
		{
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			int posY = (screenHeight / 2) - (createUI->m_transformWindow->getHeight() / 2);
			int posX = (screenWidth / 2) - (createUI->m_transformWindow->getWidth() / 2);

			MoveWindow(firstWindow, posX, posY, createUI->m_transformWindow->getWidth() - 50, createUI->m_transformWindow->getHeight(), TRUE);
		}
		else if (HIWORD(lParam) < (createUI->m_transformWindow->getWidth() - 50))
		{
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			int posY = (screenHeight / 2) - (createUI->m_transformWindow->getHeight() / 2);
			int posX = (screenWidth / 2) - (createUI->m_transformWindow->getWidth() / 2);

			MoveWindow(firstWindow, posX, posY, createUI->m_transformWindow->getWidth(), createUI->m_transformWindow->getHeight() - 50, TRUE);
		}
		else {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			ResizeWindow();
		}
		InvalidateRect(hWnd, NULL, TRUE);
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
			if (ReturnIndexObject(EDIT_ID) == -1 || ReturnIndexObject(TEXT_ID) == -1) 
			{
				break;
			}
			
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
					SetWindowText(TextCharRestant, L"");
					InvalidateRect(hWnd, NULL, TRUE);
					SetWindowText(TextCharRestant, buffernumber);
				}
			}
			break;
		}
		case BUTTON1_ID:
		{
			DestroyLoadingHelper(hWnd);
			loadingHelper = new LoadingHelper();

			if (loadingHelper->Init(hWnd)) {
				return true;
			}
			else {
				DestroyLoadingHelper(hWnd);
				return false;
			}

			nbCharacterPossible = loadingHelper->m_currentImage->m_bitMap->GetHeight() * loadingHelper->m_currentImage->m_bitMap->GetWidth() * 3;
			swprintf(buffernumber, nbCharacterPossible, L"%d", nbCharacterPossible);

			SetWindowText(TextCharRestant, buffernumber);

			//createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper->m_currentImage);
			InvalidateRect(hWnd, NULL, TRUE);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
			break;
		}
		case BUTTON2_ID:
		{
			GetWindowText(hEdit, bufferMessage, 255);
			if (bufferMessage[0] == L'\0') {
				MessageBox(hWnd, L"Ecris un message à cacher dans l'image !", L"message missing", MB_ICONERROR | MB_OK);
				break;
			}
			if (!loadingHelper || !loadingHelper->m_currentImage) {
				MessageBox(hWnd, L"Choisis un fichier comme image !", L"iamge missing", MB_ICONERROR | MB_OK);
				break;
			}

			steno->LSBEncode(loadingHelper->m_currentImage->m_bitMap, bufferMessage);

			loadingHelper->SaveImage(loadingHelper->m_currentExtension->GetCompletePath(L"_out"));

			//createUI->CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture_Encrypte", PictureWndProc, loadingHelper->m_currentImage);
			DestroyLoadingHelper(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
			break;
		}
		case BUTTON3_ID:
		{
			if (!loadingHelper || !loadingHelper->m_currentImage) {
				MessageBox(hWnd, L"Choisis un fichier comme image !", L"iamge missing", MB_ICONERROR | MB_OK);
				break;
			}

			std::wstring newMessage = steno->LSBDecode(loadingHelper->m_currentImage->m_bitMap);

			LPCWSTR lpcwstr = newMessage.c_str();

			SetWindowText(TextMessageReturn, L"");
			InvalidateRect(hWnd, NULL, TRUE);

			SetWindowText(TextMessageReturn, lpcwstr);
			DestroyLoadingHelper(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);

			break;
		}
		case 1001:  // ID of the accelerator (Ctrl+O)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON1_ID);
			if (hButton)
			{
				PostMessage(hButton, BM_CLICK, 0, 0);
			}
			break;
		}
		case 1002:  // ID of the accelerator (Ctrl+O)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON2_ID);
			if (hButton)
			{
				PostMessage(hButton, BM_CLICK, 0, 0);
			}
			break;
		}
		case 1003:  // ID of the accelerator (Ctrl+O)
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
	case WM_PAINT:
	{
		InvalidateRect(hWnd, NULL, TRUE); // NULL pour redessiner toute la fenêtre, TRUE pour effacer l'arrière-plan

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		int windowWidth = clientRect.right - clientRect.left;
		int windowHeight = clientRect.bottom - clientRect.top;

		Graphics graphics(hdc);

		int y = (*UIObject)[(*UIObject).size() - 1].m_transform.getPositionY() +
			(*UIObject)[(*UIObject).size() - 1].m_transform.getHeight();

		if (loadingHelper && loadingHelper->m_currentImage && loadingHelper->m_currentImage->m_bitMap)
		{
			int imgWidth = loadingHelper->m_currentImage->m_bitMap->GetWidth();
			int imgHeight = loadingHelper->m_currentImage->m_bitMap->GetHeight();

			float aspectRatio = (float)imgWidth / (float)imgHeight;

			int newWidth = windowWidth;
			int newHeight = static_cast<int>(newWidth / aspectRatio);

			if (y + newHeight > windowHeight) {
				newHeight = windowHeight - y;
				newWidth = static_cast<int>(newHeight * aspectRatio);
			}

			Graphics* g = Graphics::FromHDC(hdc);
			g->DrawImage(loadingHelper->m_currentImage->m_bitMap, 0, y, newWidth, newHeight);
			delete g;
		}
		EndPaint(hWnd, &ps);
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

//LRESULT CALLBACK PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		if (!loadingHelper) 
//		{
//			break;
//		}
//		int x, y = 0;
//		HDC hdc = BeginPaint(hWnd, &ps);
//		
//		RECT clientRect;
//		GetClientRect(hWnd, &clientRect);
//		int windowWidth = clientRect.right - clientRect.left;
//		int windowHeight = clientRect.bottom - clientRect.top;
//		
//		HDC hdcMem = CreateCompatibleDC(hdc);
//		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
//		HGDIOBJ hOldBitmap = SelectObject(hdcMem, hbmMem);
//
//		HBRUSH hBrush = (HBRUSH)(COLOR_WINDOW + 1);
//		FillRect(hdcMem, &clientRect, hBrush);
//
//		if (loadingHelper->m_currentImage && loadingHelper->m_currentImage->m_bitMap)
//		{
//			int imgWidth = loadingHelper->m_currentImage->m_bitMap->GetWidth();
//			int imgHeight = loadingHelper->m_currentImage->m_bitMap->GetHeight();
//
//			float aspectRatio = (float)imgWidth / (float)imgHeight;
//
//			int newWidth = windowWidth;
//			int newHeight = static_cast<int>(newWidth / aspectRatio);
//
//			if (newHeight > windowHeight) {
//				newHeight = windowHeight;
//				newWidth = static_cast<int>(newHeight * aspectRatio);
//			}
//
//			x = (windowWidth - newWidth) / 2;
//			y = (windowHeight - newHeight) / 2;
//
//			loadingHelper->m_currentImage->Draw(hdcMem, x, y, newWidth, newHeight);
//		}
//
//		BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);
//
//		SelectObject(hdcMem, hOldBitmap);
//		DeleteObject(hbmMem);
//		DeleteDC(hdcMem);
//
//		EndPaint(hWnd, &ps);
//
//		InvalidateRect(hWnd, NULL, TRUE);
//		break;
//	}
//	case WM_SIZE:
//	{
//		InvalidateRect(hWnd, NULL, TRUE);
//		break;
//	}
//	case WM_DESTROY:
//		DestroyLoadingHelper(hWnd);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}

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


void ResizeWindow()
{
	ObjectUI objectUI;
	objectUI = (*UIObject)[ReturnIndexObject(BUTTON1_ID)];
	MoveWindow(*objectUI.m_hwnd, 0, objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(BUTTON2_ID)];
	MoveWindow(*objectUI.m_hwnd, 0, objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(EDIT_ID)];
	MoveWindow(*objectUI.m_hwnd, objectUI.m_transform.getPositionX(), objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(TEXT_ID)];
	MoveWindow(*objectUI.m_hwnd, objectUI.m_transform.getPositionX(), objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(BUTTON3_ID)];
	MoveWindow(*objectUI.m_hwnd, 0, objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(TEXT3_ID)];
	MoveWindow(*objectUI.m_hwnd, objectUI.m_transform.getPositionX(), objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);

	objectUI = (*UIObject)[ReturnIndexObject(TEXT2_ID)];
	MoveWindow(*objectUI.m_hwnd, objectUI.m_transform.getPositionX(), objectUI.m_transform.getPositionY(), objectUI.m_transform.getWidth(), objectUI.m_transform.getHeight(), TRUE);
}

int ReturnIndexObject(int id)
{
	for (int i = 0; i < UIObject->size(); i++) 
	{
		if ((*UIObject)[i].m_id == id) {
			return i;
		}
	}
	return -1;
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
