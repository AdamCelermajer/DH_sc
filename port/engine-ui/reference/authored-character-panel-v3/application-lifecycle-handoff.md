# Character panel application lifecycle recovery

The panel runtime is not yet connected to the visible portrait opener. The
existing Android development panel remains distinct. This receipt records
production source boundaries, not live authored-menu acceptance.

`application-producers-original.json` captures complete original functions from
the same ELF SHA recorded in that file. `application-domains-original.json`
captures the recursive character collector, absolute rectangle producer and
touch queue leaves. Addresses are original ARM32 evidence, not runtime calls.

* `MenuManager::LoadMenu` 431ea4 sets the selected slot's RenderFX flags to84 at
  431f10, through literal store helper7a7c98. The HUD slot3 is then set to4 at
  431f28. Character slot1 must use84. Constructor flags are0.
* Multi.LoadSWF437d68 caches the constructed MenuFX before virtual Load at
  437dc8. AuthoredCharacterPanelV2 publishes its actual retained owner before
  loading and now preserves it after a required load failure. It does not
  invent an unpublication/rollback operation absent from the original body.
* RegisterDeadZones42331c sets byteb4 before Debug or character collection;
  failed required services retain that prefix, and subsequent calls return.
* RenderFX.CollectCharacters7a8acc uses `strstr` (PLT30ebd4), not a name prefix
  test. Sprite focus flag2 and visible flag1 prune whole subtrees; flag4 omits
  empty names. Name mismatch skips insertion but still descends. Actual sprite
  child/display-list order is preserved. The new helper takes real character
  identity/name/flags/ordered children through borrowed projections.
* GetAbsoluteBoundingRect416a7c uses receiver virtual bounds plus the ordered
  **parent-chain translations** from4169e8, then divides four edges by20. It
  does not substitute the desktop viewport transform or a transformed AABB.
  `authored_menu_absolute_rectangle_v3` supplies that arithmetic only; the
  production scope adapter must read the actual bound and parent matrices.
* TouchScreenBase ctor33c3d8 creates16 zero queued-event records, head1a0=0,
  tail1a4=0, orientation1a8=0 and scale1b0=1. `_IsQueueEmpty`33a9a4 compares
  those two owned indices. ProcessEvents33c568 returns immediately only for a
  genuinely empty queue; nonempty events require the actual event processing
  body. Application.ResetTouch328f40 snapshots its current-touch map and calls
  the actual touch receiver release endpoint for every entry with(-1,-1).
  The MenuManager's following ProcessEvents call is mandatory. Neither has
  been replaced by an unconditional successful callback.

New TUs `authored_menu_deadzones_v3.cpp` and its test are independent of the
current root build. The isolated APK-linked test passes DFS/substr/filter,
rectangle and sticky failure-prefix cases. It supplies external character
projections/bounds; this is not original ARM differential or live SWF menu
acceptance. See reports/android-native-owner-tests/authored-menu-deadzones-v3.

The same-player runtime/reload bridge already composes actual Save.Load20,
InitSkills configure, source property recalc and Gear requirements checks.
Remaining opener work is genuine retained HUD/base RenderFX roots, application
touch/lifecycle state and scoped native sound/localization/back deliveries,
followed by native_app rendering/input and Java portrait/back transport.
