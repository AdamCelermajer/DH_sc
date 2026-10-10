# Item virtual40 fix handoff

Implemented the original ItemManager Spawn/DeSpawn SetVisible operation.
Native Item vtable slot 0x40 resolves to GameObject.SetVisible 0x38b0f0; the call
instructions are DeSpawn 0x3eaca8 and Spawn 0x3eae14. The runner verifies the supplied
IDA vtable bytes and records their SHA-256. Native PFObjectC2 0x524538 writes 8
to flags+4, so ordinary precache preserves that flag as well as enabled8a.

Production edits (all under port/level-world):

* character_loot_item_manager_v8.hpp/.cpp: source operation set_visible 0x38b0f0;
  hide/show delivery keeps the original explicit updating85 stores.
* character_loot_item_source_v9.cpp: SAME source-inventory Spawn show route.
* world_item_object_owner_v1.hpp/.cpp: appended set_visible operation preserves
  existing operation ordinals and invokes the same retained receiver.
* world_item_graph_v3.cpp: routes set_visible to the real visibility owner,
  copies enabled8a verbatim to visible80 for true, and preserves enabled8a,
  disabled373, physical filter and object flags. Generic SetEnable is unchanged.

Corrected tests (all under port/level-world/tests):

* character_loot_item_manager_v8.cpp
* world_item_drop_inventory_v9.cpp
* world_item_live_owner_v5.cpp
* world_item_pool_graph_v3.cpp
* world_item_pool_pf_v4.cpp
* world_loot_canonical_bindings_v44.cpp
* world_loot_positive_v23.cpp
* loot_root_v47_cached.inc

New direct regression: tests/item_stage29_set_visible_v1.cpp.
Host runner: tools/run_item_stage29_set_visible_v1_host.py.
Evidence: this directory's receipt.json.

Verified under WSL ASan/UBSan using 69 rebuilt current source units and declared
older shared host support (hashes recorded; never a current whole-engine claim):

| Test | Checks | Result |
| --- | ---: | --- |
| Direct Stage28 ->29 ->30, ordinary cached Item0/category1 | 2965 | PASS |
| WorldItemLiveOwnerV5 | 2345 | PASS |
| WorldItem pool graph V3 | 1916 | PASS |
| WorldItem pool/PF V4 | 2357 | PASS |
| Canonical loot bindings V44 | 1884 | PASS |
| Positive loot V23 | 1628 | PASS |
| Item manager V8 | 3682 | PASS |
| Source inventory/drop V9 | 176 | PASS |

The direct regression uses original cached AV/schema and itemdrops BDAE and
retains the real canonical factory receiver. All 145 Items preserve enabled8a,
disabled373, filter26 and original PF flag8, while hide clears visible80 and
the manager clears updating85. It also verifies raw enabled8a=7 copying and
the independent genuine SetEnable condition path. Level/debug/device/network
and semantic-NULL Projectile leaves are explicit fixtures. This is production
Item precache behavior evidence, not SWAMP-authored Item reachability, a
current APK, or whole campaign acceptance.

The overall nine-test runner receipt remains FAIL because the extended V47
test reached a separate position184 expectation:

```
source.invoke(source.context,&fixture.target,&request,&result)!=0
&& extension.error().find("position184")!=std::string::npos
```

No further work on that extension assertion was performed after the parent's
explicit scope stop. Preserve the eight positive results separately from the
overall runner status. Sources were unchanged during the recorded test run.
No Projectile production edits or emulator/device actions occurred.
