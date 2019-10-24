#pragma once

#include "base.h"
#include "game.h"

#pragma comment(lib, "Msimg32.lib")

namespace ik {

class ball : public animation {
  friend class game;

 public:
  enum BallType { Green, Yellow, Red, Blue };

  ball(BallType btype, context& ctx) {
    set_ball_type(btype, ctx);
    fns_on_tick.push_back(
        {"ball_on_tick", [this, &ctx](const type::time_point now) {
           if (started() && visible()) {
             const auto t = time().count();
             switch (_type) {
               case Green: {
                 _rect.y = ctx.height - t / 8;
               } break;
               case Yellow: {
                 _rect.y = ctx.height - t;
               } break;
               case Red: {
                 _rect.y = ctx.height - t;
               } break;
               case Blue: {
                 _rect.y = ctx.height - t;
               } break;
               default:
                 break;
             }
             if (_rect.y <= 0) stop();
           }
         }});
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
    rect(_rect.copy({{type::rect::Width, _ball_image->bmp.bmWidth},
                     {type::rect::Height, _ball_image->bmp.bmHeight}}));
  }
  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    TransparentBlt(hdc, _rect.x, _rect.y, ctx.ball_radius * 2,
                   ctx.ball_radius * 2, _ball_image->hdc, 0, 0, _rect.width,
                   _rect.height, ctx.transparent_color);
  }

 private:
  BallType _type = BallType::Green;
  image::pimage _ball_image = nullptr;
};
}  // namespace ik
