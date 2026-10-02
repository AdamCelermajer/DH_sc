# Owned actor death metadata and requests

These development methods expose the checked non-player/null-killer native Kill
projection. Their names/arguments are authored controls; original Lua registration
and a complete Character/death state machine are not claimed.

## SetDeathContext(context)

Required fields: booleans `dead`, `network`, `suppress_events`; unsigned integer
`target_id`; signed-short integers `property_id`, `template_id`.
All numbers are float32 Lua values; the target ID must be an exactly integral
representable number in [0, 2^32). Property/template IDs are in [-32768, 32767]. An authored
context reset may reset dead; this is not original resurrection logic.

New property actors start alive, network false, events enabled, target ID 0,
property ID 0 and template ID -1. These metadata defaults are authored.

`IsDead()` reports this owned flag. Health setters alone do not reset it.

## KillNonplayer(policy)

Required policy booleans: `forced`, `loot_manager_present`. Required signed 32-bit
integer constant IDs: `kill_enemies`, `clear_enemies`, `kill_template`,
`clear_template`. Use the imported real `v2QuestObjectiveType` values from
`v2quests_pycst.bin`: 0, 1, 10, 11 respectively.

Returns a table with booleans `processed`, `dead`, `drop_loot_requested`, signed
raw `drop_loot_id` and ordered `events`. Each event has source `kind` (0..3),
unsigned `target_id`, signed `objective_id` and signed `match_id`. The match ID is the property ID for kinds 0/1 and the template ID for 2/3. Source
requests do not create inventory loot or advance quests until consumers exist.

The actor's retained property dataset generation drives HP/loot properties.
Bad arity, missing fields, wrong types, unsafe casts or invalid metadata/data
raise controlled errors. Raw table reads avoid metamethods. Property and dead
state commit only after the entire return table has been allocated, preserving
state when allocation or validation fails.

The existing ApplyNonplayerHit takes an explicit target_dead policy; callers
must supply the owned flag via IsDead when using these development APIs together.
A lethal hit returns a death request; KillNonplayer executes the next projected
step. Original controller notifications, synchronous event observers, resolved
killer/threat credit, XP, actual loot/quest consumers, animation/FSM/world and
full source gameplay remain pending.
