# Original death-state selection

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The seven captured routines are in `original-functions.json/.asm`. This is
a source-only extension after checkpoint9112998a; no APK, renderer, shared
CMake or frozen Kill/AI-death source was changed.

## Complete coordinator body

`CharStateMachine::SM_SetDeadState(bool,void*,bool)` **3c58c8**,488 bytes:

1. Call current owner `GetCharAnimTableId` **3a3228**. This reads signed
   Character+1000, falling back17 when negative or outside the current global
   CharAnim count. The outer function reads the count again and returns without
   field writes or state dispatch if the getter result is still invalid.
2. Capture the current160-byte row pointer. With machine byte+3f nonzero,
   capture `DeadlyGreatKB`+10; otherwise capture `Died`+1c. Query exact
   `('AnimStancedAnim','SL__LIST_IPHONE')` through Application+2c. The ANDS mask
   is20000 for the former branch or8000 for the latter. If that bit is present,
   reload Character+4 and call `GetAnimStance` **3a53e0**; otherwise the modifier
   is exactly0. Add wrapping32 to the captured base.
3. Reload byte+3f before storing the first result to machine+28 (existing native
   `State.animation_override`). This live alternate chooses the second branch;
   it can differ from the first selection after a callback/reentry. Retain the
   row pointer; capture `DespawnGreatKB`+18 when alternate is nonzero, otherwise
   `Despawn`+14. Query the same constant again, reloading Application+2c. Masks
   are40000/10000 respectively. A set bit again queries current Character
   stance. Store wrapping32 sum to machine+38.
4. Store incoming mode's lowbyte at machine+3e, then clear+3f. Existing native
   `State.dead_alternate` projects **+3e**; it is distinct from pending+3f.
5. Call **SM_IsAwaitingToRevive3c01ec**, which invokes actual nullable-current
   `SM_GetState3c01ac` and tests IDs0(Limbus),17(PreSpawn),16(Reviving). When
   true, clear **machine+20, the current StateInfo pointer**. This is not an
   elapsed-time reset. Null current reads-1 and does not enter this branch.
6. Nonzero force tails to `_SetState3c1938` with state12,eventc358,payload;
   zero force tails to `RaiseStateEvent3c5684` with eventc358,payload.

Clearing current before dispatch means no old Blur and previous=-1. A normal
event on the resulting null current is ignored, preserving elapsed. A forced
transition lets the genuine owner choose12 and reset elapsed before entering
Dead OnFocus. For other prior states the registered owner owns Blur, Focus,
predicates, synchronous reentry and Character notification ordering.

The scalar row names come from the actual animation schema and original
`Structs::CharAnim::read4ef64c`, captured separately in game-data's
`reference/animation-readers`. Native decoded fields3,4,5,6 correspond to source
row+10,+14,+18,+1c. This module neither rewrites animation sequence IDs nor
selects a clip from its visual name. The secondary+38 value remains owned
state for later source Despawn orchestration; this wrapper does not implement
that separate state body.

## Native binding and ownership

`DeadSelect32` borrows canonical `StateOwnerMachine40`, row storage and current
count; owns projections `pending_alternate`(+3f),`secondary_animation`(+38).
`DeadAnimationRow16` stores the four signed fields in original order.

`dh2_character_dead_select(view,mode,payload,force,services,state_services)`:
services return authored Character+1000, actual mask constant and genuine stance.
The first service is an explicit producer boundary, while the native wrapper
executes the getter fallback. No timer/dt/speed or mask values are invented.
Constant requests carry the exact branch mask; providers resolve the exact
group/key through their retained actual design registry. State dispatch calls
the existing genuine registered-owner kernels, not the four-state shortcut.

Machine/FSM/State bindings, services, retained row storage and receivers must
survive the call and synchronous reentry. Providers may mutate Character,
current selection, alternate, row words, global row pointer and count at source
reload points. Replacing the global row pointer after capture does not replace
the retained row. Unknown native receiver lifetime/destruction is outside the
borrowed contract. Caller structs require normal native alignment.

Return1 means dispatch delivered, including a source-ignored event;0 means the
table gate returned;-1 rejects malformed canonical bindings before any callback
or mutation;-2 stops at an unavailable service prefix. Deeper owner diagnostics
propagate. State behavior/notification providers must report missing methods,
never manufacture transition acceptance. No extra once-only/dead-state guard.

## Verification and limits

`character-dead-select-arm64-differential.json`: actual original coordinator,
GetCharAnimTableId, SM_IsAwaitingToRevive and SM_GetState instructions against
optimized NDK29 AArch64.2,400 cases,7,365 ordered requests,40 synchronous
reentries,8 atomic caller rejections,zero mismatches. Covers invalid/fallback17
counts, all20 prior states/null, masks and signed wrapping animation results,
mode truncation/nonzero force, alternate changes between queries, owner reloads,
global row/count replacement, live later row words and reentry. Constants/stance
and downstream state calls are explicit oracle services. Gold files retain exact
state and request snapshots; no full original death-frame parity claim.

`character-dead-select-host-audit.json`:186,572 checks,2,400 gold replays,
1,384 service failure prefixes,40 reentries,8 atomic guards,zero ASan/UBSan
findings. Fourteen compositions execute the actual central StateOwner and native
animation table/constant/stance DSOs using all80 decoded rows and exact supplied
`animations_pycst.bin`. Idle Blur uses the actual empty source body; unavailable
other Blur/Dead Focus/notification bodies are rejected. Awaiting-state forced
prefixes prove previous=-1, skipped Blur and owner-managed elapsed reset;
nonforce null events prove preserved elapsed. This is a genuine composite
adapter proof with explicit missing behavior services, not full death/backend
completion. Actual DSO origins are verified with dladdr/ldd; hashes are checked
before and after without rebuilding those dependencies:

* world `f90819b3d86eb716d9fc1e4bc9a2d5d5dd32c535f3e5677335df1f4797979695`
* runtime `3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945`
* data `13c9499376f16fdddf0639987a7cc735b12d57a14c68b1f1ba23694a5ab35569`

## Reproduction and parent integration

1. Run `tools/build_character_dead_select_oracle.ps1` from the repo root.
2. Direct Python `tests/character_dead_select_differential.py` regenerates gold
   and ARM64 report; elftools/Capstone/Unicorn use the existing dependency cache.
3. Direct Python `tests/character_dead_select_host.py` builds only an isolated
   module/executable, linking actual stable sanitizer DSOs. No shared rebuild.

Parent can add `character_dead_select.cpp` to `dh2_level_world` and a
`character_dead_select_audit` target from `tests/character_dead_select.cpp`,
linking world/game-data/script-runtime/dl with `-rdynamic` (dispatch observation
interposes the two owner exports during gold replay; genuine compositions
forward via RTLD_NEXT). Test arguments are the binary gold path and actual
Android `assets/data` directory. `character_dead_select_host.py --main-linked`
then records a separate actual central-module proof; preserve the isolated report.

AI-death's required state service can call this API only with retained real
machine, authored current table index, row backing, design/stance and registered
behavior services. Complete body/animation/physics/AI death services still need
their genuine scene/controller implementations; selecting12 alone is insufficient.
