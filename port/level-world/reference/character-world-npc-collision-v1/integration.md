# NPC physical and collision integration

The new modules borrow one registered Character world actor, the existing
`MonsterScriptHandle::object`, `session`, `machine`, `ai_events`, and retained
controller. They do not allocate properties, health, targets, an alternate FSM,
or a second paused byte.

Retain `WorldNpcAISCollisionFieldsV1` on each actual AIS owner. Its two zero
initializers are the recovered AISExternal constructor stores at `+bc` and
`+c0`. Retain **one** `WorldNpcCollisionGlobalsV1` on the world: it is the source
AISDefault common collision-count word, not a presentation counter.

Construct `CharacterWorldNpcCollisionV1` after registration and publication of
the actual ScriptSession identity. Pass the existing controller's mutable
`command_state(controller_global_blocked)` backing. The helper rejects a
different published AIS, property/life/target/FSM, or controller owner.

Its clock callback borrows actual Application frame and dt. Unknown AIS
collision overrides require a real callback; the inherited default methods
are implemented locally. `object_type` is required only if the persisted
collision reaches the noncharacter `ObjectBase+f4` branch. Current NPC
collision-to-character paths use the actual registered World classification
and relationship services. Source SetTarget uses the existing object's target
binding, with its combo halfword writes committed to the existing combat
backing before synchronous services.

Prepare `CharacterNpcBodyModel` from the actual BRES bytes and the retained
complete CPU visual Scene, as specified by the existing model helper. Retain
`CharacterWorldNpcPhysicalV1` at a stable address. Its `WorldObject` context is
installed on the genuine body and shapes; moving the owner is forbidden.

Pass a `NpcBodyRequest` with the same Session `property_view()`, actual design
AI table, actor position/rotation, and genuine static/group/debug predicates.
The new helper supplies its own physical allocation identity and actual
Character owner. `initialize` creates the real body, shape, mass, source pin
and assignment prefix, then calls required `update_pf`. A missing PF producer
returns false with `phase()==3` and preserves the attached native body. This
is a failure-prefix result, not completed NPC physical initialization.

Three physical services remain explicit:

- `peer_owner` resolves any real physics context to its actual physical owner.
  Existing player/decor contexts need a stable registry; do not cast all peers
  to the NPC class.
- `enabled80` borrows the actual `ObjectBase+80` byte for default collision
  tests. Visibility inferred from animation or renderer scene presence is
  insufficient.
- `update_pf` executes the source PF attachment/update from genuine PF object
  fields, body radius, geometry and capabilities. It must not invent flying,
  obstacle or boundary flags.

The byte80 writer is recovered: `ObjectBase::SetVisible(33dcf8)` and
`GameObject::SetVisible(38b0f0)` write false directly, or read enabled byte8a
when requested true. Character's actual virtual slot40 is GameObject's
setter; slot3c is `setUpdating(33dcf0)`. `Character::Enabled(3a598c)` delegates
to GameObject's Enabled, which invokes both setters, adjusts PF flags, wakes
the actual physical object and clears byte373. The initializing source caller
and the complete visual/PF wake services must be composed before publishing
this byte as a live producer. The constructor does not initialize byte80.

`native()` is the sole real NPC body to borrow for Stop/Pin/Unpin and physical
subobject services. `release()` must run before World clear and before closing
the borrowed VM/FSM/AI owners; DestroyBody still delivers source collision End
events. A live owner cannot be destroyed with a failed callback and silently
leave dangling shape contexts.

Validation: all eleven original Crypt placements, real monster VMs and three
BRES models pass O1/O2 ASAN/UBSAN host composition. All eleven genuine bodies
are created and destroyed. Missing PF, missing byte80 and unknown AIS override
paths fail explicitly. The moving FSM IDs, Application clock and byte80 used
to exercise collision branches are declared fixtures. The original callback
audit covers 96 whole POCharacter cases and eight AISExternal constructor
poison cases; it is not a whole native differential claim. Both new modules
pass strict ARM64 NDK compilation. No full NPC AI, movement, renderer
connection or successful PF initialization is claimed.
