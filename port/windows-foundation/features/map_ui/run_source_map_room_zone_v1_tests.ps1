$ErrorActionPreference = 'Stop'
$repository = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../../..'))
$compiler = Join-Path $repository '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$build = Join-Path $repository '.local-inputs/map-room-zone-provider-test'
New-Item -ItemType Directory -Force -Path $build | Out-Null

$sources = @(
    'port/windows-foundation/features/map_ui/source_map_room_zone_v1_tests.cpp',
    'port/windows-foundation/features/map_ui/source_map_room_zone_v1.cpp',
    'port/level-world/canonical_room_zone_v3.cpp',
    'port/level-world/game_object_set_position_v2.cpp',
    'port/level-world/game_object_relative_box_v3.cpp'
) | ForEach-Object { Join-Path $repository $_ }
$libraries = @(
    'libfoundation_data.a',
    'libcontent_xml.a',
    'libdh2_freetype237.a',
    'librecovered_trigger_contacts.a',
    'librecovered_content.a',
    'physics-backend/libdh2_box2d_201.a'
) | ForEach-Object { Join-Path $repository ('.local-inputs/windows-foundation-build/' + $_) }
$exe = Join-Path $build 'source_map_room_zone_v1_tests.exe'

& $compiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @sources @libraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 `
    -luuid -lcomdlg32 -ladvapi32 -o $exe
if ($LASTEXITCODE -ne 0) { throw 'Source Map RoomZone composition test compile failed' }
& $exe
if ($LASTEXITCODE -ne 0) { throw 'Source Map RoomZone composition test failed' }
