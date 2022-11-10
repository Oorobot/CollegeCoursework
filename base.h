#pragma once

#include <windows.h>
#include <chrono>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "context.h"
#include "type.h"

namespace ik {

class object;

namespace signals {

template <typename P>
class signal : public std::list<std::pair<object*, void (object::*)(P)>> {
 public:
  void connect(object* pobj, void (object::*pfunc)(P)) {
    this->push_back({pobj, pfunc});
  }

  void disconnect(object* pobj) {
    auto it =
        std::find_if(std::begin(*this), std::end(*this),
                     [pobj](const std::pair<object*, void (object::*)(P)>& p) {
                       return p.first == pobj;
                     });
    if (it != this->end()) this->erase(it);
  }

  void disconnect_all() { this->clear(); }

  void emit(P p) {
    for (auto& pair : *this) {
      std::invoke(pair.second, *(pair.first), p);
    }
  }
};

extern signal<type::time_point> tick_signal;
extern signal<const type::point&> btn_down_signal;
extern signal<const type::point&> btn_up_signal;
extern signal<const type::point&> dbl_click_signal;
extern signal<const type::point&> mouse_move_signal;
extern signal<char> key_signal;

}  // namespace signals

class object {
 protected:
  type::rect _rect;
  bool _visible = true;
  bool _mouse_in = false;
  bool _mouse_down = false;

 public:
  std::list<std::pair<std::string, std::function<void(const type::point&)>>>
      fns_on_btn_down, fns_on_btn_up, fns_on_dbl_click, fns_on_mouse_move;

  std::list<std::pair<std::string, std::function<void(char)>>> fns_on_key;

  std::list<
      std::pair<std::string, std::function<void(const type::time_point&)>>>
      fns_on_tick;

 public:
  void on_btn_down(const type::point& pt) {
    if (!visible()) return;
    if (pt.in(_rect)) {
      _mouse_down = true;
      for (auto& fn : fns_on_btn_down) fn.second(pt);
    }
  }

  void on_btn_up(const type::point& pt) {
    if (!visible()) return;
    _mouse_down = false;
    if (pt.in(_rect))
      for (auto& fn : fns_on_btn_up) fn.second(pt);
  }

  void on_dbl_click(const type::point& pt) {
    if (!visible()) return;
    if (pt.in(_rect))
      for (auto& fn : fns_on_dbl_click) fn.second(pt);
  }

  void on_mouse_move(const type::point& pt) {
    if (!visible()) return;
    _mouse_in = pt.in(_rect);
    if (_mouse_in)
      for (auto& fn : fns_on_mouse_move) fn.second(pt);
  }

  void on_tick(type::time_point now) {
    for (auto& fn : fns_on_tick) fn.second(now);
  }

  void on_key(char key) {
    for (auto& fn : fns_on_key) fn.second(key);
  }

 public:
  type::rect rect() const { return _rect; }
  virtual void rect(const type::rect& rect) { _rect = rect; }

  bool visible() const { return _visible; }
  void visible(bool vis) { _visible = vis; }

 public:
  object() {
    signals::tick_signal.connect(this, &object::on_tick);
    signals::btn_down_signal.connect(this, &object::on_btn_down);
    signals::btn_up_signal.connect(this, &object::on_btn_up);
    signals::dbl_click_signal.connect(this, &object::on_dbl_click);
    signals::mouse_move_signal.connect(this, &object::on_mouse_move);
    signals::key_signal.connect(this, &object::on_key);
  }

  virtual void paint(HDC hdc, context& ctx) const {}
  virtual ~object() {
    signals::tick_signal.disconnect(this);
    signals::btn_down_signal.disconnect(this);
    signals::btn_up_signal.disconnect(this);
    signals::dbl_click_signal.disconnect(this);
    signals::mouse_move_signal.disconnect(this);
    signals::key_signal.disconnect(this);
  }
};
}  // namespace ik
