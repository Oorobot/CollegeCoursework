#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include "fly_animation.h"

#define TASK_PARAM ik::type::time_point now, ik::game &g

namespace ik {
class game : public object {
 private:
  std::map<type::time_point, std::function<void(type::time_point, game&)>>
      _tasks;

 public:
  void add_task(type::duration duration,
                std::function<void(type::time_point, game&)> task) {
    _tasks.insert({type::clock::now() + duration, task});
  }

  void clear_all_tasks() { _tasks.clear(); }

 public:
  std::map<std::string, std::shared_ptr<text>> buttons;

  type::time_point _prev_paint;

 public:
  void on_tick(type::time_point now) {
    auto it = _tasks.begin();
    while (it != _tasks.end()) {
      if ((*it).first <= now) {
        (*it).second(now, *this);
        it = _tasks.erase(it);
      } else {
        break;
      }
    }
  }

  void paint(HDC hdc, context& ctx) {
    auto now = type::clock::now();
    auto fps =
        std::to_wstring(10000000 / (now - _prev_paint).count()) + L" FPS";
    _prev_paint = now;
    StretchBlt(hdc, 0, 0, ctx.width, ctx.height, ctx.bg->hdc, 0, 0,
               ctx.bg->bmp.bmWidth, ctx.bg->bmp.bmHeight, SRCCOPY);
    SelectObject(hdc, ctx.normal_font->hfont);
    TextOut(hdc, 0, 0, fps.c_str(), fps.length());
    for (auto& obj : buttons) obj.second->paint(hdc, ctx);
  }
};

}  // namespace ik

extern ik::game game;
