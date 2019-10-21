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
    SetTextColor(hdc, ctx.title_color);
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
    if (!_mouse_in)
      SetTextColor(hdc, ctx.normal_color);
    else {
      if (_mouse_down)
        SetTextColor(hdc, ctx.mouse_down_color);
      else
        SetTextColor(hdc, ctx.mouse_hover_color);
    }
    DrawText(hdc, _text, len, &rc, DT_CENTER);
  }
};

}  // namespace ik