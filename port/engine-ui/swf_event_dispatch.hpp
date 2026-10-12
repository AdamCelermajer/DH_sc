#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::ui {
// Original RenderFX::Event40 with character/name pointers widened. Unknown
// payload words and flag bytes remain caller-owned source data.
struct SwfEvent48 {
 std::uintptr_t character;
 const char* name;
 std::uint32_t kind,value0;
 float x,y;
 std::int32_t value1,buttons,cursor;
 std::uint8_t consumed,flag;
 std::uint16_t padding;
};
struct SwfEventServices24 {
 void* context;
 // 1 delivered, 0 unavailable. Native event handler may synchronously mutate
 // the complete event or reenter. Its original C++ return is not acceptance.
 int (*native_event)(void*,SwfEvent48*);
 // Source InvokeASCallback return is ignored; delivered absence is success.
 // Provider must execute the actual source method bridge or explicit service.
 int (*as_method)(void*,std::uintptr_t,const char*);
};
static_assert(sizeof(SwfEvent48)==48);
static_assert(offsetof(SwfEvent48,consumed)==44);
static_assert(sizeof(SwfEventServices24)==24);
}
// 0 success, -1 malformed caller, -2 required provider unavailable. Caller
// retains C strings, source global, event, services and callback receiver.
// Initial null name is outside this bounded native contract. Failure keeps
// already delivered source mutations; no missing backend is accepted.
extern "C" int dh2_ui_swf_send_event(dh2::ui::SwfEvent48*,std::uint32_t*,const dh2::ui::SwfEventServices24*);
