# `_UpdateAggro` flat CharacterList source path

The aggro candidate query invoked by `CharAI::_UpdateAggro` is a global
`ObjectSearcher::CharacterList` scan. It is not a `RoomObjectList`/PFRoom
enumeration. `_UpdateAggro` constructs a 16-byte ARM32 IObjectList wrapper
with the CharacterList vtable address point `0x964870`, sentinel
`ObjectManager+0x60`, current pointer `sentinel->next`, and end pointer equal
to the sentinel. It calls `TargetList::Search(float,float,IObjectList&)` at
`0x4a3428` with the chosen radius, a full-circle cone, and that wrapper.

The 100-byte source overload fetches the owner's source target position and
look vector, then passes the point-query overload at `0x4a2f34` the center,
radius, look vector, cone, and original IObjectList reference. The pinned
point-query code invokes the list virtuals in this order:

| Vtable byte offset | Method | Effect |
| --- | --- | --- |
| `+0x08` | `Reset` | current = sentinel.next; end = sentinel |
| `+0x0c` | `AtEnd` | current == end |
| `+0x18` | `Get` | current node value as `GameObject*` |
| `+0x1c` | `GetChar` | current node value as `Character*` |
| `+0x10` | `Next` | current = current.next |

The source std::list node fields used by those methods are `next` at byte 0
and value at byte 8. The C++ port exposes these as normalized borrowed
projections rather than binary overlays. It stores a projected `Character*`
in each normalized entry; `Get` returns its projected `GameObject*` and
`GetChar` returns the same Character projection. The query reads the current
node's next link only after the synchronous candidate service callbacks, so
it does not snapshot the list's links before scanning.

`character_aggro_character_list.*` adds a parallel flat-list entry and search
API. The prior `character_aggro_target_search.*` RoomRegistry API remains
unchanged for its historical scope. The new CharacterList query does not
call its `resolve_character` service because the CharacterList already
provides the Character pointer. It preserves the same all-flags/filter-2,
visibility, zonable-byte, interactive, property-word, adjusted-range,
full-cone, and closest-heap gates as that older kernel.

## Verification

Run from the repository root:

```powershell
python port/level-world/tests/run_character_aggro_character_list_host.py
```

The runner verifies the pinned original ELF SHA, exact symbol ranges and
CharacterList vtable relocations. It compiles/runs three host suites; it also
executes the original ARM32 `Reset`, `AtEnd`, `Get`, `GetChar`, and `Next`
instructions on a two-entry sentinel list. It executes the original
`Search(float,float,IObjectList&)` overload through its actual call into the
point-query overload and asserts the position, radius/cone, look vector, and
stack-passed CharacterList pointer.

The runner stops at the point-query call boundary. It does not execute the
1,172-byte candidate-query body in Unicorn because that path enters game-global
diagnostic/runtime services not initialized by this fixture. The new host
candidate query is tested against the historical one-room kernel for equal
candidate IDs, ordering, and adjusted distances; a live link mutation test
checks source-style post-callback `Next()` behavior. This does not prove
ObjectManager add/remove ownership, native wiring, live Android acquisition,
or the full point-query ARM body. Those remain separate integration gates.
