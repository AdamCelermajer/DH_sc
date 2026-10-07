# V49 authentic loot publishers and PODecor phases

Source accepted; device/runtime integration is not yet accepted.

## Implemented

loot_root_publishers_v49 ports the missing player slots only: actual ObjectBase C1 enabled8a=1 and PropertyMap constructor-empty template8 (distinct from archetype48). Its lowercase visible descriptor is produced through the EXISTING CanonicalPropertyMap init_properties/set_property("visible",NULL) body. Existing produced80 is preserved, including restore and configured0. No position, properties, Save, FSM, class selection, InitPost or whole defaults replay.

SetVisible38b0f0 reads actual visual2d8 before writing80=(requested?enabled8a:0), then invokes the real supplied VisualObject.SyncVisibility4713d0 endpoint if nonNULL. Mutation remains when the callback fails. Missing visual slot fails; it is never guessed NULL. Initial PropertyMap visible store requires no visual callback.

LootPhysicalAssociationsV49 is only an exact callback-address index. It borrows the canonical parent retained by the real PODecor C1 and verifies current manager publication/Handle/lease. Constructor publication precedes CreateShape and does not incorrectly require physical2dc assignment yet. Recognized actual owner8=NULL gets a retained NULL-owner branch; unknown addresses and changed nonNULL slots reject. NULL shape userdata takes the original NativeWorld default filtering before custom callbacks.

CanonicalPodDecorBodyV49 reuses actual dh2_decor_body_config and NativeWorld/Box2D operations, separating the body constructor from source SetPhysicalObject assignment. Constructor produces owner8/shape/body and leaves previous2dc unchanged. Assignment executes MP_NoPhysics; true destroys only the new body and preserves old2dc/PF, false releases actual previous physical, stores the new pointer then calls actual UpdatePF. No duplicated body, Assign or PF work. Positive source pin remains an explicit unsupported branch; real Decor calls false.

CanonicalDecorPhysicalConnectionV49 supplies real DecorServicesV15 visual/marker/root/construct_podecor/set_physical leaves from the SAME existing source visual, canonical base and World. Failed candidates remain retained; teardown releases bodies in reverse construction order. Floor-map LoadRoom/ExtendBoundingBox and class animation providers remain their actual existing owners.

renderer_loot_root_providers_v49.inc preserves the frozen V47 body, adding only forwarding of an unknown root address to the actual typed decor-peer index. This supports newly created generic bodies without treating legacy ObjectActor DTOs as canonical GameObjects. V47 remains frozen.

## Approved narrow player integration

Root authorized application of ONLY these player hooks; other global integration remains root owned.

Run .local-inputs/stage_renderer_loot_publishers_v49.py --apply-player-hooks once after confirming current hashes against player-hooks.json. The tool validates both baselines and uses atomic file replacement; existing bytes/newlines stay intact.

It adds metadata sibling fields, includes loot_root_publishers_v49.hpp and renderer_loot_publishers_v49.inc, calls prepare_player_source_fields_v49 immediately after binding the existing player_object, and calls publish_player_source_save_v49 immediately after the already executed Save allocation/set_character(id). It reuses the actual canonical facade, preparing missing source80 before later body/loot callbacks. Character14e8 pins exactly t.save, without another SaveLoad/Profile authority. Restore borrows existing producers.

Add three actual TUs to level-world CMake:
loot_root_publishers_v49.cpp
canonical_podecor_body_v49.cpp
canonical_decor_physical_connection_v49.cpp

Retain one LootPhysicalAssociationsV49 on the same canonical manager as a sibling of actual family graphs. For a real auxiliary Decor/AnimatedDecor record, retain CanonicalDecorPhysicalConnectionV49 with its SAME base, World, weak record lease, actual visual lookup and actual Debug/PF/peer services, then bind its existing DecorServicesV15 before class InitPost. Keep existing LoadRoom/floor and animation receivers. Retain those siblings through source destruction; release actual bodies BEFORE parent receiver/World teardown, then erase the physical address index. The connection never constructs another actor or publishes counts.

For existing OpenableGraph bodies, register constructed_decor on the actual RetainedGameObjectDecorV1 at its constructor/initialize prefix before CreateShape; its new source_owner_base_v49 getter was applied by root and exposes the already held base reference without layout/storage change. The packet does not modify those shared graph allocation methods automatically.

Source mixed peer routing and native contact queries must borrow these recognized physical receivers; do not reinterpret unknown Item/Decor pointers as BodyOwner. Parent pointer alone is not a complete filter snapshot; physical_contact rejects missing snapshots instead of inventing filters.

populate_loot_missing_contracts_v49 fills the existing player's Source14e8 association and borrows actual Gear loot_sources_v8 ItemTextOwner/formatter, with no second text cache. Other V47 service contexts/lifetimes remain retained.

## Real remaining engine/loader boundaries

The root's generic class candidate/CanonicalAuxiliaryFamilies graph is not yet connected to its current legacy Crypt ObjectActor bodies. V49 can compose that authentic family graph; it does not fabricate canonical scenery from DTO positions.

Actual default-static Decor C1 static84=1 reaches RetainedGameObjectVisualV1::initialize line92 "OptimizeStatic scene producer". This was found by a real cached Crypt candle test. No production static flags changed. The positive split-body test uses an EXPLICIT authored static=0 fixture property, reported as such, to exercise the supported dynamic visual branch. Default-static full Act1 requires real OptimizeStatic integration, potentially using the existing ModuleStaticSceneV2 implementation; it is not solved by these publishers.

Loader owns real GS/Level/current publication and source stage29 Item145. No phase/count, retrospective InitFinal or unpublished Crypt C1 substitute was introduced. Positive loot/XP/quest/death runtime acceptance remains pending those real owners and root integration.

## Evidence

compile.json: strict three new TUs plus full combined regression test, both arm64-v8a and x86_64.
renderer-compile.json: exact current root renderer with staged narrow player hooks and V44/V49/UseOOI adapters, both ABIs.
host.json: bounded ASan/UBSan current-source tests, original145 cache/Item BDAE graph plus original Crypt candle BDAE, actual Scene registry and Box2D. Final check count is recorded in the receipt. Actual NoPhysics old-pointer/PF-order, SetVisible failure prefix, same Save, canonical owner8, auxiliary construct/assign, NULL-owner and release branches tested. External Debug/network/state facts and authored static0 are declared fixtures. Older immutable host support DSOs are declared; no current APK or live runtime claim.

Shared mutation by this agent is restricted to the approved player hooks after freeze; root applied the separate typed getter. No CMake, device, ADB, emulator, global loader state or private loader files changed here.

