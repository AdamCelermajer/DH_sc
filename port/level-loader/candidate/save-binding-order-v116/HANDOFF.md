# Reached LevelEC save binding order repair V116

Exact reached failure: Required SAME typed LevelEC save runtime from actual C1 bindings, root .local-inputs/act1-final-startup.log.

The actual Level/save C1 is complete. NativeRootGSBootstrapV55::begin constructs GS, checks its completed Level child, then invokes connect_candidate. That callback composes Stage33/34. The World native_level_c1_v25 alias is assigned only AFTER begin succeeds and adopt_into returns (source_campaign_runtime_v61.cpp current905..916). The prior adapter borrowed this empty late alias despite a positive genuine LevelEC save, so startup failed before loading could begin.

The single-file patch borrows the existing retained bootstrap GS/LevelConnection/Bindings at this actual callback point. It requires same campaign World, same GS process globals, same published Level34/global Level, same genuinely complete C1 child, actual bindings, same positive save_ec/shared-control-block, ready constructed save owner and matching Level identity. A preexisting positive World alias must match that child. An empty late adapter slot is permitted only with all genuine completed ownership receipts above; it is never treated as a missing save success. Genuine no-save C1 branches remain both save_ec and typed save absent.

The two later services still retain weak_ptr loans to exactly the original typed save. The World slot publication timing remains unchanged; no early publication, new Save/cache, C1 replay, null-to-ready flag, scope rollback, callback execution or new strong owner is introduced.

Apply save-binding-order-v116.patch at shared repository root. Only renderer_source_stage34_v80.inc changes; no aggregate runtime/native_app/model_renderer file changes and no CMake entry. Root owns rebuild and fresh startup test. No independent tests, builds, syntax checks or emulator actions were run. Full SWAMP rendering/gameplay remains unaccepted.
