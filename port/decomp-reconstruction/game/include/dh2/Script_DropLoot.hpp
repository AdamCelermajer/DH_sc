// Script_DropLoot: script command that makes a named Character drop loot, optionally for a named
// second object (the loot source). Execute resolves both by name and calls Character::DropLoot.
// Original allocation 64 bytes (GetNewScriptCmdImplInstance<Script_DropLoot>).
// Reconstructed from libDungeonHunter2.so pseudocode (Execute 0x45e374, IsBlocking 0x4557e8).
#pragma once

#include <cstdint>

#include "dh2/ScriptCmdImpl.hpp"

class Script_DropLoot : public ScriptCmdImpl {
public:
    ~Script_DropLoot();                                                 // class-facts; not a vtable slot (ScriptCmdImpl's destructor is not virtual)

    // Overrides of ScriptCmdImpl slots, in vtable order (slot 0x08 and 0x0c).
    void Execute(bool a2, int a3) override;                             // slot 0x08: 0x45e374
    bool IsBlocking() const override;                                   // slot 0x0c: 0x4557e8 returns false

    // ScriptCmdImpl occupies +0x00 .. +0x0f (base layout pending, see open_questions).
    uint8_t pad_10[0x30];                                               // +0x10 .. +0x3f, nothing recovered
};
