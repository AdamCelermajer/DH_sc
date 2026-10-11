# B110 merchant shop — ready for independent review

Owner: /root/b110_merchant. Private branch fix/b110, worktree DH_wt/fix110; tested source 975f0d66. B100 a66b7f9c supplies merchant registry/context/marker. Reviewed B102 e40538b8 merged as 1652fa40 supplies pointer transport and equipment animation banks. Root owns fresh read-only review, integration, tracker resolution and release.

This authoritative report replaces the obsolete partial handoff. The draft persistent-stock adaptation in 87fcbb43 was removed in 64c42064 after root clarified original save rules. No merchant save component remains.

## Investigation before implementation

Visual first: inspected P1 v1.0.3 sequence 585–600s (opening 587s), preserved sheet .local-inputs/claude-v20.1/fix110/vid/p1sheet.png. It shows camp pouch/green circle, MERCHANT page, orange BUY then SELL, torso/hand/feet/gloves categories, unusable red X/equipped sword glyph, equipped/selected comparison, EQUIP/UNEQUIP, paper doll, VALUE/action and TOTAL GOLD. Individual immediate sales raise gold 34→56→76→126→164. Footage does not demonstrate failed buys, buyback, refill or application reload. Android recovered source/art may differ from footage version.

Logic source: .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c. Addresses below correct transcription mistakes in the earlier handoff.

| Source | Recovered invariant |
|---|---|
| Character::Interact 0x3a4d78 (~121635–121710) | Player interactor, non-quest actor+762==0, IsMerchant/HasLoot; empty-only Merchandise loot refill; ConditionId not tested here; openMerchantMenu/AdditionnalInfosForMerchant. |
| NativeMerchantSellItem 0x44058c | MenuMerchant+196 gate, valid merchant/player/index; minimum 1 BuyMultiplier value; buyback+105; transfer ONE; skin/trophy side effects. |
| NativeMerchantBuyItem 0x4408d4 | Valid identity/index; stock SellMultiplier vs buyback BuyMultiplier; minimum 1; gold before bag-full before potion cap; clear buyback and transfer ONE. Results 1/2/3 respectively. |
| ItemInventory::TransferItemTo 0x3ff858 | Whole transfer unequips both sets; split preserves remainder. |
| ctor C2 0x3ff330 / AddLoot 0x40407c / DBG_DropAllLoots 0x404658 | Inventory+45 starts false and feeds _AddLootItems; only debug drop-all enables true. Merchant uses weighted selection, not give-all. |
| II_Item_sortbybuyback 0x3fd9f8 | Stock precedes buyback; then bare value descending/name ascending. |
| MenuMerchant::Update 0x433548 | Resets+196 before MenuBase::Update; SELL admits once per input frame. |
| Character::Serialize 0x3a65c4 / Deserialize 0x3a6210 / SG_Save 0x3bc4a8 | NPC inventory excluded; SG_Save delegates an owned player only. |
| Level::QuickSave 0x3f059c / Unload 0x3f0690 / LevelSavegame::__SaveObjects 0x462d84 | Pipeline saves registered objects via Serialize plus players; no additional merchant inventory owner found. |
| PlayerSavegame::__SaveInventory 0x46a0b4 | Bound player inventory/gold/equipment/quantity/value/identified/powers; no merchant or buyback+105. |

Authored caller: port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt. btn_Action 0xc140–0xc1cb confirms BUY; SELL branch 0xc1a7 directly calls doMerchant/PickupGold. doMerchantPotions 0xc36f–0xc451 repeats native MissingPotions times; full-price/cash and missing==0 gate 0xb668–0xb6ca disables maximum. Popup deadzone swallows input; btn_Ok 0xa46c–0xa4a6 alone hides. Equipment controls use recovered inventory natives through existing runtime owner.

SWF: merchant 251; tab 227 On 2 starts zero-alpha tween, settled 6 orange; own category placement. Avatarpane218/shape217 has actual merchant matrix/bounds(221.2959869,70.80121307)-(321.4594207,255.0338191), inserted before merchant Back. Original localization supplies labels/dialogs.

Consumer evidence before repair: first result2/3 reached without mutation but ^n strings crashed shared formatter. DefineEditText 236 flags 0x8d22; gameswf::edit_text_character::format_text 0x78efb8→append_text 0x78cb90 (909826–909892) accepts Unicode 10/13 independently of auto-wrap flags, advances baseline and clears run. Repair retains flags/wrapping and permits explicit newline. Final OK test exposed exporter substituting shape82 for actual hitzone 247 leaf 217. Shape 217 has transparent solid contour 4822×948twips and six solid_triangles; exporter now transforms those actual triangles. Test uses independently observed visible centre 235,208 instead of calculating a point from exported geometry.

Evidence/test plan committed before behavior changes in 6f402e1a: normal open/close; categories/preview; buy cancel/accept/duplicate; immediate sale/equipment; buyback; currency/bag/potion failures; equip/potions; player save/reload and source stock ownership. Minimal reusable composition: inventory presenter, runtime equipment owner, source RNG and existing SaveStore.

## Final implementation

- MerchantTable parser; source bare value/prices; weighted table/item rolls; stock/buyback sort, one-unit transfers and failure ordering. Capacity 100 bag instances, potion exemption and player property 194.
- Per-level/difficulty/actor current-session shop. Close/reopen retains stock/buyback; empty shop refills on interaction. Level change/new process rebuilds stock; same-level checkpoint retains shop. Only successful player transactions save. No persistent merchant-stock adaptation.
- Normal merchantInteract→ShopUi open, characterMenu pause/HUD gate, ordinary pointer routes, frame drawing/avatar. BUY compares player equipment and gold. SELL uses authoritative RuntimeEquipmentBinding; whole current-set sale unequips before transfer, pure model clears matching both-set metadata; inactive-set sale preserves unrelated active equipment.
- BUY/maximum confirmation, immediate SELL, affordability/max disabling, exact localized error lines/deadzone/OK, once-per-frame SELL. Escape cancel/error dismissal is PC adaptation.
- Compiled original chrome/panes/hits, existing atlas/fonts; general newline formatter repair and registered regression test.
- No shared quest-dialogue API: inherited B100 emits QuestEvent::talk_to_npc; shop declines while pending active TalkToNPC targets merchant. B093 owns quest acceptance/CharacterEvent5/scripts.

## Tests and frozen executable

Quiet_run/quiet_click hidden isolated desktop, DH_AUDIO_SILENT=1; fresh/copied scratch saves only under .local-inputs/claude-v20.1/fix110. Accepted v20 and user saves unchanged. Build: powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name fix110 -Jobs 2, exit0.

Final source 975f0d66 EXE:
C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix110/frozen-ok/dh-foundation.exe

SHA256: BCD92C3055EB6EE55A09C4CE6548839A1A4F6BF3DC0A5990BC29F5F19D40556B.

Full ctest 169/170 pass, permitted session_skill_binding failure only (Asset path escapes asset root). Log fix110/ctest-ok.log, elapsed19.72s. Merchant/newline tests pass: price/failure ordering, one-unit/stale duplicate, buyback, both-set clearing, empty refill; UI confirmation/duplicate/background/visible OK/navigation/player gold; HTML newline/plain CRLF and retained one-line stats.

Final frozen matrix receipt fix110/final-checks/result.json: seven exit0/no-timeout cases, run.log/cap.ppm per directory. Scheduled releases enter ordinary production input/UI, staged merchant proximity.

| Case | Observed |
|---|---|
| poor-final | gold 0 disabled BUY; attempted release leaves player 4/stock 81/gold 0. |
| controls | Cancel unchanged; confirmed ClothArmor03 costs 300→9700/player 5/stock 80; duplicate YES ignored. UNEQUIP/EQUIP Starter; SELL pays2→9702/player 4/stock 81; duplicate SELL in frame 160 blocked, next item preserved. Unmet Mage equip does not mutate. |
| buyback | SELL Starter pays2, exact buyback at list tail repurchased for2 and equipped; final gold 10000/player 4/equipment 4. |
| potions | Maximum fills 12 at 10/unit→gold 9880; potion stock 254→242. Further buy result 3 unchanged, both localized lines render. |
| full-bag | 100 diagnostic bag rows+4 equipment; result 2 keeps gold 10000/player 104/stock 81. Background does not dismiss. |
| modal-dismiss | Result2, swallowed background, visible OK235,208 dismisses; no additional transaction. Final cap.png inspected without popup. |
| reload | Actual frontend SinglePlayer→Continue authored controller actions choose copied physical slot; Space opens merchant, SELL contains bought Druid. Exact saved gold 9702/items 4/equipment 3 preserved; new stock 81. |

Separate physical proof on f017e4e0, before the final OK-contour-only repair: frozen-final/dh-foundation.exe SHA256 7D53B9C002F19A71BDB2C0C3D127D78398B92FC5698D719164B3E62B1B7185F1; fix110/physical/injection.txt/run.log/cap.png. Own 960×640 window, no scheduled releases/fixed step. Physical Space 30ms, pointer 10ms. Opens, UNEQUIP/EQUIP, SELL frame 653 pays2→10002, cancel unchanged, confirmed BUY frame 952 costs 300→9702, Back frame 1012 closes, Space reopens stock 81/gold 9702. Final inventory boots/gloves/sword+merchant-83-ClothArmor03; StartingSuit absent. Final frozen semantic matrix repeats transactions after the contour-only change.

Visually inspected original sequence and quiet 1200×720/960×640 captures: orange tabs, categories/row symbols, comparison/paperdoll and original VALUE/TOTAL GOLD chrome; error 2/3 show both lines. Preserved early failed receipts (give-all stock 312, missing preview anchor, newline failure and narrow OK); they are not final passes. Frontend Continue actions are injected controller actions, not physical frontend clicks/natural travel.

## Match statements and limitations

**Matches video:** merchant screen structure/navigation, active tabs, equipment comparison and immediate individual sales. Production interaction now opens/trades/closes; no longer helper-only.

**Matches IDA/authored callers:** bare prices, one unit, stock/buyback distinction, failure order/capacity, empty-only weighted refill, equipment transfer, BUY confirm/SELL immediate, maximum loop, SELL update gate, error admission/newline consumer and player-only save ownership.

**Not verified/inherited gaps:** InventoryItem lacks generated powers; bare stock cannot reproduce purple names/powered prices/stats, exact roster or power RNG consumption. Hard/VeryHard variant conversion is not implemented/tested. Original trophy counters/transaction sounds not added; silent runs do not prove audio. EXE covers Warrior/Swamp only; other classes/merchants, inactive-set-only sale, empty-all-stock production refill, active quest admission and same-level checkpoint retention source/model-audited but not staged. Exact paper doll animation/drag/version/pixel parity unproven. Natural campaign travel and physical frontend Continue untested. NPC reaction/CharacterEvent5 outside B110.

Package: B100 data/3d/interface/quest_marker_merchant.bdae plus B087 blue marker; B102 reviewed 781-model manifest (778 new package targets). Merchant art compiled, uses existing atlas/fonts. Scratch reads expanded fix100 assets; root must verify packaged completeness/integrated preview. Full powered-item content remains follow-up, not completed fidelity.
