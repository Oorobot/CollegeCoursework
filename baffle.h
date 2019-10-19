#pragma once

#include "base.h"
#include "game.h"

namespace ik {

class baffle : public object {
  friend class game;

 private:
  int _balls = 0;
  enum { MaxBallNum = 60 };

 public:
  baffle() {}

 private:
};

}  // namespace ik
