# Bounded original InitPost FX prefix

`character_init_fx.hpp/.cpp` adds Character::RegisterCharacterFXTable3b4738
and the empty/invalid GrabAnimFX495430 prefix. It reuses the genuine existing
visual_fx_preload registration and DebugModules backend. No FX instance, scene
factory, particle/material animation or GPU acceptance is supplied.

The real getters3a3300/3a33d0/3a3368 read cached resolved Character Effects
index7 atCharacter+1014. Negative/out-of-range indices select row0. Register
captures Footprint then Blood then BloodDeath values before the shared
Debug.load/GetSwitch("isTracingChar_Init") prefix. It delivers each captured
nonnegative ID to genuine RegisterFXSetToLoad4967e8 in that order. A synchronous
callback mutating rows does not replace those captured IDs. Source row backing
and caller services remain alive across nested calls.

`dh2_character_init_fx_register` borrows InitFxRows16 and genuine
PreloadTable16/queue/services. Root's EffectsTables::Borrow.characters() supplies
the row array; keep that borrow alive. Root's PreloadBacking supplies the source
276sets/284dictionary IDs. Use one shared persistent DebugModules with
dh2_fx_debug_preload_service. The source Debug switch query result is ignored;
the separate AnimatedFX module result actually gates each registration.

`dh2_character_init_fx_negative_grab` requires Debug.load and
GetModule("AnimatedFX") even for negative IDs. Disabled module, signed negative
ID or ID>=current count returns1/outputNULL. Enabled valid IDs return-3 with
output unchanged, explicitly stopping at the unreconstructed _GetAnimFX/pool
factory continuation. Do not turn -3 into an accepted no-op. Missing providers
return-2 at the delivered prefix; malformed inputs return-1 before callbacks.
The native4096 table/count and preload depth128 limits are explicit port bounds.
Generic malformed aliasing or concurrent mutation is outside the borrowed API
contract; callbacks may change valid rows/counts synchronously as documented by
the underlying preload service.

Actual original cache reader projection in game-data/reference/effects-tables
has Character row1 blood79, footprint/blood-death-1. The separate source-proved
property/AI cache projection identifies Effects1/self_fx-1/FXoffset0 for all four
Crypt kinds. Thus startupGrab(-1) legitimately returnsNULL, while blood79 must
register its actual nested dictionary effects (including FX77). Registration is
not effect loading/playback: the actual source registration body does not create
AnimatedFX. Original InitPost loads the script at3b5010, executes Grab3b5040 and
Register3b5050, registers the animation set3b5090, creates the physical body
through Revive, then later calls InitScriptProcess3b54c8 when its source gate
allows it.

Evidence: character-init-fx-arm64-differential.json binds480 actual original
versus optimized ARM64 cases,4592 ordered services,360 registration cases,
120 empty/invalidGrab cases,72 captured-row mutation cases, mismatch0. Actual
recursive source registration instructions execute. Caller singleton identity,
table storage, Debug results and string ownership are explicit fixture services.
The valid Grab factory is never accepted or substituted in this corpus.

character-init-fx-host-audit.json binds the same480/4592 gold to O2 ASan/UBSan,
24881checks,8 atomic guards,2 failed-prefix checks,1 unsupported-factory guard,
one supplemental synchronous host reentry, and2 genuine shared Debug/preload
compositions using an explicit missing-file provider fixture. The supplied
blood79→dictionary77 table in that supplemental host test is a controlled input;
root's independent full cache reader/backing proof owns actual cache parity.
No sanitizer diagnostics remained. This is isolated-new-source plus shared
host DSO service proof; no packaged ARM64, full original InitPost or live claim.

Parent integration: add character_init_fx.cpp to world, compile
tests/character_init_fx.cpp linked to world, run with
reference/character-init-fx/init-fx-fixtures.bin. The test also calls frozen
visual_fx_preload/character_design_services exports. No CMake files changed here.
