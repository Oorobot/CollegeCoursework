#pragma once

#include <iostream>

#include "base.h"

namespace ik {
class game : public object {
 public:
  void on_tick(type::time_point now) {}

  void render(HDC hdc) const {}
};

}  // namespace ik

extern ik::game game;
