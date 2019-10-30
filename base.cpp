#include "base.h"

namespace ik {

namespace signal {
sigslot::signal1<type::time_point> tick_signal;
sigslot::signal1<const type::point&> btn_down_signal;
sigslot::signal1<const type::point&> btn_up_signal;
sigslot::signal1<const type::point&> dbl_click_signal;
sigslot::signal1<const type::point&> mouse_move_signal;
sigslot::signal1<char> key_signal;
}  // namespace signal

}  // namespace ik
