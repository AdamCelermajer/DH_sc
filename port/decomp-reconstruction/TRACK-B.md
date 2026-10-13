# Track B: whole-game source reconstruction

Branch `track-b/development`, cut from `feature/decomp-reconstruction` at `2482a90d` (the final decomp
merge). Track B rebuilds the whole of `libDungeonHunter2.so` as one coherent C++17 source tree that
later recompiles for ARM64 Android. Track A (the playable port on `feature/desktop-gameplay-loop`,
`port/level-world`, `port/android-native` and the other port modules) is a separate project. Track B
uses its code only as a behaviour reference and never copies it in.

Read `README.md` (rules for reconstructed code) and `STATUS.md` (decisions from the decomp phase)
first. This file adds what Track B changes.

## Scope

`tools/decomp_categories.py` no longer holds functions back for Track A. The 6,699 functions it used
to reserve or count as claimed are queued again:

| category | queued | contents |
|---|---|---|
| game-world | 1,527 | GameObject, ObjectManager, Level, rooms and zones, triggers, doors, containers, traps, projectiles, physics (PF*), visual objects and FX, lights, cameras, SceneManager |
| game-app | 1,303 | Application, state machines, Lua and script runtime, input, events, globals, rnd, streams |
| game-character | 1,143 | Character, CharAI, CharProperties, CharStateMachine, CS* skills, AIS* scripts, PlayerManager |
| game-items-save | 831 | items and inventory, quests and objectives, rewards, conditions, savegames, properties, trophies |
| game-ui | 704 | menus, HUD, GameSWF glue, dialogs, string manager |
| game-anim | 365 | animators, blenders, v2 controllers, animation sets |
| additions | 812 | claimed engine, net and pydata functions routed to their existing categories |

Vendored libraries (2026-10-13, `reports/decomp-reconstruction/vendored-libraries.md`): vendor-lua (471,
Lua 5.1.4 with float `lua_Number`), vendor-box2d (318, Box2D 2.0.1 with the DH2 `b2Settings`) and
vendor-gameswf (2,370, gameswf r1714 plus Gameloft changes). Pristine upstream sources are in
`vendor-*/upstream/`; DH2 changes are marked `// DH2:` and host-build fixes `HOST-PORT:`. Rows that match
upstream by name and size are ledgered `upstream`; the rest are reconstructed like any other row. RenderFX
glue moved to game-ui and the `sfc::script::lua` wrapper to game-app.

In-scope total: 25,218 functions. Thunks, the STL runtime and `libStormGLOFT.so` stay out of scope.
`dh2_vendor_lua` and `dh2_vendor_box2d` build and link into `dh2_game`; gameswf is headers-only until the
glitch render backend exists. Game code may include `lua.h`, `Box2D.h` and `gameswf/gameswf_value.h`.

## One tree, one definition

- **Game class headers.** Every game class has exactly one header, `game/include/dh2/<Class>.hpp`.
  Namespaced classes nest by directory (`dh2/Foo/Bar.hpp` for `Foo::Bar`). Nested classes
  (`Character::NetStructCharacter`) live in their owner's header. Free functions owned by `<global>`
  go in `game/include/dh2/global/<topic>.hpp`.
- **Sources.** Game sources go in `game-<cat>/src/<Class>.cpp`, or `<Class>_<n>.cpp` when one class is
  split across batches. Engine and library categories keep their existing layout.
- **Build.** All game categories build into one library, `dh2_game`. It sees `game/include`,
  `shared/include` and every engine category's `include/`, so a game file can include
  `glitch-video/...` or `vox-audio/...` directly.
- **Includes.** Use `#include "dh2/Character.hpp"`, never a relative path. Headers must be
  self-contained, so `tools/trackb/check_tu.sh <header>` compiles them on their own.
- **Layouts.** A layout lives only in its owner's header. Code that needs another class's field
  includes that class's header and uses the named member. If the member is not declared, the code
  does not invent a local stand-in struct. It declares nothing, marks the function `partial`, and
  records a layout request (see Reports below). The integrator applies the request to the owner header.
- **Stand-ins.** No new `deps/` stand-ins for game code. Existing engine `deps/` headers stay until
  the engine unification wave.

## Layout rules (game classes)

- Recover a layout from its constructor first, then its other methods (Hex-Rays `*(T*)(a1 + N)`
  accesses), and the vtable listing (`tools/decomp_vtable.py`). `reports/decomp-reconstruction/class-facts.json`
  gives the hints: base classes, members constructed in place, `operator new` sizes and vtable size.
- Declare members in their original order. Each member gets its original 32-bit offset as a comment:
  `CharAI m_ai; // +0x3c8`. When the original object size is known, record it as a comment on the class.
- Name members from evidence: a getter or setter name, a string, a Lua binding or a save field. Name an
  unknown field `unk_<hex offset>` with the narrowest type the accesses show. Fill an unknown gap with
  `uint8_t pad_<hex offset>[N];`. Never use `undefined4`.
- Declare virtual methods in vtable slot order, primary vtable first. Write `override` where a base
  declares the slot. With multiple vtable pointers (Character has them at +0, +4 and +0x24), declare
  the secondary bases in offset order.
- Do not `static_assert` the 32-bit sizes. The build is 64-bit, so pointers are 8 bytes.

## Decisions in force (the integrator settles the decomp open decisions for Track B)

1. `intrusive_ptr`: the counting form (`glitch-core-io`'s `boost/intrusive_ptr.hpp` with
   `IReferenceCounted`) is canonical. The non-counting stubs are retired in the engine unification wave.
2. Pointer width: in-memory pointers are host width. Binary data from files (savegames, tables, assets)
   is decoded field by field at its 32-bit file layout, never cast to a host struct.
3. pydata `Arrays` stride: the same rule. Tables are decoded at the file stride into host-sized elements.
4. A thunk is forwarded to its target. It is never reimplemented.
5. No guessing: an unknown branch, table or layout keeps the row `partial`, and its note says what is missing.
6. Engine conflicts (2026-10-13): the binary wins. `CCameraSceneNode` derives from `ICameraSceneNode`, and its
   `SViewFrustum` owns the overlapping box and matrices. `CNetPlayerInfo` derives from `NetStruct`, and its
   member id defaults to 0 as the constructor sets it. `NetStructMemberType<bool>` keeps the value at +0x1d.
   `ByteArray` owns its buffer.
7. The pydata generated headers are hand-maintained source. `gen_structs.py` and `gen_script_cmd.py` are retired.
8. Claimed by Track A means nothing in Track B. Every row is ledgered on its own evidence.
9. Access specifiers are not recoverable from the binary, and the original code reads other classes'
   fields directly (for example `Character` reads `CharAI` +0x40). Game class members are `public`. Do not
   add `private:` or `protected:` to game headers, and do not add getters only to get around access.

## Worker protocol (one batch = one owner class, up to about 20 functions)

1. Read every function in the batch: `py -3 tools/decomp_extract.py --address 0x...` (Hex-Rays). Check
   doubtful spots against the assembly (`py -3 tools/decomp_asm.py`) or IDA (ida-pro-mcp tools:
   `decompile`, `disasm`, `xrefs_to`). Do not write to the IDA database.
2. Write the bodies into the batch's own source file. Add method declarations to the owner header
   when they are missing. The owner header is yours for the batch; other headers are read-only.
3. Each function starts with `// 0x<addr> <mangled or demangled symbol>`.
4. Compile: `bash tools/trackb/check_tu.sh <your .cpp> <owner header>`. Fix until both are `OK`.
5. Add a test in `game-<cat>/tests/<class>_<n>_test.cpp` where the logic can run without the
   engine (arithmetic, state, parsing). Check it with `bash tools/trackb/check_tu.sh --test <test> <sources>`.
6. Write the ledger rows to `<category>/ledger.d/<batch-id>.csv` (header
   `elf_address,symbol,status,source_file,notes`), one row per batch function. Never edit `ledger.csv`.
7. Report back with the structured result (statuses, files and layout requests). Do not run git.

## Reports and integration

After each wave the integrator runs `py -3 tools/trackb/merge_ledger.py`, applies the layout requests
to the owner headers, rebuilds (`cmake --build build/tb-host`), runs the tests, updates
`tools/decomp_progress.py` totals in `STATUS.md`, commits and pushes.

## Known baseline failures (native MinGW, 2026-10-12)

The decomp phase reported 380 of 380 tests passing under Wine. Native MinGW at `2482a90d`: 376 of 380.
- `core-platform/filesystem_extra_test`, `sys_config_fs_test`, `win32_slim_test`: exit code
  0xc0000139 (entry point not found). Probably a DLL mismatch on this machine.
- `vox-audio/random_group_test`: exit code 0xc0000374 (heap corruption). Wine did not show it. Likely a real bug.
