#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace dh2::input_manager_v1 {
// Portable dispatch metadata replaces ARM vtable addresses. These words are
// never executable pointers; the native functions below implement dispatch.
enum class Dispatch : std::uint32_t { device, keyboard, mouse, gamepad, manager, win32 };
struct Device {
    Dispatch dispatch = Dispatch::device;
    std::uint32_t type = 0, field_8 = 0;
};
struct Channel {
    float value = 0, minimum = 0, maximum = 0, previous = 0;
    float field_10 = 0, previous_minimum = 0, previous_maximum = 0;
    std::uint8_t previous_above_threshold = 0;
    std::uint8_t padding[3]{};
};
struct Point { float x = 0, y = 0, z = 0; };
struct Keyboard { Device device; std::array<Channel,97> keys; };
struct Mouse { Device device; std::array<Channel,8> buttons, axes; };
struct Gamepad {
    Device device;
    std::array<Channel,45> buttons;
    std::array<Channel,9> axes;
    std::array<Point,4> first_vectors;
    std::array<std::int32_t,4> first_stamps;
    std::array<Point,4> repeat_vectors;
    std::array<std::int32_t,4> repeat_stamps;
    std::uint8_t vibration_74c = 0, padding_74d[3]{};
    std::uint32_t field_750 = 0, field_754 = 0;
    std::uint8_t connected_758 = 0, padding_759[3]{};
};
struct Base {
    Dispatch dispatch = Dispatch::manager;
    std::int32_t mice = 0, keyboards = 0, gamepads = 0;
    std::uint8_t enabled_10 = 0, padding_11[7]{};
};
struct Manager {
    Base base;
    Keyboard keyboard;
    Mouse mouse;
    std::array<Gamepad,4> gamepads;
};
// Borrow the original Vec3f_Origin value. It is not an input/timer store.
struct Globals { Point origin; };
enum class Status { complete, invalid_argument, missing_provider, provider_failed };
struct Clock {
    void* context = nullptr;
    // Original clock() units and signed 32-bit wrapping are preserved. Do not
    // convert the 200/400 tick comparisons into assumed milliseconds.
    std::int32_t (*read)(void*, std::int32_t*) = nullptr;
};
struct Result {
    Status status = Status::complete;
    std::uint32_t clock_reads = 0, updated_channels = 0, updated_devices = 0;
    std::uint32_t gamepad_index = 0, vector_index = 0;
};
// Reached source stores only: constructors retain untouched padding/residue.
void construct_device(Device&, std::uint32_t type);
void construct_keyboard(Keyboard&);
void construct_mouse(Mouse&);
void construct_gamepad(Gamepad&, const Globals&);
void construct_base(Base&, std::int32_t mice, std::int32_t keyboards, std::int32_t gamepads);
void construct_win32(Manager&, const Globals&);
std::int32_t num_mice(const Base&);
std::int32_t num_keyboards(const Base&);
std::int32_t num_gamepads(const Base&);
// Source Win32 mouse/keyboard getters ignore the index. Gamepad's valid source
// span is four entries; malformed indices return nullptr rather than exposing
// its original assertion/OOB path as a native memory access.
Mouse* get_mouse(Manager&, std::int32_t index);
Keyboard* get_keyboard(Manager&, std::int32_t index);
Gamepad* get_gamepad(Manager&, std::int32_t index);
std::int32_t connected_gamepad_count(Manager&);
Gamepad* first_connected_gamepad(Manager&);
void update_keyboard(Keyboard&, Result&);
void update_mouse(Mouse&, Result&);
Status update_gamepad(Gamepad&, const Globals&, const Clock*, Result&);
Status update_frame(Manager&, const Globals&, const Clock*, Result&);

static_assert(sizeof(Device)==12 && sizeof(Channel)==32 && sizeof(Point)==12);
static_assert(sizeof(Keyboard)==0xc2c && sizeof(Mouse)==0x20c && sizeof(Gamepad)==0x75c);
static_assert(offsetof(Gamepad,axes)==0x5ac && offsetof(Gamepad,first_vectors)==0x6cc);
static_assert(offsetof(Gamepad,repeat_vectors)==0x70c && offsetof(Gamepad,connected_758)==0x758);
static_assert(offsetof(Manager,keyboard)==0x18 && offsetof(Manager,mouse)==0xc44);
static_assert(offsetof(Manager,gamepads)==0xe50 && sizeof(Manager)==0x2bc0);
} // namespace dh2::input_manager_v1
