# Player CharAI death caller composition

The selected `player_ai_death_v1` module borrows the associated CharAI, Character target facts, Coordinator, extra canonical state-machine fields and same-Session cleanup services. It owns no VM, property sheet, relation map, timer collection, Save, inventory or renderer. Adam's earlier source reconstruction remains the source of reused selected modules; this adapter closes the pinned original callers over those authorities.

## Original route

The original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

| Original function | Address | Bytes | Recovered responsibility |
| --- | --- | --- | --- |
| CharAI::OnDied | 0x3d1000 | 80 | Optional GroupInfo callback, fresh active AIS virtual+0x24, AI_SetDead |
| CharAI::AI_SetDead | 0x3d6cdc | 140 | Target null/false, sync, forced dead state, two stops/stores, both aggro directions, skills, spells |
| CharStateMachine::SM_SetDeadState | 0x3c58c8 | 488 | Genuine animation row and masks, live great-knockback reads, direct state12/null |
| Character::GetCharAnimTableId | 0x3a3228 | 60 | Cached property2, signed/unsigned row bound, invalid fallback17 |
| CharAI::AI_SetTarget | 0x3d6890 | 556 | Existing selected source target kernel |
| CharAI::AI_SyncLastTarget | 0x3d49c4 | 12 | Target40 to last44 |
| CharAI::AI_ClearAllAggro | 0x3d5fa8 | 480 | Peer incoming erases, owner outgoing clear, peer OnDeAggro(owner) |
| CharAI::AI_ClearAllAggroTowardMe | 0x3d6abc | 544 | False branch: peer outgoing erases, owner incoming clear, owner OnDeAggro(peer) |
| CharAI::_SkillCleanUp | 0x3d8ae0 | 72 | Source skill vector iteration |
| CharAI::_SpellCleanUp | 0x3d8a98 | 72 | Source spell vector iteration |
| AISDefault::OnDied | 0x3dbe90 | 4 | Actual inherited empty Player/PlayerIPhone virtual leaf |

`Character::RaiseEvent(2,killer)` is a separate route: it forwards through `CharAI::RaiseAIEvent`, invokes the active AIS virtual `+0x24` with the killer, then raises FSM event 2. Keep that distinct from the `CharAI::OnDied -> AI_SetDead` composition above; wiring event 2 alone does not perform its target, timer, aggro, or skill/spell cleanup.

Monster lethal-hit route confirmed in the same ARMv7 IDB: `HitFor` `0x3a8bc4` reaches `v2Controller::Cmd_Kill` `0x40570c`; `Character::Ctrl_Kill` `0x3ad528` does fresh `IsDead`, `Character::Kill` `0x3a5b18`, then `RaiseEvent(2,killer)`. `Kill` owns the non-player loot gate and reward/aggro work; `DropLoot` `0x3a5ae4` reaches `ItemObject::DropLootTable` `0x3ecba0`. Event 2 reaches `CharAI::RaiseAIEvent` `0x3cbb34`, then Monster `AISExternal::OnDied` `0x3dd440`, then `AI_SetDead`. Current `model_renderer.cpp` lethal damage instead queues `pending_death` and starts `Died` in the frame loop; it skips this route. Spawned Monsters therefore remain gated until their retained Character/AIS, timer, aggro and cleanup owners are wired through this order.

Both Player vtables resolve virtual+0x24 to the real four-byte `bx lr` leaf. This does not invent a Lua OnDied or call OnTerminate. The nonempty virtual+0x3c OnDeAggro is a distinct dependency; linked providers must deliver it with the correct receiver.

IDA resolves the Ghost/AISExternal virtual+0x3c slot to inherited `AISDefault::OnDeAggro`, a four-byte empty leaf. Do not invent a Lua `OnDeAggro` callback for Ghosts.

GetCharAnimTableId's invalid branch at 0x3a3254/58 returns17. SM_SetDeadState independently checks that returned value against the fresh row count. An invalid row returns normally before changing the state, while the AI_SetDead timer/aggro/cleanup tail continues.

Death selects Died (row+0x1c, mask0x8000) or DeadlyGreatKB (+0x10, mask0x20000). Despawn rereads the live flag and selects Despawn (+0x14, mask0x10000) or DespawnGreatKB (+0x18, mask0x40000). Each selection reads the row scalar before querying `AnimStancedAnim/SL__LIST_IPHONE` from the actual animation constants. A reached stance needs the genuine inventory-backed Character::GetAnimStance; there is no empty-inventory fallback. The integer addition preserves source word wrapping.

The module stores the death/despawn fields, clears SM bytes3e/3f, clears an awaiting-revive state18 selection, then uses the existing Coordinator direct transition to state12 with event0xc358 and null payload. It stops current AI timer10 before current timer14, then stores timer14=-1 before timer10=-1. No blanket timer35 stop or extra dead regeneration gate is added.

Character target-change marker14d0 is constructor zero: C1 at0x3aa1b4 sets r8=0 at0x3aa240 and stores r8 halfword at0x3aa574 after movw14d0 at0x3aa570. C2 at0x3a9340 mirrors the store at0x3a9700. Both full constructor functions are statically pinned in the manifest; their bodies are not dynamically executed by this replay.

## Borrowed services and failure prefixes

GroupInfo is a canonical full-width pointer slot because the older logical word34 cannot carry native pointers. The constructor's null state reaches no group callback; nonnull requires a real provider. Active AIS is reread after that callback. The known Player AIS identity executes the proven empty leaf; an unknown active AIS requires its actual callback.

The typed animation provider returns actual cached property2/GetCharAnimTableId and row count, current scalar fields, fresh animation constants and conditional stance. Target uses the existing kernel through an ephemeral projection published to and refreshed from canonical AI fields at every provider call.

Both relation directions reuse `character_aggro_cleanup::clear_all`, with source-order snapshot and stable peer holds. Facts must come from the sole real map and match canonical tree7c/94 counts. Empty is a genuine source empty map. Missing linked providers fail after the target/state/timer prefix without clearing relations or manufacturing OnDeAggro. Port peer releases occur on all acquired-hold paths, including failure.

Skill and spell cleanup each invoke `player_skill_cleanup_session_v1::Runtime::cleanup` over the existing source preparation vectors and retained Session. Ordinary Lua callback failures follow the original normal cleanup branches. Required provider failures retain completed effects and stop the remaining tail.

## Proof and limits

The ARM runner executes all words of the 80-byte OnDied and 140-byte AI_SetDead callers, the selected forced/false SM_SetDeadState paths, the original target, synchronization, timer stop, both aggro callers and empty skill/spell vector callers. Eighteen cases cover ordinary/great animations, stance masks, a provider changing the live great flag, both Player virtual branches, group/no-group, invalid and fallback rows, prior states -1/3/4/5/12/18, and outgoing/incoming linked paths. Capture records473 distinct pinned instruction words and zero external import calls.

Group, Debug/string, constant/stance, `_SetState`, STL allocation/storage, OnDeAggro and individual OnSkillCleanUp are declared callee fixtures in the ARM comparison. Character constructors are static provenance only. Thus473 words is a bounded caller capture, not complete Character Kill or a full frame.

The selected host gate rebuilds the actual centrally selected world and sole script-runtime DSOs from recorded compiler dependencies. The host compares all eighteen ARM traces and outputs, injects reached target/animation/aggro/cleanup/state failures, rejects same-runtime reentry and output/error owner aliases, and checks only the two canonical AI timers stop. Real Knight/Mage/Rogue rows use the actual AnimationTables, animation constants, Debug/files and unchanged prepared Session for thirteen cleanup callbacks across twenty-one source vector slots. State external scene/controller/FX/prelude bodies remain declared callee fixtures in this gate; the separate focus adapter and root native gates prove their narrower closures.

Remaining reached dependencies include nonnull GroupInfo, unknown AIS callbacks, genuine stance inventory when enabled, linked OnDeAggro and source prior-state OnBlur branches beyond those supported by the selected state kernel. The module fails at those existing boundaries instead of treating them as success. Character::Kill rewards/trophy/online/campaign continuation and recovery are outside this adapter. Host source proof does not itself claim Android deployment or live gameplay.

IDA `AISPlayer::OnDeAggro` `0x3dde48` also updates Player aggro/music counters and may call `VoxSoundManager::SetMusicState`/`PlayMusic`; Android currently has title/menu audio only. Keep linked Monster cleanup closed until gameplay Vox routing is owned.
