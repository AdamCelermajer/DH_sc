# Crypt monster animation bank producer

This recovery executes the original non-player `CharAnimator::SetAnimationSet` and recursive registration/getter bodies against exact cache tables. It stages four portable metadata banks and 64 unique animation resources outside source APK assets. Manager allocation/loading, IsPlayer, constants, script, FX and audio are explicit probe services. PAB1 is the port's owned metadata format; it is not original serialization or a reconstruction of original CCDB allocation.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Cache SHA256: `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. The 40 captured routines are in `original-functions.json` and `reference/original-functions.asm`; four executed probes bind the manifest, script and exact input hashes.

## Actual producer and ordered ownership

`Character::InitPost` 0x3b4d60 calls `CharAnimator::SetAnimationSet` 0x3c9f4c at 0x3b5090 on the embedded animator at Character+0x49c. SetAnimationSet calls `GetCharUniqueAnimSetId` 0x3a54b4, stores the set ID at animator+0x3c, then tests manager Exists 0x475404. The existing-set branch emits no registration requests; each probe verifies this separately.

SetAnimationSet's IsPlayer virtual call at 0x3c9f9c selects the non-player branch: one stance, not the five player stances. It queries `AnimStancedAnim/SL__LIST_IPHONE` (210). `GetCharAnimTable` 0x3a3264 captures the table pointer before obtaining its index. The source raw SkillTree is -1 for all these actors; `GetCharSkillListId` 0x3bc5c0 yields fallback 3 and `GetCharSkillList` 0x3bc5fc obtains the empty list. Set ID is `(animation_table << 8) | skill_list`.

Template registration is first, through `_AddTemplateAnimTable` 0x3c9c7c and manager AddTemplate 0x476398. The latter uses the common loader, then a separate load selects the default library. The common pre-loop order is Limbus, PreSpawn, Spawn, MenuIdle; only Spawn contributes here. The subsequent one-stance order is Idle, Walk, Run, Attack, AttackStatic, Scared, Stunned, KnockedBack, GreatKnockedBack, Injured, Blocking, Dodging, Died, Despawn, DeadlyGreatKB, DespawnGreatKB, Revived, Reviving, Walk180, Run180, IdleOOC, IdleToOOC, IdleFromOOC, IdleSneak, WalkSneak, RunSneak, LiftIdle, LiftMove, Interact, Spells and the selected skill list. `_AddAnimTable` 0x3c9d80 executes recursively; redirects and repeated resources are retained.

The previously proved common `LoadAnimation` 0x3659ec appends the engine library before unique dictionary insertion. The refreshed dictionary points to the first library occurrence with that resource's identity; `getDatabaseIndex` 0x62dbb8 compares the original CCDB reference-control identity. Native `RegistrationSet` therefore retains every request and preserves first-resource-identity lookup. Unique resources must not replace the ordered occurrence library.

| Source property kind | Row | Anim table | Set ID | Template dictionary ID | Requests | Unique resources | Idle sequence |
|---|---:|---:|---:|---:|---:|---:|---:|
| Crypt_Skeleton | 38 | 62 | 15875 | 1203 | 44 | 23 | 596 |
| CryptSlime | 33 | 64 | 16387 | 1267 | 37 | 23 | 613 |
| CryptSlime_RE | 34 | 64 | 16387 | 1267 | 37 | 23 | 613 |
| Crypt_Ghost | 35 | 24 | 6147 | 716 | 28 | 18 | 210 |

Both slime property kinds select the exact **slime_green_v2.bdae** model and the same source set. `slime-red` is a scratch filename tag for CryptSlime_RE, not a model-color claim. Four metadata files preserve per-property authored placements; there are three logical source set IDs. The original manager's existing-set gate permits sharing immutable resources, while each character needs its own scene, playback clocks/cursors/root history and state owner.

Source placements from the hash-bound object-input provenance are four skeletons (`_prim_tmp_cultist01/03/07/08`), four CryptSlime (`_prim_tmp_cultist05/06`, `_prim_Monster`, `_prim_Monster05`), one CryptSlime_RE (`_prim_Monster02`) and two ghosts (`_prim_Monster_Ghost05/06`). Raw authored properties identify models; these probes execute GetCharModelId, not GetCharModelName's equipment/player override pipeline.

## Runtime CharAnim row and Idle Focus

The original CharAnim reader 0x4ef64c projects 37 schema fields into a **160-byte/40-word** runtime row. Interact and Spells are count/pointer pairs, not single integers. The portable projection in each metadata JSON keeps the source scalar word positions, zeros pointer words 17 and 34, and stores the actual arrays separately. Interact count is 9 with all entries -1; Spells count is 0. Idle is +0x28 (word10), Template +0x90 (word36), Walk +0x94 (word37). A caller must not cast the schema's 37 decoded fields to this layout or treat zero pointer placeholders as original addresses.

`CSIdle::OnFocus` 0x3c3020 performs the source profiling/log calls, then reads Character+0x538. A nonzero byte returns before flags or ANIM_Set. Otherwise it writes Character+0x520 = **0x2380** at 0x3c30ac; it captures the CharAnim table pointer, obtains the row, selects Idle+0x28, queries the actual stance mask and obtains stance0 through `GetAnimStance` 0x3a53e0 for non-players. That getter still queries `AnimStances/COUNT_IPHONE` (5). `ANIM_Set` 0x3cacb0 receives the sequence on the embedded animator. There is no Idle Focus pin, body removal or speed setter. `CSIdle::OnBlur` 0x3c2d3c clears +0x538. Each kind's probe exercises suppression bytes 0, 1 and 255 and observes ANIM_Set as an explicit service.

Actual Idle descriptors in cache animations_pyarray.bin:

* Skeleton596: loop -1, Type2, alternatives [1194,1195], authored speed1 and blend100ms.
* Slime613: loop -1, Type2, alternatives [1258,1258,1258,1258,1258,1259], authored speed1 and blend100ms. The repeated alternatives affect the genuine scheduler RNG producer.
* Ghost210: loop -1, Type0, clip706, authored float32 speed **1.2999999523162842**, blend100ms.

Use the existing original-derived animation scheduler and explicit shared RNG; forcing the first idle resource would lose these source decisions. Skeleton registration also requests FX preload77 via original 0x4967e8. It remains a required FX backend boundary rather than an accepted no-op in live initialization.

## Exact resources and native compatibility

The three templates are actual animation resources, not defaults synthesized from the model graph:

| Template | Bytes | SHA256 | Raw signed bounds |
|---|---:|---|---|
| skeleton_template_anim.bdae /1203 | 35488 | 98025d7f847ed2b22ff25e3cd4f5e1ad3969575590bb6d3e1553811a079b704e | 0..800 |
| slime_template_anim.bdae /1267 | 5744 | c39e568ebb138f16cbbae64abf9cb1b5f0a71f9f2deaefc38b24ce4734f9b503 | 0..2666 |
| ghost_template_anim.bdae /716 | 28556 | a5db032f75f68a2be336eb2eb770734191f762bb07d1f4587af47d14f5eb568b | **-133..1333** |

Ghost's start is original uint32 bits 0xffffff7b. Staging copies all bytes unchanged; there is no key/time offset normalization. Every requested resource uses supported full node position1/quaternion5/scale10 channels; none requires the Prince bank's scalar angle/component extensions. Model hashes, resource hashes, paths, original archive entry case and ordered IDs are recorded in each bank JSON.

The dedicated `native-reader-host-audit.json` executes the actual existing shared data, animation and scene libraries through `reader-probe.cpp`, with ASan/UBSan, not replacement source bodies. It validates native PAB1, Player, RegistrationSet and dynamic TransformSet against original-derived resource counts/bounds and first lookup indices: all four banks pass, **26,292** target samples, zero sanitizer findings. Both native Player and compiled library0 retain Ghost -133..1333. It does not execute original instructions itself or claim original full-frame/FSM/GPU parity.

The native union retains channels missing from the model. Skeleton has117 union targets/72 bound, slime22/18, ghost71/69. Native Player reports111 aggregate unbound skeleton tracks and4 for each other bank; unsupported/skipped tracks are zero. These are explicit source binding boundaries, not permission to invent nodes or prune the ordered union. Root's separate CharacterAnimationResources/Instance audit exercises actual owned Idle Focus composition and independent per-character playback; that is an additional composite adapter proof rather than a whole-game oracle.

## Deterministic isolated output and handoff

`produce.py` writes only beneath repo `.local-inputs`, refuses differing existing output bytes, and supports `--verify-only`. It checks the original ELF/cache/probe/manifest hashes and exact source-domain facts before emitting:

* `.local-inputs/character-monster-bank-assets/assets/data/monster-{skeleton,slime,slime-red,ghost}-animation-bank.{bin,json}`: PAB1 v1 owned metadata and source provenance.
* `assets/animations/`:64 exact unique original animation files, shared across banks.
* `assets/actors/{skeleton,slime_green_v2,ghost}.bdae`:3 exact original models.
* `.local-inputs/character-monster-bank-assets/producer-report.json`: every emitted asset's byte count/hash and producer inputs.

The four request lists contain146 total occurrences. Source APK assets, renderer, CMake, old Prince proofs and APKs were not edited by this task. No VM/cache/registration provider was fabricated. Next integration can use these banks with the owned resource wrapper, immutable canonical Player storage, actual default selection, per-instance scene/playback/state ownership and source Idle callbacks. Keep resources alive for borrowed bank identity; copy registration metadata before destroying its temporary owner. Preserve separate required event/AI/FX/camera providers and initialization order.

Reproduction uses the pinned direct Python with the capstone/elftools/unicorn cache on PYTHONPATH. Execute `probe.py --character KIND --output KIND-probe.json` for the four kinds, then `produce.py --verify-only`. `host-audit.py` requires the isolated already-built reader at `.local-inputs/character-monster-bank-discovery/host/reader-probe`; it hashes shared libraries before/after and refuses a changing dependency set. Build that reader with g++ C++17, ASan/UBSan, O1, no-fast-math/ffp-contract=off, linking existing dh2_engine_animation/dh2_game_data/dh2_scene_materials DSOs. It never rebuilds them.
