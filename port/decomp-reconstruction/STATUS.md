# Decompiled-function reconstruction: status

Updated 2026-10-12 by the integrator, after the final merges: glitch-scene (`aff5bf5d`), net (`74326091`),
pydata-core (`b4aa7a8b`), vox-audio (`7283788d`), core-video (`8408c960`) and the ConnectionLobby fix (`0c53e26f`).
The IDA layouts export came in by path (`c54c6057`). The final summary is `docs/DECOMP-RECONSTRUCTION-SUMMARY.md`.
Totals come from `python3 tools/decomp_progress.py`, which counts each ledger row once against its category's queue.
161 queued functions have no ledger row yet: net-session-online 146, net-transport 15. Every scene row is ledgered.
Tinyxml's 53 are ledgered now (see its NOTES.md). Video's 13 C2DDriver functions are ledgered as skipped.

## Totals

| | functions |
|---|---|
| queued | 15,164 |
| reconstructed | 7,472 |
| upstream (name-matched library code) | 1,820 |
| partial | 3,046 |
| skipped (deliberately not rebuilt) | 2,665 |
| not attempted | 161 |

Layout-blocked (partial or skipped rows whose notes need a struct or class layout): about 1,000, mostly scene (859).
Host tests: 380 of 380 pass. Windows (MinGW, under Wine): 380 of 380 pass.

Hex-Rays is the primary pseudocode source (`recovered/native/hexrays-all/`). The IDA assembly
listing is in `recovered/native/ida-asm/` for checks (`tools/decomp_asm.py`). Ghidra is the fallback
(`--source ghidra`).

## Per category

| category | queued | reconstructed | upstream | partial | skipped | not attempted |
|---|---|---|---|---|---|---|
| core-platform | 653 | 296 | 0 | 67 | 290 | 0 |
| glitch-core-io | 1,255 | 848 | 17 | 165 | 225 | 0 |
| glitch-scene-collada-gui | 4,354 | 1,365 | 3 | 1,982 | 1,004 | 0 |
| glitch-video | 1,686 | 630 | 0 | 318 | 738 | 0 |
| net-session-online | 1,235 | 781 | 0 | 183 | 125 | 146 |
| net-transport | 794 | 528 | 0 | 185 | 66 | 15 |
| pydata-accessors | 2,201 | 2,109 | 0 | 72 | 20 | 0 |
| tinyxml-and-c-libs | 1,846 | 7 | 1,755 | 8 | 76 | 0 |
| vox-audio | 1,140 | 908 | 45 | 66 | 121 | 0 |

Ledger rows outside a category's queue are not counted. net-session-online's 232 such rows are in
`net-session-online/ledger.ignored.csv`. The progress tool reports one ignored row, pydata's `0x4f2750`.

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
    rows moved with them into glitch-video's ledger. `glitch-core-io/include/glitch-core-io/video/` no longer
    exists, so do not look for their code there.
- ConnectionLobby derives from the real `Connection` (`0c53e26f`). The stand-in `Connection` in
  `net-transport/include/net-transport/deps/connection_base.hpp` is deleted. Its smaller layout let
  `Connection::disconnect` write past the object, and ASan reported it. ASan on net-transport is 31 of 31.
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
      at +0x08 and valueOffset at +0x0c. Canonical. Its static_assert is in `material_parameters_part_1b.hpp`.
    - Part 1b's `ParamEntry` is 12 bytes, with type at +0x01, count at +0x04 and offset at +0x08. It matches
      neither record, despite its comment. Its rows read struct fields, so they are consistent with
      themselves, but they cannot read the binary's table memory: a real table would be read at the wrong
      bytes. Part 1b's `MaterialParameterBlock` holds that record too.
    - Decision: unify on the two canonical records and retire `ParamEntry`. Its `set` flag becomes `ref != 0`
      for G. Done (`11edf5b0`): nothing references `ParamEntry` any more. G and M read their canonical records.
  - Pointer storage: owners M and R keep pointer slots as `boost::intrusive_ptr` in part 1c's
    `SPointerSlots`, keyed by the original byte offset. The listings do the reference-count add before the
    store and the release after it, and only an intrusive pointer reproduces that. Part 1b's raw-pointer
    slots are a port deviation and are retired in favour of `SPointerSlots`.
  - Stride: the 4th argument of the pointer forms is a byte stride, as the listings show (`a += stride`).
    The pointer declarations say `stride`. The dispatcher declarations still say `count` (open, below).
  - Pointer slot width: a stored pointer takes `sizeof(void*)` bytes on the host. The original stores 4.
    The owners size their value buffers to match. This is a port deviation and stays documented.
  - Done, merged from `feature/decomp-glitch-core-video` at `3f65fb50`. The bodies are in
    `material_parameters_typed_impl.hpp`, which only the three owner files include. The owner backends are
    `material_parameters_bind_g.cpp`, `_m.cpp` and `_r.cpp`, each with a test. The 16 M pointer rows carry
    their binding and the missing reference counting in their ledger notes. The part 1b hooks are defined
    now (`8408c960`, see Open).
- Musepack `mpc_*` queue rows are ledgered as `upstream`, matched by name. The bodies were not compared with
  upstream source. Four of them (`0x87f28c` mpc_set_replay_level, `0x8803d4` mpc_demux_ST, `0x8964ec`
  mpc_bits_can_dec, `0x8965b4` mpc_bits_enum_dec) are `upstream` in the consolidated `ledger.csv` but `skipped`
  in the vox slice ledger, which says the library function is unconfirmed. Open, see below.
- Category branches are merged as they are pushed: `glitch-core-video`, `pydata-core`, `vox-audio`,
  `net` and `glitch-scene`. The vox thread keeps its rows in `ledger.parts/`; the integrator consolidated
  them into `ledger.csv`, which is the file the progress tool reads.
- The pydata thread forwarded core-platform memory and touch thunks to their listing targets
  (`0x53419c` checked against `tools/decomp_asm.py`: it branches to `GlitchAlloc`).
- The Windows toolchain is installed by `tools/decomp_setup.sh` (see the README Setup section). On a fresh Wine
  prefix the first run of each test binary fails and the second passes (glitch-scene thread). Run the Windows
  build twice before reading a failure as real.

## Open

- intrusive_ptr exists in three forms. Core-io's `boost/intrusive_ptr.hpp` counts references
  (`intrusive_ptr_add_ref` and `intrusive_ptr_release`, with `IReferenceCounted`). Video's and scene's
  `deps/intrusive_ptr.hpp` are non-counting stubs. The runtime behaviour differs, so the owning threads
  decide. Not unified.
- glitch-core-io's `deps/glitch_core.hpp` now forwards to the shared header, so `rect` is the shared one
  (UpperLeftCorner and LowerRightCorner). Core-io's NOTES record the rename. Closed.
- core-io has a second SIDedCollection in `core/sided_collection.hpp`, separate from the shared one in
  `sid_collection.hpp`. Core-io's owner decides which one stays.
- core-platform keeps its own `deps/glitch_core.hpp`. Its rect and vector types have not been compared with
  the shared header.
- glitch-video: blockers on `IVideoDriver +0xd4`, `IAttributes` and `ITexture` remain. Their layouts come from
  usage, not from IDA, so they need evidence headers built from usage.
- glitch-video: the dispatcher declarations in `material_parameters_typed.hpp` (lines 110-116) still name the
  4th argument `count`. For codes 12-15 and 18 it is a byte stride. The impl passes the value through
  unchanged, so this is a naming fix only.
- glitch-video: the part 1b hooks are defined in `material_parameters_part_1b.cpp` (`8408c960`), and the test
  stubs are gone. Status by hook:
  - `textureAddRef(ITexture*)` and `textureStateWord(const ITexture*)`: full.
  - `textureDrop(ITexture*)`: partial. It is the ITexture form of IReferenceCounted::drop (0x31d584). The original
    calls onDelete (vtable +0x08) before the deleting destructor. The port has no vtable, so it does not.
  - `destroyLight(CLight*)`: partial (0x5aa3c8 family). The Matrix4Pool push (+0x50 handle) is not done. The
    light is deleted.
  The glitch-core-io ledger notes for 0x31d584 and 0x5aa3c8 cite these definitions. Their statuses are unchanged.
- IDA has no glitch:: layouts. The export on `analysis/ida-type-layouts` (`83528e6a`) has 2,467 local types, 526
  with members, and none of the 437 `glitch::` names has members (`glitch::scene` is 107 names, all
  forward-only). Its files are in `recovered/native/ida-types/layouts/` (`c54c6057`, brought in by path, not
  as a branch merge, since that branch's history is not this one). So the glitch structs come from usage: the
  glitch-scene thread recovers them field by field from Hex-Rays, with source addresses, into a shared header.
  No Remote Control IDA session is needed for layouts.
- glitch-scene-collada-gui: `layouts.hpp` is a notes file, not a type header, so it is not lifted.
- glitch-scene-collada-gui: 80 generated stubs in `generated_stubs.hpp` are address-named templates, not yet
  attached to their owning classes.
- Two partial `CColladaBinaryFileLoader` constructors (`0x6b9310`, `0x6b9360`) are in the scene ledger,
  but their code is still in `glitch-core-io/src/collada_binary_file_loader.cpp`.
- net-transport, layouts and send paths: `tMemberInfo`'s layout is unsettled, because `t_member_info.hpp` was
  overwritten. `CMsgRaisedEvent` and `CMsgAttackResult` are declared twice. The Bluetooth STL string layouts are
  garbled. `transport_manager_calls::Send` and the `DataPacket` vtable are stubs, so the send paths are blocked.
  About 64 blocked rows are named only in worker reports, not in the ledger.
- net-session-online: `NetBitStream` is byte-aligned only (stand-in in `deps/`). Its NOTES give the old queue count
  (1,268). The regenerated queue has 1,235.
- pydata-accessors: `0x4f2750` (`Structs::CharacterProperties::read`) is ledgered but not queued. It is the one
  ledger row the progress tool reports as ignored. Thunks `AssassinProperties`, `AttackingFollowerProperties` and
  `BanditProperties` forward to it.
- pydata-accessors, Arrays: the 27 rows of `w7_arrays27.cpp` use the 32-bit listing stride. On a 64-bit host
  `sizeof(element)` is larger for 25 of them (`Structs::Projectile` 80 against 72, `DestructibleContainer` 80
  against 68), so reads overrun their slot. Do not call the Arrays reads on a 64-bit host until the stride is
  decided: widen the tables to the host `sizeof`, or build for a 32-bit layout.
- pydata-accessors, Item family: the destructors (ArmorRef, BeltRef, Valuable, Potion, GoldItem, Item) and the
  ItemPower destructors in `structs_4` free `m_w2` twice, as the listing does. Do not call them on live objects
  until that is decided. `ItemList` and `ItemBonusAttrMonopoly` are matched to their element classes by name only.
- pydata-accessors, generators: `gen_structs.py` and `gen_script_cmd.py` still parse Ghidra text, so they cannot be
  rerun against the Hex-Rays export.
- vox-audio: `ledger.parts/` holds the older slice ledgers. 306 of its rows differ from `ledger.csv`, which is the
  consolidated record and the file the progress tool reads. Vox should retire `ledger.parts/`. The four Musepack
  rows above need a decision: keep them upstream by name, or mark them skipped until libmusepack is compared.
  `FUN_008a0364` is keyed at its queue address. Its note gives the IDA location: jump-table case 3 of
  `vox::DriverCallbackInterface::_Set3DParameter` at `loc_890364`.
- Thunks reimplemented rather than forwarded (core-platform 5, glitch-video 1, net-session-online 2)
  still need review.

Closed since the last update: the core-video shared copies (now one definition each, see Decisions); the
core-video stub bundle (kept out of the shared folder); the video material-parameter rows (routed to glitch-video);
the owner G, M and R bindings (merged at `3f65fb50`); the part 1b `ParamEntry` retirement (`11edf5b0`); the
`SParameterEntry` static_assert; the 16 M pointer ledger notes; the part 1b hook definitions (`8408c960`); the 13
C2DDriver ledger rows (now skipped, with their notes); the core-platform ledger conflict with pydata (HEAD's queue
kept, pydata's 15 status changes taken); the net-transport ConnectionLobby overflow (`0c53e26f`); the 53 tinyxml
rows (see its NOTES.md); and core-io's `rect` rename to the shared names. The core-video merge changed 40 video rows from reconstructed to partial, 2 from reconstructed to
skipped and 8 from skipped to partial. Those are the owner's ledger changes, merged as they are.

## Process

Each category works on its own branch, `feature/decomp-<category>`, cut from
`feature/decomp-reconstruction`. Each touches only its own folder and ledger. The integrator merges
them back as they push, keeps both builds green (`tools/decomp_build.sh host` and `windows`), and runs
the cross-category checks: the thunk check, and the asm spot-checks with `tools/decomp_asm.py`.
