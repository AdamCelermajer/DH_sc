# Generic canonical receiver transport

CanonicalReceiverTransportV1 removes the three-class source-dispatch restriction without implementing game classes. The main session explicitly authorized this bounded loader transport on 2026-10-06 and continues to own the actual LevelConfig/Module/RoomZone/visual/PF and actor constructor/initialization providers.

It passes the original CanonicalFactoryEntryV1 and CanonicalSourceObjectRequestV1 to one caller-supplied catalog constructor. Every registered type can reach that constructor, including LevelConfig, Module and RoomZone. The caller must return its actual CanonicalClassReceiverV1, with the canonical object lease, same Handle/class-name storage, typed properties and reached virtual/position continuations. Missing providers fail; this transport provides no replacement actor, schema, InitPost success, renderer, condition evaluation, current Level slot or world publication.

## Wiring

Use the SAME existing CanonicalPropertyMapV1, constructor/provider storage and CanonicalObjectManagerV1. Construct the transport with that property map and CanonicalReceiverTransportServicesV1 (owner lease, raw context, actual constructor callback and unknown-type Debug callback). Pass transport.services() to the existing canonical source factory/cached-file/Module-file relay. The existing CanonicalObjectFactoryAttemptV1 retains catalog lookup, class-name assignment, Add, properties, the special early LevelConfig InitPost, IsGameObject and module-offset ordering. The transport owns no manager and never calls Add or constructs a second world.

The actual class constructor owns its class implementations and constructor-side mutation. The transport retains each returned identity and source lease, including a constructor's valid failed prefix. A standard constructor exception is converted to failure while retaining an already assigned receiver prefix. Reused live constructor identities are rejected without replacing their records. A constructor reporting success without an actual identity/lease or Handle/class-name/property producer fails visibly.

Property operations delegate directly to the supplied shared property map using the actual receiver's freshly produced CanonicalPropertyActorV1. Lifecycle and position operations delegate only to that receiver's supplied continuations. Each synchronous call pins the object lease and callable so a deleting callback can erase its dispatch record while its own call safely finishes. This does not postpone registry unpublication or invent destructor behavior.

The actual deleting destructor/removal calls erased(identity) after unpublication. Duplicate Add erases only the newly constructed duplicate identity; the original canonical manager returns the existing receiver, whose retained dispatch then receives defaults/overrides/offset. receiver(object, out, error) provides a synchronous borrowed dispatch view for main's existing Spawn/old-receiver services, including a failed constructor prefix. Pin the object/candidate and do not retain that raw view across erased calls.

The constructor/provider owner must pin the property map and context but must not itself own this transport; keep them as siblings in the outer candidate aggregate. The actual candidate/source lease pins the transport through calls because CanonicalClassServicesV1 has a borrowed raw context. No copy/move of the transport is allowed. Actual failed-candidate cleanup remains main-owned and precedes cached XML journal discard.

## Build and layering

Layer after canonical-source-reentry-handoff-ddc6fa292573bde5.zip (SHA256 b0f8bfca22076750a474ba427e1b277732f7dc307f3ecdce6bd85181b245233f) and its documented Module/SAME-Level/canonical source prerequisites.

Production supplies its existing DH2_LOADER_CANONICAL_OWNER_TARGET, exporting the current canonical receiver/property headers and implementations. Then link dh2_loader_canonical_receiver_transport, with the new header/cpp. The target is intentionally available only with that existing owner: the legacy standalone factory-only snapshot does not export the required class/property interface. Preserve current production CMake and add the small new target block surgically; do not replace it with the bundled standalone CMake.

The reproduction target tests/cmake-receiver-transport uses an immutable current-root owner snapshot under vendor/receiver-transport-owners-d4ff4a071e22a6db. Its ten files and manifest are exact read-only current-source captures, including current property declarations and Item/pending-list-compatible manager APIs. The older connected-owner snapshot lacks the newer property field/list interface; it is not mixed into this build. Do not copy the reproduction vendor sources over newer main implementations or link another manager into the game.

## Evidence and remaining work

Fifteen checks execute actual catalog, manager, property-map and canonical source code with explicitly declared class/field/continuation, ZERO-default, network-ID and duplicate-destructor fixtures. They cover all 33 catalog entries reaching the constructor, failed/exception/incomplete prefixes, live-identity rejection, source lease retention/release, missing continuations, same receiver property/pose dispatch, callback self-erasure and real manager duplicate Add preserving the old identity. The generated Character-only fixture tests routing/offsets; it does not instantiate game characters.

The unfiltered original SWAMP MLX reaches the supplied constructor with the genuine LevelConfig catalog entry (0x340ca8), exact original XML source and first factory prefix. The explicitly unavailable constructor returns failure; no object is registered and retry does not replay. This verifies removal of the transport's dispatch restriction without pretending LevelConfig construction or the whole Swamp succeeds.

Host, ASAN/UBSAN/leak and Android x86_64 native execution on only emulator-5590/DH2_Loader_API37 all pass. ARM64 compiles. Exact binary/source/cache/fixture/owner hashes are in reports/canonical-receiver-transport-checks.json. Run tools/build_receiver_transport.py with host, sanitizers, x86_64 and arm64-v8a, then tools/run_receiver_transport_checks.py. Android files use /data/local/tmp/dh2-loader-receiver-transport-v1; no APK or other emulator is changed.

Main's actual class constructor/continuation wiring, complete Module InitPost and Level initialization, spawning, scene publication, save restoration and visibly rendered SWAMP mobs/chests remain required. Catalog forwarding is not proof that those classes or the full generic loader work.
