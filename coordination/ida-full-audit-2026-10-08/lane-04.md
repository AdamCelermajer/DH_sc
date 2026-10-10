# Lane 04 — incomplete semantic audit

Assigned `libDungeonHunter2.so` function numbers **5598–7463** inclusive. The CSV provides one disposition for every assigned record. Manifest coverage is distinct from semantic completion. The exact counts and unfinished IDs below are authoritative; this lane must not be treated as a completed full-coverage semantic audit.

All runtime fields are `not_run`. No source, checklist, build, test, installation, emulator, or other chat was changed. Only the two lane files were written. Numeric source address markers and function inventory labels remain candidate evidence. No search miss was classified as missing, disconnected, or out of scope.

## Confirmed static differences

### L04-F1 — script name lookup becomes case sensitive

**#7428, 0x4591f0, ScriptManager::GetIDFromName.** The original body iterates from zero or `common8`, calls `LC_API_STRCASECMP_0`, and returns the first matching index. Original ARM **0x45923c** confirms that call. Current `port/level-loader/script_manager_owner_v52.cpp:30` uses `std::strcmp`. A case-only changed script reference resolves in the original and returns `-1` in the port.

This implementation is connected: `canonical_trigger_object_v28.cpp:72–73` resolves the actual Trigger script74c/768 fields through this owner. `source_campaign_zones_v83.cpp:136`, `source_campaign_events_v75.cpp:72`, and combat/death/tutorial owners also call it. Source loading publishes the same manager in `source_campaign_runtime_v61.cpp`. Disposition **partial**, high confidence in the comparison difference. Current assets depending on case-only spelling and runtime reproduction were not established.

### L04-F2 — successful equip preserves the old AS result

**#7009, 0x43e600, NativeInvEquipItem.** With three numeric arguments and a resolved actual Character, the original invokes virtual `0x13c` equip, then **0x43e728** calls `gameswf::as_value::drop_refs` on the result and **0x43e72c** stores result type byte **0**. Current `character_menu_queries_owner_v1.cpp:255` returns `graph_.actions->equip(...)` without clearing `c.result`. `character_menu_as_bridge_v1.cpp:97` projects the preexisting native result; lines158–163 write back only if the projected result changed. An initially nonundefined result consequently survives successful port equip.

The native route appears in `authored_character_menu_routes_v1.cpp`, with the live `original_ui_session.cpp:545` and authored panel bridges. Disposition **partial**: the equipment operation exists. The differing domain is a valid successful equip call with a prepopulated result cell. No claim is made that the supplied SWF gameplay currently depends on that cell. Neighbor #7018 Unequip has no original result-clearing tail and was assessed separately.

## Connected systems examined

- **Level save/load (#5616,5624–5626,5663–5664):** Original guards, ownership and callback order were compared with `level_player_placement_v68.hpp`, `stage_loader_checkpoint_v1.hpp`, `player_level_quicksave_v29.cpp`, `level_unload_source_v1.hpp`, and live campaign adapters. Source retains the local-player query before phase/save guards, checkpoint fields, fresh saves/configs, camera deletion, menu lifetime, PM update and random reseeding. Positive online/save/placement/teardown dependencies remain bounded/unaccepted, so orchestration rows stay partial.
- **Script path conversion (#5663):** The last exact `.pyscript` marker, reached-only slash mutation, `_pyscripts.bin` replacement, and unusual names replacement count are preserved. Stage8 writes normalized data back to the actual config through `write_script150`; looking at the temporary alone would have yielded a false disconnection.
- **Inventory:** Signed16 potion quantity and signed equipment selection/swap (#5758–5761) match actual native primitives. Potion0 production for absent quantity0 and deletion/quantity branches (#5795,5817) exist, with provider/assertion/alias domains still partial. Generic `_AddItemInstance` and transfer bodies were read; they do not establish all equipment/trophy/async quest effects. Large loot power/table/AddLoot originals #5886–5888 remain unfinished.
- **Controllers:** Forced byte9 overrides global blocked/lock8. Assembly #5933 confirms R1/R2 are forwarded despite IDA's inferred one-argument prototype. #5944 uses virtual34 stop. #5957 contains a substantial online target-restoration/message path; complete networking and positive provider coverage remain unresolved. Mixed Update #5986 captures the child count and reaches actual child providers. Large Gamepad.Update #5979, NetController.Update #6051 and PS3Move.Update #6059 remain unfinished.
- **Lights/camera:** #6105 first strcmp match / miss0 maps to the actual four-name owner. Camera damping #6180 aligns ordinary arithmetic and its second root query/GetDt boundary; additions are captured before the first original virtual query, while native computes them after that query. Alias/callback mutation domains and whole camera/lighting ownership remain partial. A failing ordinary renderer read was not inferred from that ordering alone.
- **SWF audio/settings:** ARM resolves #6948/6949 double→float and audio selector2/1 before float→int and the same-settings VolumeMusic/FX write; null audio manager is a no-op. `front_ui_session_v87.cpp:499/985` connects the corresponding legacy callback and retains explicit numeric bounds. Registration alone is not acceptance for every audio/settings domain.
- **Script command storage:** All80 generated-data factory bodies #7262–7341 and all80 implementation factory bodies #7342–7420/#7438 were inspected. Concrete implementation C1 defaults/receivers/factory dispatch were compared with retained descriptors. Generated-data schemas remain candidates until every pointer/default/read/lifecycle connection is compared. Constructor parity does not establish Execute/Init leaf parity.
- **Script virtuals/scheduler/teardown:** Literal IsBlocking0, ShowTrophies1, and Wait signed comparison were inspected as exact hashes; `script_command_execution_v96.cpp` has corresponding real methods. #7248 repeats same-command Init, with a live state13 caller and assembly virtual0 verification. StopScript writes state8=2/context0=-1, preserves field4 and ignores its bool. #7454 frees commands before names and vectors. #7455 reaches `source_store_menu_cut_screen_v62`, whose implementation at `model_renderer.cpp:3127` resets the actual controller blocked low byte; its name does not indicate an unrelated callback. #7463 native destructor versus explicit source-unload lifetime closure remains partial.

## Physical helper evidence

**56 original nonvirtual thunks** were inspected in `assembly-functions.asm`: each is `SUB R0,R0,#4/#12/#16/#36` followed by a named tail `B`. These are ABI receiver adjustments. Each CSV row includes its exact instructions and target; the target's source behavior remains separate. No external import entry was identified in this assigned range.

Exact code grouping uses full `code_sha256`, not a `.clone` suffix or pseudocode resemblance. Repeated groups and every assigned function number appear below. Unique native STL/allocator/destructor helpers remain unclear unless their underlying semantics were examined. A source compiler/library helper was not declared out of scope merely because its original ABI differed.

The mechanically generated CSV body features and xrefs are an index for continuation. They do not substitute for reviewing all unique nontrivial bodies. Every such unreviewed record is explicitly retained in the unfinished list.


## Exact coverage and unfinished IDs

CSV **1866 rows**; assigned5598–7463 exactly once; all1866 decompilation success. Unique code hashes **1627**. Full bodies or identical representatives inspected for **495 records / 277 unique hashes** after the six-row recheck,dependency #6843 and row6308 provenance pass. **1371 records / 1350 unique hashes remain uninspected semantically**. The dependency-only ResetFonts #6843 row retains unclear status; this adds body inspection,not completed source parity. Source comparisons or assembly-thunk dispositions reached for **249 records** including identical clones, within stated scopes. Complete audit: **NO**.

CSV statuses: {'unclear': 1510, 'partial': 21, 'import/thunk': 56, 'matched': 92, 'clone': 187}. Categories: {'game-or-engine-body': 1057, 'lifecycle-destructor': 149, 'compiler-container-helper': 202, 'this-adjusting-thunk': 56, 'global-initializer': 51, 'literal-no-op': 87, 'literal-or-accessor': 104, 'script-construction-helper': 160}.

**Body inspection unfinished IDs (1371):**

`5599-5600, 5602-5608, 5610-5611, 5613-5614, 5618-5622, 5627, 5629-5632, 5635-5636, 5638-5647, 5649, 5651-5654, 5656, 5658-5662, 5666-5668, 5670-5673, 5675-5698, 5701-5719, 5721-5754, 5756-5757, 5762-5784, 5786-5789, 5793-5794, 5796-5812, 5815, 5818, 5820-5824, 5826-5834, 5836-5900, 5923-5924, 5926-5932, 5934-5943, 5945-5956, 5958-5960, 5962, 5964, 5966-5970, 5974-5979, 5982-5985, 5987, 5989, 5991, 5993, 5995, 5997, 5999, 6001, 6003, 6005, 6007, 6009, 6011, 6013, 6015, 6017, 6019, 6021, 6023, 6025, 6027, 6029, 6031, 6033, 6035-6037, 6039, 6041-6045, 6048-6052, 6055-6059, 6065-6066, 6068, 6070-6074, 6076-6082, 6084-6087, 6089, 6091-6095, 6097, 6100-6104, 6106-6129, 6132-6138, 6140-6150, 6152-6162, 6164-6166, 6168, 6170-6176, 6183-6192, 6195-6196, 6199-6261, 6263-6307, 6309, 6311, 6313, 6315-6317, 6319-6320, 6322-6324, 6326, 6328, 6334-6344, 6346, 6348-6355, 6357-6358, 6362-6363, 6365-6369, 6373-6400, 6402-6416, 6426, 6429, 6432-6574, 6578-6622, 6624-6654, 6656, 6658-6672, 6674, 6677, 6680-6689, 6692-6697, 6699-6729, 6731-6807, 6809-6827, 6829-6830, 6832-6837, 6839-6842, 6844-6862, 6872-6878, 6895-6909, 6911-6947, 6950-6968, 6970-6997, 6999-7008, 7010-7017, 7019-7161, 7244-7247, 7253, 7256-7260, 7421-7427, 7429, 7433-7434, 7442-7443, 7445-7453, 7456-7462`

**Source comparison/thunk disposition unfinished IDs (1617):**

`5598-5615, 5617-5623, 5627-5647, 5649, 5651-5662, 5665-5668, 5670-5757, 5762-5794, 5796-5816, 5818-5943, 5945-5956, 5958-5960, 5962, 5964, 5966-5985, 5987, 5989, 5991, 5993, 5995, 5997, 5999, 6001, 6003, 6005, 6007, 6009, 6011, 6013, 6015, 6017, 6019, 6021, 6023, 6025, 6027, 6029, 6031, 6033, 6035-6037, 6039, 6041-6060, 6062-6066, 6068, 6070-6074, 6076-6087, 6089, 6091-6095, 6097-6104, 6106-6162, 6164-6166, 6168, 6170-6179, 6181-6261, 6263-6309, 6311, 6313, 6315-6317, 6319-6320, 6322-6324, 6326, 6328, 6330-6331, 6334-6344, 6346, 6348-6424, 6426, 6429, 6432-6674, 6676-6689, 6692-6697, 6699-6729, 6731-6827, 6829-6830, 6832-6837, 6839-6947, 6950-6997, 6999-7008, 7010-7017, 7019-7163, 7173, 7183-7184, 7186, 7193, 7195, 7197, 7199, 7201, 7217-7218, 7228, 7244-7247, 7253-7254, 7256-7260, 7262-7341, 7421-7427, 7429-7437, 7439-7453, 7456-7462`

CSV review_depth not_completed is machine-indexed only. full_pseudocode_only/schema_candidate still needs source comparison; clones inherit body evidence only. Individual virtual,registration,failure and lifetime edges in unfinished records remain open.

All1,866 local pseudocode files use CRLF; LF-normalized SHA-256 matches the export for all1,866 records. Raw hashes are recorded separately in CSV. Normalized discrepancies: 0.

## Initial source hash snapshot (post-pause versions below supersede affected entries)

Hashes captured at report construction/rechecked immediately after. Initial earlier read hashes were not retained; an earlier concurrent mutation cannot be excluded. Candidate-marker source hashes appear in CSV and do not imply parity.

- `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` — `f88ceb1ed519625d059b20fefdffabf78ff65282d1b707760d8f0adb42b8b3ae`
- `port/android-native/app/src/main/cpp/model_renderer.cpp` — `68df536883d56f0cd74f5b47052daafd07491faf5a75eec901790fd26afbe015`
- `port/android-native/app/src/main/cpp/original_ui_session.cpp` — `c4db25ae30ec4bc60ffda88594b8e724c10564c4b1520ee229972894a5b9dc76`
- `port/android-native/app/src/main/cpp/source_campaign_character_frame_v111.cpp` — `7c1e3ab60388ce086676d1e0f0f1b2d97ff32a81c91c02028fc62ff680b7b739`
- `port/android-native/app/src/main/cpp/source_campaign_release_v88.cpp` — `d98ae5cebbd12db699de9c9e22d2bc2a621736e60624f6ffe7f7db43b3fb74b3`
- `port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp` — `6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e`
- `port/android-native/app/src/main/cpp/source_campaign_zones_v83.cpp` — `63782810dabad4c646f2dbda48edf5af6ad791d0b3d1be335bd18e7ee701a447`
- `port/android-native/app/src/main/cpp/source_main_menu_leaves_v114.cpp` — `856695564e3d4ef45e745ff42dc5c8825c70abc44927464c648818a4adc2794d`
- `port/engine-ui/authored_character_menu_routes_v1.cpp` — `2278432a5203c45a2f65a67070141181d927152e2aaf27cb25f0aa907a27ef4e`
- `port/engine-ui/character_menu_as_bridge_v1.cpp` — `d4eed9e0068e06533654fc74d239f5d5a79134b60f4ad721838195844467c302`
- `port/engine-ui/character_menu_queries_owner_v1.cpp` — `6eadda418201913f83566f26e8e8ab14d364f4051b66a30e8cdfbe088e83a7d3`
- `port/game-data/item_inventory_v1.cpp` — `8659c2db2da2e361e6ab4353f85a3cd9859f452890426b8bf5fa07f8d98ecdd6`
- `port/game-data/item_inventory_v1.hpp` — `e5f67b4cf43a9c726c79eb1a0dc5455738a7fc1b1a0f6ce0a4b7aa987186a847`
- `port/game-data/player_savegame_v1.cpp` — `ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43`
- `port/level-loader/command_c1_descriptors_v59.inc` — `1141188625d183b9e61150654e9b7111eedeb15deab2e1a7c7a710a547e41e07`
- `port/level-loader/level_player_placement_v68.hpp` — `6cea6637d54926cdc5d3fce0a223ca76572fa37b9815a3949c02d0e3c4a7509b`
- `port/level-loader/level_script_paths_v51.hpp` — `eb7d0281174ce34dc397652dc7f34a25837c3434073c28c9d46ceebf82887408`
- `port/level-loader/level_unload_source_v1.hpp` — `6419f0dac9f21a521389a53cbf9b34c8fc85edb418a065a6e11e82ae691270c4`
- `port/level-loader/script_command_execution_v96.cpp` — `c0f46c590960ed92f40cf167ac9f6e209f2c1fbd67d9fd75d504a169975edded`
- `port/level-loader/script_command_factories_v52.inc` — `951153e59985b87f52e1f4c591c5bb1af0de5bd9b4faee3dd95874c8401a687e`
- `port/level-loader/script_command_receivers_v59.cpp` — `e5c4a4345e5cc5a409eabdff3c93b40cb7d9659005415393b222a56be944fac2`
- `port/level-loader/script_data_schemas_v52.inc` — `fbdb3d7b6480937b3b93817fca9f258829bd2fd234940f503f020c12f4e811a2`
- `port/level-loader/script_manager_execution_v96.cpp` — `e2d26215cf9a29dbe823547ec9e5d651b66715a3c74c977fcf114833ba4d9763`
- `port/level-loader/script_manager_owner_v52.cpp` — `263c7ac802be1a4747c81b904db0c37824aea031434aef965350b4305c825b42`
- `port/level-loader/script_manager_owner_v52.hpp` — `fd7a912f40be81fb171621d98685a4a9f7ced0bbf36cb831a67261a1d1556dd8`
- `port/level-loader/stage_loader_checkpoint_v1.hpp` — `e451cadbb410477a077dd31407969d3f7fa4bf922fe181b6790cc7d79173b222`
- `port/level-loader/stage_loader_v51_config_scripts.hpp` — `180379d82de69607067f9cf7d3932ff19d428ce1472561788f9640e0a7117874`
- `port/level-world/canonical_trigger_object_v28.cpp` — `b3b28f69c5ae96aaec80ca1b5ce0223cd16d9ce7b750842e5bcdf2d3cede5673`
- `port/level-world/character_ai_attack.cpp` — `8f85ccec8afd24c0d3b47cf5ecad897a53364a7e2b467c19a831b987d0473562`
- `port/level-world/character_controller_commands.cpp` — `68cb770d894bb2504947678701a03173a6a6cb693ac47df080b18723c96bb342`
- `port/level-world/gameplay_camera_damping_v1.cpp` — `84e5de5d02ef505a7b8d8b494862e6e1d8970e68ce5d15880a3caa1d08d592b2`
- `port/level-world/light_set_name_owner_v3.cpp` — `b23974c16157058ec0130e7cd43f478c2ffee3660d771f7c98b5e4776e13b877`
- `port/level-world/player_controller_attachment_v70.cpp` — `3c12550e9a0eca5ea1cd08698ba14f80761b606c0763d94e80e1618be411ea30`
- `port/level-world/player_level_quicksave_v29.cpp` — `c0150a2372db98216802afb25bdde6f6e88d31e13adfa5df08baf48ab160c4a4`

## Byte-identical assigned code groups

- `0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec` — 5648, 5650, 6067, 6069, 6088, 6090
- `fa04763cc015275f81b1d864426513948ce8bfb90e0889b5098b558301847cdd` — 5669, 6075, 6096, 6312, 6314, 6318, 6321, 6325, 6327, 6332, 6345, 6347, 6690, 6698, 6730, 6828, 6831, 6838
- `4bf140484fc2f9fea647ea2040137ada36b996e3161e9f98a95e6dd5ca3f3244` — 5697, 5698
- `379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f` — 5699, 5700, 5901, 5902, 5903, 5904, 5905, 5906, 5907, 5908, 5909, 5910, 5911, 5912, 5913, 5914, 5915, 5916, 5917, 5918, 5919, 5920, 5921, 5922, 5971, 5972, 5973, 5980, 5981, 6046, 6047, 6053, 6054, 6060, 6098, 6099, 6130, 6131, 6139, 6193, 6194, 6197, 6198, 6330, 6331, 6356, 6359, 6360, 6361, 6364, 6370, 6371, 6372, 6401, 6417, 6418, 6419, 6420, 6421, 6422, 6423, 6424, 6575, 6576, 6577, 6623, 6655, 6657, 6676, 6678, 6679, 6808, 6863, 6864, 6868, 6869, 6870, 6871, 7162, 7163, 7173, 7193, 7195, 7197, 7199, 7201, 7228
- `34b97ea285a49248d21f74e46f26848d3f2c1a90e381ca2c3fe219705331487e` — 5701, 5982, 5987, 6202, 6264, 6554, 6624, 6798, 6842
- `6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877` — 5759, 6310, 6333, 6430, 6431, 7164, 7165, 7166, 7167, 7168, 7169, 7170, 7171, 7172, 7174, 7175, 7176, 7177, 7178, 7179, 7180, 7181, 7182, 7185, 7187, 7188, 7189, 7190, 7191, 7192, 7194, 7196, 7198, 7200, 7202, 7203, 7204, 7205, 7206, 7207, 7208, 7209, 7210, 7211, 7212, 7213, 7214, 7215, 7216, 7219, 7220, 7221, 7222, 7223, 7224, 7225, 7226, 7227, 7230, 7231, 7232, 7233, 7234, 7235, 7236, 7237, 7238, 7239, 7240, 7241, 7242, 7243
- `7dacf1801d93c81fdc0b91850ff950c71de2e972dff198461c38db25df91d5e3` — 5923, 5924
- `81fc02196ce9d39e50dd0caec6a3324bd533fc28a510c8f96f1482715a5ca477` — 5930, 5931
- `691c5ddb063af36a37e0b43c3e9109db3f4d9fd5270c3f8c049dc38a66d36bdc` — 5961, 5963, 5965, 5988, 5990, 5992, 5994, 5996, 5998, 6000, 6002, 6004, 6006, 6008, 6010, 6012, 6014, 6016, 6018, 6020, 6022, 6024, 6026, 6028, 6030, 6032, 6034, 6038, 6040
- `007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47` — 6061, 6262, 6329, 6425, 6427, 6428, 6675, 6691, 7229
- `a1cb3ffb8b3e253f53d250d96222c4c09013c9651ace8339f035251c9baf246a` — 6132, 6133
- `982e4d439db97b660345b346393a13679d4ab5509bbd982a30207acfafa8d479` — 6163, 6167, 6169
- `1e9cfba2aa079a322a19a6e3bac84d327f853a96a689809f4bd87eb9d62dcdb5` — 6195, 6196
- `0c90168747506508f1694f11816da2bc0d35c37c142aab9979d7df81dee44c72` — 6199, 6200
- `b4eac6cb7a0f09fb886862ee433215f092dd81da844a58cdd3d4fb983b94a791` — 6229, 6230
- `f416402943ee916b41746cb17d079a0ca62008ef27542498aafc48b605416f85` — 6365, 6783
- `7d4973e2d49315e4a2de98e41e720d422c790f5ff48c20f93cd780638220f7c3` — 6685, 6687
- `641488d8a5049f3409b94ba9ddcc287ae2a2f08047e2c7cfb9800afc82153898` — 6692, 6693
- `3114164fa30b4921f7b7ca1d220d4acd799dad2d01258778b99c590c668c8a19` — 7183, 7184
- `2c3e08165ca6bf10b2269f429d5758f45b40f0ce61d6e0dd467c2898c29d318c` — 7244, 7245
- `e960d776ae2bfb05e7f2bd756805366f4de24f1edec9504b1fe30be3056ae422` — 7246, 7247
- `0d6332da55006986382d1269bf36292b9dc9f928e3fceaba66d199ca71497eb7` — 7256, 7257

Source reread: no hash changes during report construction.

## Post-pause bounded source recheck

This section supersedes stale citations/hashes for the six affected citations **5720, 6151, 6655, 6673, 6910 and 7455**; dependency row **6843** additionally records the newly inspected font-reset owner edge without changing its unclear status, following post-pause-source-impact.md. All six original bodies were reread; selected assembly verified setup/add-child/ref-drop, HUD invalidation/Touch and ScriptManager low-byte reset. Statuses remain **unclear, unclear, clone, unclear, unclear, matched**, respectively. Four formerly machine-only bodies were now read, giving494 body-inspected records /276 hashes including dependency #6843,1372 still uninspected. Source-comparison completion is not promoted: the five candidate/clone rows retain unfinished whole-function connections. Full37,813-record semantic audit remains incomplete; all runtime fields stay not_run.

| Row | Refreshed source scope | Remaining qualification |
|---|---|---|
| 5720 CharacterTable getter | CharacterGameDesign.cpp:51/69/72; game_design_tables.cpp:13; renderer:944/967; session:699 | Local CharacterTable still first strcmp/miss-1. New extra-group delegation pins its process owner, but actual group readiness and every caller/load/reload route remain unresolved. |
| 6151 shake admission | original_ui_session.cpp:825/838 | Positive Character branch is connected to SAME Level/Camera128 and local-player predicate. Null diagnostic and other original callers unclosed. |
| 6655 LostFocus | native_menu_process_bootstrap_v104.inc:47; native_character_menu_v4.inc:337 | Literal original no-op is accepted on the same named render. Exact clone remains; complete focus/lifetime coverage not asserted. |
| 6673 SetupScene | native_menu_preview_v121.cpp:90; renderer_native_menu_preview_v121.inc:182/184; renderer:1409 | Physics-before-scene and add-child/ref-drop order represented; geometry publication explicitly preserves physics/camera. Original Collada ownership and failure/retry domains not fully closed. |
| 6910 HUD refresh | original_ui_session.hpp:108; session:553/1068; process bootstrap:20; renderer:1799 | Actual InfoHUD and controls state reached through settings refresh. All local-roster/controller/null/error branches unclosed. |
| 7455 Flush | script_manager_owner_v52.cpp:93; release adapter:341; renderer:3127 | SAME-manager reset still performs low-byte store; retain bounded matched effect. Unload/gameplay acceptance separate. |

### Process design owner and teardown

CharacterGameDesign::Snapshot retains process_design_owner (cpp:49/99). Its six local registrations include CharacterTable (69–79); extra known groups delegate at58–63. Renderer load_game_design:967–972 borrows OriginalUiSession's current shared process owner and passes it into that snapshot. The session publishes at1167 under a mutex; current borrow699–703 checks a retained owner, not ready70/all names. SourceProcessArrays get_member_id:75–89 has per-group names/member readiness and genuine empty-size/miss rules. OriginalUiSession destructor1144–1145 only retires the matching global weak lender; strong existing design loans remain pinned. Readiness, initialization timing, VM borrowers and whole reload behavior remain bounded rather than inferred from ownership alone.

Menu setup reaches native_menu_preview.setup_scene90 through prepare_main175; its service construct_scene at renderer_native_menu_preview:184 captures the loaded physics backend, calls geometry publication with release_world_owners=false, and verifies that backend afterward. Current renderer1409–1416 preserves loaded physics/camera on that branch. Source destroy_scene42–55 releases the actual root, only drops a still-owned failed constructor reference, then removes scene nodes/objects/animations/textures. This establishes an actual owner route, not complete original Collada lifecycle parity.

NativeCharacterMenuV4 is shared-owned (native_character_menu_v4.inc:337), with process admission make_shared at native_menu_resources_v93.inc:260 before prepare_process_directory261. Its enable_shared_from_this base (:5) therefore supports the new weak reset callback. Some other construction sites assign make_unique into the SAME shared_ptr; the allocation expression alone is not evidence of an expired weak owner. Menu teardown releases front/HUD transport and Flash menu-update ownership before disposal (native_character_menu56–73); OriginalUiSession HUD D0/slot clear1445–1453 and transport release1460–1465 retain same-owner/error gates. Failure/reentry/lifetime completeness remains unverified.

### Font-reset owner edge (dependency #6843, not a newly completed CSV disposition)

Original MultiMenuManager.ResetFonts #6843,0x437a38 walks **all four** nonnull primary fields134/138/13c/140 (ARM437a44–437a64). Current NativeSetOptions Language path applies the option then invokes reset (swf_menu_options.cpp:50–53); front_ui_session_v87.cpp:488–491/1820 forwards the registered reset. Process bootstrap113–116 locks the actual shared NativeCharacterMenuV4 weak owner. reset_process_fonts119–135 borrows actual populated slots0/2/3 and checks receipt/movie identity before SwfMovie::source_reset_fonts_v119. Source primary1 enrollment also exists (native_menu_resources263–265), but this reset expressly skips slot1 (:124). The populated level-owned slot1 reset path was not established in this bounded pass; **do not call this a matched complete four-slot reset**. No scoped missing classification is made.

SwfMovie375–391 pins its movie owner, permits same-owner synchronous AS reentry, clears edit-text values, then selects the font platform by that actual player. Font platform217–224 calls exact texture releasers before clearing text/font/core/image registries; TextRenderOwner126–128 invokes the backend clear hook before its face/image/clone clears. Separate movie services original_ui_movie_services_v1.inc:4–20/44 retain the resource Impl and route exact image release to that GPU owner. Failed later release/reset can retain a reached prefix; rollback, duplicate-image release, same-owner control-block identity across every facade, primary1 coverage and all live language/teardown cases remain unverified. These new edges provide no integrated runtime acceptance.

### Current bounded recheck hashes

The original eight principal files were hashed at first bounded read and reread before report publication. Additional cited dependencies were hashed at report construction; their earlier initial-read hashes were not retained. The current fingerprints below supersede those files' initial-audit versions only within this section and the six updated CSV rows.

- `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` — `3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638`
- `port/android-native/app/src/main/cpp/model_renderer.cpp` — `de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca`
- `port/android-native/app/src/main/cpp/native_app.cpp` — `a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3`
- `port/android-native/app/src/main/cpp/native_character_menu_v4.inc` — `d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557`
- `port/android-native/app/src/main/cpp/native_gslevel_menu_v27.inc` — `3927943baa41c71b3c3f4a30511a8cb4b795e89849e8584528661cae608e2ce2`
- `port/android-native/app/src/main/cpp/native_menu_preview_v121.cpp` — `c4b5b58f21dec3e0e95861c2b6c486c120aed1f12e9557ad0c4a248e24af73d6`
- `port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc` — `21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d`
- `port/android-native/app/src/main/cpp/native_menu_resources_v93.inc` — `c4ed613435541f69ef9d668c926e4713ae3e62dcd757888a90529d200581a57e`
- `port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc` — `d6fa672f4b1b8de09dc8df6519bb8e3c5bf170cea82e0ca4eb47698b3fb2fa2f`
- `port/android-native/app/src/main/cpp/original_ui_session.cpp` — `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18`
- `port/android-native/app/src/main/cpp/original_ui_session.hpp` — `4efa3ebdd7cb260492ad7be7817737a59fdd0c742b3149812f5141617b16388f`
- `port/android-native/app/src/main/cpp/renderer_native_menu_preview_v121.inc` — `83697221da867b93aa04c668a9c8af1f64a11e38136837ca26e6a37c7ca9c541`
- `port/android-native/app/src/main/cpp/source_campaign_release_v88.cpp` — `d98ae5cebbd12db699de9c9e22d2bc2a621736e60624f6ffe7f7db43b3fb74b3`
- `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp` — `a663cf0ae3dacbbafb1882774954877472b057b5ab6b3ea9f3453dc2cd607fb0`
- `port/engine-ui/swf_menu_options.cpp` — `b838646cd0b36ec2eb97a1b14e2d28078a30420db854aa8f9d870e30918b8fc1`
- `port/engine-ui/swf_movie.cpp` — `3e2f1e905273ad2ab659417f2466ae93194162559d36f771d8d6c7334de079dc`
- `port/engine-ui/swf_text_font_platform_v1.cpp` — `30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8`
- `port/engine-ui/text_render_owner_v2.cpp` — `6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140`
- `port/game-data/game_design_tables.cpp` — `a7c5a884708d27c379e80187aecf40873e40b0881202809fde8fa3e1bd181dba`
- `port/level-loader/script_manager_owner_v52.cpp` — `263c7ac802be1a4747c81b904db0c37824aea031434aef965350b4305c825b42`
- `port/level-world/character_game_design.cpp` — `51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8`
- `port/level-world/character_game_design.hpp` — `7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e`

Principal source changes during this bounded read: none detected. Final dependency reread changes: none detected.

## Additional post-sweep provenance recheck — row6308

**#6308 GameSWFUtils::PreloadGlyph,0x41724c remains unclear.** The stale source-facade overlay hash6ba7aa32… is replaced by current0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8. The relevant selected overlay helper is now **swf_movie.cpp:583–605** (wrong-type guard587), not former569. The production helper at port/engine-ui/swf_movie.cpp:325–347 was also reread and rehashed.

Full original body and assembly were inspected. Original ABI is three static arguments (R0 codes,R1 Character,R2 MenuFX); IDA's inferred GameSWFUtils*this/fourth argument is misleading. It rejects absent/wrong-type Character, uses field text when codes is NULL, calls RenderFX.PreloadGlyphs for each44-byte filter and once unfiltered using field font-name/style/height20, and sums integer preload results. Current helper finds the actual edit-text/movie-player platform and clones a name/bold/italic descriptor before native glyph materialization. Its loop uses text-glyph-record fonts then the field font. Null-code fallback,filtered record propagation/count equivalence,the summed integer result,all font/style/filter branches and failure/lifetime parity remain unresolved. This prior row had only machine indexing,so changed fingerprint alone cannot establish a semantic regression against a previous proved match.

The actual source selector was checked: port/engine-ui/CMakeLists.txt:104 calls dh2_select_source_facade_v1; gameswf_source_facade_v1.cmake:26–27 removes the production TU and appends overlays/source-facade-v1/swf_movie.cpp. The overlay is the selected implementation for this owner path. Its live Init23 caller native_process_startup_v119.inc:210–213 requires main2 and preloads six font fields with digit/uppercase/lowercase ASCII packs. That connection does not establish all original filtered or nullable-text domains. Earlier font-reset discussion cites the production facade body; selected overlay ResetFonts369–385 follows that same visible owner/root/platform flow,without broadening #6843's unclear disposition.

Only row6308 and this report were changed in this follow-up. CSV still has1866 unique assigned rows; status unchanged; runtime remains not_run. Aggregate body inspection now495 records/277 hashes,1371 records/1350 unique hashes unfinished. The source-comparison completion count249 and exact unfinished comparison IDs remain unchanged because this is a bounded unclear comparison.

Current recheck fingerprints:

- `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp` — `0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8`
- `port/engine-ui/swf_movie.cpp` — `3e2f1e905273ad2ab659417f2466ae93194162559d36f771d8d6c7334de079dc`
- `port/engine-ui/CMakeLists.txt` — `92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625`
- `port/engine-ui/gameswf_source_facade_v1.cmake` — `e7608376d2f022c333580ee93edc531c583cc5cd4483979a44d0e550962c3946`
- `port/android-native/app/src/main/cpp/native_process_startup_v119.inc` — `256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa`

All five fingerprints were reread after this pass: no changes detected. No game source/build/test/emulator work.
