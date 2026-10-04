# Native FSM getter and outer-update adapter

This additive implementation is separate from the earlier read-only `NOTES.md`
and from the frozen Lua-named state registry. `character_native_fsm.hpp/.cpp`
does not implement the original twenty-state factory, stun/scare bodies, or an
entire actor frame. It does implement the complete surrounding Update prelude
and getter/preset bodies recovered in this directory.

`NativeFsm24` borrows a stable `State*`, the actual Character identity, and a
separate nullable-current projection. An absent StateInfo returns -1 even when
the stored ID is another value. The elapsed getter preserves the raw uint32
word as a signed int32 before the source float32 conversion. Script callback
argument count is ignored. The CString preset helper recognizes only exact
`Limbus` (0) and `PreSpawn` (17); all other strings select 3. It does not produce
the original owned property string or select/register that state.

The synchronous outer provider receives these ordered requests:

1. Profile begin; then the adapter captures elapsed.
2. Raw engine dt, followed by wrapping elapsed addition.
3. Pending mask2: unless current ID9, stun duration UINT_MAX, mode flags bit11,
   null payload, force false.
4. Reload pending mask4: unless current ID8, scare duration UINT_MAX, mode flags
   bit10, null payload, force false.
5. Reload current presence/ID/Character owner; dispatch current virtual update.
6. Profile end.

Original addresses and the source nullable-state predicates are documented in
`NOTES.md`. Providers may update current presence, ID, pending masks, policy,
and Character identity at these reload points. The borrowed `State*` binding
must stay valid and unchanged throughout the outer invocation. Providers must
not throw. A nonzero native provider result stops at its delivered prefix and
returns -2; it does not manufacture successful gameplay effects or roll back
already delivered calls. Malformed structural inputs return -1 before effects.

The current provider can call `dh2_character_native_fsm_bounded_tail` for the
existing source-built Idle3/Move4/Attack5/Dead12 handlers. It passes dt0 to
`dh2_character_state_update`, because elapsed was already advanced. Other IDs
return -2. IdleCommonUpdate, movement/animation/body/timer/AI effects, and the
real stun/scare implementations require genuine providers. A null current
does not call a body. No missing native state method is accepted as empty.

## Executed proof

The immutable original probe has 1,536 Update cases, 60 pairs of raw signed
integer getter requests, and six preset strings. Original Update, predicates,
current reloads, getters and owned-string compare instructions execute. Clock,
profiling, integer push, complete stun/scare/current methods and libc string
primitives are explicitly supplied services. The source integer constructor's
signed `__aeabi_i2f` call is separately captured under `integer/`; the original
getter oracle records its exact integer-push argument rather than claiming an
original Lua VM was executed.

Optimized ARM64 `character_native_fsm_differential.py` compares 1,662 cases
against those original records: 1,536 updates, 120 integer outputs and six
presets, plus six atomic rejections. `native-update-fixtures.bin` preserves
7,128 ordered requests. The ARM64 artifact is an isolated native helper, not
an Android APK or a replacement ARM32 runtime.

The sanitized host audit replays that original corpus, exercises six native
failure prefixes and nine malformed-input guards, and makes 360 direct script
callback calls including ignored argument counts and signed float conversion.
Fourteen compositions execute the actual linked world-library state producer;
these include zero-dt tails, Move's event3f transition to Idle with elapsed
reset, and rejection of an unsupported current method. Twenty-five cases bind
the callbacks into a genuine private float32 Lua VM and source argument-value
bridge, including high-bit elapsed. This is a composite adapter proof with
explicit effect services, not original full-frame or live monster/rene parity.

## Central integration handoff

Production world library: add `character_native_fsm.cpp` only after this source
freeze. Suggested host target `character_native_fsm_audit` uses
`tests/character_native_fsm.cpp`, links actual `dh2_level_world`,
`dh2_script_runtime`, and `${CMAKE_DL_LIBS}`, and has the same sanitizer options
as the existing state audits. Its sole argument is
`reference/character-native-fsm/native-update-fixtures.bin`.

Reproduction from the repository root in PowerShell:

```powershell
& port/level-world/tools/build_character_native_fsm_oracle.ps1
$env:PYTHONPATH='C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages'
$env:PYTHONDONTWRITEBYTECODE='1'
$python='C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe'
& $python port/level-world/tests/character_native_fsm_differential.py --library .local-inputs/character-native-fsm/oracle.so
& $python port/level-world/tests/character_native_fsm_host.py
# Only after central world integration:
& $python port/level-world/tests/character_native_fsm_host.py --main-linked --runtime-dir /home/adampalace/dh2-world-build/script-runtime
```

Neither production CMake nor the owner, frozen named-state module, renderer,
APKs, or old reports were edited for this adapter.
