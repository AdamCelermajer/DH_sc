# Lane 09 handoff: enemy death timer publication

**Source delivered.** The NPC death path now publishes the exact `CharAI::AI_SetDead` timer-ID reset (`timer33` and `timer34` become `0xffffffff`) to the retained Script/CharAI lifecycle before reciprocal aggro callbacks can synchronously reenter that same NPC. This closes a stale-live-timer window during lethal damage; the existing source cleanup order and the single TimerStore remain authoritative.

Changed files:

- `port/level-world/npc_death_owner_v2.hpp`
- `port/level-world/npc_death_owner_v2.cpp`
- `port/android-native/app/src/main/cpp/renderer_npc_death_v2.inc`

Evidence: IDA pseudocode at `0x3d6cdc` and `0x3d1000`, plus original ARM assembly at `003d6d34–003d6d44`, show both IDs reset immediately after the two timer stops and immediately before `AI_ClearAllAggro`; `OnDied` reaches this whole body after its optional GroupInfo and selected AIS OnDied calls. The renderer hook copies the reset at the `ai_death_clear_all_aggro` service boundary, before invoking the original reciprocal-clear owner.

Existing interfaces retained: `NpcDeathOwnerV2::on_died` remains the sole CharAI OnDied/AI_SetDead path; `NpcSkillReactionsV2` still borrows the same NPC FSM/TimerStore; root retains renderer/native-app lifecycle and Hit8/kill wiring. The appended optional service callback preserves existing aggregate initializers.

No build, test, emulator run, APK integration, or gameplay verification was performed. Source delivery is not verified lethal gameplay. Remaining acceptance is root wiring this owner into the existing Kill/SkillApply paths and exercising lethal melee and skill damage in an integrated APK, including state-12 cleanup, target/effect release, loot/XP ordering, and finite death animation.
