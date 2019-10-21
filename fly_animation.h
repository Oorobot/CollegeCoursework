#pragma once

#include <cmath>
#include "base.h"

namespace ik {
class fly_animation : public animation {
 private:
  type::rect _real_rect;
  enum { AnimationDuration = 500 };
  bool _inplace = false;

  // y=axx+by
  double a, b;

 public:
  fly_animation(const type::rect& rc = type::rect(100, 100, 100, 100))
      : _real_rect(rc) {
    visible(false);
    rect(_real_rect.copy(type::rect::Field::Y, 0));
    cal_param();
  }

  void cal_param() {
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
      int pt = static_cast<int>(time().count());
      if (pt >= AnimationDuration) {
        if (_inplace) visible(false);
        _rect.y = _inplace ? 0 : _real_rect.y;
        stop();
        _inplace = !_inplace;
        return;
      }
      if (_inplace && pt < AnimationDuration) pt = AnimationDuration - pt;

      if (pt == 0 || _real_rect.y == 0)
        _rect.y = 0;
      else
        _rect.y = static_cast<int>(a * pt * pt + b * pt);
    }
  }

  void real_rect(const type::rect& rc) {
    _real_rect = rc;
    rect(_real_rect.copy(type::rect::Field::Y, 0));
    cal_param();
  }

  void show() {
    _inplace = false;
    visible(true);
    start();
  }

  void hide() {
    _inplace = true;
    start();
  }
};
}  // namespace ik
