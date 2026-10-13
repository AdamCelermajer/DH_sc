// RenderFX: the gameswf menu renderer wrapper (lib-gameswf, engine side). This header only
// carries what MenuFX needs: the base-class layout (original size 0x100) and the virtual slots.
// Placeholder written by the MenuFX layout wave; the engine unification wave should replace it
// with the full recovered layout. Offsets are original 32-bit ARM offsets.
#pragma once

#include <cstdint>

namespace gameswf {
class player_context;
class character;
class as_object;
class as_value;
class render_state;
class root;
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

    // A menu event: the record mySendEvent (0x42cf68) fills. Size at least 0x26 at 32-bit.
    struct Event {
        gameswf::as_object* target;   // +0x00 RenderFX::Find result for the event's character
        const char* name;             // +0x04 event name (OnEvent passes it to strstr; mySendEvent stores it)
        EventType type;               // +0x08 OnEvent tests it against 6
        uint8_t pad_0c[0x18];         // +0x0c .. +0x23, zeroed by mySendEvent, not recovered
        uint8_t unk_24;               // +0x24 zeroed by mySendEvent
        uint8_t unk_25;               // +0x25 zeroed by mySendEvent, read by GetCharacterPath
        char* GetCharacterPath();     // 0x421e48
    };

    // Result array of FindCharacters: a gameswf array, data at +0x00 and count at +0x04.
    struct CharacterArray {
        gameswf::character** data;    // +0x00
        int count;                    // +0x04
    };

    // One controller record, 0x28 bytes (GetController(i) = this + 0x58 + 0x28 * i, 0x7a7c74).
    // Reset (0x7a84c4) clears two gameswf smart pointers at +0x10 and +0x14 of the record.
    class Controller {
    public:
        void Reset();                 // 0x7a84c4
        uint8_t pad_00[0x28];         // +0x00 .. +0x27, the smart pointers at +0x10 and +0x14 not typed here
    };

    // Non-virtual methods. The addresses are the RenderFX symbols in the binary; signatures come from them.
    void SetEventListener(EventListener* listener);                       // 0x7a7c6c
    Controller* GetController(int index);                                 // 0x7a7c74
    void SetInputBehavior(int behavior);                                  // 0x7a7c98
    gameswf::character* GetFlashRoot();                                   // 0x7a7ca0, root +0x10
    gameswf::root* GetRoot();                                             // 0x7a7cac, m_root
    void SetWireFrame(bool enabled);                                      // 0x7a7c34
    void SetTextBufferingEnabled(bool enabled);                           // 0x7a7cb4
    int GetAlpha(gameswf::character* character);                          // 0x7a7cd8
    bool IsAnimOver(gameswf::character* character);                       // 0x7a7dd4
    bool IsAnimOver(const char* name);                                    // 0x7a9204
    void RegisterDisplayCallback(gameswf::character* character,
                                 void (*callback)(gameswf::render_state&, void*), void* userData);  // 0x7a7e80
    void RegisterDisplayCallback(const char* name,
                                 void (*callback)(gameswf::render_state&, void*), void* userData);  // 0x7a91d8
    void SetContext(gameswf::character* context);                         // 0x7a7ee8, m_context
    gameswf::character* GetContext();                                     // 0x7a7ef0
    void Render();                                                        // 0x7a9a94
    void Finalize();                                                      // 0x7a97ac
    gameswf::as_object* Find(const char* name);                           // 0x7a9160
    CharacterArray* FindCharacters(gameswf::character* context, const char* prefix, int index);  // 0x7a8c08, returns &m_foundCharacters
    void SetMember(const char* target, const char* name, gameswf::as_value& value);  // 0x7ab5d4
    void SetMember(const char* target, const char* name, int value);      // 0x7ab6c8
    void SetMember(const char* target, const char* name, const char* value);  // 0x7ab730
    void SetFocus(gameswf::character* character, int flag);               // 0x7ac228
    void SetFocus(const char* name, int flag);                            // 0x7ac498
    void ResetFocus(int flag);                                            // 0x7ac410
    int PlayAnim(gameswf::character* character, const char* anim, int flag);  // 0x7aba04
    int PlayAnim(const char* name, const char* anim, int flag);           // 0x7abb18
    int InvokeASCallback(gameswf::character* character, const char* name,
                         const gameswf::as_value* args, int count);       // 0x7abe0c
    int InvokeASCallback(const char* name, const char* function,
                         const gameswf::as_value* args, int count);       // 0x7ad7e8
    void FormatHTML(const char* name, const char* format, ...);           // 0x7a93bc
    void FormatHTML(gameswf::character* character, const char* format, ...);  // 0x7a947c
    int SendEvent(Event& event);                                          // 0x7abf34
    // Declared for FlashAnimManager (batch g1-game-ui-FlashAnimManager-797a71-0). The return type of Find(name, context)
    // is not in the symbol; it is typed character* because FlashAnimManager stores the result in a character slot.
    gameswf::character* Find(const char* name, gameswf::character* context);  // 0x7a8a84, DepthSearch from context for name
    void BeginDisplay();                                                  // 0x7a9afc, root->begin_display
    void EndDisplay();                                                    // 0x7a9ac8, root->end_display
    void SetPosition(gameswf::character* character, int x, int y);       // 0x7aa3f0
    void GotoFrame(gameswf::character* character, int frame, bool flag);  // 0x7a7d34
    int SetColorTransform(gameswf::character* character, unsigned int first, unsigned int second);  // 0x7a9dd0
    // Declared for MenuDebugHUD (batch g1-game-ui-MenuDebugHUD-c7396f-0). The symbols carry no return type and the callers
    // discard the result, so void is written.
    void SetText(gameswf::character* character, const char* text, bool flag);  // 0x7a92e0 RenderFX::SetText(gameswf::character*, char const*, bool)
    void FormatText(gameswf::character* character, const char* format, ...);   // 0x7a94dc RenderFX::FormatText(gameswf::character*, char const*, ...)

    // +0x3c: gameswf root pointer; +0x40: current context character; +0x58/+0x80/+0xa8/+0xd0:
    // RenderFX::Controller (4 of them, reset in MenuFX::Unload); +0x68: focus; +0xf8: flags word.
public:
    // The former pad_04 filler (+0x04 .. +0xff, original size 0x100) is split into the fields below.
    CharacterArray m_foundCharacters;  // +0x04 FindCharacters result (0x7a8c08 writes data at +0x04, count at +0x08, returns &this+4)
    uint8_t pad_0c[0x30];              // +0x0c .. +0x3b, not recovered
    gameswf::root* m_root;             // +0x3c root movie (GetRoot; MenuFX passes root+0x10 to SetContext)
    gameswf::character* m_context;     // +0x40 current context character (GetContext)
    uint8_t pad_44[0x14];              // +0x44 .. +0x57
    Controller m_controllers[4];       // +0x58 .. +0xf7, GetController(i) = &m_controllers[i]
    uint32_t m_flags;                  // +0xf8 flags word: bits 0x01, 0x08, 0x40 tested by MenuFX::SetMenu and PopMenu
    uint8_t pad_fc[4];                 // +0xfc .. +0xff
};
