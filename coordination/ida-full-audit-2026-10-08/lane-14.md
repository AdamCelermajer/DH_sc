# Lane 14 — GameSWF, RenderFX/MenuFX, renderer, GLU and Box2D

Assigned primary inventory: **libDungeonHunter2.so indices 24260–26125 inclusive**. Baseline marker: 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This report reads the current dirty working tree. Only lane-14.md and lane-14.csv were written. No game source edits, builds, tests, emulator launch, or runtime acceptance were performed.

## Coverage and limits

The CSV has **1,866 rows and 1,866 unique assigned IDs**, with **zero missing, duplicate or out-of-range IDs**. All 1,866 decompilations succeeded; there are no failed-body records or external imports in this range. Every assigned pseudocode body was read/fingerprinted and every assigned assembly block scanned: **108,415 ARM instruction lines**. Source candidates, body calls/literals/control facts and xrefs were catalogued per record. Detailed semantic comparisons cover the subsystem anchors below. This is inventory-disposition coverage, not proof of complete semantic parity for every record.

There are **1,578 unique exact pseudocode body texts**, **153 repeated-body groups**, and **288 additional identical-body records**. Each record remains a separate CSV row. Final clone counts differ from 288 where a row has its own explicit comparison or is an ABI adjustor. The complete repeated-body member list appears below.

The entire native xref export was used to follow callers/callees across the assigned range. Internal branches were excluded from the caller/callee lists. Lists are bounded to eight incoming and twelve outgoing sites and include total site counts. Raw vtable bytes add references for **413 assigned records** where normal data xrefs may be absent. Indirect target sets remain unresolved unless an explicit path is described below.

**Every row has integrated_runtime_acceptance=not_run.** No static source match or isolated existing test is counted as integrated gameplay acceptance.

| Comparison | Records |
|---|---:|
| matched | 44 |
| partial | 588 |
| disconnected | 2 |
| missing | 0 |
| unclear | 945 |
| import/thunk | 5 |
| clone | 282 |
| failed-body | 0 |
| out-of-scope | 0 |

Matched means a scoped static behavioral match within the valid input/body domain described in the row. It does not mean identical ARM/host layouts, debug behavior, all providers/callers, or gameplay acceptance. Partial means a related behavior was compared but differences or unclosed branches remain. Unclear means body evidence and source candidates/reasons were recorded without claiming equivalence. No missing claim is inferred from a text-search miss.

## Changes since the pause

The prior CSV source hashes were rechecked. **Three cited files changed, affecting 54 existing rows**: gfnt_text_backend_v1.cpp, swf_text_font_platform_v1.cpp and text_render_owner_v2.cpp. Their current bodies were re-read; earlier and current hashes remain in row evidence and the table below. Current sources add GFNT resource sharing and clear-font hooks, and the platform releases exact image owners before clearing font/image/core registries. F5 remains partial because key width, filter ownership, atlas eviction and flush/reset ordering are not thereby proven equivalent.

| File | Before pause SHA-256 | Current SHA-256 |
|---|---|---|
| `port/engine-ui/gfnt_text_backend_v1.cpp` | `b2ecd5a9eae3f9a514e8d07a6ea4de1852b7ae34b1d6f72fbaae861aabe92ef7` | `87da563d2b7a36d7afff3461e973b20059b0250cc369f4cbf99647ae381e3e9a` |
| `port/engine-ui/swf_text_font_platform_v1.cpp` | `1faa3f2ed49d7493f30557acccd76be895dd34bef8c9703ea4817bf8c5897cc1` | `30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8` |
| `port/engine-ui/text_render_owner_v2.cpp` | `9912b4c218bbfe01bd2430ced3bb92c35abe2e84d371e905513d39b226106a29` | `6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140` |

The AVM2 frame connection, GLES renderer and PhysicalWorld/Box2D scene links remain unchanged from their previously cited hashes. The active facade reset addition is addressed in the later bounded S8 recheck; final source references use current hashes.

## Findings

### F1 — Native nonfinite normalization differs from active GameSWF math

Unverified/failing path: morph fill interpolation or world color-transform composition produces NaN/Infinity, then writes a matrix or color coefficient. Native #24631 @0x7947b8 and #24634 @0x794d38 compare arithmetic results with -FLT_MAX/+FLT_MAX and write zero when either comparison fails. For the first matrix coefficient, ARM comparisons occur at 0x7947f4..0x79481c and the failure branch stores zero. The assembly uses __aeabi_fcmpge/__aeabi_fcmple, which also fail for NaN; the pseudocode appearance of simple less/greater comparisons is insufficient here.

Caller closure: #23719 morph2_character_def::display @0x761b9c -> #24442 fill_style::set_lerp @0x78454c -> #24631, and cross-lane #23422 character::update_world_cxform @0x75488c -> #24634. The active selected gameswf_types.cpp stores raw lerp/products/inverse/read-scale outputs. Related #24643/#24651/#24656 have native normalization too. Source: `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_types.cpp:121 [sha256=f5f6b21e42eb2030837670b42137a5599d0bf2333ec7e6670c37f7ede648d302]` and `port/engine-ui/vendor/gameswf1714/base/utility.h:117 [sha256=9fbaacfc6e7d7da6ec742ffc5c6fbdfaaf6100bf644c8f9189211992edbfedba]`.

Disposition: partial, high confidence in the body difference. No supplied-asset trigger or live output failure was established. The GLES finite-input error check does not reproduce native coefficient normalization.

### F2 — String boundaries differ in writer and length readers

Writer #25316 @0x7b6bb8 writes each byte through its callback before testing that byte for NUL: ARM 0x7b6bd0..0x7b6bfc. It includes the terminator; an empty string writes one byte. The active tu_file::write_string checks while(*src) before writing and omits the terminator. Source: `port/engine-ui/vendor/gameswf1714/base/tu_file.cpp:474 [sha256=ef437d2f1c64c1ae06d3d31dc5b064440539c5f35663b4846fb9404c63c0c8b6]`. No incoming non-self direct call or data xref was present in the export, so dynamic/export reachability remains unresolved. This is a confirmed static difference, not a claim of live serialization failure.

Length readers #24432 @0x783f48 and #24437 @0x784288 gather bytes into scratch storage, append NUL and call #23957 tu_string::operator=(char const*), truncating at an embedded NUL. #24432 is called by font reads #25714/#25717; #24437 by ABC constant-pool reader #25440 @0x7b9c90. Current stream code appends every requested byte; tu_string::operator+=(char) increases length even for NUL. Source: `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_stream.cpp:258 [sha256=39953c40ce3c833ea18e7b82cb182fb8bd6a1f493bc7a30a64953aec2b0bcdff]` and `port/engine-ui/vendor/gameswf1714/base/container.h:1575 [sha256=b6a1bda8445842823bfc2f52d1160f732a9a08b9026b19407b685192b9178b33]`. The ordinary NUL-terminated reader #24436 has the same byte content but a different storage strategy.

Disposition: partial; embedded-NUL occurrence in actual input assets was not established. Original stream bool constructor/scratch256 layout also differs from stock input-only stream layout.

### F3 — AVM2 interpreter bodies are disconnected from source root frames

Unverified/disconnected path: an AVM2 root enters frame advance. Native cross-lane #24105 root::advance @0x775304 has no AVM2 rejection and advances its movie through virtual offset0x5c. Native #25554 @0x7c4130 calls #25552 @0x7c35fc; the operator is also in _ZTVN7gameswf13as_3_functionE @0x991310+0x6c.

Current swf_frame_root_advance rejects r->m_def->m_is_avm2 with Unsupported source AVM2 frame interpreter before movie advance, although gameswf_avm2.cpp is compiled and its interpreter exists. Source: `port/engine-ui/swf_frame_connection.cpp:145 [sha256=9f3ddea3d3321f0dfc3718d3f1e670558c84a6bb99875adb2f6f9556c7664837]` and `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_avm2.cpp:136 [sha256=2841ee351ebfc30eb06e4de87cf88dc3eeb95f0d54e13a869cb9b9386cf84fd1]`.

Disposition: disconnected for #25552/#25554 specifically on this root-frame path. It does not assert that supplied game menus use AVM2, or that all direct alternate invocation paths are blocked. The cross-lane native root body/hash is preserved in these CSV rows without adding a second primary coverage row.

### F4 — Original nonreturn anomalies have explicit dispositions

Native #25358 tu_timer::sleep @0x7b78f0 is exactly one ARM branch to itself, with no external sleep callee. The source calls Windows Sleep or Unix usleep(ms*1000): `port/engine-ui/vendor/gameswf1714/base/tu_timer.cpp:155 [sha256=35a2f641a6fcac06049055c19a3602b18049ff4531d6e771228302a13653b449]`. No incoming non-self caller was observed. It is a self-branch/nonreturn-body, not a failed decompilation or external import.

Native #25649 inst_info_avm2 varargs constructor @0x7c9acc reserves/stores the first argument format, then unconditionally calls abort at0x7c9b20. Source instead iterates va_arg(...,int) until ARG_END: `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_disasm.cpp:95 [sha256=da41b1d087f9c8b24653d2de43b550f365160d96f14764bef314558f8fd916af]`. Native #25659 log_disasm_avm2 @0x7ca2b8 has 68 constructor call sites. Diagnostic activation/runtime reachability is unresolved.

Both are partial with high confidence in the static difference and not_run. An intentional working replacement is possible; these findings do not classify them as absent features.

### F5 — Glyph atlas contract is not established by the source image registry

Unverified path: #24547 preload_glyph_codes or #24595 display_glyph_records -> #25748 get_glyph_region -> #25747 add_glyph_region. The original key packs an unsigned-eight-bit size and a filter triplet. Allocation failure leads to renderer flush, an eviction retry, cache reset and a third attempt. FT load flag4, optional monochrome conversion, filtering, atlas lock/region bounds and copy-to-atlas behavior are present in the native bodies.

The current owner uses a per-face code|(size<<16) map and individual image texture registry, with filters handled later in the display pipeline. Current post-pause GFNT disabled-cache miss/resource sharing and font-reset release hooks were inspected too. Sources: `port/engine-ui/text_render_owner_v2.cpp:68 [sha256=6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140]`, `port/engine-ui/swf_text_font_platform_v1.cpp:77 [sha256=30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8]` and `port/engine-ui/gfnt_text_backend_v1.cpp:56 [sha256=87da563d2b7a36d7afff3461e973b20059b0250cc369f4cbf99647ae381e3e9a]`.

Disposition: partial. Actual glyph/image producers and retained textures exist. Size aliasing, filtered-key reuse, lock/flush/reset/eviction order, failure rollback and callback ownership remain unproved. This is not evidence that fonts or caches are absent.

### F6 — HTML support depends on the actual caller

Native #25058 RenderFX::SetText @0x7a92e0 checks Is(32), then calls #24597 with the requested HTML bit. #25059-#25063 are name/varargs format wrappers. The plain source adapter rejects parse=true; authored localization directly invokes source_set_text_v1(...,true,true), with HTML supplied by the edit-text sidecar. Sources: `port/engine-ui/renderfx_text_connection.cpp:29 [sha256=d59f9d9a4b644483ab1152c868815be353fb246c70d03aeab264e56279d2e126]` and `port/engine-ui/authored_menu_localization_v1.cpp:77 [sha256=632e6e2e098a27da06b4b7e2a851e8f5ee897d69307f3dd01357930aaf3268f2]`.

Disposition: partial, not missing. The exact caller routing and original varargs formatting semantics are not closed for every generic FormatHTML consumer. Working localization does not prove all HTML callers.

## Subsystem comparisons and cross-boundary closure

### S1 — Sprite scheduling and ownership

#24260/#24261 root scale/mouse forwards and #24264/#24265 reverse-tag lookup/remove-only passes have scoped static matches. #24358 advance is in the native sprite vtable @0x990698+0x64 and the active overlay reaches swf_frame_sprite_advance and dh2_ui_swf_sprite_frame on the same player/history/identity.

Detailed comparison preserved construction/load10, visible-or-first-load gate, copied/cleared goto batches up to12 iterations, PLAY frame increment/wrap, enterFrame12, actions then children and loaded-state order. Source uses bounded4096 storage where original batches dynamically allocate. Children retain snapshot identities in reverse and process forward. #24338 GC marking maps to base traversal before captured child count with fresh child/epoch reads. Complete reentry/GC/storage parity is partial. Sources: `port/engine-ui/swf_frame_schedule.cpp:12 [sha256=2401099b82e8ba5e5f093d5a33690cbe3258363081b74730dce41c13f3d6bc59]` and `port/engine-ui/overlays/frame-v1/gameswf_sprite.cpp:246 [sha256=5ba3c07c2f106a685ebf43294f17036e885ed96b3cd7f4956ba838c59104250a]`.

Smart/weak pointer helpers were compared with actual base definitions and the selected refcount backend; identical set_ref bodies do not establish all parent/collector epochs. Sources: `port/engine-ui/vendor/gameswf1714/base/smart_ptr.h:71 [sha256=5f5f23ea12fa5bbe15695ebba88a1157491b846a4b94fb7dd8fc720999a4730a]` and `port/engine-ui/vendor/gameswf1714/base/weak_ptr.h:141 [sha256=8e316940123f0fee2e26b22a334bc3c8e113196c32d026f3cfdb6c8ad357f4db]`.

### S2 — Streams, tags, containers and resource helpers

#24410-#24437 reader comparisons cover MSB-first packed bits, signed extension, float16 rebias111, fixed1/65536, scalar alignment, vu32 continuation masks and tag type>>6/length&0x3f/extended length/LIFO end-seek. Matches exclude invalid bit widths, debug diagnostics and native endian/host-layout differences. F2 retains constructor/storage/string differences.

Array/hash/queue helpers were inspected for element destruction, growth, frozen-storage byte, null/zero handling and -2 empty/-1 chain-end sentinels. Stock array::resize(0) frees storage and may shrink; many original clear paths retain capacity and separately reserve(0) on destruction. Generic helpers remain partial/unclear. Source: `port/engine-ui/vendor/gameswf1714/base/container.h:331 [sha256=b6a1bda8445842823bfc2f52d1160f732a9a08b9026b19407b685192b9178b33]`.

Image/membuf/file/random/timer/zlib bodies #25239-#25389 have individual rows. Zlib reset/rewind/error/logical-position and4096-byte inflate callback paths were read against selected source. JPEG-disabled stubs must be interpreted with build defines rather than classified as missing decoders.

### S3 — Edit text, HTML and display

The selected edit-text overlay calls SwfEditTextFieldV1 with the actual definition/font/field. The sidecar imports/exports fresh fields around synchronous providers. set_text_value retains the supplied untruncated string for optional bound-variable writes; bound refresh compares then converts again before setting. HTML p/font/b/i/u/img,20-twip image dimensions, world matrix/color, actual embedded shapes/bitmap binding and buffered same-player fields were traced. Larger raster/filter/layout/reentry branches remain partial. Sources: `port/engine-ui/edit_text_field_v1.cpp:30 [sha256=9cc385100bf673491aade151e2b35e99e428022d824c37be8c7f34403d831f9a]`, `port/engine-ui/text_layout_v1.cpp:167 [sha256=5f4aaeb895f6529a7681ccebe7a0df86ef05e9d9e18527ac3d347c48e72b8316]` and `port/engine-ui/swf_edit_text_connection_v1.cpp:129 [sha256=a3f6b534c0cffa163407351c93c3250122060f2e7d65360e866f5ba2972253c5]`.

### S4 — RenderFX/MenuFX event, callback, input, search and teardown

Registration #25071 selects #25133 fscommand handler. ARM #25133 reads the originating weak player/root, then owner+0x94; #25012 loads listener+0xfc and calls virtual+4 only if nonnull. Active facade Scope binds/restores callback state, validates the origin player and retains the captured listener/provider through reentry. Receiver AS callbacks use its sprite environment or sprite parent. Sources: `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:112 [sha256=0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8]` and `port/engine-ui/swf_actionscript_connection.cpp:190 [sha256=c3e873f984d9033fd9950a8963738110b6cd07e0cc6db34cbc6c39aeea70c9ee]`.

#25124 event dispatch has a scoped valid-provider match: native delivery first; consumed suppression; kinds0/1/2/4/6/7/8/9/10/11 map to the original AS handlers; release invokes onRelease before fresh-name selection19/1/20/12. Source: `port/engine-ui/swf_event_dispatch.cpp:3 [sha256=48fd9df725c52d79dcfb4a49f1bf301b7afb3bafef97b66dce2fcc4f6a77ecee]`.

#25125/#25129/#25130/#25131 focus/navigation/cursor/pending updates reach actual connection/history/controller/viewport projections. SearchIndex maps actual hierarchy/name order and retains original256-byte entries/128-byte query domains. MenuFX register/set/pop/state/catalog methods map owned render catalogs and shared roster providers, but generic manager-stack code does not prove concrete state virtual closure. Base/MenuFX unload resets controllers in both phases and clears retained catalog/active arrays. Source: `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:220 [sha256=0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8]`.

### S5 — ActionScript interpreter, builtins, ABC and diagnostics

#25488 AVM1 execute is130,918 pseudocode bytes and is reached through #25489 and #25766. Scoped comparison covered variable/member access, function/method calls, FSCommand GetURL, function2 registers/local barriers, target restoration and return paths through actual sprite action batches. Complete opcode/fork ABI/storage/reentry equivalence remains unresolved. Source: `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_action.cpp:475 [sha256=5d9581fb1fbfe0b1c648e3a91315f16bf407f52fd49a066b14ad7ab76d9b8665]` and `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_function.cpp:74 [sha256=ee06a9e066268b580e51d043710b0cc9873ea7e2435f77c80a2afd4cbd38bcef]`.

Builtin constructors/registration/value conversions, array sort STL instantiations, date/key/math/selection/sound/loader/timer and ABC bodies have separate rows. Name/source hits are candidates; no-op/debug native paths and F3/F4 anomalies are retained explicitly.

### S6 — Custom renderer, GLU and tessellation

#25768-#25888 includes base renderer defaults, bitmap_info_ogl, render_handler_glitch, BufferedRenderer, materials and driver wrappers. Stock OpenGL bitmap_info_ogl is not selected by the current source list and is not the original Irrlicht-backed implementation. Current SwfMovie::Impl emits SwfDraw to Android SwfGpu. #25854 layout is in its native vtable@0x991880+0x10; #25860 flush is called on texture/begin/end/queue/mask transitions and unlocks the atlas before material/draw. Replacement queue/offset/atlas/reset/filter/wrap/mask/teardown parity is partial. Sources: `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:186 [sha256=0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8]` and `port/android-native/app/src/main/cpp/swf_gpu.cpp:86 [sha256=140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd]`.

GLU API/default callbacks #25150-#25183 and half-edge/sweep/render/dictionary/priority routines #25918-#25983 remain unclear or identical-body clones. Current shape tessellation uses ear_clip_triangulate::compute, while GLES masks use stencil. Exact topology/error/callback equivalence was not established; no source absence is inferred. Sources: `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_tesselate.cpp:789 [sha256=6211e78037c6d150f45db87ec7a2693737a78bb2ebd487a8777af89c6457b809]` and `port/android-native/app/src/main/cpp/swf_gpu.cpp:437 [sha256=140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd]`.

The five import/thunk rows are ABI adjustors: #25017/#25138/#25140 subtract0x100 before their MenuFX target; #25635/#25637 subtract0x20 before canvas destructors. Actual assembly was inspected. They are not external imports or missing application methods. #25794 is only a wireframe setter match, with draw output unaccepted.

### S7 — Box2D body/proxy/contact/solver lifecycle and actual scene owner

The active dh2_level_world links dh2_box2d_201. NativeWorld owns b2World, installs boundary/filter/contact/destruction listeners and calls Step from update. Native cross-lane #1860 PhysicalWorld::update @0x34bd08 calls #26086 Step. Source: `port/level-world/physical_world.cpp:66 [sha256=104dd9049d1ca95ed2bfdcfb6a83748d0a34f9e6447c0c1ec015d26bcc5c0ba4]`, `port/level-world/CMakeLists.txt:92 [sha256=78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608]` and `port/physics-backend/CMakeLists.txt:14 [sha256=8e7041f90cf62cc78a52085cac6ddcceface17825916f493ff836561616d0d10]`.

Valid-input static matches cover #26025 pair commit callback/userdata/final/remove order; #26086 Step lock/dt/invDt/dtRatio, Collide even dt0, positive-dt Solve, optional TOI, debug/invDt/unlock; #26125 skip only when both bodies sleep and update with the same world listener; #26120 old/new manifold and wake/slow flag; #26099/#26105 allocator14-size lookup, zero behavior,128-table growth and4096-byte chunk chain. Neighboring body/mass/shape/proxy/bounds/move/query, polygon/circle, Solve/TOI/island and teardown branches remain partial. Debug assert/poisoning behavior is excluded from scoped release-domain matches. Source: `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2World.cpp:733 [sha256=ad0dd30502b442b6b27bc05e62239ff6ef823ade04a19c34482665c25ecbdd56]` and `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2ContactManager.cpp:226 [sha256=7c370f2d5012f921ce8b2a3ae194332792f5e773f3031e61e7c2c4f4a9dcb6a8]`.

## Unresolved gate

This report provides exact assigned-record dispositions. It does **not** establish complete semantic equivalence for all nontrivial records. The unclear/partial sets, disconnected AVM2 root path, unclosed renderer/GLU/filter/atlas callbacks, compiler/STL ABI, source storage/GC/catalog reentry, asset triggers and runtime output remain explicit.

Candidate source locations sometimes identify a related class/adapter instead of a one-to-one native method; line1 denotes a whole-file candidate rather than an exact method at that line. Full source-name matches are not proof of parity. The merged audit cannot be called complete on this lane’s static count alone.

The exact unclear index set by body category follows. Other unresolved records are enumerated individually as partial/disconnected/clone in CSV. No runtime acceptance is inferred.

| Category | Count | Exact indices |
|---|---:|---|
| Box2D-body | 8 | 25946, 26035, 26037, 26042, 26110, 26115, 26123-26124 |
| GLU-tessellation-body | 83 | 25163-25164, 25166-25169, 25171-25175, 25177-25183, 25918-25945, 25947-25983 |
| menu/UI-body | 57 | 24997, 25007-25011, 25013-25014, 25023, 25033-25034, 25036, 25045, 25047-25048, 25051-25056, 25064-25065, 25069-25070, 25072, 25076-25079, 25082-25088, 25090-25091, 25101-25104, 25106-25112, 25114-25116, 25120-25121, 25128, 25148 |
| native-body | 585 | 24315, 24334, 24336, 24344, 24365, 24368-24369, 24374, 24377-24378, 24384, 24387, 24400-24403, 24405, 24407, 24409, 24438, 24440-24448, 24452-24461, 24471-24472, 24477, 24479-24494, 24498-24499, 24502, 24504-24505, 24507-24508, 24511-24516, 24529, 24535, 24542-24543, 24545, 24547, 24549-24550, 24552, 24554, 24560-24562, 24578, 24582-24583, 24603, 24659-24661, 24664-24665, 24667-24669, 24671, 24673-24685, 24687, 24689, 24691-24693, 24695-24698, 24701-24704, 24706-24707, 24709-24712, 24714-24715, 24719-24724, 24726-24730, 24735-24736, 24753-24755, 24769, 24771-24772, 24781-24782, 24784-24791, 24794-24798, 24800-24802, 24806-24819, 24821-24828, 24830, 24833-24835, 24838-24864, 24867, 24869, 24871-24876, 24878-24880, 24886, 24888-24890, 24895-24896, 24899-24903, 24905-24908, 24910-24911, 24914-24919, 24922-24930, 24933, 24935-24938, 24941-24952, 24954-24968, 24970-24971, 24973-24974, 24976, 24979, 24981-24982, 24984, 24986, 24988-24994, 25031-25032, 25043, 25073, 25089, 25144, 25239, 25243-25248, 25250, 25252, 25254, 25256, 25258, 25260, 25262-25275, 25279, 25282, 25284-25291, 25296, 25298, 25300, 25304, 25306, 25308, 25310, 25313-25315, 25317, 25319-25320, 25322-25330, 25332-25333, 25335, 25338-25339, 25345-25346, 25348-25349, 25352, 25354, 25360, 25362, 25367-25378, 25382-25389, 25392-25393, 25400, 25421, 25423, 25425-25430, 25432-25434, 25436-25437, 25439-25443, 25453-25454, 25459, 25467, 25469-25470, 25472-25473, 25476, 25485-25487, 25491-25498, 25500-25529, 25535-25537, 25541, 25544-25545, 25547, 25553, 25594-25595, 25598-25603, 25607-25611, 25615-25616, 25618, 25620-25634, 25636, 25638, 25640, 25642-25644, 25647-25648, 25650-25651, 25659-25662, 25666, 25668-25670, 25672-25674, 25676, 25678-25680, 25683-25684, 25688, 25692, 25704, 25706-25707, 25709-25711, 25713-25715, 25717-25718, 25739, 25749, 25751, 25754, 25759, 25767, 25776-25777, 25818, 25820, 25857, 25890, 25892, 25895-25896, 25902-25904, 25906, 25908-25909, 25911, 25913, 25916 |
| no-op | 1 | 24370 |
| scalar/constant/virtual-wrapper | 81 | 24297, 24366, 24376, 24392, 24394-24399, 24662, 24690, 24694, 24699-24700, 24725, 24773, 24777-24780, 24792, 24803-24804, 24829, 24831, 24837, 24865, 24881, 24904, 24912, 24920, 24932, 24939, 24969, 24977, 24987, 24998-24999, 25002-25003, 25005-25006, 25016, 25044, 25046, 25113, 25165, 25170, 25176, 25241, 25280, 25293, 25301-25303, 25311, 25321, 25336-25337, 25340-25341, 25351, 25353, 25355-25356, 25361, 25381, 25391, 25444-25445, 25499, 25530-25531, 25654, 25663-25664, 25807, 25889, 25905, 25912 |
| static-initializer | 10 | 24271, 24474, 24642, 24975, 25021, 25449, 25645, 25891, 26012, 26098 |
| template-helper | 120 | 24269, 24277, 24332, 24470, 24509-24510, 24663, 24666, 24708, 24716-24718, 24731-24734, 24737-24752, 24756-24768, 24774, 24793, 24805, 24832, 24866, 24882, 24921, 24940, 24978, 25022, 25184-25191, 25193-25208, 25210-25211, 25213-25238, 25446, 25448, 25490, 25533, 25665, 25675, 25677, 25681-25682, 25833-25835, 25898 |

## S8 — Bounded reset interface and process-owner recheck

This supplements the helper reread and retains F5 partial. It was scoped to the changed headers and application reset owner/release edges requested after post-pause-source-impact.md. No Language end-to-end acceptance, rollback, build, test, emulator or runtime result is asserted.

### Provenance

The four interface contracts were read directly: GFNT declares resource-sharing state and clear_fonts; TextFontBackendsV2 adds its optional clear_fonts callback; TextRenderOwnerV2 declares clear_fonts_v119; the per-player platform declares source_reset_fonts_v119; SwfServices carries release_image_v119 and SwfMovie declares its root reset entry. These are current source contracts, not a compiled/native ABI equivalence claim.

`port/engine-ui/gfnt_text_backend_v1.hpp:1 [sha256=4e9425ce9e4133c8f7c4a3df9d023692ad25b4232b08438f296ee4b164e6034e]`

`port/engine-ui/text_render_owner_v2.hpp:1 [sha256=4e72399c7fe8fee28f60216060a3ef6549b0b85cc41a04945caca93fcfb3db8a]`

`port/engine-ui/swf_text_font_platform_v1.hpp:1 [sha256=611c1327217e41f618b78b7aed57b7603109fbe751a4a9f0e897a9a89c8637f0]`

`port/engine-ui/swf_movie.hpp:1 [sha256=5a1d3684ebb49e610118e5aba42c432e475cc6b9fe3b13888fdd49d3a3513c5a]`

The active overlay path is port/engine-ui/overlays/source-facade-v1/swf_movie.cpp. The prior lane citation was `6ba7aa327a5986c4a1266a4359ae3ad56317f5fbc114f9db0956aef888d8655d`; current `0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8`. Its mtime is **2026-10-08 18:01:48.3333352 local / 15:01:48.3333352 UTC**, before the 18:11:30 impact cutoff. The current hash stayed stable across this bounded recheck. This refresh is an older lane-provenance correction, not evidence of a new post-18:11 edit. native_character_menu_v4.inc stays d6d00fb8… with mtime17:53:16.6793376, matching the impact snapshot. The three previously reread helper cpp hashes remain current.

### Supported application-to-movie owner path

1. prepare_native_menu_process_v104 requires the supplied application to equal application_services_v5 and publishes NativeCharacterMenuV4 through shared ownership. The class derives enable_shared_from_this and the global manager is shared_ptr. The process path explicitly creates it with make_shared before calling prepare_process_directory_v104. `port/android-native/app/src/main/cpp/native_menu_resources_v93.inc:256 [sha256=c4ed613435541f69ef9d668c926e4713ae3e62dcd757888a90529d200581a57e]`; `port/android-native/app/src/main/cpp/native_character_menu_v4.inc:337 [sha256=d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557]`.
2. Bootstrap stores a callback capturing weak_from_this. FrontUiSession stores the callable; its reset_option_fonts fails if no callable is bound. The callable locks its captured manager for the synchronous call and fails when expired, avoiding a direct Front -> strong manager callback cycle. It establishes the originally captured manager, not proof that every later global application/reload generation is the same. `port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc:113 [sha256=21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d]`; `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:1820 [sha256=3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638]`.
3. reset_process_fonts_v119 visits slots in order0,2,3, skips empty receipts and explicitly skips slot1 as level-owned. Slot0/2 receipts and borrows pin Front Impl plus the corresponding movie; slot3 receipt pins its HUD movie and the borrow requires a loaded gameplay HUD. Before delivery it requires nonnull actual/lease, receipt.actual_owner and equality between receipt.identity and the actual SwfMovie pointer. Both receipt and borrow lease stay in scope during reset. There is no control-block/generation equality check or atomic all-slot snapshot in this function. `port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc:119 [sha256=21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d]`; `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:1418 [sha256=3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638]`; `port/android-native/app/src/main/cpp/original_ui_session.cpp:638 [sha256=d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18]`.
4. SwfMovie::source_reset_fonts_v119 pins its Impl, requires a loaded root and enters the same core Scope with menu reentry enabled. It recursively sets actual edit fields to empty text, then conditionally finds the registered platform for owner->player and resets it. If for_player returns no platform, that branch is skipped and owner->finish determines the result. `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:369 [sha256=0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8]`.
5. The platform retains resource/service owners. Front/HUD image-release closures address their actual GPU. Separate MoviePlatformLease services retain their resource Impl through platform_owner; the remove_image closure routes to that resource GPU. `port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc:4 [sha256=d6fa672f4b1b8de09dc8df6519bb8e3c5bf170cea82e0ca4eb47698b3fb2fa2f]`; `port/engine-ui/swf_text_font_platform_v1.cpp:136 [sha256=30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8]`.
6. Platform reset first calls release_image_v119 for each captured image, then TextRenderOwner clear invokes its optional backend hook and clears face/image/clone maps, then platform device/projected/font/core/pixel/image registries are cleared. GPU removal rejects an active display, missing/white/mismatched image; it uses the same texture identity, calls the checked removal path and erases only after successful release. Release uses context-generation-aware GL deletion and budget retirement. `port/engine-ui/swf_text_font_platform_v1.cpp:217 [sha256=30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8]`; `port/engine-ui/text_render_owner_v2.cpp:126 [sha256=6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140]`; `port/android-native/app/src/main/cpp/swf_gpu.cpp:378 [sha256=140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd]`.

### Native comparison and open obligations

Native cross-lane **#6843 MultiMenuManager::ResetFonts @0x437a38** checks four primary slots and calls **#25099 RenderFX::ClearFonts @0x7aac54 with a null context** for each positive slot. Assembly corroborates this ABI point: slot load at0x437a44, MOV R0,#0 at0x437a48, BL ClearFonts at0x437a58, four-iteration counter and4-byte slot advance. #25099 selects the default context, visits its registered players, snapshots each player's character list through #25042/#25041 before empty-text writes, then clears FT and bitmap face tables and resets both caches. #25064 ClearGlyphTextureCaches separately resets the two caches; #24623 resets atlas epochs, region allocation and backing data. Native bodies/hashes and source edges are in the affected CSV rows.

The supported process0/2/3 path does not establish that its independent per-player contexts equal the original default context's full player union, or that repeated native ClearFonts calls and current per-slot calls have equivalent multiplicity/order. Current recursive clearing interleaves discovery with field callbacks; the native list is collected before the clearing loop. Reentrant mutations and external retained field/font/image pins remain unverified.

Slot1 has a separate level/prefix owner: NativeMenuPrefixProviderV62 resolves manager/world/application, borrows a real panel movie by its source identity and has an all-four-slot reset method. This is a separate source route, not proof that the process option callback also resets slot1 or that full Language reset chooses/closes both routes. `port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc:42 [sha256=55a921153c713f4ce640a1257307ad42fe240aea2adc7789a543a64fb70f1eed]`.

Failure is incremental: fields may already be blanked, earlier images released or earlier slots reset when a later release/borrow fails. There is no all-slot or image-release rollback in these functions. Retrying after partial image release, restoring projections, stale retained pins, generation/rebind behavior and exact original failure policy remain open.

The manager destructor releases its Flash/menu transport and logs rejection; it does not explicitly unbind the stored font-reset callback. Front reset_failed clears movies/font owners/images but does not assign/clear process_font_cache_reset_v119. The callback's weak lock prevents using an expired manager, but supported ownership does not prove every teardown/reset/rebind ordering. `port/android-native/app/src/main/cpp/native_character_menu_v4.inc:69 [sha256=d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557]`; `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:1403 [sha256=3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638]`.

Disposition: #25099 remains partial; F5 remains partial; #25064 remains unclear. Other affected rows retain their prior dispositions and not_run. Only source-interface/owner/reset evidence and current fingerprints were added. No full Language closure, rollback, live renderer acceptance or whole-lane semantic completion follows.

## Identical text-body groups

Hashes cover the exact pseudocode suffix starting at its first opening brace. Every member is listed and remains its own CSV row. Equal text, including no-op bodies, does not prove source/runtime equivalence. Explicit record comparisons may supersede clone.

| Body SHA-256 | Canonical | All members |
|---|---:|---|
| `4fdb695b2bbc4e19a82e9e4d080eb6cd035ca837ace0b19b07b30a2a6b6b754d` | 24270 | 24270, 24612, 25555 |
| `bf5d08cd0cff7c33e6d23b52e500a862f61ae235e42f4f157ab3272d03926a9f` | 24277 | 24277, 24280, 24868, 24870, 24887, 25038, 25415, 25416, 25417, 25418, 25419, 25420, 25534, 25563, 25691, 25727, 25752, 25894 |
| `ff3ef80fdb9311a50e06c0dbd322988610f609b715b6ac63d8fe8583319104c0` | 24287 | 24287, 24289, 24615, 25209, 25394, 25403, 25404, 25405, 25406, 25407, 25408, 25451, 25538, 25646, 25702 |
| `ba39aadd2f225734d0b6dc14b54604a09884590e96917775e7bee4d06e4102aa` | 24290 | 24290, 24425 |
| `b869248da588e1540ade19e7e050b3597c72f8c84c8adab62df99063fcaf28b5` | 24340 | 24340, 24346 |
| `59ee5f987ed8d51e0297c178b3a7fd2d3f3b22e86071294581f9c8343603d01a` | 24348 | 24348, 24349 |
| `398bc1efa18b79493b6a7b48e5f297c3e372c6daba14404c5fd13473ad63bd39` | 24352 | 24352, 24621 |
| `f59c11ae5dbf5176d12967fdfac337fb26c8ef4154abfc520ded7b16738235db` | 24366 | 24366, 24367 |
| `997da6a1e2743c35e65bfd4e33fb141f4ca2d7b09962ae19399de7e27fc6e9b0` | 24370 | 24370, 24371, 24372, 24373, 24375, 24383, 24385, 24386, 24388, 24389, 24462, 24770, 24913, 24996, 25150, 25151, 25152, 25153, 25154, 25155, 25156, 25157, 25158, 25159, 25160, 25161, 25162, 25342, 25343, 25363, 25364, 25365, 25366, 25390, 25685, 25686, 25768, 25769, 25770, 25771, 25772, 25774, 25775, 25778, 25787, 25789, 25792, 25793, 25984, 25985, 25994, 25995, 26003, 26021, 26024, 26045, 26046, 26054, 26055, 26056, 26058, 26087, 26088 |
| `ab967fe193ca1dc0b0672132393250f6918cefd9f39dd12cb1fdb2ad9fa81afa` | 24376 | 24376, 24379, 24380, 24381, 24382, 24390, 24391, 24393, 25276, 25277, 25278, 25318, 25344, 25357, 25359, 25379, 25719, 25773, 25788, 25790, 26057 |
| `2895a6e3acf3365f5d633115e73f4b19e4e5d5b48d02c088e0c304b32e09d50b` | 24401 | 24401, 24404 |
| `93cd264647c2955dbebbb27bb8198b4fa66a1926e133bc6accb5db6f130da166` | 24407 | 24407, 24408 |
| `ff7f644e79daae9c62c55b1748de79426eb2a78f19f4e44ad60a97c672caa453` | 24426 | 24426, 24427 |
| `b4ecc595933552713ce50be6f47306fd6019a87a64cab2e369d49ece7b09f534` | 24428 | 24428, 24431 |
| `13e9a15596d13aa45e80626c1c21655ebcef67e34b2cfac733b8a64b651c9c75` | 24433 | 24433, 24434 |
| `9d0a7ed1ba19cee8f4588b302193312eb2805d1431ed42a6503c32279beccabc` | 24438 | 24438, 24439 |
| `1b053c2525d321e0b7c195bf5e0d553d383c937533a302ce67496962ca302eb1` | 24446 | 24446, 24451 |
| `8ec848f80e9349add19d6e1555fcf08c16667ce628ec3a423e1fcb2cd9be589c` | 24447 | 24447, 24450 |
| `7b2a419fc56b8d54103bb834fcd69fd9384ef59f31c45cb15cd1d44813ea8e00` | 24448 | 24448, 24449 |
| `1a1fa2d6b0083c2ce9b3d387e801e5160ee365b743961c5c7835a886723dd4a4` | 24464 | 24464, 25025, 25395, 25398, 25540, 25700, 25808, 25811 |
| `a1f5fc9f80e8e513ec84121d51a39ceb1dc13e5ce776e9eb1c0c1e09f9fc53fd` | 24467 | 24467, 24469, 24531, 24883, 25399, 25809 |
| `c6a82c3c3f6f671bb7e864a95e2229a8bc343b1e9e764ac4bf414b3087308ccb` | 24472 | 24472, 24473 |
| `5f60a0e7f8c290c68850f43e7a44b6e887575d0ffe6e7131f2e2d28d988c5fe9` | 24505 | 24505, 24506 |
| `2709969d9307515a5573c3c92c07da1b46308df3e334011036c094da5003b3c3` | 24517 | 24517, 24520 |
| `6984db8391cca3190d5c0188cb50d80b3e56eb7127aeabaf34ab21cf33d4312c` | 24519 | 24519, 25597 |
| `c8c8b40c182574231f90a14521be1bb79937eba39a682f541d889f41e14c53ad` | 24532 | 24532, 25452 |
| `4cd955c23fe343eb7af8c8f9432d973dd18753f0304d2550e6d2a50d3d434fa0` | 24565 | 24565, 24567 |
| `c4516ea7b355b07b3d814ad73ab1d5bdb54962e810a9463a1bb0e6fdbf7b2318` | 24568 | 24568, 24570 |
| `f5e77c48a19ade98753fa6dce81e6f29c16b29dafe708aa86d8afa7ddcaaf10f` | 24577 | 24577, 24579 |
| `e971f2b66a70575ae7cdf71fad01d9394f60f90dd30916d887e2d8382d678a18` | 24580 | 24580, 24581 |
| `6a319d1320ce10ef094c2582f29b637d83fa1e0566365ec02123b37b5969f35a` | 24582 | 24582, 24584 |
| `350ced4fbecbf4a9652e6feb098a56866a2d474cc95356105439fc4558ad0158` | 24602 | 24602, 24604 |
| `97e309808d70a20139e5081f2fea266af7e7548458d9c165be4768fb2ffe4c74` | 24614 | 24614, 25558, 25690 |
| `63603841f0fc4da344e4e3faf5edf924f4d3a7bdc149cfffe6b380c28da1f701` | 24624 | 24624, 24625 |
| `878025db22ff630c274a25b29fdb2e8b87a3ddc4b393f7f51019397ad6f8d925` | 24666 | 24666, 25447 |
| `82e81f333d8c9cfa7ccd5b42d9cca630b0b4c5266d787166555010888e5adba3` | 24668 | 24668, 24670 |
| `988ba0e8cb8d6101fe8765c7e0842c18f48c501162f9073991e7aa9f749a895c` | 24671 | 24671, 24672 |
| `62c5e750019de99806384c4d4e557eca4039282992401a472c7f4d0391da20a7` | 24685 | 24685, 24686 |
| `601cec7f93beb437cf06175f253ddf5497944786ab199147be548720765f42b2` | 24688 | 24688, 24893, 24980 |
| `54f9c521d76b10cbe44451b5aea5f59e9ba3d878b593fa352849c87e00fde99d` | 24712 | 24712, 24713 |
| `643a13debd0ccfd45318d3c909a58b0baa24a423bdde00ae968494fedac3e52d` | 24739 | 24739, 25463 |
| `e6dfee124be0dcde8c2a16173d36bb2cbc879978351b18968e2735c224aa313b` | 24782 | 24782, 24783 |
| `065c878d3bc20ddb288f37bc194ca62993a7a69369e22124d097c9d09f4a4f01` | 24797 | 24797, 24799 |
| `814d88f34ebb358e3600ceda990f7da278984e84ba0c6fbb1a5f456d57c2c5a4` | 24818 | 24818, 24820 |
| `6029da7528ba319bb5294307e1486a7b6d89491f07e6a1d53e6cbf3d7d26e0da` | 24835 | 24835, 24836 |
| `09747b42f6f4cfa06a1a0ac1debf6606af07329777f556d159f57e754251ab64` | 24874 | 24874, 24877 |
| `6d44faaa07d3f687e9fa58ed8e001f14758d681575c8e102b915986c0fedb31d` | 24884 | 24884, 25474 |
| `7d80fc3fe9e9ab650af0b85b48462f88f8d90e95ed4ae212be4f9991695174d2` | 24894 | 24894, 25907 |
| `1582bec4c054cc0a35824043db17540a675d5dfaba15685efffd4a53397acde4` | 24895 | 24895, 24898 |
| `e990459ca6442e881114eb69ed317b2543afd2e95f3dc37e94720128a25bd41d` | 24896 | 24896, 24897 |
| `00edc307c9cce103d8446ed3f8a1a0dd6d50db7a273907ad95e51f7d79114e45` | 24908 | 24908, 24909 |
| `25ead2a77e8cdc0674d2b7e84076c7f1913d15db545ad97c96cca6f57bc2c769` | 24927 | 24927, 24931 |
| `bdef1babc521f1d64cd8febdfc92418a777f3a62b3f37cd9ee077c9eafcac156` | 24933 | 24933, 24934 |
| `07086046e2e6e4fbaba640fde3f727327b0a7e701c0dcfa66be0b389968f73a7` | 24971 | 24971, 24972 |
| `51f1824b9394143ef2eb9949ac3720ee2858c1939595dccc7d0170f167d33753` | 24981 | 24981, 24983 |
| `41584778cb7578418d7cc3fe780bac7994f96547b56881ad0012fed11c00f40a` | 24984 | 24984, 24985 |
| `5724b6036c3c451e33ba9275a54026f38b647b2646ce0e2497e774ff6c45dc64` | 24993 | 24993, 24995 |
| `fef69ac915afd588ff52a18a76aa758ba6905295d4feff9c069b45d72699cd70` | 25016 | 25016, 25380 |
| `a9bf28688867a78f4a9a391f64fed8f6831961f036ef5bb659d642d38f424d98` | 25022 | 25022, 25024, 25720 |
| `33156cfd6e3fb45d104c81f158fe385ce266e2f680f41df4214a4af6cddec05a` | 25030 | 25030, 25461, 25733 |
| `d5584cc651b3a14f6ebd028cd9f8841aebb48306b4459b8d5086d399119d6db8` | 25034 | 25034, 25035 |
| `9cb24d7fe7d93074fcb244efb774bccf994d72c87afff41e5866ad2356a99c92` | 25036 | 25036, 25037 |
| `a098630c1163c4049e2aff5430966f49e44f34a0172e7af49b9815bbe92faa94` | 25060 | 25060, 25062 |
| `b3f240d0802a276af337917de3595e1fc62f68a88a58bf11a5a71714d5db517a` | 25061 | 25061, 25063 |
| `b0e1e4fa8cf6777077fc94aa1c42294f45d7e022a120742b0d74d7cda199aebb` | 25092 | 25092, 25897 |
| `4c17ab546771c3b42f5e9ebcfd3fb2a73ba05f296d4c016b0dbb9e185b6d4efa` | 25094 | 25094, 25095 |
| `468250ffe75044e6130fd954e97a1666930d75fac1f7608e987d70bc7d0cfc68` | 25103 | 25103, 25105 |
| `e524f596a42bb007f29c3e8d388411d52e314e7289b1f3ccde9af4489b74abee` | 25113 | 25113, 25117 |
| `e9aa74820e557ec601a57cc97c326106914550893ca4ed8aeb8929fc5ee0b836` | 25120 | 25120, 25122 |
| `4755ceb871e3bbc773f4704c76b86824b65fddc3b5e27116ad270457faa65d48` | 25185 | 25185, 25192 |
| `1f597bc43a24a9c8059e19b8dbe30118f866d20bd70a25545a431a525d7c1d52` | 25205 | 25205, 25396, 25401, 25450, 25606, 25701 |
| `5a4bcc24f036da41c310bf43fa6a1a6ad9af239e69e48c58f870fb1eee25c67e` | 25207 | 25207, 25810 |
| `e05fac1bcf1531c642aca35730d619ffb17d43b04e96bbf2d3188fcb95e21dda` | 25239 | 25239, 25240 |
| `a459609a5626127a3e0bd532bf7cd44502bd5ea1a6c751b31338fb3294ea5739` | 25241 | 25241, 25242 |
| `22dc7f83b1244c1d8058e96cab05fa6d07d013ca34b2d6ebcfd50b61b3fcfef9` | 25248 | 25248, 25249 |
| `48e071a088eb0a056e5a59cd9ef5c374b4a9fbf968ced73b41bfd66da0ae9add` | 25250 | 25250, 25251 |
| `1a1dea1327e322347c83ec4754aacce3c3c398fe23609b73e499d55af4629d97` | 25252 | 25252, 25253 |
| `b7c7bb2944cdc8b7de8efeb8b62b8cd51637ed3679bd6d8116db1009f4bdddd1` | 25254 | 25254, 25255 |
| `bb9b9dd32e5f6cc817c5f5c54b2fae725630eb657c8324d84e5bd5509d13d49f` | 25256 | 25256, 25257 |
| `5f1b2e6bcedd5bf24b59639b54c7e5d037ebdc6faec1d0a3f84250b991e45127` | 25258 | 25258, 25259 |
| `1c5ce455341c2cff5d2ac1a8448ab0976603cc68754c9e57ae91bfe2f85a3a13` | 25260 | 25260, 25261 |
| `1b1f2766596f7765bd1b4b7ba877122e31ddccc302e201339bbc2c76c6d71fbb` | 25280 | 25280, 25281 |
| `6f2a90b2820dc655f0a1bbf7a6803e4296e8115d389e8343cfba3ff8f6ed5232` | 25282 | 25282, 25283 |
| `496a0ec2a08c1b60a787128143132203e4dd09a990802e0897081b4df0e3dcb5` | 25291 | 25291, 25292, 25294, 25295 |
| `b774f0065897a9be7db7bc85050e3307330e331cb294c17087dcae7a4a545efc` | 25296 | 25296, 25297 |
| `0268f6ccb8dffba4d97af9a8640e076b94237443636e3a51fb2b37ac893dfe88` | 25298 | 25298, 25299 |
| `c20383478bf7dbfbb74be7819cdaae5324203403bd4a7632b75907ee7b5c996a` | 25304 | 25304, 25305 |
| `8d71ae70f851ea5b9d230cd056a617b5df0cbdc344046658d2ffe40b99220ab6` | 25306 | 25306, 25307 |
| `6878597f5e6bf52896fe0620f0b2bc10dc2051b1a0ac4ec1bbf2755668802967` | 25308 | 25308, 25309 |
| `e3dd6dc4a75c84fa35edca0649b710a42710e3002eb89c42c76a495acb066337` | 25311 | 25311, 25312 |
| `105fbdeb5492c1c5ce4c7306c14746991cee711ba17be0d8963d673e0756ae93` | 25330 | 25330, 25331 |
| `f9c6d02790e14efac6c4954d524e05484bde4ff8bc9d9dc19470616d52fe2b72` | 25333 | 25333, 25334 |
| `a0ecd3e55ad8dc59c8a0cbebe7a6301223272fbe4b6ab0b202791ccee5cca745` | 25346 | 25346, 25347 |
| `34c0c67df1ae86fd42e5fa93349bd1546f09a957515087c284e30647d428d0bd` | 25349 | 25349, 25350 |
| `4a237a62da25bb8d2f2917552c32abda04b1f4bca7434194d8a61241f0126a57` | 25421 | 25421, 25422 |
| `ae9405a7bd888579c86e2a2b8b02d30e2d78483fe08373ebbcc2eb6671af2221` | 25423 | 25423, 25424 |
| `0f474a9b74f782e9e6de3fca634d931e344ea18d86b051426fcf618deaaf7688` | 25432 | 25432, 25438 |
| `2dfeaa16a78759fe5f23400faba96b7d6377e468e6c8e864f9d8869f33779908` | 25444 | 25444, 25596 |
| `4fd4bab97b3cb3784a378cce03cba54e43c95b2b4561a621b342565bc1b2a2f2` | 25464 | 25464, 25465 |
| `3f4c8257dbb3da62d06d9f44f8f1c13b1879a46bcb79353347c1e937dd484efe` | 25544 | 25544, 25760 |
| `0fd7b04cc041b933e9b52d3e147058389900e14d5cea6b653f793eec717cae43` | 25546 | 25546, 25548 |
| `a9a1e600cb3fc57182e9f6c6dcb5160cc98aaddad6e7b2f5dd7c88ed0aa9db50` | 25547 | 25547, 25756 |
| `0aeadba47499eef988a875f7b2cf11faf77c19b3b5c76cde6cf7a220c6e9eda2` | 25550 | 25550, 25551 |
| `c30326ba6212ca21a1b417105f546a135189ffc67a2180960f34b7e13fcc1fa1` | 25559 | 25559, 25560 |
| `bff1af070a1f458cb3a83dea125027b92fce7fb08fcf0ac1395956a24f4ddabb` | 25565 | 25565, 25726 |
| `27c8095f652298fa6a154c6db3fce80b091b7adc8a365b05319041a543bebebe` | 25566 | 25566, 25568 |
| `106d5f121c648153041d5f133743aab6c9a0f4e4ec319c30ec62ef5ce1a6be99` | 25567 | 25567, 25587 |
| `34db6f35d8e094ae58f0271c557562464487fade1cad1d47aff417043ca6a9fe` | 25580 | 25580, 25728 |
| `9f462a5dbc75104f31911cac70daa5dd974f000b85e9ff71a357e6f65e259573` | 25581 | 25581, 25729 |
| `2b9c164463dc68a80cb54356d9072f6ef531c75aeb945d5f41e925bdfd4f7446` | 25589 | 25589, 25591 |
| `949fe11613c07c1ee1b4d02527ab2d02c7fd72d5ff0efc9343a636cb22a409d7` | 25592 | 25592, 25593 |
| `987af3ea3caac8144dd3ad74e8f6df6f40766a52f2c3b3a5fb5e6e21e9532b67` | 25611 | 25611, 25613 |
| `fb8d9a89e1581159630ff734d47abcbd06014b8aaeab04d884e12413885b6d82` | 25615 | 25615, 25617 |
| `8d0cd8656bbc4de2800cc0a416f43543bd26d03af37e9dec3ea01f0b940d4ba2` | 25618 | 25618, 25619 |
| `d02210a726530b9345a4a5b7c35b695589a527bb69f672c82c41f73cd2a5b895` | 25635 | 25635, 25637 |
| `f295900c326e66dac55a34f12c9bc95040ac58b6dc3b6d4c45834c7107094b38` | 25636 | 25636, 25639 |
| `710507f150370ffc88bf370333600c3a15b76043bc0847b3c396f3d47ec9c4a2` | 25640 | 25640, 25641 |
| `f710eb172b36f4f6837850f14b8a9206bafd80892e64ae9575c81dacaa8f6b5e` | 25666 | 25666, 25667 |
| `974f9b8a6f577d5f413a678fd59ed61c820fe6e69d4596a8897052bd3eead715` | 25707 | 25707, 25708 |
| `031a97dfe53058a50cb0d1f10482e003caed10e22e4f1c27af608d4bffeb2215` | 25715 | 25715, 25716 |
| `55ce1aaf4c294e577024329d9f8e4153bf6c44834951488fe52b4773d37de862` | 25737 | 25737, 25738 |
| `6fe5e6988cc3a4440aa50a8d0aec5af040df6b3057adea7719937f1cf108cd23` | 25743 | 25743, 25744 |
| `636e45214e467ddff82a3f5ce48e19c72e88aa741dae78c02c36c4a79cbc6b2d` | 25754 | 25754, 25755 |
| `92e1fd2510b71ce30d53ee0bd2d9bb924f45a74880804c588d7d5141f2e39f65` | 25761 | 25761, 25763 |
| `4527c26e019c5e06aa40b92388dfb097d36233fba492180b14fdfdf4f1e63385` | 25764 | 25764, 25765 |
| `83b1ec7cb6ec85c691ff639ebc016947ee2067298c44c1b05952264857e7830c` | 25807 | 25807, 26080, 26081, 26082, 26096, 26097 |
| `2a7f21f1ef09191b2df164629aa582e81963d410757edacd085fe88ebc574cd5` | 25824 | 25824, 25826 |
| `4ae9c560cdbf6741e9c3a7670f80c5d33de25c9a34fac20af9cf0fe20e2e1ea1` | 25825 | 25825, 25842 |
| `695462d65763aa40ed051ed2c863bc3bcbf1cd52c1fa745143f049dab487958b` | 25827 | 25827, 25829 |
| `bd79c70dec88bb8528bbc0f4c01abc90c4362a30c5590c4ccd0671ddb6b96344` | 25828 | 25828, 25845, 25848 |
| `82b8897ee6207ab47ef48a3aea7b4354cfc6c8e82ca7a975a9156fb7527075af` | 25830 | 25830, 25832 |
| `89bab6c4fa52967a1f99f1c0afc50664e425db189a0bdb12971deba9b0bc72cc` | 25837 | 25837, 25847 |
| `a05ae0f810fc7c930b075c914c698311c963859abba351ae9bbbb6df36235696` | 25838 | 25838, 25839 |
| `c3da6d418b168b4ef7ff20ca40b4b0132917d76fe95a266f26e376c4bfdac170` | 25841 | 25841, 25843 |
| `6e8f3b9eb32d18bcbda9a28afccf2f3de8b1918b6da69f90a113a9225cc3307b` | 25844 | 25844, 25846 |
| `5c2bc0e2da28a95af44c02d4d099292ccfadacd98ae89d9ed94ce299a64ae8cd` | 25861 | 25861, 25866 |
| `4bf38bbd1e1f6eb3bdd0caf6ea92cf9f94582f3dedb33d537bd05777f969f1a6` | 25892 | 25892, 25893 |
| `5bdcaffbc760cb6c154fefefe0b807eebdf0b55ac4b927848b347e4c5445807f` | 25906 | 25906, 25915 |
| `418035603e133a34d7cc93ad4649735cf202000a8d3c84e992fc9bfd566e0485` | 25908 | 25908, 25910 |
| `0d4af74ef615b27946c581394197b83a4a34c873cea03a48175e2587dda37c3a` | 25909 | 25909, 25917 |
| `9d73b7fca0009dde6119608538085d466bbf0ad473454a84f28559f5779b3e3d` | 25913 | 25913, 25914 |
| `546011bf0c1ce7e0fa2cfcccff2e509f503a723cc03b68a87536ee7caad04e48` | 25991 | 25991, 25993 |
| `fdb1d2a01c889daf8371dcf73501b3ea540a16bdc1a33e7241984e35abc3f98d` | 26007 | 26007, 26008 |
| `399d1af425a99352bc263c04d74832670c211bc67bed1224fe3cf203238e62e9` | 26014 | 26014, 26015 |
| `a5b68bdd83afb3f430e1c1838d38a8acf2f82a68e537fc02b3f94a527e81f2d5` | 26039 | 26039, 26040 |
| `46d0b8b83ff9911f7edd584cad5b50bc314a46b13e2ea86674425ab20e72460e` | 26043 | 26043, 26044 |
| `da031c452aabebfdd62b997e4b09829001f3f4a87529487377d997989fd63dc8` | 26078 | 26078, 26079 |
| `89579cd31eababaa03cd152561c00d0ec757e01271c89224529aad23f2489f20` | 26083 | 26083, 26084 |
| `571b07715c649a5dea28b9e4b90590621ab020c74b848d89ebdeaf205cafa909` | 26090 | 26090, 26091 |
| `845e5d5b462a4ce33485d2d7c999263869c21be02b1d6d3d9032b9db00aed033` | 26100 | 26100, 26101 |
| `dcb873b8658692f2575a0b90f0ba1363bfa6fb6d06927e571076b7aa918aad6a` | 26103 | 26103, 26104 |
| `eea347a368a8e7cd585022442d326f0665a53fc8f5dd03092624020a559ed62f` | 26113 | 26113, 26114 |
| `49e420ab88e81dafbc37631511e2192957c8dd76f67c0dbb9a147cd25a4bc528` | 26121 | 26121, 26122 |

## Current source hash manifest

Paths are relative to repository root. CSV records include positions. Historical before/current fingerprints remain in the pause/S8 sections. This manifest uses current complete file bytes.

| Source file | SHA-256 |
|---|---|
| `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` | `3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638` |
| `port/android-native/app/src/main/cpp/front_ui_session_v87.hpp` | `11623552099bc57c48518dd7203cde649ff4b960a1b1b487cd9bb15e1fd93f03` |
| `port/android-native/app/src/main/cpp/native_character_menu_v4.inc` | `d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557` |
| `port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc` | `21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d` |
| `port/android-native/app/src/main/cpp/native_menu_resources_v93.inc` | `c4ed613435541f69ef9d668c926e4713ae3e62dcd757888a90529d200581a57e` |
| `port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc` | `55a921153c713f4ce640a1257307ad42fe240aea2adc7789a543a64fb70f1eed` |
| `port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc` | `d6fa672f4b1b8de09dc8df6519bb8e3c5bf170cea82e0ca4eb47698b3fb2fa2f` |
| `port/android-native/app/src/main/cpp/original_ui_session.cpp` | `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18` |
| `port/android-native/app/src/main/cpp/swf_gpu.cpp` | `140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd` |
| `port/android-native/app/src/main/cpp/swf_gpu.hpp` | `ee1afe07ce0b08ab129accbe3013e34b1e93e69857ffb30faeedcb2532393dce` |
| `port/engine-ui/authored_character_menu_session_v1.cpp` | `cb0956b6acf1a350d157acb4e0717c3505ee97ca6baf888d0dbd453490e3e839` |
| `port/engine-ui/authored_menu_localization_v1.cpp` | `632e6e2e098a27da06b4b7e2a851e8f5ee897d69307f3dd01357930aaf3268f2` |
| `port/engine-ui/authored_shared_menu_roster_v27.cpp` | `663dbf677be1de28ec545ef9a7cc7bb3f67e52f5a242a2b1efc143125c72e302` |
| `port/engine-ui/edit_text_field_v1.cpp` | `9cc385100bf673491aade151e2b35e99e428022d824c37be8c7f34403d831f9a` |
| `port/engine-ui/gfnt_text_backend_v1.cpp` | `87da563d2b7a36d7afff3461e973b20059b0250cc369f4cbf99647ae381e3e9a` |
| `port/engine-ui/gfnt_text_backend_v1.hpp` | `4e9425ce9e4133c8f7c4a3df9d023692ad25b4232b08438f296ee4b164e6034e` |
| `port/engine-ui/menu_stack_owner_v1.cpp` | `6b14bd7632bfc32ca47ef68359efeadf20c32092235b48431454a4ba8ceea008` |
| `port/engine-ui/menu_stack_v1.cpp` | `8c68bb6f8ebcb0c5bc33bc1373e656d9d14165521fbf922337e3766d6c7c8f2d` |
| `port/engine-ui/overlays/edit-text-v1/gameswf/gameswf_text.h` | `b42ac00a9af1e748592344f97b41633eff8ad5f650f7a5e86368fde68f69e72f` |
| `port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp` | `5c0c14bfaf8504e36284316294f53d2badf444a30d804fbc13951620f2fc55f0` |
| `port/engine-ui/overlays/font-v1/gameswf_font.cpp` | `c587ec54baf8a2b68d747ad8f5c8039334df27d6c55f764fc30a8137f2053d1d` |
| `port/engine-ui/overlays/frame-v1/gameswf_button.cpp` | `dbd2725c9058f828b8c3e65bbb27f83ed7666652dba8101c175007c05d12381d` |
| `port/engine-ui/overlays/frame-v1/gameswf_sprite.cpp` | `5ba3c07c2f106a685ebf43294f17036e885ed96b3cd7f4956ba838c59104250a` |
| `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp` | `0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8` |
| `port/engine-ui/renderfx_text_connection.cpp` | `d59f9d9a4b644483ab1152c868815be353fb246c70d03aeab264e56279d2e126` |
| `port/engine-ui/swf_actionscript_connection.cpp` | `c3e873f984d9033fd9950a8963738110b6cd07e0cc6db34cbc6c39aeea70c9ee` |
| `port/engine-ui/swf_cursor_input.cpp` | `921210ebadefa716cc95a63d7fea2d428652dee7317dee7078aebd6cd09c4dfc` |
| `port/engine-ui/swf_edit_text_connection_v1.cpp` | `a3f6b534c0cffa163407351c93c3250122060f2e7d65360e866f5ba2972253c5` |
| `port/engine-ui/swf_event_dispatch.cpp` | `48fd9df725c52d79dcfb4a49f1bf301b7afb3bafef97b66dce2fcc4f6a77ecee` |
| `port/engine-ui/swf_frame_connection.cpp` | `9f3ddea3d3321f0dfc3718d3f1e670558c84a6bb99875adb2f6f9556c7664837` |
| `port/engine-ui/swf_frame_schedule.cpp` | `2401099b82e8ba5e5f093d5a33690cbe3258363081b74730dce41c13f3d6bc59` |
| `port/engine-ui/swf_input_connection.cpp` | `58ca56695d9eb330d6a2d89f2e165d194459ac79af60116f65278245215d484d` |
| `port/engine-ui/swf_menu_unload_v91.hpp` | `1ccec757156ca4dc0fe37b66b47aa0bdde79ead31416fddda6308f308c9063aa` |
| `port/engine-ui/swf_movie.hpp` | `5a1d3684ebb49e610118e5aba42c432e475cc6b9fe3b13888fdd49d3a3513c5a` |
| `port/engine-ui/swf_renderer_wireframe_v62.hpp` | `eaf2383f15d7314f3ac2971c7f674242aa2a8fa5c5870845f54995d773ba0f60` |
| `port/engine-ui/swf_text_font_platform_v1.cpp` | `30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8` |
| `port/engine-ui/swf_text_font_platform_v1.hpp` | `611c1327217e41f618b78b7aed57b7603109fbe751a4a9f0e897a9a89c8637f0` |
| `port/engine-ui/swf_viewport_connection.cpp` | `7f5278e000a91a79088be113a3b6f79354a22bc2845be2cbeab922ad17a2b813` |
| `port/engine-ui/text_display_v2.cpp` | `6b2d1c5693d3985b4724b2ca4f67d8e420262dd925be44482c0774b4214ebaa1` |
| `port/engine-ui/text_filter_v1.cpp` | `6ffb1fb2a75abe79b75c6b9d5ca252b2568819a5be2323e6f3c829861154a332` |
| `port/engine-ui/text_layout_v1.cpp` | `5f4aaeb895f6529a7681ccebe7a0df86ef05e9d9e18527ac3d347c48e72b8316` |
| `port/engine-ui/text_render_owner_v2.cpp` | `6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140` |
| `port/engine-ui/text_render_owner_v2.hpp` | `4e72399c7fe8fee28f60216060a3ef6549b0b85cc41a04945caca93fcfb3db8a` |
| `port/engine-ui/vendor/gameswf1714/base/configvars.cpp` | `f14f5428cdde59692fb25307b367c9a87b929246fb861651fc265950ce5f8bf2` |
| `port/engine-ui/vendor/gameswf1714/base/container.cpp` | `1c3e1197f39462e2c83b01bd0e87085146ffd7b588292b38bacccc680780b639` |
| `port/engine-ui/vendor/gameswf1714/base/container.h` | `b6a1bda8445842823bfc2f52d1160f732a9a08b9026b19407b685192b9178b33` |
| `port/engine-ui/vendor/gameswf1714/base/ear_clip_triangulate_float.cpp` | `55b5dbc065169426ce8ae6d8c18d1f49a2a94c67fbd072ee36f08cac7131d24f` |
| `port/engine-ui/vendor/gameswf1714/base/ear_clip_triangulate_impl.h` | `102e47fa610862534411a357bc54a6d5352c4fbbcca7874e2523f77512315ba4` |
| `port/engine-ui/vendor/gameswf1714/base/ear_clip_triangulate_sint16.cpp` | `447b8ca8e01e4e3e1286a8dea44bd0279a0e1e5ab70065512bd65e43fc1f1e68` |
| `port/engine-ui/vendor/gameswf1714/base/grid_index.h` | `444f95cf391c9358c5e6eda41a220b15ca4f1f816fc1d291c74b26bee265ca58` |
| `port/engine-ui/vendor/gameswf1714/base/image.cpp` | `1792c5afa7ae101351d3d376198293be759649ca669ffa911631767c44d1496a` |
| `port/engine-ui/vendor/gameswf1714/base/jpeg.cpp` | `1d3ba9f2b2b75108f5b24bcee0da823cadf7f1b2471e81cf033229474a686ec3` |
| `port/engine-ui/vendor/gameswf1714/base/membuf.cpp` | `728dcd9d163bba1d969ee21f1328693720d6dcb6b65d09abcba51f3805d00395` |
| `port/engine-ui/vendor/gameswf1714/base/smart_ptr.h` | `5f5f23ea12fa5bbe15695ebba88a1157491b846a4b94fb7dd8fc720999a4730a` |
| `port/engine-ui/vendor/gameswf1714/base/tu_file.cpp` | `ef437d2f1c64c1ae06d3d31dc5b064440539c5f35663b4846fb9404c63c0c8b6` |
| `port/engine-ui/vendor/gameswf1714/base/tu_gc_singlethreaded_refcount.h` | `e220635d4ab333ea37a3f624c598dc58cd1f4d43a4311712da431570e49bf723` |
| `port/engine-ui/vendor/gameswf1714/base/tu_loadlib.cpp` | `eb4ee9f9b9a4e885e24cf8f6ffa2f870d6565ef43e13dd975e223d175403edb7` |
| `port/engine-ui/vendor/gameswf1714/base/tu_random.cpp` | `e03b604c2d1b3ecb7a3951ad5d06a190a55edc1fba5031f1e32984de351f5345` |
| `port/engine-ui/vendor/gameswf1714/base/tu_timer.cpp` | `35a2f641a6fcac06049055c19a3602b18049ff4531d6e771228302a13653b449` |
| `port/engine-ui/vendor/gameswf1714/base/utility.h` | `9fbaacfc6e7d7da6ec742ffc5c6fbdfaaf6100bf644c8f9189211992edbfedba` |
| `port/engine-ui/vendor/gameswf1714/base/weak_ptr.h` | `8e316940123f0fee2e26b22a334bc3c8e113196c32d026f3cfdb6c8ad357f4db` |
| `port/engine-ui/vendor/gameswf1714/base/zlib_adapter.cpp` | `38645cc90857938d674925a6dc2020f282d6995c887e8a30ffba69a0cfd0f850` |
| `port/engine-ui/vendor/gameswf1714/gameswf/extensions/lib3ds/gameswf_3ds_inst.cpp` | `1f343dcbfd14fb0e18d6613e066c7d19eb49c6e1f6487062bfb1a661cd50e912` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf.h` | `42d46d43f83f870d276ad822f76dc21f1677809fcf46d02bb2f36ebef9d9d182` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_abc.cpp` | `4de2dfba28ec03638644f7221a9e49baaa736016ac14b75f8d4168c71db46292` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_action.cpp` | `5d9581fb1fbfe0b1c648e3a91315f16bf407f52fd49a066b14ad7ab76d9b8665` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_action.h` | `28706ebd4426f2d061633bb145109b6362d8497ab7b46bfc0d2cf760d5b70ec9` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_array.cpp` | `4d7204c54c9a8848ff3b1f9c97cecd6faecca386ee10bc786e3e7eff335af0d4` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_boolean.cpp` | `31135daea21d9ea9879717a9e89f6eaa01c9ad8c58ddae14256c506e3b01a1a1` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_broadcaster.cpp` | `e84304c3290cbe19231b9114793857c2a49769c3cf1d6b50d5b926c0c22a101c` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_color.cpp` | `221961087a650059de8642fbc89a376075e431bb46d7a7daa8b85e3096ed3d4f` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_color_transform.cpp` | `83d28d19107cee3509bcb6f1bd988aee0c8d9eb01ce9818c8a7912bcb750095f` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_date.cpp` | `f23efa1dd22eff9b311f758a9ec63b165e210b906b094f14339a49e7d5f6f1de` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_event.cpp` | `486c664c1b1ab6fee1142c062ab6cafba2b1ce85fce4ff6ffaa040b1ec55866d` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_flash.cpp` | `c450ce4b6a1804eceaf564c926730661c9472677941ff5e539f90201cc3bfa48` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_geom.cpp` | `7a7f2b8907d65d58052bde665ad8dcbba1454b16131878cad697201914e0c309` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_global.cpp` | `25e41a9d1ccec05d7ac8fef7c7fd26200787fe02f335de0c5b2fe50b9f5eac49` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_key.cpp` | `f06cc47c0a98ef2656ee521892734f890ff245ffd0ae8432d768ff5432daba64` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_math.cpp` | `ab9ef95334a045269d01c4e11837db93fbaa4dbc8f9bd84a2f05d6ef538a3b26` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_matrix.cpp` | `1ec8a22bd1a9d370ba79a180fca00ecec8968ede94ecf90580e1dccb269b5f6a` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_mcloader.cpp` | `c88592f63f03ee324636bfc6d82e9b0b57b1fa27da17e6a94f6dd86894a62f53` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_mouse_event.cpp` | `df11262eeda3c55d8c81170e6681b30a650f0e0fd3223fca0a8f0bc17a56c080` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_netconnection.cpp` | `256b74da3f94f2a0c133f14780f1a74a5b80bb47e250b01c0bc0cb5dab437a0e` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_netstream.cpp` | `248613514cb520d646ed3e5e65a7e11f510b5e38dd97a06522b63f6090a35c18` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_number.cpp` | `9c1d47b46e953d22f1264982a71f830e299d40051cae00d7cf9a572dd0e9819f` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_point.cpp` | `57bb4c5daddb66115add1b63d75619775e7858430310ea91ef2a41ede43f36e8` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_selection.cpp` | `1bdcf2d5e8ca3ec724bf99c399f5fa323200ad005aecb6c11136fb167f750c28` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_sound.cpp` | `4098afe697400257ac1f5ad8f3378edb4eda660a164588ea43f0533ea3ca8285` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_string.cpp` | `4952c8bdb91e3b3191a51ec3acb2b65fe58b23c2b800b7581f2ab1f9646e732a` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_textformat.cpp` | `81277faaae2f38c35bc4fdc4b50f9d4cf77cb6543a3d2f56028b701d3c249613` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_classes/as_transform.cpp` | `10521aa15186f12d25db7c5e17fd412553e538f6b4a341e90f5cc8f6588895a0` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_as_sprite.cpp` | `3c47380122f32be1d415a2c4909a0e0eb6c4b7738839131c6700d7c7d764b2c3` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_avm2.cpp` | `2841ee351ebfc30eb06e4de87cf88dc3eeb95f0d54e13a869cb9b9386cf84fd1` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_button.cpp` | `545a4b3738bff4296f239f88294dfaa9b760fbde5eb0133bb84c716ee603fed4` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_canvas.cpp` | `55887b8c20074095390639c90adc1b5412d25a8373b2b094e72361ca3d7fd659` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_disasm.cpp` | `da41b1d087f9c8b24653d2de43b550f365160d96f14764bef314558f8fd916af` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_dlist.cpp` | `b79a8e3ae160549f32ba815dc3835ef70dcc4f48207b530719b69e016d6ca13e` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_environment.cpp` | `fa16307cdb23750edce31a526b4ef018b5283f9b9a8c17f429c7a4ca61155268` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_environment.h` | `a40f8da1cc24deff43728e3b66859a580f92fe1d8a18b38120c904f00a10f424` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_fontlib.cpp` | `f7456fbb7890a1a510c0d8c90709bb97188e764730c569fa8955d9de518b2ad5` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_freetype.cpp` | `71c1adea62c24d1575bbae49309ca97032acac26f79b58997417f4ae2ebb8ec4` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_function.cpp` | `ee06a9e066268b580e51d043710b0cc9873ea7e2435f77c80a2afd4cbd38bcef` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_function.h` | `282bb890be505bd86c5c771d2d8ec1b0ef31757fc8f69396821472dbbecd2e27` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_impl.cpp` | `b33c4b45d26be047919c7c69005d2847231f036b0b4beb1222165af25b193168` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_object.cpp` | `3fd7aa70729a54d6656658d037c36bc45e30e00a8ef82101bc647085e9715ea5` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_render.cpp` | `1cd22c4613d93804ba8ee731191ee26e231931b777eef4c1f46475e389ae1795` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_render_handler_d3d.cpp` | `a69a143be3692f4b5e459115e8b39f777e70611db2fd135f20380534d154d942` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_render_handler_ogl.cpp` | `5e3995cec70b6aa5393a67bfe7026619904db84b42456e97829f5bd1d829326e` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_root.cpp` | `d009ae1a49afeda3a19d05d5544bd92ee5e3d1ff5c263fb2dbf1bb898ff82392` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_shape.cpp` | `3f1ca1cd216e40ceb76d6a3d3b0cba48263d3910791fc6f44a6d8dab046c5e70` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite.cpp` | `8f5fd5b128661b51cc449c7784a32b9379dbca16af3de876cb992a3910631a06` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite.h` | `9dcd137626d10bf56c202f2a4c9efa59d52d01ddc5186cae480ab5e3230c8053` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite_def.cpp` | `754ec8d127db2aab343473a067edaf0d4f0461afe980708a59c99d817f2918f9` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_stream.cpp` | `39953c40ce3c833ea18e7b82cb182fb8bd6a1f493bc7a30a64953aec2b0bcdff` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_stream.h` | `e553802ad432cd299c6c365ea97cbe1e921c2a52d59798e2e3d2b309e049c717` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_styles.cpp` | `e6a6a6065b0b09c77956726c6d88482574b88277ccc1f7ca6129d16833747be4` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_styles.h` | `a695a19c17eac781fea758f7399ab633a76118ec5192203a6018e7c10e70297b` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_tesselate.cpp` | `6211e78037c6d150f45db87ec7a2693737a78bb2ebd487a8777af89c6457b809` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_text.cpp` | `a379c22eef14441decfbbcb29250f68c1ac640c7e93bf6cd751f8dc0e0dbb1fa` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_types.cpp` | `f5f6b21e42eb2030837670b42137a5599d0bf2333ec7e6670c37f7ede648d302` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_value.cpp` | `9b095b249baf367533f2bdc34c7450edd386f62d05da2191a630a122f2f09bc4` |
| `port/engine-ui/vendor/gameswf1714/gameswf/gameswf_value.h` | `08133bad2dca3cc3d2341794add31013e22b1cf3fe62d8fd37e198be4888a51a` |
| `port/level-world/CMakeLists.txt` | `78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608` |
| `port/level-world/physical_world.cpp` | `104dd9049d1ca95ed2bfdcfb6a83748d0a34f9e6447c0c1ec015d26bcc5c0ba4` |
| `port/physics-backend/CMakeLists.txt` | `8e7041f90cf62cc78a52085cac6ddcceface17825916f493ff836561616d0d10` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/Shapes/b2CircleShape.cpp` | `0629b27c6fd842dea90070e8ea7dcb27fc0eb86bafe9aadaa13b114dfb61aa20` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/Shapes/b2PolygonShape.cpp` | `2281e552b7cf16b79d704a89160550c5e0a56da2a210ce926fd0719301cb61e1` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/Shapes/b2Shape.cpp` | `2bcfc135682d3c311ea246b2b8d2ce14ccc7694a5d586348761ae12bc9cd696e` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2BroadPhase.cpp` | `cfa4503a3c9a95db001e10dcc4cd6a9f27a97cbe5c676e46a5751cab7a8c06af` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2Collision.h` | `d1f825e57717f8163ff8e937b02f2e39a99a16714a37919fa83daca9a62f84a9` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2PairManager.cpp` | `0ba3ce40b97bc5c4c50aacbd131f665a63e44c8130c2ddf4c9ecc9abe75e2f4c` |
| `port/physics-backend/box2d-2.0.1/Source/Common/b2BlockAllocator.cpp` | `12426570e627db745da2bc5f7edbe0629d95b834dfabf87ef621a91623ae39f5` |
| `port/physics-backend/box2d-2.0.1/Source/Common/b2BlockAllocator.h` | `b9056e01399ee810604c54097a25d71f8e867e0ffad2712bc98498fce2d5bfce` |
| `port/physics-backend/box2d-2.0.1/Source/Common/b2Math.cpp` | `9dd538fca46377fc5ddfccaa5aaf293397c83d06e8400426856f0dbb8489a688` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2Contact.cpp` | `a0875445d2a2b253fa2815b26373611fe1cfdfcc24084cae74dba20a21e81b2b` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2Body.cpp` | `54055e4d49d2dbfb618768bac214855b20eceba404864a585eaf665cd9698cf3` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2Body.h` | `634324f14ef3835a5d33e2043a80dbcc4fb2bcbc1db9f9089e8205be998216ea` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2ContactManager.cpp` | `7c370f2d5012f921ce8b2a3ae194332792f5e773f3031e61e7c2c4f4a9dcb6a8` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2World.cpp` | `ad0dd30502b442b6b27bc05e62239ff6ef823ade04a19c34482665c25ecbdd56` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2WorldCallbacks.cpp` | `335f631fd1ab520f44025ca684568905be30fe5ab0eb6f9449af1a756be64ee9` |

## Input evidence hashes

| Input | SHA-256 |
|---|---|
| `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/functions.jsonl` | `3922efb894cf09cee9690708bc3976eaf6bc1372f7a584808ad9583ef3d8ff40` |
| `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/xrefs.jsonl` | `a4052ed15489b398edee7660a63e8286a35ffd642856a3c3ce82becf2efc8008` |
| `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/vtables-and-rtti.json` | `736cbb5f94e520533b70c5ec0b322ab2e422a168eda5f3139d141ae8f31e0169` |
| `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/assembly-functions.asm` | `2545683b66dbb3f8400f64774593b439ac72f4d9647f3f8506b3b028dbb9eb77` |
