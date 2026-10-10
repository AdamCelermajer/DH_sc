$ErrorActionPreference = 'Stop'
$repository = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$compiler = Join-Path $repository '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$build = Join-Path $repository '.local-inputs/map-session-marker-test'
New-Item -ItemType Directory -Force -Path $build | Out-Null

$sources = @(
    'port/windows-foundation/features/map/runtime_source_map_actor_markers_v1_tests.cpp',
    'port/windows-foundation/features/map/runtime_source_map_actor_markers_v1.cpp',
    'port/windows-foundation/features/map/runtime_source_map_menu_provider_v1.cpp',
    'port/windows-foundation/features/map/runtime_source_map_page_v1.cpp',
    'port/windows-foundation/features/map_ui/map_ui.cpp',
    'port/windows-foundation/features/frontend/art/original_art.cpp',
    'port/windows-foundation/combat_session.cpp',
    'port/windows-foundation/world.cpp',
    'port/windows-foundation/playable_actor_world.cpp',
    'port/windows-foundation/original_combat_properties.cpp'
) | ForEach-Object { Join-Path $repository $_ }
$libraries = @(
    'libfoundation_data.a',
    'libcontent_xml.a',
    'libdh2_freetype237.a',
    'librecovered_trigger_contacts.a',
    'librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a'
) | ForEach-Object { Join-Path $repository ('.local-inputs/windows-foundation-build/' + $_) }
$exe = Join-Path $build 'runtime_source_map_actor_markers_v1_tests.exe'

& $compiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @sources @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 `
    -luuid -lcomdlg32 -ladvapi32 -o $exe
if ($LASTEXITCODE -ne 0) { throw 'Runtime source map marker test compile failed' }
& $exe $repository
if ($LASTEXITCODE -ne 0) { throw 'Runtime source map marker test failed' }
