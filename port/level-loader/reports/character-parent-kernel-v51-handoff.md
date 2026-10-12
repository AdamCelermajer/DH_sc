# Shared qualified GameObject initialization V51

This proposal extracts the existing control into one borrowed-storage kernel. Generic GameObjectInitializationOwnerV1 and ConditionDataInitV3 keep their public interfaces and become wrappers. The Character bridge uses the same actor's V48 fields and accepts only its current reached38be5c NPC request. No second CanonicalBase, pose, properties, visual, save, condition backend or stage cache is allocated.

| Subtask | Status |
|---|---|
| One shared original GameObject control and generic wrappers | Native verified |
| One shared original ConditionData control and generic wrappers | Native verified |
| Actual Character field adapter and reached-request guard | Native verified |
| Character signed16 sound adapter with upper-halfword guard | Native verified |
| Original cached/controlled NULLvisual/required producers control cases | Native verified against coherent751 |
| Existing ordered actual NPC/model prefix through qualified parent | Native verified against coherent751 |
| Whole Character/level activation | Still requires main-owned reached services |

Adopt the four-file proposed patch for game_object_initialization_owner_v1.hpp/.cpp and condition_data_init_v3.hpp/.cpp. Add game_object_initialization_services_v51.hpp, game_object_initialization_fields_v51.hpp, game_object_initialization_kernel_v51.hpp, condition_data_services_v51.hpp, condition_data_kernel_v51.hpp and character_parent_init_post_v51.hpp. The optional stateless source_device_pipeline_v51.hpp exposes the exact original three-byte/renderer-mask branch with no device storage. Parent owns CMake and source integration.

At remaining_init request38be5c, compose GameObjectInitializationServicesV1 over the actual record. condition_init must call the shared ConditionData kernel on the actual typed view and real main Conditions snapshot/ConditionList services. check_spawn_probability must reuse actual actor270 and the same existing application/IsPlayer provider; after the first NPC check it preserves the same cache. set_position uses the existing whole V7 kernel over actual pose/pointer cells and publishes the actual position into its existing runtime semantic view. Reached Device/LoadVisual/Sync callbacks use genuine platform and RetainedCharacterFamilyVisualV6 services. NonNULL compiled conditions require actual ConditionList D1/CustomFree lifetime cleanup.

The Character adapter writes the actual int16 idle-sound cell. The generic adapter preserves the existing canonical owner's signed16 logical projection in its already-owned int32 map cell; it adds no shadow. InitFinal stays available for the existing generic owner. The Character InitPost view omits its later InitFinal-only fields and the bridge does not invoke InitFinal.

Native tests compare original cached-rejection and controlled omitted-NULLvisual branches using actual condition/cache/SetPosition/device kernels, with no successful visual callbacks. Optimization/device inputs for omission remain explicit source-control fixtures and do not prove first-SWAMP activation. Normal constructor optimization0 fails at actual LoadVisual; absent actual renderer mask fails at Device. The real factory/model test follows existing LoadBase/Recalc/model/scale order, then enters the shared parent conditions/cache/pose prefix and stops honestly at required Device. Whole Character completion remains false.

Native result: shared generic original-control cases4 and actual required producer stops2 passed; Character signed16 upper-halfword guard passed. Actual ordered NPC/Faery models enter shared parent conditions/cache/SetPosition and stop at required Device, retaining expected source prefix and RNG count8. Old generic initialization/condition archive objects are excluded. Whole Character/level activation remains false.
