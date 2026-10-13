// GSFlashMenu: state that runs the Flash menu stack. Update swaps m_menu for m_pendingMenu when they
// differ and pops itself once the current menu is hidden (m_popped). The ctor loads "menu_MainMenu"
// into m_pendingMenu the first time m_loadMainMenu is set.
// Original 32-bit size not recovered (no operator new size in class-facts); the ctor writes up to +0x29.
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x3841b4, 0x38421c).
#pragma once

#include <cstdint>

#include "dh2/StateBase.hpp"

class StateMachine;
class MenuManager;
class MenuBase;

class GSFlashMenu : public StateBase {
public:
    GSFlashMenu();                                                  // 0x3841b4 / 0x38421c
    ~GSFlashMenu() override;                                        // 0x384284, 0x3842b8 (D1/D2), 0x3842ec (D0)

    // Overrides of StateBase slots, in vtable order. Slots +0x10 .. +0x28 (Pause, Resume, Draw2D,
    // Sleep, WakeUp) are inherited unchanged.
    void Ctor(const StateMachine* machine) override;                // slot 0x08: 0x3846fc
    void Dtor(const StateMachine* machine) override;                // slot 0x0c: 0x3845f8
    void Update(StateMachine* machine, double dt) override;         // slot 0x18: 0x384460
    void Draw(const StateMachine* machine) override;                // slot 0x1c: 0x3841a8 MenuManager::Draw(0)

    static GSFlashMenu s_inst;                                      // GSFlashMenu::s_inst, the object GSInit and GSEndGame switch to

    // StateBase occupies +0x00 .. +0x03 (vptr).
    MenuManager* m_manager;                                         // +0x04 MenuManager::GetInstance() at construction
    MenuBase* m_menu;                                               // +0x08 menu currently shown
    MenuBase* m_pendingMenu;                                        // +0x0c menu to show on the next Update; "menu_MainMenu" when m_loadMainMenu was set
    uint8_t unk_10[0x10];                                           // +0x10 16-byte buffer; the ctor clears its first byte
    char* unk_20;                                                   // +0x20 points at unk_10 (set by the ctor)
    char* unk_24;                                                   // +0x24 points at unk_10 (set by the ctor)
    bool m_popped;                                                  // +0x28 set when the state pops itself (Update, Dtor); cleared by the ctor
    bool m_loadMainMenu;                                            // +0x29 set by the caller; the ctor loads the main menu once and clears it
};
