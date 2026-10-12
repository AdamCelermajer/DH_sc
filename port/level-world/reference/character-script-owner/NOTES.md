# Native selected-script ownership coordinator

This additive owner composes the captured source selector and Stage0..6 lifecycle with a private native Lua VM, LuaScript alias map, path and per-script loaded-file set. It does not implement every original virtual AIS method or gameplay global. It is outside the frozen b449 APK checkpoint.

## Source and ownership

The immutable discovery is `../character-script-ownership/NOTES.md`, its captured routines and `ownership-probe.json` (SHA256 `710f6a5639324e049aba1b12ca759a1317d2c71cf84af4ee961f6e7a02c12f92`). Original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

Authored `__player__` selects the real iPhone factory3ccfe4. It constructs an owned private VM through CharAIScript3d8fb0→LuaScript37c674→Instance31b268→newstate31b224. Constructor bool1 defers registrations, not VM allocation. Initial active remains untouched; pending is published after construction. The default and ordinary player factories also own private deferred VMs; a fallback's scripted0 suppresses common-script loading, not Stage1/2 binding.

`ScriptConstructorFields72` projects only constructor-written fields. Counters and collection sizes in that projection describe initial fields, not subsequently executed gameplay state. Derived fields are absent for the smaller AISDefault allocation. Native session identity is opaque and uses real 64-bit pointers; no original object-layout or complete vtable emulation is claimed.

## Exact binder null gate

LuaScript constructor leaves script+18 zero. The embedded Arguments/Binder begins at script+10, so bindMethod319af4 reads its null target at Binder+8 and returns before Lua effects. Captured Stage1/global Stage2 calls do not change that target. The new differential executes this original null-gated body and observes zero imported calls.

The catalogue retains all 300 original caller descriptors (35 Stage1,265 Stage2). Stage2 contains135 global and130 method calls. The owner delivers all170 globals to required provider services and skips130 methods through this established initial gate. It does not substitute fake callbacks for the skipped methods. This is the initial source constructor policy; later method-target mutation is outside this module.

## Stage ordering and failure

Stage0 uses the unchanged source selector/replacement kernel. Supported owned factory projections are default, ordinary player and iPhone player; other script kinds fail explicitly before replacement. Stage1 directly opens Base→Math→Table→String, retaining VM stack tops2/3/4/5, then delivers35 globals. Repeated Stage1 is permitted and retains top10. Stage2 writes borrowed Character identity before135 globals and assigns `data/scripts/ai/` afterward. Stage3 loads `_commons` iff scripted; Stage4 loads external filename iff present. Stage5 uses original alive-owner Timer33/34 initialization and the selected inherited empty Init method, plus explicit InitVCB when required. Stage6 publishes active=pending and preserves pending, reaching step7.

File resolution reproduces the source first `.lua` substring suffix policy. A successful load inserts the complete resolved path into that session's set. A missing cache item or protected Lua load error is source AddFile false: those prefixes remain observable and loading still reaches publication, matching the ignored source return. Invalid byte providers, VM allocation/open errors and failed required service delivery return protected native failure without pretending the original operation succeeded. There is no hidden once-only guard or transaction rollback. Nested completed-loader calls restore outer borrowed service/facts and constructor-kind scopes.

The cache provider owns manager/resource retrieval; this class does not implement the manager's shared StreamBuffer cache or flush policy. Registration providers own the actual recovered callback implementations and their Character dependencies. The owner does not silently create missing gameplay globals. Services and callback userdata must outlive their VM, including close-time finalizers. Live active-script choice, complete AI state collections and virtual class implementations remain separate integration work.

## Teardown

Session lifetime clears loaded filenames and path, then `dh2_script_alias_clear_contents` (backup→main, recording unchanged), closes the owned VM, and finally destroys the alias wrapper. The wrapper remains alive while actual Lua finalizers can execute alias callbacks. Default empty constructor collections need no synthetic gameplay cleanup. Other borrowed providers own their resources and must remain valid through this sequence.

## Evidence and reproduction

`character-script-owner-arm64-differential.json` binds the final O2 Android26 ARM64 source and oracle:303 constructor/ordered descriptor comparisons,6 atomic guards,130 original null-method calls identified,0 mismatches. Constructor and registration evidence comes from executed frozen original probes. This probe executes native flat helper exports, not the full C++ owner under ARM64 or a complete original frame.

`character-script-owner-host-audit.json` binds the actual owner audit DSO, rebuilt `libdh2_level_world.so` and `libdh2_script_runtime.so`. dladdr confirms actual world lifecycle and timer-adapter exports execute. ASan/UBSan:6,283 checks,0 findings;170 required global deliveries per completed initial load, two independent private sessions, non-scripted default/player factories, repeated binding, seven failed provider prefixes, cached common bytes, source-false missing/bad Lua paths, nested loader context and actual close-time finalizer. A real TimerStore expiry reaches the genuine selected timer-call adapter/VM. The registration-delivery fixture implements genuine Trace/alias/timer bindings only and explicitly verifies an unreconstructed gameplay global remains nil. It is not proof of the full gameplay namespace or original whole-frame parity.

The exact original cache common script is extracted into workspace scratch:13535 bytes, SHA256 `20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c`, from cache ZIP SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

Run NDK helper proof with `tools/build_character_script_owner_oracle.ps1`, then `tests/character_script_owner_differential.py --library .local-inputs/character-script-owner-discovery/oracle.so`. Run read-only DSO audit with `tests/character_script_owner_host.py`. Default world build is `/home/adampalace/dh2-world-build`; runtime is its `script-runtime` child. Neither shared build is modified by this runner. Once centrally integrated, `--main-linked` executes the owner itself from the rebuilt main world DSO and writes a distinct report.

Central CMake handoff: add only `character_script_owner.cpp` to the world DSO, preserving the catalogue include in the same directory and linking the actual script runtime. An audit executable uses `tests/character_script_owner.cpp`, links `dh2_level_world`, `dh2_script_runtime` and `${CMAKE_DL_LIBS}`, and takes `owner-fixtures.bin` plus the exact extracted common script. Do not recompile selector/lifecycle/timer kernels into that executable.
