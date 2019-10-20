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
    if (hbmp) {
      DeleteObject(hbmp);
    }
    if (hdc) {
      DeleteDC(hdc);
    }
  }
};

struct context {
  int width = 0, height = 0;
  std::shared_ptr<image> bg;

  context() {
    width = GetSystemMetrics(SM_CXSCREEN) * 2 / 5.0;
    height = GetSystemMetrics(SM_CYSCREEN);
  }
  context(HINSTANCE hinst, HDC hdc) {
    width = GetSystemMetrics(SM_CXSCREEN) * 2 / 5.0;
    height = GetSystemMetrics(SM_CYSCREEN);

    bg.reset(new image(hinst, hdc, IDB_BG));
  }
};
}  // namespace ik