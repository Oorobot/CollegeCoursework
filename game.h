#pragma once

#include <functional>
#include <iostream>
#include <map>
#include "button.h"

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
  std::map<std::string, std::shared_ptr<button>> buttons;

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

  game() {}
  void paint(HDC hdc, context& ctx) const {
    StretchBlt(hdc, 0, 0, ctx.width, ctx.height, ctx.bg->hdc, 0, 0,
               ctx.bg->bmp.bmWidth, ctx.bg->bmp.bmHeight, SRCCOPY);
    for (auto& obj : buttons) obj.second->paint(hdc, ctx);
  }
};

}  // namespace ik

extern ik::game game;
