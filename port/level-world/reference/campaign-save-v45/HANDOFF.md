# Campaign save V45 frozen source package

Archive `handoff-v45.zip`, SHA256
`18314133ffcd7381f6f6b543c1aead0e1a10a890117935a5482f0fc796cf45d6`.
`manifest.json` records 45 source/reference/test/receipt entries. This document
is supplemental and does not alter that archive.

Production level-world TUs: `private_save_file_transport_v45.cpp`,
`campaign_save_profile_v45.cpp`, `player_save_metadata_writer_v45.cpp`,
`player_save_inventory_writer_v45.cpp`, `player_save_collections_writer_v45.cpp`,
`campaign_save_filename_v45.cpp`. Game-data adds
`player_save_state_fields_v45.cpp`; rebuild all consumers of changed
`player_savegame_v1.hpp` and `quest_savegame_v1.hpp` coherently.
Existing dependencies include PlayerProfileIndex, SavegameJobsOwnerV2,
savegame_build_frame_v2, PlayerSaveLoadOwnerV1, source cache tables/inventory
and exact current PlayerSavegame/QuestSavegame owners. Renderer supplement is
`renderer_campaign_save_bindings_v45.inc`; it is staged, not published live.

The actual Android application supplies its private files directory and pins
ONE transport and ONE SavegameJobs owner. It constructs the actual campaign
profile receiver at the existing SaveLoad create_profile boundary. Source
filenames are `dh2_%03u.savegame`, `dh2_%03u_single.checkpoint` and
`dh2_%03u_multi.checkpoint`; flags choose the suffix. Transport preserves
source `.bak`/w+b/job order, with bounded modern regular-file/path protection.
Queued save jobs do not establish disk persistence before their real delivery.

All fifteen source tag writers are present as typed composition. Positive
Quest child data still requires the actual condition/objective serializers;
no empty success is supplied. Full live registration/restore requires actual
MapLoc names and default records (not FastTravel), genuine QEST restore, real
global current difficulty, current profile and save identities, application
job delivery and reached online/checkpoint selectors. Do not replay C1 zeros
over restored state or publish full campaign save readiness before these bind.

Validation: 8 production TUs × both Android ABIs, staged renderer × both ABIs;
O1/O2 ASan/UBSan disk/profile jobs22, actual-cache metadata/inventory/PROP232,
original metadata320, original filename24, collections217 and SAME Save
field/read-prefix15 checks per optimization. Quest child and outer load-effect
callbacks remain fixtures. These are source/host proofs, not a live complete
15-section campaign roundtrip claim. Weak registration/provider leases prevent
the profile→writer→SaveLoad→profile ownership cycle found during sanitizers.
