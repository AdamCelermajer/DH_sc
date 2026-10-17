# Character physics-position virtual

`GameObject::Stop` calls the virtual at byte offset `+0x64` only when a
physical object exists. This slot is `IsUpdatingPositionFromPhysics`, not a
general movability test.

The pinned original image proves two implementations:

- Base `GameObject` vtable address point `0x964750` has slot `+0x64` at
  `0x9647b4`, targeting `0x34006c`. Its complete two-instruction body returns
  1.
- `Character` vtable address point `0x965f38` has slot `+0x64` at `0x965f9c`,
  targeting `0x3a2e44`. Its three instructions load `Character+0x520`, extract
  bit 1, and return that 0/1 result.

`Character::Character(ObjectBase::GO_IDS)` (`0x3a9340`, 1448 bytes) loads the
`_ZTV9Character` address from GOT entry `0x9978a0`, adds 8 for the Itanium
address point at `0x3a9404`, and stores that primary vptr at the object start
at `0x3a9418`. The pinned Ghost is a `Character`; this evidence does not show
a Ghost-specific C++ subclass or separate Ghost vtable.

The small `character_physics_position` adapter retains the full-width owner
identity and borrows the live 32-bit flags word. It delegates bit decoding to
the existing `dh2_move_policy`, which already reproduces the source flag
mapping. Source Idle flags `0x2380` and Move flags `0x23c1` both have bit 1
clear. In these two states, Stop therefore skips its velocity/position/body
reset branch. This says nothing about other states.

The host runner executes the original base and Character getter bodies, checks
all 65,536 low-word patterns plus high-bit/seeded full-width flag values, and
verifies the two vtable entries, their relative relocations, and the constructor
address-point/store bytes. Its portable replay checks high-width Character
identities, non-aliasing guards and exact 0/1 output.

Reproduce from the repository root:

```powershell
python port/level-world/tests/run_character_physics_position_host.py `
  --original-elf ..\test_strategy\libDungeonHunter2.so
```

This is the Character getter and exact vtable binding only. The caller still
needs to resolve the current Character object and its live flags from native
ownership. It does not reconstruct the Ghost controller frame, state-specific
flag producers beyond the cited Idle/Move prefixes, physical setters, or the
other GameObject derived-class overrides.
