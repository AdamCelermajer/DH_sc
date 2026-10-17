# Recovered PFObject path capability mask

The original ARM32 `PFObject` stores path capabilities at object offset
`+0x14`. Its constructor sets the default mask to `2` (swimming allowed,
flying clear). `SetFlying` toggles bit 0, `SetSwimming` toggles bit 1, and
`CanPathOn` accepts a null floor only as false, accepts any floor with a zero
type mask, and otherwise requires every floor requirement bit to be present in
the object mask:

```text
(floor_type_mask & object_path_mask) == floor_type_mask
```

The playable `Character` inherits this baseline through `GameObject`/`PFObject`;
its `KnightPlayerBase` properties do not override it. Lua can still mutate the
mask at runtime, so `2` is the newly constructed player's starting state, not
a guarantee about every later gameplay state.

## Original-code differential test

`tests/can_path_on_arm.py` loads an externally recovered copy of the original
`libDungeonHunter2.so` into Unicorn and executes the native `CanPathOn` and
flying/swimming setter instructions against bounded synthetic object/floor
memory. It also executes the actual constructor block up to the store at
`PFObject+0x14`. It does not run a full constructor, instantiate a game world,
or validate native navigation integration. The original binary is not copied
into this source component; follow [provenance and rights](../../RIGHTS.md).

With Python 3, `unicorn`, `pyelftools`, and the external original ARM32 ELF:

```powershell
python port/pf-object/tests/can_path_on_arm.py `
  --original <path-to-original-armeabi-v7a-libDungeonHunter2.so>
```

The checked original library SHA-256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The
current run passes 285 `CanPathOn` calls, eight setter calls, and 294
assertions with zero mismatches.
