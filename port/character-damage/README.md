# Non-player damage and death request projection

Portable C reconstructs the numeric non-player paths of `Character::HitFor`
(original ELF `0x3a8bc4`, 1,704 bytes). The attacker handle is unresolved. The
source owns four property sheets and receives explicit boundary policy values.
It does not construct the original Character or complete its death state machine.

## Behavior

- An already dead target is untouched.
- Offline damage is suppressed when the local player is absent or dead.
- Online damage is enabled when the manager is absent or its mode is 0 or 5.
- Monster invincibility suppresses the damage delta for monster targets.
- Damage is unsigned raw fixed point; negation wraps in 32 bits. The signed
  arithmetic shift of the damage bits is retained as the original stack local.
- Even a suppressed zero delta runs property Add and its original routing.
- Configuration or debug forced kill runs property Set to zero.
- A final signed HP value at or below zero is set to zero and requests death.
  The death reason becomes 3 unless the target's network virtual method is true.
- A death request does not set the target's dead flag. Original `v2Controller::
  Cmd_Kill` forwards a separate virtual command; that command's owner is pending.
- Property types can make Add or Set ineffective. Their original behavior is
  preserved instead of assuming an unconditional final HP write.

All booleans must be 0 or 1. Invalid inputs, overlapping outputs/inputs and bad
tables reject without committing state or outputs. Buffs are empty.

## Evidence

`differential-validation.json` records 2,835 original ARM32 / compiled source
ARM64 / host comparisons, with zero mismatches. It checks all four sheets,
surrounding original reserved bytes, death reason, death command count, the
damage stack local, source output guards and stack restoration. It exercises
real property routing, synthetic types -1..63, signed/wrapping extremes,
dead/absent players, online modes, debug/config suppression and forced kills.

The actual original HitFor, IsDead, IsMonster, IsPlayer, AI index/type getters,
property Add/Set/Recalc and Cmd_Kill bodies execute. AI records, player manager
lookup, online/config/debug decisions, temporary strings, the network virtual
method, the kill command target and unresolved attacker handle are fixtures.
Player warning/audio behavior and resolved attacker achievements are outside
this projection. No original Lua VM, world, full combat dispatch or game loop
is claimed.

`host-build-validation.json` records 12,000 address/undefined behavior sanitizer
iterations with recovery disabled. `android-build-validation.json` records a
source ARM64 library with 16 KiB ELF alignment.

## Reproduce

```sh
python port/character-damage/build.py --host \
  --report port/character-damage/host-build-validation.json
python port/character-damage/build.py --ndk /path/to/android-ndk \
  --report port/character-damage/android-build-validation.json
python port/character-damage/tests/differential.py \
  --original /private/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so \
  --cache /private/cache/files --host port/character-damage/build/damage-host.so \
  --arm64 port/character-damage/build/damage-arm64.so \
  --report port/character-damage/differential-validation.json
```

The build currently uses the Windows NDK compiler for Android and a Linux/WSL
compiler for host. The differential driver requires Unicorn and pyelftools.
Use ELF zero load base for addresses; imported decompiler addresses add 0x10000.
