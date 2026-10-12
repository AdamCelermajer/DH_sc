# Kind7 Script_WaitCamera: one static source extension

SOURCE_ONLY_UNCOMPILED_UNRUN. Bounded pure pyelftools/Capstone decoding only; no emulation/Unicorn/objdump/compiler/NDK/WSL/native/device job or whole-binary code scan. Exact current code/literal slices, offsets, bytes, hashes, vtable/GOT resolutions and symbol sizes are retained in kind7-source-proof.json. ELF SHA comes from existing V52 provenance.

This successor is based on the immutable reviewed kind4 candidate (47 classes/schemas), so its minimal diff adds only kind7. Frozen kind4, frozen46 and active/Main/root/selected sources remain unchanged by this worker. The four surgical hunks add the actual concrete Script_WaitCameraReceiverV59, factory case7, exact C1 descriptor, and exact Data schema. Candidate copies are replacement/review sources, not additional CPPs to link alongside originals.

Authentic C1/data evidence:

- Command factory458b48: allocate16bytes with memory hint0; store source data_c+0xc=NULL at458b6c, skip4 byte0 at458b70, source vptr at458b78, kind8=-1 at458b80. No receiver extra field exists and no padding is initialized. Actual vtable968b40/address-point968b48 resolves Init455628, Execute45c5e8, IsBlocking45932c.
- Data Structs::WaitCamera factory457518: allocate12bytes, explicit raw32 field4=0 at45753c and raw32 field8=0 at45754c, plus source vptr at457544. Actual vtable96ab68/address-point96ab70 points reader4ffdc0.
- Exact schema: read raw32 kind+4 through inherited base4ff828/459090, then field+8 via StreamReader.readAs<float>4db94c. Field8 remains raw IEEE754 bits in native ScriptScalarV52; no duration/timer meaning is inferred. Original constructor bits0 and both explicit defaults are retained.
- Data reader4ffdc0 symbol size120. Its literal endian guard1 bypasses swap code. Float helper4db94c symbol size176 passes r2=4/r3=0 to actual streamvirtualslot0x18; normal return requires read count4 and returned statusr1=0, then returns4db99c. Source policy/log/assert failure paths are captured but never emulated; native existing V52 bounds/error guards remain explicit safety additions.
- Data Free4c6e84 tail-branches qualified base4c6c68 BX LR; no scalar/pointer cleanup writes in this reached body. Existing V52 native ownership teardown supplies the same value-storage lifetime without Main effects.

Exact Main runtime boundaries (not implemented here):

|Method|Original byte/symbol scope|Required actual Main body|
|---|---|---|
|Init|455628, STB_WEAK,size4,BX LR|The actual class vtable proves base Init; V59 may complete its existing literal no-op. No new five-body Init hook is needed.|
|Execute|45c5e8, STB_WEAK,size136:120 code bytes +16 literal bytes|Actual existing DebugSwitches.s_inst9a1d18: call load337888; construct a temporary native string with original key isTracingScriptCmd; call GetSwitch337a88; destroy string. It also checks source stack guard (__stack_chk_guard/__stack_chk_fail). No command/Data/skip/context read or demonstrated camera operation occurs. Query result is discarded, so no returned boolean is proved. Main owns dispatch, debug-load/query effects and its native string/guard implementation.|
|IsBlocking|45932c, STB_WEAK,size68:60 code bytes +8 literals|Actual Application singleton99f72c -> GetCurrentLevel31f594 -> sole GSLevel.s_level9a2638. Return0 if current Level=NULL or SAME Level pointer+0x128=NULL; otherwise return raw byte+0x84 from that actual receiver. Never cache/copy this flag into loader state or use Data+8 as a guessed timer.|

The captured getter31f594 is32bytes (24 code/8 literals) and reads the sole source s_level global. DebugSwitches.load512 and GetSwitch196 engine bodies are identified by symbols/call sites, not implemented or recursively decoded. Native standard-string bodies and original stack guard are likewise Main/runtime scope. IsBlocking's +128 receiver type/creation and +84 field producer must be supplied/proved by Main; this loader extension creates no camera object or blocking VM.

Native representation stays the existing SAME V59 receiver/header and SAME owned V52 parsed Data, with native pointer widths and strong Data leases. Canonical descriptor checks, failure prefix retention and base-only teardown are unchanged. No executable opcode record, copied ARM vptr, speculative extra scalar, second App/manager/global/VM or permissive Main leaf is added. Execute/IsBlocking/dialogue/cinematics remain Main.

kind7_source_only_test.cpp is UNRUN test text: genuine native class construction, exact explicit Data C1 defaults, synthetic kind7/float NaN payload raw-bit preservation, SAME header/Data assignment, literal base Init, required Main Execute stop, and synthetic one-script manager load/unload. Its cache is a transport fixture; original ARM/helper and actual Main methods are not executed, and no PASS exists.

Future admitted private serial check from port/level-loader:

    g++ -std=c++17 -O0 -g -Wall -Wextra -Icandidate/command-kind7-v65 -I. candidate/command-kind7-v65/script_command_receivers_v59.cpp candidate/command-kind7-v65/script_manager_owner_v52.cpp candidate/command-kind7-v65/kind7_source_only_test.cpp -o candidate/command-kind7-v65/native-test
    candidate/command-kind7-v65/native-test

The unchanged V52 CPP copy makes its quoted schema include resolve to the local candidate; it is not an integration edit. Do not compile/link both original and candidate implementations. Any syntax/runtime lane still needs parent/Main admission; none is launched by this document.

Coverage:48 candidate classes/schemas,43 literal-base Init classes, five nontrivial Main Init classes. Static potential missing slots now32 after kind4/7; authored reach is still unknown outside existing common/SWAMP receipts. No additional local pair was sampled/decoded; V60's37 local pairs remain unknown. Whole-level/camera/render/gameplay acceptance remains open.
