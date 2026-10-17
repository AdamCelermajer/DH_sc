# Native Ghost skill initialization checkpoint — 2026-10-04

**Status:** Verified on the Android API 37 emulator with 16 KiB pages.

**Download APK:** [crypt-native-ghost-skill-init-candidate.apk](https://github.com/Noamcelermajer/DH_sc/releases/download/native-ghost-skill-init-2026-10-04/crypt-native-ghost-skill-init-candidate.apk)

**APK:** 26,040,516 bytes; SHA-256 `0dbb44c703d1768a36f2186b05c2f8ca9d9c787f5ddf3426e3c842edcd4d04ff`

**Source ref:** release tag `native-ghost-skill-init-2026-10-04`

**Compiled-source archive:** [download ZIP](https://github.com/Noamcelermajer/DH_sc/releases/download/native-ghost-skill-init-2026-10-04/crypt-native-ghost-skill-init-compiled-source.zip); 873,414 bytes; SHA-256 `f1933915ab15535ea8904b1103c1580d73659dcfdde17bd2cbf3b465b7b24d68`
**Build:** 452 compiler/build inputs, 255 assets, 16 ELF64 libraries, 53 bounded AI units, and 84 required export groups.

The raw source ZIP preserves the exact compiled input bytes, including differences
from Git line-ending normalization. It excludes the external SDK/toolchain and
asset bundle; it is not a standalone complete-game source distribution.

Public evidence is in [`reports/reconstruction-2026-10-04/native-ghost-skill-init`](../reports/reconstruction-2026-10-04/native-ghost-skill-init/): [artifact](../reports/reconstruction-2026-10-04/native-ghost-skill-init/artifact.json), [runtime test](../reports/reconstruction-2026-10-04/native-ghost-skill-init/runtime/crypt-script-smoke.json), [ABI/export validation](../reports/reconstruction-2026-10-04/native-ghost-skill-init/abi-exports.json), and [frozen build inputs](../reports/reconstruction-2026-10-04/native-ghost-skill-init/frozen-inputs.json).

## What is integrated

The two authored Crypt surprise Ghosts now run the bounded initialization stages using their native properties and one retained VM per actor. The order matches the original `CharAI::InitScriptProcess` caller:

1. Initialize HP and MP from the live property/class projection.
2. Run `CharAI::SetSkillsAndSpells` against the real Skill/Faery tables, native-owned vectors, DebugSwitches state, script path and InitVCB callback.
3. Run `CharAI::UpdateAllSkills` using the actual current Character state-machine predicates and the owned vectors.
4. Dispatch AIS `OnInitPost`.
5. Dispatch AIS `OnInitFinal` because the original `Character::Update` caller passes `InitFinal=true`.

The AIS is created pending, receives its OnInit and initialization callbacks on that same VM, and is published as active before the source `InitScriptProcess` phases begin. Native CharAI active/pending fields are synchronized immediately before `SetSkillsAndSpells` reads the active AIS. The catalogue backing, native faery-vector storage, actor owner and VM remain owned across world reload and Android activity recreation; a retained initialized Ghost is not initialized or healed again.

## Authored Ghost data and observed result

For both `_prim_Monster_SURPRISE_01` and `_prim_Monster_SURPRISE_02`, the selected source data produces:

| Result | Observed value |
|---|---:|
| HP / maximum HP | 25,600 / 25,600 raw source units |
| MP / maximum MP | 23,552 / 23,552 raw source units |
| Ordinary skill entries | 0 |
| Faery entries | 5, all null because their `SpellScript` fields are empty |
| `UpdateAllSkills` slots | 0 skill, 5 faery |
| Non-null `OnSkillUpdate` calls | 0 |
| InitVCB / Post / Final calls | 1 / 1 / 1 |
| Source initialization phases | `1,2,3,4,5` |

The run also verified that each Ghost's VM and vector/catalogue backing survived reload and recreation without replaying the ambush or rerunning initialization. The DebugSwitches file remained 665 bytes with matching before/after-recreation hash. The original authored trigger and Lua script were not modified. This evidence does not establish physical ARM64 behavior.

## Source caller and bounded scope

The original `Character::Update` body is ELF `0x3abe98`. At `0x3ac3f4–0x3ac404`, it passes `1` to `CharAI::LoadNInitScriptProcess(bool)` (`0x3cf3a4`) for the embedded CharAI at `Character+0x3c8`. The callee returns early if active AIS already exists; otherwise it runs `LoadScriptProcess`, rechecks active AIS, and calls `InitScriptProcess(bool)` (`0x3ce7c0`) only after an active AIS is available. That process calls `_InitHpMp`, `SetSkillsAndSpells`, `UpdateAllSkills`, `OnInitPost`, and then `OnInitFinal` when the saved boolean is nonzero. The maintained lifecycle caller is documented in [the source lifecycle notes](../port/level-world/reference/character-script-lifecycle/NOTES.md).

The native development path invokes this bounded lifecycle directly during world setup. It does not reproduce the surrounding `Character::Update` scheduling and eligibility checks, including the virtual updatability check, excluded FSM states, Character field gates, current-level/concurrent-AI limits and their eviction bookkeeping. This checkpoint therefore covers the selected Ghost `LoadNInitScriptProcess(true)` and its source-ordered `InitScriptProcess` work; it does not claim complete Character initialization or a source-equivalent scheduler.

If a native initialization callback fails, the port treats the current world-build attempt as terminal and discards that attempt. This is an explicit port failure policy; it is not claimed as the original game's retry or cleanup behavior.

Review found that the earlier terminal deactivation kept actor copies alive after
body teardown. It now retires timers/bodies, clears saved actor copies and owning
groups, and releases the catalogue. The same-process UI test selected a texture
and observed two Ghost references with zero remaining references, groups, saved
actors or catalogue owners. Re-entering Crypt created each Ghost once with full
health, phases `12345` and `retained=0`. The automated popup selector timed out;
the re-entry step used a manual assisted UI tap. This is verified native behavior,
with that test automation limitation recorded separately.

Evidence: [terminal discard and manual assisted re-entry](../reports/reconstruction-2026-10-04/native-ghost-skill-init/terminal-discard/terminal-discard-manual-completion.json).

## Remaining boundaries

Independent new source work covers `OnSkillUpdate`, the Usable/Active check callers,
and `Value::getBool`. The getBool gate passes 276 original ARM comparisons,
269 host cases and 42 cases using the actual reused float32 Lua APIs. These units
are not connected to production skill ReturnValues storage or callbacks yet.

The per-VM resolved-path helper passes 60 real Lua checks, including preservation
of the original skill registry on duplicate loads. It starts after path resolution;
full `LuaManager::AddFile` and the shared byte cache remain open. Its reset contract
requires replacement of the actual VM even when an allocator reuses the same
numeric handle. Preallocation before Lua execution, empty-payload rejection and
4095-byte path/8 MiB source bounds are port policies, not original allocation-failure
or limit parity.

- Nonempty skill and faery script loading, declaration, allocation and callbacks are not implemented as a complete path.
- Separate source-only proofs cover bounded [`OnSkillUpdate`](../port/level-world/reference/character-ai-skill-script-update/NOTES.md), [`OnSkillCheck_Usable`/`OnSkillCheck_Active`](../port/level-world/reference/character-ai-skill-script-check/NOTES.md), and a [per-VM executed-path cache helper](../port/level-world/reference/lua-script-load-once/NOTES.md) (60 checks total). These do not reconstruct the whole `LuaManager::AddFile` body or establish native nonempty skill loading.
- Other AIS factories and the full 265 Character function bindings remain incomplete; only the bounded supported OnInit closures are connected.
- Autonomous Ghost AI frames, acquisition, pursuit, combat and source AI/DoT timer expiry are not enabled. AI/DoT timers remain paused until their real providers are connected.
- Full profile/network PlayerInfo ownership, original `GSLevel` stack ownership, clean-checkout packaging and physical-device validation remain outside this checkpoint.

## Evidence and source paths

- Latest runtime report and screenshots: [`crypt-script-smoke.json`](../reports/reconstruction-2026-10-04/native-ghost-skill-init/runtime/crypt-script-smoke.json) and neighboring files under [`runtime`](../reports/reconstruction-2026-10-04/native-ghost-skill-init/runtime/).
- Native ordered initialization and retained owners: [`model_renderer.cpp`](../port/android-native/app/src/main/cpp/model_renderer.cpp).
- Source lifecycle kernel: [`character_script_lifecycle.cpp`](../port/level-world/character_script_lifecycle.cpp) and [original-call notes](../port/level-world/reference/character-script-lifecycle/NOTES.md).
- `SetSkillsAndSpells` caller and Ghost fallback-list evidence: [`character_ai_set_skills_and_spells.cpp`](../port/level-world/character_ai_set_skills_and_spells.cpp) and [source notes](../port/level-world/reference/character-ai-set-skills-and-spells/NOTES.md).
- `UpdateAllSkills` caller and state-query boundary: [`character_ai_update_all_skills.cpp`](../port/level-world/character_ai_update_all_skills.cpp) and [source notes](../port/level-world/reference/character-ai-update-all-skills/NOTES.md).
- Native Ghost list/vector adapter: [`native_ghost_skills.cpp`](../port/android-native/app/src/main/cpp/native_ghost_skills.cpp).

This is a source and runtime milestone, not a full-game completion claim.
