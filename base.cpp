#include "base.h"

namespace ik {

namespace signals {
signal<type::time_point> tick_signal;
signal<const type::point&> btn_down_signal;
signal<const type::point&> btn_up_signal;
signal<const type::point&> dbl_click_signal;
signal<const type::point&> mouse_move_signal;
signal<char> key_signal;
}  // namespace signals

}  // namespace ik
