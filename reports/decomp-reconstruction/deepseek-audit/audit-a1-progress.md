# Audit A1 progress (read-only bug hunt, worklist-a1.csv)

Output: `audit-a1.csv` (findings only, 8 rows). No source, header, ledger or test was edited; no builds, no git, no subagents.

## Status: 600 of 600 rows visited

Every worklist row was looked up in `recovered/native/hexrays-all` and in `port/decomp-reconstruction/*/src` (address comment first, symbol-name fallback). Depth varied:

- Full comparison of the main logic: about 200 rows (roughly the first 110, plus small leaf functions such as conversions, getters, constructors, dispatch and event handlers).
- Entry-level comparison only (first 60-120 lines of the listing and the matching recon body; long tails not read): about 360 rows. These are mostly functions over 60 pseudo-lines, Lua binding bodies, network state machines and large UI/game-world routines. Treat them as "no mismatch seen in the visible part", not as cleared.
- Several recon bodies are documented in their own file as PARTIAL (for example the `FS_*` menu natives, `PlayerInfo` setters, `CharStateMachine` ctor paths). Those notes were checked against the listing where visible.

## Findings (audit-a1.csv)

| severity | function | kind | note |
|---|---|---|---|
| medium | LC_API_STRTRIM (0x89cd00) | other | Original strips leading spaces in place; recon is a no-op. No callers in the port. |
| medium | Console::onEvent (0x335b14) | compare | Signed clamp in original vs unsigned clamp in recon; with zero menu items selection diverges (-1 vs 1). Same in touch-move path. |
| medium | MenuBase::FS_PlayMusic (0x4210a8) | side-effect | VoxSoundManager::PlayMusic call left out (documented PARTIAL). Menu music cue never plays. |
| medium | MenuBase::FS_SetText2 (0x424540) | side-effect | RenderFX::FormatHTML call left out (documented). Menu text not updated. |
| medium | MenuBase::FS_IsMapLocLocked (0x4248dc) | side-effect | Lock state not published as _root.retval (documented). |
| medium | CMatchingGLLive::SetPlayerParam (0x81f04c) | side-effect | Forward to lobby observer is a no-op (documented PARTIAL). |
| low | Console::DBG_TryStartingLevel (0x33179c) | other | Original passes the register holding `this` as the level name; likely a decompiler mix-up, unverified against asm. |
| low | DebugSwitches::GetSwitch/SetSwitch/DelSwitch (0x337a88 family) | side-effect | Missing-name path does not call DebugSwitches::load() first. Callers usually load earlier (app_entry.cpp:155), so likely dormant. |

No high-severity finding was confirmed.

Noted but not logged (documentation only, no behaviour difference found): stale PARTIAL comment on `VisualObject::ApplyMaterialEffect` (the recon body now performs the node search it says is omitted); `CharAI::AI_CancelSkill`, `GetStatValue`, `SetSpellCooldownTimerId` and similar assert paths that return where the original would read out of bounds (documented deviations); `ConfigFile::XOR` clamps values of 512+ characters (documented).

## Unmatched rows (no reconstruction found by address comment or symbol name): 16

- 0x487dd8 Array2d<rnd::Tile*>::Array2d (template in header; constructor not checked)
- 0x48bd60 rnd::Rule::Impl::Impl(Rule const&, Rule::Impl*)
- 0x3c1ac4 CharStateMachine::CharStateMachine() (C1)
- 0x3fc494 ItemInstance::ItemInstance(int, uint) (C2)
- 0x47df64, 0x47f194, 0x47f5a0, 0x47eb90, 0x47eec0, 0x47ed70 ObjectiveTemplate_* template instantiations (name is mangled differently in recon)
- 0x51d40c PFFloor::PFFloor(...) (C2)
- 0x521540 PFRoom::PFRoom(...) (C1)
- 0x843cd8 item::item() (C1)
- 0x82cb60 GLXPlayerUserFriend::GLXPlayerUserFriend() (C1)
- 0x832c2c GLXPlayerLeaderboard::addScoreEntry(...)
- 0x8401cc GLXPlayerPing::~GLXPlayerPing() (D2)

Several of these may exist under a different comment or in a file the search did not cover. None were marked as missing by the port itself.
