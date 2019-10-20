#pragma once

#include <windows.h>
#include <chrono>
#include <memory>
#include "sigslot.h"

#include "context.h"

namespace ik {

namespace type {
using clock = std::chrono::system_clock;
using time_point = clock::time_point;
using duration = std::chrono::milliseconds;

class rect {
 public:
  enum Field { X, Y, Width, Height };

 public:
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  rect() {}
  rect(int _x, int _y, int _width, int _height)
      : x(_x), y(_y), width(_width), height(_height) {}

  rect(const RECT& rc)
      : x(rc.left),
        y(rc.top),
        width(rc.right - rc.left),
        height(rc.bottom - rc.top) {}

  rect copy(Field field, int value) const {
    rect new_rect = *this;
    switch (field) {
      case Field::X:
        new_rect.x = value;
        break;
      case Field::Y:
        new_rect.y = value;
        break;
      case Field::Width:
        new_rect.width = value;
        break;
      case Field::Height:
        new_rect.height = value;
        break;
      default:
        break;
    }
    return new_rect;
  }

  RECT to_RECT() const { return RECT{x, y, x + width, y + height}; }
};

class point {
 public:
  int x, y;

 public:
  point() : x(0), y(0) {}
  point(int _x, int _y) : x(_x), y(_y) {}

  bool in(const rect& rc) const {
    return x >= rc.x && x <= rc.x + rc.width && y >= rc.y &&
           y <= rc.y + rc.height;
  }
};

}  // namespace type

namespace signal {
extern sigslot::signal1<type::time_point> tick_signal;
extern sigslot::signal1<const type::point&> btn_down_signal;
extern sigslot::signal1<const type::point&> btn_up_signal;
extern sigslot::signal1<const type::point&> dbl_click_signal;
}  // namespace signal

class object : public sigslot::has_slots<> {
 protected:
  type::rect _rect;
  bool _visible = true;

 public:
  type::rect rect() const { return _rect; }
  void rect(const type::rect& rect) { _rect = rect; }

  bool visible() const { return _visible; }
  void visible(bool vis) { _visible = vis; }

 public:
  object() {
    signal::tick_signal.connect(this, &object::on_tick);
    signal::btn_down_signal.connect(this, &object::on_click);
    signal::dbl_click_signal.connect(this, &object::on_dbl_click);
  }
  enum { TickDuration = 35 };
  virtual void on_tick(type::time_point now) {}
  virtual void on_click(const type::point&) {}
  virtual void on_dbl_click(const type::point&) {}

  virtual void paint(HDC hdc, context& con) const {}
  virtual ~object() {}
};

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
}  // namespace ik
