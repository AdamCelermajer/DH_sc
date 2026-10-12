# Kind4 Script_SetCamera: bounded static source extension

SOURCE_ONLY_UNCOMPILED_UNRUN. This candidate adds one missing class/schema from908 bytes of bounded original ARM code/literal source recovery, plus exact symbol metadata. No emulation/Unicorn/compiler/NDK/WSL/objdump/native/device job ran. The ELF SHA is the one established by retained V52 provenance; exact current byte slices/file offsets/hashes are recorded in kind4-source-proof.json. No whole-binary code scan occurred.

Authentic kind4 factory-table names: Script_SetCamera and Structs::SetCamera. Command factory458c20 calls allocator310570 with r0=16/r1=0 and then source stores: data_c+0xc=NULL at458c44; skip4=0 byte at458c48; source vptr at458c50; kind8=-1 at458c58. It writes no extra operand field and does not initialize padding. Literal/GOT recovery gives actual command vtable968b88/address-point968b90, Init455628 (BX LR), Execute455678, IsBlocking45567c.

Data factory4575e4 calls allocator310570 with r0=32/r1=0. It writes only CString pointer+0x14=NULL at457608 and pointer+0x0c=NULL at45760c, plus source vptr at457614. Other scalar fields/padding remain unproduced until Read. Actual vtable96abb0/address-point96abb8 has reader5027f0.

Exact reader schema (offsets hexadecimal, raw bits preserved; no guessed field meaning):

|Source Data field|Read action|
|---|---|
|+0x04|raw32 kind via base4ff828 ->459090|
|+0x08|raw32 CString length via3df1a0|
|+0x0c|owned CString of length+8 via317454; explicit NUL terminator|
|+0x10|raw32 second CString length via3df1a0|
|+0x14|owned CString of length+10 via317454; explicit NUL terminator|
|+0x18|raw32 via459090|
|+0x1c|raw byte via4db89c|

The source reader frees each prior CString pointer before allocating length+1, reads its exact bytes, and terminates it; fresh C1 makes those pointers NULL. Its literal endian guards are1, so source swap branches are not reached on this path. Primitive helper success paths pass actual stream virtual-read slot0x18 sizes4/1 with flag0 and do not normalize the byte. The candidate uses the existing V52 native bounded single-read/owned-string implementation. Native bounds/no-replay/lease safety additions remain the existing explicit V52 contract.

Runtime leaves proven by exact bounded bytes and .dynsym sizes:

- Execute455678: STB_WEAK,size4,bytes1eff2fe1 = BX LR. No load/store/call or demonstrated camera effect. r0 is unchanged, so this is not proof of a returned boolean value. Main still owns Execute dispatch and may implement this exact empty body with its transport-completion convention.
- IsBlocking45567c: STB_WEAK,size8,bytes0000a0e3 1eff2fe1 = MOV r0,0; BX LR, establishing literal false. Main owns the blocking/VM dispatcher; no new VM is created here.
- Init455628: actual class vtable slot0 points the proven base BX LR. Existing V59 initialization can legitimately complete that body; no five-body Main Init adapter is substituted or replayed.

Minimal proposed integration is exactly four files/hunks in kind4-minimal-proposed.diff: add one concrete Script_SetCameraReceiverV59; add factory case4; add exact C1 descriptor; add exact V52 schema. Candidate names/APIs intentionally match V59/V52 so Main can apply the four surgical hunks to its actual existing owners. Do not link original and candidate CPP copies together or create another manager/App/global. Existing V59/V63/Main/root/selected files are untouched.

Full unique copies of those four files are provided only for review/isolated reproduction. script_manager_owner_v52.cpp is an UNMODIFIED copy: its quoted schema include must resolve to this candidate during a future private check. It is not another integration edit. Root/header types and factory-table tags are reused unchanged. Parsed Data cstring getters are NULL before Read, while constructor scalar defaults remain absent; no speculative scalar/padding zero-fill is added.

kind4_source_only_test.cpp is uncompiled test text: synthetic raw field bytes (including embedded NUL and byte0x7f), C1 fields/unproduced extras, two owned CString views, SAME command/Data assignment, literal base Init, required Main Execute stop, and one synthetic whole-manager load/unload. It neither executes original ARM readers nor supplies real Main effects. No native PASS receipt exists.

Future admitted private serial reproduction from port/level-loader:

    g++ -std=c++17 -O0 -g -Wall -Wextra -Icandidate/command-kind4-v64 -I. candidate/command-kind4-v64/script_command_receivers_v59.cpp candidate/command-kind4-v64/script_manager_owner_v52.cpp candidate/command-kind4-v64/kind4_source_only_test.cpp -o candidate/command-kind4-v64/native-test
    candidate/command-kind4-v64/native-test

A syntax-only future lane can inspect the candidate CPP/test with these two include roots, but no such lane is authorized/launched by this document. Runtime and original differential verification need separate admission.

Coverage is now47 candidate constructor/schema kinds,42 literal-base Init classes and five required nontrivial Main Init classes. Existing34 possible missing table slots are reduced by this ONE source-proven extension to33. Additional authored map reach is still unknown: no previously unsampled pair was decoded, and V60's37 local pairs remain unknown. Visible full-level/camera/gameplay readiness is not established.
