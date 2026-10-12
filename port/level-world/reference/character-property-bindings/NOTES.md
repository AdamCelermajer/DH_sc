# Character GetProp Lua property binding

The additive `character_property_bindings.hpp/.cpp` implements actual `Character::_GetProp` and the raw word service used by `CharProperties::_GetProperty` / `PROPS_GetFromSheet`. It reads genuine current borrowed sheets; it never recalculates a property or substitutes a fixed value. `dh2_character_property_bind` installs `GetProp` through the existing source-Value projection bridge. No existing owner/runtime/header/CMake/renderer or frozen report was edited.

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Complete captured bodies:

| Function | Address | Bytes | Instruction SHA256 |
|---|---|---:|---|
| Character::_GetProp | 0x3b9d8c | 440 | aca6d61d2d0a5e330c79a38a01dde437fde4a4a14216edd969a3846f1879b08f |
| CharProperties::_GetProperty | 0x3dedb4 | 292 | c227dbb320f18ab04022f3d61d90e90285652215e56bce8c89c8146aa0fa706f |
| CharProperties::PROPS_GetFromSheet | 0x3b55d4 | 164 | 131099abb0a16a3590cd19fcb923b58dd3b7e220733cf18e46adda6d3125d5ac |

`value-helpers` captures actual Arguments operator[]0x37baf8, getUInteger0x38d798, getPointer0x31b580, getBool0x31bc80 and ReturnValues.pushInteger0x37cb24. `conversions` captures getNumber0x31bbf0 and signed integer Value constructor0x37ca9c. `temp-producers` captures property global initialization0x3df548, ApplyClassToSheet0x3df314, ApplyClass0x3df3b8, GetInt0x3df6e0 and RecalcProperties0x3e0810. RecalcProperty and its actual traversal helpers use existing hash-bound game-data captures.

## Exact callback guards and selection

Arguments are source sfc Value records, with original 112-byte stride. No arguments or first type other than NUMBER3 returns zero values. The first number is read twice through actual getUInteger before the unsigned `>223` guard, and again at the selected getter branch. This executes actual unsigned AEABI conversion: truncation toward zero, negative/NaN to0, positive overflow/infinity to UINT_MAX. Negative property numbers therefore select property0; numbers223.x select223, while224 is rejected. Native explicit conversion avoids undefined C++ out-of-range casts. These getters have no intervening source callback that changes the retained Value record.

With no second argument, read Character+`0xff4` (header followed by current cached payload at+`0xff8`). With second type BOOLEAN1, false reads the same cached sheet and true reads **CharProperties::s_temp**, at`0x9a2c78`, symbol `_ZN14CharProperties6s_tempE`, size900. Every other second type except light userdata2 falls back to cached. Numeric1 and nonempty strings do not mean booleantrue. In particular source table projection type7 falls back to cached even when its `_this` contains a valid sheet identity.

Second type2 selects external-sheet mode. Actual getPointer returns the stored pointer, tested once for null and read again before PROPS_GetFromSheet. Null returns zero values without calling the sheet helper. Nonnull uses the pointed Structs::CharacterProperties. Native uses an explicit identity resolver returning a validated borrowed payload instead of dereferencing/truncating an arbitrary native64 pointer. The resolver is a projection boundary, not a supplied property getter/result.

The callback tail calls pushInteger with the raw **signed** property word. It does not divide by256 or reinterpret it as an unsigned OID. ReturnValues constructs a NUMBER with signed-int-to-float conversion, so values beyond float's exact-integer range lose precision. The separate `FromFixed` callback used by monster.lua retains its existing actual source conversion behavior. Original ReturnValues STL allocation/append is an explicit numeric-return projection service in the instruction oracle; whole original VM/STL allocation parity is not claimed.

## Word layout and shared temporary producer

Original `_GetProperty` validates signed index0..223, obtains linked `_ZN7Structs19CharacterProperties13m_dataOffsetsE` at`0x999cf0` (896bytes), and loads `sheet + offsets[index] + 4`. All224 actual offsets are verified as4*index. Native PropertySheet16 therefore projects224 int32 payload words, excluding the original vtable/header. The property operation is read-only; original diagnostic mode0 invalid signed indices return-1. Null external PROPS_GetFromSheet likewise returns-1 in nonfatal diagnostic mode. Fatal/debug assertion/log machinery is outside this native boundary; the Lua callback normally guards these inputs earlier.

The true-boolean sheet is **not** CharacterTable defaults, base, gear or a new recalculation. My initial discovery message called it defaults before resolving the GOT symbol; that interpretation was corrected before freezing implementation/evidence. The linked initial s_temp payload is896 zero bytes, verified before oracle fixture setup. Global initialization stores its vtable at0x3df5c4; it does not initialize payload from table defaults. ApplyClass(true) passes this same global sheet as destination to `_LoadClass(...,true)` at0x3df3e4. GetInt(true) also reads it directly, then ASR8. RecalcProperties(bool) updates owner base/cache; it does not make GetProp(true) implicitly recompute. Caller ownership of current temporary contents and complete class-selection/write producers remains explicit. Tests use verified zero startup payload for actual base/default fixtures and a distinct synthetic current temporary payload to prove selection, not runtime class ownership.

## Genuine property and Lua fixtures

Actual CharacterTable bytes `character_properties_pyarray.bin` SHA256 `516ba82f631174d4c0a24708342549b5993c402f68c2c1dabd5ddc9eea138784`, names `ba0d987fe70d900dac7d8eb29dc28d87f48e48a15063b87c38e78bf5a2f83801`, fields `7c955f759840dc7808b17ddb54213ce886abd5cdf6607cacc9048a3626803cc2`. Actual original RecalcProperty executes all224 fields for ten Crypt rows33–42 and KnightPlayerBase263:2,464 raw cache results. Input base is the actual row, saved/gear/resolved start from actual default row, types from actual type row, empty buff map. The host executes genuine existing native `dh2_property_resolve` and compares all2,464 words and complete cache sheets before using them. Equipment/buff/class/spawn/HP producers and SetLevel recomputation are not reconstructed by this binding.

The original plaintext scripts retain `.luac` extension: exact cache ZIP members `com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/{_commons,monster}.luac`. ZIP SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. Host uses genuine native Lua, source file loading, existing alias map and existing fixed-point scalar callbacks. Eleven VM sessions load actual AI commons and monster:ten Crypt property fixtures and one Knight cross-fixture. This is not eleven published live NPCs.

Monster top-level reads SkillTree field28. Aliased monster_OnInit reads LevelMax20, LevelMin21, LevelOffset22. All44 reads use this genuine GetProp and exact original-derived caches. Cached number conversions and Init's dynamic-level selection are verified. Table field/OID lookup adapters use actual native decoded CharacterTable/ClassTable names, but remain caller service fixtures while the parent separately recovers the exact GetPyStruct/GetPyOID callbacks. Position, host-player level/difficulty/current-level-range and SetLevel delivery are explicit input/recording fixtures. Their original Character/world bodies and live Lua owner/lifecycle integration are not claimed. Additional real Lua calls check cached/temporary/external/table `_this`, first-string rejection,224 rejection, nil-second fallback, negative/NaN property0.

## Native ABI, proof and integration

PropertyBindings48 owns no sheets: `resolved`, `temporary`, context and sheet identity resolver remain live through all VM calls/finalizers. PropertySheet16 needs readable aligned224-word backing, count224..65536, reserved0. Native output/count/error storage must be disjoint from all borrowed projections and selected sheets; resolver must preserve receiver/backing and cannot reenter the same VM. Native guard failure is a protected provider error, not an original crash/assertion policy. No object/handle resolution or sheet publication/destruction is owned here.

`reports/character-property-bindings-arm64-differential.json`:25,200 original-versus-O2ARM64 callbacks,432 direct sheet/helper cases,21,091 ordered sheet getters, zero mismatches. All224 fields across actual and distinct synthetic sheets, no/false/true/type2/type7/other second types, invalid first types, extra arguments, signed/float boundaries/subnormal/NaN/infinity and randomized raw numbers. Direct cases include signed index extremes and null PROPS_GetFromSheet.

`reports/character-property-bindings-host-audit.json`: same gold under ASan/UBSan;2,464 genuine cache words,11 actual-script sessions,165 Lua property checks,44 monster initialization reads,18 malformed/provider/unaligned/alias guards; zero findings/mismatches. Gold CPB1 `property-bindings-fixtures.bin` SHA256 `acb997f8cda65d6bc2547de544bf73d68e37c0c0792085f4574ac3d31748d78a`. Production CPP `303366c2d540cc3251705a0cc26145c8ff863806210dbdf20226ce79982aa6ef`, header `eb38bfc456cb80f567156477e12863671c969a247557f28a0fafd462cf6ba215`. Reports bind exact original/ARM64/host/script/data/dependency bytes and before/after source stability. Existing DSO dependencies are byte-bound; their compiler provenance is supplied separately by the parent, not inferred from the current source hashes.

From repo root with established Python/Unicorn environment:

```powershell
& port/level-world/tools/build_character_property_bindings_oracle.ps1
python port/level-world/tests/character_property_bindings_differential.py
python port/level-world/tests/character_property_bindings_host.py
```

Parent integration: add `character_property_bindings.cpp` to world, link its bind import to script-runtime. Optional host target `character_property_bindings_audit` uses `tests/character_property_bindings.cpp`, linked world+game-data+script-runtime+dl, include level-world. Its four arguments are CPB1 gold, extracted exact AIcommons, exact monster.luac, data asset directory. The dedicated binder supplies these and records dladdr plus DSO hashes:

```powershell
python port/level-world/tests/character_property_bindings_host.py --main-linked /home/adampalace/dh2-world-build/character_property_bindings_audit
```

This saves a separate main-linked report without asserting central compiler provenance. No APK/ADB, packaged instruction proof, live monster ownership or full gameplay namespace/AI parity is part of this isolated handoff.
