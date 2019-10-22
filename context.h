#pragma once

#include <Windows.h>
#include <memory>
#include "res.h"

#include "resource.h"

namespace ik {

struct context {
  int width = 0, height = 0;
  image::pimage bg, red, blue, green, yellow;
  font::pfont title_font, btn_font, normal_font;
  int tick_duration = 30;
  COLORREF title_color = RGB(0, 0, 0), mouse_hover_color = RGB(255, 0, 0),
           mouse_down_color = RGB(0, 255, 0), normal_color = RGB(0, 0, 255);

  context() {
    width = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * (2 / 5.0));
    height = static_cast<int>(GetSystemMetrics(SM_CYSCREEN) * (6 / 7.0));
  }
  context(HINSTANCE hinst, HDC hdc) : context() {
    bg = image::make_image(hinst, hdc, IDB_BG);
    red = image::make_image(hinst, hdc, IDB_RED);
    blue = image::make_image(hinst, hdc, IDB_BLUE);
    green = image::make_image(hinst, hdc, IDB_GREEN);
    yellow = image::make_image(hinst, hdc, IDB_YELLOW);

    LOGFONT logfont;
    memset(&logfont, 0, sizeof(logfont));
    logfont.lfHeight = 45;
    logfont.lfWidth = 30;
    logfont.lfUnderline = 0;
    logfont.lfItalic = 0;
    logfont.lfCharSet = GB2312_CHARSET;
    lstrcpy(logfont.lfFaceName, L"华文行楷");
    title_font = font::make_font(CreateFontIndirect(&logfont));

    logfont.lfHeight = 40;
    logfont.lfWidth = 30;
    lstrcpy(logfont.lfFaceName, L"宋体");
    btn_font = font::make_font(CreateFontIndirect(&logfont));

    logfont.lfHeight = 13;
    logfont.lfWidth = 9;
    normal_font = font::make_font(CreateFontIndirect(&logfont));
  }
};
}  // namespace ik