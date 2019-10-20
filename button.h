#pragma once

#include <cmath>
#include "base.h"

namespace ik {
class button : public animation {
 private:
  type::rect _real_rect;
  enum { AnimationDuration = 500 };

  // y=axx+by
  double a, b;

 public:
  button(const type::rect& rc = type::rect(100, 100, 100, 100))
      : _real_rect(rc) {
    rect(_real_rect);
    double k = _real_rect.y, t = static_cast<double>(AnimationDuration);

    a = (-2 * std::sqrt(10) * std::sqrt((k + 10) * std::pow(t, 4)) -
         k * std::pow(t, 2) - 20 * std::pow(t, 2)) /
        std::pow(t, 4);
    b = (2 *
         (-std::sqrt(10) * std::sqrt((k + 10) * std::pow(t, 4)) +
          k * std::pow(t, 2) + 10 * std::pow(t, 2)) *
         (2 * std::sqrt(10) * sqrt((k + 10) * std::pow(t, 4)) +
          k * std::pow(t, 2) + 20 * std::pow(t, 2))) /
        (k * std::pow(t, 5));
  }

  void on_tick(type::time_point now) {
    if (started() && visible()) {
      int pt = time().count();
      if (pt >= AnimationDuration) {
        _rect.y = _real_rect.y;
        stop();
        return;
      }
      if (pt == 0 || _real_rect.y == 0)
        _rect.y = 0;
      else
        _rect.y = a * pt * pt + b * pt;
    }
  }

  void paint(HDC hdc, context& ctx) const {
    if (!visible()) return;
    Rectangle(hdc, _rect.x, _rect.y, _rect.x + _rect.width,
              _rect.y + _rect.height);
  }
};
}  // namespace ik
