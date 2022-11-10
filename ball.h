#pragma once

#include <random>
#include "base.h"
#include "game.h"

#pragma comment(lib, "Msimg32.lib")

namespace ik {

class ball : public animation {
  friend class game;

 public:
  enum BallType { Green = 1, Red = 2, Yellow = 3, Blue = 4 };
  enum Status { Top, Run, Bottom };

  std::function<void(BallType)> eliminated_fn;

  int clicked_time = 0;
  double down_speed = 0;
  int down_start = -100;
  Status status = Bottom;
  int rand_num;

  ball(BallType type, context& ctx) {
    std::random_device rd;
    std::uniform_int_distribution<> dist(1, 3);
    rand_num = dist(rd);
    fns_on_btn_down.push_back(
        {"ball_click", [this, &ctx](const type::point& pt) {
           if (status != Run) return;
           ++clicked_time;
           if ((_type == Red && clicked_time >= 2) ||
               (_type != Red && clicked_time >= 1)) {
             if (!down_speed) {
               down(ctx);
               status = Bottom;
               eliminated_fn(_type);
             }
           }
         }});
    fns_on_mouse_move.push_back(
        {"ball_mouse_move", [this, &ctx](const type::point& pt) {
           if (_type == Yellow) {
             std::random_device rd;
             std::uniform_int_distribution<> dist(-10, 10);
             int dx = dist(rd);
             while (!((dx = dist(rd)) &&
                      _rect.x + dx <= ctx.width - ctx.ball_radius * 2 &&
                      _rect.x + dx >= 0))
               ;
             _rect.x += dx;
           }
         }});
    set_ball_type(type, ctx);
    fns_on_tick.push_back(
        {"ball_on_tick",
         [this, &ctx](const type::time_point now) { on_tick(now, ctx); }});
  }

  void start() {
    if (status != Top) {
      animation::start();
      status = Run;
    }
  }

  void down(context& ctx) {
    stop();
    status = Bottom;
    down_speed = (ctx.height - _rect.y) / 500.0;
    down_start = _rect.y;
    start();
  }

  void on_tick(const type::time_point now, context& ctx) {
    if (started() && visible()) {
      const auto t = static_cast<int>(time().count());
      if (down_start == -100) {
        switch (_type) {
          case Green: {
            _rect.y = ctx.height - t / ctx.speed;
          } break;
          case Yellow: {
            _rect.y = ctx.height - t / ctx.speed;
          } break;
          case Red: {
            _rect.y = ctx.height - 2 * t / ctx.speed;
          } break;
          case Blue: {
            _rect.x = static_cast<int>(
                (ctx.width - ctx.ball_radius * 4) *
                (1 + std::sin(rand_num * t / 1000.0 + rand_num)) / 2);
            _rect.y = ctx.height - t / ctx.speed;
          } break;
          default:
            break;
        }
      } else {
        _rect.y = down_start + static_cast<int>(t * down_speed);
      }
      if (_rect.y <= 0 || _rect.y > ctx.height) {
        stop();
        if (_rect.y <= 0) status = Top;
      };
    }
  }

  void set_ball_type(BallType btype, context& ctx) {
    _type = btype;
    switch (_type) {
      case Green:
        _ball_image = ctx.green;
        break;
      case Yellow:
        _ball_image = ctx.yellow;
        break;
      case Red:
        _ball_image = ctx.red;
        break;
      case Blue:
        _ball_image = ctx.blue;
        break;
      default:
        break;
    }
    rect(_rect.copy({{type::rect::Width, ctx.ball_radius * 2},
                     {type::rect::Height, ctx.ball_radius * 2}}));
  }
  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    TransparentBlt(hdc, _rect.x, _rect.y, ctx.ball_radius * 2,
                   ctx.ball_radius * 2, _ball_image->hdc, 0, 0,
                   _ball_image->bmp.bmWidth, _ball_image->bmp.bmHeight,
                   ctx.transparent_color);
    if (_type == Red) {
      auto rc = _rect.to_RECT();
      SelectObject(hdc, ctx.normal_font->hfont);
      SetTextColor(hdc, RGB(255, 255, 255));
      DrawText(hdc, (clicked_time >= 1 ? L"1" : L"2"), 1, &rc,
               DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
  }

 private:
  BallType _type = BallType::Green;
  image::pimage _ball_image = nullptr;
};
}  // namespace ik
