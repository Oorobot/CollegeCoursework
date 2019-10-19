#pragma once

#include "base.h"
#include "game.h"

namespace ik {

class ball : public animation {
  friend class game;

 public:
  enum BallType { Green, Yellow, Red, Blue };

 private:
  BallType _type = BallType::Green;
};
}  // namespace ik
