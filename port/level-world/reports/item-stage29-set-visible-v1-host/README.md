# Item pool virtual40 correction

ItemManager::PreCache creates the five retained Items in each original cached
audiovisual category and calls DeSpawn on them. DeSpawn and later Spawn select
ItemObject vtable slot `0x40`, which resolves to GameObject::SetVisible
`0x38b0f0`. The native call instructions are `0x3eaca8` and `0x3eae14`.
The supplied IDA vtable export is checked by the host runner and identified in
`receipt.json`.

The pool operation now calls that visibility route. SetVisible copies the
existing enabled8a byte into visible80 when showing, and writes zero when
hiding. It leaves enabled8a, disabled373, object flags and physical filter
state alone. The pool retains its explicit updating85 store after the
visibility/inventory/physical sequence. Generic SetEnable and its original
Enabled/Disabled events remain separate.

`item_stage29_set_visible_v1.cpp` executes Stage28 -> Stage29 -> Stage30 through
LifecycleV36 and the actual Stage29 body. Item precache uses original cached
Item/AV metadata, authored itemdrops BDAE, and the retained canonical factory
receiver and visual/physical/PF graph. An ordinary non-gold/non-potion cache
row identifies the checked Item category. All 145 real pool receivers are
checked before the subsequent Projectile fixture is lent.

Level cells, current-scope validation, debug, device/network facts and
semantic-NULL Projectile inputs are declared fixtures. This verifies the
production Item precache behavior; it does not establish SWAMP-authored Item
reachability, the complete live campaign, or a current APK. The WSL ASan/UBSan
runner rebuilds its current source units and declares its older shared host
support libraries and their hashes. It never uses adb or an emulator.

Run from the repository root with:

```powershell
py -3 port/level-world/tools/run_item_stage29_set_visible_v1_host.py
```

The runner also executes the affected V8, V9, pool graph/PF/live, V23, V44 and
V47 component regressions. The latest command results, source hashes, and
concurrent-change checks are retained in `receipt.json`.

The direct regression passed 2965 checks and seven Item component regressions
passed under ASan/UBSan. The overall extended run remains FAIL at V47's
position184 expectation; see HANDOFF.md for the exact scope and results.
