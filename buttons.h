#pragma once

#include "fly_animation.h"

namespace ik {
class text : public fly_animation {
 public:
  std::wstring text;
  font::pfont font;
  COLORREF color = RGB(0, 0, 0);

 public:
  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    auto rc = this->object::rect().to_RECT();
    if (font) SelectObject(hdc, ctx.title_font->hfont);
    SetTextColor(hdc, color);
    DrawText(hdc, text.c_str(), text.length(), &rc, DT_CENTER);
  }
};

class button : public text {
 public:
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
    DrawText(hdc, text::text.c_str(), text::text.length(), &rc, DT_CENTER);
  }
};

}  // namespace ik