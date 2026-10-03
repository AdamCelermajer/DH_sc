# Prince registration/default-library producer

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; cache SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. The 32-routine manifest and assembly are saved alongside these notes. `probe.py` executes the actual original registration producer on cache-backed native-format structs; `load-probe.py` separately executes actual append and unique-map operations.

## Resource identity

`KnightPlayerBase` is character-property row263. Its `AnimTable` property is48; `SkillTree` is14. Actual `GetCharAnimTableId` (`3a3228`) reads Character+1000 and `GetCharSkillListId` (`3bc5c0`) reads +1068. Actual `GetCharUniqueAnimSetId` (`3a54b4`) returns `(table<<8)|skill`, therefore12302. Invalid animation-table IDs fall back17, invalid skill-list IDs3; this probe supplies the valid source row.

Character table48's `Template` field at native +90 is sequence279. `_AddTemplateAnimTable` (`3c9c7c`) accepts its single direct step and tail-calls `AnimSetManager::AddTemplateAnim` (`476398`) with dictionary ID1111. This resolves to:

`data/3D/characters/prince/animations/prince_template_anim.bdae`

Exact cache entry: `com.gameloft.android.GAND.GloftD2SS/files/data/3d/characters/prince/animations/prince_template_anim.bdae`; 19,908 bytes; SHA256 `23054c4f06f75cdb677541ca728f5f689f8e018b3f05f25008297172ff49a102`. It has81 full transform channels (27 each quaternion5, scale10, position1),27 scene nodes, and DB clip bounds0..3599ms. It is an animated resource, not a zero-track graph template.

The separate character `ModelFile` property is dictionary78, `data/3D/characters/prince/prince_modular.bdae` (35 scene nodes). Actual `GetCharModelId` (`3a31e8`) reads +1004 and returns78 with the installed original dictionary count. The exact model cache entry/bytes/hash are in `probe.json:model_property`. `GetCharModelName` (`3a54d4`) additionally contains equipment/game-state override branches; those branches are captured but not executed in this bounded probe. Model dictionary78 does not establish animation-default identity. Fixture model key20000 from earlier coordinator audits is not the source template registration.

Actual `AddTemplateAnim` calls `LoadAnimation` at47641c, then loads the returned Animation path again at476438, and calls `setDefaultAnimationLibrary` (`62fc90`) at476444. `load-probe.json:add_template` executes this common body with an already-created manager set fixture and verifies both loads designate the same1111 resource handle. The two resource loads produce one library append and one default selection.

## Ordered registration

Actual `CharAnimator::SetAnimationSet` (`3c9f4c`) first checks manager `Exists` at3c9f78. An existing set causes zero further registrations. For a fresh player set it queries the actual cache constant labels `AnimStances/COUNT_IPHONE`=5 and `AnimStancedAnim/SL__LIST_IPHONE`=210, then registers the template before ordinary clips.

The complete result is **158 ordered resource requests backed by116 distinct dictionary IDs**. `probe.json:registration_calls` preserves every request, including duplicates, in actual call order. `resources` contains116 exact unique cache identities in first-seen order; `first_unique_order` contains those dictionary IDs. No numeric map sorting is used to infer registration order.

Before the five-stance loop the actual caller reads fields+58(Limbus),+64(PreSpawn),+80(Spawn),+5c(MenuIdle), in that order. The first three are-1 for this row; MenuIdle registers1063. MenuOnSelect+60 is not one of these calls.

The stance loop follows the original instruction sequence: Idle, Walk, Run, Attack, AttackStatic, Scared, Stunned, KnockedBack, GreatKnockedBack, Injured, Blocking, Dodging, Died, Despawn, DeadlyGreatKB, DespawnGreatKB, Revived, Reviving, Walk180, Run180, IdleOOC, IdleToOOC, IdleFromOOC, IdleSneak, WalkSneak, RunSneak, LiftIdle, LiftMove, Interact[], Spells[], SkillList[]. Native offsets and all recursive table requests are retained in `native_character_table_fields` and `table_calls`.

`_AddAnimTable` (`3c9d80`) allows a variant when `(stanced_mask & requested_flags)==requested_flags` or the stance index is0. Zero flags therefore allow every stance, rather than meaning only stance0. Redirect steps recursively call this routine at3c9f1c with variant0/flags0/mask0. The original zero-flag registrations can cross adjacent sequence IDs: the resulting116 resources include four root-troll clips. These original requests must not be silently filtered by a Prince filename prefix. Skill-list14 supplies sequences347,350,349 and thirteen-1 entries.

The current coordinator17-clip fixture projects onto the source list as follows (zero-based library occurrence indices):

| Dictionary ID | First index |
|---|---:|
|1040|2|
|1041|7|
|1126|8|
|1114|9|
|956|11|
|959|14|
|962|17|
|967|19|
|955|20|
|957|21|
|969|22|
|958|23|
|960|24|
|971|25|
|961|26|
|963|27|
|1023|41|

This is a projection, not the complete source bank. The template is library occurrence0, MenuIdle occurrence1.

## Duplicate append versus game lookup

`LoadAnimation` (`3659ec`) resolves dictionary paths with native row stride12/path+8, loads the database, and appends to the dirty dynamic set through virtual+2c at365b94. It performs unique game-ID map insertion only afterward at365c8c. The original database-vector append (`6601d4`, through `62e8a8`) does not deduplicate.

`load-probe.json` executes actual original `LoadAnimation`, actual unique red-black-map insertion/lookup, and actual `UpdateAnimationIndices` (`364af4`). Requests `[1111,1040,1040,1040,1040,1040,1041]` produce7 library entries but3 map keys. Game map indices are0 for1111,1 for1040,6 for1041. Repeated1040 registrations append every time while returning the first inserted map value. No map insertion is replaced by a test service.

`UpdateAnimationIndices` asks `getDatabaseIndex` (`62dbb8`) for each retained map value. That routine compares the **first CCDB word**, the reference-control/resource identity, and returns the first matching vector index. It does not compare dictionary ID or the second CCDB word. The resource loader in the probe preserves repeated path identity; real cache loading/destruction remains a service boundary. An occurrence adapter must preserve158 engine library positions independently of116 game-ID records and map each retained game's first resource identity to its first engine index. Deduplicating the compilation input would lose source indices.

## Current native reader/compiler boundary

All116 requested resources resolve uniquely in the supplied original cache and all116 pass the current native `Player::load` validation against the35-node Prince model using explicit `MissingTargets::ignore`. `reader-probe.cpp` is linked to the existing audited ASan/UBSan direct source objects; `reader-probe.json` records the executable/source hashes and0 sanitizer findings. This is reader validation, not original pose parity.

Ten resources contain17 additional transform channels outside the original bounded full-node1/5/10 implementation:10 position-axis channels and7 scalar angle channels with authored axis defaults. The original factory and post-load recovery in `../component-angle` corrected an earlier mistaken scale interpretation of type9.

| IDs | Extra channel types/targets |
|---|---|
|1029,1028,1027|positionZ4 on left/right weapon anchors (2 each)|
|984|positionY3 on right weapon anchor|
|1022|positionZ4 on left weapon anchor|
|1009,1014|positionX2 on left weapon anchor (1 each)|
|1077,1078|angle9 on three calf/forearm nodes (3 each), scalar keys plus authored axis defaults|
|1079|angle9 on one forearm node, scalar keys plus authored axis default|

They have single channels/samplers and no compression. Legacy Player still reports these17 as skipped. Native dynamic TransformSet now accepts position2..4 and authored angle9 after original-factory compiler/sampling proof:9,376 comparisons, including template-first, extra-clips-first and no-template registration. Static TransformSet remains restricted to full channels. The full158-entry bank and live coordinator integration require their own checks; this subset proof does not claim either. Six full-node tracks in IDs1147,1133,1146 are unbound to this model; the dynamic compiler keeps such union entries and node application skips their null binding.

## Probe boundaries and reuse

`probe.py` supplies Character IsPlayer, constant lookup, manager registration/file loading and leaf script/FX/audio services. It executes SetAnimationSet, the recursive registration helpers, and actual Character table/skill/model-ID getters. It neither runs equipment/property inheritance nor real file caching. Native table structs are reconstructed from verified cache bytes solely to run the original request producer.

`load-probe.py` supplies identity-preserving CCDB handles, string-storage allocation, profiling and clock services. Actual map and vector routines execute. `AddTemplateAnim` uses an explicit already-created manager-set lookup fixture. Destruction and historical cache reference ownership are not claimed.

Run both Python probes directly with the existing Unicorn/Capstone/ELF dependencies. Optional `probe.py --resources-output <workspace scratch>` extracts the116 unique verified bytes for the isolated reader audit; it does not edit Android assets. All production source/CMake/APK files remained unchanged in this recovery task.
