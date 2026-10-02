# Recovered quest table reader

This source reader validates the complete original `v2quests_pyarray.bin` and
returns portable scalar fields, list entries and string spans. Its view borrows
immutable file bytes; the Lua bridge copies the file into an owned generation.
Original pointers, vtables and allocation layout are excluded.

## Native reader evidence

The actual original array reader (ELF 0x4b8acc), quest reader (0x504a94) and
nested condition/objective/reward/script readers execute in the differential
driver. Reads and bounded allocation/disposal are explicit fixtures. All 21,817
cache bytes are consumed, with SHA-256
`cbbde6ad8aef5f69ed913da5b62b4246a9c8584219c03256c3a2f8de8e8a1b36`.

Every source host/ARM64 accessor matches the actual native destinations:

- 64 quests and name-count agreement with the separately recovered array names.
- 80 condition records and 66 list objectives.
- 128 embedded accept/end objective records.
- 222 rewards across normal/hard/very-hard lists.
- 896 script slots, with all scalar, string and list fields compared.

All 21,817 host truncations and 128 ARM64 truncations reject atomically. A trailing
byte rejects; source output guards pass. The host sanitizer gate runs 12,000
mutations and pointer/index/alias rejection checks with recovery disabled.
ARM64 load segments align to 16 KiB. `differential-validation.json` contains
native function hashes, calls, semantic rows and build/source identities.

## Portable schema

The 284-byte native quest row has four scalar IDs at +4, five list count/pointer
pairs at +0x14, embedded 44-byte objective stubs at +0x3c/+0x68, target level at
+0x94, repeatable byte at +0x98, state at +0x9c, fourteen script strings at
+0xa4, priority at +0x114 and act at +0x118. Conditions/rewards are 16-byte
native records with three scalar fields after the vtable. Objective stubs have
three common fields, two strings and three type-specific arguments.

The source uses signed raw integer fields and file-offset/length spans. Objective
arguments keep their serialized meaning; they are not all character IDs or counts.
Counts are capped at 4096 per table/list and file size at 4 MiB. All nested
lengths and exact consumption are validated. Backing bytes must remain alive and
immutable for borrowed C views. Failures preserve outputs.

## Owned Lua API

- `DH2GetQuestCount()` returns the current imported generation's row count.
- `DH2GetQuestRecord(row)` returns the four `ids`, `conditions`, `objectives`,
  three reward lists, `accept`, `end`, `target_level`, `repeatable_byte`, `state`,
  fourteen `scripts`, `priority` and `act`. Objective tables contain `common`,
  `strings` and `args` arrays. Rows use zero-based indexes; arrays use Lua indexes.
- `DH2CreateQuestKillObjective(row, objective, current, completed)` creates the
  existing owned kill-objective object from a serialized counted-kill record.
  Objective index is zero-based. Current is a checked signed integer; completed
  is boolean. The object retains its exact dataset generation.

The constructor supports native types 0/10 (KillXEnemies/KillEnemyTemplate),
uses argument 0 as property/template match ID and argument 2 as required count,
and maps these to source event kinds 0/2. Required count must be positive.
The real cache contains 34 supported objectives. Unsupported types reject.
Clear objectives need compiled live-world counts and are not created here.

These are authored development getters/constructors, not original Lua registration.
Float32/int32 Lua numbers retain the runtime's numeric profile. A malformed import
preserves the previous generation; input bytes are copied through a protected
Lua allocation call. Record tables are snapshots, and getter failures do not change
the dataset or prior objective progress.

Quest compile/world/level gating, conditions, automatic event dispatch, markers,
completion observers, rewards, persistence and full source gameplay remain pending.

## Reproduce

```sh
python port/quest-data/build.py --host --report port/quest-data/host-build-validation.json
python port/quest-data/build.py --ndk /path/to/windows-ndk --report port/quest-data/android-build-validation.json
python port/quest-data/tests/differential.py \
  --original /private/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so \
  --cache /private/cache/files --host port/quest-data/build/quests-host.so \
  --arm64 port/quest-data/build/quests-arm64.so --report port/quest-data/differential-validation.json
```

Host execution uses Linux/WSL; Android build uses the Windows NDK. Differential
execution requires Unicorn and pyelftools. Addresses use the ELF zero load base.
