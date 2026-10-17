# ObjectManager flat Character-list ownership

`ObjectManager::Add` owns a circular list sentinel at `ObjectManager + 0x60`.
The pinned ARM32 node is 12 bytes: next at `+0`, previous at `+4`, and the
`Character*` at `+8`. The sentinel is empty when both links point to itself.
`Add` first resolves the source name lookup. A resolved existing object takes
the duplicate branch and calls byte-offset `+4` on the incoming object's
vtable, then returns without appending. On the normal path, Add stores the
object, obtains its handle, and executes `ObjectHandle::operator Character*`.
Only a non-null Character result allocates a list node and appends it at the
sentinel tail. The source code has no pointer-identity deduplication check.

The source call chain is pinned: `Spawn` calls `GetNewObject`, and
`GetNewObject` calls `Add`. The XML loader also calls `GetNewObject`. The host
runner checks these exact ARM `BL` edges in addition to the function ranges.

`ObjectManager::Remove` resolves the `ObjectHandle`, removes its room-list
membership, converts the same handle to `Character*`, then scans the flat
Character ring. At each node it saves the next pointer before testing the
value. On a match it rewires both neighboring links, frees the 12-byte node,
and continues from the saved next pointer. Thus every occurrence of the
target pointer is removed and the order of nonmatching entries stays intact.
Characters are borrowed values in the list; this node-removal pass frees the
list node, not the Character itself.

## Runtime adapter boundary

The maintained `Owner` is a normalized host/native representation, not a
binary overlay on ARM32 `std::list` memory. `CharacterCursor` operates directly
on its stable `Node` ring. `cursor_next` reads the current node's live `next`
link only when called, after candidate callbacks. It does not snapshot the
whole list or reinterpret another standard-library implementation's private
node layout. `cursor_get` and `cursor_get_char` expect the node value to be a
stable `dh2::character::aggro_search::Character` projection with a non-null
GameObject projection.

## Original-code oracle

Run from the repository root:

```powershell
python port/level-world/tests/run_character_aggro_object_manager_list_host.py
```

The runner verifies the complete pinned ELF hash, exact symbols/ranges and
bytes, vtable bytes, and the `Spawn -> GetNewObject -> Add` / XML caller edges.
It compiles and runs the normalized owner and live-link cursor checks, then
executes the original ARM32 `Add` body for repeated same-pointer appends,
resolved-name duplicate handling, and the non-Character gate. It executes the
original ARM32 `Remove` body through the flat-ring scan and
`CharAI::RemoveFromGroup` call for remove-all/order-preservation and no-match
cases. The fixture supplies explicit owner services for name lookup, global
handle resolution/construction, map storage, node allocation, and node free;
the original list manipulation and dynamic-type methods run as machine code.

The ARM `Remove` run stops at `0x348fec`, after the flat-list scan and group
removal call but before unrelated manager-map and total-object cleanup. The
fixture uses empty room/other-manager rings. This does not prove full object
destruction or production manager initialization. The separate point-query
body at `0x4a2f34` still requires its own full ARM candidate-search oracle; the
older CharacterList report stops at the call boundary.
