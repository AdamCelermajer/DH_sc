# Existing Crypt world canonical publication

This is development adoption of the existing DACT actors, not XML construction
or proof of a complete ObjectManager/Level runtime. Keep the existing actor,
properties, life, animation, VM and FSM allocations.

WorldScriptContext must retain one CanonicalObjectManagerV1, one
CanonicalPropertyMapV1 and one CanonicalExistingActorPublicationV3 referencing
that manager. The Item factory and its 145-slot manager borrow these same owners.
Creating the property map does not deliver InitProperties/defaults to old actors.

Perform adoption during load_world after the existing ScriptCharacterObject and
retained receiver names have been published, before CharacterWorldRuntime actor
registration. For each NPC use canonical_adoption_v2 with the real source name,
archetype and room, then publication.publish with pointers returned by
source_name/source_archetype. Do not use the normal guarded canonical setters
after the VM was constructed. Preserve the source list insertion order.

CanonicalPlayerFacetV3 borrows the same player ScriptCharacterObject and
prince_runtime. It requires caller-produced source Handle, type, room, across-room
byte, catalog class pointer, name and archetype. Current player runtime does not
retain all of these fields; the facet rejects that absence. A Character table row,
Save class or selected menu row cannot supply a missing ObjectBase catalog/name
producer. No second base/Character or copied health inventory is permitted.

Source Add reserves the name-map node, publishes the SAME Handle and receiver,
then sets strings/room, appends AsChar/Module lists, performs the requested network
assignment and finally invokes the publication observer. The adoption helper
rejects changing inputs before this prefix and prevents a second attempt after
partial failure. Network policy must come from the actual caller; passing false
solely to skip an unavailable source AssignObjectNetworkId is not a valid binding.

Register target-world actors only after successful canonical publication. Their
handle_key must read the same shared Handle key, not a cached previous registry
key. If an already registered actor must be migrated, stop input/target queries
and rebuild the registry as a whole; sequential reindexing can collide with old
player/NPC keys. Preserve actor identities. No independent source Handle copy is
introduced. The manager's published observer is the correct successful Add tail
for language/name registry publication.

GetLocalPlayer/TestEnableCondition must use the actual retained phase manager.
An initialized but empty PlayerManager legitimately returns no Character; that
branch continues to current Level difficulty. If a Character exists, source
33e70c..720 loads Character+14e8 directly and tests byte14 of that Save object.
It does not load Save.profile+8. Missing Save/zero byte14 returns unchanged.

The fixture canonical-existing-actor-publication-v3 passed on isolated Android
native execution (binary 6e965f37b1f470e8b4f618c02372ab4170e2ac053b236ff4abb3e840e84fed1b).
HUD's canonical-retained-actor-adoption-v3 fixture additionally composes the real
retained NPC adoption borrow. Neither fixture establishes live renderer adoption,
network assignment, player metadata production or whole Level readiness.
