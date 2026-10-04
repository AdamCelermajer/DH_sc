# Character target provider reconstruction

This module implements the complete selected Character classification/interaction bodies, base GameObject policies, and the logical handle resolver used by RoomObjectList.GetChar. It is a service backend for the separately proven target-search coordinator. It does not establish native room membership, equipment lifetimes, faction/relationship ownership, or live target acquisition.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Five capture groups bind 37 routines in `original-functions.json` and accompanying ASM files. The ARM64 report binds each manifest, harness, source/header, actual AI asset, literal, oracle and gold corpus hash. No frozen target-search module, existing notes, CMake, renderer or APK was edited.

## Genuine type and property producers

`Character.GetCharAIId` (`0x3a2fec`) reads resolved payload word1 at owner+`0xffc`. It validates signed ID against the actual AI table count and uses row8 for an invalid ID. `GetCharAI` (`0x3a3024`) selects a 68-byte runtime row, and `GetCharType` (`0x3a3054`) reads row+`0x38`. The native `Types16` is a borrowed projection of `data::AiTables.rows[i].type` in original table order; it is not an supplied monster/player boolean. The oracle executes the actual `AIProps::read` (`0x506f3c`) on all 76 original `ai_pyarray.bin` rows. Prince row44 has type1.

`IsMonster` (`0x3a3064`), `IsFaerie` (`0x3a3094`) and `IsSummoned` (`0x3a30ac`) mean type4, type3 and type5 respectively. Character `IsCharacter` (`0x3a2e1c`) returns1; `IsDead` (`0x3a2ed4`) returns raw unsigned byte+`0x1449`, including noncanonical values. No meaning is inferred from a model or dictionary ID.

The cached search fields owner+`0x1310/+0x1314` are resolved payload indices198/199: `Special_Sneak` and `Special_Sneak_Detection`. The cache payload starts owner+`0xff8`, so `0xff8 + 198*4 = 0x1310`. Actual `property-rules.json` names these IDs and gives raw default0, raw/effective type4. Existing native property resolution owns their calculation; this module's `dh2_character_sneak_fields` copies the current signed words from `CombatProperties896`, without converting them to bool or re-resolving items. Original `IsCharacterValid` (`0x4a1ab8`) compares the reference detection against candidate sneak as signed integers; its complete instruction comparison is already in the frozen target-search corpus. The new host audit checks signed extremes and exact payload copies, separately labeled as 25 projection checks rather than original function comparisons.

## IsPlayer correction and exact name rule

`Character.IsPlayer` (`0x3a49f0`, 76 bytes) returns true for type1 and false for every other nonzero type. Type0 loads CString+`0x44`, calls PLT `0x30ebd4` with source literal `0x8c3170`, then compares the returned pointer with the input name pointer. The import relocates to `strstr`, not a name-intern routine. The 16-byte literal is `PlayerCharacter\0`, SHA256 `389930aff155f7a24a27042572dfcfff26916fa5c2b0e37a7c5d05164751bb58`; the test loader's relocated shim address is `0x5011310`.

Therefore type0 recognizes names beginning with `PlayerCharacter`: exact name and `PlayerCharacterPrince` succeed; `xPlayerCharacter`, `Player`, an empty/short string and lowercase spelling fail. Native `strncmp(name,"PlayerCharacter",15)==0` expresses precisely that prefix predicate. Names must be valid NUL-terminated borrowed strings. Null type0 name is rejected at the native boundary; source would dereference it. Type1 does not inspect name.

Earlier conversational intern/name-equality shorthand was incorrect. The existing `reference/character-ai-update/NOTES.md` lines61–71 was already corrected by the parent when this proof was frozen. A scoped production scan found no existing type0 equality implementation: previous modules used explicit virtual IsPlayer boundaries. Prince type1 was unaffected; this statement does not grant broad name-classification parity to other modules.

## Interaction and zonable order

`IsInteractive` (`0x3a4870`) invokes candidate virtual IsDead first. If dead and other is present, it calls candidate AI's **AI_IsFriend** (`0x3d511c`). A true result and a nonmonster candidate return1 immediately, bypassing the remaining gates. Otherwise it rejects disabled byte+`0x81`, invisible byte+`0x8a`, type3 and type5, then calls virtual IsDead again. A living candidate must have flag+`0x520 & 0x2000`; success returns raw byte+`0x415`. The Friend symbol is `_ZNK6CharAI11AI_IsFriendEPK10GameObject`, 720 bytes, instruction SHA256 `5b7cc3cd157969e3301a40fbec86806e68de7ed8cc453fff3fab3f3f7500f81c`. An earlier master/nonsummoned description was wrong; no existing native master enum implementing this routine was found. Friend remains an explicit genuine service, never autoaccepted.

`GetInteractionType` (`0x3a47e8`) calls candidate AI_IsEnemy(other) when other is present. If enemy and other virtual IsPlayer is false, it returns8 immediately, including dead/summoned candidates. Otherwise summoned type5 returns-1; virtual dead returns-1; living monster returns8, other living types3. `IsZonable` (`0x3a36e4`) invokes own virtual IsPlayer; true returns0. Type3 then returns0; otherwise it tailcalls the actual base `GameObject.MeetCondition` (`0x38ab60`, returns1). These are synchronous calls. Native state/table fields are read after services at their source points, and tests mutate AI ID, visibility and flags while reentering IsZonable.

Native callbacks have Request24 `{service,reserved=0,subject,other}` and write an uintptr result. Service1/2 are actual virtual Dead/Player; service3/4 are actual Friend/Enemy; service5 is actual virtual IsCharacter. Subject/other are stable borrowed object identities. Callbacks may reenter the module and mutate live valid projections; storage/table/string lifetimes and alignment must remain valid throughout the call. Faction ownership and Friend/Enemy internals are explicit unresolved backend boundaries here, even though the Friend source body is captured. The constructor's flags1 target-search path uses Enemy, not Friend.

## Radius and base object policies

Base GameObject `IsInteractive` (`0x3883b0`) returns0, `GetInteractionType` (`0x38ad74`) returns-1, base ObjectBase `IsCharacter` (`0x33dcd0`) returns0, and base `IsZonable` (`0x3883b8`) returns raw byte+`0x2ed XOR 1`. Native base query tests all 256 byte values.

Base `GetInteractionRadius` (`0x38ad7c`) computes X extent `hiX-loX`, Y extent `hiY-loY`, replaces X with Y only when X is strictly greater than Y, then multiplies by0.5 in single precision. It uses the smaller XY extent, not the body-radius maximum. Its unordered comparison preserves source NaN classification, source subtraction order and signed zero behavior. The 512 original/ARM64 cases include inverted bounds, infinity, NaN, subnormal and random float words; NaN payload equality is not claimed.

Character `GetInteractionRadius` (`0x3a374c`) is an 8-byte tail to `AI_GetMeleeRadius` (`0x3d4c34`). The already proven `dh2_attack_melee_radius` in `character_attack_geometry.hpp` supplies this producer over current real equipment/item/AI inputs: integer weapon Param5 plus decoded AI row melee_radius, invalid AI ID fallback8, ranged weapon query result ignored. Reuse that kernel; do not substitute base AABB radius or rendered body radius. This new corpus does not duplicate the existing 9,393-case geometry proof.

## Complete handle path and borrowed ownership

RoomObjectList `GetChar` (`0x4a1cfc`) invokes its current object virtual+`0x18`, then actual `ObjectBase.GetHandle` (`0x33dd2c`) and `ObjectHandle.operator Character*` (`0x33ff54`). GetHandle dereferences object+`0x2c`, writes ObjectManager+`0x78` frame into the shared handle+8, and copies its 12 bytes into a local handle. A null room object is unsafe in this real source path. The earlier frozen search corpus's null entries were explicit GetChar service fixtures, not a claim that this source handle chain safely accepts null objects.

`GetObject` (`0x33fdc0`) returns0 for key0, regardless of cached pointer. For nonzero key, a nonnull cached pointer with matching current frame bypasses lookup. Otherwise it performs actual signed map operator[] (`0x33fc88`), inserting an empty record when absent, then reads ObjectListItem+`0x18` and updates **local** cached pointer/frame. The Character conversion invokes resolved object's actual virtual+`0x24`; false returns0, true returns the retained object pointer. Shared cached pointer is not updated. Fresh GetHandle stamps shared frame before the copy, so an old cached pointer becomes current by this path even when its prior frame was stale. This source behavior is retained, not repaired into a fresh registry lookup.

Native Registry24 borrows caller-owned ordered Record16 `{signed key,reserved=0,object identity}` storage, count/capacity and frame. It implements equivalent map lookup/empty insertion and local/shared cache effects. It does not reproduce original RB node pointers, strings, tree allocator, registration/destruction or ObjectManager lifetime. Object identities/shared handles must remain stable even when array records shift during insertion. Local/shared/output/registry/record storage cannot overlap. Capacity exhaustion returns2 after frame stamp/local copy, leaving result output untouched. Entry validation returns1 before changes; service failure returns2 after prior source-order effects. Callers must distinguish these from an ordinary successful null resolution.

The original oracle creates and mutates the real signed RB map through actual map operator[]; string allocation/copy, insert_unique, node copy and RB balancing instructions execute. Only heap allocation/deallocation and the room virtual object getter are storage/provider fixtures. The actual Character/base IsCharacter bodies execute. Cases include negative/INT_MIN/INT_MAX keys, zero key with nonnull cache, missing-key insertion, wrong cached object identity, stale prior frame, frame wrap and synchronous nested resolution. The actual copied local stack handle and full in-order registry effects are compared.

## Frozen proof and reproduction

`reports/character-target-providers-arm64-differential.json`: 3,550 original versus optimized ARM64 cases, 2,241 ordered callbacks, zero mismatches: 1,762 Character predicates, 252 complete handle cases, 512 IEEE radius cases and 1,024 base policies. `reference/character-target-providers/provider-fixtures.bin` is their PVD1 host corpus. `reports/character-target-providers-host-audit.json` replays the same gold under ASan/UBSan, adds malformed/alias/provider/capacity and signed projection checks, and binds exact compiler inputs/executable with zero findings.

From repository root with the supplied Python environment:

```powershell
& port/level-world/tools/build_character_target_providers_oracle.ps1
python port/level-world/tests/character_target_providers_differential.py --library .local-inputs/character-target-providers-discovery/libcharacter_target_providers.so
python port/level-world/tests/character_target_providers_host.py
```

Parent CMake integration should add production `character_target_providers.cpp`, host target `character_target_providers_audit` from `tests/character_target_providers.cpp`, link actual dh2_level_world, and run with the PVD1 path. Optional `tests/character_target_providers_host.py --main-linked /home/adampalace/dh2-world-build/character_target_providers_audit` records current DSO/dependency hashes separately. Main-linked/packaged instruction proof and live game wiring are not claimed by this isolated handoff.
