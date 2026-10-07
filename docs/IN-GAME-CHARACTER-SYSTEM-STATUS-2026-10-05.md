# In-game character system ownership and integration

The other chat owns the main menu and character selection. This chat owns the
in-game character screens: stats, inventory, equipment, skills and faery
selection, plus their connection to targeting, combat, animation and rendering.

## Completed source work in this milestone

- Full native Save.Load coordinator, including every mask branch, named section
  selection and conditional volatile quest tail. Original/native differential
  proof passes 1,780 cases and 23,017 ordered callbacks; every required rejection
  prefix is checked. Production source compiles for ARM64 Android.
- Full skill reload now positively composes the genuine same-Save Load owner
  with all three populated GearV1/player V6 graphs. Both sanitized and optimized
  proofs pass 1,334 checks. A real nonnull profile still requires its section
  provider, even when the Save's slot is -1.
- Native menu stack ownership/navigation kernels are frozen after 4,800
  original/native cases and 27,099 ordered callbacks, including nested pushes.
- Genuine AS argument transport now separates ordinary `to_string` from the
  original navigation's `to_xstring`. Object/null debug formatting is not
  silently replaced by virtual object string conversion. Deferred getters run
  only when reached and unchanged results remain untouched. Real shared/HUD
  movie tests pass both host modes and ARM64 compilation.
- Native potion creation, splitting, full transfer and cleanup now compose on
  the three actual Gear graphs. The partial transfer keeps source ownership
  guarded while recipient callbacks run.

## Remaining integration

These source proofs are not a completed visible inventory menu. Original
character-menu screen startup, focus, show/hide, input and world pause/HUD
connections must use the actual retained movie graph and native stack.
The native menu Push/Pop entry facade is being completed separately from the
frozen stack. Original source string-backed raw tag3 and 32-bit debug pointer
text are not claimed equivalent to upstream/native 64-bit representations.

Targeting now has a candidate connection to source controller/look-at kernels,
but the renderer does not yet supply the genuine intrusive world registry,
source flags, zones and target-node/cache state required by that connection.
It must borrow the existing actor/property/life graph instead of creating a
second player or fabricated enemy state.

Equipment meshes are present in the accepted equipped-player APK. Full skill
animations, weapon effects and combat effects still need the source event and
FX ownership connections. Full save confirmation requires genuine SG_Save,
profile/file backing and named-section writers; the Load coordinator test does
not prove file IO or save round trips.

## Next checkpoint

The intended checkpoint is a coherent in-game character system on the same
equipped player: open/close the original screens, show real inventory and
stats, equip items and train/select skills, then return to the connected HUD
and player. Build/install/visible emulator validation follows a complete
connected feature. This milestone produced no new APK and did not refresh or
change the emulator.

Evidence:

- `port/game-data/reports/player-save-load-owner-v1-host-audit.json`
- `port/engine-ui/reports/character-menu-as-names-v2-host-audit.json`
- `port/level-world/reports/character-skill-populated-v6-host-audit.json`
- `port/engine-ui/reference/menu-stack-v1/freeze-manifest.json`
