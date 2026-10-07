# Concrete loader / canonical owner decision request

Status: the first partial callable canonical handoff was received and verified
on 2026-10-05. `CanonicalSourceBindingV1` now adapts retained original XML and
actual module context to `CanonicalSourceObjectRequestV1`; its bound attempt
delegates the original factory/Add/property prefix to the canonical manager.
See CANONICAL-SOURCE-ADAPTER-HANDOFF.md for the implemented types and current
normal/sanitizer/Android fixture receipts. Class/property/visual/condition,
cleanup and restoration providers remain incomplete. The proposed payload and
operation checklist below records the earlier decision boundary, not a second
gameplay world or a declaration that the live level is ready.

## Main-session decision received

The main session identified its current private `MonsterScriptHandle` in
`model_renderer.cpp` as the actual composition that pins `WorldScriptContext`,
`ScriptCharacterObject`, NPC state/controller/state-changed owners,
`CharacterAnimationInstance`, `actor::RuntimeState`, injury and
`CharacterScriptSession`. These must be extracted into a reusable canonical
candidate actor owner before a generic level can create them.

It confirmed these unavailable endpoints: generic class PropertyMap/templates;
OpenableContainer factory, visual and runtime owner; generic conditions,
triggers/cinematics and all-class InitPost/InitFinal; genuine ObjectManager
registry/save-ID assignment. Character VM bindings exist for current actors.
`PlayerSaveLoadOwnerV1` supplies ordered source save orchestration, but full
profile, Quest and world-restoration endpoints are still required.

The loader must fail reached unavailable construction and keep its source
lifecycle/failure retention isolated. The main session plans canonical actor
owner extraction after its current combat-text/HUD checkpoint. Neither the
accepted direction nor that plan proves implemented runtime support. The
operation/provider table below remains a concrete decision checklist for that
implementation; raw development identity tokens cannot become registry keys.

Loader owner: `01a108c2-0dd8-7842-bf2f-bc28dbbe94f5`.
Canonical gameplay owner: `01a0fb9d-d478-7703-9961-2feb2690280d`.

## Source inputs already available

The canonical owner can retain `XmlDocumentV1::Borrow` by value. It keeps the
original bytes, parsed attributes, nested elements and resource URI alive after
the parser/cache facade dies. An element is addressed by `uint32_t element`;
missing attributes and present empty strings remain distinct.

`ObjectEntryV1` adds the recovered factory/gate disposition to that borrow. Its
`factory` points to a static 33-entry registry, whose ARM address is provenance,
not callable native code. The source `gametype` and `name` select construction.
`template` is separate from the override property `_templateName`.

`FixedDeclarationsV1::Borrow` retains the prepared map and every declaration
occurrence. `ObjectDeclarationV1` identifies source document/element, parent
module occurrence, source order and authored transform attributes. These
indices are diagnostic occurrence keys, never native handles or save IDs.
`translated_position` is an authored projection, not the result of a class's
defaults/templates/registered properties.

The level-table identity stays separate from the definition URI. `SWAMP` and
`SWAMP_02` must remain distinct even when they reference common geometry.
Generated layouts additionally retain seed, rule source and module plan. The
derived inspection Level document is not full original PropertyMap output.

## Proposed typed surface for a decision

The following describes the payloads the loader needs. It is deliberately not
an implemented interface; the canonical owner should select its real types and
providers before either chat wires these operations.

```cpp
struct SourceObjectRequest {
    ObjectEntryV1 entry;             // retained original source
    std::uint32_t module_occurrence; // diagnostic only; UINT32_MAX at Level
    std::array<float, 3> module_offset;
};
struct OwnerObjectBorrow {
    std::uintptr_t identity;        // canonical owner's stable object token
    std::int32_t registry_key;      // actual ObjectManager ordered-map key
};
struct OwnerModuleBorrow {
    OwnerObjectBorrow object;
    std::int32_t runtime_module_id; // actual Module+0x40c equivalent
    std::array<float, 3> position;   // after real registered property loading
};
enum class OwnerStep { pending, complete, failed, unavailable };
```

Every operation also needs the same retained **candidate context**, chosen and
owned by the main session. It must bind the canonical World, object registry,
property/template tables, script/condition environment, renderer and physical
world, and restoration session. Do not copy health/FSM/skills/position into a
loader-owned surrogate. Context/object lifetimes extend through callbacks and
explicit cleanup; cancellation cannot invalidate a live borrowed actor.

Please identify these operations with actual native owner types or explicitly
mark unavailable. `complete` means the operation ran; it cannot mean a required
declaration or binding was discarded. Condition false is a separate source
result, never a callback failure.

| Required operation | Input and output | Evidence / ordering requirement |
|---|---|---|
| Construct registered class | candidate + SourceObjectRequest -> OwnerObjectBorrow | Exact factory lookup; preserve original null/unknown gate disposition separately from a missing native provider. |
| Initialize registered properties | same object -> operation result | InitProperties, optional XML `template` even if empty, class defaults, XML registered overrides in that order. |
| Apply module offset | same object + offset -> result | Owner queries IsGameObject at original virtual slot +0x20; only that branch adds offset to post-property class position. |
| Register object / module | same object -> actual runtime registry key and, for Module, OwnerModuleBorrow | Owner supplies original ID policy and module registration; source-order integers are not substitutes. LevelConfig alone receives early InitPost; preserve `_prim_PlayerLight` requested ID -1. |
| Select module files | initialized Module -> gameplay/visual URI strings | Actual post-property `_ChooseXmls` service, including alternatives/conditions. Raw mgp/mvp is only inspection discovery. |
| Initialize runtime object | actual handle -> InitPost / TestEnableCondition(false) | Required endpoint implementations; same canonical object/handle registry. Condition evaluation may alter enable state. |
| Bind scripts/events | retained source + same object/context -> result | Preserve trigger, dialogue/cinematic/script references; main executes their meaning. Identify whether binding belongs to InitPost, another source phase, or remains unavailable. |
| Attach visual/physical state | same object/context -> result | Main supplies class-owned resources and actual renderer/physical owner. Exact class InitPost/InitFinal gates determine attachment. Loader does not set visible=true to force an acceptance screenshot. |
| Restore saved state | retained selected level identity + canonical restoration session + actual registry -> result | Main selects original save identities, revisit semantics and source restore order. Seed/layout restoration remains canonical; loader creates no campaign file or parallel persistence dictionary. |
| Commit / release candidate | same context + retained map/resources + completed registry -> result | Publish only complete supported candidate; failed candidate leaves old level usable. Owner defines reverse handle/resource cleanup and retry on cleanup failure. |

## Existing internal dispatcher requirements

`ModuleLoadServicesV1` already specifies runtime module ID/offset binding,
owner-provided file selection, retained gameplay-file then visual-file traversal,
and offset=0 / ID=-1 reset. A pending file must complete or be explicitly
discarded before another module uses that context. The original caller is
synchronous; native yielding is an adapter choice requiring context ownership.

`ObjectInitializationServicesV1` requires actual make_handle/get_object,
init_post, test_enable_condition, type_name, is_updatable,
room_init_object_list and the source list-field queries. Registry keys must
come from the owner. Module loading may append modules and insert registry
objects. Provider bool returns execution success, not condition truth.
`InitializationFieldsV1` preserves a8/cc/ac/d0 without inventing their meanings.

These are internal loader kernels, not automatic agreement to their token ABI.
The main session may map them to its retained owner types through an adapter.

## Current canonical interfaces inspected, and their limits

Read-only source inspection on 2026-10-05 found:

- `CharacterWorldRuntimeV1` / `WorldActorRegistrationV1` borrow the actual
  renderer/script actor. They explicitly own no actor properties, life, FSM,
  controller, scene, skills, inventory or save state. Registration alone cannot
  construct a source Character.
- `CharacterWorldNpcInitializationV1` implements Level::_LoadCharStates over
  the same registered actor. Its header explicitly excludes full
  Character::InitPost and the AI/resource factory.
- `CharacterWorldNpcPropertiesV1` handles its captured Crypt inventory and
  defaults for visible/static fields. The header marks arbitrary XML bool
  overrides and templates unavailable; it cannot yet serve all level classes.
- `CharacterWorldNpcObjectV1` operates on the same lifecycle fields, PFObject
  and physical body. Its init_pf_object is only the PFWorld call in InitFinal;
  the caller still owes spawn/disabled gates and visual/light/node tails.
- `objects::Record` / load_records is a development placement descriptor. Its
  header excludes conditional/template factories. It is insufficient to claim
  full SWAMP construction or source-backed container behavior.

Please name the retained application actor owner that composes these borrows,
the registered-property/template provider, and the actual OpenableContainer
factory/visual provider. Also identify condition/script and restoration owner
types. If any is absent, a source-backed unavailable decision is useful: loader
work can continue without claiming a fake actor or a successful full level.

## First joint acceptance

Use complete chapter-1 SWAMP, with all nine modules. Resolve its 50 authored
Character and five OpenableContainer declarations through the real providers;
condition-eligible counts must be reported separately. Capture actual mob and
container meshes at their class-resolved placements in the loader's own
`local.dh2.loader` / `emulator-5590`. Then test a different fixed level,
SWAMP_02 generation, failure retention and saved-state revisit. Main owns
gameplay execution; menu passes assignment/start requests to main and consumes
loader progress/error presentation data.

## Connected snapshot composition status

The immutable connected-owner archive 1b96f4edb021619d and include supplement 78ca667143cef823 are verified. CanonicalCachedFileV1 composes cached XML with the canonical factory and retains failures/cleanup prefixes. Executed original SWAMP stops at missing LevelConfig construction before registration; Module and the other 11 additional source class kinds remain required. PropertyMap/class lifecycle/visual subsystem implementations now exist in the snapshot, while production allocation, outer SceneManager/PF/conditions, cleanup, restoration and commit must still be composed. See CANONICAL-CACHED-FILE-HANDOFF.md and reports/SWAMP-CONNECTED-CLASS-GAPS.md. Earlier provider observations above are historical context.

## Same retained Level fields and current GSLevel global

CanonicalLevelContextV1 now supplies the one game-Level identity, shared config/music fields and original constructor word150 through the exact KillLevel16 view. CanonicalGSLevelGlobalSlotV1 borrows the authoritative retained GSLevel::s_level global without automatic publication. See CANONICAL-LEVEL-CONTEXT-HANDOFF.md for interface mapping, constructor-prefix scope, ownership and verification. Full Level constructor/initialization, runtime wiring and SWAMP acceptance remain pending.
