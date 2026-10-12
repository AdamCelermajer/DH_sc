# Decompiled-function reconstruction: status

Updated 2026-10-12 by the integrator, after merging core-video wave 4 (`db85055f`) and vox-audio (`7283788d`,
merge `63c0f0b2`). Totals come from `python3 tools/decomp_progress.py`, which counts each ledger row once
against its category's queue. 1,341 queued functions have no ledger row yet. They are still to do; 13 of
them are video's C2DDriver functions.

## Totals

| | functions |
|---|---|
| queued | 15,164 |
| reconstructed | 7,305 |
| upstream (unmodified library code) | 1,796 |
| partial | 2,519 |
| skipped (deliberately not rebuilt) | 2,203 |

Hex-Rays is the primary pseudocode source (`recovered/native/hexrays-all/`). The IDA assembly
listing is in `recovered/native/ida-asm/` for checks (`tools/decomp_asm.py`). Ghidra is the fallback
(`--source ghidra`).

## Per category

| category | queued | reconstructed | upstream | partial | skipped |
|---|---|---|---|---|---|
| core-platform | 653 | 284 | 0 | 82 | 287 |
| glitch-core-io | 1,255 | 848 | 17 | 165 | 225 |
| glitch-scene-collada-gui | 4,354 | 1,279 | 0 | 1,487 | 581 |
| glitch-video | 1,686 | 672 | 0 | 271 | 730 |
| net-session-online | 1,235 | 704 | 0 | 163 | 115 |
| net-transport | 794 | 528 | 0 | 185 | 66 |
| pydata-accessors | 2,201 | 2,082 | 0 | 99 | 20 |
| tinyxml-and-c-libs | 1,846 | 0 | 1,734 | 1 | 58 |
| vox-audio | 1,140 | 908 | 45 | 66 | 121 |

## Decisions in force

- Hex-Rays is the primary source. Ghidra is the fallback only.
- Undefined corners (uninitialised fields, null inputs, reads past a table) stay `partial`.
  That is the no-guessing rule.
- 120 IMaterialParameters Cvt rows stay `partial`. The port refuses type codes 19 and up,
  where the original reads past its table.
- Ghidra's `elf_address` for `FUN_` entries is 0x10000 low. `tools/decomp_categories.py` moves each one
  to the address in its name and drops it when a named entry already sits there.
- Routing matches the function name, not its parameter list or return type. `head()` in
  `tools/decomp_categories.py` strips the return type (`strip_return_type`) and the parameter list.
  Two fixes this session:
  - Parameter types no longer decide the category (`IIrrXMLReader` no longer moves scene or gui functions).
    The reroute moved 246 core-io queue rows to scene and video, and dropped their skipped ledger rows.
  - A return type such as `boost::enable_if<glitch::video::...>::type` no longer sends a video function to
    core-io. The fix moves 311 rows from core-io to glitch-video. All 311 are `glitch::video` functions
    (IMaterialParameters, IVideoDriver screen-position helpers, the intrusive_ptr allocators). Their ledger
    rows moved with them. The code for them is still in `glitch-core-io/include/glitch-core-io/video/`
    until glitch-video moves it.
- Ledgers are LF only (`.gitattributes`). The core-io and video ledgers were rebuilt from their queues: each
  holds only rows its queue owns, and a row for a function another category owns is dropped there, so the
  new owner ledgers it. Other categories' ledgers are unchanged by this merge.
- Shared headers, one definition each, in `port/decomp-reconstruction/shared/include/glitch-shared/`
  (see its README):
  - `glitch_core.hpp`: the glitch::core value types, the glitch::video colour words, and CMatrix4 (from
    glitch-video). Video's and scene's `deps/` headers forward here.
  - `sid_collection.hpp`: SIDedCollection. glitch-core-io's `core/sid_collection.hpp` forwards here (the two
    copies were identical).
  - `gl_function_table.hpp`: GLFunctionTable. glitch-video's `deps/gl_function_table.hpp` forwards here (the
    two copies were identical).
  - The stub classes that core-video's copy of `glitch_core.hpp` also carried (IVideoDriver, ITexture, CLight,
    IShader, and so on, plus IReferenceCounted) are left out. Putting them in the shared folder would give
    every category that includes it a second definition of those classes.
- Material-parameter binding (decided 2026-10-12 from the part headers and their listings). The 311 rows
  moved from core-io are `IMaterialParameters` member templates (owners G, M and R). Their bodies are free
  functions in four part files, and each row's address is cited in exactly one part header. The bindings:
  - Binding point: `IMaterialParametersTyped<Derived>` in `material_parameters_typed.hpp`. The derived class
    (`IMaterialParameters<Owner, ...>`) supplies its owner's table through one accessor and needs the
    `using Base::...` lines that file lists. The member templates forward to the part free functions.
  - Coverage: 282 part-file signatures, 0 implemented twice. Bind 271 of them (243 reconstructed and 28
    partial). Owner G: 88 (part 1a 64, part 1b 24). Owner M: 100 (part 1b 51, part 1c 49). Owner R: 83
    (part 1c 26, part d 57). 11 rows that part 1a cites as skipped stay unbound. 29 rows with no part
    implementation (the intrusive_ptr rows, setArrayParameter and setParameterElement callers) stay unbound.
    40 unbound rows in total. Each is recorded in its ledger note.
  - Table records. Checked with a compiler (2026-10-12):
    - Owner G: part 1a `SParamEntry` is 20 bytes, with type at +0x06, count at +0x08 and offset at +0x0c,
      and it has a static_assert. Canonical.
    - Owners M and R: `SParameterEntry` in `material_parameters.hpp` is 16 bytes, with type at +0x06, count
      at +0x08 and valueOffset at +0x0c. Canonical. It has no static_assert yet; add one.
    - Part 1b's `ParamEntry` is 12 bytes, with type at +0x01, count at +0x04 and offset at +0x08. It matches
      neither record, despite its comment. Its rows read struct fields, so they are consistent with
      themselves, but they cannot read the binary's table memory: a real table would be read at the wrong
      bytes. Part 1b's `MaterialParameterBlock` holds that record too.
    - Decision: unify on the two canonical records and retire `ParamEntry`. Its `set` flag becomes `ref != 0`
      for G. The core-video thread keeps both copies in step until its M and R bindings land, then migrates
      the 1b rows to the canonical records.
  - Pointer storage: owners M and R keep pointer slots as `boost::intrusive_ptr` in part 1c's
    `SPointerSlots`, keyed by the original byte offset. The listings do the reference-count add before the
    store and the release after it, and only an intrusive pointer reproduces that. Part 1b's raw-pointer
    slots are a port deviation and are retired in favour of `SPointerSlots`.
  - Stride: the 4th argument of the pointer forms is a byte stride, as the listings show (`a += stride`).
    The typed header names it `count`. Its declarations should be corrected to `stride`.
  - Pointer slot width: a stored pointer takes `sizeof(void*)` bytes on the host. The original stores 4.
    The owners size their value buffers to match. This is a port deviation and stays documented.
  - Not done yet: the member templates and the owner table accessors are not written. The binding is the
    next step for glitch-video, with the 271-row set above as the acceptance test.
- Musepack `mpc_*` queue rows are ledgered as `upstream`, matched by name. The bodies were not
  compared with upstream source. The vox thread later marked four of them (`0x87f28c`, `0x8803d4`,
  `0x8964ec`, `0x8965b4`) `skipped`, since it could not confirm the library function.
- Category branches are merged as they are pushed: `glitch-core-video`, `pydata-core`, `vox-audio`,
  `net` and `glitch-scene`. The vox thread keeps its rows in `ledger.parts/`; the integrator consolidated
  them into `ledger.csv`, which is the file the progress tool reads.
- The pydata thread forwarded core-platform memory and touch thunks to their listing targets
  (`0x53419c` checked against `tools/decomp_asm.py`: it branches to `GlitchAlloc`).
- The Windows toolchain is installed by `tools/decomp_setup.sh` (see the README Setup section).

## Open

- intrusive_ptr exists in three forms. Core-io's `boost/intrusive_ptr.hpp` counts references
  (`intrusive_ptr_add_ref` and `intrusive_ptr_release`, with `IReferenceCounted`). Video's and scene's
  `deps/intrusive_ptr.hpp` are non-counting stubs. The runtime behaviour differs, so the owning threads
  decide. Not unified.
- glitch-core-io keeps its own `deps/glitch_core.hpp` with the stub classes and the rect members X1, Y1,
  X2, Y2. The shared rect uses UpperLeftCorner and LowerRightCorner. The two `rect` types must not meet in
  one translation unit. None does today. Core-io can move to the shared header once its 33 uses of X1..Y2
  are renamed (core-io's decision).
- core-io has a second SIDedCollection, `core/sided_collection.hpp`. It is not identical to the shared copy
  and its tests are separate. Core-io's owner decides which one stays.
- core-platform keeps its own `deps/glitch_core.hpp`. Its rect and vector types have not been compared with
  the shared header.
- glitch-video: the 311 moved rows are in its ledger, but the code is still under `glitch-core-io/include/
  glitch-core-io/video/`. Video should reconstruct them there, then remove the core-io copy.
- glitch-video: 13 queued functions have no ledger row yet, all video's C2DDriver family (`0x59f0b0` to
  `0x59fbd4`, `0x5fea04`, `0x5fed6c`).
- glitch-video: the material-parameter binding above is decided. The core-video thread has pushed owner G
  (88 rows) at `622dc3fe` on its branch. Owners M and R are still in progress there. Merge when they land.
- glitch-video: four hooks are declared in `material_parameters_part_1b.hpp` (lines 124-127) and defined
  nowhere in the library. Any executable that links the binding must supply them:
  - `destroyLight(CLight*)`: the last-reference release of a CLight (Matrix4Pool push, LC_API_FREE).
    Called at `material_parameters_part_1b.cpp:305`.
  - `textureAddRef(ITexture*)` and `textureDrop(ITexture*)`: the reference count at +0x04 of an ITexture, and
    IReferenceCounted::drop (0x31d584). Called at lines 401, 403, 455 and 459.
  - `textureStateWord(const ITexture*)`: the packed state word at +0x38 (type bits 0 to 1). Called at line 394.
  The only definitions are test stubs in `tests/material_parameters_part_1b_test.cpp` (lines 15-18). The host
  and Windows builds link because only that test pulls in the object, so the binding is test-only until the
  real definitions exist. The glitch-video owner must supply them with the texture and light code.
- IDA archive `ida 2.zip` (new, 2026-10-12): `recovered/native/ida-types/` holds the local-types C header
  and one listing. The header has no glitch:: class bodies (only forward declarations and std internals).
  The type library names 105 `glitch::scene::` classes, but their layouts stay encoded in IDA's binary
  format until the user exports them on the Windows PC. The scene threads are still blocked on those
  layouts. Remote Control session needed.
- glitch-scene-collada-gui: `layouts.hpp` is a notes file, not a type header, so it is not lifted.
- glitch-scene-collada-gui: 80 generated stubs in `generated_stubs.hpp` are address-named templates, not yet
  attached to their owning classes.
- Two partial `CColladaBinaryFileLoader` constructors (`0x6b9310`, `0x6b9360`) are in the scene ledger,
  but their code is still in `glitch-core-io/src/collada_binary_file_loader.cpp`.
- net-transport: `connection_manager` uses the real `CConnection` since `d14bdc02`, and the stand-in is
  deleted from the code. `net-transport/NOTES.md` still describes the stand-in. The net thread should update
  its NOTES.
- net-session-online: its NOTES say 1,268 queued. The regenerated queue has 1,235. The NOTES count is stale.
- pydata-accessors: `0x4f2750` (`Structs::CharacterProperties::read`) is ledgered but not queued. The
  inventory marks it claimed. It is the one ledger row the progress tool reports as ignored. Three thunks
  forward to it.
- pydata-core branch: its core-platform queue is stale (679 rows against 653 at HEAD). HEAD's queue, as
  regenerated by the tool, is authoritative.
- vox-audio: `ledger.parts/` still holds the pre-fix `FUN_` rows (`0x89xxxx`, `w9-fun.csv`). `ledger.csv` is
  the file the progress tool reads, and it has them at the queue addresses. The vox thread should retire
  `ledger.parts/`. `FUN_008a0364` is keyed at its queue address. Its note gives the IDA location: jump-table
  case 3 of `vox::DriverCallbackInterface::_Set3DParameter` at `loc_890364`.
- Thunks reimplemented rather than forwarded (core-platform 5, glitch-video 1, net-session-online 2)
  still need review.

Closed since the last update: the core-video shared copies (now one definition each, see Decisions); the
core-video stub bundle (kept out of the shared folder); the video material-parameter rows (routed to
glitch-video with the code location noted above).

## Process

Each category works on its own branch, `feature/decomp-<category>`, cut from
`feature/decomp-reconstruction`. Each touches only its own folder and ledger. The integrator merges
them back as they push, keeps both builds green (`tools/decomp_build.sh host` and `windows`), and runs
the cross-category checks: the thunk check, and the asm spot-checks with `tools/decomp_asm.py`.
