# Same Crypt canonical publication V4

The renderer now retains one CanonicalObjectManager/PropertyMap/publication
owner on its existing WorldScriptContext. Existing NPC receivers are adopted
without replaying constructors, property defaults, VM loading, or FSM setup.
The player uses PlayerSpawnMetadataV4's actual internal-input0 source name and
archetype and CanonicalPlayerFacetV3 over the existing player object/runtime.
Publication precedes CharacterWorldRuntime registrations; registrations borrow
the published shared Handle and its key, rather than deriving keys from identity.

The old CanonicalObjectManager constructor omitted its original Flush tail.
Original C1 34a404 calls Flush3496b8 at34a60c. The constructor-empty branch
inserts the reserved null map node0 (349944..349958), then stores next-key1
(34995c..349960). This is now reproduced; no zero-key resolver weakening was
introduced. Existing-actor publication also now uses a read-only name-conflict
preflight. Its old create=true reservation was followed by Add's second lookup,
which correctly skipped unpublished nodes and allocated an extra source key.
Add is now the sole allocator. Failed publication cannot be accepted as a
completed restore merely because its Handle/map mutation prefix exists.

Source network Add reads the same live EquipmentPlatform online fact. The
actual offline AssignObjectNetworkId3431c0 branch returns before network actor
mutations. Online transport remains a required boundary. Across-rooms87 remains
unavailable for the player until its actual producer runs; global room−1
publication does not fabricate it.

Cold replacement/deactivation shuts down gameplay consumers, drops canonical
manager receiver leases, then releases the player facet World pin. A GL-only
restore retains the same metadata/manager/publication and source bytes81/84.

Validation:

- Full current renderer syntax passed arm64-v8a and x86_64.
- Android `canonical-manager-constructor-v4`: reserved null0, first key1,
  next key2, source count0 before Add.
- Android `canonical-retained-actor-adoption-v3`: same identity/Handle/props/life,
  mutation preflight, exactly one source allocation/publication and no retry.
- Android `world-item-pool-pf-v4`: actual145 cache-backed Item graphs,
  2352 checks, source constructor-null PF user prefix, suspend/rebind,
  source SceneManager membership/release, actual POItem/Box2D/filter lifecycle.
  Empty geometry/registry fixtures are explicit and are not read by that
  null-user branch. Device/condition/Debug endpoints remain explicit fixtures
  in this test; drop/award and positive world navigation are not claimed.

The pool proof does not establish live CtrlKill acceptance. Its current APK
and exact source hashes are recorded in each native test receipt.
