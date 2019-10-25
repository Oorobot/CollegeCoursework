#pragma once

#include <Windows.h>
#include <cmath>
#include <functional>
#include <memory>
#include <random>
#include <vector>
#include "res.h"
#include "resource.h"
#include "type.h"

namespace ik {

using animation_fn = std::function<type::point(int, int, int)>;

struct context {
  int green_tool = 5, yellow_tool = 60, red_tool = 5, blue_tool = 40;

  int width = 0, height = 0;
  int speed = 16;
  int ball_radius = 16;
  int max_board_balls = 50;
  image::pimage bg, red, blue, green, yellow;
  font::pfont title_font, btn_font, normal_font;
  int tick_duration = 30;
  COLORREF title_color = RGB(0, 0, 0), mouse_hover_color = RGB(255, 0, 0),
           mouse_down_color = RGB(0, 255, 0), normal_color = RGB(0, 0, 255);

  COLORREF transparent_color = RGB(255, 255, 255);

  context() {
    width = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * (2 / 5.0));
    height = static_cast<int>(GetSystemMetrics(SM_CYSCREEN) * (6 / 7.0));
  }

  context(HINSTANCE hinst, HDC hdc) : context() {
    // images
    bg = image::make_image(hinst, hdc, IDB_BG);
    red = image::make_image(hinst, hdc, IDB_RED);
    blue = image::make_image(hinst, hdc, IDB_BLUE);
    green = image::make_image(hinst, hdc, IDB_GREEN);
    yellow = image::make_image(hinst, hdc, IDB_YELLOW);

    // fonts
    LOGFONT logfont;
    memset(&logfont, 0, sizeof(logfont));
    logfont.lfHeight = 60;
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