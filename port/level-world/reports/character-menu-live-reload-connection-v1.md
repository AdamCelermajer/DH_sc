# Outer original NativeReloadSkills connection

`CharacterMenuLiveReloadConnectionV1` composes the original coordinator with
the existing retained player, Save, Gear and PropertyView. Constructor checks
same Save identity, same shared PropertyState and its saved/resolved buffers;
Gear and V6 legitimately have distinct view objects over that same storage.

Use `services()` as CharacterMenuReloadActionGraphV1.reload. `reload(result)`
also runs the full original coordinator directly, exposing the reached phase.
Its service bridge performs actual BuffOwner remove-all, same LoadOwner mask
0x20, V6 inner native reload through SkillSaveLoadConnectionV1, V6 update,
and fresh Save level/class reads. It retains required recalc, requirement-check,
current MenuFX and source Invoke endpoints through a caller lifetime lease.
Missing endpoints reject; no UI callback or equipment mutation is a no-op.

Source3a9d10 is precisely Character::INV_CheckItemsRequirements, not a general
inventory refresh: iterate all9 source slots, fresh requirement query, unequip
each rejected slot with equipment=-1. Only if changed: UpdateGearsProperties,
recursive requirement check, UpdateSkin, ValidateHPMP. Existing Gear Impl::prune
matches this. Expose a guarded public call to that method; refresh_effects()
adds unconditional effects and must not replace the source endpoint.

Source3e0810 RecalcProperties(true) loads the class into the actual base sheet
in uncached mode0, then executes RecalcProperty0..223. The provider must use
the actual live PropertyView, including source Buff groups. The separate
PropertyState convenience recalc does not itself borrow live Buff groups.
`CharacterMenuRecalcOwnerV1` now provides this concrete endpoint: retains
actual GameDesign class rows, same mutable base/saved/gear/resolved storage and
same live view; calls the existing original-proven dh2_class_recalc_base with
that view. Its false branch resolves224 properties without loading a class.
The focused actual-cache test passes O1 ASan/UBSan against current shared world,
data and runtime libraries, with actual KnightPlayerBase/class tables and a
declared live Buff sheet input; it verifies true/false and different-view guards.
Source3e0af8 remove-all stops actual TimerStore timers, releases sheets/FX,
clears declarations, then RecalcProperties(true), already implemented by the
retained BuffOwner. Its reached recalc provider must also remain genuine.

The original coordinator's final current RenderFX→Invoke uses exactly
`_root.menu_CharacterMenu`, `IsSpecTime`, one source boolean argument. The
caller must enter the correct retained movie Scope and preserve callable=false
as the source Invoke result, rather than require a replacement widget.

New adapter strict Android arm64 syntax passes. Existing whole original outer
coordinator differential remains its ordering proof; no live movie activation
or fully constructed player-composition host run is claimed by this syntax check.
File/class/load limits remain in character-skill-save-load-connection-v1.md.
