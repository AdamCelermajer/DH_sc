# One-call source return observation

`script_runtime_return_v1.c` is an additive replacement TU that includes the
unchanged `script_runtime.c`. Build it instead of that frozen TU; existing APIs,
private protected operation machinery, busy/stack/status guards and source
object bridge remain in the included source. The new header defines only
`dh2_script_vm_call_first_source_v1` and its40-byte first-return observation.

The source return protocol is bound by the existing original captures/probes in
`reference/game-bindings/return-projection-gold.json` and adjacent ASM. Source
Value number is **tag3 float**, not an integer tag. All actual returns are
projected in source order, including table `_this` member lookup and its
synchronous `__index` effects. Functions/thread/full userdata map to nil0;
strings stop at the first NUL. Table is tag7 even when `_this` is absent.
The first result is captured before later projection, but its observer runs
only after all projection succeeds. There is one actual Lua call, not a
second-call emulation or fixed return-count buffer.

The observer sees borrowed first-string bytes while the return is stack-rooted.
It cannot throw, destroy, rebind or use generic VM reentry. Positive source Lua
errors, unsupported error object-4 and required-service epoch failure-5 retain
their meanings. A caught required failure remains-5 even if the Lua body returns.
All paths restore the previous stack; the VM remains busy during observation.

The new host audit checks17 actual calls,13 observers/busy guards, maximum256
returns and30 checks under ASan/UBSan/LSan. It includes original evidenced arities
1/2/17/48/256, later table-member effects, first-NUL strings, numeric17.75,
nil/function projection, source errors, required failure caught/uncaught,
later-return projection error and observer-required failure. This is an actual
native VM protocol/lifetime audit grounded in the captured source protocol;
it is not a claim that a complete original ARM32 Lua VM ran in that host audit.

The HUD SkillInfo adapter consumes this observation after source `OnSkillInfo`;
it checks return_count and tag3 before timer conversion. Borrowed private Session
ownership is mandatory and stays separate from the generic runtime API.
