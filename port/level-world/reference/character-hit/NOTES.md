# Character HitFor: offline monster continuation

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Complete `Character::HitFor(unsigned,GameObject*)` is at `0x3a8bc4`, 1704
bytes, SHA256 `b01deb99cff30fee694bd246951f00f863c82a3c0135b3302290a475443f8230`.
The manifest/assembly preserve the whole routine, including excluded branches.

The native bounded branch completes offline nonplayer receivers whose handle
tail resolves null, a nonplayer Character, or the receiver itself. This covers
the actual self/self monster DoT call. It executes existing genuine property
and target-handle kernels; no replacement health sheet, dead flag or AI owner
is introduced. Full Kill is a required borrowed service. Online, receiver-player
and distinct-player-attacker achievement paths return unsupported at their
reached source boundary, after preserving original prefixes.

## Actual source order

1. Receiver virtual `IsDead` (+34) at `3a8bf8`. True returns immediately,
   before any game, debug, property or attacker-handle work. Null attackers are
   therefore allowed for this prefix; reaching their original unsafe GetHandle
   dereference is an explicit native unsupported boundary.
2. Snapshot cached HP36 (+1088) and MaxHP38 (+1090) at `3a8c40/4c`.
   GetCoopGame `36e478(Game,0,true)`, then capture returned game.mainPlayer+660
   at `3a8c64`. This is not Game.GetHostPlayer and is not reread later.
3. Debug.Load `337888`, Debug.GetSwitch `337a88` for exact `GOD_Monster`
   (source literal `8c3410`). A true switch calls real IsMonster `3a3064`.
   True monster suppresses damage and skips the online/main-player checks.
4. Otherwise COnline getter `7fd794` reads byte+5. The online body starts at
   `3a8cb4` and is outside this native domain. Offline: null captured mainPlayer
   suppresses damage; nonnull invokes its live IsDead at `3a8f48`. Alive yields
   wrapping `0-damage` and signed arithmetic `damage>>8` credited points. No
   dt, duration, positive scaling or HP clamp precedes the property operation.
5. Always PROPS_Add(36,raw_add) `3e0708`, even if raw_add=0 (`3a8d10`).
   Source type32 writes saved HP then resolves; type8 writes resolved HP.
6. Application.DebugSwitch `320e14` for exact `OneShotKill` (`8c3420`) AFTER
   Add. If false, Debug.Load/GetSwitch follow at `3a8fdc/8ff8`. A true result
   queries receiver IsPlayer (`3a8d3c`). Nonplayer PROPS_Set(36,0). Player's
   child+418 classification branch starts `3a9038` and is explicitly unsupported.
7. Read CURRENT cached HP at `3a8d5c`. HP<=0 sets36=0 again at `3a90f4`,
   reloads CURRENT controller+378 at `3a90fc`, invokes Cmd_Kill(attacker,false)
   `40570c`. Only after successful full delivery: receiver live remote query
   v54 at `3a9114`, then lifecycle+11c=3 iff false. The native method never
   writes dead1449. `HitResult40.after_hp` records this pre-Kill HP read, not
   a later artificial final HP snapshot.
8. Two separate live receiver IsPlayer queries at `3a8d7c` and `3a8f70`.
   Each zero skips its player low-health/audio/tutorial/arming branch. They
   cannot be cached or inferred from a prior classification callback.
9. Actual attacker GetHandle `33dd2c` at `3a8e60`, actual Character cast
   `33ff54` at `3a8e68`: GetHandle stamps the SHARED handle frame from manager
   +78 before copying; local resolver `33fdc0(...,false)` preserves existing
   nonzero cached pointer, otherwise uses actual signed-key map operator[]
   `33fc88`, whose missing key inserts an empty record. Only LOCAL cache fields
   are refreshed. Nonnull result invokes its virtual IsCharacter (+24).
10. Null Character returns. Otherwise attacker IsPlayer (+28) at `3a8e88`;
    false returns. A self pointer also returns (`3a8e94/98`), even if this
    final attacker query differs from earlier receiver queries. Distinct player
    enters host-player/achievement tail at `3a8e9c` and is unsupported here.

There is **no unconditional AI event or hit-FX dispatch in nonlethal self/self
monster HitFor**. The old health-only audit deliberately omitted the handle
tail and thus could not claim complete HitFor; the new coordinator executes it.
Remaining CancelSneaking/Text/Sound in enclosing F_ApplyResult are separate
required services and must not be accepted merely because HitFor completes.
Historical DoT notes mentioning generic unbound HitFor/AI/hitFX were broader
than the actual self/self method; those frozen reports remain historical.

## Kill boundary is substantial

Captured separately in `kill-boundary-functions.{json,asm}`:
Cmd_Kill `40570c` reads controller.controllable+4, calls its virtual+58 with
attacker and force=false. No controller lock/forced/global-debug gate exists
in this command. The corpus executes these instructions; virtual+58 is an
explicit oracle fixture only.

Actual Character.Ctrl_Kill `3ad528` first queries IsDead; if alive calls
Character.Kill `3a5b18`, then TAIL CALLS Character.RaiseAIEvent `3a4d5c`
with event2 and attacker (`3ad564..570`). That event must not be omitted by a
future live Cmd_Kill provider.

Kill itself queries IsDead again, sets dead1449=1 at `3a5b5c`, sets HP36=0,
then performs player/level/reward/aggro/owner/remoteness and scene FX paths.
It records killer+144c, processes aggro participants and ownership, can call
Character.OnCharKill `3bf828`, and uses level dispatch/FX services `339090`
after remoteness/byte14e4 gates. None is reconstructed by this coordinator;
we do not substitute health_hit/combat_application or mark dead on its behalf.

## Native API and lifetime

`character_hit.hpp`: HitActor32 carries retained Character identity, PropertyView,
live controller and lifecycle. HitAttacker24 borrows the attacker's actual shared
Handle16 and registry. Registry backing/key order/capacity and all projection
pointer identities remain stable through the invocation. Sheet values and
controller/lifecycle may change synchronously. Handles/objects must stay alive;
this adapter does not make stale cached source pointers safe.

HitServices16 returns delivery0. All queries are explicit and live. The
main-player callback must perform source GetCoopGame(0,true)->mainPlayer, and
Kill must perform the complete Cmd_Kill backend. Missing services fail with
-2 after the reached prefix. Scalar output is committed only on an accepted
invocation. Malformed inputs return -1 without callbacks or mutations; unsupported
body branches return -3 with their source prefix; complete bounded branch1.
No once-only or reentry suppression is added.

## Proof and reproduction

`tools/build_character_hit_oracle.ps1` builds an isolated O2 ARM64 DSO with
existing properties.cpp and character_target_providers.cpp. Python
`tests/character_hit_differential.py` executes actual original HitFor, property
write/type/resolution, Cmd_Kill virtual dispatch, handle/cast/cache/map routines.
Game/debug/virtual/full Kill/allocation are explicitly supplied fixture services.
Supported source blocks are not skipped. Unsupported player/online/null-attacker
branches stop at their exact reached boundary and compare only those prefixes.

Final corpus: 2600 golden cases plus16 original/native synchronous reentry
cases;31205 ordered requests;251 unsupported prefixes;10 atomic malformed
guards;zero mismatch. Outputs, all four sheets, shared handle, inserted registry
records, lifecycle and ordered calls compare exactly. Raw unsigned damage
includes signed-ASR and wrapping boundaries. Callback mutation covers live
HP/controller/lifecycle rereads; reentry at Application switch runs another
actual HitFor on the same receiver before the outer HP read.

Host `tests/character_hit.cpp` replays all2600 gold cases through an isolated
sanitized module linked to ACTUAL central world/data DSOs. New runner
`tests/character_hit_host.py` binds DSO/executable/source/input hashes and verifies
dependencies stay unchanged. Genuine compositions use real missing-file fopen
(ENOENT only), owned DebugSwitches, source-derived76-row target types, property
and handle kernels, aggression/vitals and actual DoT Calculate/Apply.
Game/online/Application/party remain explicitly supplied session fixtures.
Two nonlethal HitFor calls finish; lethal stops at missing full Kill BEFORE
remote/lifecycle/dead effects; two same-receiver reentries finish; registry
capacity failure preserves reached HP/handle stamp; enclosing DoT completes
HitFor then fails at required CancelSneaking. No full live DoT/FX/Kill or
original whole-frame parity is claimed. ASan/UBSan findings0.

Parent integration: add `character_hit.cpp` to world DSO; target
`character_hit_audit` from `tests/character_hit.cpp`, linked world+game-data+dl.
No libm wrappers/interposition. Run direct Python
`port/level-world/tests/character_hit_host.py --main-linked` after central rebuild.
This worker changed no frozen module/CMake/renderer/APK or shared build.
