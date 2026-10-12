# Character Lua movement and combat commands

The captured original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` and the matching assembly bind complete Stop, HeadTo,
MoveTo, Attack, Flee and HasPath wrapper bodies. These are native source wrappers;
the oracle executes the original ARM32 only during development verification.

`character_script_commands.hpp/.cpp` retain argument type rejection, ordered
Value.getNumber conversions, absolute and relative point arithmetic, raw target
selection, borrowed getter pointers and zero-return command semantics. One-object
HeadTo and MoveTo both call the original controller MoveTo object endpoint. Flee
requires an object argument but computes its destination from the Character's
current target. HasPath projects the original nonempty path-list test. No path
count is inferred from movement or target presence.

Zero-argument HeadTo/MoveTo reaches an original assertion followed by invalid
argument storage access; native code returns an explicit -3 boundary. Required
service failure returns -2 after already delivered effects, without rollback.
No missing controller, pathfinding, attack or position backend is accepted as
successful. Native identities retain all 64 bits.

The original/O2 ARM64 differential report records 4,196 cases and 3,649 ordered
requests. It includes object/nil arguments, boolean and numeric conversion,
absolute/relative coordinates, signed zero, infinity and NaN. The original
Value.getNumber instruction body executes for the supported primitive cases;
its string dependency is an explicit numeric-result fixture. Source Vec3f_K and
heading are supplied fixtures; the static vector initializer is not verified by
this report. Numeric conversion of fixture identities uses their original
32-bit numeric identity values, not native pointer truncation.

Frozen production header SHA256:
`51f22d25809578af83f6da1f07b4cacbe2be6dbd2e1d5e35eec7b0477089e3c9`.
Frozen implementation SHA256:
`2d352d1f464a59a1591503f93a788c7321f95be503a440f1615eaf8318f43770`.
SCM1 gold SHA256:
`17ef355f7aa490056f40daaa19be3931364dd9bdb66b66d3afbcfad7fbce194c`.
Optimized ARM64 oracle SHA256:
`cbb9c0d917787f554f753a6788a66277dd2f1c8031faf8a8997d11c26accca78`.

Reproduce the oracle with `tools/build_character_script_commands_oracle.ps1`,
then run `tests/character_script_commands_differential.py`. Preserve the historical
report and gold before an intentional new source revision; that script writes
the fixed report and corpus paths. The actual main CMake target is
`character_script_commands_audit`; pass `command-fixtures.bin` as its sole argument.
Its ASan/UBSan replay has 170,449 checks, eight guards and zero mismatches.

The additive CharacterScriptSession command binding occupies the exact original
registration positions. The real original monster OnEnemySpotted callback runs
through SetTarget/HeadTo into the actual reconstructed ControllerCharacter and
PathTo kernels in the host session audit. FindPath returns zero through an
explicit failed-navigation provider; Character event and attack deliveries in
that test also use explicit providers. This establishes command composition,
not live scene navigation or full enemy AI. The renderer has not yet supplied
these optional command/target/FSM/object backings, and no new tested APK is
claimed by this source stage.
