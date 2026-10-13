// Script_DoTutorial: script command that starts a tutorial script once for the local player and
// clears its per-tutorial flag. Only on the normal path (no character difficulty, no online
// session); a menu id of 17 saves settings, otherwise a background job is started.
// Original allocation 64 bytes (GetNewScriptCmdImplInstance<Script_DoTutorial>).
// Reconstructed from libDungeonHunter2.so pseudocode (Execute 0x460950, IsBlocking 0x455970).
#pragma once

#include <cstdint>

#include "dh2/ScriptCmdImpl.hpp"

class Script_DoTutorial : public ScriptCmdImpl {
public:
    ~Script_DoTutorial();                                               // class-facts; not a vtable slot (ScriptCmdImpl's destructor is not virtual)

    // Overrides of ScriptCmdImpl slots, in vtable order (slot 0x08 and 0x0c).
    void Execute(bool a2, int a3) override;                             // slot 0x08: 0x460950
    bool IsBlocking() const override;                                   // slot 0x0c: 0x455970 returns false

    // ScriptCmdImpl occupies +0x00 .. +0x0f (base layout pending, see open_questions).
    uint8_t pad_10[0x30];                                               // +0x10 .. +0x3f, nothing recovered
};
