#include "Graphics.h"
#include "G_Algorithm.h"
#include "framework.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                      // 当前实例
WCHAR szTitle[MAX_LOADSTRING];        // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];  // 主窗口类名

HDC memhdc;
HBITMAP mBitmap;
static HBRUSH mbrush;
static SIZE size;  //窗口大小

//数据变量：
static int input_x0 = 0, input_y0 = 0, input_x1 = 0, input_y1 = 0, input_a = 0,
           input_b = 0, input_r = 0;
static int L_x0 = 0, L_y0 = 0, L_x1 = 0, L_y1 = 0;
static char ll = 'D';  // ll用来识别用那种算法画直线
line l = {0, 0, 0, 0, 'D'}, L_MOVE = {0, 0, 0, 0, 'D'},
     ClipLine = {0, 0, 0, 0, 'D'};
struct circ {
  int a;
  int b;
  int r;
} c = {0, 0, 0}, C_MOVE = {0, 0, 0};
static std::vector<line> Line;
static std::vector<circ> Circle;
static std::vector<char> operate;
static std::vector<POINT> Poly, Out, _Bezier, _Out;
static POINT Ppoint;
//状态模式变量：
enum mode { Paint, Clip, Fill, Bezier };
mode m = mode::Paint;
bool L_bottondown = false, fill = false, MOVE_Point = false;
static int MOVE_TIME = 0, ClipMode = 0, PaintMode = 0, PointSite = 0,
           FillMode = 0;

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK LBOX(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CBOX(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PRZ(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: 在此处放置代码。

  // 初始化全局字符串
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_GRAPHICS, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // 执行应用程序初始化:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICS));

  MSG msg;

  // 主消息循环:
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

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICS));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GRAPHICS);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // 将实例句柄存储在全局变量中

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  HDC hdc;
  switch (message) {
    case WM_CREATE:
      size.cx = GetSystemMetrics(SM_CXSCREEN);
      size.cy = GetSystemMetrics(SM_CYSCREEN);
      hdc = GetDC(hWnd);
      memhdc = CreateCompatibleDC(hdc);
      mBitmap = CreateCompatibleBitmap(hdc, size.cx, size.cy);
      SelectObject(memhdc, mBitmap);
      SetWindowText(hWnd, L"Paint:圆");
      break;
    case WM_SIZE:
      size.cx = LOWORD(lParam);
      size.cy = HIWORD(lParam);
      return 0;
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // 分析菜单选择:
      switch (wmId) {
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        case ID_DDA:
          ll = 'D';
          DialogBox(hInst, MAKEINTRESOURCE(IDD_LBOX), hWnd, LBOX);  // DDA算法
          break;
        case ID_MID:
          ll = 'M';
          DialogBox(hInst, MAKEINTRESOURCE(IDD_LBOX), hWnd, LBOX);  //中单算法
          break;
        case ID_BRE:
          ll = 'B';
          DialogBox(hInst, MAKEINTRESOURCE(IDD_LBOX), hWnd,
                    LBOX);  // Bresenham算法
          break;
        case ID_CIRCLE:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_CBOX), hWnd,
                    CBOX);  //中点花园算法
          break;
        case ID_PRZ:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_PRZ), hWnd, PRZ);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_SETFOCUS:
      InvalidateRect(hWnd, NULL, TRUE);
      break;
    case WM_PAINT:
      PAINTSTRUCT ps;
      hdc = BeginPaint(hWnd, &ps);
      mbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
      SelectObject(memhdc, mbrush);
      Rectangle(memhdc, -1, -1, size.cx + 1, size.cy + 1);
      //三种算法的直线绘制
      if (Line.size() >= 1) {
        for (auto& v : Line) {
          if (v.ll == 'D')
            DDALine(memhdc, v.x0, v.y0, v.x1, v.y1);
          else if (v.ll == 'M')
            midLine(memhdc, v.x0, v.y0, v.x1, v.y1);
          else if (v.ll == 'B')
            BresenhamLine(memhdc, v.x0, v.y0, v.x1, v.y1);
        }
      }
      //鼠标移动时直线与圆随之绘制
      if (L_bottondown) {
        if (PaintMode != 0 && m == Paint)
          DDALine(memhdc, L_MOVE.x0, L_MOVE.y0, L_MOVE.x1, L_MOVE.y1);
        circle(memhdc, C_MOVE.a, C_MOVE.b, C_MOVE.r);
        mbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        SelectObject(memhdc, mbrush);
        Rectangle(memhdc, ClipLine.x0, ClipLine.y0, ClipLine.x1, ClipLine.y1);
      }
      //圆的绘制
      if (Circle.size() >= 1) {
        for (auto& v : Circle) {
          circle(memhdc, v.a, v.b, v.r);
        }
      }
      //多边形的绘制
      if (Poly.size() >= 2) {
        for (size_t i = 0; i < Poly.size(); i++) {
          if (i == 0)
            MoveToEx(memhdc, Poly[i].x, Poly[i].y, NULL);
          else
            LineTo(memhdc, Poly[i].x, Poly[i].y);
        }
      }
      // Bezier曲线
      if (_Bezier.size() > 0) {
        for (auto& v : _Bezier) {
          Ellipse(memhdc, v.x - 10, v.y + 10, v.x + 10, v.y - 10);
        }
        if (_Bezier.size() >= 2) {
          for (size_t i = 0; i < _Bezier.size(); i++) {
            if (i == 0)
              MoveToEx(memhdc, _Bezier[i].x, _Bezier[i].y, NULL);
            else
              LineTo(memhdc, _Bezier[i].x, _Bezier[i].y);
          }
        }
        if (_Bezier.size() >= 3) {
          for (double i = 0.0; i <= 1.0; i += 0.001) {
            DeC_Bezier(_Bezier, _Out, i);
            // for (auto& v : _Out)
            SetPixel(memhdc, _Out[0].x, _Out[0].y, RGB(0, 139, 139));
          }
        }
      }
      //多边形裁剪后的多边形绘制
      if (Out.size() >= 2) {
        HPEN CPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        SelectPen(memhdc, CPen);
        for (size_t i = 0; i < Out.size(); i++) {
          if (i == 0)
            MoveToEx(memhdc, Out[i].x, Out[i].y, NULL);
          else
            LineTo(memhdc, Out[i].x, Out[i].y);
        }
        CPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        SelectPen(memhdc, CPen);
      }
      //填充的绘制
      if (fill && m == Fill && FillMode == 0)
        ScanLineFill4(L_x0, L_y0, 0, RGB(255, 255, 255), memhdc);
      if (fill && FillMode == 1) PolyScan(Poly, memhdc);
      BitBlt(hdc, 0, 0, size.cx, size.cy, memhdc, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
      break;

    case WM_RBUTTONDOWN:
      switch (m) {
        case mode::Paint:
          if (operate.size() >= 1) {
            if (operate.back() == 'L') {
              operate.pop_back();
              Line.pop_back();
            } else if (operate.back() == 'C') {
              operate.pop_back();
              Circle.pop_back();
            } else if (operate.back() == 'P') {
              if (Poly.size() > 1) {
                L_bottondown = true;
                Poly.pop_back();
              } else if (Poly.size() == 1) {
                L_bottondown = false;
                Poly.clear();
              }
              operate.pop_back();
              // else if (Poly.size() == 0) L_bottondown = false;
            }
          }
          break;
        case mode::Clip:
          Out.clear();
          break;
        case mode::Fill:
          fill = false;
          break;
        case mode::Bezier:
          if (_Bezier.size() > 0) _Bezier.pop_back();
          break;
        default:
          break;
      }
      InvalidateRect(hWnd, NULL, TRUE);
      break;
    case WM_LBUTTONDOWN:
      switch (m) {
        case mode::Paint:
          L_x0 = GET_X_LPARAM(lParam);
          L_y0 = GET_Y_LPARAM(lParam);  //鼠标左键获取直线第一个端点或圆心
          if (PaintMode == 2) {
            Ppoint.x = GET_X_LPARAM(lParam);
            Ppoint.y = GET_Y_LPARAM(lParam);
            Poly.push_back(Ppoint);
            operate.push_back('P');
          }
          L_bottondown = true;  //打开鼠标移动时的绘制
          break;
        case mode::Clip:
          L_x0 = GET_X_LPARAM(lParam);
          L_y0 = GET_Y_LPARAM(lParam);
          L_bottondown = true;
          break;
        case mode::Fill:
          if (FillMode == 0) {
            MessageBoxW(hWnd, L"以该点为种子区域扫描填充", L"WARNING", MB_OK);
            fill = true;
            L_x0 = GET_X_LPARAM(lParam);
            L_y0 = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
          }
          break;
        case mode::Bezier:
          Ppoint.x = GET_X_LPARAM(lParam);
          Ppoint.y = GET_Y_LPARAM(lParam);
          for (auto& v : _Bezier) {
            MOVE_Point = Inside(v, Ppoint);
            if (MOVE_Point == true) {
              L_bottondown = true;
              break;
            }
            PointSite++;
          }
          if (!MOVE_Point) _Bezier.push_back(Ppoint);
          break;
        default:
          break;
      }
      break;
    case WM_MOUSEMOVE:
      if (L_bottondown) {
        switch (m) {
          case mode::Paint:
            L_x1 = GET_X_LPARAM(lParam);
            L_y1 = GET_Y_LPARAM(lParam);  //获得直线第二点与计算圆半径的数据
            if (PaintMode == 0) {  //区分画圆或直线
              UINT z =
                  (L_x0 - L_x1) * (L_x0 - L_x1) + (L_y0 - L_y1) * (L_y0 - L_y1);
              double m = sqrt(z);
              C_MOVE = {L_x0, L_y0, int(m)};  //鼠标移动时圆的数据存储
            }
            if (PaintMode == 2 && Poly.size() > 0) {
              L_x0 = Poly.back().x;
              L_y0 = Poly.back().y;
            }
            L_MOVE = {L_x0, L_y0, L_x1, L_y1, 'D'};  //鼠标移动时直线的数据存储
            break;
          case mode::Clip:
            L_x1 = GET_X_LPARAM(lParam);
            L_y1 = GET_Y_LPARAM(lParam);
            ClipLine = {L_x0, L_y0, L_x1, L_y1, ll};
            break;
          case mode::Fill:
            break;
          case mode::Bezier:
            if (MOVE_Point == true) {
              _Bezier[PointSite].x = GET_X_LPARAM(lParam);
              _Bezier[PointSite].y = GET_Y_LPARAM(lParam);
            }
            // InvalidateRect(hWnd, NULL, TRUE);
            break;
          default:
            break;
        }
        if (MOVE_TIME == 5) {
          MOVE_TIME %= 5;
          InvalidateRect(hWnd, NULL, TRUE);
        }
        MOVE_TIME++;
      }
      break;
    case WM_LBUTTONUP:
      switch (m) {
        case mode::Paint:
          C_MOVE = {0, 0, 0};
          L_MOVE = {0, 0, 0, 0, ll};  //移动数据绘制清零
          L_x1 = GET_X_LPARAM(lParam);
          L_y1 = GET_Y_LPARAM(lParam);
          if (PaintMode == 0) {
            L_bottondown = false;
            UINT z =
                (L_x0 - L_x1) * (L_x0 - L_x1) + (L_y0 - L_y1) * (L_y0 - L_y1);
            double m = sqrt(z);
            c = {L_x0, L_y0, int(m)};
            Circle.push_back(c);     //存储圆的数据
            operate.push_back('C');  //存储操作-画圆
          } else if (PaintMode == 1) {
            L_bottondown = false;
            l = {L_x0, L_y0, L_x1, L_y1, ll};
            Line.push_back(l);       //存储直线的数据
            operate.push_back('L');  //存储操作-画直线
          }
          break;
        case mode::Clip:
          L_bottondown = false;
          if (ClipMode == 0) {
            if (Line.size() > 0) {
              InitBoundary(ClipLine);
              CS_LineClip(Line.back().x0, Line.back().y0, Line.back().x1,
                          Line.back().y1, ClipLine.x0, ClipLine.x1, ClipLine.y1,
                          ClipLine.y0);
            }
            ClipLine = {0, 0, 0, 0, 'D'};
          }
          if (ClipMode == 1) {
            if (Poly.size() > 0) {
              InitBoundary(ClipLine);
              SoutherlandHodgmanClip(Poly, Out, ClipLine);
              ClipLine = {0, 0, 0, 0, 'D'};
            }
          }
          break;
        case mode::Fill:
          break;
        case mode::Bezier:
          MOVE_Point = false;
          L_bottondown = false;
          PointSite = 0;
          break;
        default:
          break;
      }
      InvalidateRect(hWnd, NULL, TRUE);
      break;
    case WM_KEYDOWN:
      switch (wParam) {
        case VK_TAB:
          m = (mode)((m + 1) % 4);
          switch (m) {
            case mode::Paint:
              if (PaintMode == 0)
                SetWindowText(hWnd, L"Paint:圆");
              else if (PaintMode == 1)
                SetWindowText(hWnd, L"Paint:直线");
              else if (PaintMode == 2)
                SetWindowText(hWnd, L"Paint:多边形");
              break;
            case mode::Clip:
              if (ClipMode == 0)
                SetWindowTextW(hWnd, L"Clip:剪切直线");
              else if (ClipMode == 1)
                SetWindowTextW(hWnd, L"Clip:剪切多边形");
              break;
            case mode::Fill:
              if (FillMode == 0)
                SetWindowText(hWnd, L"Fill:区域填充");
              else if (FillMode == 1)
                SetWindowText(hWnd, L"Fill:多边形填充");
              break;
            case mode::Bezier:
              SetWindowText(hWnd, L"Bezier曲线");
              break;
            default:
              break;
          }
          break;
        case 'L':  //切换鼠标画直线的算法
          if (ll == 'D') {
            ll = 'M';
            MessageBoxW(hWnd, L"中点算法画直线", L"提示", MB_OK);
          } else if (ll == 'M') {
            ll = 'B';
            MessageBoxW(hWnd, L"Bresenham算法画直线", L"提示", MB_OK);
          } else if (ll == 'B') {
            ll = 'D';
            MessageBoxW(hWnd, L"DDA算法画直线", L"提示", MB_OK);
          }
          break;
        case VK_RETURN:
          switch (m) {
            case Paint:
              if (PaintMode == 2 && Poly.size() > 0) {
                Ppoint.x = Poly.front().x;
                Ppoint.y = Poly.front().y;
                Poly.push_back(Ppoint);
                operate.push_back('P');
                L_bottondown = false;
                InvalidateRect(hWnd, NULL, TRUE);
              }
              break;
            case Clip:
              break;
            case Fill:
              if (FillMode == 1) {
                fill = true;
                InvalidateRect(hWnd, NULL, TRUE);
              }
              break;
            case Bezier:
              break;
            default:
              break;
          }
          break;
        case '1':  //切换绘画子模式
          switch (m) {
            case mode::Paint:
              PaintMode = (PaintMode + 1) % 3;
              if (PaintMode == 0)
                SetWindowText(hWnd, L"Paint:圆");
              else if (PaintMode == 1)
                SetWindowText(hWnd, L"Paint:直线");
              else if (PaintMode == 2)
                SetWindowText(hWnd, L"Paint:多边形");
              break;
            case mode::Clip:
              ClipMode = (ClipMode + 1) % 2;
              if (ClipMode == 0)
                SetWindowTextW(hWnd, L"Clip:剪切直线");
              else if (ClipMode == 1)
                SetWindowTextW(hWnd, L"Clip:剪切多边形");
              break;
            case mode::Fill:
              FillMode = (FillMode + 1) % 2;
              if (FillMode == 0)
                SetWindowText(hWnd, L"Fill:区域填充");
              else if (FillMode == 1)
                SetWindowText(hWnd, L"Fill:多边形填充");
              break;
            case mode::Bezier:
              break;
            default:
              break;
          }
          break;
        case VK_UP:
          if (Poly.size() > 0)
            for (auto& v : Poly) Pan_Rotate_Zoom(0, -10, 0, 0, 0, 1, 1, v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_DOWN:
          if (Poly.size() > 0)
            for (auto& v : Poly) Pan_Rotate_Zoom(0, 10, 0, 0, 0, 1, 1, v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_LEFT:
          if (Poly.size() > 0)
            for (auto& v : Poly) Pan_Rotate_Zoom(-10, 0, 0, 0, 0, 1, 1, v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_RIGHT:
          if (Poly.size() > 0)
            for (auto& v : Poly) Pan_Rotate_Zoom(10, 0, 0, 0, 0, 1, 1, v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_NUMPAD1:
          if (Poly.size() > 0)
            for (auto& v : Poly)
              Pan_Rotate_Zoom(0, 0, Poly.front().x, Poly.front().y, 15, 1, 1,
                              v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_NUMPAD2:
          if (Poly.size() > 0)
            for (auto& v : Poly)
              Pan_Rotate_Zoom(0, 0, Poly.front().x, Poly.front().y, -15, 1, 1,
                              v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_NUMPAD4:
          if (Poly.size() > 0)
            for (auto& v : Poly)
              Pan_Rotate_Zoom(0, 0, Poly.front().x, Poly.front().y, 0, 2, 2, v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        case VK_NUMPAD5:
          if (Poly.size() > 0)
            for (auto& v : Poly)
              Pan_Rotate_Zoom(0, 0, Poly.front().x, Poly.front().y, 0, 0.5, 0.5,
                              v);
          InvalidateRect(hWnd, NULL, TRUE);
          break;
        default:
          break;
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

// 框的消息处理程序。
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
INT_PTR CALLBACK LBOX(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  BOOL lp;
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      switch (wParam) {
        case IDOK:
          input_x0 = GetDlgItemInt(hDlg, ID_X0, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"第一个点 X 未输入", L"提示", MB_OK);
          }
          input_y0 = GetDlgItemInt(hDlg, ID_Y0, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"第一个点 Y 未输入", L"提示", MB_OK);
          }
          input_x1 = GetDlgItemInt(hDlg, ID_X1, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"第二个点 X 未输入", L"提示", MB_OK);
          }
          input_y1 = GetDlgItemInt(hDlg, ID_Y1, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"第二个点 Y 未输入", L"提示", MB_OK);
          }
          l = {input_x0, input_y0, input_x1, input_y1, ll};
          Line.push_back(l);
          operate.push_back('L');
          EndDialog(hDlg, LOWORD(wParam));
          return TRUE;

        case IDCANCEL:
          EndDialog(hDlg, LOWORD(wParam));
          return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}
INT_PTR CALLBACK CBOX(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  BOOL lp;
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;
    case WM_COMMAND:
      switch (wParam) {
        case IDOK:
          input_a = GetDlgItemInt(hDlg, IDC_CA, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"原点坐标未输入", L"提示", MB_OK);
          }
          input_b = GetDlgItemInt(hDlg, IDC_CB, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"原点坐标未输入", L"提示", MB_OK);
          }
          input_r = GetDlgItemInt(hDlg, IDC_CR, &lp, false);
          if (lp == false) {
            MessageBoxW(hDlg, L"半径未输入", L"提示", MB_OK);
          }
          c = {input_a, input_b, input_r};
          Circle.push_back(c);
          operate.push_back('C');
          EndDialog(hDlg, LOWORD(wParam));
          return (INT_PTR)TRUE;
        case IDCANCEL:
          EndDialog(hDlg, LOWORD(wParam));
          return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}

INT_PTR CALLBACK PRZ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  BOOL lp;
  int P_x = 0, P_y = 0, R_x = 0, R_y = 0, R = 0;
  double Z_x = 1.0, Z_y = 1.0;
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;
    case WM_COMMAND:
      switch (wParam) {
        case IDOK: {
          WCHAR str_R[10] = {0}, str_Zx[10] = {0}, str_Zy[10] = {0};
          P_x = GetDlgItemInt(hDlg, IDC_PanX, &lp, false);
          P_y = GetDlgItemInt(hDlg, IDC_PanY, &lp, false);
          R_x = GetDlgItemInt(hDlg, IDC_X, &lp, false);
          R_y = GetDlgItemInt(hDlg, IDC_Y, &lp, false);
          R = GetDlgItemInt(hDlg, IDC_RotateA, &lp, false);
          if (GetDlgItemTextW(hDlg, IDC_ZoomX, str_Zx, 10))
            Z_x = wcstod(str_Zx, NULL);
          else
            Z_x = 1.0;
          if (GetDlgItemTextW(hDlg, IDC_ZoomY, str_Zy, 10))
            Z_y = wcstod(str_Zy, NULL);
          else
            Z_y = 1.0;
          if (Poly.size() > 0) {
            for (auto& v : Poly) {
              Pan_Rotate_Zoom(P_x, P_y, R_x, R_y, R, Z_x, Z_y, v);
            }
          }
          EndDialog(hDlg, LOWORD(wParam));
          return (INT_PTR)TRUE;
        }
        case IDCANCEL:
          EndDialog(hDlg, LOWORD(wParam));
          return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}