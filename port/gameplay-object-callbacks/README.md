# Read-only GameObject and Character callback projections

This isolated host module ports six small value reads from the original Lua
callback layer. Its views are new port types; they are not the original native
object layout, a Lua userdata binding, or Android gameplay integration.

## Recovered callback behavior

The registration evidence is `reports/lua-registration-trace.json`; the
corresponding original Ghidra pseudocode exports are in
`recovered/native/decompiled/libDungeonHunter2.so/`. ELF addresses below are
from the callback registration/decompiler index. These exported C-like files
are generated reverse-engineering evidence, not the game's original source.

| Lua binding | Original ELF routine | Observed callback operation | Port view input |
| --- | ---: | --- | --- |
| `GameObject:GetID` | `0x38ebe4` | Passes callback `self` to `ReturnValues::pushPointer` | Returns the same pointer token; it does not invent an integer ID |
| `GameObject:GetName` | `0x38ebf0` | Pushes the string pointer stored at `this + 0x44` | Borrows the name pointer from the view |
| `GameObject:GetPosition` | `0x38e700` | Pushes floats at `this + 0x160`, `+0x164`, `+0x168` | Copies the supplied world XYZ floats verbatim |
| `Character:GetState` | `0x3b6d78` | Calls `CharStateMachine::SM_GetState` at `this + 0x4fc`, then pushes the ID | Requires a pointer to the authoritative current state value |
| `Character:GetStateTime` | `0x3b6d6c` | Pushes the integer at `this + 0x55c` | Requires a pointer to the owner-maintained state-time value |
| `Character:GetHitCount` | `0x3b6cc8` | Reads a 16-bit value at `this + 0x14d0` and pushes it as an integer | Promotes the caller-supplied unsigned 16-bit value |

The registration report was produced from the supplied native ELF; its
`original_sha256` is captured in the host evidence JSON. Pseudocode function
offsets and byte lengths are read from the checked-in `function-index.jsonl`.
The host runner checks each callback's exact registration scope/address and the
body fragments above before exercising the owned port views.

## Port boundary

`GameObjectView` takes a borrowed name and a borrowed three-float world
position. Existing `port/actor-runtime::ActorInstance` values can supply those
two fields after the MLX/MGP loader has resolved module origin. The host test
uses the owner-supplied Infected Village cache and checks the four statically
authored Ambush Characters against their imported names and world positions.

`CharacterView` additionally requires the current state ID, state time and hit
counter from their real owners. The module does not set these values, choose
defaults, simulate state transitions or advance time. It only permits each
getter to read an explicitly supplied value. The normal Lua diagnostic actor's
`GetState`/`GetHitCount`/`GetName` remain authored test controls and are not used
to support these callback claims.

This boundary does not implement `IsDead`: its original callback dispatches a
virtual method (`vtable + 0x34`) on the concrete runtime object. Nor does it
turn `MoveTo`, `Attack`, or `Kill` into immediate changes. Their recovered
wrappers enqueue commands on `v2Controller`; target and range helpers delegate
to `CharAI` and inventory owners. They must be connected to those systems
before being callable in gameplay. No physics, collision, path planning,
renderer state, animation, combat or Lua `ReturnValues` are emulated here.

## Build and run

From the repository root, with Python 3, GCC/Clang, and the separate owner cache:

```powershell
python port/gameplay-object-callbacks/tests/run_host.py `
  --cache ../cache/files `
  --report port/gameplay-object-callbacks/build/validation.json
```

The host test uses strict warnings, imports the Infected Village MLX and its
two MGP modules through existing readers, and verifies name/position reads on
four cache-authored Characters. It also checks raw state/time/hit values,
unsigned 16-bit hit-count promotion, pointer identity, output preservation on
missing state owners, and direct pass-through of stored float values. The
runner statically rechecks registration and pseudocode evidence. The cache is
read in place; no cache or Android source is changed, and the test does not
exercise Android or claim a playable game.
