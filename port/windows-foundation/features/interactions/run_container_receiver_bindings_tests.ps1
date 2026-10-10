$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../../..')).Path
Set-Location $repo

$cxx = '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
if (-not (Test-Path $cxx)) { throw "Pinned native compiler is missing: $cxx" }
$sources = @(
    'port/windows-foundation/features/interactions/container_receiver_bindings_tests.cpp',
    'port/level-world/canonical_destructible_container_v16.cpp',
    'port/level-world/destructible_container_data_v16.cpp',
    'port/level-world/canonical_openable_container_v1.cpp',
    'port/level-world/openable_container_owner_v1.cpp',
    'port/level-world/openable_container_property_connection_v1.cpp',
    'port/level-world/game_object_spawn_probability_v1.cpp',
    'port/level-world/container_animation_connection_v21.cpp',
    'port/level-world/retained_generic_animator_v21.cpp',
    'port/level-world/retained_gameobject_visual_v1.cpp',
    'port/level-world/generic_animation_callbacks_v21.cpp',
    'port/level-world/base_index_animation_controller_v21.cpp',
    'port/level-world/native_scene_lights_v113.cpp',
    'port/level-world/module_static_scene_v2.cpp',
    'port/level-world/gameobject_scene_binding_v1.cpp',
    'port/level-world/authored_scene_subtree_v2.cpp',
    'port/level-world/visual_mesh_box_fallback_v2.cpp',
    'port/level-world/visual_aabb_dispatch_scope_v3.cpp',
    'port/level-world/retained_visual_child_v91.cpp',
    'port/level-world/retained_map_mesh_v93.cpp',
    'port/level-world/base_named_animation_controller_v1.cpp',
    'port/level-world/light_set_name_owner_v3.cpp',
    'port/level-world/scene_manager_map_owner_v2.cpp',
    'port/level-world/gameobject_scene_root_registry_v1.cpp',
    'port/level-world/scene_preload_owner_v81.cpp',
    'port/engine-skinning/visual_skin_selection_v6.cpp',
    'port/engine-skinning/visual_skin_owner_v6.cpp',
    'port/engine-skinning/skin_pose_cache_v32.cpp',
    'port/engine-resources/admitted_cpu_bytes_v40.cpp',
    'port/engine-resources/resource_budget_v37.cpp'
)
$includes = @(
    'port/game-data', 'port/level-world', 'port/level-loader',
    'port/engine-animation', 'port/engine-skinning', 'port/scene-materials',
    'port/engine-resources', 'port/engine-textures', 'port/engine-ui',
    'port/script-runtime', 'port/physics-backend/box2d-2.0.1/Include'
) | ForEach-Object { "-I$_" }
$archives = @(
    '.local-inputs/windows-foundation-build/libfoundation_data.a',
    '.local-inputs/windows-foundation-build/librecovered_content.a',
    '.local-inputs/windows-foundation-build/libcontent_xml.a'
)
foreach ($archive in $archives) {
    if (-not (Test-Path $archive)) { throw "Required native foundation archive is missing: $archive" }
}
$output = 'port/windows-foundation/features/interactions/test-output/container_receiver_bindings_tests.exe'
New-Item -ItemType Directory -Force (Split-Path $output) | Out-Null
& $cxx -std=c++17 -O1 -static -ffunction-sections -fdata-sections `
    -Dfinite=_finite -include exception @includes @sources @archives `
    '-Wl,--gc-sections' -o $output
if ($LASTEXITCODE -ne 0) { throw "Native link failed with exit code $LASTEXITCODE" }
& $output
if ($LASTEXITCODE -ne 0) { throw "Native test failed with exit code $LASTEXITCODE" }
