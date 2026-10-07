# Source private VM construction and library stack policy

This source integration is newer than the frozen b449 checkpoint. The original
ownership evidence is in `../../level-world/reference/character-script-ownership/`
and its `vm-core-probe.json`; it executes actual original Lua creation, library
opening and close. No original ARM32 VM is shipped.

`dh2_script_vm_create_empty` now creates an owned native64 Lua VM without
libraries. The existing convenience `create` remains available and preserves
its eager libraries/zero-stack policy. Selected source AIS constructors use
the empty policy; their deferred-binding bool does not disable VM ownership.

`dh2_script_vm_open_source_library` directly invokes the pinned Lua core for
Base, Math, Table or String. The original binding order is Base -> Math ->
Table -> String, leaving stack tops 2 -> 3 -> 4 -> 5. Repetition appends another
five values; there is no introduced once-only guard. Other VM operations retain
their previous stack restoration policy and preserve these initial results.

The direct calls use the pinned `luaD_pcall` protected primitive. This retains
successful stack results and protects even the first stack/closure allocation;
a new C closure pushed before a public `lua_pcall` would allocate outside its
protection. Stack growth occurs inside the protected call. A failure restores
the stack and records a diagnostic, while earlier global mutations can remain.
This is an explicit native memory/error boundary, not transactional rollback.

`reports/script-vm-ownership-host-audit.json` records 921 private-VM checks,
438 existing core regressions and 123 genuine game-binding regressions, all
passing through the actual sanitizer-instrumented runtime DSO. The new audit
checks library absence before binding, exact intermediate/repeated stack sizes,
independent private globals, retained stacks through calls/errors, preserved
eager behavior, busy-VM guards and bounded allocator failure. There are zero
ASan/UBSan findings. Its source/binary/compiler bindings refer to that saved
build; newer alias/session work has separate proofs.

Both Android projects built ARM64 and x86_64 with the additive policy. These
checks do not prove complete Character namespace registration, NPC behavior,
manager resource ownership, packaged original VM instruction parity or physical
device behavior. The owned-session coordinator is a separate integration.
