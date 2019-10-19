#pragma once

#include <iostream>
#include "button.h"

namespace ik {
class game : public object {
 public:
  button start;

 public:
  void on_tick(type::time_point now) {}

  game() : start((type::rect(100, 300, 100, 30))) { start.visible(true); }
  void render(HDC hdc) const { start.render(hdc); }
};

}  // namespace ik

extern ik::game game;
