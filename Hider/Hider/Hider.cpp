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

		hBrushTransparent = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		transformBase = TransformUI(0, 0, createUI->m_transformWindow->getWidth() / 2, 50, 0, 0);
		buttonLoadCase = createUI->CreateButton(hWnd, BUTTON_LOAD_1, L"Choisir un fichier", &transformBase);
		object = ObjectUI(BUTTON_LOAD_1, "Btn1", transformBase, &buttonLoadCase);
		UIObject->push_back(object);

		transformBase = TransformUI(createUI->m_transformWindow->getWidth() / 2, 0, createUI->m_transformWindow->getWidth() / 2, 50, 0, 0);
		buttonLoadCase2 = createUI->CreateButton(hWnd, BUTTON_LOAD_2, L"Choisir un fichier", &transformBase);
		object = ObjectUI(BUTTON_LOAD_2, "Btn1", transformBase, &buttonLoadCase2);
		UIObject->push_back(object);

		transformBase = TransformUI(0, createUI->m_transformWindow->getHeight() - 250, createUI->m_transformWindow->getWidth(), 30, 0, 0);
		TextCharRestant = createUI->CreateButton(hWnd, CLEAR_IMAGE, L"Clear Image", &transformBase);
		object = ObjectUI(CLEAR_IMAGE, "Clear", transformBase, &TextCharRestant);
		UIObject->push_back(object);

		transformBase = TransformUI(0, createUI->m_transformWindow->getHeight() - 140, createUI->m_transformWindow->getWidth(), 20, 0, 0);
		TextCharRestant = createUI->CreateTextZone(hWnd, TEXT_RESTANT_1, L"", &transformBase, ES_LEFT);
		object = ObjectUI(TEXT_RESTANT_1, "Text1", transformBase, &TextCharRestant);
		UIObject->push_back(object);

		transformBase = TransformUI(0, createUI->m_transformWindow->getHeight() - 120, createUI->m_transformWindow->getWidth() / 2, 110, 0, 0);
		hEdit = createUI->CreateInput(hWnd, EDIT_ID, L"Message a cacher", &transformBase);
		object = ObjectUI(EDIT_ID, "Edit", transformBase, &hEdit);
		UIObject->push_back(object);

		transformBase = TransformUI(0, createUI->m_transformWindow->getHeight() - 180, createUI->m_transformWindow->getWidth() / 2, 40, 0, 0);
		buttonSteno = createUI->CreateButton(hWnd, BUTTON2_ID, L"Stenographier un message", &transformBase);
		object = ObjectUI(BUTTON2_ID, "Btn2", transformBase, &buttonSteno);
		UIObject->push_back(object);

		transformBase = TransformUI(createUI->m_transformWindow->getWidth() / 2, createUI->m_transformWindow->getHeight() - 180, createUI->m_transformWindow->getWidth() / 2, 40, 0, 0);
		ButtonReveal = createUI->CreateButton(hWnd, BUTTON3_ID, L"Reveler un message", &transformBase);
		object = ObjectUI(BUTTON3_ID, "Btn3", transformBase, &ButtonReveal);
		UIObject->push_back(object);

		transformBase = TransformUI(createUI->m_transformWindow->getWidth() / 2, createUI->m_transformWindow->getHeight() - 75, createUI->m_transformWindow->getWidth() / 2, 25, 0, 0);
		TextMessageLabel = createUI->CreateTextZone(hWnd, TEXT3_ID, L"Message cache : ", &transformBase, ES_LEFT);
		object = ObjectUI(TEXT3_ID, "Text3", transformBase, &TextMessageLabel);
		UIObject->push_back(object);

		transformBase = TransformUI(createUI->m_transformWindow->getWidth() / 2, createUI->m_transformWindow->getHeight() - 50, createUI->m_transformWindow->getWidth() / 2, 50, 0, 0);
		TextMessageReturn = createUI->CreateTextZone(hWnd, TEXT2_ID, L"", &transformBase, ES_LEFT);
		object = ObjectUI(TEXT2_ID, "Text2", transformBase, &TextMessageReturn);
		UIObject->push_back(object);

		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		HWND hStatic = (HWND)lParam;

		SetBkMode(hdcStatic, TRANSPARENT);

		if (GetDlgCtrlID(hStatic) == TEXT_RESTANT_1) {
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
		else {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			ResizeWindow(width, height);
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
		switch (LOWORD(wParam))
		{
		case EDIT_ID:
		{
			if (ReturnIndexObject(EDIT_ID) == -1 || ReturnIndexObject(TEXT_RESTANT_1) == -1) 
			{
				break;
			}
			
			if (HIWORD(wParam) == EN_SETFOCUS && !isTextCleared)
			{
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
					SetWindowText(TextCharRestant, buffernumber);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;
		}
		case BUTTON_LOAD_1:
		{
			DestroyLoadingHelper(hWnd);
			loadingHelperDecrytpe = new LoadingHelper();

			if (loadingHelperDecrytpe->Init(hWnd)) {

				nbCharacterPossible = loadingHelperDecrytpe->m_currentImage->m_bitMap->GetHeight() * loadingHelperDecrytpe->m_currentImage->m_bitMap->GetWidth() * 3;
				swprintf(buffernumber, nbCharacterPossible, L"%d", nbCharacterPossible);

				SetWindowText(TextMessageReturn, TEXT(""));
				SetWindowText(TextCharRestant, buffernumber);
				InvalidateRect(hWnd, NULL, TRUE);
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
			}
			else {
				DestroyLoadingHelper(hWnd);
				break;
			}
			break;
		}
		case BUTTON_LOAD_2:
		{
			DestroyLoadingHelper(hWnd, true);
			loadingHelperEncrypte = new LoadingHelper();

			if (loadingHelperEncrypte->Init(hWnd)) {

				InvalidateRect(hWnd, NULL, TRUE);
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
			}
			else {
				DestroyLoadingHelper(hWnd, true);
				break;
			}
			break;
		}
		case BUTTON2_ID:
		{
			GetWindowText(hEdit, bufferMessage, 255);
			if (bufferMessage[0] == L'\0') {
				MessageBox(hWnd, L"Ecris un message à cacher dans l'image !", L"message missing", MB_ICONERROR | MB_OK);
				break;
			}
			if (!loadingHelperDecrytpe || !loadingHelperDecrytpe->m_currentImage) {
				MessageBox(hWnd, L"Choisis un fichier comme image !", L"iamge missing", MB_ICONERROR | MB_OK);
				break;
			}

			steno->LSBEncode(loadingHelperDecrytpe->m_currentImage->m_bitMap, bufferMessage);
			loadingHelperDecrytpe->SaveImage(loadingHelperDecrytpe->m_currentExtension->GetCompletePath(L"_out"));

			loadingHelperEncrypte = loadingHelperDecrytpe;
			loadingHelperDecrytpe = nullptr;

			SetWindowText(hEdit, L"\0");
			SetWindowText(TextCharRestant, L"\0");

			//DestroyLoadingHelper(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case BUTTON3_ID:
		{
			if (!loadingHelperEncrypte || !loadingHelperEncrypte->m_currentImage) {
				MessageBox(hWnd, L"Choisis un fichier comme image !", L"iamge missing", MB_ICONERROR | MB_OK);
				break;
			}

			std::wstring newMessage = steno->LSBDecode(loadingHelperEncrypte->m_currentImage->m_bitMap);
			LPCWSTR lpcwstr = newMessage.c_str();

			SetWindowText(TextCharRestant, L"\0");
			SetWindowText(TextMessageReturn, TEXT(""));
			SetWindowText(TextMessageReturn, lpcwstr);

			//DestroyLoadingHelper(hWnd, true);
			InvalidateRect(hWnd, NULL, TRUE);

			break;
		}
		case CLEAR_IMAGE:
		{
			DestroyLoadingHelper(hWnd);
			DestroyLoadingHelper(hWnd, true);
			InvalidateRect(hWnd, NULL, TRUE);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
			break;
		}
		case 1001:  // ID of the accelerator (Ctrl+O)
		{
			HWND hButton = GetDlgItem(hWnd, BUTTON_LOAD_1);
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		int windowWidth = clientRect.right - clientRect.left;
		int windowHeight = clientRect.bottom - clientRect.top;

		Graphics graphics(hdc);

		int ymin = (*UIObject)[ReturnIndexObject(BUTTON_LOAD_1)].m_transform.getPositionY() +
			(*UIObject)[ReturnIndexObject(BUTTON_LOAD_1)].m_transform.getHeight();
		int ymax = (*UIObject)[ReturnIndexObject(TEXT_RESTANT_1)].m_transform.getPositionY();

		// Espace disponible entre les deux objets UI
		int availableHeight = ymax - ymin;

		// Calcul de la largeur de chaque image (la moitié de la largeur de la fenêtre)
		int imageAreaWidth = windowWidth / 2;

		// Dessiner la première image (decrypt)
		if (loadingHelperDecrytpe && loadingHelperDecrytpe->m_currentImage && loadingHelperDecrytpe->m_currentImage->m_bitMap)
		{
			int imgWidth = loadingHelperDecrytpe->m_currentImage->m_bitMap->GetWidth();
			int imgHeight = loadingHelperDecrytpe->m_currentImage->m_bitMap->GetHeight();

			// Calcul du rapport d'aspect
			float aspectRatio = static_cast<float>(imgWidth) / static_cast<float>(imgHeight);

			// Largeur et hauteur de l'image ajustée pour s'adapter à la fenêtre
			int newWidth = imageAreaWidth;
			int newHeight = static_cast<int>(newWidth / aspectRatio);

			// Si la hauteur dépasse l'espace disponible entre les deux objets UI, ajuster la taille
			if (newHeight > availableHeight) {
				newHeight = availableHeight;
				newWidth = static_cast<int>(newHeight * aspectRatio);
			}

			// Positionner la première image à gauche, centrée horizontalement dans la moitié gauche de la fenêtre
			int xPosition = (imageAreaWidth - newWidth) / 2;
			int yPosition = ymin;

			Graphics* g = Graphics::FromHDC(hdc);
			g->DrawImage(loadingHelperDecrytpe->m_currentImage->m_bitMap, xPosition, yPosition, newWidth, newHeight);
			delete g;
		}

		// Dessiner la deuxième image (encrypt)
		if (loadingHelperEncrypte && loadingHelperEncrypte->m_currentImage && loadingHelperEncrypte->m_currentImage->m_bitMap)
		{
			int imgWidth = loadingHelperEncrypte->m_currentImage->m_bitMap->GetWidth();
			int imgHeight = loadingHelperEncrypte->m_currentImage->m_bitMap->GetHeight();

			// Calcul du rapport d'aspect
			float aspectRatio = static_cast<float>(imgWidth) / static_cast<float>(imgHeight);

			// Largeur et hauteur de l'image ajustée pour s'adapter à la fenêtre
			int newWidth = imageAreaWidth;
			int newHeight = static_cast<int>(newWidth / aspectRatio);

			// Si la hauteur dépasse l'espace disponible entre les deux objets UI, ajuster la taille
			if (newHeight > availableHeight) {
				newHeight = availableHeight;
				newWidth = static_cast<int>(newHeight * aspectRatio);
			}

			// Positionner la deuxième image à droite, centrée horizontalement dans la moitié droite de la fenêtre
			int xPosition = imageAreaWidth + (imageAreaWidth - newWidth) / 2;
			int yPosition = ymin;

			Graphics* g = Graphics::FromHDC(hdc);
			g->DrawImage(loadingHelperEncrypte->m_currentImage->m_bitMap, xPosition, yPosition, newWidth, newHeight);
			delete g;
		}

		EndPaint(hWnd, &ps);
		break;
	}



	case WM_DESTROY:

		DestroyLoadingHelper(hWnd);
		DestroyLoadingHelper(hWnd, true);
		delete createUI;
		createUI = nullptr;
		delete steno;
		steno = nullptr;
		Gdiplus::GdiplusShutdown(gdiplusToken);

		PostQuitMessage(0);
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


void ResizeWindow(int width, int height)
{
	for (size_t i = 0; i < (*UIObject).size(); i++)
	{
		ObjectUI objectUI;
		objectUI = (*UIObject)[i];
		float ratioX = objectUI.m_transform.getWidth() / createUI->m_transformWindow->getWidth();
		float ratioY = objectUI.m_transform.getHeight() / createUI->m_transformWindow->getHeight();

		float newX = objectUI.m_transform.getPositionX();
		float newY = objectUI.m_transform.getPositionY();

		if (newX != 0)
		{
			newX = (objectUI.m_transform.getPositionX() / createUI->m_transformWindow->getWidth()) * width;
		}		
		if (newY != 0)
		{
			newY = (objectUI.m_transform.getPositionY() / createUI->m_transformWindow->getHeight()) * height;
		}

		MoveWindow(*objectUI.m_hwnd, newX, newY, ratioX * width, ratioY * height, TRUE);
	}
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
	if (loadingHelperDecrytpe == nullptr || loadingHelperDecrytpe->m_currentImage == nullptr || loadingHelperDecrytpe->m_currentExtension == nullptr)
	{
		loadingHelperDecrytpe = new LoadingHelper();
		if (loadingHelperDecrytpe->Init(hWnd)) {
			return true;
		}
		else{
			DestroyLoadingHelper(hWnd);
			return false;
		}
	}
	return true;
}

void DestroyLoadingHelper(HWND hWnd, bool isEncrypt)
{
	if (loadingHelperDecrytpe && !isEncrypt)
	{
		loadingHelperDecrytpe->~LoadingHelper();
		delete loadingHelperDecrytpe;
		loadingHelperDecrytpe = nullptr;
	}
	if (loadingHelperEncrypte && isEncrypt)
	{
		loadingHelperEncrypte->~LoadingHelper();
		delete loadingHelperEncrypte;
		loadingHelperEncrypte = nullptr;
	}
}
