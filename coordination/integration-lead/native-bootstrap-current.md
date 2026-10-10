# Current native bootstrap

Accepted v18 is preserved: SHA `79e566435081650206a03ecb028736a97770fa28d38722438a1a90645e7819d8`, 572 manifest files, 75 CTests, 29 positive packaged cases and two negative saves. This accepts the verified Stats/combat-text subset. Full Equipment, Skills, Faery, Quest and Map pages remain required.

Root development now has 78 passing foundation tests. Both real Character and Player catalog aliases traverse native factory C1, manager Add, PropertyMap Load, embedded ConditionData, application RNG and the same-current-WGL Renderer visual quality policy. Existing native family LoadVisual is reached. Whole InitPost/InitFinal and host publication remain incomplete.

The required leaf is the SAME World/SceneManager/PF graph in `RetainedCharacterFamilyVisualV6::bind`. Current native fixture roots are a real registry; `make_shared<int>(1)` is a fixture world lifetime token and cannot establish a live World owner. No successful no-op updatePF is acceptable.

The existing production provider in `renderer_character_campaign_v62.inc` pins `WorldScriptContext`, actual roots, sewn floor collision world and navigation registry. It checks actual-world equality with `record.services.world`, then captures a weak reference to the same record and calls `canonical_character_update_pf_v62(record, floors->collision_world, navigation->registry(), error)`. Floor `UINT32_MAX` may legitimately bypass obstacle registration; that branch is not a positive PF registration proof.

Root owns navigation/PF, native fixture/build/closure and publication. `source_factory_luna` investigated the actual provider and made no competing PF implementation. Only new `source_character_owner_factory_visual_binding*` files are available to that worker after root agrees on the retained floor authority.

Root accessors now expose Gear's existing prepared `ItemTextOwnerV5` and PlayerSkills' existing constructed native skill owner. Power descriptions belong to `SourceItemResourcesV88::presentation()`, not Gear's private power table. Inventory and Equipment workers require an explicit typed pin to those same resources; no opaque lease downcast or duplicate presentation owner.

The record-based Campaign FSM loan exposes real V1/V3 Session pointers, Session-owned PropertyView and same-record SkillTables. `canonical_session_skill_binding*` consumes these references, requires separate host ActorId-to-native-record refresh, retains native pins and uses Session's existing external sequence API. Initialized native VM/FSM cast acceptance remains pending bootstrap.

Active page work: Equipment native Gear transaction binding, Inventory actual Text/Power/index resolver composition, Skills fresh ABI-compatible native connected probe/mutation fixture, Faery same native queries/actions/Save binding. Skills worker has exclusive WSL `/home/adampalace/dh2-world-build` and `.local-inputs/character-menu-native-v1-host/snapshot` refresh ownership. Root does not use either artifact.

Active Act1 follow-ups: enemy real scoped DoSkill admission; interaction native container link/run and NPC quest-event connection; Quest same EventManager/Objective fixture; Map actual source Show/RenderMap kernel recovery; progression barrier source identity/conditions verification. Other tracks retain concrete completed components and explicit native publication dependencies. No full Act1 completion is claimed.
