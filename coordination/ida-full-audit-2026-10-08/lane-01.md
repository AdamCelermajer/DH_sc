# Lane 01 — libDungeonHunter2.so IDs 0–1865

**PARTIAL SEMANTIC AUDIT. All 1,866 inventory records have exactly one disposition; full unique-body/source coverage remains unfinished.**

There are 1,866 successful decompilations, zero failed bodies and 1,614 distinct raw code hashes. Pseudocode inspected: 649 records. Bounded source comparisons: 63 records. Remaining source comparisons: 1,451 records. Bodies not fully inspected: 1,217 records. The exact remaining IDs and all repeated hash groups are listed below.

All runtime acceptance is `not_run`. No build, test, install, launch, source/checklist edit or external-chat message occurred. Fixture generators/tests were read only. Direct exported xrefs and data references are attached to every row; lack of direct calls does not prove unreachability through vtables/callbacks.

## F1 — save producer block count changes exact-multiple commit timing

Original #569 Buffer::ensureBuffer (0x316d98) computes `(offset+length)/block_size+2` pointer slots, allocates real buffers through `slots-1`, then appends a NULL sentinel. Default #568 block size is 2048. Original #520 Savegame::UpdateJobs reads `(end-start)/4-1` at 0x314b10–0x314b30 for the real block count. #540 emits the smaller of 2048 and logical remaining bytes. A naturally built 2048-byte stream therefore has two real blocks: 2048 bytes and zero bytes. Its first disk write retains invalid header FFFFFFFF; the second update writes the empty block and commits the count.

Current `port/level-world/savegame_jobs_owner_v2.cpp` uses `ceil(size/2048)` for block_count, commits in one update and omits the empty write. This is a confirmed static commit-timing/file-callback-count difference. No actual authored save of exact-multiple size was established. IDs 520/569/570 remain `partial`.

Original #520 also restores the queued in-memory header after the first write at 0x314834–0x314864. Current unique stream bytes remain invalid until job destruction. Its private API leaves external observation of that internal difference unestablished.

`tools/produce_savegame_jobs_v2.py` fabricates vector end using `ceil(size/2048)+1` slots instead of executing #569, masking its 2048-byte producer edge. `tests/level_savegame_backend_v2_native.cpp` compares update counts/traces to that fixture, which cannot settle the omitted edge. Neither was executed here.

Connection is concrete: native_app.cpp advances process-owned ApplicationSaveFilesOwnerV61, which updates the same retained queue used by flush-before-read and Level enqueue. #521 filename existence gates draining the whole queue in both bodies. Level serialization preserves backup enqueue, sorted count/size/four-byte-tag directory, cache copy and owned write enqueue. Generic raw mode, callback pairs, uncached sized zero-fill, duplicate tags and short-read assertions remain partial/unmapped. Source hashes and exact locations are in CSV.

## F2 — App14 EventManager update has no scoped production binding

Original #1258 Application::_Update calls #1398 EventManager::Update at 0x32c564 on App+14 after PlayerManager and before StateMachine. #1398 drains pending events, runs each deleting destructor, then handles delayed detach. Source EventManagerOwnerV12::update exists, but the actual native frame and campaign services have no reviewed binding to it.

Scoped searches of production cpp/hpp/inc under port/level-world, port/level-loader and port/android-native/app/src/main/cpp, excluding tests/reference/reports/vendor, examined EventManagerOwnerV12, update expressions and producer references. Only implementation and test calls were found; the actual native frame and owner publication were read. The disposition is medium-confidence `disconnected`, not missing implementation. No genuine App14 pending producer or gameplay-visible pending event was established; the practical trigger remains unverified.

App14 and Level's inherited EventManager are different objects: application_services_owner_v5.cpp publishes a heap App14 owner; level_constructor_v3.cpp constructs the Level base. Cross-range #8205 Objective_EventReceiver::Unregister (0x47adbc) schedules delayed detach on the current Level base; game_event_runtime_v75.cpp does likewise. The absent App14 update alone does not establish a Level-objective failure. Original Level drain ownership still needs tracing.

Compared attach/detach/raise preserve insertion order, ignore priority for sorting, suppress duplicate registration, snapshot records, borrow the same event and stop only handler result 1. #1399 ARM branches directly to Raise at 0x338ebc; source raise_async is synchronous. #1381/#1384 remain partial because native tokens deduplicate requests and skip removed records, while original raw node references can repeat frees. Flush/destructor clear nodes without calling pending payload deleting destructors; observer-release closure remains partial.

## F3 — native touch transport represents part of the original queue

Original #1449 has a 16-slot ring, full rollback and move coalescing. #1452/#1453 maintain 48-byte touch history/time/swipe fields; #1464 expires stale pressed slots after 0.5. #1488 pops before callbacks, applies orientation/scale and raises App14 types 4/5.

Current AuthoredMenuTouchScreenV3 stores dimensions/scale/orientation, eight active bytes and queued rows, but process requires a whole nonempty consumer callback. Actual dispatch_source_app_touch_v121 maps Android pointers to slots and directly raises typed events on the same App14; it clears active bytes before release callbacks and bypasses the original queue. No concrete process_touch assignment or source_head/source_tail producer was located in the reviewed composition. Rows remain `partial`, with history/coalescing/timeout/coordinate equivalence unverified. A direct input producer and typed event handler exist, so this is not classified as missing gameplay input.

## Object, conditions and physics

#1637 preserves key-zero NULL, cached address/frame fast path and default map-node insertion on stale/missing keys. CanonicalObjectManagerV1::resolve_handle_v4 maintains those effects with retained identities; #1573 refreshes SAME shared handle frame. #1844 ARM confirms a hidden ARM12 return pointer, explaining misleading IDA arguments. Current Add publishes map/count/handle before name/archetype/room/lists/network and destroys the new actor on duplicate. The 33-name catalog exists; every actual constructor still needs review.

#1846/#1847 staged native owners preserve property/template/default/override ordering, early LevelConfig InitPost, special _prim_PlayerLight_S room -1 and y/z/x module offset before SetPosition. Constructor/network/deleting-D0 leaves remain partial. #1791 multi-phase InitPost was read but not fully compared.

Full #1839 ObjectManager.Update was compared to canonical_object_update_v102.cpp and source_campaign_object_update_bindings_v105.cpp: same-owner cells/lists, online/Level gates, room/remote/start queues, pending splice, deletion, conditions, AI/selected frames, post-update handle resolution and debug suffix are wired into Level.Update(float 1). Derived frame/remove/network leaves remain incompletely audited. #1760 deletion dedup and #1809 reload-front queue match bounded valid domains. #1576/#1603/#1604 compare actual enabled8a, conditions a8/ac or cc/d0, PlayerInfo660→Character14e8 Save.quest-ready14 and Level118 difficulty; selected virtual44/48 receives the same actor after the byte store.

#1858 destroys then NULLs caller body; #1859 ARM is a definition-null guard and backend CreateBody tail branch despite IDA extra arguments. #1860 verifies uint32 App.dt8c→float32×0x3a83126f→Step(iterations 10); current NativeWorld and campaign provider bind that produced dt and same world. #1864 backend destruction matches, but original debug load/body trace calls are unmapped. #1865 adjusts this by -12 into #1866 outside this range; target closure stays unfinished.

## Imports, hash groups and ABI caveats

IDs 0–351 were individually read in pseudocode and checked against full ARM/imports.json. #0 is the ELF PLT resolver; 351 external stubs use GOT indirect transfer. Inventory aliases are not authoritative imports: #111 LC_API_FREE_0 resolves _ZdlPv/operator delete. Exact imported names are recorded.

Only matching complete unconditional empty/literal bodies with no PC-relative calls/data are confirmed clones. Other repeated raw hashes remain target-dependent or unresolved. #1583/#1586 share bytes but branch respectively to Deserialize and Serialize; pointer-adjustment thunks have exact targets and unfinished source closure. Raw hash counts and confirmed clones are separate.

Unmapped unusual bodies: LuaAllocator #410 preserves shrink allocations and copies oldsize on growth; CustomRealloc #415 copies newsize; CustomMemalign #416 asserts then ignores alignment. CMutex.Unlock #431 overwrites its conditional decrement with old count for values above one, and #432 IsLocked returns count==0. #578/#583 static stream/string/uint64 ABI was checked in ARM; CString serializers are not equivalent by name.

## Exact remaining coverage

terminal_disposition means a proved import/resolver disposition; reviewed_scope means bounded source comparison, including partial/disconnected; unfinished leaves full semantic/source closure outstanding. No value certifies runtime acceptance.

Bounded source comparisons (63): 493, 520-521, 525, 528-529, 531-532, 534, 539, 544-548, 568-571, 1377, 1379, 1381, 1384, 1386-1389, 1396-1399, 1448-1449, 1452-1455, 1464-1465, 1470-1472, 1485, 1488, 1573, 1576, 1603-1604, 1637, 1674, 1760, 1809, 1839, 1844-1847, 1856-1860, 1864.

Unfinished source comparisons (1451): 352-492, 494-519, 522-524, 526-527, 530, 533, 535-538, 540-543, 549-567, 572-1376, 1378, 1380, 1382-1383, 1385, 1390-1395, 1400-1447, 1450-1451, 1456-1463, 1466-1469, 1473-1484, 1486-1487, 1489-1572, 1574-1575, 1577-1602, 1605-1636, 1638-1673, 1675-1759, 1761-1808, 1810-1838, 1840-1843, 1848-1855, 1861-1863, 1865.

Pseudocode not fully inspected (1217): 353-354, 356-363, 365-397, 401, 403-406, 408-409, 411-414, 417-425, 433-488, 491-492, 494-510, 512-519, 522-524, 526-527, 530, 533, 549, 552-557, 561-567, 574-576, 581, 584-597, 600-607, 609-610, 617-675, 677-683, 685-686, 689-719, 721-738, 740-764, 766, 770, 772, 775-793, 800, 809, 829, 835-840, 842-847, 849-854, 856-875, 877-884, 886-890, 892, 894, 896, 898, 900-903, 905, 907, 909, 911, 913, 915, 917-919, 921-922, 924, 927, 931-932, 934-938, 940-943, 945, 952, 954, 958-959, 961-963, 965-966, 968, 970-1257, 1260-1289, 1291-1294, 1296-1297, 1299-1376, 1378, 1380, 1382-1383, 1385, 1390-1395, 1400-1401, 1404-1409, 1411-1445, 1450-1451, 1456-1463, 1467-1469, 1473-1484, 1486-1487, 1489-1505, 1511-1512, 1514, 1516-1519, 1525-1526, 1528, 1530-1533, 1537-1554, 1560, 1567-1568, 1571-1572, 1574, 1579-1580, 1582, 1584-1585, 1587-1601, 1605-1606, 1608, 1610-1613, 1616-1621, 1624-1636, 1638-1643, 1651, 1657, 1666, 1671, 1673, 1675-1719, 1721-1727, 1729-1759, 1761-1790, 1792-1808, 1810-1838, 1840-1843, 1861-1863.

Unfinished systems include STLport/runtime helpers, math/formatters, profiling/memory, generic serialization/threading/UserProperties, Lua binding/value lifetime, Glitch attributes/CMessage networking, much of Application/platform lifecycle, Console/debug/subtitles/state-machine, remaining touch/accelerometer variants, ObjectBase init/destruction and full ObjectManager networking/removal/draw/flush/C1/D0/class constructors. The required full-coverage gate remains open.

## Repeated raw hashes

Repeated raw groups: 48. Confirmed trivial duplicate rows: 157 across 9 groups. CSV stores the full SHA/group membership for every record.

- `34b97ea285a49248d21f74e46f26848d3f2c1a90e381ca2c3fe219705331487e` — 352-354, 356, 396-397, 401, 433, 471, 491, 549, 581, 584, 626, 646, 662, 671, 689, 712, 1289, 1404, 1409, 1493, 1546, 1553-1554 — target-dependent or unresolved; no clone parity claim.
- `379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f` — 355, 398-400, 426-429, 489-490, 536, 598-599, 608, 611-613, 676, 687-688, 720, 739, 765, 767, 774, 796-799, 802-808, 810-818, 820-826, 828, 830-834, 904, 929-930, 939, 946, 953, 955, 964, 1290, 1295, 1298, 1402-1403, 1410, 1446-1447, 1466, 1534-1536, 1555-1559, 1561, 1569-1570, 1656, 1672, 1849-1855 — confirmed empty/literal duplicates; source parity unfinished.
- `44d1d8a4e42bad3be57edb34695742d66c5a2826777a8efc400002a3125eec73` — 459, 567 — target-dependent or unresolved; no clone parity claim.
- `e8e4c9916b546b81aa97e069deda42393f3afe0105e711a4a9b1746c6ad86ff4` — 505, 746 — target-dependent or unresolved; no clone parity claim.
- `007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47` — 541, 614-616, 855, 876, 928, 951, 1644-1645, 1647, 1650 — confirmed empty/literal duplicates; source parity unfinished.
- `6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877` — 542, 684, 768-769, 773, 794-795, 827, 848, 926, 933, 947-948, 950, 956, 960, 1506-1509, 1520-1523, 1562-1566, 1646, 1648-1649, 1652-1655, 1661-1664, 1667-1670 — confirmed empty/literal duplicates; source parity unfinished.
- `10545ade12f94adeb0309d99530c41b2938671dd9d426d4520fa81a4d471680c` — 568, 576 — target-dependent or unresolved; no clone parity claim.
- `ced7dbf6f57d2c5d42b43fcd506845398183e0adbdcd41968192ca035de20d04` — 590-591 — target-dependent or unresolved; no clone parity claim.
- `3eecbd05963866b222953258f3961d5bf5f455c1c4e86b5c97180aaefdc98992` — 609-610 — target-dependent or unresolved; no clone parity claim.
- `02b8fc97b38021cd684cea0882748d25240137cf5d3ae1744e805e07cb5444fe` — 685-686 — target-dependent or unresolved; no clone parity claim.
- `d4a7df181f8af768787ede9a55eb13588a1bd0ed23f69d4964cf67c8655802fe` — 721-722 — target-dependent or unresolved; no clone parity claim.
- `1c3760b1c8dd9ea04a8a8264eaa1fe85731643cab7e8c0ee02c6abfc20c7c39b` — 743, 1415 — target-dependent or unresolved; no clone parity claim.
- `9448dc4c8e2d0453cdbef676eba4b5322852c33e65e1691a7da41e8bd20e21d9` — 772, 775, 783 — target-dependent or unresolved; no clone parity claim.
- `6d868ec4dedc5aeefac5872fe025b82ad5de0c7b07b5e34de844e941bc096418` — 776, 778, 784 — target-dependent or unresolved; no clone parity claim.
- `9482f41bdb65959bc6590c1de13110314d82cc44a98a99d94bfcd29a1fb3bbac` — 777, 781-782, 789 — target-dependent or unresolved; no clone parity claim.
- `e2c7554dcd956172ec8462492a5edb51968a743be7ddb65c9c4bb0fcec292444` — 792-793 — target-dependent or unresolved; no clone parity claim.
- `47ee0df12deb3f97e1f891946e9a885da7411bcdea8dec23b6580ff65986f116` — 800, 809 — target-dependent or unresolved; no clone parity claim.
- `0efcbe12bd7d505235ffab66b191e312b762871400d6f0870fd53b7c997bdd8d` — 801, 819 — confirmed empty/literal duplicates; source parity unfinished.
- `49d9b3d87e87845adabd10dc393f61bda139c803243c8f9143012e3e2234e767` — 835, 837 — target-dependent or unresolved; no clone parity claim.
- `f68520e7644c47739b5b74691da29778feb6b6b8fdeed6ee79f647d2e3ded16e` — 842, 849, 856, 886, 892, 894, 896, 898, 900, 1560, 1666 — target-dependent or unresolved; no clone parity claim.
- `f784f97224e88b07c2539cb007a7a0f937f635d59f538c9f3e1d31af77ad9831` — 843, 851 — target-dependent or unresolved; no clone parity claim.
- `e9aa3de4cac78444c77884629c2ebaa68f910f3b5f3ce6e18846378a625a8fdf` — 846, 854 — target-dependent or unresolved; no clone parity claim.
- `4ea8131aec844af6b6592fd054511beaed8c651c555fdb7f80ade67f97f98cb1` — 881, 887 — target-dependent or unresolved; no clone parity claim.
- `3fc49adc7394262080e1e06db1aa4d983864b2d9b150c07306faf456deb4732e` — 895, 914 — confirmed empty/literal duplicates; source parity unfinished.
- `471b7d0a76e32e0a0c5a06ecf98a5e4412ec1872088cfe87d5f450206f4474ba` — 905, 907, 909, 911, 913, 915, 917, 919, 922, 924, 927 — target-dependent or unresolved; no clone parity claim.
- `b4e46592b5e5d282ef2e4da99947d1fc7fd175acf1830c60f7e60d3c434fb8ca` — 906, 923 — confirmed empty/literal duplicates; source parity unfinished.
- `703b91ad4f1775562a3a50bf01ddfe5837f8c337240e2d4c658428f493e3ec3b` — 908, 912 — confirmed empty/literal duplicates; source parity unfinished.
- `3e8a4d187fd09086abbee6fdcaed9e6195f6b5b8b3b55f27497a024e2d6b3d88` — 916, 920 — confirmed empty/literal duplicates; source parity unfinished.
- `6143299c415a779d14d46483b05772d81c6b595bc189b4499503e30275041932` — 949, 957, 967, 969, 1658 — confirmed empty/literal duplicates; source parity unfinished.
- `90f5106ab87567f0598ad6b90617ed2a9e529cc87b88479636f6cbcd5bf2ff00` — 1042, 1049 — target-dependent or unresolved; no clone parity claim.
- `e3048fe5dfb52114ccace0ebdbca03d1655fce4c9e903eb766206b9b275e3020` — 1209, 1222 — target-dependent or unresolved; no clone parity claim.
- `88fcad4b6a3d917b066150b094b52598ce3d09d516265cb49c02e9d9de28529e` — 1216, 1226 — target-dependent or unresolved; no clone parity claim.
- `a464b492a141b0921b85dc23c1afc195df57c318f0df2719493adbc6869d4ef6` — 1352-1353 — target-dependent or unresolved; no clone parity claim.
- `09e19d7bfd85ceec0b03304abd68699ab3fe674bd498654c1cef89dfed624a91` — 1377-1378 — target-dependent or unresolved; no clone parity claim.
- `519789b0ed5a1369c249c454406cd73a9006e48a4f59557628c7001ed4c79f32` — 1393, 1634, 1775 — target-dependent or unresolved; no clone parity claim.
- `329f0de66165da8b8c18a43a2147d46c2ec03bd162130486e585149fd005cdcd` — 1394, 1635, 1776 — target-dependent or unresolved; no clone parity claim.
- `a5d1ebb661b3004e8aa73d2371157e025609734a594fa060d11bec5372cee31b` — 1400-1401 — target-dependent or unresolved; no clone parity claim.
- `cadff9dff0cce75c8e2fd48f36ec0a221a1002cda49615b1591ae530f9dbef45` — 1420-1421 — target-dependent or unresolved; no clone parity claim.
- `7f736286c127ff739532460f02c2390c842e5f914fd8c7b83ba3eea5c3158695` — 1510, 1513, 1515, 1524, 1527, 1529 — target-dependent or unresolved; no clone parity claim.
- `a488a72710f7d93eb3952908d3d4c692da62193bd5207a9755cfe87e1695ba64` — 1511, 1525 — target-dependent or unresolved; no clone parity claim.
- `f3f51222ff3ab6fe112d2223d6d4b210dcc8d70c043cfdbcc6d09bcd82f27ab7` — 1512, 1526 — target-dependent or unresolved; no clone parity claim.
- `0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec` — 1583, 1586, 1607, 1609, 1720, 1728 — target-dependent or unresolved; no clone parity claim.
- `47e30047b9c90e299cc78910fbe6d24dce4837d7d2bae83691b346f0cad213ac` — 1627-1628 — target-dependent or unresolved; no clone parity claim.
- `5cad71a3611757df03f304414030386f18aa73d42b3e4fa9533cfb9e34ed485e` — 1753, 1764 — target-dependent or unresolved; no clone parity claim.
- `b553617bb3d3e36bc2dbc7b4a1882bdb5e283bd926bf1ef5b38c4f2da9ee976c` — 1754, 1765 — target-dependent or unresolved; no clone parity claim.
- `8ac0234ff560907dd451a97e946d327e087c8111af00f186a31a562d97884e6d` — 1769, 1784 — target-dependent or unresolved; no clone parity claim.
- `beece75e68045f2395909051fd639bb2f7affa0da3f9f71f63ee129f1d55ca9c` — 1770, 1785 — target-dependent or unresolved; no clone parity claim.
- `afdc21bb7c48845d5ed7c4a64793f1d0566375b63830112980dbf2fa5f75157c` — 1856-1857 — target-dependent or unresolved; no clone parity claim.

## Source fingerprints

Working tree read under HEAD marker75a7c2fe3261403e841ffbeb46734e9e4e84e75c. Hashes are current cited contents, not historical manifest values. Final recheck records any observed change.

| Source | SHA-256 |
| --- | --- |
| `port/android-native/app/src/main/cpp/native_app.cpp` | `a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3` |
| `port/android-native/app/src/main/cpp/native_character_menu_v4.inc` | `d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557` |
| `port/android-native/app/src/main/cpp/native_menu_application_event_v120.inc` | `7fe6acf92c39fb7d18e852ea57aeddae2e917e76c196009427a0592ee77a91c0` |
| `port/android-native/app/src/main/cpp/native_process_startup_v119.inc` | `256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa` |
| `port/android-native/app/src/main/cpp/source_campaign_object_update_bindings_v105.cpp` | `db4bc112ba35427fed2c494ff6490ffb0a6457cbf0ab27f91b8b39560327fa31` |
| `port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp` | `6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e` |
| `port/engine-ui/authored_character_panel_platform_v4.cpp` | `7c51e7d7afb0dd575a883abdda58d87f620988d51bd63a026baf845f6e6a23ed` |
| `port/engine-ui/authored_menu_touchscreen_v3.cpp` | `872f75765716c2132fae85a5ac569b9fa16c78ef0c2ef2be596328862f5a86c4` |
| `port/engine-ui/authored_menu_touchscreen_v3.hpp` | `0068aa6872ca310686841aed94989b34f47d2dd045c4115c2595065799b43fb7` |
| `port/level-loader/CMakeLists.txt` | `a9c0f2066450d8380a002b64bcff88bcd8a69d1a9c4023a56e11d266c489b16d` |
| `port/level-loader/game_event_runtime_v75.cpp` | `72ae02a51b05a1ff0c1c618c49a7675c887c74f6fe1794f21fd22f0c9fb5cd27` |
| `port/level-loader/level_constructor_v3.cpp` | `039c0c476f29314f7c2f87b04217b4b632575786563167143ad6ea23df00e5c6` |
| `port/level-loader/level_gameplay_update_v66.cpp` | `c40fa4705d4deba74f27e20f89a1b767ffc4a43363fb2b150b91d5b3809faf4f` |
| `port/level-world/CMakeLists.txt` | `78e9bae6ecf3c3dd4fe31ff187654440246ce92fb4741e85edb55d2d5b285608` |
| `port/level-world/application_save_files_owner_v61.cpp` | `42420fcd13c8689a80615c02b2b3cda400a6c8640a372a815ab17e9ffe3b45da` |
| `port/level-world/application_services_owner_v5.cpp` | `7c0f3a164b107fb70c4e5f79cc9851e5a81a01687beda79b7a1a772088b6ce6f` |
| `port/level-world/canonical_object_factory_v1.cpp` | `84eb8a2eb9aa8909f8d3f9918ec0fde83d92e434d69a4ce64978a8259bb08463` |
| `port/level-world/canonical_object_manager_v1.cpp` | `28c80aba116cfb16dd4e1807cb72d76085bc66dc4816aae05cc3dc21963cb46f` |
| `port/level-world/canonical_object_update_v102.cpp` | `b4748c5f00e2f6c3b8dff1c426901dad29a4ceac701abed04dd9bb41d8c1fb38` |
| `port/level-world/canonical_spawn_owner_v1.cpp` | `032cd11dc4f7e372096c88d0fd09a1f71a26834d9e65124b2366af28a2c9e391` |
| `port/level-world/event_manager_owner_v12.cpp` | `a5459a41647b51ce7baa4759a61128916e8dde82a7b3dd1a5290bf834b2f1050` |
| `port/level-world/event_manager_owner_v12.hpp` | `6368cb5529e52acc0e47916842f5d43609b63021a4a4fb8a90ad7760669a5783` |
| `port/level-world/level_savegame_cache_v1.cpp` | `8c85d364b762f4725d21db51013333a34d3f95ac4c903e309e11189ab70f5d9e` |
| `port/level-world/level_savegame_writer_v2.cpp` | `77e1a65512e13da92103e768c5bd7f3d53299affc80ffeeaf3131ff77fca23cb` |
| `port/level-world/object_enable_condition_v2.cpp` | `7ae820713fbfa032d98dc3fcfc9e739cd7931c814903f31c8bcc819789fdc78e` |
| `port/level-world/physical_world.cpp` | `104dd9049d1ca95ed2bfdcfb6a83748d0a34f9e6447c0c1ec015d26bcc5c0ba4` |
| `port/level-world/physical_world.hpp` | `870d89be3afb6b0eb12d3af3b84dbcdd795afabde209126611b0682dd097f517` |
| `port/level-world/savegame_file_gate_v2.cpp` | `a2c92a2642fc59a66abd22a5a16db7dbc623aea0c9c9838660a3c09663226dda` |
| `port/level-world/savegame_jobs_owner_v2.cpp` | `ca30da7fc92d3d78a35e2af029950e8866feca9b5addf3920ff35cb8b54589e6` |
| `port/level-world/savegame_stream_v2.cpp` | `38ec4dd1251e0759986e2a63459c17cb8a0a805d31cf921fddcd2ffb286d0bc7` |
| `port/level-world/savegame_stream_v2.hpp` | `a27fd4b745a34558ac3cace1cacf29c9ce849f96a59684dfc42ab61e0e05f19f` |
| `port/level-world/tests/level_savegame_backend_v2_native.cpp` | `27d641f41325233534b37f17a17c876be4339b388bc05c30ec89f2c1fb5e30fb` |
| `port/level-world/tools/produce_savegame_jobs_v2.py` | `c218d4d645225c22fc44278673d484b8e316ac55baa8c373f4a1f91a2226510c` |

Final source-hash recheck: one cited file changed after the resumed initial check; its current fingerprint is above and the observed change is recorded below. The other 32 cited fingerprints remained unchanged.

## Recheck after the pause

Final working-tree fingerprint capture: 2026-10-08 14:58 UTC. All 33 table hashes agreed with the current cited files at this capture.

The initial resumed check at 2026-10-08 14:53 UTC found all 33 cited fingerprints and the HEAD marker unchanged. A subsequent final check detected an edit to `port/android-native/app/src/main/cpp/native_character_menu_v4.inc` after that check. Previous SHA-256: `8544c4db2b00f0547df8650e14e21e7fb62535008dab8b3eb5b1f448b285e143`; current SHA-256: `d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557`. The observed declaration now derives NativeCharacterMenuV4 from `std::enable_shared_from_this<NativeCharacterMenuV4>`; the prior inspected declaration had no such base. A full pre-edit file snapshot was not saved, so this lane does not claim that declaration was the only edit.

The current touchscreen field remains at line 12. The current menu source and Git diff against HEAD were read; scoped process_touch/source_head/source_tail producer searches were repeated and still found only the exposed queue getters, with no concrete production queue provider. F3 remains partial. F1/F2 cited implementation files did not change. No CSV source citation references the changed native_character_menu file; its Markdown fingerprint was updated. This audit made no source edits. Files outside the 33-file fingerprint set have no pre-pause hash baseline, so their pause-time changes cannot be established by this lane.

Every CSV row, address/name/decompilation status/hash group, cited source hash and line bound, runtime flag and Markdown coverage/group list was rechecked. Original pseudocode integrity matches all 1,866 exported SHA-256 values after LF normalization (the files contain CRLF); input hashing does not convert unexamined bodies into inspected coverage. Confirmed clones remain restricted to 157 trivial duplicate rows across 9 groups with no PC-relative calls/data, all with unfinished source parity. Target-dependent repeated hashes, including 1583/1586, remain thunk/unclear.

Coverage remains partial: 1,866 disposition rows, 649 inspected records, 63 bounded source comparisons, 1,451 unfinished source comparisons and 1,217 bodies not fully inspected. Every runtime flag remains not_run. No implementation tests or runtime execution occurred.
