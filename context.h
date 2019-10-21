#pragma once

#include <Windows.h>
#include <memory>

#include "resource.h"

namespace ik {

struct image {
  HBITMAP hbmp = nullptr;
  BITMAP bmp = {};
  HDC hdc = nullptr;

  image() {}
  image(HINSTANCE hinst, HDC _hdc, int i) {
    hbmp = LoadBitmap(hinst, MAKEINTRESOURCE(i));
    GetObject(hbmp, sizeof(bmp), &bmp);
    hdc = CreateCompatibleDC(_hdc);
    SelectObject(hdc, hbmp);
  }

  ~image() {
    if (hbmp) DeleteObject(hbmp);
    if (hdc) DeleteDC(hdc);
  }
};

struct context {
  int width = 0, height = 0;
  std::shared_ptr<image> bg, red, blue, green, yellow;

  context() {
    width = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * 2 / 5.0);
    height = GetSystemMetrics(SM_CYSCREEN);
  }
  context(HINSTANCE hinst, HDC hdc) : context() {
    bg.reset(new image(hinst, hdc, IDB_BG));
    red.reset(new image(hinst, hdc, IDB_RED));
    blue.reset(new image(hinst, hdc, IDB_BLUE));
    green.reset(new image(hinst, hdc, IDB_GREEN));
    yellow.reset(new image(hinst, hdc, IDB_YELLOW));
  }
};
}  // namespace ik