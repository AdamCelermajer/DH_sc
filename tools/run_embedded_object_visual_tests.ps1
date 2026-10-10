$ErrorActionPreference = 'Stop'
$taskRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$taskCompiler = Join-Path $taskRoot '.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
$taskBuild = Join-Path $taskRoot '.local-inputs/embedded-object-visual-test'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
Copy-Item -LiteralPath (Join-Path $taskRoot '.local-inputs/publication/checkpoint/reference/openable-container-v1/cache/go_chest_swamp.bdae') -Destination $taskBuild
Copy-Item -LiteralPath (Join-Path $taskRoot 'port/windows-foundation/features/interactions/test-assets/go_swamp_urn_breakable.bdae') -Destination $taskBuild
$taskSources = @('tests/embedded_object_visual_tests.cpp','embedded_scene_clips.cpp','original_character.cpp','retained_animation_owner.cpp') |
    ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskLibraries = @('libfoundation_data.a','libcontent_xml.a','libdh2_freetype237.a','librecovered_trigger_contacts.a','librecovered_content.a','physics-backend/libdh2_box2d_201.a') |
    ForEach-Object { Join-Path $taskRoot ('.local-inputs/windows-foundation-build/' + $_) }
$taskExe = Join-Path $taskBuild 'embedded_object_visual_tests.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskExe
if ($LASTEXITCODE -ne 0) { throw 'Embedded object visual compile failed' }
& $taskExe $taskBuild
if ($LASTEXITCODE -ne 0) { throw 'Embedded object visual test failed' }
$taskActorSources = @('tests/retained_animation_owner_tests.cpp','embedded_scene_clips.cpp','original_character.cpp','retained_animation_owner.cpp') |
    ForEach-Object { Join-Path $taskRoot ('port/windows-foundation/' + $_) }
$taskActorExe = Join-Path $taskBuild 'retained_actor_regression.exe'
& $taskCompiler -std=c++17 -O2 -DNDEBUG -Wall -Wextra -Werror -Wno-missing-field-initializers -static `
    @taskActorSources @taskLibraries -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -o $taskActorExe
if ($LASTEXITCODE -ne 0) { throw 'Retained actor regression compile failed' }
& $taskActorExe (Join-Path $taskRoot '.local-inputs/windows-shared-assets')
if ($LASTEXITCODE -ne 0) { throw 'Retained actor regression failed' }
