// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"
#include <fstream>
#include <commdlg.h>



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hEdit;
HWND hSaveButton;
HWND hLoadButton;
WCHAR savedNote[10000]; // This will store the saved note, you can increase the size if needed.
HWND hWordCountLabel;
HWND hClearButton;
WCHAR wordCountText[100];
const WCHAR* NOTE_FILE_PATH = L"notes.txt";




// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   hEdit = CreateWindowEx(0, L"EDIT", L"",
       WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
       10, 10, 400, 200,
       hWnd, (HMENU)1, hInstance, NULL);

   hSaveButton = CreateWindowEx(0, L"BUTTON", L"Save Note",
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       10, 220, 100, 30,
       hWnd, (HMENU)2, hInstance, NULL);

   hLoadButton = CreateWindowEx(0, L"BUTTON", L"Load Note",
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       120, 220, 100, 30,
       hWnd, (HMENU)3, hInstance, NULL);

    // Create Word Counter label
   wsprintf(wordCountText, L"Words: 0");
   hWordCountLabel = CreateWindowEx(0, L"STATIC", wordCountText,
       WS_CHILD | WS_VISIBLE | SS_LEFT,
       230, 225, 150, 25,
       hWnd, NULL, hInstance, NULL);

   // Create Clear button
   hClearButton = CreateWindowEx(0, L"BUTTON", L"Clear",
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
       320, 220, 100, 30,
       hWnd, (HMENU)4, hInstance, NULL);



   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if (message == WM_COMMAND && HIWORD(wParam) == EN_CHANGE && (HWND)lParam == hEdit) {
        int wordCount = 0;
        int len = GetWindowTextLength(hEdit);
        if (len > 0) {
            WCHAR* text = new WCHAR[len + 1];
            GetWindowText(hEdit, text, len + 1);

            WCHAR* context = NULL; // context variable for wcstok_s
            WCHAR* token = wcstok_s(text, L" \t\n", &context);
            while (token != NULL) {
                wordCount++;
                token = wcstok_s(NULL, L" \t\n", &context);
            }
            delete[] text;
        }
        wsprintf(wordCountText, L"Words: %d", wordCount);
        SetWindowText(hWordCountLabel, wordCountText);
    }



    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case 2: // Save button
        {
            OPENFILENAME ofn;
            WCHAR szFileName[MAX_PATH] = L"";
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
            ofn.lpstrDefExt = L"txt";

            if (GetSaveFileName(&ofn)) {
                int len = GetWindowTextLength(hEdit) + 1;
                WCHAR* text = new WCHAR[len];
                GetWindowText(hEdit, text, len);

                std::wofstream outFile(szFileName);
                outFile << text;
                outFile.close();

                delete[] text;
                MessageBox(hWnd, L"Note saved to file!", L"Info", MB_OK);
            }
        }
        break;

        case 3: // Load button
        {
            OPENFILENAME ofn;
            WCHAR szFileName[MAX_PATH] = L"";
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = L"txt";

            if (GetOpenFileName(&ofn)) {
                std::wifstream inFile(szFileName);
                std::wstring content((std::istreambuf_iterator<WCHAR>(inFile)), std::istreambuf_iterator<WCHAR>());
                inFile.close();

                SetWindowText(hEdit, content.c_str());
                MessageBox(hWnd, L"Note loaded from file!", L"Info", MB_OK);
            }
        }
        break;

            // Inside WndProc function, inside the WM_COMMAND case
        case 4: // Clear button
            SetWindowText(hEdit, L"");
            wsprintf(wordCountText, L"Words: 0");
            SetWindowText(hWordCountLabel, wordCountText);
            break;
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

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
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
