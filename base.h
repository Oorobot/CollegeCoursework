#pragma once

#include <windows.h>
#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include "sigslot.h"

#include "context.h"
#include "type.h"

namespace ik {
namespace signal {
extern sigslot::signal1<type::time_point> tick_signal;
extern sigslot::signal1<const type::point&> btn_down_signal;
extern sigslot::signal1<const type::point&> btn_up_signal;
extern sigslot::signal1<const type::point&> dbl_click_signal;
extern sigslot::signal1<const type::point&> mouse_move_signal;
}  // namespace signal

class object : public sigslot::has_slots<> {
 protected:
  type::rect _rect;
  bool _visible = true;
  bool _mouse_in = false;
  bool _mouse_down = false;

 public:
  std::list<std::pair<std::string, std::function<void(const type::point&)>>>
      fns_on_btn_down, fns_on_btn_up, fns_on_dbl_click, fns_on_mouse_move;

 private:
  void on_btn_down(const type::point& pt) {
    if (pt.in(_rect)) {
      _mouse_down = true;
      for (auto& fn : fns_on_btn_down) fn.second(pt);
    }
  }

  void on_btn_up(const type::point& pt) {
    _mouse_down = false;
    if (pt.in(_rect))
      for (auto& fn : fns_on_btn_up) fn.second(pt);
  }

  void on_dbl_click(const type::point& pt) {
    if (pt.in(_rect))
      for (auto& fn : fns_on_dbl_click) fn.second(pt);
  }

  void on_mouse_move(const type::point& pt) {
    _mouse_in = pt.in(_rect);
    if (_mouse_in)
      for (auto& fn : fns_on_mouse_move) fn.second(pt);
  }

 public:
  type::rect rect() const { return _rect; }
  virtual void rect(const type::rect& rect) { _rect = rect; }

  bool visible() const { return _visible; }
  void visible(bool vis) { _visible = vis; }

 public:
  object() {
    signal::tick_signal.connect(this, &object::on_tick);
    signal::btn_down_signal.connect(this, &object::on_btn_down);
    signal::btn_up_signal.connect(this, &object::on_btn_up);
    signal::dbl_click_signal.connect(this, &object::on_dbl_click);
    signal::mouse_move_signal.connect(this, &object::on_mouse_move);
  }
  virtual void on_tick(type::time_point now) {}

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
