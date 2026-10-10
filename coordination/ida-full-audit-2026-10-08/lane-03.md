# Lane 03 — incomplete semantic audit

Assignment: `libDungeonHunter2.so`, function numbers **3732–5597** (1,866 records). Baseline marker from the coordinator README: `75a7c2fe3261403e841ffbeb46734e9e4e84e75c`. Source was read from the dirty working tree.

**This lane is not complete.** The CSV has a disposition row for every assigned record; row coverage does not imply semantic coverage. Detailed pseudocode review covers 3732–3947 (216 records), 5060–5084 (25 timer/helper records), and 4071 (Character RaiseEvent). The post-pause bounded recheck adds 4329, 4764, and their in-lane unique-set-ID neighbor4077. Another 158 exact literal/no-op records elsewhere were inspected as eight byte-hash groups, with every member listed in CSV. The inspected total is **403 records**; **1,463 remain unfinished**. Their rows explicitly use `review_state=unfinished`, `body_category=inventory_unreviewed`, and `comparison_status=unclear`.

All 1,866 records report successful decompilation. No failed decompilations or external import records were encountered in this range. There are 1,559 distinct assigned byte hashes. Every runtime value is `not_run`. No build, test, installation, emulator launch, source edit, or checklist edit was performed.

`matched` below describes the stated local comparison, not complete integrated acceptance. `clone` describes an inspected identical literal/no-op body; separate class dispatch and source mapping can remain unknown. A helper name or source-search miss is never treated as a missing game implementation.

## Findings and system paths

### L03-F1 — destructible InitPost calls Sync where the original applies its mesh box

**Partial; high static confidence; default callback path.** Original `Container::InitPost` #3859 at `0x39f910` calls `VisualObject::ApplyMeshBox` after prespawn/spawn/idle selection. ARM instruction `0x39fa34` branches to `0x470a54` (`ApplyMeshBox`). `DestructibleContainer::InitPost` #3893 at `0x3a11dc` calls the base InitPost before replacing event callbacks and establishing animation stages.

Port `port/level-world/canonical_destructible_container_v16.cpp:38` calls `services_.visual_sync`. The native default supplied at `port/android-native/app/src/main/cpp/renderer_campaign_noncharacter_v69.inc:985`–988 calls `visual->sync(error)` on the same retained visual. `port/level-world/retained_gameobject_visual_v1.cpp:249` updates root pose/world transforms/skinning. Its separate `apply_mesh_box` at line340 writes flat state and relative/absolute AABBs and calls `update_pf` at lines349–352. Thus this default continuation does not perform the original ordered bounds/PF operation at this point.

An externally prefilled `visual_sync` can override the default. Earlier visual initialization may also apply bounds; that does not establish equivalence of the later operation. The scoped failure/unverified path is a destructible with a visual, using the shown default services, reaching Container InitPost after animation selection. Integrated gameplay effects were not run.

### L03-F2 — the destructible player-only stat gate is not represented explicitly

**Partial; medium confidence; actual actor/stat lender unresolved.** Original #3887 `0x3a0da0` first decrements remaining animation stages and plays sound when stages remain. Otherwise it raises DestroyGameObject, invokes Container Interact, obtains Handle-as-Character, calls selected virtual `0x28` IsPlayer, and only then increments stat217. Count>199 and local-player checks precede the destroy_200_breakables trophy. Assembly `0x3a0f34` dispatches IsPlayer and `0x3a0f3c` exits on false before `0x3a0f50` AddInt.

Port `canonical_destructible_container_v16.cpp:55` tests `as_character`, then line56 directly increments stat217. Its services at `canonical_destructible_container_v16.hpp:32`–35 have Character conversion, increment/get stat, and local-player callbacks, but no explicit IsPlayer callback. The native lender shown at `renderer_campaign_native_lenders_v89.inc:264`–272 prepares visual/script data and does not supply this actor/stat continuation. A separately supplied callback might narrow to players; its implementation was not established. This is a gate/connection uncertainty, not a whole-project missing claim.

For comparison, Openable #3918 explicitly checks IsPlayer in `openable_container_interaction_v2.cpp:29`, with a concrete same-Character provider at `source_campaign_container_targets_v104.cpp:30`.

### L03-F3 — TriggerTrap whole methods and Lua receiver closure remain unestablished

**Partial/unclear.** #3799 InitPost `0x39dee0` orders RNG gate, TriggerTraps row ID/visual selection, Zone InitPost, condition, same-trap timeline callback registration, sound, and external script loading. #3797 binds inherited GameObject plus GetOwner/GetDamager/Remove as function and method registrations with the actual trap context. #3815 calls OnTrigger and vetoes only an explicit first boolean false; absent/no-return/nonboolean results allow triggering. #3818 sends the authored event name to OnAnimEvent.

#3822 Update `0x39eedc` orders ZoneEx Update, visual Sync, selected SpecificUpdate, pending-victim damage (OnDamageRoll or GOAttack/ApplyResult), transfer into the previous-victim set, and pending-set clear. #3823 excludes victims already in either pointer-identity set before calling CanTrigger. Completion #3810 clears previous victims after activation, returns to idle, and handles removal through Disable/Delete.

The canonical V37 class represents fields and both victim sets, but its methods require whole_init_post/whole_update/whole_destroy callbacks (`canonical_trigger_trap_v37.cpp:25`–32). `catalog_generic_v70.cpp:80`–88 enforces same receiver/authority and obtains methods through lend_trigger_trap; lines135–138 bind that wrapper. Native renderer lines1057–1071 wrap preexisting optional generic leaves with lifetime checks. A concrete original-method body supplied to lend_trigger_trap, including its Lua registrations, was not established. The class name and callback requirements cannot justify parity or missing labels.

Original assembly resolves misleading pseudocode parameter labels. GetDamager #3813 uses R2 as trap context, retains R1 as ReturnValues, dispatches trap virtuale4, and pushes the integer. GetOwner #3814 pushes owner3f8 from the trap in R2. Completion #3810 uses the trap context in R1. Projectile callback #3755 takes Projectile in R0, reads impacted object3cc and source trap380, calls GOAttack then ApplyResult, and returns0 for a valid Character hit or1 otherwise. These receiver/ABI contracts matter for future integration.

### L03-F4 — TimerTrap, ProjectileTrap, and SlotContainer owners are unmapped

**Unclear, not proven absent.** #3756 ProjectileTrap SpecificUpdate and #3777 TimerTrap SpecificUpdate were read completely: application-dt countdown, expiry/activation tests, delay reset before spawning/activation, and activated victim transfer. ProjectileTrap #3759 obtains projectile and active fields after TimerTrap InitPost. Create methods require class/type16 or17, assign owner/hurt override/position, execute InitPost/Spawn, and add an initial room object.

Slot #3932 cycles its powers/duration/RGB material-effect entries while the container is not opening/opened. #3943 calls Openable InitPost, parses slot powers/time, derives ItemInstance RGB divided by255, and retains GL_ material from the actual visual scene. Constructors start index/time at -1 and empty slots; destruction releases material, vector, and strings before the Openable parent.

Scoped rg searches over `port/level-world`, `port/level-loader`, and the native app, excluding report/vendor/reference/test copies, found original factory entries, table registration, and projectile activation command fields. They did not establish corresponding concrete runtime owners or authored asset reachability. A factory table name or textual-search miss does not demonstrate semantic parity or absence. These reviewed records remain unclear.

### L03-F5 — container paths preserve several local semantics; network delivery is unresolved

Container SetState #3841 clears scene400 for state3, sets it otherwise, and stores state afterward. The clear/set callback at `container_animation_connection_v21.cpp:9` confirms that operation. Spawn #3842 changes state0 to1 before playing spawn. Completion #3843 handles state1->2/idle and3->4/idleactive, otherwise clearing200 when original IsActive returns false. Its wrapper passes `timeline.loop!=0`; exact IsActive equivalence was not established.

Opened #3880 calls loot/OnOpen; other event names forward OnAnimEvent. Destructible opened #3896 raises DestroyGameObject with null actor before DoOpen; fx #3885 is a real original no-op. Original DoOpen omits the opener argument entirely when null, requiring preservation of argument count by the script leaf. Openable Unlock/TryUnlocking preserve the signed16 inventory quantity and consume flag. Derived key-name lookup #3919 remains after the base spawn gate. V21 callback connections weakly capture and pin the actual retained container during delivery.

Save/restore #3855/#3858: `noncharacter_save_connection_v89.hpp:80`–114 locally retains current-Level-before-reset checks, no-read branch, qualified base stream order, state-byte consumption even when visual is absent, SetState, fresh KeepPhysics lookup, and cached visual playback. Complete savegame consumer and leaf closure remain unverified.

Network incoming #3863 and outgoing #3865 require opened-bit handling, opener network-ID translation, state reset/open transition, physical attach/detach, animation selection, and optional unique-ID update. `ContainerNetStructV1` represents ordered 1/16/32-bit logical members and explicitly separates codecs/transmission. Their concrete delivery and history-tree lifetime were not established; the static struct is insufficient acceptance evidence.

### L03-F6 — CharTimers core agrees locally; storage and expiry closure remain partial

#5060–5084 were read. Source TimeLeft returns elapsed and duration separately. In-range pause/resume/stop and out-of-range no-mutation behavior agree. The lowest inactive slot is reused. Start resets active, pause, repeat, duration, elapsed, event, and reference. The original constructor reserves20 slots; native storage/growth is separate.

#5074 Update blocks on ScriptManager30, uses application dt, snapshots initial slot count, and catches up expiry. Repeat0 deactivates before a final event; positive repeat decrements after subtracting duration; negative repeats continue indefinitely; duration0 never fires. Default event is41 and payload is the Timer object, preserving ID/reference access. `character_timers.cpp:30`–43 implements this core. Native guards add explicit failure codes and reject growth while updating (line19), so complete equivalence is partial.

`character_script_timers.cpp:44` installs StartTimer/StopTimer. The live frame at `source_campaign_character_frame_v111.cpp:148`–154 passes same dt8c and ScriptManager byte30 through source_campaign_character_timers_update_v102 before AI/FSM/animator/GameObject. Native expiry receiver, reentry, and BuffExpired paths remain unverified. #4071 Character RaiseEvent sends event54 to CharProperties::BuffExpired and other events to CharAI. The examined native raise at `source_campaign_character_fsm_v101.cpp:290` routes the AI kernel; the complete selected BuffExpired continuation was not traced.

## Cross-boundary and hash evidence

CSV records incoming references, including callback data refs, and direct calls/tail branches to actual IDA function starts. Local branches are excluded. Relevant edges include TriggerTrap InitPost -> callbacks39e738/39ec78; createBindings ->39ea64/39ea90/39dd18; TransferVictims ->CanTrigger39ea9c; Update/projectile callback ->Character F_GOAttack3b061c/F_ApplyResult3b01b8; Container InitPost ->ApplyMeshBox470a54 and Lua load38ef60; DoOpen ->ItemObject DropLootTable3ecba0; interaction ->EventManager339090/character properties/trophy; Timer Update ->Character RaiseEvent3a4d5c. Zero direct call xrefs can mean a virtual entrypoint; zero does not prove unreachable behavior.

Original assembly was read for every inspected thunk and the ABI/order questions above. Byte-identical adjusting thunks can have different absolute targets because PC changes. CSV preserves each original target and classifies those as import/thunk. Identical data/getter/call bytes also require per-address relocation semantics; grouping alone is not parity. Every assigned ID has its code hash and full assigned-group list in CSV.

Most nontrivial Character/formula/CSM/Lua/stats, state-machine, animator, AI/AI scripts/properties, projectile-manager, door/spawn/item/liftable, and Level paths remain unfinished. The following generated ledgers name exact IDs and source fingerprints.


## Post-pause bounded recheck — RegisterAnim and animation dictionary registration

This recheck follows `post-pause-source-impact.md`. The earlier session snapshot f1604fe5 is superseded by8ab0110f; the earlier FSM snapshot3c4ceb5d is superseded bye4a9925f for the bounded citations reread here. No prior whole-subsystem coverage is promoted. #4329 and #4764 remain **unclear**, now with inspected bodies and concrete current source connections. Neighbor #4077 was also inspected locally and remains unclear. The updated inspected total includes these three records; all other unfinished IDs remain explicit.

**#4329, Character::_RegisterAnim (0x3b70d8).** Original pseudocode and ARM assembly require a present numeric arg0 and otherwise do nothing. No return values are pushed. Assembly uses callback user Character inR2 plus0x49c (CharAnimator), converts float through __aeabi_f2iz at3b712c, then tailcalls #4764. Current `character_script_session.cpp:212`–226 preserves the ordinary guard and calls the session's registration provider; :304–308 selects original callback3b70d8 for RegisterAnim/RegisterActorAnim. Provider failure now returns REQUIRED_SERVICE_FAILURE (:225), whereas the new finite/range rejection (:220–222) is an ordinary callback diagnostic. Exceptional original float-conversion behavior was not established.

The concrete campaign provider is `source_campaign_character_fsm_v101.cpp:747`–754, borrowing the animator from the same retained Character visual and calling source_add_animation_dict_v116; bind_npc at:768 supplies it. This establishes a current selected NPC service path. Complete registration in every global/method scope, preview/player dispatch and VM close/finalizer lifetime remain open.

**#4764, CharAnimator::ANIM_AddAnimDictToSet (0x3c9b14).** Original disabled38 gate precedes all effects. When set3c==-1, its unique ID is computed/stored before testing dictionary ID<0. Current `character_same_scene_animator_v6.cpp:5`–9 preserves this order. `retained_character_family_visual_v6.cpp:8`–10 supplies the same-actor formula table<<8|skill_list, matching the local #4077 body. Current positive-ID path uses the retained set manager, real dictionary/clip bank and registration cells, debug trace, and render rebind (:10–25). Campaign preparation at `renderer_character_campaign_v62.inc:310`–320 supplies the manager, dictionary and actual Debug provider.

Original manager neighbor #8006 AddAnim (0x47653c, outside this lane) was read as an edge: it looks up/creates a set, handles a negative set key, and conditions LoadAnimation/Debug on the existing set's frozen3c byte and newly-created LG_DEVICES branch. Complete equivalence of manager/device guards, shared path aliases, failed prefixes/default libraries, late render attachment and teardown was not established. Native renderer command Init also calls the same method for both animation words (`renderer_campaign_script_init_v116.inc:28`–31). No acceptance is inferred from the method being present.

**New lookup/failure neighbors.** Original GetPyOID #2996 (0x37f5fc, outside this lane) was read: two exact string arguments capture the process arrays owner, call GetOID and push its integer result. Current `script_design_bindings.c:19`–30 preserves no-effect arity/type guards and a successful numeric -1 result; `character_script_session.cpp:228`–237/334 translates delivery failure into REQUIRED_SERVICE_FAILURE. The VM trampoline (`script_runtime.c:89`–92) increments required_failure_epoch before a Lua diagnostic, and source-status call:441–453 returns required failure if that epoch changes even if authored Lua catches the diagnostic. The GetPyOID lookup now precedes the RegisterAnim adapter through this wrapper.

`character_game_design.cpp:48`–66/99 pins and delegates known original groups outside the six local tables to process_design. `model_renderer.cpp:944`–950/967–972 borrows and passes current process arrays. `original_ui_session.cpp:699`–703 provides a mutex-protected weak-owner loan, published at:1167 and conditionally retired at:1145. `source_process_arrays_v101.cpp:75`–89 distinguishes successful member miss/empty declared row count (-1) from missing names delivery failure. Thus the earlier six-table-only absence cannot describe the provider-equipped initializer. The no-provider overload still exists, and failed current-process borrow can leave that path.

Remaining lookup obligations: timing of process publication and completed names/data loading versus design initialization; equality/order of process AnimDict IDs and the animator's separately decoded dictionary; exact live owner under reload and UI retirement; and all selected source callbacks through finalizers. A published/pinned owner does not prove ready groups. Historical Stage17 logs and external fixture/build claims were not treated as current-source runtime acceptance. **Runtime remains not_run.**

### Rechecked source fingerprints

Current hashes were captured and rechecked for this bounded comparison. Initial table snapshots for unrelated files remain historical; the session/FSM entries above are refreshed only for the explained rereads.

| Source path | Current SHA-256 |
| --- | --- |
| `port/level-world/character_script_session.cpp` | `8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e` |
| `port/level-world/character_script_session.hpp` | `c29d1a90ed5648e942db68820afe0fc3c370bdbb9272f9ca8e426870e2a55633` |
| `port/level-world/character_game_design.cpp` | `51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8` |
| `port/level-world/character_game_design.hpp` | `7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e` |
| `port/level-world/character_same_scene_animator_v6.cpp` | `8a65600fd518e7a7407f6571b9ac54f69710100329469765b531f010dcb68eee` |
| `port/level-world/character_same_scene_animator_v6.hpp` | `4cdacdd12b8fc60cc77d8bb110e19a184698ed4d1207947dbbdc6f6efa19efc1` |
| `port/level-world/retained_character_family_visual_v6.cpp` | `279a016b9f1d58664738825fdee47f412922fe3e25633f10a06549decb477049` |
| `port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp` | `e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391` |
| `port/android-native/app/src/main/cpp/source_process_arrays_v101.cpp` | `fde21042364d703a50a51fb7785aaac3acd258e9d8519f7f55a4af80c3ec197a` |
| `port/android-native/app/src/main/cpp/model_renderer.cpp` | `de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca` |
| `port/android-native/app/src/main/cpp/original_ui_session.cpp` | `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18` |
| `port/android-native/app/src/main/cpp/renderer_campaign_script_init_v116.inc` | `38ad6e66382229ba72647bc81791644c93c583fb602b7f24eeff0eddb0123a75` |
| `port/android-native/app/src/main/cpp/renderer_character_campaign_v62.inc` | `ad287e056ae6117d991da925e80576433ec8f7292867e8098cf2a58e347cf27b` |
| `port/script-runtime/script_design_bindings.c` | `d3fa2b17d974aec16a130ab9f6ffd547e89f5cc24b26f18e98f970ad8b931e0a` |
| `port/script-runtime/script_runtime.c` | `571f4009dfac62b4251d9e4555f6b3cf343166a29d8f65ab92d5bc9d55391b9b` |

Changed during finalization: none detected.


## Coverage ledger

Exactly one CSV row exists for every ID3732–5597 (1,866 rows), with zero duplicates or omissions. Inspected:403 records /202 assigned code-hash representatives. Unfinished:1,463 records. Status counts: {'partial': 67, 'unclear': 1567, 'import/thunk': 28, 'matched': 23, 'clone': 181}. All runtime statuses:not_run.

### Exact unfinished IDs

3949-3960, 3965, 3967, 3970, 3976-4005, 4008-4013, 4015-4070, 4072-4076, 4078-4165, 4167-4303, 4305-4328, 4330-4452, 4454-4500, 4543-4735, 4738-4763, 4765-4806, 4808-4900, 4903-4924, 4927-4987, 4990-5047, 5049-5054, 5056-5059, 5120-5121, 5123-5133, 5135, 5137-5166, 5169-5175, 5178-5220, 5224-5302, 5304-5317, 5323-5373, 5378-5436, 5441-5452, 5457-5496, 5499-5502, 5505-5542, 5546-5548, 5552-5554, 5556-5573, 5578-5579, 5585-5586, 5588, 5590-5591, 5593-5597

### Source fingerprints

SHA-256 values were captured at report finalization and rechecked then. No source change was detected between cited-row hashing and finalization. Early reads were not individually fingerprinted, so this does not prove prior stability.

| Source path | SHA-256 |
| --- | --- |
| `port/level-world/canonical_trigger_trap_v37.hpp` | `0abd413886be191e877c397075a99d5574d80b8b06640af07bbf1bb7fe28c8ab` |
| `port/level-world/canonical_trigger_trap_v37.cpp` | `c8c65e8f5c727f7190b866d61c3e293e4f7cc21bdfa60b81b1637dff65ba16b8` |
| `port/level-world/canonical_trigger_trap_declarations_v37.inc` | `cf8ea822e132eea5de78fcb2cd7967793544cfec5b19f906e7538b123946296d` |
| `port/level-world/canonical_exit_zone_v29.cpp` | `583c95b07dd911506eb168eb1b6e56952ffe9aa7fb292fbed225e02cae97e8a4` |
| `port/level-world/canonical_property_map_v1.cpp` | `d2f32d25e001815237b74e1203a5927d912c8f13e744e41db177c0c9e26d4a72` |
| `port/level-world/canonical_property_declarations_v1.inc` | `5525db00248e259892e569d240352ef154cbc9cc807f65022d6759d9211224a3` |
| `port/level-world/openable_container_owner_v1.cpp` | `2de92939e0661c71ae4958a0ed29c553c73d26f9b4a32b4947f9a4a723d9a290` |
| `port/level-world/openable_container_owner_v1.hpp` | `13b2bcccb6c33413eac7f06087c79d0ba1c411b41c76e705aa2ae1bdfd189a74` |
| `port/level-world/canonical_openable_container_v1.cpp` | `b930290fac48910afaf41410af4a04e5415f81367024a454e7f522884082ee09` |
| `port/level-world/canonical_openable_container_v1.hpp` | `c72b0ba8c7e4e1cefa11cfaba63378bbf37923b71188e6626d8c70b077920dc9` |
| `port/level-world/canonical_destructible_container_v16.cpp` | `071ef899e0a5c1b7c60c6e4d6bb09b85b4292dca3365afca380f77e8dfa8441f` |
| `port/level-world/canonical_destructible_container_v16.hpp` | `ed5cf7597b4bb8eaa4621fd08a030f54f2ef59ac0ccb1e97c37da0834ecc35d0` |
| `port/level-world/openable_container_interaction_v2.cpp` | `8ee7faff7f0223f9195d7318aca43cda524b0f0641a7ad3efd58c8144210e54e` |
| `port/level-world/container_animation_connection_v21.cpp` | `199e491464deeceefa25162ce4064f414c4206a6740931e0ea948230e8b1a74d` |
| `port/level-world/retained_gameobject_visual_v1.cpp` | `8b5bab7038393ac5b5d438a592bc1d88216b968b7f2295f6f80fe70667246701` |
| `port/level-loader/catalog_generic_v70.cpp` | `7155c7680847e931edc9b24387d2292706d21f2f24299845adb04432543d2d0d` |
| `port/level-loader/catalog_containers_v67.cpp` | `3198a8617e55673b0ee01d6e3755ef12109658d262493300a9f739b5c8d1b59d` |
| `port/level-loader/noncharacter_save_connection_v89.hpp` | `d7ac98f9cd449a01c4204c812946b0b56e0aac5b35e40828cebd68e6b8516615` |
| `port/android-native/app/src/main/cpp/renderer_campaign_noncharacter_v69.inc` | `f0dbe3cfa4bbe61bb1ee2c9266704b4b264c32629a2006aaea6547a004c04fa4` |
| `port/android-native/app/src/main/cpp/renderer_campaign_native_lenders_v89.inc` | `1c1c91ca264ee39fbe436b79cbf482845b0277bb53d2c04fc9d7654cd1ec779b` |
| `port/android-native/app/src/main/cpp/source_campaign_container_targets_v104.cpp` | `5c50b5e0c4b7915f35468adaf94b001116c19b0d737d4921b6c649b79a55cb7e` |
| `port/android-native/app/src/main/cpp/source_campaign_noncharacter_virtual_v105.cpp` | `bd8ba3052c62f2e46fe688db83d7b2284cfee464fe24a7efa8a6b0063e25344a` |
| `port/level-world/character_timers.hpp` | `22c775ebf458563dfe1dccfe519b94cbd0e519897bf1c85bec1ec2d111eb0883` |
| `port/level-world/character_timers.cpp` | `8a7ea63fb09debcf850731c59ff7530fd5cc10d9b6753d3dfb6c97ae413c71f4` |
| `port/level-world/character_script_timers.cpp` | `4e43fafc6ed2f58998fc53630ab0e8397045608af1f9a1bbcffb381778a6fa21` |
| `port/level-world/character_script_call_timer.cpp` | `d0dd468be95f75c40d31903c78d583815a1d3ae0678a37baebfa07df0ca7fceb` |
| `port/level-world/character_script_session.cpp` | `8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e` |
| `port/android-native/app/src/main/cpp/source_campaign_character_frame_v111.cpp` | `7c1e3ab60388ce086676d1e0f0f1b2d97ff32a81c91c02028fc62ff680b7b739` |
| `port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp` | `e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391` |
| `port/level-world/canonical_object_factory_v1.cpp` | `84eb8a2eb9aa8909f8d3f9918ec0fde83d92e434d69a4ce64978a8259bb08463` |

### Repeated assigned byte hashes

Singleton and repeated groups are fully represented in CSV. The groups below include unfinished members; grouping is not semantic review of those members.

| Code SHA-256 | Assigned function numbers |
| --- | --- |
| `fa04763cc015275f81b1d864426513948ce8bfb90e0889b5098b558301847cdd` | 3737, 3760, 3781, 3811, 3876, 3894, 3928, 3946, 4147, 5413, 5451, 5469, 5517, 5572 |
| `007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47` | 3741, 3742, 3744, 3745, 3835, 3837, 3845, 3930, 3948, 3961, 3962, 4014, 4166, 4925, 4926, 5115, 5320, 5321, 5374, 5375, 5377, 5454, 5497, 5498, 5544, 5545, 5549, 5583 |
| `6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877` | 3743, 3826, 3827, 3832, 3909, 4453, 4902, 4988, 5319, 5322, 5376, 5437, 5438, 5439, 5440, 5453, 5455, 5456 |
| `c366d8ed454478fb5e15477fad1fa194964eeb90cc6feae9d03055eddf8bb1ab` | 3746, 3838, 5550 |
| `d1513099dfb32f6b726ef0291358899b1e740ef2dcd0aca545af729041c22a30` | 3747, 3839, 5551 |
| `f68520e7644c47739b5b74691da29778feb6b6b8fdeed6ee79f647d2e3ded16e` | 3748, 3965, 5378 |
| `0ded1e40bf45877f3e1ebf69d486a944141c910157afc4f3813ea8d390a694ec` | 3762, 3764, 3783, 3785, 3803, 3805, 3854, 3857, 3869, 3871, 3897, 3899, 3920, 3922, 3936, 3938, 3967, 3970, 4023, 4026, 4091, 4093, 4117, 4126, 5328, 5330, 5346, 5348, 5389, 5392, 5409, 5411, 5443, 5445, 5461, 5463, 5531, 5534, 5563, 5565 |
| `379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f` | 3807, 3825, 3840, 3867, 3881, 3885, 3905, 3966, 3968, 3969, 3971, 3972, 3973, 3974, 3975, 4006, 4007, 4304, 4501, 4502, 4503, 4504, 4505, 4506, 4507, 4508, 4509, 4510, 4511, 4512, 4513, 4514, 4515, 4516, 4517, 4518, 4519, 4520, 4521, 4522, 4523, 4524, 4525, 4526, 4527, 4528, 4529, 4530, 4531, 4532, 4533, 4534, 4535, 4536, 4537, 4538, 4539, 4540, 4541, 4542, 4736, 4737, 4807, 4901, 4989, 5048, 5055, 5085, 5086, 5087, 5088, 5089, 5090, 5091, 5092, 5093, 5094, 5095, 5096, 5097, 5098, 5099, 5100, 5101, 5102, 5103, 5104, 5105, 5106, 5107, 5108, 5109, 5110, 5111, 5112, 5113, 5114, 5116, 5117, 5118, 5119, 5122, 5134, 5136, 5167, 5168, 5176, 5177, 5221, 5222, 5223, 5303, 5503, 5504, 5543, 5555, 5574, 5575, 5576, 5577, 5580, 5581, 5582, 5584, 5587, 5589, 5592 |
| `6143299c415a779d14d46483b05772d81c6b595bc189b4499503e30275041932` | 3828, 3829, 3830, 3831, 3833 |
| `cb40dc3e600a334d856e94a92521cecd3fa2335af7172f9674889e706fcfe9a6` | 3892, 3915 |
| `e34f159d8496478038eb98a6eaabe206fc47d8561808b5e411c705b0a406c17c` | 4067, 4069, 4141, 4143, 4145 |
| `b8e49fcd24da872f8fe48770d935d29de0d6cf358ee178bb6cc935aee415ae63` | 4103, 4106 |
| `9346390478ac25f31c420e82a8e92f8ead0ebdeac0168e3d688f6f4d3e50ebf6` | 4110, 4119 |
| `db718df0c3e09676ca54829169f9bcedbd0232fd7b91f37a681fef56378a6aca` | 4111, 4120 |
| `5d8df14fcec8b1f37b797ed88bfbd68a5ff1d473f73ab900c22403caca3cb4f0` | 4112, 4121 |
| `275660862d8d99bd3fb4cbbc0ebf116da54d4553dc132e7238c46903c923bc17` | 4113, 4122 |
| `49a94345e0524d97e34920d7852a943aaea785ecea7ede3f46e336666488c61d` | 4114, 4123 |
| `a83908297796698553c42fa2204f25242f4ae2ebc8022ddc67365bd5580f3d84` | 4115, 4124 |
| `d425755ee355475fbdc50f106b0c61db9dd281730bbf6058594b5d3bc4f4c998` | 4116, 4125, 5530, 5533 |
| `02086debb5ef29a5e70623a7de864053dcdc20adde824d74145c13221c616272` | 4152, 4156 |
| `0ee3db94754220238f4c8f1ff4ef8984adb728ec143e17cf4a59214839ca2807` | 4174, 4177, 4179, 4181, 4185, 4187, 4189, 4191, 4193, 4195, 4197, 4199, 4201, 4203, 4205, 4207, 4209, 4211, 4213, 4215, 4217, 4219 |
| `5e288ab97c9fcfb2cea933fa4a23dfdffd06487b9580d1734e0aaf2ea9af94c6` | 4223, 4451 |
| `c46facc1fb70ff1029982a4e602c159575a670781d6724931d9e02b5b3002f65` | 4224, 4225 |
| `ba8c6a19d644bb9bd0ecf6050d7b56bb33b38c1e6387e8ac792168e5b1c2bb78` | 4564, 4565, 4566, 4567, 4568, 4569, 4570, 4571, 4572, 4573, 4574, 4575, 4576, 4577, 4578, 4579, 4580, 4581, 4582, 4583 |
| `120f58fc4d34a09358235e4827ffe0957086b44bde498d5f2e5f02ec009d31bc` | 4627, 4628 |
| `519789b0ed5a1369c249c454406cd73a9006e48a4f59557628c7001ed4c79f32` | 4706, 5296, 5433 |
| `329f0de66165da8b8c18a43a2147d46c2ec03bd162130486e585149fd005cdcd` | 4707, 5297, 5434 |
| `e046450c17e35671e1d5bcb077486c5b74429dc24c100f611e8fc8edb20d9c92` | 4734, 4735 |
| `b576532d6b53caa8f2bad02c29ba3ab863085da18345cfa566a53ed64c9154b0` | 4783, 5225 |
| `0e3aabbbc8cf294e938d9f598be3b6cc0d1fcdb24ded33eb25cf84ca9823f134` | 4786, 5226 |
| `99ee4ab69abba2b6427e07103be06ed2a1af107a58a2d74a27e32716a6480ed1` | 5121, 5500 |
| `ebbe76554669e36e2b2d711acd2cfde19936e24f7935bc73d35da4feff61c998` | 5323, 5336 |
| `ddb4491878566e71b4405d4305a9f7611692e454b185d84e12ac44ed778e109c` | 5354, 5355 |
| `f0856b562d01b119f6a7af46d8bab52f254efdc315f0e92a131773a15648aa43` | 5357, 5358, 5476 |
| `5d2f9d169accfda4cc84d9b348870926dcd85074c515405f9041f9b684193e7c` | 5361, 5364, 5479 |
| `d7e6d4a66bb7f9c458f501f67a687c64f78090566bc07781ce603c639ec2f800` | 5415, 5416 |
| `a5d1ebb661b3004e8aa73d2371157e025609734a594fa060d11bec5372cee31b` | 5471, 5472 |
