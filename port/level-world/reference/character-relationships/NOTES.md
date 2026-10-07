# Source Enemy and Friend relationship bodies

The new `character_relationships.hpp/.cpp` implements `CharAI.AI_IsEnemy` (`0x3d574c`) and `AI_IsFriend` (`0x3d511c`) over current borrowed handles, object/cache projections and genuine decoded faction rows. It is a backend for the frozen target provider's Enemy/Friend services. No source master relationship is consulted by either body. No same-faction acceptance, nearest-target substitution, faction fallback bool or unconditional accepted interaction predicate is introduced.

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Four new capture groups bind10 complete routines: two relationship bodies; actual faction ID/table getters, table/row/entry readers; ObjectBase/GameObject/Character constructor producers. Existing frozen provider captures bind the actual handle/map/type/virtual helpers that execute in this new oracle; property resolution reuses its existing original capture. These dependencies and actual asset bytes are bound in the reports. No existing provider/search/CMake/renderer/APK file was changed.

## Exact identity and call order

Both calls accept an explicit GameObject or select live AI+`0x40` when the argument is null. A missing selected object returns0. They execute actual const `GetHandle` (`0x33dd70`) and `GetObject(false)` (`0x33ff8c`→`0x33fdc0`) without Character conversion. GetHandle stamps the shared handle's frame and copies it; the resolver honors current nonnull cache, otherwise performs signed map lookup/empty insertion. The returned object can differ from the original input object. These effects occur before relationship virtual calls.

A resolved object's **ObjectBase GO_ID word+`0xf4`**, rather than its AIProps Type, must be0 to take the Character/faction branch. `ObjectBase` constructor `0x33f310` retains its incoming GO_IDS argument in r7 and stores it at object+`0xf4` (`0x33f488`). GameObject constructors forward that argument; Character's `0x3aa1b4` forwards into GameObject C2. These captures identify the field producer; the module borrows the actual caller's GO_ID rather than inventing factory classification from model name or AI row. Source factory/object registration ownership is not reconstructed here.

Friend returns0 immediately for unresolved or nonzero-GO_ID results. Enemy instead queries the **original selected input object** virtual IsInteractive(owner), and, when nonzero, GetInteractionType(owner)==8. It reloads AI owner+4 separately for each fallback call. It does not switch these fallback virtuals to the resolved/cache object. Tests bind actual Character/base GameObject virtual bodies, including recursive relationship calls from the actual Character interaction body. Other derived interactive classes require their genuine vtable/backend services and are not autoaccepted.

In the Character branch, source validates faction IDs with direct getters, then Enemy invokes current owner virtual IsPlayer. Only when owner was a player does it invoke resolved candidate virtual IsPlayer; two players returnfalse before faction traversal. Candidate pointer and the initial GO_ID decision remain retained. Afterwards source reloads the AI owner and faction table, obtains current owner/candidate faction IDs and scans the owner's row in serialized order. Friend omits the player virtuals and player-pair exception.

The first entry whose ID equals candidate faction determines the result: Enemy returns its signed sign bit (`value<0`); Friend returns `value>0`. Zero and absent entries returnfalse. Equal faction IDs have no extra rule. Repeated IDs must remain ordered; a later opposite-sign entry never overrides the first match. Callback return words represent actual 32-bit virtual return values. The native coordinator's explicit result pointer distinguishes success0 from malformed boundary1 and provider/capacity failure2, after prior observable effects.

Source diagnostic assertion/log paths for corrupt global counts are outside the valid table domain. Native tables require at least11 rows so fallback10 exists; actual cache has16. Native boundary rejection is not a reconstruction of the original diagnostic crash/log machinery.

## Genuine faction and property producers

Actual `AIFactionTable.read` (`0x4b3a38`) executes on original `ai_factions_pyarray.bin` SHA256 `d302382d9b8079e71f9915d6fed7c262b1f9f8bb8f1a0eb6f060859a1a992e3d`. Its actual `AIFactions.read` (`0x4dd158`) and `AIFaction.read` (`0x4eb408`) allocate/fill all16 rows and71 entries through original stream-reader instructions. Runtime row and entry strides are12, including their header/vtable words; the logical native row borrows `data::AiFactionEntry {id,value}` from already decoded `AiTables.factions`, without supplying an Enemy/Friend bool.

`Character.GetCharAIFactionId` (`0x3a3180`) reads resolved payload word0 at owner+`0xff8`; a negative/out-of-range ID uses10. This is property `AIFaction`, raw default10/type1 in the actual property rules. `GetCharAIFaction` (`0x3a31b8`) selects that runtime row. AIProps Type is separately produced by resolved property1/actual AI table, as established by the provider proof. Friend/Enemy do not use master/target-sticky properties to override faction lookup.

The new oracle executes actual `RecalcProperty` (`0x3dfe60`) with actual CharacterTable default/type and base rows, initialized default saved/gear/resolved sheets, and an empty buff map. It resolves property0,1,198,199 for all ten Crypt-prefixed rows33–42 and KnightPlayerBase row263. These are explicitly base/default/saved/gear fixtures; they do not claim runtime class/buff/equipment composition or spawn ownership.

All ten Crypt rows resolve faction7; their AI IDs are40 (slime/skeleton),68 (ghost/dog), or25 (ghost midboss). KnightPlayerBase resolves faction11/AI44. The actual directional faction7 row has `11:-1000`, and faction11 row has `7:-1000`: both directions are hostile. Their current fixture Sneak/Detection words are0. The complete 11x11x2 relationship matrix contributes242 original/native cases.

The signed search fields +`0x1310/+0x1314` are `Special_Sneak`198 / `Special_Sneak_Detection`199, not generic visibility/interaction words. Original cached payload starts+`0xff8`. The earlier `reference/prince-live-attack-services/NOTES.md` line29 naming was identified for parent clarification; this proof does not edit that frozen note. The earlier mistaken label “Master” for `0x3d511c` is corrected to the verified `_ZNK6CharAI11AI_IsFriendEPK10GameObject` body.

## Native ABI and ownership

`dh2_character_relationship(out, op, State16*, candidate, Registry24*, Factions16*, Services16*)`, op1Enemy/op2Friend. State16 owns no objects: its owner/target pointers are current projections of AI+4/+40. Object48 embeds the frozen provider Character32 projection, shared Handle16 pointer and actual GO_ID. Noncharacter objects may have null Character properties; AI owners and resolved GO_ID0 objects need valid resolved-cache storage. Identities, objects, handles, strings, property sheets, table rows/entries and callbacks must remain live and valid throughout synchronous calls.

Registry24 is the frozen provider's signed logical map projection. Its record.object stores an Object48 pointer here, not a classification bool or the object's numeric callback identity. Shared handles cache Object48 pointers. The borrowed record array has caller-supplied capacity and can shift records on empty insertion; object/handle addresses remain stable. It does not emulate original RB tree allocation, object publication/removal or ObjectManager destruction. Caller storage must keep mutable registry/handle spans distinct from projections/tables and result storage. A failed map capacity check occurs after shared-frame stamp; output remains untouched.

The three genuine virtual service boundaries are1 IsPlayer,2 IsInteractive,3 GetInteractionType. Request24 carries stable numeric subject/other identities (zero other for IsPlayer). Tests connect these to actual native frozen provider kernels, with recursive Enemy/Friend services returning this coordinator's actual result. Actual source virtuals execute in the ARM32 oracle; no relationship boolean is returned by a fixture. Selected virtual callbacks may synchronously reenter, replace the owner or mutate faction cache words. Those values are read again at their source points. Full AIS/room publication/current-registry integration is a separate caller responsibility.

## Frozen proof and integration

`reports/character-relationships-arm64-differential.json`: 4,332 original versus optimized ARM64 cases,4,789 ordered callbacks, zero mismatches. Cases include every actual faction pair/player type combination, invalid faction fallback, directional/missing/duplicate entries, signed value extremes, source type0 player prefix, explicit/live/absent targets, cached identity remapping, signed/INT_MIN/INT_MAX map keys/empty insertion, noncharacter fallback with null Character cache, actual recursive interaction/dead-friendly branches, owner/faction replacement and nine synchronous nested Friend queries. Shared frame stamps, owner identity, current faction words and full ordered map contents are compared.

`reports/character-relationships-host-audit.json`: same4,332 original-derived records under ASan/UBSan, plus21,660 atomic boundary checks and12,996 provider/capacity/malformed-resolved-pointer checks; zero mismatches/findings. Gold `reference/character-relationships/relationship-fixtures.bin` SHA256 `741c5e74bd691ae67c53f43ef9b3a043588dcde41a3ef385c42c2d7b3b21a002`. Production CPP SHA256 `94c45e7344ae68f77520405aca003ddd7cc2cceacfcbbb29e7bb8442cf4cc321`; header `360426e4401b72b07512164f0321c09a80f4e4c9a5f08388cbc526ef79079cc4`. Isolated optimized ARM64 oracle `4c272314a40f6297b7c128c9eee30dde17d2ff82d735aa2e4ca3ed314241663c`.

From repository root, with the established Python/Unicorn environment:

```powershell
& port/level-world/tools/build_character_relationships_oracle.ps1
python port/level-world/tests/character_relationships_differential.py --library .local-inputs/character-relationships-discovery/libcharacter_relationships.so
python port/level-world/tests/character_relationships_host.py
```

Parent CMake: add `character_relationships.cpp` to actual world target; host target `character_relationships_audit` from `tests/character_relationships.cpp` linked to the actual world DSO containing frozen target providers. Run with the REL1 gold path. Then:

```powershell
python port/level-world/tests/character_relationships_host.py --main-linked /home/adampalace/dh2-world-build/character_relationships_audit
```

This optional command records exact current DSO/executable/dependency hashes in a separate main-linked report. It does not claim compiler-input provenance unless supplied by the parent's build capture. No central build, packaged instruction audit, live renderer binding or gameplay parity is claimed by this isolated handoff.
