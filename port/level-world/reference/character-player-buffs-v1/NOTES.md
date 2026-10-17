# Player buff owner over the current authorities

This adapts Adam's `character_buffs` and `character_skill_buff_bindings_v3` from
`c3ae797332a82a30a586b9156cddc25445e36a4c`. The same files were inspected at
`791e961b12233100b303038c961666834f4beb9d` and have no changes. Adam's newer V6
player composition still delegates to this V3 buff owner. Its private Session
and timer store are not imported into the current runtime.

`character_player_buffs_v1::Owner` owns one signed-key map of declarations and
one insertion-ordered deque of stable instances per declaration. Each instance
owns its only 224-word buff sheet. Groups are borrowed by the existing
`PropertyView`; they are never merged into a copied PropertyState. `attach`
republishes the groups after an adapter refreshes that view. `owned_sheet`
resolves only current registered instance identities, preserving full-width
native pointers and refusing arbitrary lightuserdata.

`Bindings` retains the captured Character identity, a borrowed PropertyView,
the service descriptor, and actual class/FX catalog counts. `Services::invoke`
returns zero on delivery and receives a live PropertyView plus the typed
`Request` and `Response`. The existing operation enum remains:

| Operation | Required existing provider |
| --- | --- |
| timer_start | Coordinator.start_timer(duration, repeat 0, event 0x36, instance) |
| timer_stop | Coordinator.stop_timer(id), including source id-1 no-op |
| timer_time_left | dh2_character_timer_time_left over Coordinator.timers(); elapsed/duration |
| fx_load | Existing FX creation provider; numeric id bypasses catalog guard |
| fx_release | Existing FX release provider; null reference is source no-op |
| fx_object | Existing animated FX object provider |
| fx_enable | Existing FX indexed enable provider; enabled 1 |
| apply_class | dh2_class_apply(rows,count,id,owned sheet,view.resolved) |
| recalculate | dh2_class_recalc_base(rows,count,mutable view.base,view) |

The original `_ApplyPropClass` at `0x3babdc`, 460 bytes, selects its identity
branch at `0x3bad04..0x3bad60`. That branch calls
`PROPS_ApplyClassToSheet` at `0x3df314`, 164 bytes, which supplies buff=true to
`_LoadClass`; it then calls `RecalcProperties(true)`. The class must read the
live resolved property sheet as its buff source and write the owned instance
sheet. The following full recalculation applies the actual base ClassID and
resolves every property through the same published buff groups. ClassID-1 in a
base sheet is an original no-op class dispatch followed by full resolution.

No provider failure rolls back completed source effects. Add can retain an
empty declaration after FX failure, an uninitialized instance after timer
failure, or all weaker-strength writes made during selection. Remove can retain
an empty declaration after release failure; successful class writes survive a
following recalc failure. Group/sheet view backing is reserved before growth,
then refreshed without allocation on shrink/failure paths.

The Lua `create_buff`, `remove_buff`, and `apply_buff` callbacks bind into the
existing VM through source-value registration. They preserve the source
number-tag guards, unsigned class range comparisons, signed creation ID,
Boolean capacity semantics, optional nil/number fields, and name strings.
ApplyBuff uses unsigned conversion of its ClassID, so a numeric -1 becomes 0
before dispatch. Direct Owner.apply(-1) still reaches the class kernel no-op
and recalculation. Finite ARM signed/unsigned float conversion saturates;
negative unsigned values become 0. String/identity/table number conversions
require unavailable Value services and explicitly fail. An unspecified actual
FX count is represented by UINT32_MAX and fails only a reached nonnumber FX
guard. Reached FX operations require actual backend delivery.

The caller must end timer delivery and close its VM before `retire`, then keep
the borrowed Character/view/services alive through that call. The source
destructor portion frees instance sheets and releases nonnull FX; it adds no
timer stop or recalculation. Owner/service mutation is single-threaded and
forbids synchronous same-owner reentry, destruction, or rebinding. The Owner
does not create a VM, PropertyState, timer store, inventory, Save, or ClassTables.

## Verified source and selected host proof

The pinned ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`tests/character_player_buffs_v1_original.py` runs 40 complete original
ApplyBuff callbacks with actual normal/AW Fire, Water, Wind, Earth and Lightning
resistance classes. Base inputs are ClassID-1, KnightPlayerBase, MagePlayerBase
and RoguePlayerBase. Original Arguments/Value getters, nested class formulas,
uncached base-class reads, all-property resolution, map traversal and deque
helpers execute their actual instructions. There are no intercepted imports
or arithmetic/property substitutions in these 40 cases.

The C++ audit compares the 224-word buff sheet and all four 224-word property
sheets against the original after application and after removal. Removal's ARM
fixture unlinks its caller-owned group and invokes original RecalcProperties;
the original allocator/timer/FX/DelBuff STL bodies are not executed by this
oracle. Native retention, expiry, removal, signed group ordering, view refresh,
provider prefixes, native callback conversions and identity retirement are
verified in 14 focused Coordinator/owner checks.

The unchanged recovered `faerie_celest.luac` then performs 40 source updates in
one retained real Lua VM per base case, using these exact native buff callbacks.
Normal/AW class choice, same-owner retained identity, prior removal and required
FX failure caught by Lua pcall are checked. The prerequisite RegisterSkill/math
helpers and selected-level/element values are fixture providers. This does not
prove the frozen faery/save wrapper providers, the selected Android build, or
actual live gameplay; those are separate current-runtime integration gates.

Run `tests/run_character_player_buffs_v1_host.py` with the compiler, recovered
cache, pinned ELF and a short output directory. By default the runner links the
actual selected `dh2_level_world` target and its existing shared Adam Lua target.
It checks the selected commands for the current buff/Coordinator/class/property
sources, refuses a second buff owner, confirms shared-library imports, verifies
ELF ranges, regenerates the frozen original fixture, and records input and
binary hashes. An explicit `--isolated` option is retained as a diagnostic.

`reports/character-player-buffs-v1-selected-host-audit.json` records the selected
gate: 40 original full-state cases, 14 ownership/provider checks and 40 unchanged
Celest same-VM updates. This target linkage proves current host selection; it
does not execute the Android Runtime wrapper, frozen faery/save callbacks or
actual gameplay. Those remaining boundaries are recorded separately.
