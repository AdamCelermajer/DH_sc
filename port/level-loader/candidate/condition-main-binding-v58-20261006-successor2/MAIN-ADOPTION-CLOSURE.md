# Minimal adoption closure (reviewed source only; not compiled)

Exact selected baseline: 772 files, manifest SHA2565455733504332842ad97c98c5862a00b8cc7292bd463758e5c3b4195168e0c81. Successor1 baseline matches this manifest. Actor hpp/cpp changes are purely additive against the selected actor and introduce zero member storage. actor-review.diff has real unified hunk coordinates for that selected baseline. It must not be applied to shared Main's older/diverged actor without porting the same existing cells first.

Incremental four production files against selected772:
1. retained_character_actor_v1.hpp: include CharacterEnableConditionFieldsV58 and declare enable_condition_fields_v58.
2. retained_character_actor_v1.cpp: fresh_canonical only source_bools_[0x8a]=1; define the checked view method. It returns existing member/map cells and never inserts cells while borrowing.
3. character_enable_condition_fields_v58.hpp: typed pinned cell view; borrow() returns ObjectEnableConditionBorrowV2. Header only; no symbols/TU.
4. character_condition_binding_v58.hpp: inline bind_character_conditions_v58(actor,actual,lists,services,error) and character_parent_init_post_with_conditions_v58(record,request,response,lists,services,error). Header only; no symbols/TU.

Main production uses the one-call wrapper in remaining_init's reached38be5c branch. It derives actual fields from record.actor, verifies same identity/receiver and condition cell pointers, captures the checked view/list services, then invokes existing character_parent_init_post_v51 on that SAME record. Only that V51 route's complete ordered-qualified guard permits execution. Installing the callback performs no C1/Init/Assign. Invalid binder input leaves services.condition_init unchanged. Wrapper services is by value, so invalid/unreached calls do not mutate the caller's service configuration.

Direct prerequisite headers from the coherent selected parent graph:
- character_parent_init_fields_v48.hpp
- game_object_initialization_fields_v51.hpp
- game_object_initialization_kernel_v51.hpp
- game_object_initialization_services_v51.hpp
- condition_data_kernel_v51.hpp
- condition_data_services_v51.hpp
- character_parent_init_post_v51.hpp
- canonical_character_family_v4.hpp (existing SAME actor/record owner, not a new allocation)
- object_enable_condition_v2.hpp (Main root already has ABI-compatible pointer fields)

The root/selected canonical_gameobject_base_owner_v1.hpp hashes are identical. Main's root initialization API has the same condition_init ABI but defines GameObjectInitializationServicesV1 inside game_object_initialization_owner_v1.hpp. To adopt the V51 kernel, move that definition into game_object_initialization_services_v51.hpp and include it from the existing owner header; do not retain a second definition. Likewise root condition_data_init_v3.hpp embeds ConditionDataRowV3/ConditionDataInitServicesV3; move those definitions into condition_data_services_v51.hpp and include that one definition from the existing V3 header. Root owner cpp kernels may remain until their separately reviewed V51 delegation adoption; no additional cpp is needed for these new V51 template headers.

Prior V55 condition owner closure (already present/compiled in selected772; absent in shared root at review): condition_list_owner_v55.hpp/.cpp and canonical_condition_receivers_v55.hpp/.cpp. ConditionTablesV55::load and ConditionListOwnerV55::{services,construct,assign,destroy} live in condition_list_owner_v55.cpp. CanonicalConditionFactoryV55::{factories,borrow,evaluate} and seven genuine C1/D0 source profiles live in canonical_condition_receivers_v55.cpp. Reuse the existing original Conditions snapshot and actual World authority; no second global or quest state. World/receiver and Eval owner references are weak; external service callbacks must preserve that intended ownership.

Minimal CMake delta after those prerequisites exist:
- retained_character_actor_v1.cpp already belongs to Main dh2_level_world (port/level-world/CMakeLists.txt line225); keep exactly one copy there. New V58 headers require no target_sources entry.
- If V55 is not already compiled for Main, add ONLY condition_list_owner_v55.cpp and canonical_condition_receivers_v55.cpp to that same owning target, once. Alternatively link the existing single condition-owner target; never compile/link both. Selected standalone target dh2_loader_conditions_v57 currently owns those two cpp files (tests/cmake-receiver-transport/CMakeLists.txt lines588-591).
- Preserve existing object_enable_condition_v2.cpp in dh2_level_world (line113); do not add another evaluator or enable implementation.
- C++17 already required by existing owners; include the ONE coherent level-world tree first. No new library/world/base/save target is introduced.

This is an incremental closure against selected772, NOT a claim that shared root needs only four files. Shared RetainedCharacterActorV1 lacks V48/V51 cells/methods and selected earlier V38/41/42 graph support while containing Main additions (including kill fields) that the selected snapshot lacks. Main must port the existing source cells/authority into its current actor while preserving those additions, and compose the existing canonical family/ordered initialization dependencies. Do not overwrite Main's actor with the full selected candidate files. Those full files provide a review baseline only.

Test text reviewed: ScriptCharacterObject uses its existing five-argument constructor (const identity/name), native PropertyMap defaults produce is_demo_only f1=0, selected CanonicalConditionFactoryV55 accepts the existing World pin, Conditions data arguments preserve argument8=stub pointer/argument4=count, and field mutations alias the same actor cells. Test also covers foreign alias/identity/compiled-cell rejection and unreached parent wrapper rejection. Execution pending RAM hold; no compile/run PASS is asserted. Existing V57 ordered host NPC test is separate prior evidence.