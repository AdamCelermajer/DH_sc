# PlayerInfo character-level member

This source slice implements the concrete integer network-member mutation used
by `PlayerInfo::SetCharacterLevel(int)`. It is a bounded host kernel and does
not create a live `PlayerInfo` or establish the Android host-player producer.

## Recovered mutation order

`PlayerInfo::SetCharacterLevel(int)` dispatches to the virtual member at
`PlayerInfo+0x310`, whose integer member value is `+0x20` (`PlayerInfo+0x330`).
`NetStructMemberType<int>::SetValue` compares first; equal values return without
dirtying. On a change it stores the new word, then tail-branches to
`NetStructMember::SetChanged`.

`SetChanged` reads member `+0x18`, writes byte `+0x1c = 1`, copies that stamp to
`+0x10` and `+0x14`, snapshots the old process-global 64-bit change serial into
member `+0x08`, and increments the global serial with 64-bit wrap. There is no
separate notification callback in this leaf.

The wrapper's 168-byte ARM body also reads its local temporary's `+0x20` before
writing it. The runner exposes that initial stack residue. If it differs from
the incoming signed int, source first marks the temporary, then calls the
member's virtual `SetValue`; this can advance the global serial once for the
temporary and a second time for a changed PlayerInfo field. If the residue
matches the input, the temporary mark is skipped. Native callers must not
assume the residue.

## Source producer evidence

`PlayerManager::_ManageCharacters` checks whether its PlayerInfo has the
backing `CharProperties`; if so it compares `PlayerInfo+0x330` with
`CharProperties::PROPS_GetInt(19,false)` and calls `SetCharacterLevel` on a
mismatch. The getter arithmetic-shifts the fixed property word right by 8 and
returns an ordinary integer. The manager passes that result directly without
another fixed conversion. `PlayerInfo::Reset` also calls the setter with `-1`.

This proves an authored field-reconciliation route and the setter's effects.
It does not prove which PlayerInfo is hosting in the Android reconstruction,
when the network manager populates its backing properties, or how the live
`PlayerManager`/`PlayerInfo` lifecycle should be instantiated. Native OnInit
fixtures remain explicit and must not be treated as a runtime producer.

## Validation

Run:

```powershell
python port/player-info-level/tests/run_character_level_member.py
```

The runner executes the original `PlayerInfo::SetCharacterLevel`,
`NetStructMemberType<int>::SetValue`, and `NetStructMember::SetChanged` ARM
instructions. Only the vtable and global serial bindings are synthetic; the
dirty-field and serial mutation instructions run from the original image. The
report is
`port/player-info-level/build/character-level-member/validation.json`.
