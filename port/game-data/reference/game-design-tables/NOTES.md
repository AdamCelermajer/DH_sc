# Native registered design-table getters

`game_design_tables.hpp/.cpp` implements the proved name-lookup domain needed
by monster script initialization, using borrowed genuine decoded name tables.
It owns registration keys, not Character/AI/class records or Application tables.
The original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Original registration and getters

The existing immutable `port/script-runtime/reference/design-bindings/registry-probe.json`
executes the actual PyDataArrays constructor registration call sites. It records
142 ordered calls / 138 effective names; later registration assigns the same
map key again. This module selects the actual six registered getter identities
below in their delivered order, without inventing namespace aliases.

| Registered name | Original getter | Actual cache name count | Lookup body |
| --- | --- | ---: | --- |
| AIProps | 0x4af590 | 15 | Fixed field array, std::string stride 24, C string at +20 |
| AITable | 0x4af51c | 76 | Reread runtime count and pointer array, ascending strcmp |
| ClassFuncList | 0x4af1e0 | 1 | strcmp with sole field list_entries, return 0 / -1 |
| ClassTable | 0x4af16c | 260 | Reread runtime count and pointer array, ascending strcmp |
| CharacterProperties | 0x4af110 | 224 | Fixed field array, std::string stride 24, C string at +20 |
| CharacterTable | 0x3fa188 | 448 | Reread runtime count and pointer array, ascending strcmp |

CharacterTable is the registered group for `Arrays::CharacterTable`; there is
no invented `ArrayCharacterTable` alias. AI namespaces are exactly AIProps and
AITable; AIStates comes from the independent constants table. Current Player
is actual AITable row 44, KnightPlayerBase CharacterTable row 263 and SkillTree
CharacterProperties field 28. Monster top-level reads SkillTree, while its Init
reads LevelMax/LevelMin/LevelOffset and ClassTable/Buff_Speed.

GetPyStruct and GetPyOID both call PyDataArrays::GetOID `0x4bd640`. This looks up
the registered group in the source map at manager +0x1c and invokes its getter
pointer from node +0x28. There is no separate struct-versus-array namespace gate.
An absent registration returns signed -1. The registered getter returns the
first exact strcmp match in ascending field/row order, or -1 on a miss. Case is
significant. Both registration/query keys and member C strings stop at the first
NUL. No interned-pointer equality, lowercase normalization or row sorting occurs.

registerClassByName `0x4bdccc` delegates to source map operator[] `0x4bdb8c` and
stores the supplied getter pointer, replacing an earlier assignment. The focused
capture contains these complete bodies, map find `0x4bd4d0`, GetOID, and all six
getters with exact original byte hashes. In the differential oracle the actual
map insertion, RB/string helpers and getter bodies execute from the immutable
ELF. Allocator/libc/thread imports are explicit storage/arithmetic services. A
null getter's source diagnostic/assertion path is not a valid callable provider;
native registration rejects a null descriptor.

## Public native domain and ownership

DesignNames16 borrows a current name pointer array/count. DesignRegistration24
borrows its descriptor and has an owned-key counterpart in GameDesignTables.
DesignRegistry16 exposes the ordered registrations. The C provider is compatible
with persistent script_design_bindings kind 1; kind 0 deliberately returns
delivery failure, because constants belong to another backend.

`dh2_game_design_find` performs the first-match member search.
`dh2_game_design_tables_lookup` uses the latest matching registration and then
the live descriptor. Descriptor indirection is necessary: array getters reread
the original global count and name pointer on each call. It does not capture a
stale count at registration. Fixed field descriptors must retain their original
224 / 15 / 1 dimensions. Names, descriptors and pointer arrays must survive the
VM and be terminated/readable; outputs are disjoint from borrowed string backing.
The native layer rejects malformed alignment/count/alias projections atomically
instead of imitating undefined original memory access. Delivery success is 0;
authored group/member miss is successful delivery of -1. Delivery error is 1.

GameDesignTables copies registration keys with source first-NUL semantics,
retains ordered registrations, and rebuilds the view after registration. The
view remains valid until the next registration/move; names remain borrowed.
Copying the owner is disabled, avoiding stale pointers into another owner's
copied keys. Moving transfers ownership. Allocation failure preserves the
observable registration list. Borrowed row names can be reloaded synchronously
between calls; stale caller pointers are not an owned-loader service.

Only the six listed registered getter domains are proved. The configured registry
is a selected subset, not the original 138-name Application manager. A full
live provider must explicitly route other original registered namespaces to
their own genuine providers or report delivery failure; it must not present
their absence from this subset as an original Application lookup miss. Generic
registration does not prove arbitrary source getter bodies. Application
construction, original name-loader allocation/global lifetime and all other
registered table ownership remain separate work. Post-loader names in the
original oracle are borrowed from the exact cache streams.

## Instruction and sanitizer evidence

`tests/game_design_tables_differential.py` produces GDT1 table-fixtures.bin.
The O2 ARM64 exports match 1,110 actual registered GetOID queries and 1,054
direct original getter calls, with zero mismatches. Eleven original registrations
include the six delivered names plus synthetic duplicate/case/embedded-NUL/high
byte keys. It covers all 1,024 actual field/row names, exact miss cases, first
matching duplicate member names, and live zero-count array reloads. Original
execution invokes the six getters 2,157 times. No supplied lookup result or
map-finder stub substitutes for original GetOID traversal. Native owned-key
registration/allocation is covered by the host audit; the ARM64 audit compares
the explicit ordered registration view and getter result domain.

Final source/ARM64 report: `reports/game-design-tables-arm64-differential.json`.
GDT1 SHA256:
`9d30139f4e441179c49ab31e0fbef16f61ebadd644a77af0520986517d8b3f2a`.

`tests/game_design_tables_host.py` builds only new source/test plus frozen Level
source against the actual main world/GetProp, game-data and runtime DSOs. It
replays the 1,110 original registered queries through the owned C++ registration
builder, observes temporary-key lifetime under sanitizers, verifies borrowed
reloads and 20 total table/Level malformed guards. It validates table names
against actual decoded CharacterTable/ClassTables/AiTables and AI schema data.

The same test binds the actual runtime constants loader to kind 0. Exact cache
ai/design bytes match the original CST1 records and native reload metadata;
there is no supplied cap callback. It replays 870 original cap-100 Level cases
/ 779,520 sheet words and loads actual _commons.luac and monster.luac for ten
Init sessions. GetPyStruct/GetPyOID now use the native registered provider, not
the previous test metadata adapters. It performs 160 actual VM checks, including
cross Struct/OID dispatch and first-NUL keys. ASan/UBSan reports zero findings;
the test records 2,145,945 checks.

DebugSwitch effects, world position/difficulty/range, and host-player receiver
identity are still explicit fixtures/services. The host level itself is queried
from a genuine borrowed Knight property receiver through native GetLevel.
This proof does not establish live host-session ownership, complete monster AI,
whole Application initialization, packaged instruction execution or full-frame/
full-game parity. Compiler input hashes, exact dependency/executable hashes,
cache/script/asset bindings and actual dladdr origins are recorded. Dependency
compiler provenance is not inferred by the isolated runner.

## Reproduction and integration

```powershell
& port/game-data/tools/build_game_design_tables_oracle.ps1
$env:PYTHONDONTWRITEBYTECODE='1'
$env:PYTHONPATH='C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages'
& 'C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe' port/game-data/tests/game_design_tables_differential.py
& 'C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe' port/game-data/tests/game_design_tables_host.py
```

The parent can add game_design_tables.cpp to game-data production and add a
`game_design_tables_audit` test target linking game-data, world, script-runtime
and dl. Its arguments are Level CHL1 gold, commons, monster, data-assets directory,
ai constants bytes, design constants bytes, then GDT1 gold. The same runner
accepts `--main-linked /home/adampalace/dh2-world-build/game_design_tables_audit`
and writes a separate main-linked report. It requires Level from the actual
world DSO and table exports from game-data, and does not rebuild the central
tree. The parent should bind central compiler commands/input snapshots.
