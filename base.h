#pragma once

#include <chrono>
#include "sigslot.h"

namespace ik {

namespace type {
using clock = std::chrono::system_clock;
using time_point = clock::time_point;
using duration = std::chrono::milliseconds;
}  // namespace type

extern sigslot::signal1<type::time_point> ticker;

class object : public sigslot::has_slots<> {
 public:
  object() { ticker.connect(this, &object::on_tick); }
  enum { TickDuration = 50 };
  virtual void on_tick(type::time_point now) {}
  virtual void on_click(int x, int y) {}
  virtual void on_dbl_click(int x, int y) {}
};

class animation : public object {
  type::time_point _start_time;
  bool _started;

 public:
  animation() : _started(false) {}

  bool started() const { return _started; }

  bool start() { _start_time = type::clock::now(); }

  bool stop() {
    if (started()) {
      _started = false;
    } else {
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
}  // namespace ik
