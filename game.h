#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include "animation.h"
#include "ball.h"

#define TASK_PARAM ik::type::time_point now, ik::game &g

namespace ik {

struct circulation_task {
  type::duration duration;
  type::time_point last_run;
  std::function<bool(type::time_point, game&)> task;
};

class game : public animation {
 private:
  std::map<type::time_point, std::function<void(type::time_point, game&)>>
      _timed_tasks;
  std::list<circulation_task> _circulation_tasks;
  type::time_point _prev_paint;

 public:
  std::map<std::string, std::shared_ptr<text>> buttons;
  std::vector<std::shared_ptr<ball>> balls;

  void add_timed_task(type::duration duration,
                      std::function<void(type::time_point, game&)> task) {
    _timed_tasks.insert({type::clock::now() + duration, task});
  }

  void clear_timed_tasks() { _timed_tasks.clear(); }

  void add_circulation_task(type::duration duration,
                            std::function<bool(type::time_point, game&)> task) {
    _circulation_tasks.push_back({duration, type::clock::now(), task});
  }

  void clear_circulation_tasks() { _circulation_tasks.clear(); }

  void start(context& ctx) {
    animation::start();

    add_circulation_task(type::duration(500), [&](TASK_PARAM) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dist(ctx.ball_radius,
                                           ctx.width - ctx.ball_radius);
      auto b = std::make_shared<ball>(ball::Green, ctx);
      b->rect(b->rect().copy({{{type::rect::X, dist(gen)}}}));
      b->start();
      b->fns_on_btn_down.push_back(
          {"ball_click", [b](const type::point& pt) { b->stop(); }});
      balls.push_back(b);
      return false;
    });
  }

  void message(const std::wstring& txt, type::duration close, context& ctx) {
    if (buttons.find("message") == buttons.end()) {
      buttons["message"] = std::make_shared<text>();
      buttons["message"]->real_rect(
          type::rect(0, ctx.height / 3, ctx.width, 100));
      buttons["message"]->font = ctx.btn_font;
    }
    buttons["message"]->text = txt;
    buttons["message"]->show();
    add_timed_task(close, [&](TASK_PARAM) { buttons["message"]->hide(); });
  }

  game() {
    fns_on_tick.push_back(
        {"game_on_tick", [this](type::time_point now) { process_tasks(now); }});
  }

  void process_tasks(type::time_point now) {
    {
      auto it = _timed_tasks.begin();
      while (it != _timed_tasks.end()) {
        if ((*it).first <= now) {
          (*it).second(now, *this);
          it = _timed_tasks.erase(it);
        } else {
          break;
        }
      }
    }
    {
      auto it = _circulation_tasks.begin();
      while (it != _circulation_tasks.end()) {
        bool erase = false;
        if (it->last_run + it->duration < now) {
          erase = it->task(now, *this);
          it->last_run = now;
        };
        if (erase)
          it = _circulation_tasks.erase(it);
        else
          it++;
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
    for (auto& obj : buttons) obj.second->paint(hdc, ctx);
    for (auto& obj : balls) obj->paint(hdc, ctx);

    SelectObject(hdc, ctx.normal_font->hfont);
    TextOut(hdc, 0, 0, fps.c_str(), fps.length());
  }
};

}  // namespace ik

extern ik::game game;
