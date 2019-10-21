#pragma once

#include "fly_animation.h"

namespace ik {
class title : public fly_animation {
  LPCWSTR text = L"StopTheBall";
  int len = 0;

 public:
  title() { len = std::wcslen(text); }
  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    auto rc = this->object::rect().to_RECT();
    SelectObject(hdc, ctx.title_font->hfont);
    DrawText(hdc, text, len, &rc, DT_CENTER);
  }
};

class button : public fly_animation {
  LPCWSTR _text;
  int len = 0;

 public:
  button(LPCWSTR text) : _text(text) { len = std::wcslen(text); }
  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    auto rc = this->object::rect().to_RECT();
    SelectObject(hdc, ctx.btn_font->hfont);
    DrawText(hdc, _text, len, &rc, DT_CENTER);
  }
};

}  // namespace ik