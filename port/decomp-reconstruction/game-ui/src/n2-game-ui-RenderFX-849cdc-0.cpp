// Batch n2-game-ui-RenderFX-849cdc-0 (RenderFX, part 1 of 4): the menu renderer's accessors, the display and focus
// switches, the constructor and the search entry points.
// Calls into gameswf use the names of the binary's vtable slots (tools/decomp_vtable.py on gameswf::character and
// gameswf::render_handler): is(int) +0x08, goto_frame(int) +0x14c, set_play_state +0x94, get_current_frame +0x138,
// get_frame_count +0x13c, set_display_callback +0x154, render_handler::set_orientation +0xa8.
// class id 2 in is(2) is the value the original passes; it is kept as a literal.
#include "dh2/RenderFX.hpp"

#include <cstring>

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_character.h"
#include "vendor-gameswf/gameswf_root.hpp"

// 0x7a7c34 RenderFX::SetWireFrame(bool)
// PARTIAL, whole body omitted. The original calls the render handler's vtable slot +0x40 (pure in gameswf::render_handler, so
// the implementing class decides). The derived class's name for that slot is not recovered, and its argument list in the
// decompilation is not reliable (this is passed where the flag is expected).
void RenderFX::SetWireFrame(bool /*enabled*/) {}

// 0x7a7c6c RenderFX::SetEventListener(RenderFX::EventListener*)
void RenderFX::SetEventListener(EventListener* listener) {
    m_eventListener = listener;
}

// 0x7a7c74 RenderFX::GetController(int): the controller record at 0x58 + 0x28 * index.
RenderFX::Controller* RenderFX::GetController(int index) {
    return &m_controllers[index];
}

// 0x7a7c88 RenderFX::SetControllerEnabled(int, bool): the enabled byte of the record (+0x24 of the record).
void RenderFX::SetControllerEnabled(int index, bool enabled) {
    m_controllers[index].m_enabled = enabled;
}

// 0x7a7c98 RenderFX::SetInputBehavior(int): stores the behavior in the word at +0xf8 (m_flags).
void RenderFX::SetInputBehavior(int behavior) {
    m_flags = static_cast<std::uint32_t>(behavior);
}

// 0x7a7ca0 RenderFX::GetFlashRoot() const: the movie character of the root (the root's m_movie, at root +0x10).
gameswf::character* RenderFX::GetFlashRoot() const {
    return m_root->m_movie.get_ptr();
}

// 0x7a7cac RenderFX::GetRoot() const: m_root (+0x3c).
gameswf::root* RenderFX::GetRoot() const {
    return m_root;
}

// 0x7a7cb4 RenderFX::SetTextBufferingEnabled(bool)
// Binary: LDR R3,[R0,#0x3C]; STRB R1,[R3,#0x85]. The root byte at +0x85 has no name that matches text buffering in
// gameswf_root.hpp (m_shift_key_state is the only name there), so the store is written at its offset.
void RenderFX::SetTextBufferingEnabled(bool enabled) {
    *(reinterpret_cast<std::uint8_t*>(m_root) + 0x85) = enabled;
}

// 0x7a7cc0 RenderFX::SetAutoLoadGlyphsEnabled(bool): stores the byte at root +0x87 (gameswf::root::unk_87, which has no
// reader in the header).
void RenderFX::SetAutoLoadGlyphsEnabled(bool enabled) {
    m_root->unk_87 = enabled;
}

// 0x7a7ccc RenderFX::SetRenderCachingEnabled(bool)
// Binary: LDR R3,[R0,#0x38]; STRB R1,[R3,#0x98]. The object at +0x38 is not typed in the header (pad_0c covers it).
void RenderFX::SetRenderCachingEnabled(bool enabled) {
    std::uint8_t* object = *reinterpret_cast<std::uint8_t**>(pad_0c + 0x2c);  // +0x38
    object[0x98] = enabled;
}

// 0x7a7cd8 RenderFX::GetAlpha(gameswf::character*)
// Binary: if character is null return 0, else load the pointer at character +0x48 and return the int at its +0x18.
// The pointer and the object behind it are not typed in the gameswf headers, so they are read at their offsets.
int RenderFX::GetAlpha(gameswf::character* character) {
    if (character == nullptr) {
        return 0;
    }
    const std::uint8_t* object = *reinterpret_cast<const std::uint8_t* const*>(reinterpret_cast<const std::uint8_t*>(character) + 0x48);
    return *reinterpret_cast<const int*>(object + 0x18);
}

// 0x7a7cec RenderFX::GetText(gameswf::character*)
// Binary: null returns null; a character whose is(32) is false returns null; otherwise the inline text at +0x139 when
// the byte at +0x138 is not 0xff, else the pointer at +0x144.
char* RenderFX::GetText(gameswf::character* character) {
    if (character == nullptr) {
        return nullptr;
    }
    if (character->is(32) == 0) {
        return nullptr;
    }
    std::uint8_t* base = reinterpret_cast<std::uint8_t*>(character);
    if (base[0x138] != 0xff) {
        return reinterpret_cast<char*>(base + 0x139);
    }
    return *reinterpret_cast<char**>(base + 0x144);
}

// 0x7a7d34 RenderFX::GotoFrame(gameswf::character*, int, bool): a character of class id 2 moves to the frame and gets
// the play state (play when flag is set; the original stores flag ^ 1, and PLAY is 0).
void RenderFX::GotoFrame(gameswf::character* character, int frame, bool flag) {
    if (character != nullptr && character->is(2)) {
        character->goto_frame(frame);
        character->set_play_state(flag ? gameswf::character::PLAY : gameswf::character::STOP);
    }
}

// 0x7a7d90 RenderFX::GetFrameCount(gameswf::character*): the frame count of a class id 2 character, else 0.
int RenderFX::GetFrameCount(gameswf::character* character) {
    if (character != nullptr && character->is(2)) {
        return character->get_frame_count();
    }
    return 0;
}

// 0x7a7dd4 RenderFX::IsAnimOver(gameswf::character*): the current frame is the last one (frame count - 1 or later).
// The original calls the current frame and the frame count once before reading them again; the first two results are
// discarded and are kept here.
bool RenderFX::IsAnimOver(gameswf::character* character) {
    if (character == nullptr || !character->is(2)) {
        return false;
    }
    character->get_current_frame();
    character->get_frame_count();
    const int current = character->get_current_frame();
    return current >= character->get_frame_count() - 1;
}

// 0x7a7e5c RenderFX::FSCommand(char const*, char const*): forwards to the event listener's OnFSCommand (slot +0x04).
void RenderFX::FSCommand(const char* command, const char* args) {
    if (m_eventListener != nullptr) {
        m_eventListener->OnFSCommand(command, args);
    }
}

// 0x7a7e80 RenderFX::RegisterDisplayCallback(gameswf::character*, void (*)(gameswf::render_state&, void*), void*):
// returns 1 when the callback was registered on the character, 0 for a null character.
int RenderFX::RegisterDisplayCallback(gameswf::character* character,
                                      void (*callback)(gameswf::render_state&, void*), void* userData) {
    if (character == nullptr) {
        return 0;
    }
    character->set_display_callback(callback, userData);
    return 1;
}

// 0x7a7eb4 RenderFX::SetOrientation(gameswf::orientation_mode): the render handler's set_orientation (slot +0xa8). The mode
// is passed through in r1, as the asm shows.
void RenderFX::SetOrientation(gameswf::orientation_mode mode) {
    gameswf::s_render_handler->set_orientation(mode);
}

// 0x7a7ee8 RenderFX::SetContext(gameswf::character*): m_context (+0x40).
void RenderFX::SetContext(gameswf::character* context) {
    m_context = context;
}

// 0x7a7ef0 RenderFX::GetContext(): m_context (+0x40).
gameswf::character* RenderFX::GetContext() {
    return m_context;
}

// 0x7a7fe4 RenderFX::DestroyContext(gameswf::player_context*): destruct when the pointer is not null.
void RenderFX::DestroyContext(gameswf::player_context* context) {
    if (context != nullptr) {
        gameswf::destruct(context);
    }
}

// 0x7a850c RenderFX::RenderFX() (C1), 0x7a85f8 (C2): the same body. Every field starts at zero. The four controllers
// clear their 36 bytes, set their enabled byte to 1 and reset their smart pointers. The +0x54 word is set to 0xffffff
// with the top byte kept as (old top byte & 0xfe); a new object has no old top byte, so it is set to 0 here.
RenderFX::RenderFX() {
    m_foundCharacters.data = nullptr;
    m_foundCharacters.count = 0;
    std::memset(pad_0c, 0, sizeof(pad_0c));
    m_root = nullptr;
    m_context = nullptr;
    std::memset(pad_44, 0, sizeof(pad_44));
    pad_44[0] = 1;                                   // +0x44
    pad_44[0x10] = 0xff;                             // +0x54 word, low three bytes 0xffffff
    pad_44[0x11] = 0xff;
    pad_44[0x12] = 0xff;
    pad_44[0x13] = 0;                                // +0x57: (old top byte & 0xfe), see above
    for (Controller& controller : m_controllers) {
        std::memset(controller.pad_00, 0, sizeof(controller.pad_00));
        controller.m_enabled = true;
        controller.Reset();
    }
    m_flags = 0;
    m_eventListener = nullptr;
}

// 0x7a87fc RenderFX::DepthSearch(int)
// PARTIAL, whole body omitted. The search walks a gameswf::array<int> stack at RenderFX +0x18 and a per-depth index
// with its own records (RenderFX::DepthSearchData and SearchIndex, pack0 batch of this wave). Neither record has a
// layout in dh2/RenderFX.hpp yet, so the walk is not written.
gameswf::character* RenderFX::DepthSearch(int /*depth*/) {
    return nullptr;
}

// 0x7a8a84 RenderFX::Find(char const*, gameswf::character*): a search from context for name, at depth 0. Null when the
// name or the context is null.
// PARTIAL, whole body omitted: it initializes the DepthSearchData at RenderFX +0x18 and then calls DepthSearch(0), and
// DepthSearchData is not declared in dh2/RenderFX.hpp.
gameswf::character* RenderFX::Find(const char* /*name*/, gameswf::character* /*context*/) {
    return nullptr;
}
