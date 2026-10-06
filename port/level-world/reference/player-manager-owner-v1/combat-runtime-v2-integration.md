# Same-manager combat lookup

`PlayerManagerCombatRuntimeV2` owns the existing `PlayerManagerOwnerV1` map and
PlayerInfo scalar projections. It does not allocate actors, properties, Save,
inventory, or controllers. Hit1 executes original330794: GetLocalPlayer(0,true)
then reads that record's Character660. Hit11 uses GetPlayerByCharacter and the
same record's local66c. Apply20 returns that record identity.

Root's current direct Crypt launch has no authored NativeStartGame inputs.
Its explicit development request is {internal0,controller0,local_index0,localtrue,
same existing Character,null optional base-id}. Source AddPlayer creates the
map entry and computes ordinals. Adoption publishes the already-created same
Character at the source3722ec field store. It does not call or complete whole
AddCharacter; source factory/Save/skills/controller tails remain required.

The development initializer produces only Reset373bdc scalar fields. Embedded
CNetPlayerInfo/network-property constructors are not completed or claimed.
Offline GetLocalPlayer/GetPlayerByCharacter never read these embedded fields.
Online platform query is still real and mandatory; reaching online routing
requires its original services. Optional base-ID is used only by later class
count queries; absent producer must remain absent.

Integration: include header and renderer_player_manager_v1.inc; add runtime
unique_ptr member stated there. Bind once after same player_object is available,
before constructing execution facade, using the explicit request above. Set
combat_services.hit={&t,player_manager_hit_v2}. At application provider start,
call t.player_manager->application(*q,out): handled1 maps0, negative maps-1,
zero continues existing provider. Preserve its error when negative.

The include also resolves Hit6 through the same retained Application
SavegameManager. Original320e14 first calls hasOption46d4a8, then
isOptionToggled46d418. The latter requires type0 and stored value equal to the
descriptor maximum; an absent option returns0 through the source lookup.

This resolves Hit1, Hit6 and local-player/player-info lookup. Source
controllerKill/remote query, ApplyFX/state/sound/AI and whole
lethal Kill tails remain mandatory. No successful missing provider is supplied.
