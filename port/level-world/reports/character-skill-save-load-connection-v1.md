# Same-Save native reload connection

`CharacterSkillSaveLoadConnectionV1` borrows the sole `PlayerSaveLoadOwnerV1`
and source-selected Character skill-list owner. It supplies `SkillSaveReloadServicesV6`
to `CharacterPlayerSkillsV6::native_reload_skills`, and `load_mask(0x20)` for
the outer NativeReloadSkills saved-property phase. Its callbacks reject any
other Save or Character identity. Retain the connection and load owner for the
duration of callbacks; never construct a new Save to satisfy menu startup.

For the actual development player's default Save constructor, slot is -1 and
profile is null. Existing source-differential LoadOwner proves Load(8) and
Load(0x20) reach **no service callbacks**, including no filename/open operation.
This is successful execution of the source null guard, not a file-miss stub.
Original load differential receipt: game-data/reference/player-save-load-v1/
load-original-v1.json, 1780 whole coordinator cases / 23017 boundaries.

SG_ReloadSkills first releases the previous skill allocation, obtains the
current source Character list, initializes every saved level to zero, clears
both slot maps, then invokes Load(8). A null profile therefore genuinely resets
unsaved development skill training/equipment. A nonnegative slot with null
profile reaches filename then real Savegame construction; an existing profile
reaches SKIL. Those production file/named-section endpoints remain required.
This adapter does not manufacture a profile, filename, private-file parser or
missing-file result. The recovered outer Load coordinator does not prove the
whole Savegame file class and its file-miss semantics.

Focused composed test `tests/character_skill_save_load_connection_v1.cpp` passes
O2 and O1 ASan/UBSan. It executes actual SG_ReloadSkills + actual LoadOwner,
proves reset/publication/zero callbacks, and rejects different Save, missing
filename and missing SKIL providers. The selected list is a declared test input;
no production design/player-list extraction is claimed by this test. New adapter
strict Android arm64 syntax passes. GCC warning suppression applies only to the
existing frozen Save implementation's compact indentation.

Outer NativeReloadSkills still requires genuine remove-buffs, native skill
reload/update, recalculate(1), inventory checks, Save level/class, MenuFX and
authored IsSpecTime invocation. This connection resolves only its Same-Save
Load masks and inner SG_ReloadSkills composition; it does not substitute those
remaining services with successful empty callbacks.
