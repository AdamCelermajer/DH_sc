# Controller attack to actual melee command

This is original-source recovery with executable service fixtures, not a native attack-module or live parity claim. The supplied original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. [original-functions.json](original-functions.json) binds ten routines. [melee-probes.json](melee-probes.json) records1,216 original cases:1,088 complete melee/Character-entry executions and128 gated/offline/network controller executions. Native comparisons=0; mismatches=0.

## Entry and source gates

Character.Ctrl_Attack `0x3ad87c` adds `0x3c8` to obtain CharAI, sets r2=false, and branches to AI_DoMeleeAttack `0x3d01ac`. The v2Controllable thunk `0x3ad874` subtracts `0x374` before that call. These original instructions execute in the controller corpus. This is the concrete route that should eventually replace a direct renderer `prince_event(0xc354)`.

v2Controller.Cmd_Attack `0x405b04` first checks forced byte+9. Forced bypasses global `s_blocked` and locked byte+8; otherwise either block returns before all attack work. The global's identity/initialization/producers are bound in [Prince AI notes](../prince-live-ai/NOTES.md). An allowed command ultimately invokes controller's borrowed controllable+4 virtual+`0x38`, which resolves through the Character thunk above.

AI_DoMeleeAttack begins with owner virtual+`0x34` IsDead. Nonzero returns. It then reads owner+`0x528` bit0; set returns. Otherwise it calls owner virtual+`0x124` CanRangeAttack. True redirects to actual AI_DoRangeAttack `0x3d076c` with the **original requested target and mode**, then returns. The range routine is captured but is a named redirected service in this corpus. There is no assumed melee acceptance when ranged capability is present.

The melee path calls actual SM_IsAttacking `0x3c02d0`, which obtains live state and compares it with5. If attacking and AI.last `+0x79` is nonzero, it returns. If attacking and last is zero, it writes continued `+0x78`=1 **before** target-list construction. If not attacking, it writes continued=0 before explicit-target assignment or acquisition. Neither path resets index `+0x74`, last `+0x79`, or finisher `+0x7a`. The corpus seeds these fields with nonzero sentinel values and verifies their preservation.

## New attack and target assignment

For a nonattacking actor:

1. With an explicit nonnull target, call AI_SetTarget(requested,mode). Without one, create a fresh ObjectSearcher.TargetList with owner, arguments `(1,0,1)` and search as described below. A nonempty list assigns the **first record's object** with AI_SetTarget(first,false); an empty list retains an existing AI target. It does not force target=null.
2. Re-read live AI target. If null, raw signed owner object-of-interest byte+`0x14a8` equals8, and heading-active byte+`0x1b5` is zero, set seeking+`0x4a`=1, assign owner object-of-interest pointer+`0x14a4` using SetTarget(false), then execute SyncLastTarget `0x3d49c4`. IsPlayer controls diagnostic logging, not whether this fallback assignment occurs.
3. Mode=true returns after target work, before state change. This is the speculative/network form.
4. For mode=false, a null live target proceeds directly. A nonnull live target must pass AI_IsInMeleeRange `0x3d6188` with a null argument, meaning its current-target form. Failure returns.
5. Invoke SM_SetAttackState `0x3c6488` with the live target and false. Its source false branch tailcalls state RaiseEvent `0x3c5684` with event `0xc354` and target payload. No forced direct state change or accepted result is fabricated by this audit.

AI_SetTarget's full owner/notification/sight semantics are a separately captured boundary in [Prince AI notes](../prince-live-ai/NOTES.md). This probe observes its source call arguments and supplies target/candidate pointer mutation; it does not claim its complete backend effects or last-target updates for every false-argument path. SyncLastTarget itself executes original instructions where called. The state RaiseEvent backend is also explicit.

## Continued attack and acquisition

In the attacking/last=false branch, the original ignores the incoming explicit target after creating its temporary list. For heading-active=false it tests current target using AI_CanAttack(null). If accepted and target.IsDead is false, it retains the current target. Otherwise it clears/switches the temporary list's sort function through actual SetSortType clone `0x3d015c`, then searches. Heading-active=true goes directly to its narrower angular search. A nonempty result sets the first object's target with false; empty search retains the existing target. The temporary list is destroyed before return. This branch sets continued=1 but raises no attack-state event; later original animation-end/combo consumers handle continued attack.

Heading-active=false searches using float words `(0,0x40c90fdb)`; the latter is the original single-precision2π value. Heading-active=true resolves actual original constant operands to `CharacterDesign / Attack_FrontalAngle`, performs signed integer ASR1 **before** conversion to float, and multiplies by exact float bits `0x3c8efa35` (degrees-to-radians). The already parsed cache constant is90; the source therefore uses45 degrees converted to float radians. It does not use an invented cone constant. The corpus uses that90 fixture, binds the parsed constants SHA, and records each actual string operand. Complete PyDataConstants map loading is not executed here.

Original ObjectSearcher.TargetList constructor `0x4a2730` executes its buffer/sort/flag writes, with constructor helper callbacks `0x4a2240` and `0x4a191c` supplied. Actual SetSortType clone executes, including pop operations. Search `0x3d0020` is a named fixture recording both float arguments and returning an ordered zero/one-record list. Broadphase collection, filter membership, distance sorting, ownership/refcounts, and query center production remain unimplemented by this audit. The first-record selection is genuine original caller behavior; the fixture does not establish which live monster should occupy that record.

Diagnostic log `0x337a88` return values0/1 are varied. A true result plus owner.IsPlayer causes the original list-pop diagnostic branch after selecting a target. This is included explicitly rather than assuming logging return0. String/log/temporary destruction callbacks are fixtures, not a logging reconstruction.

## Network speculative scheduling

After generic controller gates pass, Cmd_Attack calls network singleton `0x7fd794`, reads its byte+5, and requires controller+`0xa` plus nonnull Character+`0xc` to enter the speculative branch. These are actual original gates, supplied independently in the128 controller fixtures; they are not inferred from the actor heading byte.

The speculative branch snapshots Character AI target+`0x408`, last target+`0x40c`, and continued+`0x440`, then calls melee with mode=true. It compares resulting target/continued with the snapshots. Its original restoration choreography uses SetTarget(last,true), SyncLastTarget, SetTarget(target,true), then restores continued. If target or continued changed, it additionally allocates/sends source `CMsgControllerAction` opcode0, owner byte ID from+`0x108`, and target's low16-bit ID (zero for null). The unchanged branch tests actual SM_IsAttacking/current continued and selects its source restoration/no-send path. The probe executes these instructions, retaining packet type/opcode/IDs in the trace; network singleton, packet allocation and enqueue are named fixtures.

After this speculative branch, the original still invokes controllable virtual+`0x38`. For Character this reaches real Ctrl_Attack and melee mode=false. Thus the controller corpus observes one ordinary melee call offline, and speculative true **followed by** ordinary false when all three online gates pass. These modes are not duplicate frame integration or independent animation clocks.

## Native implementation handoff

A future `character_ai_attack` coordinator needs live owner, flags+`0x528`, state, heading-active, object-of-interest pointer/result, and AI target/last/continued/last/seeking fields. It needs synchronous services for owner dead/ranged capability/player, range redirect, actual target-list construction/search/pop/destruction, AI_SetTarget and SyncLastTarget, current-target CanAttack/melee-range, and SM_SetAttackState. Controller command integration additionally needs actual global/locked/forced and network gates, controllable dispatch, source snapshot/restoration and packet services. A service must not replace target acquisition with a constant accepted target or range query with true.

The existing native CanAttack flow can supply its decision component with genuine callbacks. Inventory ranged/melee predicates can use the decoded item-table query. The separately recovered controller/path/look modules can supply their source wrappers. Hostility/range radii, target-list search/binding and actual inventory ownership still need concrete source producers before the live renderer can claim the complete attack-command path.

Reproduce with `tests/prince_live_attack_discovery.py` and the bound local original ELF. It executes complete original melee and controller callers, actual state getter/IsAttacking/SetAttackState wrapper, Character thunk/entry, target-list constructor/sort clone and SyncLastTarget; the explicit service boundaries above remain. No ARM64/host-native differential, full ObjectSearcher, full range attack, network backend, complete FSM, full AI or gameplay parity is claimed. No renderer, CMake, APK, ADB or existing source was edited.
