// Script_CloseDoor: script command that closes a named Door and reports it as blocking while the
// door is open or opening (Door states 1 and 3). Execute(bool, int) looks up the door by name from the command data
// (ScriptCmd m_w3 at +0x0c of the data record), keeps the handle, and calls Door::Close.
// Original 32-bit size 0x48 (72). Allocated by GetNewScriptCmdImplInstance<Script_CloseDoor>.
// Reconstructed from libDungeonHunter2.so pseudocode (Execute 0x45d650, IsBlocking 0x455884).
#pragma once

#include <cstdint>

#include "dh2/ScriptCmdImpl.hpp"

class Door;

class Script_CloseDoor : public ScriptCmdImpl {
public:
    ~Script_CloseDoor();                                                // class-facts; not a vtable slot (ScriptCmdImpl's destructor is not virtual)

    // Overrides of ScriptCmdImpl slots, in vtable order (slot 0x08 and 0x0c).
    void Execute(bool a2, int a3) override;                             // slot 0x08: 0x45d650
    bool IsBlocking() const override;                                   // slot 0x0c: 0x455884

    // ScriptCmdImpl occupies +0x00 .. +0x0f (base layout pending, see open_questions).
    Door* m_door;                                                       // +0x10 door found by name, null when absent or not closed-able
    bool m_instant;                                                     // +0x14 copied from the data record m_w4 (+0x10)
    uint8_t pad_15[3];                                                  // +0x15
    uint8_t pad_18[0x30];                                               // +0x18 .. +0x47, nothing recovered
};
