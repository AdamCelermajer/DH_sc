The live adapter borrows the existing CombatSession, PlayableActorWorld,
ActorPopulation, and retained source actor owner. It creates no follower world,
controller, Lua VM, haste animation, distance threshold, or timer store.

`LiveWorldBindings::owner` must return the same world ActorState pointer and a
lease for its original Character identity, master cell and PF path list. Source
GetTargetPosition uses the same original own-node/cache/position selector;
population enabled status is the actual activation producer. Missing own-node
construction, cache production, host player, path, master or state fails.

`live_invoke` implements same-owner queries and requires complete original
controller/skill operations for commands. Successful SetTarget, ClearTarget and
SetMaster must publish to the same borrowed fields or the adapter fails. The
source path list count is the actual HasPath result. FindPath's `found` flag,
physical collision flags and state animation are not used as substitutes.

For the canonical V1 record path, `dispatch_source_rene_character_event_v1`
authenticates the same retained record, active AISExternal session,
`TargetBindings`, CharAI+0x50 fields, and record-based CampaignFsm path receipt
before dispatching a reached source Character event. The receipt pins the
original `actor.runtime.path` and FSM context across callbacks such as
`MoveTo`/`HasPath`; it does not copy, mutate, or search a second path. Target
callbacks use the same `CharacterScriptSession::dispatch_target`; master
callbacks use the same Session VM and actual optional VCB aliases. The
`dispatch_live_rene_character_event` bridge remains for already-projected
`LiveWorldBindings` call sites. Neither bridge generates events.

The existing CampaignFsm target/master frame kernels remain the producers for
range, sight, and master transitions. Their `raise` route must deliver the
selected AIS virtual at the root event coordinator; root should enroll the
record callback above at that reached endpoint, without adding another
per-frame target/master update or replacing `SourceCharacterNpcContextProvidersV1::bind_events`.
This feature does not create range/sight events or a second FSM.

`bind_live_native` is a registration provider for the actual
`owner_register_binding` boundary. It verifies the same pending/active VM,
binds known companion native names, and leaves unrelated native bindings with
their existing providers. Retain NativeBindingOwner through that session's
lua_close. The original Character object provider must already be established.
StartTimerCB must retain the original Lua function in its actual scoped timer
owner; source buffs, level setup and other skill services remain required.

The world bridge compiles and `live_companions_tests` passes native assertions
for actor identity, PF path predicate, master/host relationship, own-node/cache
selection, disabled-actor position, target/master publication, absent command
services and detached actor rejection. These are executable same-world fixtures,
not a claim of a playable full CharacterScript owner. The script integration
translation unit compiles but requires the real ScriptOwnerV2 and script runtime
libraries at link time. No new VM or fake service is linked to hide that boundary.

Current Windows root still needs to pass the actual retained master/path/script
owner borrows into these seams, prove PF identity publication, and provide full
original controller/skill/timer services. The only located Swamp placement is
`_prim_NPC_PriestGood` / `WanderingPriest` / AI50 / `rene`; source does not
establish that this is named Castor. Priest dialogue and
faery embedded particle clouds remain with the interaction and effects owners.
Faery glow is embedded authored visual content, not an assumed EffectsTables row.
