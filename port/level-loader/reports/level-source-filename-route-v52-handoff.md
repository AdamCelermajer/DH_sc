# V52 source stage7 root filename resolver

**Use original bare LevelList.row.file in C1.** SWAMP row41 is `001_swamp.mlx`,
not `worlds/001_swamp.mlx`. All51 original rows contain no slash/prefix. The old
worlds path is a transport fixture accepted by C1 substring row lookup; original
LoadFile produces4 missing queries for it and never removes worlds/. No alias
was added. Actual bare SWAMP reaches `data/scene/001_swamp.mlx` through source search.

New level_root_filename_route_v52.hpp/.cpp implements source3f3b40's ordered
['',data/,data/scene/,data/3d/modules/] loop. Every prefixed raw name queries actual
Application mode. Original320678 checks nonempty actual cc/d0 plus substring
[.mlx,.mgp,.mvp,.xml]. If true, try original uncompiled name first; after miss,
compiled attempt removes FIRST matched [old/,debug/,ps3/,iphone/] token. Existing
frozen compiled_level_paths_v1 is reused; duplicates/mode/open order are preserved.
Outputs stay atomic on failure and scoped resolver rejects runtime reentry.

Concrete current Main proposal is reference/level-source-filename-route-v52/
main_level_root_filesystem_v52.hpp/.cpp.proposed. Its actual openResource boundary
uses recovered SAME resources234 then trace338 fields, source FileHandle joins,
.savegame RES_PATH override, real Application.lowercaseA8, actual shared working
directory/initialized CFS map, and current Main cache read_admitted metadata stop
before payload allocation. It returns the actual mounted canonical cache URI.
Source spellings/canonical identities remain distinct. No ApplyFilenameHacks,
q:/iphone stripping, global basename, worlds or pvr fallback is inserted.

Original openResource routes through FileHandle->CFS.open56dc40, whose inline
ObfuscationMap branch is reached if initialized. This differs from the texture
createAndOpenFile path investigated earlier. Actual original RB-tree find56db48
executed against initialized source-layout fixtures, preserving exact key/value
lookup: leading ./ or .\ removal, working-prefix advancement from START when
strstr matches, slash formatting for lookup; missing map preserves original input;
colon names bypass cwd prefix; otherwise actual cwd/optional slash is prepended.
Fixture map entries do not establish any production alias or pvr selection.

Proofs pass:58 original Level/mode query traces (51rows+3witnesses+4nonempty-range
cases),75 original CFS/RB-tree cases,24 original FileHandle joins; native comparison
and current Main cache-source host probe pass ASAN/UBSAN. Actual supplied ZIP was
read via current descriptor/pread body; all51 authentic row names resolved through
metadata, one mounted fd owner. Android AssetManager/fd/log and renderer/Application
source-field lifetimes are explicit transports/fixtures. Root payload/XML parser,
whole Level initialization, current Android/UI owners and budget/GPU are unproved.
Current cache header/source matches the tested V50 snapshot on final readback.

Main integration: bind SAME actual app cc/d0/lowerA8, actual FS path/global/map
owners and current cache/renderer checks; source-loading-binding-v52.inc.proposed
sets filename_provider and resolve_filename to the retained resolver. Recovered
initial globals/path constructors are source initial states, not absent-provider
defaults. Later native host/mount/map changes must update the SAME owned fields.
Do not use private save files_directory. Parent owns CMake/graph adoption.

Two boundaries stay explicit. Original LoadFile returns1 after all file misses;
this native resolver requires a real resource and fails otherwise as a deliberate
modern missing-resource guard. When actual Level140 already holds generated stream,
original LoadFile bypasses filename opens entirely: use actual retained stream
transport/root parser. Looking up a procedural rule XML here is not a Level root
or a successful generated map. This adapter serves filename-open occurrences.

Source fixtures/scripts/current-cache dependency hashes are frozen in the packet.
V50 dependencies remain frozen separately; no shared/Main/selected source changed,
no device/GPU actions, and the five unresolved alpha texture routes remain open.
