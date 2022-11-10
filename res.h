#pragma once

#include "base.h"

namespace ik {

class image {
 public:
  using pimage = std::shared_ptr<image>;
  HBITMAP hbmp = nullptr;
  BITMAP bmp = {};
  HDC hdc = nullptr;

  ~image() {
    if (hbmp) DeleteObject(hbmp);
    if (hdc) DeleteDC(hdc);
  }

  static pimage make_image(HINSTANCE hinst, HDC _hdc, int i) {
    return pimage(new image(hinst, _hdc, i));
  }

 private:
  image() {}
  image(HINSTANCE hinst, HDC _hdc, int i) {
    hbmp = LoadBitmap(hinst, MAKEINTRESOURCE(i));
    GetObject(hbmp, sizeof(bmp), &bmp);
    hdc = CreateCompatibleDC(_hdc);
    SelectObject(hdc, hbmp);
  }
};

class font {
 public:
  using pfont = std::shared_ptr<font>;
  HFONT hfont = nullptr;
  ~font() {
    if (hfont) DeleteObject(hfont);
  }

  static pfont make_font(HFONT hfont) { return pfont(new font(hfont)); }

 private:
  font(HFONT _hfont) : hfont(_hfont) {}
};

}  // namespace ik