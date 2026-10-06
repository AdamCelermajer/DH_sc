# Same-player authored ChangeFaery V8

New files: `character_menu_faery_connection_v8.hpp/cpp`, `level_faery_placement_v8.hpp/cpp`, `renderer_character_faery_v8.inc`. They preserve the frozen V1 ABI and compose existing V1 menu action/query transport over the same V6 Save, skills, spell vectors and property/FSM owners.

## Source corrections

Original ChangeFaery3ae99c: fresh difficulty→SG_GetFaeryCount assertion→SG_SetCurrentFaerie→CharAI.UpdateAllSkills→fresh Character420. If nonNULL: Character.GetCharModelName3a54d4→GameObject.SetVisualObject394d34(model,NULL,true)→CharAnimator.ANIM_AddSetToRenderObject3c99a0. The earlier V1 callback names `retarget_effect` and `current_hud_player/set_faery_interface` remain ABI-compatible names only; the old position/movement/effect interpretation was incorrect.

NativeHUDSetActiveFaery outer43ef1c..40 actually calls Application.GetCurrentLevel31f594 twice, then Level.PlaceFaeryAndFollowers3f0898(selectedCharacter). It does not issue a separate HUD refresh. Existing per-frame HUD and FaeryCastV2 read the selected ID from the same Save and source V6 spell vector.

Character420 is embedded CharAI58. Its genuine constructor storesNULL at3cec78; positive Level placement publishes the same field at3f09a4. `PlayerFaeryAssociationV8` is additive backing for this previously missing field, not a new faery actor. Root must create it once at actual player CharAI construction, retain it across menus/restore as appropriate to that CharAI owner, and never reset it per UI dispatch. Existing actor adoption must borrow/adopt an actual field rather than silently default it.

## Root insertion

Include the two new headers and renderer include after `renderer_character_mutations_v4.inc`. Retain one `PlayerFaeryAssociationV8` on the same player CharAI lifetime (actual source identity). Call `supplement_character_menu_faery_v8(out,borrow,error)` after the existing mutation supplement. `borrow` builds genuine source currentLevel/model/visual/animator callbacks and calls `current_world_faery_services_v8(p,association,source,placement,out,error)`.

The helper already binds SAME canonical manager Character list, source GetCharType via actual AiTables/character.resolved[1], source difficulty, real PlayerManager count6c4/GetPlayer/GetLocalPlayer, actual Save selection and writable player420. No duplicate registry, Save, skills, TimerStore or HUD state is constructed. Required missing reached callback names propagate honestly.

Source faery type3 and follower type2 are proved IsFaerie3a3094/IsFollower3a307c→GetCharType3a3054→GetCharAI→row38. Normal monster4/player1 rows skip before position/visual calls after real list traversal; no blanket NPC predicate is used.

## Positive branch providers

`LevelFaeryPlacementV8` implements the complete NONNULL-selected-player domain needed by NativeHUDSetActiveFaery. Qualifying actor: GetLookAtVec(selected)+GetTargetPosition(selected)→SetPosition(actor,true)→ForceUpdatePosition. Follower then DisableZoning. Faery: publish actor420 into each actual PlayerInfo character→read old master50→GetLocalPlayer(0,true)→AI_SetMaster(localCharacter or selected)→SG_GetCurrentFaerieId(-1) from that actual character→nested Character.ChangeFaery(selected,id) ONLY→restore old master. The nested callback MUST exclude the outer NativeHUD Level-place tail to avoid recursive placement.

The null-selected caller's original online/global-player branches remain explicit required continuation. Campaign positive faery actors still need same retained visual/controller/animation/AI master services. They cannot be represented by NULL fixtures, invented model names or spawned unlocked rows. This package does not claim full faery factory/InitPost or campaign unlock.

## Proof

Actual ELF functions and symbol sizes are captured in `original-functions.asm`/`symbols.json`. Both modern ABI renderer-context strict syntax passed through a new temporary copy only; root model_renderer was untouched. Android source placement regression: `reports/android-native-owner-tests/level-faery-placement-v8/receipt.json`, actual APK55164f78, 40 checks, real CanonicalObjectManager publication of12 fixture Character receivers. Declared position/visual/player services test source order, association/master mutation and missing nested-Change failure prefix; it does not claim live campaign faery rendering or real PlayerSkillV6 VM callback execution.
