﻿#include "StopTheBall.h"
#include "buttons.h"
#include "context.h"
#include "framework.h"
#include "game.h"
#include "ui.h"

#define MAX_LOADSTRING 100
#define GLOBAL_TIMER_ID 98

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HDC mdc = nullptr;
ik::game game;
ik::context ctx;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_STOPTHEBALL, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STOPTHEBALL));

  MSG msg;

  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);
  auto icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = icon;
  wcex.hIconSm = icon;
  wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_STOPTHEBALL);
  wcex.lpszClassName = szWindowClass;

  return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;
  HWND hWnd = CreateWindowW(
      szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0,
      ctx.width, ctx.height, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      switch (wmId) {
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case ID_HELP:
          show_help(hWnd);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      game.paint(mdc, ctx);
      HDC hdc = BeginPaint(hWnd, &ps);
      BitBlt(hdc, 0, 0, ctx.width, ctx.height, mdc, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
    } break;
    case WM_CREATE: {
      ::SetTimer(hWnd, GLOBAL_TIMER_ID, ctx.tick_duration, nullptr);
      game.rect(ik::type::rect(0, 0, ctx.width, ctx.height));
      HDC hdc = GetDC(hWnd);
      mdc = CreateCompatibleDC(hdc);
      SetBkMode(mdc, TRANSPARENT);
      ctx = ik::context(hInst, mdc);
      ctx.hwnd = hWnd;
      HBITMAP hbmp = CreateCompatibleBitmap(hdc, ctx.width, ctx.height);
      SelectObject(mdc, hbmp);
      ReleaseDC(hWnd, hdc);
      ui_welcome(ctx);
    } break;
    case WM_TIMER:
      if (wParam == GLOBAL_TIMER_ID) {
        ik::signals::tick_signal.emit(ik::type::clock::now());
        InvalidateRect(hWnd, NULL, FALSE);
      }
      break;
    case WM_LBUTTONDOWN:
      ik::signals::btn_down_signal.emit(
          ik::type::point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_LBUTTONUP:
      ik::signals::btn_up_signal.emit(
          ik::type::point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_LBUTTONDBLCLK:
      ik::signals::dbl_click_signal.emit(
          ik::type::point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_MOUSEMOVE:
      ik::signals::mouse_move_signal.emit(
          ik::type::point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
      break;
    case WM_KEYUP:
      ik::signals::key_signal.emit(std::toupper(wParam));
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}