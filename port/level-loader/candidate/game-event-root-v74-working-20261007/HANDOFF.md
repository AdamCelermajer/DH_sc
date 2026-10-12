# Native Level194 owner for Root conditions and first SWAMP

Source-only coherent adoption from existing retained loader implementation.
No build/test/hash/fixture/device jobs; historical V50/V55 receipts are not a
current Root runtime claim. Root stays read-only for this session.

Adopt game_event_manager_v50.hpp/.cpp, game_event_level_fields_v50.hpp,
stage_loader_v50_game_events.hpp and game_event_state_borrow_v74.hpp. Compile
ONLY game_event_manager_v50.cpp once on existing dh2_level_world. Apply the
included small context declaration diff and method inc surgically to CURRENT
Root CanonicalLevelContextV1: one game_events194_ shared manager member and
game_event_fields_v50 method alongside actual raw constructor field194. Do not
replace current context wholesale or drop V65/other ongoing Main changes.

Before genuine stage6, initialize one GameEventTablesV50 from the existing real
cache's data/pydata/v2eventmanager_pyarray.bin then v2eventmanager_pyarraynames.bin.
No fixture data or registered-name table substitutes. If Main already owns this
table, use its compatible typed borrow rather than create a duplicate table.

Bind source stage6_game_events_v50 on SAME LifecycleServicesV36 stage_body[6].
Its original order is Debug trace, actual PlayerManager byte6c9=1, genuine
PlayerManager.Update378fb4, C1 of distinct native manager, publish SAME Level194,
then original storage Load. Main must provide actual PM receiver/byte/update and
table. Dispatcher remains sole130/progress owner. Storage Load uses bounded
scheduling while preserving constructor/publication order; all authored rows
and eleven objective constructor/reset types remain retained. This owner is
distinct from App EventManager14 and original EventManagerOwnerV12.

Lookup correction: static original ELF symbol4796f0 is
GameEventManager::GetEventByID(int), size40, NOT GetEventState. It returns NULL
for negative or index>=signed vector count, otherwise the same event slot.
Existing GameEventManagerV50::by_id preserves exactly that lookup. A caller then
reads SAME returned GameEvent.state0. New borrow_game_event_state_v74 validates
actual194 against Level's genuine shared owner, returns an alias retaining this
manager/event and actual state0 pointer, and preserves NULL lookup outputs.
Main NativeConditionRuntimeV69 event_state callback can consume this helper;
original NULL/assertion branch policy remains Main-owned. No raw pointer cast.

Stage31 Compile/Register/Update, objective/quest effects and original functional
Unload479968 remain Main-required on THESE same rows/objectives/manager. The
storage destructor is not functional Unload. Keep raw194 and owner until real
teardown and native receiver unregistration complete; clear only afterward.

Parent selected private graph already has actual Level194 context ownership.
Main Root must add those exact fields/method coherently and rebuild at the big
integrated milestone. This handoff closes source owner availability and typed
lookup, not actual event execution or complete first-SWAMP acceptance.
