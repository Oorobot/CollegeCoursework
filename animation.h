#pragma once

#include <cmath>
#include "base.h"

namespace ik {

class animation : public object {
  type::time_point _start_time;
  bool _started;

 public:
  animation() : _started(false) {}

  bool started() const { return _started; }

  void start() {
    _start_time = type::clock::now();
    _started = true;
  }

  bool stop() {
    if (started()) {
      _started = false;
      return true;
    } else {
      return false;
    }
  }

  type::duration time() const {
    if (started()) {
      return std::chrono::duration_cast<type::duration>(type::clock::now() -
                                                        _start_time);
    }
    return type::duration(0);
  }
};

class fly_animation : public animation {
 private:
  type::rect _real_rect;
  int _animation_duration = 500;
  bool _inplace = false;

  // y=axx+by
  double a, b;

 public:
  fly_animation(const type::rect& rc = type::rect(100, 100, 100, 100))
      : _real_rect(rc) {
    visible(false);
    rect(_real_rect.copy({{type::rect::Field::Y, 0}}));
    cal_param();

    fns_on_tick.push_back(
        {"fly_animation_on_tick", [this](type::time_point now) {
           if (started() && visible()) {
             int pt = static_cast<int>(time().count());
             if (pt >= _animation_duration) {
               if (_inplace) visible(false);
               _rect.y = _inplace ? 0 : _real_rect.y;
               stop();
               _inplace = !_inplace;
               return;
             }
             if (_inplace && pt < _animation_duration)
               pt = _animation_duration - pt;

             if (pt == 0 || _real_rect.y == 0)
               _rect.y = 0;
             else
               _rect.y = static_cast<int>(a * pt * pt + b * pt);
           }
         }});
  }

  void set_animation_duration(unsigned int value) {
    _animation_duration = static_cast<int>(value);
    cal_param();
  }

  void cal_param() {
    double k = _real_rect.y, t = static_cast<double>(_animation_duration);

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

  void real_rect(const type::rect& rc) {
    _real_rect = rc;
    rect(_real_rect.copy({{type::rect::Field::Y, 0}}));
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
