# Original interpolation policy and registration producer

This bounded trace identifies the original interpolation argument producer. The
hash-identified ARM32 ELF is a desktop test oracle only. Production files and
historical static-compiler reports were not changed by this trace.

`ISceneNodeAnimator::E_INTERPOLATION_MODE` string table at `0x99b868` contains
`Default`, `Step`, `Linear`, then null. Original `getStringsInternal` at
`0x667c38` returns that table. Thus their integer values are 0, 1, and 2.

The inherited complete-object constructor `0x669828` writes zero to `this+0xc`
at `0x6698cc`. Its base-object constructor `0x6698fc` writes zero at `0x66999c`.
The game `AnimatorSet` complete constructor `0x3676b8` calls engine
`CSceneNodeAnimatorSet` base constructor `0x660ce4` at `0x367714`; the engine
constructor calls the inherited base constructor at `0x660cfc`. Neither later
constructor initialization nor the game applicator construction changes the
interpolation field. `AnimSetManager::GetAnimator` at `0x4762c4` constructs a
fresh game AnimatorSet at `0x476334`. The two-slot controller constructor
`0x476b4c` calls GetAnimator twice at `0x476ba4` and `0x476bb4`.

Original `getAnimationValue` at `0x65f7b4` reads the field at `0x65f7c0`, then
`0x65f890..0x65f89c` produces the fifth accessor argument as `field != 1`.
Default=0 and Linear=2 therefore enable interpolation; Step=1 disables it.
Unknown field values also enable it. This establishes the constructor-produced
policy needed by the Prince two-slot sampler; it is not a justification for
discarding the source cached-key cursor behavior.

Original `init` at `0x660af4` and `setCurrentAnimation` at `0x65f8c8` preserve
the field. Inherited `serializeAttributes` at `0x667cb4` and
`deserializeAttributes` at `0x667cb8` are each a single `bx lr`. No named
interpolation mutator exists in this ELF symbol table. The captured game
selection/callback and blended controller scale/play methods contain no
interpolation-field write. Nested `this+0xc` writes in callback setup belong
to the returned timeline/event manager, not the AnimatorSet field. This is a
bounded live-path audit, not a proof against every possible indirect external
write anywhere in the game.

`probe.py` executes actual inherited, engine, and game constructors, engine
init and selection, and the original policy caller. The game constructor uses
a valid immutable zero-target database fixture; timeline and applicator
constructor instructions execute. Allocation is an explicit service fixture.
The selected segment and downstream sampler are explicit fixtures only in the
focused policy-argument probe. No pose arithmetic parity is claimed here.
Seven constructor cases, four init/selection preservation cases, eight no-op
attribute cases, and six original sampling-argument cases pass. The existing
separate static raw sampler corpus already executes the actual typed accessor
bodies for both interpolation booleans.

## Registration and the newly found dynamic compiler boundary

`CAnimationSet::addAnimationLibrary(database)` at `0x6601d4` calls actual
database-vector `push_back` at `0x62ecac` and returns the appended index.
The focused probe inserts IDs `248,241,243,241,328` into a preallocated vector;
the actual original instructions retain that order and return indices 0..4.
The URI overload `0x6600fc` delegates to virtual slot `+0x2c` after loading.
There is no ID sorting in these registration methods.

The game resource producer is **dynamic**, which differs from the static
compiler previously reconstructed. `AnimationSet::CreateAnimSet` at
`0x364ca0` constructs `CDynamicAnimationSet` via `0x3648c4` and sets mismatch
behavior to 1 through virtual slot `+0x1c` at `0x364cf0`. Its constructor
default is mismatch 0, but the game producer changes it to 1. `GetAnimator`
executes virtual slot `+0x38` when resource `+0x70` is dirty; the target is
`CDynamicAnimationSet::compile` at `0x62f61c`.

That compiler iterates registered libraries in vector order (`0x62f658`) and
each library's serialized animation records in increasing index
(`0x62f678..0x62f6b0`). Its `addAnimation` at `0x62f354` builds an ordered
copied 16-byte SChannel union at `+0x74`, using the original compatibility
table and URI comparison. In the supported full node types 1/5/10 the table's
submatrix is identity, so their deduplication key is exact URI and type.

Dynamic strict mismatch 0 removes a target if any registered library has
neither a matching track nor a clip-database default
(`0x62f708..0x62f978`). Mismatch 1 retains those targets. This pruning happens
before final bindings and does not consult the designated default library.
Dynamic binding construction first queries each clip database. If its default
is null, it queries the designated default database at `+0x68` for **both**
mode1 and mode2 (`0x62f850..0x62f86c` to `0x62f7cc`). The static compiler
`0x660710` instead uses a transformation-template fallback only for mode1.

`AddTemplateAnim` at `0x476398` first loads/registers its animation through
`0x3659ec`, then loads that record's resource and passes its database to
`setDefaultAnimationLibrary` at `0x62fc90`. It does not install the static
transformation-template filter. `AnimationSet::_UpdateAnimationIndices` at
`0x364af4` traverses the game ID map but looks up each already registered
database index via `0x62dbb8`; it does not reorder the library vector.

Actual Prince ID-registration schedule still depends on the character caller
and asset registration table. Supplied input bank order remains an explicit
boundary until that producer is recovered. The dynamic source distinction is
reported to the parent and playback worker; the static TransformSet proof is
not relabeled as dynamic Prince parity.
