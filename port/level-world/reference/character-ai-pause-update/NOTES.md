# CharAI::AI_PauseUpdate

Original `0x3cb748`, 52 bytes, loads Character owner `AI+4` before storing byte1
at `AI+0x18`. It calls that captured owner's embedded CharTimers at `+0x3b4`
with the unmodified uint32 duration, repeat0, event`0x31` and null user pointer.
There is no duration validation, timer cancellation, result gate or unpause
after this call. The timer return word is discarded by the semantic caller.

The maintained kernel shares `AIFrameState32::paused` with the existing frame
dispatcher and passes owner identity/embedded offset to a concrete timer-store
provider. It does not perform arithmetic on a native Character pointer using
the old object's offset. Provider mutation/replacement is retained; missing or
failed Start leaves the already-set pause field intact. Valid projections and
callback storage must remain alive throughout the synchronous invocation.

The comparison executes every original caller instruction and captures all
five Start arguments, including the stack user-pointer argument. Cases include
zero, max/high-word durations, noncanonical old pause bytes and callback owner/
pause mutations. A separate host composition uses maintained character timers
and RaiseAIEvent to prove event31 clears the frame pause projection on expiry.
It checks blocked clock, zero-duration nonexpiry and storage failure. These
checks do not reproduce original timer/event bodies in the comparison or prove
native Android AI binding. The original timer kernel has its separate audit.
