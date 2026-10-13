// Script command that saves every player, then loads another level by name. The target level name
// and level id are read from the argument block that ScriptCmdImpl holds at +0x0c (block +0x08 and
// +0x10). Derives from ScriptCmdImpl and adds no members:
// GetNewScriptCmdImplInstance<Script_ChangeLevel> allocates 0x10 bytes, the ScriptCmdImpl size.
// Original 32-bit size 0x10 (16).
// Reconstructed from libDungeonHunter2.so pseudocode (Execute 0x45cba0, IsBlocking 0x4558fc).
#pragma once

#include "dh2/ScriptCmdImpl.hpp"

class Script_ChangeLevel : public ScriptCmdImpl {
public:
    // Overrides of ScriptCmdImpl slots, in vtable order. Init and Update are inherited (slots 0x00, 0x04).
    void Execute(bool a2, int a3) override;                              // slot 0x08: 0x45cba0
    bool IsBlocking() const override;                                   // slot 0x0c: 0x4558fc returns false
};
