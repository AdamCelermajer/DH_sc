// Batch g1-game-character-PlayerInfo-3a5de8-1: SetInCutscene, and the copy constructor and operator= (skipped, see below).
// The constructor (C2 0x374cc0 and its C1 twin 0x37418c) is one C++ constructor, defined in
// g1-game-character-PlayerInfo-3a5de8-0.cpp with the C1 row, so this file does not define it again.
// Declarations come from dh2/PlayerInfo.hpp. The ledger rows are in game-character/ledger.d/g1-game-character-PlayerInfo-3a5de8-1.csv.

#include "dh2/PlayerInfo.hpp"

// 0x374cc0 PlayerInfo::PlayerInfo()   (C2). Not defined here: the same code is the C1 constructor 0x37418c, which
// g1-game-character-PlayerInfo-3a5de8-0.cpp defines. That body builds the CNetPlayerInfo base, the 25 blocks, registers
// the factory and declares the blocks in the original order (+0x288, +0x310, +0x338, +0x360, +0x2b0, +0x388, +0x2e8,
// +0x3b0, +0x3d8, +0x400, +0x428, +0x450, +0x478, +0x4a0, +0x4c8, +0x4e8, +0x508, +0x528, then +0x548..+0x638), then calls
// Reset(). It is partial there (the stale-value SetChanged in the member constructors is not reproduced).

// 0x3777d0 PlayerInfo::PlayerInfo(PlayerInfo const&) and 0x378808 PlayerInfo::operator=(PlayerInfo const&): not written.
// Both begin by calling the CNetPlayerInfo copy constructor (0x377448) or operator= (0x378708). The port deletes those
// (cnet_player_info.hpp), so the PlayerInfo copy and assignment are deleted too (dh2/PlayerInfo.hpp). Ledger: skipped.

// 0x459900 PlayerInfo::SetInCutscene(bool)
// The Hex-Rays text builds a temporary NetStructBool on the stack and calls SetChanged on it when its uninitialised
// value byte differs from the argument. Nothing reads that temporary: the byte is the argument in every case (when the
// compare is equal, the byte already holds it). The effect is the virtual SetValue of the +0x4e8 block (m_inCutscene),
// vtable slot +0x1c.
void PlayerInfo::SetInCutscene(bool inCutscene) {
    m_inCutscene.SetValue(inCutscene);
}
