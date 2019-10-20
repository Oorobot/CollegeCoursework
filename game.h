#pragma once

#include <iostream>
#include "button.h"

namespace ik {
class game : public object {
 public:
  button start;

 public:
  void on_tick(type::time_point now) {}

  game() : start((type::rect(100, 300, 100, 30))) { start.visible(true); }
  void paint(HDC hdc, context& ctx) const {
    StretchBlt(hdc, 0, 0, ctx.width, ctx.height, ctx.bg->hdc, 0, 0,
               ctx.bg->bmp.bmWidth, ctx.bg->bmp.bmHeight, SRCCOPY);
    start.paint(hdc, ctx);
  }
};

}  // namespace ik

extern ik::game game;
