#include "framework.h"
#include "Hider.h"
#include "LoadingHelper.h"
#include <vector>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    CreateAWindow(hInstance, nCmdShow, L"HiderApp", L"HiderApp", WndProc);

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
    static HWND hPicture;
    static bool isTextCleared = false;

    LoadingHelper* loadingHelper = nullptr;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    switch (message)
    {
    case WM_CREATE:
        CreateButton(hWnd, BUTTON1_ID, L"Choisir un fichier", 650, 50, 150, 30);
        CreateButton(hWnd, BUTTON2_ID, L"Stenographier le message", 550, 250, 350, 50);
        hEdit = CreateInput(hWnd, EDIT_ID, L"Message a cacher", 600, 150, 250, 50);

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
                        CreateAWindow(GetModuleHandle(NULL), SW_SHOW, L"PictureClass", L"Picture", PictureWndProc, loadingHelper);
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    break;
                }
            }
            break;
        }

    case WM_DESTROY:
        Gdiplus::GdiplusShutdown(gdiplusToken);

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK PictureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LoadingHelper* loadingHelper = (LoadingHelper*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (loadingHelper) 
        {
            loadingHelper->Draw(hdc, 0, 0); 
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:

        delete loadingHelper; 
        loadingHelper = nullptr;

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateButton(HWND hWnd,int button_id, LPCWSTR message,int posX, int posY, int largeur, int longueur)
{
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
}

HWND CreateInput(HWND hWnd,int input_id, LPCWSTR message, int posX, int posY, int largeur, int longueur)
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

void CreateAWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR ClassName, LPCWSTR WindowName, WNDPROC func, LoadingHelper* helper)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = func;
    wc.hInstance = hInstance;
    wc.lpszClassName = ClassName;

    RegisterClass(&wc);

    LONG longueur = helper == nullptr ? CW_USEDEFAULT : helper->GetBitMap().bmWidth;
    LONG largeur = helper == nullptr ? CW_USEDEFAULT : helper->GetBitMap().bmHeight;

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