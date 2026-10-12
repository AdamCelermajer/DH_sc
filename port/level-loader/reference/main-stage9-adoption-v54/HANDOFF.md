# Main stage 9 adoption V54

`current-root-stage9-source-v54.patch` adds the frozen V53 eight hpp/cpp files under `port/level-loader`, the10-property schema fragment under `port/level-world`, one class-whitelist/include edit, and four source TUs to **existing** `native_loading_sources_v50.cmake`. That file is already included by `port/level-world/CMakeLists.txt:689`; it deduplicates real source paths into `dh2_level_world`. Native target already links that owner. Do not `add_subdirectory(port/level-loader)` for production: its standalone floor/scene libraries would duplicate the runtime authorities. Do not compile a second PropertyMap/light-set Names/driver implementation.

The patch’s exact preimage hashes are in current-root-source-receipt-v54.json. Main was read only. Final readback found only model_renderer.cpp moved during audit; schema/CMake/device/C1/current canonical owner preimages stayed identical. Patch adds no diff to moving model_renderer.cpp. V53 ZIP SHA remains ccfe3cceee83fd704e81d82987c39e362ec9ce48dd45cc13b6bdb7616b34f580.

The additional typed `main_stage9_lightset_binding_v54.hpp/.cpp.proposed` is the immediate renderer consumer. Copy to current `port/android-native/app/src/main/cpp` and add its cpp to existing dh2_native source list (or include implementation once from model_renderer; choose ONE). The binding passed strict syntax-only `-Wall -Wextra -Werror` against current selected headers/current Main APIs (latest syntax pass during766 rebuild). No selected or old library linked, no Android build/device run. It:

- Verifies preparation’s SAME Level and canonical manager and a sole empty `external.stage_body[9]` slot.
- Defers config borrowing until first state9 call, after actual stage8 publication; looks up the actual Module factory GO4 receiver and identical manager lease/context/key, then borrows its genuine strings. Never writes config38 or casts away const.
- Calls existing `model_renderer::borrow_current_native_level_v27(CanonicalCurrentLevelBorrowV1&,string&)` and matches current GSLevel.s_level.
- Uses existing live `borrow_actual_device_driver_type_v55(int32_t&,string&)`; original Device.IsFixedPipeline381668 is `(type &7)!=0`. Current ActualDeviceProvider validates live EGL/context/display/thread/budget generation and actual GLES2 driver returns8, so the predicate is source-derived. No hardcoded false/device flags.
- Requires its device owner to match the real `dh2::android_resources::budget_lease_v39()` control block. A dummy owner or lost context fails.
- Reuses actual V52 filename resolution + CanonicalCachedFile/manager/class dispatcher/Level160/18c. Assigned stream140 stays an explicit original-stream producer requirement.
- Uses weak callbacks to its owning binding; retained external file journals fail if the binding is destroyed, avoiding raw-this lifetime hazards.

Current root hooks/owners are exact:

| Main owner/API | Actual current place | Binding |
|---|---|---|
| WorldScriptContext.native_level_c1_v25/native_gslevel_v27 | model_renderer.cpp | SAME C1 candidate, never new Level |
| RendererCanonicalWorldV4.manager/properties | renderer_canonical_world_v4.inc:7–13 | Alias this canonical facet; source registry must be the genuine fresh C1 producer before preview/player adoption |
| assign_network / language_added | same include:19–44 | Reuse actual offline/online fact and sole language registry |
| canonical_vec3_origin_v1 | same properties ctor | Genuine ZERO default |
| GSLevel.s_level borrow | model_renderer.cpp exported borrow_current_native_level_v27 | Current Level identity validation |
| actual Device type/current generation | actual_device_android_v54.cpp:34 / ActualDeviceProviderV54 | Existing real renderer-type borrow + original scalar mask |
| mounted cache owner | RendererLevelApplicationV25.files->cache | Existing V52 actual filesystem/mount callback; save files_directory is unrelated |
| actual config factory | CanonicalLevelModuleBindingsV2.level_config(identity) | SAME registered GO4 lease/context/string storage |

**Runtime assembly still open:** current Main imports SourceLoadingV43/nativeGSFrame sources through V50 CMake, but has no SourceLoadingV43 runtime assembly in renderer yet; renderer_native_level_c1_v25.inc explicitly retains C1/GS and logs Level.Init pending. `renderer-stage9-hook-v54.inc.proposed` shows the exact weak World/current manager/Level guard and stage9 attachment at the forthcoming genuine source assembly. `source_modules`, existing actual root-file inputs, same class dispatcher and V52 owner/resolver must be the ones that assembly supplies; they are deliberately not fabricated preview aliases. Existing canonical properties constructor currently passes null LoadTemplate assertion callback; source `_templateName` still requires its authentic assertion9 diagnostic binding.

Real native LightSceneNode/CLight/SceneManager root/automatic list/type, functional Player attachment and full light-set material/filter/driver owners are absent from current Main source examined. V53’s required services remain unbound until Main supplies that one engine owner. This adoption adds the genuine GO19 source receiver; it does not introduce alternate native light engine implementations or claim source registration is GPU lighting.

Read-only `git apply --check` now PASSES against current Main. The first private diff doubled CRLF in context; successor normalizes unified patch to LF (source unchanged). Older V54 ZIP remains immutable and is superseded. Apply the checked source patch against current hashes, compile existing owner + native consumer once, then run real source stage9/10 checks. Full lighting/emulator proof remains a separate Main step.
