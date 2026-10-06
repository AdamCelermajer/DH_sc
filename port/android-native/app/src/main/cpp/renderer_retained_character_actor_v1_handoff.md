# Sole retained Character graph migration

Add `retained_character_actor_v1.cpp` to level-world. Include its header in the
renderer. Replace the common fields of `MonsterScriptHandle` with inheritance:

```cpp
struct MonsterScriptHandle : dh2::character::RetainedCharacterActorV1 {
  std::shared_ptr<WorldScriptContext> context;
  MonsterScriptHandle(std::uintptr_t id,std::shared_ptr<WorldScriptContext> world,
                      const std::string& actual_catalog_name)
    : RetainedCharacterActorV1(id,world,actual_catalog_name),context(std::move(world)) {
    bodies={this,state_body};
  }
  ~MonsterScriptHandle() override { close(); }
  // Keep every existing static renderer callback here unchanged.
};
```

Remove the duplicate common object/machine/facts/predicates/bodies/diagnostics/
controller/AI/animation/runtime/bounds/injury/session fields. Existing callbacks
still access those exact inherited fields. The destructor closes the VM and
graph while derived callback context exists; the opaque base World pin then
outlives the derived context. Do not move World callbacks into a generic loader.

Factory construction is `make_shared<MonsterScriptHandle>(identity,context,entry.name)`.
Pass `handle->canonical(handle)` to the canonical ObjectManager. Its whole Add
publishes the SAME Handle and writes source name/archetype/room. Publish byte87
only from actual registered ObjectBase property default/override results; the
constructor did not write that byte. `canonical()` deliberately returns a null
byte87 borrow until that producer runs.

Create the `ScriptCharacterObject` after Add using `source_name()`. Its const
name is a VM/diagnostic snapshot of the already committed source CString. Use
the SAME actual PropertyState/CombatActorState owners. Then:

```cpp
handle->facts.idle=actual_idle_sequence;
handle->diagnostics=std::make_unique<StateOwnerDebugDiagnostics>(actual_debug,actual_files);
WorldNpcStateServicesV1 services{};
services.remaining_methods={handle.get(),MonsterScriptHandle::state_method};
services.outer={handle.get(),MonsterScriptHandle::native_frame};
services.diagnostics=handle->diagnostics.get();
services.diagnostics_required=1;
if(!handle->construct_graph(same_object,actual_resources,services)) fail(handle->error());
// Script input keeps existing genuine commons/monster includes, Host, Level,
// target/object services, commands and application timer service descriptors.
if(!handle->construct_script(context->design.borrow(),input)) fail(handle->error());
// Allocation above intentionally does not load Lua. At source InitPost3cf1f0:
if(!handle->load_script()) fail(handle->error());
```

The methods are single-attempt: a failed prefix remains retained, and the
caller must destroy the candidate rather than retry/duplicate a VM or FSM.
Borrow `&handle->shared_handle()` in World registration; remove the separate
WorldActor Handle storage for these characters. Canonical Add and World query
must mutate/observe one field. Player migration requires its own retained owner
and is not implied by this NPC extraction.

This is construction/lifetime extraction, **not full Character::InitPost**.
Current DACT registration/animation/session/spawn-vitals ordering remains a
development initialization path until the recovered InitPost coordinator is
composed. Original InitPost 3b4d60 sets byte1394 BEFORE CheckSpawnProbability;
it includes LoadBaseProperties/Recalc, GameObject::InitPost, MeetCondition,
SG_Load, script loading, FX registration, animation setup, sound initialization,
Revive, physical initialization and later script/group tails. Those operations
must not be silently replaced by `construct_graph` or repeated on GL reload.

Verification: strict ARM64 and x86_64 production compilation passed against
the current renderer's actual compiler flags and include order. Runtime graph
host test is separate; compilation alone does not prove complete InitPost.
