# Character menu exclusive writer handoff

Current worker `/root/character_state` stops writing after this handoff. Integration lead will replace it with GPT-6 Luna HIGH. This feature remains ACTIVE and unfinished; do not describe passing component tests as completed menu integration.

## Ownership

Own `port/windows-foundation/features/character_menu/` and feature reports. Root exclusively owns `main.cpp` and CMake. Current files:

- `character_menu.hpp/.cpp`: modal Presenter, current source fullscreen transform, shared actor/property/profile projection.
- `export_art.py`, `original_art.cpp`, `source_layout.json`: original SWF contours/placements, named icon states, new original colored solid contours, actual DefaultStats328+386.
- `menu_text.hpp/.cpp`: source first-line layout, current shared HudTextV1 StringManager cache, localized label symbol mappings, numeric OID, borrowed item text seam.
- `menu_stats.hpp/.cpp`: NEW exact NativeGetPlayerStats arithmetic + original combat-bonus kernel and default386 values/visibility.
- `character_menu_tests.cpp`, `menu_text_tests.cpp`, `art_export_tests.py`.
- Existing historical reports `reports/feature-character-menu.json`, own `source_text_v16.json`; older scope/status descriptions do not include latest v18 solids/stats and should be updated after runtime verification.

## What changed now

1. Actual audited FlashCamera.Update42cd84 calls SetViewport(driver W/H), SetBounds(mode0), so current `MenuViewTransform.scale_x=W/480`, `scale_y=H/320`, x=y0. `scale=max(axes)` is raster detail ONLY. Main root already updated geometry/glyph independent axes; `.local-inputs/v18-integration-verification/fullscreen-menu.png` confirms menu fills window. No fake background rectangle.
2. Source background shape90 actually covers approximately x[-.25,480.45], y[-.2,319.8]. Current menus have no active frame0 clipDepth masks. Earlier hypothesis that masks caused gray overflow was corrected to root/integration lead. Generic exact clipping code supports separately reached actual source mask scopes; active-menu visibility remains actual AS problem.
3. Export now includes default Stats386 right panel: actual source59545/59559 pushes CharacterSheetNew328 AND CharacterSheetStats386. Old first presenter omitted386.
4. Fixed CharIcon277 named detail-button frames Defence/Offence/Recovery/Magic/Stats. Fixed resistance347 frames Fire/Water/Lightning/Earth/Air. Top tabs262 use actual original AS named frames.
5. Added `Frame.solids`, `MenuArt.solids`, type `MenuSolidBatch { geometry, rgba, after_bitmap_role }`. These are exact original single-solid-fill source contours/colors, including white plus311 and disabled brown overlay315; no invented visual rectangles. ROOT MUST INTERLEAVE texture0 `OverlayRenderer.drawTriangles(vertices,0,solid.rgba)` after matching bitmap role, then draw text. Current source solids include exact timeline-order anchor. Multi-solid-fill/line-style/color-transform branches still unsupported/documented. Inventory callbacks removing/replacing role anchors need coordinated handling.
6. `frame()` now projects `NativeMenuStats` once from SAME `OriginalCombatProperties`. Attack50/class51+/dual58, crit63/class64+, defense59, armor71, resistance74..78, genuine `dh2_combat_bonus` for main/off damage79..82. All source integers use ARM ASR8 helper and wrapping additions. `original_stats_path_visible` skips hidden text BEFORE localization callbacks; no unnecessary unreached label lookup.
7. Stats visibility currently uses exact supplied facts dual/twohand and source elementType97/100<0 to hide LH/TWOH/RH and missing elemental subtree. STAFF/BOW special 2H distinctions remain UNBOUND: require actual main/off ItemRecord164 and exact `dh2_equipment_queries_v1`, do not infer from class/actor. This is an explicit next fidelity gap.
8. Static label symbols in `menu_text.cpp` now map right386 labels to actual source GAMEPLAYMENUS keys (attack/defense/armor/critical/resistances/statistics, RH/LH/TWOH). TWOH key corrected to `GAMEPLAYMENUS_SUMMARY_2_HAND`, but latest source key edit has not been recompiled; normal single-hand tests don't reach it.

## Shared text and item services

`MenuLocalization` was upgraded from Localization to SAME single `HudTextV1` owner, not a parallel cache. Existing load/label/symbol/string_id signatures preserved. NEW:

```
bool bind_profile(const CharacterState*, error);
bool borrow_text(HudTextV1*&, HudTextEnvironmentV1&, error);
```

Root can construct stable `ItemTextOwnerV5(actual ItemTable, actual CharacterTable, *sameText, environment)` and pass genuine services to inventory descriptors. Borrow ends on successful localization reload/destruction. Same profile address borrowed for source name callbacks; symbol temporary borrow restores previous profile. Environment Application version/title callbacks remain missing if actual source directives reach them.

ROOT CMAKE NEW dependencies: `features/character_menu/menu_stats.cpp`, and `port/engine-ui/hud_text_v1.cpp`, `hud_text_format_v1.cpp`, `localization_parse_ex_v1.cpp` unless already linked. Root/integration lead notified. Existing source contents/constants/localization/combat are foundation dependencies. Test `menu_text_tests` also needs `port/engine-ui/text_layout_v1.cpp` solely for independent actual align_line comparison.

Layout helper follows ACTUAL Android recovered text_layout_v1, not stock vendor GameSWF: initial x=max0(left+indent), initial y=textheight + font metricdelta; RECT minima are not added. Original font103/287 DefineFont3 HasLayout0, constructor descent/leading0; source default Latin get_fontfile42b38c resolves same `data/Fontin SmallCaps.ttf` irrespective bold suffix. Runtime metrics overload requires actual root scale. No guessed halfheight. Glyph offsets use field.matrix linear part + returned baseline, then independent viewport axes.

Source data needed: common_text four pydata files, gameplaymenus.symbols/english, menu.symbols/english (Title actual MENU_HELP_04_TITLE), global.english (source decimal/thousands defaults). Full original native assets contain them; windows-shared-assets initially lacked symbols. Root staging may now differ; verify actual package resources.

## Tests / current status

- `character_menu_tests.exe` PASS for source fullscreen 960x640/1920x1080/480x900 hits, actor/profile owner safety, live projection. Last compile/run was BEFORE `frame()` started calling new menu_stats; update target/link and rerun.
- Latest standalone compile of menu_text tests included new menu_stats, original solids header/art, HudTextV1 + format + ParseEx and existing archives; test PASS on `port/android-native/app/src/main/assets`. Last run precedes final TWOH symbol typo correction only.
- `art_export_tests.py` PASS after source correction: independent UV clipping; actual map clipDepth scope reached; shape90 stage coverage. Running it regenerates original_art.cpp and source_layout.json. Active menus truly have no frame0 masks; do not claim masks fix menu artifact.
- Warning-clean native Clang compiler: `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`.
- Python: `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe`.
- Full root runtime check pending for NEW solids drawing and Stats values/labels. Current inspected fullscreen screenshot has correct detail-button icons/full viewport, but blank right text/all default Fire elements because it predates latest projection/export/visibility.

## Workers / integration contracts

- `/root/inventory_feature`: ready `inventory_menu.hpp` main provider category424 exact frames/hits and source details456. Real names are Item.record.word17 plus material grammar, NOT Item.name (model URI). All1322 source IconName EMPTY; category icons are correct. `source_bare_item_descriptors` uses genuine ItemTextServicesV5 name/stats/requirements and passes actual staged-data test. Details selected instance must stay same owner.
- `/root/equipment_feature`: ready `equipment_menu.hpp` Presenter const SAME CharacterState+ItemTable+PropertyState+EquipmentAdapter, original9 slots; replaces exact /inv_anim/btn role icons/text. Explicit mutations only via adapter; no refresh per frame.
- `/root/skill_ui_feature`: ready built-in icon resolver/source3class-frame callback, append/release select/assign/train and exact source hit contours. Requires SAME actual skill authority and NativeGetSkillDetails owner. Source class frame supplied, not inferred. Root must activate real callbacks.
- `/root/quests_feature`: no same native CharacterMenuQuestsV51/PlayerSavegameV1 published by current MenuBindings. Coordinate actual root graph before quest tab; no fixture/duplicate save.
- `/root/video_fidelity`: verified ACTUAL supplied original stills:391 Stats(points2),393 Stats(points0),396 Skills HeadSplitter,399 Skills InnerStrength,390 Inventory overview,335 Torso detail,340 Hands detail,370 Feet detail. Earlier their label/timestamp claims were wrong and corrected after pixel inspection. Use these exact original pixels, not old recon. Path `.local-inputs/fidelity-video-v18/reference-NNN.png`. In391/393 plus symbols remain visible even statpoints0; lower style darkens.

## Next bounded work

1. Ensure root compiles new stats/text dependencies, interleaves exact source solids, opens/captures stats. Compare against original391/393; no completion claim based only on tests.
2. Fill exact original class label/level/top HP/MP/XP labels and active heart tab source frame/color; current root screenshot lacks class/level header and native prefixes. Use actual source AS symbols/fields, no hardcoded labels.
3. Add real source stats training button admission/mutation callback from SAME source property/class/gear owner; original white plus and deactivated timeline should be tied to actual stat points without hiding symbols.
4. Bind actual main/off item records for staff/bow/twohand visibility and elemental named icon state; root properties getter alone lacks weapon-kind facts.
5. Activate inventory/details/equipment/skill workers' same-owner providers and native hit routes in root, then runnable visual/input/mutation checks and original video comparison. Core stays root exclusive.

No further edits by old worker after this handoff.
