#pragma once

#include <windows.h>
#include <chrono>
#include <map>

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

  rect copy(std::map<Field, int> values) const {
    rect new_rect = *this;
    for (auto& p : values) {
      switch (p.first) {
        case Field::X:
          new_rect.x = p.second;
          break;
        case Field::Y:
          new_rect.y = p.second;
          break;
        case Field::Width:
          new_rect.width = p.second;
          break;
        case Field::Height:
          new_rect.height = p.second;
          break;
        default:
          break;
      }
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
}  // namespace ik
