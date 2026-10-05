# Faery cast lifecycle V2 candidate

New files only; prior frozen HUD, Save and faery gameplay V1 files are unchanged.

## Renderer integration

Retain one `FaeryCastOwnerV2` with real `FaeryCastServicesV2`; pass the same `PlayerGameplayBinding` used by HUD and skill actions. Route controller Begin/End cast through `command(binding, begin, network_origin)`. Route the existing StateOwner state 7 body to `state_body(binding, CastBodyV2::focus/blur/event/update)`. Route Character events 0x20/0x21 to `character_event`, the actual animator `do_spell` event to `animation_event`, and actual step Begin/End events with source step index and mode to `animation_step`. These are source callbacks, not synthetic per-frame events.

`cast_animation` must read the real player's CharacterModel CastAnim list: return 0 for an authored animation, 1 for the source invalid model/index no-op, any other value for failure. `faery_type` must read the selected original GetCharFaery row. Stance, animator, sneaking, event and network callbacks must use their actual native owners. State transition uses the existing same StateOwner event 50006; its original registered predicate is null. There is no separate FSM, Save, script VM or spell flag state.

Cast activation itself consults player CharacterModel, selected Faery row and retained spell instance. It does not require a fabricated companion actor. Companion Character+0x420 placement/model services remain a real requirement of faery selection V1.

Use `player_char_ai_keys_v1()` / `player_iphone_ai_keys_v1()` only as exact original dispatch identities for the real retained CharAI/AISPlayerIPhone. They are not executable native vtables. Call the retained V6 `update_timers` against its sole TimerStore; dispatch original helper 0x3cb77c (event 33) and 0x3df3f0 (event 34) through `player_timer_helper_v1`. Required actual aggro facts are CharAI+0x8c HasAggro and +0xa4 IsAggroed, not target pointers. DoT requires actual self/self F_DotAttack followed by F_ApplyResult.

## Source evidence

ELF SHA256: 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.

- CTRLIsAllowed 0x3ad430: scared attack-gate bit 4, stunned bit 2. CmdBeginCast 0x4055f8 / CmdEndCast 0x405654: actual forced byte 9 overrides global block/locked byte 8.
- AI_BeginSpell 0x3d81c0 / AI_EndSpell 0x3d7f60: shared CharAI+0xd0/+0xd1 flags, retained selected spell usability, source network operation 3/4. AI_IsSpellActive is constant false in this ELF.
- SM_SetCastState 0x3c6394: actual CastAnim, AnimStancedAnim constant mask 0x400000, same animation override and event 50006/state 7.
- CSCast focus 0x3c39d0: flags 0x6301, event 0x20, animation -1, speed 1, heading 0, CancelSneaking. Blur 0x3c3934 raises 0x21. Event 0x3c0024 / update 0x3c0020 are no-op bodies.
- SpellFocus 0x3d8038 -> Pre 0x3da8b8; SpellEvent 0x3d8ba4 -> Use 0x3da794; SpellBlur 0x3d8b28 -> Post 0x3da6c0. Actual animator event 0x3d4434 requires state 7 and exact do_spell.
- Animator step Begin 0x3d3dd4 / End 0x3d3d68: step index 0 and mode 1; continued/last share skill flags; actual StopLoop(true).
- UpdateRegen 0x3cb77c / RegenTick 0x3bdd90: remote update rejection, actual aggro or state 5/6/7 combat; raw HP39/40 then freshly reread MP44/45 through genuine RegenV6.
- HandleDots 0x3df3f0: freshly read resolved raw properties126..131, elements -1..4, positive amounts, fresh death test for each, actual DoT/apply services.

## Verification and limits

`faery_cast_state_v2_differential.py` executes original ARM and compiled native ARM64 kernels: 512 state-body cases, 288 animation-step cases, 1024 controller gate cases, 768 ordered service calls; 1824 cases, zero mismatches. Report: `faery-cast-state-v2-arm64-differential.json`. Profiler instrumentation is excluded; backend services are declared fixtures. All four candidate translation units pass strict ARM64 C++17 syntax checking.

Full Begin/End/Pre/Use/Post renderer wiring and actual unlocked campaign casting are not yet runtime verified. Timer adapter callbacks are source-derived and syntax checked, not claimed differential verified. Missing actual model, animator, network or DoT services return explicit failure after the correctly reached mutation prefix. No campaign faery is unlocked and no artificial world identity is supplied.
