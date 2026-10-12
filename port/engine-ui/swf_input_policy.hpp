#pragma once
#include <cstdint>
namespace dh2::ui {struct SwfInputHistoryFlags {std::uint8_t mouse9c,need9d,enter_e9,padding;};}
// Original notify_set_member decision/stores. 0 no source notification,
// 1 mouse9c set,2 enter_e9 set (caller MUST execute notify_need_advance on this
// character and fresh weak-parent chain),-1 malformed. Incoming value is not
// read by this source routine, including nil/non-function assignments.
extern "C" int dh2_ui_swf_note_assignment(dh2::ui::SwfInputHistoryFlags*,const char*);
