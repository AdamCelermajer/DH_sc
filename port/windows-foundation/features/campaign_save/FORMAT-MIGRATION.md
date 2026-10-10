# Campaign checkpoint extension proposal

The existing `GameSave` remains the single authority for canonical CharacterState,
inventory, equipment, actor vitals/property sheets and combat RNG. Its version-1
actor checkpoint normalizes temporary behavior to idle/dead. It cannot reconstruct
a source actor in Limbus, Spawn or AwaitingToSpawn, a running script, trigger
state, native quest registration or condition bindings.

This feature does not change core GameSave, write a file or enable campaign saving.
It provides complete-source coverage checks, original QEST/ObjectBase producers,
detached quest loading, and an explicit whole-world transactional restore boundary.

## Proposed version 2 outer format

Root owns the core format migration. Retain the current canonical record encoding
once, then append a bounded source extension table inside the same checksummed
file. The file magic/version must identify version 2; never append these records
to a file claimed to be version 1.

Each extension entry should encode a stable authored owner key, source definition
revision, codec version, payload byte count and payload bytes. Keys are authored
level/module/object names or persistent player identity with explicit collection
selector; runtime addresses, leases, vtables and handle registry tokens are absent.
Sort by owner key/codec for deterministic output. Limits in the proposed in-memory
snapshot are 4096 sections, 16 MiB per section and 64 MiB total source bytes.
Existing whole-file checksum and atomic replacement should cover canonical data
and extensions together. This feature introduces no separate asset-side save file.

Version-1 migration is valid only for its existing ordinary matching-roster
checkpoint policy. A source campaign requiring lifecycle/quest/script records
must reject a version-1 file as incomplete; do not fill absent fields with guessed
initial state. Unknown version, duplicate key, missing requirement, changed
definition revision, unknown codec, malformed native payload and trailing data
must fail before live publication.

## Required source producers

The host enumerates every registered source owner and all fragments required by
its actual original save/restore method. Coverage is exact; an ObjectBase fragment
does not prove that an entire Trigger/Character is restorable.

Implemented source payload adapters:

- QEST collection delegates `quest_save_collection_v45` using the SAME regular or
  volatile QuestSavegame writer supplied by the native quest adapter. Saved data
  consists of difficulty counts, stable quest indices, original quest state and
  definition-dependent objective cells, then progress IDs/act. Fresh detached
  `QuestPersistenceOwnerV51` receivers load it; count mismatch and incomplete
  consumption are failures even though legacy count mismatch can return success.
- ObjectBase delegates `object_base_serialize_v3` for produced `visible80` and
  `enabled8a` bytes. Source restore rebuilds condition tested caches; neither those
  caches nor condition-list pointer tokens belong in the file. This is only a
  fragment and has no standalone live restore factory here.

Actor lifecycle, trigger-zone remainder, script context and game events remain
explicitly unsupported requirements. Requesting them rejects capture/restore.
Do not serialize OriginalCampaignRuntime's private Context/TriggerState or the
in-house lifecycle status as an alleged original campaign checkpoint.

## Restore ordering and ownership

Validate canonical data, actual immutable source revisions, exact coverage and
every native payload first. The restore factory then constructs a detached whole
world/profile: canonical inventory, actor roster, source lifecycle owners, physics,
quest objectives/prerequisites/registration, script owner, triggers and collision
contacts. All service/backend work that can fail belongs in this stage. Borrowed
native Character identities may be used during reconstruction but are never saved.

`PreparedCampaignRestore::commit()` is one noexcept publication of the fully
staged owner graph. It must include canonical GameSave state and RNG in that same
publication, not call ordinary `restore_game_save` and then several source loaders.
If any source owner cannot stage/rebind its relationships, preparation fails.
Destroying the prepared object without committing leaves the live graph unchanged.
The core and native owners must actually implement this contract before the
campaign save UI can advertise success.

The detached QEST helper intentionally does not register native objectives,
publish a player/profile, synchronize regular/volatile state, or bind conditions.
The shared quest adapter/native profile remains the eventual publication owner.
