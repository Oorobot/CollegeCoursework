#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include "animation.h"
#include "ball.h"
#include "buttons.h"

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
  std::map<ball::BallType, int> prev_tool_nums;
  std::map<ball::BallType, int> tool_count;

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
      if (ctx.speed != ctx.default_speed) return !started();
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

    fns_on_key.push_back(
        {"game_on_key", [this, &ctx](char c) { use_tool(c, ctx); }});
  }

#define CHECK_TOOL_COUNT(type)      \
  if (tool_count[type] <= 0) break; \
  tool_count[type]--;

  void use_tool(char t, context& ctx) {
    if (!started()) return;
    switch (t) {
      case 'G':
        CHECK_TOOL_COUNT(ball::Green);
        message(L"使用道具：回复", type::duration(1000), ctx);
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
        break;
      case 'Y':
        break;
      case 'R':
        CHECK_TOOL_COUNT(ball::Red);
        message(L"使用道具：灭迹", type::duration(1000), ctx);
        add_timed_task(type::duration(500),
                       [this, &ctx](TASK_PARAM) { balls.clear(); });
        break;
      case 'B':
        CHECK_TOOL_COUNT(ball::Blue);
        message(L"使用道具：时光倒流", type::duration(1000), ctx);
        add_timed_task(type::duration(500), [this, &ctx](TASK_PARAM) {
          ctx.speed = ctx.default_speed * 2;
        });
        break;
      default:
        break;
    }
  }

  void check_tool(context& ctx) {
    if (scores[ball::Green] % ctx.green_tool == 0 &&
        scores[ball::Green] != prev_tool_nums[ball::Green]) {
      prev_tool_nums[ball::Green] = scores[ball::Green];
      tool_count[ball::Green]++;
      message(L"获得道具：回复", type::duration(1000), ctx);
    } else if (scores[ball::Red] % ctx.red_tool == 0 &&
               scores[ball::Red] != prev_tool_nums[ball::Red]) {
      prev_tool_nums[ball::Red] = scores[ball::Red];
      message(L"获得道具：灭迹", type::duration(1000), ctx);
      tool_count[ball::Red]++;
    } else if (scores[ball::Blue] % ctx.blue_tool == 0 &&
               scores[ball::Blue] != prev_tool_nums[ball::Blue] &&
               ctx.speed == ctx.default_speed) {
      prev_tool_nums[ball::Blue] = scores[ball::Blue];
      message(L"获得道具：时光倒流", type::duration(1000), ctx);
      tool_count[ball::Blue]++;
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
    prev_tool_nums.clear();
    balls.clear();
    buttons.clear();
    tool_count.clear();
    fns_on_key.clear();
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
    if (started()) {
      int ball_on_board = 0, ball_running = 0;
      for (auto& b : balls) {
        if (b->status == ball::Status::Top)
          ball_on_board++;
        else if (b->status == ball::Status::Run)
          ball_running++;
      }

      if (ctx.speed != ctx.default_speed && ball_running == 0) {
        message(L"时光倒流 结束", type::duration(1000), ctx);
        ctx.speed = ctx.default_speed;
      }

      wchar_t fmt_buf[50] = {0};
      info.push_back('\n');

      wsprintf(fmt_buf, L"绿色 回复(G)*%d %d/%d\0", tool_count[ball::Green],
               scores[ball::Green],
               ((scores[ball::Green] + ctx.green_tool) / ctx.green_tool) *
                   ctx.green_tool);
      info += fmt_buf;
      info.push_back('\n');

      wsprintf(
          fmt_buf, L"红色 灭迹(R)*%d %d/%d\0", tool_count[ball::Red],
          scores[ball::Red],
          ((scores[ball::Red] + ctx.red_tool) / ctx.red_tool) * ctx.red_tool);
      info += fmt_buf;
      info.push_back('\n');

      wsprintf(fmt_buf, L"蓝色 时光倒流(B)*%d %d/%d\0", tool_count[ball::Blue],
               scores[ball::Blue],
               ((scores[ball::Blue] + ctx.blue_tool) / ctx.blue_tool) *
                   ctx.blue_tool);
      info += fmt_buf;
      info.push_back('\n');

      wsprintf(fmt_buf, L"黄色 %d\0", scores[ball::Yellow]);
      info += fmt_buf;
      info.push_back('\n');

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
    DrawText(hdc, info.c_str(), info.length(),
             &RECT({5, started() ? ctx.ball_radius * 2 : 5, 300, 300}),
             DT_LEFT | DT_TOP);
  }
};

}  // namespace ik

extern ik::game game;
