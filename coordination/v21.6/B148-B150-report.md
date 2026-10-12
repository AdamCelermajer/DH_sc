# B148-B150 report (v21.6 merchant UI, branch v21.6-merchant)

Branch `v21.6-merchant` (worktree `C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\v216-merchant`), base `488d9e82`. Not pushed. Tracker not edited.

Commits:
- `2fe75f4f` B149: BUY rows and BUY details use the rarity colour.
- `fcb1999d` B150: buy confirmation is a modal layer above the page.
- this report commit (see `git log`).

Build: llvm-mingw clang++ 23.1.3, CMake 3.22.1 + Ninja, Release, configured from `P:` (subst to the worktree). Build dir `.local-inputs/v21.6-merchant/build` (gitignored, primary checkout). Frozen reference for the "before" captures: `.local-inputs/windows-source-clock-v21.5/dh-foundation.exe`.

## Status

| Bug | Investigation | Change | Unit | Runtime (hidden window) | Outcome |
|---|---|---|---|---|---|
| B148 Broken Ring stats | done (IDA + data + runtime) | none | n/a | done (powered rings show stats) | Not reproduced as a divergence; needs user confirmation (see below) |
| B149 BUY rarity colour | done (IDA + screenshots) | fixed | PASS | PASS (before/after) | fixed, runtime-verified |
| B150 confirm dialog | done (SWF evidence from B133, reference tile) | fixed | PASS | PASS (before/after, potion confirm) | fixed, runtime-verified for the potion buy path |

CTest on the final HEAD: **179/179 pass** (`.local-inputs/v21.6-merchant/ctest-final.log`). `merchant_shop_v1_tests` prints `merchant_shop_v1_tests passed` (run standalone on both the B149-only and the final state).

Runtime captures (local, gitignored): `.local-inputs/v21.4/verify/cases/<case>/final.png` with `run.log` and `exe-sha256.txt`. Harness: `.local-inputs/v21.4/verify/tools/run-case.ps1`, isolated copy of `seeds/slot1`, `--position -10900,12000,255` (swamp merchant camp), real Space/click tokens, hidden desktop.

## B148: "Broken Ring" shows no stats

### Visual evidence
- User screenshot `...-4a9645d1...png` (SELL tab, Ring 1): two "Broken Ring" rows with white names; detail panel shows name, EQUIP/UNEQUIP, VALUE 30, no stat lines and no requirement line. Equipped "Broken Ring" also has no stat lines.
- Our build, hidden window, BUY tab Ring 1 (`cases/b148-fix2-broken/final.png`): a stock "Broken Ring" whose name is blue (2 powers) shows `+ 1 to strength` and `+ 5 MP`. Stock "Rusted Ring" (green, 1 power) shows `+ 0.50 MP regeneration`; "Peasant's Ring" shows `+ 1% to spell crit.`. So powered rings do render stat lines.
- Direct observation: the blank Broken Ring in the user's sell list has a white name, i.e. no powers under the palette rule (`item_power_font_palette_row_v1`: 0 powers = row 6). Inference: that item carries no power lines.

### Logic evidence (IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `ItemInstance::_UpdateStats` 0x3fb290 (line 181626): clears the stats string, reads `Item+88` (= loot record `words[22]`, the item type). Types <= 12 only: mask `0x1780` (types 7,8,9,10,12) = ARMOR_RATING from `Item+140`; type 6 (`0x40`) = armour + BLOCK_RATING (`+144`); mask `0x3F` (types 0-5) = DAMAGE_DESC. Type 11 (rings), 13 (gold) and 14 (potion) give no text.
- `NativeInvGetItemDetails` 0x44ca5c (line 236024): `ItemStatsDesc` = `_UpdateStats` text, `ItemReqsDesc`, `ItemMagics` = NumPowers, and one `ItemPowers%dDesc` per power. A ring's only stat lines are therefore its powers.
- Port: `port/game-data/item_presentation_v5.cpp:26` `item_update_stats_v5` uses the same masks and order; `source_item_descriptors.cpp` maps power i to `ItemInfo(i+2)`. The class comparison found no omitted item class.
- Data: all 48 ring rows (`Ring01..Ring16`, `_Hard`, `_VeryHard`, type 11) have zero `BaseStat`/`BaseProp`/`Param` words (`words[23..25]`, `words[35..40]`), so jewellery carries no base stats through properties.

### Expected
- A ring shows stat lines only for its powers (IDA + data). A zero-power ring is blank in the original too, by the code above.

### Uncertainties
- No original reference frame for a ring detail was available; the expectation of stats for the zero-power Broken Ring is not supported by the original logic.
- The user's Broken Ring is in their own save. If it was created with powers and lost them through our save/inventory path, that is a different bug. The user's save was not supplied and was not inspected.
- Whether the original rolls 0 powers for this entry is a loot RNG question (NumProb row from `loot_table` entry `w3`); our stock path is the faithful LootCreationV8 roll, but the RNG streams are not comparable across builds.

### Decision
- No code change. Asked for: an original screenshot of a ring detail, or the Broken Ring item record from the user's save (powers list), to confirm.

## B149: BUY list and BUY detail names lack the rarity colour

### Visual evidence
- User screenshot `...-cb44347f...png`: BUY list names and the BUY detail name are white; the equipped "(2H) Useless Great Club" (SELL-side row) is green.
- Before (frozen v21.5, `cases/b149-before-list/final.png`, Torso BUY list): "Conscript Armor", "Druid Armor", "Thief Armor", "Imbued Armor" all white.
- After (`cases/b149-fix2-list/final.png`, same route): Conscript Armor purple, Druid and Thief green, Imbued blue; BUY detail name purple.

### Logic evidence (IDA)
- `NativeMerchantGetItemsListForSlot` 0x444168 (line 231723): for each list row it computes `ItemInstance::GetColor(item)` and writes `<font color='#%X'>name</font>` into the row's `ItemName` (lines 231944-231966). The inventory list (`NativeInvGetItemsListForSlot` 0x44bb94, line 235674) does the same.
- `ItemInstance::GetColor()` 0x3fa710 (line 181150) = FontPalette entry from `GetFontDef(GetNumPowers())`. The port's `item_power_font_palette_row_v1` implements that power-count rule.

### Root cause
- `ShopUi::frame` built the BUY pane with the plain `bindings_.item_name` (`itemDisplayName`). Only the SELL pane got the coloured callback (`sell_item_name`). The BUY list rows and the BUY detail name read the BUY bindings, so they stayed plain.

### Change (commit 2fe75f4f)
- `features/merchant/merchant_shop_ui_v1.{hpp,cpp}`: `stock_item_name(...)` callback; BUY pane frame uses it (`buyBindings`). The confirmation prompt keeps the plain name.
- `main.cpp`: `coloredItemName` helper (same descriptor + palette path as the sell callback), declared in the enclosing scope because the ShopUi callbacks outlive `openMerchantShop`; sell callback validates the player row and then calls it; stock callback calls it directly.
- Bug found and fixed during verification: a first version declared the helper inside `openMerchantShop`, and the callbacks captured it by reference, so the first BUY frame crashed (access violation, exit `0xC0000005`, no `Merchant opened` line flushed). Bisected: base code and library-only changes run clean; the dangling reference was the cause. The committed version is the fixed one.

### Tests
- `merchant_shop_v1_tests`: BUY rows and detail use `STOCK:` text, SELL rows keep `SELL:`, plain binding not used on BUY. PASS.
- Runtime: before/after BUY list pair above (unit-only for the callback routing; runtime for the visible colour).

## B150: buy confirmation dialog icons not opaque, text overlaps the list

### Visual evidence
- User screenshot `...-0c827f96...png`: "BUY (2H) USELESS GREAT CLUB FOR 360 GOLD?" over the list; list rows visible through the box and under the check/cross medallions; page not dimmed.
- Before (frozen v21.5, `cases/b150-before-potion/final.png`, potion buy confirm): page bright, "99 Potion" row and "Potion" text visible through the box, check and cross over list rows, prompt over the avatar.
- Reference: `.local-inputs/claude-v20.1/fix111/resume/ref-p2-386-397.png` tile 4 (Part 2, 397 s; cited in `coordination/v21.3/B133-report.md`): page dimmed, ornamental WarningBox, opaque-looking green check and red cross medallions, prompt legible.

### Logic evidence
- `coordination/v21.3/B133-report.md` ("Exact original dim recovered"): the original `menu_confirm2` (`dqcharmenu_droid.swf`) root sprite 785, settled frame 5, shows a full-stage black shade (`shape232` in sprite 233, RGBA `[0,0,0,191/255]`, covering 480x320). The modal is drawn above the whole page.
- Export: `features/character_menu/export_art.py` `walk` appends solid-fill shapes to `art` with textured triangles only, so `merchant_art().confirm` has no solids (block 4 is `{}`). The shade is lost.
- The equipment confirmation (`equipmentActionConfirmation`, main.cpp) already draws the dim (191/255) and the confirm art after the page. The merchant confirmation went through the normal page order (batches, then text), so page text and rows stayed bright and drawn over the box.

### Expected
- Page (batches and text) dimmed by 191/255 black, then the WarningBox art, then the prompt on top. The check and cross sit on the dimmed page.

### Root cause
- Missing modal layer for the merchant confirmation (no shade, confirm drawn in page order). The export loss of the shade solid is the reason the dim is not in `merchant_art()`.

### Change (commit fcb1999d)
- `main.cpp`: in the character-menu draw, when `merchantModal` is true, page batches with role `menu_confirm2/` are skipped; `drawMerchantModal()` draws the 191/255 dim and then the confirm batches once, before the first `menu_confirm2` text (after all page text). The confirm prompt is drawn after that. Equipment confirmation path untouched.
- Not done: regenerating `merchant_art().confirm` solids via the exporter (the exporter's shared solid handling is out of scope and the export script refuses regeneration of original_art.cpp). The dim constant is the same one the equipment path uses.

### Tests
- `merchant_shop_v1_tests`: with the buy confirmation visible, `frame.art` contains `menu_confirm2/` batches and the confirm text keeps the plain name (`PLAIN:Sword`). PASS.
- Runtime: before/after potion confirm pair (`b150-before-potion`, `b150-fix2-potion`). After: dimmed page, legible prompt, medallions read as opaque on the dim.

## Verification summary

| Item | Unit | Runtime (hidden window, isolated copy of slot1) |
|---|---|---|
| B148 | none (no change) | powered ring stats shown; zero-power ring blank in both builds (IDA logic) |
| B149 | merchant_shop_v1_tests (PASS) | before/after BUY list captures |
| B150 | merchant_shop_v1_tests (PASS) | before/after potion buy confirm captures |
| Full suite | ctest 179/179 on final HEAD | - |

Runtime was run as direct swamp launch with `--position` (test placement, same as B110/B134 runs), real Space and click tokens, window 1200x720 (Buy action near 680,690 in that window).

## Remaining gaps (honest)
- B148: no original reference; the user's Broken Ring may or may not be a zero-power roll in the original. Needs the user's screenshot of the original or the item record. No change made.
- B149: BUY colour follows the same `GetColor` rule as the SELL and inventory rows, verified from IDA; the colours themselves were compared against the user's SELL/equipped colours, not pixel-checked.
- B150: runtime-verified only for the potion buy confirmation. Non-potion BUY and SELL confirmation use the same code path but were not captured. The Yes/No hit contours were not re-tested. The medallion translucency was judged visually against the reference tile, not pixel-compared. The shade constant (191/255) comes from the B133 SWF analysis and is reused, not exported.
- Power lines: the detail art has four power slots (`ItemInfo2..5`); the original writes one line per power. Items with more than four powers would lose lines here. Not observed in these captures; not changed.
- Not run: the user's own save and inventory (not supplied).
