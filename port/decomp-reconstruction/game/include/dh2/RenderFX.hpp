// RenderFX: the gameswf menu renderer wrapper (lib-gameswf, engine side). This header only
// carries what MenuFX needs: the base-class layout (original size 0x100) and the virtual slots.
// Placeholder written by the MenuFX layout wave; the engine unification wave should replace it
// with the full recovered layout. Offsets are original 32-bit ARM offsets.
#pragma once

#include <cstdint>

namespace gameswf {
class player_context;
class character;
}  // namespace gameswf

class RenderFX {
public:
    struct Event;
    struct Cursor;
    enum EventType : int;

    // Secondary interface MenuFX and MenuManager implement. Vtable of 3 slots, no virtual dtor.
    class EventListener {
    public:
        virtual void OnEvent(Event& e);                                // slot +0x00
        virtual void OnFSCommand(const char* command, const char* args);  // slot +0x04
        virtual bool CanHandleEvent(Event& e);                         // slot +0x08 (0x42ca80 returns true)
    };

    virtual ~RenderFX();                                                // +0x00, +0x04
    virtual void Load(const char* swfFile, gameswf::player_context* ctx);  // +0x08
    virtual void Unload();                                              // +0x0c
    virtual void Update(int dt, bool a);                                // +0x10
    virtual void UpdateCursor(Cursor& cursor, int a);                   // +0x14

    // +0x3c: gameswf root pointer; +0x40: current context character; +0x58/+0x80/+0xa8/+0xd0:
    // RenderFX::Controller (4 of them, reset in MenuFX::Unload); +0x68: focus; +0xf8: flags word.
public:
    uint8_t pad_04[0xfc];  // +0x04, up to the original size 0x100
};
