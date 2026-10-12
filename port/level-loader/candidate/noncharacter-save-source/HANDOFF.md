# Actual non-Character OBJS source connection and V86 enrollment

SOURCE ONLY. Shared Root untouched. No tests/build/compiler/runtime/hash/freeze/proof or fixtures. Header/paired source successors are adopted private b/selected owner; renderer bridge is a standalone disjoint .inc for Parent/Main assembly. Existing SavegameStreamV2 / object_save_restore_v3 / level_savegame_objects_v2 remain sole native kernels/owners; no new save state, actor map/registry, pools or World.

Include outside model_renderer namespace: source_campaign_save_objects_v86.hpp, source_campaign_fx_v77.hpp, noncharacter_save_connection_v89.hpp and source_release_journal_v69.hpp. Include renderer_campaign_noncharacter_save_v89.inc INSIDE namespace after actual V69 composition source. After compose_campaign_noncharacter_v69 returns and after attach_catalog_owners hook has run, BEFORE any XML call:

    bind_campaign_noncharacter_saved_objects_v89(actual, composition,
        release_journal_sibling, genuine_optional_restore_lender, error);

This attaches the exact public bind_source_campaign_noncharacter_saved_objects_v86(actual.actual_world,journal,weak_same_record_delegate,error). SourceSave86 retains the actual journal as independent class owner; weak delegates lock SAME World/V69/journal only per delivery. No second save directory or factory bind. Actual metadata/eligibility is lent first; methods defer actual typed fields until source virtual Save/Load. Checkpoint28 and original LevelSavegame __SaveObjects/__LoadObjects ordering remain authoritative; no scenery/zone/unknown class is skipped by this adapter.

REQUIRED context fix in Main V86 object(): preserve a returned out.receiver_lease_v86; use object->lease only if absent. The supplied lease owns a per-call dispatch with SAME actual receiver pin, protecting raw out.context. Existing unconditional overwrite would drop dispatch and dangle context. Exact hunk v86-delegate-context-lease.patch; no Root edit performed. Cached payload callbacks have their own independent gameobject_context_owner holding weak V69+actual identity, so later saved groups never retain a dead prior dispatch pointer. Current Main V86 scoped receiver lifetime header pair was copied into selected owner. Manager source_ordered_first_v38 aliases SAME source_ordered_begin to close new V86 public spelling; no iteration/map duplicate.

Core API: journal.save_header(id,out,error) lends actual eligibility/name/room/disabled fields; journal.save_connection(id,genuine NonCharacterRestoreServicesV89,connection,error) chooses typed actual C1 record; connection.bind_methods(LevelSaveObjectBorrowV2&,error) preserves original type queries/context, binds SAME Save/Load and pins identity only. Typed pre-C1 auxiliary record closures choose exact receiver projection at delivery (no erased-type cast). make_noncharacter_record_save_v89 handles actual completed Auxiliary/Light records; make_noncharacter_receiver_save_v89 handles SAME post-C1 container aliases; make_noncharacter_base_save_v89 consumes existing canonical base borrower for proven GameObject inherited cases. Journal keeps connection in the existing admission entry, not a new save lookup registry. Failed mutation prefixes latch and cannot replay.

Source primary vtables (original address table metadata) prove these payloads:

- Dummy, SpawnPoint, Decor, AnimatedDecor, CheckpointZone, QuestMoveInZone, SoundEmitter, RoomZone, ColBox, Floor, TriggerTrap: GameObject Serialize38b9e4 / Deserialize38b91c (6 bytes: ObjectBase visible80/enabled8a then archetype270).
- TriggerZone and TriggerZoneExitLevel: Trigger398978 /39890c (14 bytes, adding timer3b8 then activated3b4). Deserialize reads fresh current Levelf3 AFTER base; when f3 && reset3b0 returns without reading tail. TriggerObject Serialize3996a4 is actual tail branch to Trigger serializer; Deserialize399580 continues genuine TestInteractiveCond and actual animations after parent skip branch too.
- Door3e7b74/3e7a6c: GameObject6 + state3a8 word (10). Restored states1/3 call genuine Opened(false), others genuine Closed(false).
- Openable/Destructible primary slots both Container39f8e0/39f6f0: GameObject6 + LOW BYTE of state394 (7). Container reads current Level then reset390 BEFORE any stream; resetfalse and f3/f4 returns without base/tail reads. Otherwise base then state byte; visualNULL leaves state unchanged. Positive actual visual requires true SetState; state2 idle, state4 true derived KeepPhysics, reached SetPhysicalObject(NULL,false) and idleactive. Cached visual read remains before KeepPhysics leaf.
- LightPoint primary slots ObjectBase33e1f0/33e0f8 (2). New source_save_byte_v89 exposes existing map cells only, inserts no missing default. Actual Light80/ac/d0 producers are absent in current owner; required when corresponding Save/Load touches those cells. Actual checkpoint28 filter may avoid a call; metadata binding itself does not demand unreached fields.

Existing ObjectBase kernel gains one HOST observation pointer visible_produced at end of its borrow. No extra source field/wire bytes: Deserialize marks SAME lifecycle.visible_written immediately after actual stream write80; Serialize rejects reading unproduced80. This prevents V69 SyncVisibility from rejecting its genuine restored80 producer, and preserves partial read prefix if enabled8a later fails. Existing Character borrow aggregates omit the optional observer and remain unchanged.

Restore services already lent by bridge: actual V69 source_visibility for positive SAME Visual2d8, current actual Level f3/f4, actual retained visual.play for recovered Animator38 args (0/1,0,0). Native source strings idlelocked/idle/idleactive/activate are literal-recovered. Optional genuine Main lender retains its own independent owner; it supplies actual assertion mode, Door transitions, Container SetState/KeepPhysics/physical detach and TriggerObject TestInteractiveCond/CanActivate when reached. Missing leaves fail honestly, never return permissive success. Header contains source ordering only; no new animation/condition/quest/gameplay body or clock.

Additional Root-owned boundary remains explicit: Module vtable Serialize388710 is GameObject6 + visited3fc bool (7), Deserialize388f00 also resolves actual handle400/type11 then RoomZone.SetVisited3964bc. Current CanonicalModuleV1 has private zone_ but no3fc producer/accessor; bridge refuses generic6 payload for Module/Block at reached Save/Load. LevelConfig vtable is ObjectBase2, and its distinct owner/base-byte projection is required. Root original Module/Block/LevelConfig metadata must lend same owner headers; no invented checkpoint flag or skipping. Other unknown nonjournal types similarly require original serializer at reached body. Save worker should provide those real Root-owned serializers/owner fields.

Source dependencies: existing object_save_restore_v3.cpp sole TU plus same header; current level_savegame_objects_v2 pair; canonical LightPoint pair actual-cell accessor; loader journal/canonical constructor-state/factory/Auxiliary record successors from SAME journal source directory. No new CPP/link target is needed for inline save helper/bridge. Source closure diff noncharacter-save-source.patch plus the V86 context hunk. All linking/runtime/disk/parity claims remain pending BIG integrated menu -> selected saved Character -> native SWAMP -> movement/camera/HUD/profile-menu milestone only.


Parent V91 adds the distinct LevelConfig header/ObjectBase2 path in
levelconfig-objectbase-projection-v91.patch. It uses the already published
checked Stage34.config borrower and SAME actual manager/config lease/context,
not a second canonical GameObject. Header28/48/30/64/81 is lent before filtering;
original LevelConfig checkpoint28=0 naturally avoids virtual payload. If a
payload is reached it invokes ObjectBase2 only through existing actual80/8a
cells; missing unproduced80 remains required, never manufactured by a default.
The mutable getter inserts no map entry, and no Scene/GameObject side effect
is substituted for the native ObjectBase virtual. No tests or runtime proof.


Paired levelconfig-condition-c1-v91.patch supplies actual inherited nested
ConditionDataC1 tested flagsac/d0 and lends both to ObjectBase Deserialize.
Source ObjectBaseC2 calls ConditionDataC1 at33f424/33f42c; that C1 stores compiled
+1c=NULL and tested+20=0 at33edc4/33edc8, as already reconstructed by the existing
canonical base owner. These are genuine constructor stores on SAME Config,
not stream defaults or an invented flag-skipping path. Byte80 remains unproduced
until a real producer writes it; the mutable getter never inserts missing cells.
