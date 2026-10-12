# Current spell query source handoff

Original callback `Character::_GetCurrentSpellInfo`, address `0x003b6e30`, 80 bytes, ignores Arguments and calls four services in this order:

1. `Character::SG_GetCurrentFaerieId(-1)` (`0x003bb98c`).
2. `Character::GetCharFaery(first_id)` (`0x003aeac0`); its returned row pointer is discarded, but the actual lookup/validation must execute.
3. `Character::SG_GetCurrentFaerieId(-1)` again, reading the live owner/difficulty again.
4. `Character::SG_GetFaerieLevel(second_id,-1)` (`0x003bbc18`).

The final signed integer is delivered to `ReturnValues::pushInteger` (`0x0037cb24`). A null source save pointer yields current ID 0 and level -1; a populated save resolves each `-1` difficulty against the current source global. The query does not accept a difficulty argument from Lua.

`character_current_spell_v1.hpp/.cpp` exports the bounded source coordinator and native callback `dh2::character::skills::current_spell_info_v1`. `CurrentSpellBindingsV1` borrows the same Character identity and an actual `CurrentSpellServices16V1`. Providers must survive the VM and its finalizers. A nonzero service result is a required failure preserving its reached prefix; output is not published until all four requests finish. The validation request's difficulty field is unused; selected/level services receive -1. The callback still queries the source before a missing return-storage failure, and always produces one numeric result on success, including zero Arguments.

The gameplay owner must implement providers over the same pinned Faery tables, PropertyView and PlayerSavegame, and the genuine selected difficulty. It must execute GetCharFaery's source list fallback 0, bounds/constant/type validation, not accept a no-op validation. In the source, GetCharFaeryListId reads raw resolved property 29. Saved Faery levels are unsigned16. Initial save construction/storage and assertions remain caller-owned; this component does not synthesize campaign state.

Verification: 512 executions of the original callback versus optimized ARM64, 2,048 identical ordered service requests. Source helper bodies are captured, but SG/table helpers and integer push are declared service fixtures in this differential. A separate isolated production-DSO ASan/UBSan/LSan replay passes 12,847 checks, ten guards and two actual Lua callback calls. No complete player startup, main World linkage, Android gameplay or campaign claim follows from this leaf proof. Parent owns central selection; V3 gameplay owns the real provider composition.

Select `character_current_spell_v1.cpp` once in World. Do not compile a second production copy into a linked host test. Host target source is `tests/character_current_spell_v1.cpp`, argument `reference/character-current-spell-v1/source-gold.bin`; link World and ScriptRuntime. The isolated host runner intentionally uses its own query DSO until central selection.
