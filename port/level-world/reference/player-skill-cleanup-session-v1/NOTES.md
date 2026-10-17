# Same-Session Player skill cleanup

## Source

The original input is `libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` pins complete `_SkillCleanUp` (72 bytes at
0x3d8ae0), `_SpellCleanUp` (72 bytes at 0x3d8a98), and
`CharAISkillScript::OnSkillCleanUp` (212 bytes at 0x3daafc), plus their
Lua Call and ReturnValues dependencies. This module adds those three source
callers over the maintained preparation Owner and retained Session. It adds
no Lua VM, resource-library, timer, death-state, or instance-retirement body.

Each original loop captures its vector count once, reloads the begin pointer
for each iteration, skips null slots and calls OnSkillCleanUp in slot order.
The skill phase precedes the spell/faery phase; the latter count is read only
after the skill phase completes. Empty vectors still complete their phases.

OnSkillCleanUp first constructs ReturnValues, then reads the Character's
live LuaScript association. A null script takes normal destruction without a
Lua call. Otherwise SetSkill receives the instance's retained Arguments at
instance+0xc. Ordinary SetSkill Error skips the cleanup callback and destroys
normal returns. Success erases previous values only when the vector is
nonempty, reloads the Character/script association, calls no-argument
OnSkillCleanUp and destroys normal returns regardless of its ordinary Error.
The caller does not query a boolean result. It does not erase preparation
instances, clear their vectors, close the VM or stop skill timer field18.

## Borrowed API and failure prefix

`Runtime(Session* const* source_script_slot, Owner&, character)` borrows the
live script slot, sole retained Session and preparation Owner. The canonical
nonnull Session identity is fixed at construction. Its VM, Character identity
and AIS identity must agree with the preparation state. A temporarily null
live slot exercises the source null-script branch without creating a VM.

`cleanup(List, index, result, error)` dispatches one nonnull retained instance.
`cleanup(List, result, error)` dispatches the full source vector loop.
`cleanup_all(result, error)` dispatches skills followed by faeries. Native
AI_SetDead can use its two separately ordered required cleanup providers;
the module owns none of the surrounding death, aggro or timer operations.

Zero completes source delivery, including ordinary Lua errors. Negative
required-service or unsupported-return failures stop at the reached Call,
retain ReturnValues and prior effects, and perform no invented destructor,
callback, rollback or continuation. `Result::last_lua_status` distinguishes
the Lua boundary status from the adapter return. Retained failed native
ReturnValues are released when this Runtime itself retires.

The live slot, Session, preparation state, retained Arguments and callbacks
must outlive the Runtime and synchronous calls. The Session already borrows
the canonical callback flags; this adapter does not copy or own another flag
word. Native bindings verify source active AIS against the same preparation
AI and Session AIS identity. Providers must not reenter, reprepare, rebind or
retire owners during a call. Close the VM before retiring preparation Owner.

## Evidence boundary

The host fixture loads real cache declarations, commons and unchanged skills
for KnightPlayerBase, MagePlayerBase and RoguePlayerBase. Each prepared class
contains 16 skill slots and five faery slots, including eight nulls. The full
cleanup delivers 13 SetSkill and 13 OnSkillCleanUp calls while retaining the
same VM, flags, preparation vectors, load count and property state. Generated
Lua overlays isolate ordinary errors, return erasure and required-failure
prefixes; a real borrowed timer field remains unchanged by this caller.

The original replay executes all three complete ARM caller bodies. Lua Call,
ReturnValues construction/destruction and vector erase are explicit resource
boundaries; the compiled counterpart uses the actual retained host Session
and return observer. Cases cover ordinary errors, empty/nonempty returns,
null script, null slots, both vector orders and empty vectors. ARM void return
is ignored. The stack-canary failure path is not claimed. Required and
unsupported failures are tested at the real host VM boundary, not attributed
to a modeled original service failure.

The runner compiles the module exactly once in the actual selected world DSO
and links one selected VM DSO. Before parent selection it can stage only the
new TU into that same target. It discovers the actual selected compiler
dependencies, snapshots reached project sources/headers, configured CMake
inputs, exact replay/reference files and cache inputs, then clean rebuilds
the selected target. After execution it compares dependency closure, hashes
and commands. Adjacent unselected headers are not guarded. Raw replay is
retained separately if source provenance changes. Host/original evidence
does not certify Android or live native death delivery; the parent records
those separately.

The final selected-world gate passed three real class cleanups, six protocol
cases, three failure-prefix cases and eleven guards. All 23 original ARM
comparisons matched. Its 552 reached project inputs remained unchanged across
the snapshot, rebuild and execution; the cleanup TU and VM TU each compiled
exactly once in their actual selected DSOs.
