# Additional AIS kinds and actual Crypt external-script producers

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `capture.py` records six complete original vtables and108 routines. `virtual-dependencies` captures the actual alias-membership/Call/state-update dependencies. `kind-probe.json` executes24 factory/lifecycle cases and4100 VCB truth-table cases; backend allocation, private VM creation/close, strings and alias/VM calls are explicit services.

## Authored identities change the integration priority

`authored-ai-rows.json` verifies all six character/AI input blobs byte-for-byte against the supplied cache and binds both current Crypt provenance reports. AI is authored property index1. Direct Crypt_Skeleton, Crypt_SkeletonXL, CryptSlime and CryptSlime_RE use AI row40, Script `monster` (7 bytes); Crypt_Ghost uses row68 with the same script. DefaultFairy uses row20, Script `follower` (8 bytes). WanderingPriest uses row50, Script `rene` (4 bytes). These names lack a `__` prefix, so actual SetScriptByName3ceeb0 selects **AISExternal**, not builtin AISMonster/AISFaery. Scripted becomes1 after factory selection. No `__monster__` or `__faery__` script appears among the76 supplied rows. This is authored input evidence; final live property resolution and spawning remain separate producers.

The same report binds exact cache paths/bytes for `_commons.luac`, `monster.luac`, `follower.luac`, `rene.luac`. Source external loading uses the actual owner path `data/scripts/ai/`; Stage3 common precedes Stage4 external script. Native registration providers must implement those scripts' actual dependencies before claiming their successful execution. Current prototype VM/owner audits do not implement the full namespace.

`__npc__` maps to AISDefault and preserves the old filename field. There is no separate AISNPC factory/vtable in the recovered selector. An empty Script falls back to ordinary AISPlayer only when owner name is exactly `Player`, otherwise AISDefault; scripted0 is a selection flag, not a no-VM constructor.

## Factory and teardown facts

|Class|Factory/ctor|Original ARM32 bytes|Additional constructor stores|
|---|---|---:|---|
|AISDefault|3cce14→CharAIScript3d8fb0(true)|0xc4|flags+b8,counter+bc,frame+c0=0|
|AISMonster|3ccbe4→CharAIScript3d8fb0(true)|0xc4|same three zero stores|
|AISFaery|3cccf8→CharAIScript3d8fb0(true)|0xc8|same three zeros,last-master value+c4=-1|
|AISExternal|3ccaf4→3dd0e4→CharAIScript3d8fb0(true)|0xc4|same three zero stores|

All retain the base owned private deferred VM, empty alias/file/state collections and null borrowed Character/current-state pointers. The original allocator sizes are evidence; do not allocate native objects with ARM32 sizes. Default/monster/faery inherit the actual empty Init/InitPost/InitFinal/Terminate methods. External initial methods call Lua. Deleting destructor paths all reach CharAIScript→LuaScript→owned Instance close before freeing the original allocation. Empty-container original probes observe one close and one free; a nonempty AI-state collection is not newly implemented here.

Correction to frozen owner commentary: AISDefault is0xc4, not0xb8. It does have initialized+b8/+bc/+c0 fields. `ScriptConstructorFields72::derived_present==0` for default currently means player-only vector/skill extension is absent; it must **not** imply that flags/collision-counter/frame fields are absent. Parent may correct the header comments `Only constructor-written fields. Derived fields...not the smaller AISDefault allocation` to distinguish common AISDefault fields from player-only extensions, without changing the already correct zero-valued projections or historical reports.

## Complete initial virtual and callback-flag kernel

NEW `character_script_virtual.hpp/.cpp` implements two bounded operations. `dh2_character_script_initial_virtual(session,kind,slot)` handles actual initial slots8/c/10/14 (Init/Post/Final/Terminate) for all six captured classes. Only kind `script_external` calls the literal corresponding Lua name, through the existing alias resolver and genuine VM discard-source call. The inherited empty methods have no invented VM calls. Native VM diagnostics are exposed; source callers ignore LuaScript Call results, so later owner integration must distinguish a delivered source Lua error from malformed native/provider failure instead of turning a protected diagnostic into script acceptance or an invented rollback.

`dh2_character_script_init_vcb(flags,aliases,external)` reconstructs Default InitVCB3dc7d8 and external InitVCB3dcec8. The queried function37c2a0 is **LuaScript::IsInVFTable**: unsigned-hash membership of the main alias map+34. It does not query Lua globals and is not HasFunction. Default clears flags first, queries two names in order and stores after each result; external calls that base then adds10 ordered bits. Membership can be true even if the mapped Lua global is missing.

|Order|Requested alias key|Flag bit|
|---:|---|---:|
|0|OnTargetHit|0x800|
|1|OnTargetMissed|0x1000|
|2|OnUpdate|1|
|3|OnFriendSpotted|2|
|4|OnTargetOutOfRange|4|
|5|OnTargetInRangedRange|8|
|6|OnTargetInCloseRange|0x10|
|7|OnTargetInMeleeRange|0x20|
|8|OnMasterOutOfRange|0x40|
|9|OnMasterInRangedRange|0x80|
|10|OnMasterInCloseRange|0x100|
|11|OnMasterInMeleeRange|0x200|

There is no source once-only guard or persistent boolean replacing this recomputation. The pure module uses actual existing native alias membership. Both default masks (4) and external masks (4096) were executed against original instructions, including all intermediate flag observations and query order.

## Remaining external update and event boundaries

Complete actual OnUpdate3dce64 order: AISDefault.OnUpdate3dc798 (already reconstructed source pause/collision kernel); reload flags+b8 and call Lua `OnUpdate` iff bit1; call CharAIScript.CallStateUpdate3d8eb4; then CallStateConditions3d8ea0. The latter helpers each independently reload current state+b4: nonnull update reads state+14, conditions reads state+2c, then calls LuaScript. A callback can change current state between them. Do not cache one state pointer/name for both calls or omit the state calls when OnUpdate bit is absent. A full native state-record registry and its RegisterAIState/ChangeAIState producers are not provided by the initial module.

The complete captured vtables explicitly identify all inherited and overridden event bodies. External no-argument range methods use their matching VCB bits; FriendSpotted uses bit2 and pushes a source GameObject userdata representation. Enemy/neutral/death/revival/projectile and several other callbacks are unconditional overrides. Source OnCombatResults ignores its GameObject argument and pushes the supplied result pointer as a Value through31a46c. GameObject userdata push386f28 builds the original wrapper object; do not substitute raw identity for every such event without recovering that Value producer. Inherited methods with native Character side effects remain real services/kernels, never accepted no-ops. Monster/fairy OnUpdate and their override events are also substantive native bodies; their constructor ownership alone is insufficient for live dispatch.

## Exact owner extension plan

Keep frozen `ScriptOwner` supported-kind failures until integration is authorized. A functional external extension must: permit source external construction with common fields; retain its private VM/alias/file set; perform Stage4 real cache/manager loading; dispatch initial slot8 through this module after owner timers; invoke external InitVCB after that Lua Init; preserve actual delivered Lua-error/source-false prefixes while reporting native/provider failures; publish active only at source Stage6. Default/monster/faery InitVCB uses the base operation. Add explicit faery+c4 field ownership if builtin faery is enabled. Player-only vector/skill fields remain separate.

Stage1 global implementations, cache manager buffer ownership, nested Include capability, Character bindings and nonempty AI state registry remain explicit providers. Include/cache worker owns the source-specific nested operation; the frozen owner's ephemeral advance services cannot serve later Include calls without an explicitly retained provider lifetime. Never fake globals, default external classes to AISDefault, invent current-state records or silently accept unsupported event methods.

## Validation and reproduction

`reports/character-script-virtual-arm64-differential.json`:4,124 comparisons (4100 VCB+24 initial virtual),6 atomic native rejections,0 mismatches; optimized Android26 O2 helper library. Original exact factories and empty-container teardown are discovery probes, not full native ownership parity. Alias/VM bodies are explicit services in this CPU comparison.

`reports/character-script-virtual-host-audit.json`:49,244 ASan/UBSan checks,0 findings, actual rebuilt script-runtime DSO and alias symbols verified by dladdr. It replays all4100 original VCB masks through native STL alias membership,24 initial methods,5 genuine Lua calls,4 actual returned-table `_this` projections, fresh alias target global replacement,20 inherited empty methods, missing-target protected error and caller guards. Missing mapped globals still set VCB bits; no full script namespace or whole-AI frame claim.

Build with `tools/build_character_script_virtual_oracle.ps1`; run `tests/character_script_virtual_differential.py --library .local-inputs/character-script-kinds-discovery/virtual-oracle.so`, then `tests/character_script_virtual_host.py`. Parent CMake handoff: add new cpp to world DSO linked to actual script runtime; test cpp takes `reference/character-script-kinds/virtual-fixtures.bin` and links world+runtime+dl. Frozen owner/runtime/CMake and packaged APKs were not edited by this task.
