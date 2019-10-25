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
  std::map<ball::BallType, int> scores;

 public:
  std::map<std::string, std::shared_ptr<text>> buttons;
  std::vector<std::shared_ptr<ball>> balls;
  std::function<void(int, type::duration)> finish_fn;

  void add_timed_task(type::duration duration,
                      std::function<void(type::time_point, game&)> task) {
    _timed_tasks.insert({type::clock::now() + duration, task});
  }

  void add_circulation_task(type::duration duration,
                            std::function<bool(type::time_point, game&)> task) {
    _circulation_tasks.push_back({duration, type::clock::now(), task});
  }

  void start(context& ctx) {
    animation::start();

    add_circulation_task(type::duration(500), [&](TASK_PARAM) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> type_dist(ball::Green, ball::Blue),
          x_dist(0, ctx.width - ctx.ball_radius * 2);

      auto b = std::make_shared<ball>(
          static_cast<ball::BallType>(type_dist(gen)), ctx);
      b->eliminated_fn = [this, &ctx](ball::BallType type) {
        scores[type]++;
        check_tool(ctx);
      };
      b->rect(b->rect().copy({{type::rect::X, x_dist(gen)}}));
      b->start();
      balls.push_back(b);
      return !started();
    });
  }

  void check_tool(context& ctx) {
    if (scores[ball::Green] % ctx.green_tool == 0) {
      message(L"获得道具：回复", type::duration(1000), ctx);
      add_timed_task(type::duration(500), [this, &ctx](TASK_PARAM) {
        auto it = balls.begin();
        while (it != balls.end()) {
          if ((*it)->status == ball::Top) {
            it = balls.erase(it);
          } else {
            ++it;
          }
        }
      });
    } else if (scores[ball::Red] % ctx.red_tool == 0) {
      message(L"获得道具：灭迹", type::duration(1000), ctx);
      add_timed_task(type::duration(500),
                     [this, &ctx](TASK_PARAM) { balls.clear(); });
    }
  }

  void stop(int score, type::duration d, context& ctx) {
    animation::stop();
    reset();
    finish_fn(score, d);
  }

  void reset() {
    _timed_tasks.clear();
    _circulation_tasks.clear();
    scores.clear();
    balls.clear();
    buttons.clear();
    signal::tick_signal.disconnect_all();
    signal::tick_signal.connect(dynamic_cast<object*>(this), &object::on_tick);
  }

  void message(const std::wstring& txt, type::duration close, context& ctx) {
    if (buttons.find("message") == buttons.end()) {
      buttons["message"] = std::make_shared<text>();
      buttons["message"]->real_rect(
          type::rect(0, ctx.height / 3, ctx.width, 200));
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
    std::wstring info;
    int score = 0;
    for (auto& kv : scores) {
      score += (kv.first * kv.second);
    }
    if (started()) info += std::to_wstring(score) + L"分  ";
    info += std::to_wstring(10000000 / (now - _prev_paint).count()) + L" FPS";
    _prev_paint = now;
    StretchBlt(hdc, 0, 0, ctx.width, ctx.height, ctx.bg->hdc, 0, 0,
               ctx.bg->bmp.bmWidth, ctx.bg->bmp.bmHeight, SRCCOPY);
    for (auto& obj : buttons) obj.second->paint(hdc, ctx);
    for (auto& obj : balls) obj->paint(hdc, ctx);

    SelectObject(hdc, ctx.normal_font->hfont);
    SetTextColor(hdc, ctx.title_color);
    if (false) {
      int ball_on_board = 0;
      for (auto& b : balls)
        if (b->status == ball::Status::Top) ball_on_board++;

      Rectangle(hdc, -1, 0, ctx.width, ctx.ball_radius * 2);
      double percent = ball_on_board * 1.0 / ctx.max_board_balls;
      Rectangle(hdc, -1, 0, static_cast<int>(ctx.width * percent),
                ctx.ball_radius * 2);
      auto spercent = std::to_wstring(static_cast<int>(percent * 100)) + L" %";
      DrawText(hdc, spercent.c_str(), spercent.length(),
               &RECT({0, 0, ctx.width, ctx.ball_radius * 2}),
               DT_VCENTER | DT_CENTER | DT_SINGLELINE);
      if (percent >= 1.0) stop(score, time(), ctx);
    }
    TextOut(hdc, 5, started() ? ctx.ball_radius * 2 : 5, info.c_str(),
            info.length());
  }
};

}  // namespace ik

extern ik::game game;
