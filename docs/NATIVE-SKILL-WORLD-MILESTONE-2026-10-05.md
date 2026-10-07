# Native skill and world checkpoint, 2026-10-05

Checkpoint: `port/android-native/build/checkpoints/dh2-native-skill-world-4156b785.apk`.
SHA-256: `4156b78563a7dc855eb95da8b3f0a012d6d7770ca1727eaf801e3d3755aa8656`.
Installed and tested on visible emulator-5554. Independent menu and loader emulators were untouched.

## Integrated changes

- All eleven Crypt NPCs now initialize through the recovered level character-state pipeline. The authored presets select the actual retained FSM, animation, script and property owners. Source Idle produces state 3 and flags `0x2380`; no targetability flags are assigned by the renderer.
- The original 69-row trophy catalog has a retained native manager. Actual source skill Begin queries now reach this owner, removing the previous unconditional missing-provider-8 failure. Achievement notification and full campaign Save continuations still require services.
- World skill execution now borrows the same player/NPC properties, life, combat fields, Gear, FSM and canonical aggro tables. There is one aggro-table authority; vector-copy rebinding preserves table pointers. Missing damage side effects remain explicit failures.
- The live Headsplitter HUD action is accepted and runs clip 1234 through source state 3 -> 6 -> 3, with the original animator event and closing event. The verified cast has zero targets. This establishes activation/animation, not enemy skill damage.
- Source saved-option queries use one retained original settings owner and the actual descriptor/type/maximum rule. The owner retains constructor-empty settings; the original loadSettings lifecycle is still required.
- The existing skill/faery/potion HUD and stats/equipment/skills/faeries panel remain functional over the same native Save and gameplay owners. Campaign faeries remain genuinely locked.

## Exact checkpoint verification

ARM64 and x86_64 native/Java builds passed. Installed APK SHA matches the checkpoint. The APK contains 18 ELF64 libraries with at least 16 KiB PT_LOAD alignment, 771 asset entries and the canonical uncompressed cache ZIP containing 6,833 original files. No separate cache download is required. No ARM32 reference runner is packaged.

`port/android-native/reports/native-skill-world-final/gameplay-character-panel-smoke.json` verifies actual widget transactions: armor 4 -> 2 -> 4, Headsplitter slot reassignment/restoration, disabled training with zero points, original faery names and locked states, full-health potion refusal with stock 5, and healing from raw HP 32,218 to 42,265 with stock 4. The damage used to test healing comes from the existing enemy attack development command; it is not evidence of complete NPC AI.

`port/android-native/reports/native-skill-world-final/skill-world-activation-smoke.json` verifies eleven source-initialized NPCs, the 69-row trophy owner, accepted Headsplitter clip 1234 and its complete source state cycle on the same exact installed APK. `native_targets` is 0 and `skill_damage_verified` is false. Runtime logs show no fatal native signal, native frame failure or GL error during this smoke.

Provenance: `port/android-native/reports/native-skill-world-4156b785-checkpoint-validation.json`.
Exact compiler dependency/source snapshot: `port/android-native/build/checkpoints/dh2-native-skill-world-4156b785-source.zip`, with 1,288 recorded inputs. Newer agent candidates are deliberately outside this snapshot.

## Next substantial integrations

Three parallel agents have explicit large subsystem ownership:

1. Faery cast controller/state/animator lifecycle and a single V6 gameplay timer authority. The candidate passes 1,824 original ARM/native ARM64 differential cases. Renderer integration and unlocked campaign runtime casting are still pending. Timer 33 needs actual aggro/regen facts; timer 34 needs genuine DoT calculation/application.
2. NPC physical body, original collision routing, shared AIS state, then seeking/movement/combat scheduling. Queue and CanAttack candidates pass host checks and original instruction audits, but a counter-only collision callback cannot implement original NPC behaviour.
3. Full player aggro/deaggro combat state/music ownership and the genuine skill damage side-effect services. The aggro owner passes 376 original instruction cases and 2,267 host checks per optimized sanitizer build; actual level/sound producers and renderer wiring are still pending.

The original animated character-menu lifecycle, complete skill damage/FX/status/kill/audio tails, full NPC AI/health presentation, faery companion model/follow/campaign unlocks, all campaign maps and persistence, and physical ARM64 device testing remain incomplete. NPC source Idle is connected, but the old rendered NPC scheduler is not yet replaced by the complete physical/AI pipeline. Player framing/scaling also needs further work. This is a development checkpoint, not a fully playable reconstruction. The overall goal remains active.
