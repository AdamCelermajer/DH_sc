# Native Character frame foundations checkpoint — 2026-10-04

**Status:** this Crypt development APK passes Android 17/API37 testing with
16 KiB pages. The full game and autonomous Ghost frames remain unfinished.

**Download:** [APK](https://github.com/Noamcelermajer/DH_sc/releases/download/native-frame-foundations-2026-10-04/crypt-native-frame-foundations-candidate.apk).
APK: 26,068,604 bytes; SHA-256 `1b3a255122f0248f5d6608d1b2fe15def631645765ce40792481ceb89125a6e8`.
Source ref: release tag `native-frame-foundations-2026-10-04`.

**Exact compiler sources:** [ZIP](https://github.com/Noamcelermajer/DH_sc/releases/download/native-frame-foundations-2026-10-04/crypt-native-frame-foundations-compiled-source.zip),
888,934 bytes; SHA-256 `cc06bdc53e364873e23ab360a38336dc60e05b911204fff8dd3fcd3818189660`.
It preserves all 460 actual repository compiler/build inputs as raw bytes.
The external toolchain and assets are excluded; it is not a standalone
complete-game distribution.

Build verification: 255 assets, 16 ELF64 libraries, 57 bounded AI/script source
units, 91 required export groups, native ARM64 and x86_64, signing and
16 KiB ELF/ZIP alignment. Physical ARM64 execution remains untested.

Evidence: [artifact](../reports/reconstruction-2026-10-04/native-frame-foundations/artifact.json),
[runtime](../reports/reconstruction-2026-10-04/native-frame-foundations/runtime/crypt-script-smoke.json),
[exports](../reports/reconstruction-2026-10-04/native-frame-foundations/abi-exports.json),
[frozen inputs](../reports/reconstruction-2026-10-04/native-frame-foundations/frozen-inputs.json).

## Source changes and native boundaries

| Component | Verification | Native scope |
|---|---|---|
| Character classification/getters | 17 complete bounded original bodies; 514 ARM comparisons, all 130 executable instructions, zero mismatches | Cached-ID/type predicates select Ghost AIS using real loaded properties/tables; existing nongated enemy selection also uses them. |
| Character::IsZonable | 45 ARM comparisons and 19 sanitized host/guard cases | Source Player/Faerie predicates compose the Ghost result. Diagnostic logging only; room enrollment remains pending. |
| Character::CanUpdate | 316-byte range: 308 active code bytes, 77 instructions and 8 literal bytes. 26 primary/348 independent ARM comparisons; zero mismatches, pointer/failure guards and ASan/UBSan checks pass. | Compiled/exported. Real scene culling, visibility, player/online/respawn owners and native frame invocation remain pending. |
| Session update/state callbacks | 48 ARM comparisons for two reused state wrappers; real Lua zero-argument, fresh state/alias, return effects and failure tests | Same-VM methods compiled. Actual native AIS frame/state ownership remains pending. No new complete original helper-body credit. |
| Session resolved-path cache | 19 new real-Lua cache cases and 60 existing helper checks: loads, hits, failure effects/retries and VM replacement | Compiled and tied to the Session VM. Actual AddFile/path resolution/shared byte-cache owners and native calls remain pending. |

Review fixed a misaligned Visual read before address validation and late Visual/root
alias checks. Death/respawn fixtures now reach those branches. The `+0x80` input
is correctly named current visibility, matching earlier SetVisible evidence.
Source read order captures the entry Visual, refreshes later reads at their
original points, and retains effects before a provider failure.

Original-address notes: [classification](../port/level-world/reference/character-ai-classification/NOTES.md),
[zonability](../port/level-world/reference/character-zonability/NOTES.md),
[CanUpdate](../port/level-world/reference/character-update-eligibility/NOTES.md),
[Session update/cache](../port/level-world/reference/monster-external-script-updates/NOTES.md).

## Live app results

Actual touch/root-motion travel entered the unchanged GhostAmbush01 trigger from
an explicit fan player-start test override. Original Wait/Spawn commands produced
both Ghost bodies. The bundled start remains outside this remote trigger;
the previous override is restored after testing.

Both surprise Ghosts gave seven identical observations through initialization,
reload and rotation:

| Fact | Both Ghosts |
|---|---:|
| AI ID / faction ID | 68 / 7 |
| Type / Monster | 4 / 1 |
| Player / Faerie / NPC | 0 / 0 / 0 |
| Projected port death / zonable | 0 / 1 |

Unchanged monster OnInit and all five bounded HP/MP, SetSkillsAndSpells,
UpdateAllSkills, Post and Final phases run on the same retained VM. Authored
Ghost data produces zero ordinary skills and five null-script faeries.
A nonlethal fixture changed one Ghost's raw HP from 25,600 to 25,344; this
does not test complete combat damage calculation. Damaged HP, VM, timers,
vectors/catalogue backing and the 665-byte Debug file survived reload/rotation
without initialization or ambush replay. The flat Character list retains
14 owned nodes. The Prince and Ghost views were visually inspected.

The terminal-discard correction retains its
[earlier artifact-specific proof](NATIVE-GHOST-SKILL-INIT-CHECKPOINT-2026-10-04.md).
This milestone's live gate covers ambush, initialization, reload and rotation.

## Remaining integration

- The native adapter supplies a DACT instance name and normalized port death.
  Original Character name/raw-death producers remain unproved. Ghost type4
  does not enter the type0 Player name branch.
- Zonability diagnostics do not populate original zones or establish InZone ownership.
- CanUpdate is not invoked by the native frame. The enclosing Character::Update
  lazy-load/concurrent-AI scheduler and actual culling services remain open.
- Update/state/cache methods are compiled and host-tested. Native autonomous
  acquisition, target retention, pursuit, attacks and nonempty skills remain open.
- AI/DoT timers remain paused until their actual callback providers are connected.
- Complete bindings, Arguments/ReturnValues, level stack, factories, combat,
  loot/quests, UI/audio, saves and campaign coverage remain open.

The [project checklist](PROJECT-CHECKLIST.md) tracks all completion gates.
The combined mapping contains 531 unique original ranges / 838 evidence records
across 79 manifests: 366 addresses beyond Adam's ledger, 1,821 combined unique
addresses. These measure evidence reach, not finished gameplay or a completion percentage.
