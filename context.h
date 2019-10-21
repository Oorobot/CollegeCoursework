#pragma once

#include <Windows.h>
#include <memory>
#include "res.h"

#include "resource.h"

namespace ik {

struct context {
  int width = 0, height = 0;
  pimage bg, red, blue, green, yellow;
  pfont title_font, btn_font;

  context() {
    width = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * 2 / 5.0);
    height = GetSystemMetrics(SM_CYSCREEN);
  }
  context(HINSTANCE hinst, HDC hdc) : context() {
    bg = make_image(hinst, hdc, IDB_BG);
    red = make_image(hinst, hdc, IDB_RED);
    blue = make_image(hinst, hdc, IDB_BLUE);
    green = make_image(hinst, hdc, IDB_GREEN);
    yellow = make_image(hinst, hdc, IDB_YELLOW);

    LOGFONT logfont;
    memset(&logfont, 0, sizeof(logfont));
    logfont.lfHeight = 45;
    logfont.lfWidth = 30;
    logfont.lfUnderline = 0;
    logfont.lfItalic = 0;
    logfont.lfCharSet = GB2312_CHARSET;
    lstrcpy(logfont.lfFaceName, L"华文行楷");

    title_font = make_font(CreateFontIndirect(&logfont));

    memset(&logfont, 0, sizeof(logfont));
    logfont.lfHeight = 30;
    logfont.lfWidth = 30;
    logfont.lfUnderline = 1;
    logfont.lfItalic = 0;
    logfont.lfCharSet = GB2312_CHARSET;
    lstrcpy(logfont.lfFaceName, L"宋体");
    btn_font = make_font(CreateFontIndirect(&logfont));
  }
};
}  // namespace ik