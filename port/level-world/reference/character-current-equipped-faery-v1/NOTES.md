# Current equipped faery callbacks

The original ELF callbacks are `_GetCurrentEquippedFaeryId` at `0x3b6da0` (36 bytes) and `_GetCurrentEquippedFaeryLevel` at `0x3b6df8` (56 bytes). The checked ARM runner executes each full callback. It intercepts only the saved getter(s) and `ReturnValues::pushInteger` to compare call order, signed integer delivery, and ignored `Arguments`.

The ID callback reads `SG_GetCurrentFaerieId(-1)` once. The level callback reads that same ID once, passes the result to `SG_GetFaerieLevel(id,-1)`, then returns the level. Neither calls `GetCharFaery` or validates an equipped row. The level method is intentionally not implemented by calling the larger four-step CurrentSpell query, which has different source behavior.

Both adapters borrow `character_current_spell_v1::SavedBindings` through the existing `saved_services()` implementation and therefore retain one `PlayerSavegameV1`. Its source null-save behavior is ID `0` and level `-1`, with no difficulty read. A nonnull save still requires the matching Character identity, a supported difficulty and initialized level backing for the level call. No faery grant, default row, profile load, or additional owner is invented here.

Host checks import the callbacks from the actual project-selected `dh2_level_world` library. The generated wrapper has no source injection or separate implementation copy. Its CMake and actual Ninja dependency guards are included in `validation.json`; unselected drafts are excluded. Native callbacks reuse the same saved-service bindings; Android compilation and live gameplay require their separate receipts.

Run `tests/run_character_current_equipped_faery_v1_host.py --compiler <g++> --original-elf <libDungeonHunter2.so> --output <ignored-build-directory>`. The focused host and original ARM gates report separately; this unit alone does not indicate native wiring or faery gameplay integration.
