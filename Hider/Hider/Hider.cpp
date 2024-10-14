#include "framework.h"
#include "Hider.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{   
    LPCWSTR CLASS_NAME = L"Hider";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Hider",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Créer un bouton
        CreateWindow(
            L"BUTTON",  // Précise le type de la fenêtre
            L"Charger une image", // Texte du bouton
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
            50,         // Position X
            50,         // Position Y
            150,        // Largeur
            50,         // Hauteur
            hWnd,       // Handle de la fenêtre parente
            (HMENU)BUTTON_ID, // Identifiant du bouton
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pas de données supplémentaires

        CreateWindow(
            L"EDIT",            // Type de contrôle
            L"Message a cacher",                // Texte initial
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, // Styles
            50,                // Position X
            150,                // Position Y
            200,               // Largeur
            30,                // Hauteur
            hWnd,              // Handle de la fenêtre parente
            (HMENU)EDIT_ID,    // Identifiant du champ d'édition
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);             // Pas de données supplémentaires
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_SETFOCUS:
        // Effacer le texte du champ d'édition lorsque le focus est donné
        if ((HWND)wParam == GetDlgItem(hWnd, EDIT_ID)) {
            SetWindowText(GetDlgItem(hWnd, EDIT_ID), L"");
        }
        break;

    case WM_DESTROY:
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