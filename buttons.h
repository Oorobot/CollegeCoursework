#pragma once

#include "button.h"

namespace ik {
class title : public button {
  LPCWSTR text = L"StopTheBall";
  int len = 0;

 public:
  title() { len = std::wcslen(text); }
  void paint(HDC hdc, context& con) const {
    auto rc = this->object::rect().to_RECT();
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, text, len, &rc, DT_CENTER);
  }
};
}  // namespace ik