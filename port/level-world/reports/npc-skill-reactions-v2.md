# Same-NPC SkillApply reactions V2

New production units: `npc_skill_reactions_v2`, `character_knockback_reaction_v1`,
`character_slow_reaction_v1`, and `character_head_object_v2` (hpp/cpp pairs).
No renderer, native app, CMake, Java or original frozen effect kernel was edited.

## Bind once to the retained MonsterScriptHandle

Construct `NpcSkillReactionsV2` with its SAME `CharacterWorldNpcStateOwnerV1`,
`NpcInjuryRuntimeV1`, canonical property view, loaded AI/animation/design tables,
session TimerStore and TimerServices. Slow borrows the SAME BuffOwner/property
view plus actual ClassTables and EffectsTables. A separately created timer,
property state, or FSM is not an acceptable binding.

* Apply callback: `application(q,out)` handles services 11–16. Return 1 means
  handled, 0 means not its operation, negative means required provider failed.
  Convert handled 1 to the delivery-success convention of the surrounding
  SkillApply service. Preserve the negative failure and its reached state.
* State-owner remaining methods: `body(machine,q)` for source focus/blur/event
  of Scared8, Stunned9, KnockedBack10 and Injured11. Convert 1 to source-service
  delivery success; 0 continues other existing method receivers.
* Remaining state updates: `update(machine,q)` for states8/9/10. It does not
  increment elapsed time. Source KnockedBack Update3c0038 is literally empty.
* Existing outer NativeFsmUpdate callback: delegate `fsm_set_stun` and
  `fsm_set_scare` requests to `frame_effect(q)`. The existing owner alone advances
  the clock and enforces its pending flags.
* Timer43/44: `timer_event(event,payload)` delivers to the existing machine.
  Its original AI/event body clears pending flags before the state continuation.
* Admit states8/9/10 in the NPC animation wrapper currently limited to3/11.
  Existing `dh2_character_animation_ai` already has genuine empty begin/endStep
  branches for these states. Completion22 must still go through actual CharAI,
  selected AISExternal VM OnEndOfAnim and the SAME FSM; no manufactured completion.

## Actual source calls and their providers

Push12 uses Character::SetKnockedBack3c5ea0. AI flags bit4 is the Boss gate;
animation index has original fallback17. GreatKnockedBack is table field8,
KnockedBack field16. AnimStancedAnim masks are800/400. Great overwrites FSM gate
with18; normal clears18. Direct requests transition10, event c35b and attacker;
other requests deliver that event to the actual machine.

Focus3c4a48 sets Character flags2341, selects animation−1, applies physical
filter(group0, category51c, mask3, secondaryfalse) when gate8/body exist, then
gate10 overwrites gate20, locks the actual controller, performs real LookAt and
CancelSneaking, and unpins the actual body. Blur3c48e0 unlocks, resets filter and
pins, re-reading body presence. Event27 resets filter; event23 checks actual
IsDead then stops actual animation and invokes SetDeadState(true,0,true).

Stun13 uses existing original-verified native effect setter with kind0, mode1,
force0. Scare14 uses kind1, mode1 and request force. Source timers are43/44;
their original duration and pending-bit gates are retained. Existing native
effect focus/blur/update/event kernels are reused without changing their
verified source arithmetic or random ordering. Remaining services must bind
actual IsPlayer, StopLoop, source random integer draws, point heading and object
heading. No blanket acceptance is supplied.

`character_command_head_object_v2` implements Cmd_HeadTowards4053d0 and
Character Ctrl_HeadTowards(GameObject)3ad8d4. Borrow existing controller and
CharacterHeadingOwnerV1, SAME logical `PathController.heading.active` uint32
storage, actual canonical Vec3fOrigin, and actual remote/position services.
NULL target returns only when heading is inactive; active heading takes the
actual source zero-point stop path. Scare blur must call this with NULL, not a
successful no-op. Point heading uses the existing whole native heading owner.

Slow15 implements PROPS_DebuffSlow3e2a5c. Boss/duration0/ClassDict miss are genuine
source returns. Actual Debuff_Slow class plus AUTO_DEBUFF_SLOW effect (missing
effect name gives source−1) create a real timed BuffInst via existing BuffOwner.
ApplyClassToSheet writes its retained sheet, then resolves48,47,46 in source
order. Reached missing BuffFX/timer/inst/property services fail explicitly.

CancelSneaking16 already composes `NpcInjuryRuntimeV1::application`, uses actual
IsPlayer and resolved198, and republishes changed byte415 to SAME canonical
character. NPC nonpositive198 returns before player skill-sheet services.

## Verification and boundaries

Strict NDK ARM64 C++17 `-Wall -Wextra -Werror` syntax checks pass all four new
production units and `tests/character_knockback_reaction_v1.cpp`. That test
checks source focus/blur/death event ordering, gate overwrite, ignored event22,
five reached failure prefixes and missing LookAt rejection. Test callbacks are
trace receivers, not production physical/animation providers. Runtime execution
of this new test remains for root's Android runner; no live reaction pass is
claimed here. Existing original differential native-effect suites remain the
evidence for the reused stun/scare kernels.

The actual scope regression `_commons.luac` is13535 bytes at
`.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/_commons.luac`.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Exact reaction source capture: `reference/npc-skill-reactions-v2/original-source.asm`.
Anim row mapping: `reference/character-monster-bank-readiness/skeleton-probe.json`.
SkillApply call modes: `reference/character-skill-combat-v6/original-functions.asm`.
