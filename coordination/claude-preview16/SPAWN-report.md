# SPAWN report (p16/spawn): generic spawn character from template

Status: IN PROGRESS (skeleton written first; updated as work proceeds).

## Investigation (evidence, AGENTS.md rule)

### Original logic (IDA pseudocode, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `GameObject::_Summon` (pseudocode line ~108187). Caller gate: caller state word `*(caller+...)`==3 and CharacterTable index < size (`Arrays::CharacterTable::size`). Arg 0 = CharacterTable row index (getUInteger), arg 1 = bool `spawn` (v15).
- Position: `summon_spot` node absolute position if present (VisualObject::GetSpecificNode), else caller position (a3+88..90). Optional offsets: when arg5 is bool true, offset is applied in the caller's look frame (GetLookAtVec; arg2 lateral, arg3 forward, arg4 up); otherwise arg2..4 are absolute coordinates. Floor: PFWorld::GetFloorHeightAt; no floor -> caller position.
- Creation: `Character::CreateNPC(row, 0, nullptr, ...)` (0x3ad1f8 / 0x3ad064), `Character::SetInitialPosition(npc,pos)`, `GameObject::SetPosition(npc,pos,1)`, `GameObject::SetRotation(npc, caller Euler)`, sets npc+5348 = 1 (summoned flag), then `RoomZone::AddInitialObject` of the caller's room; if refused (or no room) `ObjectManager::AddNoRoomObject` + flag +751 = 1 and `GameObject::ZoneEntered`.
- Spawn state: if arg1 (`spawn`) is true -> `CharStateMachine::SM_SetSpawnState(sm, 0, 0)` (same SetSpawnState(false,false) that Script_SpawnCharacter uses, i.e. PreSpawn17 -> Spawn1 with the Spawn clip).
- Forward anim only when the 3-number form is used with non-zero arg2 (v19). Online: CMsgSpawnObject when online state is 3/4 (offline: no effect).
- Returns the new NPC as userdata.

### Visual evidence
- Reference (v1.0.3, not yet frame-sampled for this task): lizard intro entrance at 226 s / 228 s stills per `runtime-enemy-source-spawn-admission-handoff.json`. Spawn-clip frame sequence: see "Visual" section below (filled after capture).

### Existing owners (port)
- Population: `ActorPopulation::load` (actor_population.cpp), `select_template_profile` (CharacterTemplateTableV78 weighted draw, caller RNG), `ActorProfileLibrary` (actor-profiles-v2.xml).
- Session: `CombatSession::initialize` builds ONE session from the population (no runtime add API exists; `PlayableActorWorld::bind_actor/remove_actor` exist below it).
- Lifecycle: `OriginalActorLifecycle` (add/spawn/animation_finished/combat_enabled). Combat enables only at state 3 with enabled+physical+collision flags (main.cpp ~1604).
- Enemy AI: `bindEnemyAI` decision provider applies to all session actors; permission provider gates combat.

### Data (CharacterTable, rc3-equivalent shared assets)
- `Swamp_Moth_Minions` row 370: AI 41, AnimTable 44, ModelFile 66, Scale 50, LevelMin 256, LevelMax 512, no Level field. No actor profile in actor-profiles-v2.xml (profile XML only has Swamp_Moth_Type1 / _RESPAWN / Swamp_MovieMoth for model 66).
- `Swamp_Moth_Type1` row 371: AnimTable 42, ModelFile 66, Level 256, LevelMin 256, LevelMax 768.
- Lizard rows: `Swamp_LizadMan_Type1` (profile present).

## Limits (updated at the end)
