# Canonical factory and shared PropertyMap V1

The callable native prefix is ObjectManager constructor, ordinary/special-name
query, GetNewObject class lookup, Add and whole LoadFromXML ordering. It is not
the all-class InitPost/InitFinal/condition/save-restoration lifecycle. Both ABI
syntax checks pass. Root's standalone map/Add test passed on emulator5554;
its receipt identifies receiver/network fixtures. The PropertyMap executable
test is ready for the same runner and has not yet been run by this lane.

## Binding ownership

Keep one `CanonicalObjectManagerV1` in the candidate's actual World composition.
Its map/list nodes pin `CanonicalObjectBorrowV1.lease`; they own no second life,
FSM, inventory, controller, animation, position or scene. Supply the same
receiver shared Handle, room64, typef4 and produced byte87 to both the manager
and existing World registration. A key0 is valid: the source next-key counter
starts0. Empty handles also have key0 but cachedNULL/frameUINTMAX. Key alone is
not a validity test.

Constructor creates the retained receiver. Factory then writes the actual
catalog name through `class_name20` (source34b5d0), before Add. `Character` and
`Player` both construct GO_ID0 but have separate class-schema keys. Container
GO_ID7 and AnimatedDecor GO_ID20 are not their catalog ordinals25/1.

Keep one **application shared** `CanonicalPropertyMapV1`, because original
class/template descriptors are globals and shared between actors and floors.
Bind each actor's actual class-name20, template-name8 and typed source-offset
writers through `CanonicalPropertyActorV1`. `RetainedCharacterActorV1` exposes
`properties()` over the same receiver. Chest bindings must use their same
base/container fields. Descriptor offsets/defaults come from the executed
original declaration streams, not DACT kinds or arbitrary XML keys.

Bind `source.position_rotation_default` to
`&canonical_vec3_origin_v1()`. The complete original Point3D global initializer
312e00 executed without fixtures, overwriting a sentinel with Origin(0,0,0),
I(1,0,0), J(0,1,0), K(0,0,1). Source `LoadTemplate`51419c is only assertion9.
For a nonempty template provide the actual assertion-policy/debug bridge;
there is no recovered external template XML parser to substitute. Empty
SetTemplate is a source no-op and retains the current name.

## Loader adapter and ordering

`canonical_loader_request_v1<ObjectEntryV1>` retains the real loader XML entry
by value, source module occurrence, offset and **actual runtime module ID**.
The occurrence is diagnostic only. Missing XML and empty strings stay distinct.
It keeps filter string and source context alive with the XML lease.

For each retained request create one `CanonicalObjectFactoryAttemptV1`, then
execute with constructor/property/position class services. Its attempt object
must outlive callbacks and failed-candidate cleanup. Do not retry a delivered
attempt: its stage/prefix identify actual completed mutations.

The constructor returns a real receiver lease and class-name field. The
manager handles duplicates by invoking the new receiver's actual deleting
destructor, retaining the existing canonical actor, and then properties run
against that existing actor, as in original source. Required network assignment
failure leaves the map/count/character-list prefix intact; it does not report
a completed actor. Missing ordinary query executes real Debug load/query;
Player/LocalPlayer aliases require real GetLocalPlayer(0,true), HighestThreat
requires its whole original service.

Class services call shared PropertyMap:

1. `init_properties(actor.properties())`.
2. `set_template` only if XML template is present (even empty).
3. `load_defaults`.
4. `load_overrides` over registered descriptors in sorted key order, including
   missing attributes. Missing means SetToDefaultValue, not skip.
5. Only LevelConfig receives early virtual InitPost.
6. Actual IsGameObject gates post-property module-offset addition and source
   SetPosition(...,true). `_prim_PlayerLight` becomes `_prim_PlayerLight_S`
   and uses room/index−1. General class InitPost is a later manager pass.

The property map preserves source `_templateName` effects: defaults save and
restore it; override iteration caches the starting descriptor tree, but each
SetProperty resolves against the current name again. A missing `_templateName`
can therefore reset the name before subsequent overrides. Bool parsing is
atoi!=0 (`true` becomes0); int uses atoi; float uses strtod then float; vector
parsing collapses comma delimiters and retains zero for absent components.
Vector strings exceeding source strcpy256's safe domain fail after its actual
zero prefix. An unavailable typed writer fails at the reached field. No
successful empty field provider is installed.

## Remaining production contracts

The Character lane owns genuine visual/physical/InitPost/InitFinal receivers;
the container lane owns OpenableContainer runtime and same base visual/PODecor.
The source application still must compose actual network-ID assignment,
GetLocalPlayer, assertion/debug, conditions/events and all reached class
constructors. Unsupported declaration classes remain explicit required
providers; their PropertyMap schemas are not guessed.

Candidate commit must wait for the later initialization/condition/save passes.
Before releasing receiver leases, clear actual World/script/physical registry
borrows through genuine Remove/Flush/controller/body cleanup. This map module
does not implement source ObjectManager::Remove/Flush and its C++ lease release
is not a substitute for those callbacks. Preserve the existing level until
candidate cleanup/commit completes. No full SWAMP50+5 runtime acceptance is
claimed by this prefix handoff.
