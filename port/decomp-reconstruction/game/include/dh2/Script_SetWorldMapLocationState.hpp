// Script_SetWorldMapLocationState: script command that sets a world-map location state on the local
// Player's Character through Character::SG_SetMapLocState. Execute reads the command data (m_args at
// +0x0c of the base): +0x08 location index (I32), +0x0c state value (I32).
// Original 32-bit size 0x40 (64). Allocated by GetNewScriptCmdImplInstance<Script_SetWorldMapLocationState>.
// Reconstructed from libDungeonHunter2.so pseudocode (Execute 0x45ce78, IsBlocking 0x4558ec).
#pragma once

#include <cstdint>

#include "dh2/ScriptCmdImpl.hpp"

class Script_SetWorldMapLocationState : public ScriptCmdImpl {
public:
    // Overrides of ScriptCmdImpl slots, in vtable order (slot 0x08 and 0x0c).
    void Execute(bool skipping, int module) override;                   // slot 0x08: 0x45ce78, Character::SG_SetMapLocState with a -1 last argument
    bool IsBlocking() const override;                                   // slot 0x0c: 0x4558ec (8-byte body)

    // ScriptCmdImpl occupies +0x00 .. +0x0f. No member is written by the command itself.
    uint8_t pad_10[0x30];                                               // +0x10 .. +0x3f, nothing recovered
};
