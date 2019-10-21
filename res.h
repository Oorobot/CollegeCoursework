#pragma once

#include "base.h"

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

using pimage = std::shared_ptr<image>;

inline pimage make_image(HINSTANCE hinst, HDC _hdc, int i) {
  return std::make_shared<image>(hinst, _hdc, i);
}

struct font {
  HFONT hfont = nullptr;

  font(HFONT _hfont) : hfont(_hfont) {}
  ~font() {
    if (hfont) DeleteObject(hfont);
  }
};

using pfont = std::shared_ptr<font>;

inline pfont make_font(HFONT hfont) { return std::make_shared<font>(hfont); }

}  // namespace ik