# CharAISkillScript skill update and cleanup callers

`character_ai_skill_script_update.{hpp,cpp}` rebuilds the normal 212-byte
`OnSkillUpdate` caller at`0x3dabd0` and reuses its ReturnValues/SetSkill/erase
sequence for `OnSkillCleanUp` at`0x3daafc`. The native adapter calls the actual
retained Monster AIS VM and existing script arguments; the full VM and Lua
callback bodies remain external runtime dependencies. The original full ELF
hash, exact symbols/ranges and literal bytes are pinned in`original-functions.json`.

## Exact source order

1. Capture the skill object (`r6`) and construct local `ReturnValues` through
   `0x31b434`. This occurs even if the Character has no Lua script.
2. Read skill+4 owner, then Character+`0x3e4` LuaScript. Null script skips the
   two Lua calls and converges on the normal ReturnValues destructor.
3. Call `LuaScript::Call("SetSkill", Arguments const&, ReturnValues&)` at
   `0x37c390`, using the captured skill's embedded Arguments at+`0x0c`.
   The constructor populated those with script name and skill index. This
   caller does not replace/rebuild those arguments or read current FSM state.
4. Read local ReturnValues+8, the embedded Error+4 code. Nonzero skips update
   and destroys the resource. Raw callee`r0` is discarded.
5. For zero error, read ReturnValues+`0x24` value-vector pointer, then its begin
   and end words. If unequal, call `_M_erase(begin,end)` at`0x31c3cc` once to
   clear the whole captured range. It is an actual value-destructor boundary,
   not a successful empty callback. Equal begin/end skips this operation.
6. Reread skill+4 owner, then the fresh Character+`0x3e4` script after erase.
   Call `LuaScript::Call("OnSkillUpdate", ReturnValues&)` at`0x37c494`.
   This overload supplies no explicit Lua arguments; the existing ReturnValues
   is output storage, not passed as a Lua argument. No second null-script guard
   exists in the original. A provider-cleared owner/script is an explicit
   invalid port boundary, not a successful no-op or null skip.
7. Destroy ReturnValues through`0x31b398`. Neither the second call's raw return
   nor its Error code controls this cleanup. Check the compiler stack canary
   and return; a corrupted canary calls`__stack_chk_fail` at`0x30e310`.

No native state, current skill ID, cooldown or last-ID+`0x18` is written here.
The no-argument FSM predicates in `UpdateAllSkills` precede this child caller
and remain outside this module.

## Cleanup and unload evidence

`CharAI::_SkillCleanUp` at`0x3d8ae0` and `_SpellCleanUp` at`0x3d8a98` walk
their retained vectors, skip null slots, and call
`CharAISkillScript::OnSkillCleanUp` at`0x3daafc` for each nonnull object. The
cleanup callback uses the same ReturnValues construction, `SetSkill` with the
embedded Arguments, Error-word gate, full return-value erase and no-argument
Lua call sequence; its callback name is `OnSkillCleanUp`. The list helpers do
not delete or clear the entries.

`CharAI::AIUnLoadScriptProcess(bool)` at`0x3cc9dc` runs both cleanup loops,
then calls each nonnull object's D0 deleting destructor and resets the vector
end. `CharAISkillScript` D1 at`0x3cc238` destroys embedded Arguments at `this+12`;
D0 at`0x3cc448` adds object deallocation. The renderer's
`NativeMonsterInitialization` destructor invokes both VM-backed cleanup lists
before timer retirement. Its `skills` member is destroyed before its `vm`
member, so retained script instances release their Arguments before VM teardown.
Cleanup provider failures are logged and do not escape the C++ destructor.

## Lua current-skill semantics and remaining providers

The unchanged11291-byte`skills/_commons.luac` is pinned as supporting source
evidence. `DeclareSkill` captures registration name/ID. `RegisterSkillEx`
stores callbacks and that ID in the private skill registry. `SetSkill(name,id)`
selects the registry's callback globals, stores `SKILL_ID` from the registry's
ID and stores `SKILL_NAME`. The supplied second argument is not used by this
Lua body. Missing names recursively select the authored default entry. Thus
an adapter cannot fake skill selection by assigning an arbitrary state ID or
directly calling a guessed per-skill function. `OnSkillUpdate` is the currently
selected global callback, which may perform real source effects.

The production adapter owns a live ReturnValues resource/vector for each
synchronous callback and invokes the current retained Monster VM through the
existing `Session::call_all` bridge. It maps retained skill script identities
to their actual embedded script name/index Arguments, clears the full returned
range, and preserves Lua error words separately from provider failures. The
VM, Lua implementation, allocator internals, and script callback bodies are
not rebuilt here. The host provider owns a real `std::vector` and physically
clears its storage; named callback observations are fixtures, not rebuilt Lua
or allocator behavior.

## Projection, ownership and errors

State borrows a live Character projection. Its script identity is read fresh
after construct and after optional erase. Captured skill identity+`0x0c` remains
the original embedded argument identity even if a provider changes projection
metadata. A provider constructs/retains a real ReturnValues handle and mutable
error/vector projections. Resource identity remains fixed; live vector backing,
replaced owners and scripts remain retained through the synchronous return.
Native/ARM object layouts are not overlaid. Value endpoints are provider-owned
native ranges; no original112-byte Value stride is imposed on native storage.

One owning thread, no same state/resource reentry or destruction from callbacks;
independent owners/resources may nest. Controls and read metadata are aligned
and disjoint. Port input guards run before source effects. A malformed later
projection or provider error stops at that reached phase and preserves all
prior effects/resources. No extra destructor, rollback or unwind cleanup is
added. A failed provider owns the retained resource until its outer owner
disposes it; source normal cleanup only occurs on normal completed branches.
The source's raw/unspecified callee return words are separate from typed port
service error status.
A Lua error returned normally by the real Call provider is projected into
`ReturnValues.error` while the port operation succeeds. It must not be mapped
to a port error: SetSkill's normal error branch still destroys ReturnValues,
and the second call's normal error code is ignored before that destructor.

## Verification and scope

The host gate covers229 behavior cases,13 returned-error/exception/missing
provider cases,16 malformed reached-source boundaries and11 preflight guards.
It tests normal update and cleanup paths, initial missing script, SetSkill error words, empty/nonempty vectors,
owner/script replacement during construct/SetSkill/erase, captured bindings,
independent nesting and preserving prefix effects without added cleanup.

The original ARM runner compares189 update cases against the compiled caller. It
executes all48 normal source instructions, including the real owner/script
loads, Error-code branch, captured range comparison, fresh second call and
normal destructor ordering. The five called dependency bodies are intercepted
as named observing/mutating fixtures, with nonzero raw returns proving that
caller does not mistake them for script errors. Source call names, overload
arity and embedded argument offset are checked. ELF symbols/range hashes,
literal strings and unchanged skill-common source hash are checked first.
The actual relocated`0x30e310` import resolves to`__stack_chk_fail`; no import
is modeled or executed by normal comparison cases. Compiler corruption trap
and16-byte literal pool are excluded from instruction execution coverage.

The cleanup function ranges and list/unload/destructor order are checked by
exact symbol, address, size, and SHA-256; cleanup path control flow is host-tested
through the shared kernel, not separately instruction-compared against ARM.

Count: **one ARM-oracle-compared caller; zero new dependency/Lua callback bodies**.
Native VM integration is wired; full gameplay skill parity remains unverified.
