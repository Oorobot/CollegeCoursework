#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include "animation.h"

#define TASK_PARAM ik::type::time_point now, ik::game &g

namespace ik {
class game : public object {
 private:
  std::map<type::time_point, std::function<void(type::time_point, game&)>>
      _tasks;
  type::time_point _prev_paint;

 public:
  std::map<std::string, std::shared_ptr<text>> buttons;

  void add_task(type::duration duration,
                std::function<void(type::time_point, game&)> task) {
    _tasks.insert({type::clock::now() + duration, task});
  }

  void clear_all_tasks() { _tasks.clear(); }

  void message(const std::wstring& txt, type::duration close, context& ctx) {
    if (buttons.find("message") == buttons.end()) {
      buttons["message"] = std::make_shared<text>();
      buttons["message"]->real_rect(
          type::rect(0, ctx.height / 3, ctx.width, 100));
      buttons["message"]->font = ctx.btn_font;
    }
    buttons["message"]->text = txt;
    buttons["message"]->show();
    add_task(close, [&](TASK_PARAM) { buttons["message"]->hide(); });
  }

  game() {
    fns_on_tick.push_back({"game_on_tick", [this](type::time_point now) {
                             auto it = _tasks.begin();
                             while (it != _tasks.end()) {
                               if ((*it).first <= now) {
                                 (*it).second(now, *this);
                                 it = _tasks.erase(it);
                               } else {
                                 break;
                               }
                             }
                           }});
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
