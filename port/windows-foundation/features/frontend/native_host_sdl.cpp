#include "native_host.hpp"

#include <SDL.h>
#include <algorithm>
#include <unordered_map>
#include <utility>

namespace dh::foundation::frontend {
namespace {
struct WatchState {
    Uint32 window_id{};
    std::vector<HostEvent> events;
};
std::unordered_map<NativeHostInput*, WatchState> watched_events;

int virtual_key(SDL_Keycode key) noexcept {
    switch (key) {
    case SDLK_RETURN: return 13;
    case SDLK_BACKSPACE: return 8;
    case SDLK_LEFT: return 0x25;
    case SDLK_UP: return 0x26;
    case SDLK_RIGHT: return 0x27;
    case SDLK_DOWN: return 0x28;
    case SDLK_ESCAPE: return 0x1b;
    case SDLK_TAB: return 0x09;
    case SDLK_SPACE: return 0x20;
    case SDLK_LSHIFT: case SDLK_RSHIFT: return 0x10;
    default:
        if (key >= SDLK_a && key <= SDLK_z) return int(key - SDLK_a) + 'A';
        if (key >= SDLK_0 && key <= SDLK_9) return int(key);
        if (key >= SDLK_F1 && key <= SDLK_F12) return 0x70 + int(key - SDLK_F1);
        return int(key);
    }
}

int watch_event(void* raw, SDL_Event* source) {
    auto* host = static_cast<NativeHostInput*>(raw);
    auto found = watched_events.find(host);
    if (!host || found == watched_events.end()) return 1;
    auto& state = found->second;
    HostEvent event;
    bool enqueue = false;
    switch (source->type) {
    case SDL_KEYDOWN: case SDL_KEYUP:
        if (source->key.windowID != state.window_id) break;
        event.kind = HostEvent::Kind::key;
        event.key = virtual_key(source->key.keysym.sym);
        event.down = source->type == SDL_KEYDOWN;
        event.shift = (source->key.keysym.mod & KMOD_SHIFT) != 0;
        enqueue = true;
        break;
    case SDL_TEXTINPUT:
        if (source->text.windowID != state.window_id) break;
        event.kind = HostEvent::Kind::text;
        event.text = source->text.text;
        enqueue = !event.text.empty();
        break;
    case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
        if (source->button.windowID != state.window_id || source->button.button != SDL_BUTTON_LEFT) break;
        event.kind = HostEvent::Kind::pointer;
        event.pointer = 0;
        event.phase = source->type == SDL_MOUSEBUTTONDOWN
            ? input::PointerPhase::down : input::PointerPhase::up;
        event.point = {float(source->button.x), float(source->button.y)};
        enqueue = true;
        break;
    case SDL_MOUSEMOTION:
        if (source->motion.windowID != state.window_id || !(source->motion.state & SDL_BUTTON_LMASK)) break;
        event.kind = HostEvent::Kind::pointer;
        event.pointer = 0;
        event.phase = input::PointerPhase::move;
        event.point = {float(source->motion.x), float(source->motion.y)};
        enqueue = true;
        break;
    case SDL_WINDOWEVENT:
        if (source->window.windowID != state.window_id || source->window.event != SDL_WINDOWEVENT_FOCUS_LOST) break;
        event.kind = HostEvent::Kind::focus_lost;
        enqueue = true;
        break;
    default:
        break;
    }
    if (enqueue) state.events.push_back(std::move(event));
    return 1;
}
}

NativeHostInput::~NativeHostInput() {
    SDL_StopTextInput();
    SDL_DelEventWatch(watch_event, this);
    watched_events.erase(this);
    window_ = nullptr;
}

bool NativeHostInput::attach_focused_window(std::string& error) {
    if (window_) { error = "Native input host already attached"; return false; }
    auto* window = SDL_GetKeyboardFocus();
    if (!window) { error = "Diagnostic focused SDL window unavailable"; return false; }
    window_ = window;
    watched_events.emplace(this, WatchState{SDL_GetWindowID(window), {}});
    SDL_AddEventWatch(watch_event, this);
    SDL_StartTextInput();
    error.clear();
    return true;
}

std::vector<HostEvent> NativeHostInput::take_events() {
    auto found = watched_events.find(this);
    if (found == watched_events.end()) return {};
    auto result = std::move(found->second.events);
    found->second.events.clear();
    return result;
}

bool NativeHostInput::post_pointer(input::Point point, bool down) {
    auto* window = static_cast<SDL_Window*>(window_);
    if (!window) return false;
    SDL_Event event{};
    event.type = down ? SDL_MOUSEBUTTONDOWN : SDL_MOUSEBUTTONUP;
    event.button.type = event.type;
    event.button.windowID = SDL_GetWindowID(window);
    event.button.button = SDL_BUTTON_LEFT;
    event.button.state = down ? SDL_PRESSED : SDL_RELEASED;
    event.button.clicks = 1;
    event.button.x = int(point.x);
    event.button.y = int(point.y);
    return SDL_PushEvent(&event) == 1;
}

bool NativeHostInput::post_key(int key, bool down) {
    auto* window = static_cast<SDL_Window*>(window_);
    if (!window) return false;
    SDL_Keycode symbol = SDLK_UNKNOWN;
    if (key == 13) symbol = SDLK_RETURN;
    else if (key == 8) symbol = SDLK_BACKSPACE;
    else if (key == 0x25) symbol = SDLK_LEFT;
    else if (key == 0x26) symbol = SDLK_UP;
    else if (key == 0x27) symbol = SDLK_RIGHT;
    else if (key == 0x28) symbol = SDLK_DOWN;
    else if (key == 0x1b) symbol = SDLK_ESCAPE;
    else if (key == 0x09) symbol = SDLK_TAB;
    else if (key == 0x20) symbol = SDLK_SPACE;
    else if (key == 0x10) symbol = SDLK_LSHIFT;
    else if (key >= 'A' && key <= 'Z') symbol = SDLK_a + (key - 'A');
    else if (key >= '0' && key <= '9') symbol = SDLK_0 + (key - '0');
    else if (key >= 0x70 && key <= 0x7b) symbol = SDLK_F1 + (key - 0x70);
    else return false;
    SDL_Event event{};
    event.type = down ? SDL_KEYDOWN : SDL_KEYUP;
    event.key.type = event.type;
    event.key.windowID = SDL_GetWindowID(window);
    event.key.state = down ? SDL_PRESSED : SDL_RELEASED;
    event.key.repeat = 0;
    event.key.keysym.sym = symbol;
    event.key.keysym.scancode = SDL_GetScancodeFromKey(symbol);
    event.key.keysym.mod = SDL_GetModState();
    return SDL_PushEvent(&event) == 1;
}

bool NativeHostInput::post_ascii_text(const std::string& text) {
    auto* window = static_cast<SDL_Window*>(window_);
    if (!window) return false;
    for (unsigned char ch : text) {
        if (ch >= 128) return false;
        SDL_Event event{};
        event.type = SDL_TEXTINPUT;
        event.text.type = SDL_TEXTINPUT;
        event.text.windowID = SDL_GetWindowID(window);
        event.text.text[0] = char(ch);
        event.text.text[1] = '\0';
        if (SDL_PushEvent(&event) != 1) return false;
    }
    return true;
}
}
