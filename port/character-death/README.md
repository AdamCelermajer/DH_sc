# Non-player death and quest request projection

This owned C projection covers original `Character::Kill` (ELF 0x3a5b18,
1,776 bytes), for non-player targets with a null killer. It owns portable actor
metadata and four property sheets; it does not construct the original Character.

## Recovered behavior

- Already dead targets are untouched and produce no external requests.
- Live targets become dead and HP uses original property Set to zero. Property
  routing can leave final HP nonzero for unsupported types; the dead flag still
  becomes true, as in the original.
- If the current level has no loot manager, DropLoot is requested using final
  property 9's raw loot ID. This precedes the forced/network/event suppression
  checks and does not itself create loot.
- Forced kills, network targets and the suppress-events actor flag skip quests.
- Other null-killer deaths request KillXEnemies then ClearEnemies, with the
  actor's signed-short property-record ID. A template ID other than -1 also
  requests KillEnemyTemplate then ClearEnemyTemplate, in that order.
- Each event retains objective constant ID, target object ID and signed
  property/template match ID. Native null killer, two zero flags and -1 event value are implicit.
  Source kinds replace native event vtable addresses; padding is not copied.

The caller supplies constant IDs and level/force policy. Property/template IDs
must be in [-32768, 32767]; boolean flags must be 0 or 1. Rejected arguments,
aliases or malformed tables preserve state and outputs. Buffs are empty.

The raw fields are Character +0x64 (target object ID), +0x13c8
(property-record ID), and +0x13ca (template ID). SafeGetCharPropsId at
0x3b3d38 reads +0x13c8; SafeGetCharPropsTemplateId at 0x3b36ec stores/reads
+0x13ca. TestCharPropId::Compare (0x47b820) and TestCharTemplate::Compare
(0x47b05c) use these getters. Event +12 holds the target object ID and +24
holds the match ID. These are identifiers; quest counts require consumers.

## Evidence

`differential-validation.json` records 2,510 original ARM32/source ARM64/host
comparisons with zero mismatches: 800 policy/event paths, 455 synthetic property
type paths and 1,255 repeated already-dead calls. All four sheets, original
reserved fields, dead flag, loot request/ID and ordered semantic event payloads
match. Source output guards and stack restoration pass.

Actual original Kill, IsDead/IsPlayer/AI indexing, HP Set/Recalc, GetCurrentLevel,
DropLoot/GetLoot and RaiseAsync forwarding execute. AI records, current level,
loot-manager presence, constant lookup, network virtual method, Loot::Drop and
the event queue are explicit fixtures. Death instruction bytes are unmodified;
the driver buffers ELF parser input in memory to avoid slow WSL filesystem seeks.

`host-build-validation.json` records 12,000 address/undefined behavior sanitizer
iterations with recovery disabled. `android-build-validation.json` checks source
ARM64 ELF load alignment at 16 KiB.

Player death counters, a resolved killer's threat-list credit and XP,
achievements, actual loot creation, quest consumers, state/animation/world
ownership, progression and saves remain pending. This is not full source gameplay.

## Reproduce

```sh
python port/character-death/build.py --host \
  --report port/character-death/host-build-validation.json
python port/character-death/build.py --ndk /path/to/android-ndk \
  --report port/character-death/android-build-validation.json
python port/character-death/tests/differential.py \
  --original /private/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so \
  --cache /private/cache/files --host port/character-death/build/death-host.so \
  --arm64 port/character-death/build/death-arm64.so \
  --report port/character-death/differential-validation.json
```

The current Android builder uses Windows NDK; host uses Linux/WSL. Differential
execution requires Unicorn and pyelftools. Addresses use the ELF zero load base.
