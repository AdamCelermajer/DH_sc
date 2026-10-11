# B110 merchant shop — ready for independent review

## Review correction investigation (2026-10-11, before editing)

Reviewer reproduced two authored admission failures on the frozen candidate: SELL of equipped StartingSuit succeeded, and ordinary BUY at 12/12 potions opened a confirmation then native result3. Reinspected P1 585–600s sheet: equipped Ceremonial Garb/Useless Blade selections have the dark disabled SELL button while a different unequipped selection can sell; the footage does not show potion capacity.

Full displaySelectedItemInfos caller audit in authored-actions.txt: BUY first handles selected-row visibility and gold (0xb4c7–0xb5b6). InvSlotId9 computes NumPotions/MaxNumPotions/MissingPotions (0xb5b7–0xb651); maximum is disabled when full price exceeds cash OR MissingPotions==0 (0xb668–0xb6cb). Ordinary btn_Action is additionally disabled when MissingPotions==0 (0xb6cc–0xb6fc). SELL first enables based on selected-row presence (0xb7be–0xb834), then disables when ItemEquipped OR ItemEquippedOtherHand (0xb85c–0xb8a6). Native transfer unequip side effects do not grant UI admission.

IDA NativeInvGetItemsListForSlot 0x44bb94, ~235660–235776: ItemEquipped compares selected instance with GetEquippedItem for selected slot; ItemEquippedOtherHand compares the paired right/left weapon or ring slot. Other-set storage alone is not this field. NativeGetNumPotions 0x44996c provides held count and player's max byte. Plan: one shared ShopUi action admission for rendering and release, using selected identity, current-set selected/paired equipment bindings, player gold, category9 and potion count/capacity. Preserve native defensive result3 tests but no longer call that result the ordinary UI full-capacity behavior.

Focused tests before edit: current-slot and paired-hand/ring equipped selections render disabled and return no command/modal; inactive-set-only equipment remains sellable. UNEQUIP then SELL succeeds. Capacity BUY and maximum both render disabled and repeated clicks return no confirmation/transaction/error; after maximum fills capacity, ordinary action stays blocked. Quiet production receipts must show blocked frame before an admitted UNEQUIP→SELL and unchanged save/stock/gold at full potions.

Owner: /root/b110_merchant. Private branch fix/b110, worktree DH_wt/fix110; tested source d6567ebc. B100 a66b7f9c supplies merchant registry/context/marker. Reviewed B102 e40538b8 merged as 1652fa40 supplies pointer transport and equipment animation banks. Root owns the same reviewer's recheck, integration, tracker resolution and release.

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
- BUY/maximum confirmation, immediate SELL, shared rendering/release admission for selected/paired equipment, gold and full category9 potion capacity; max-price disabling; exact localized error lines/deadzone/OK; once-per-frame SELL. Escape cancel/error dismissal is PC adaptation.
- Compiled original chrome/panes/hits, existing atlas/fonts; general newline formatter repair and registered regression test.
- No shared quest-dialogue API: inherited B100 emits QuestEvent::talk_to_npc; shop declines while pending active TalkToNPC targets merchant. B093 owns quest acceptance/CharacterEvent5/scripts.

## Tests and frozen executable

Quiet_run/quiet_click hidden isolated desktop, DH_AUDIO_SILENT=1; fresh/copied scratch saves only under .local-inputs/claude-v20.1/fix110. Accepted v20 and user saves unchanged. Build: powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name fix110 -Jobs 2, exit0.

Final source d6567ebc EXE:
C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix110/frozen-admission/dh-foundation.exe

SHA256: 48D470E09DF9908FB3099D7D18CFD56114D8ED73AF52E299BC65FF98E6F8A68F.

Full ctest 169/170 pass, permitted session_skill_binding failure only (Asset path escapes asset root). Log fix110/ctest-admission.log, elapsed17.97s. Merchant/newline tests pass: price/failure ordering, one-unit/stale duplicate, buyback, both-set clearing, empty refill; UI confirmation/duplicate/background/visible OK/navigation/player gold; current-slot/paired-hand/paired-ring blocked sale and inactive-set-only allowed sale; UNEQUIP then SELL; full potion action disabled without confirmation/error; below-capacity BUY/maximum still admitted; HTML newline/plain CRLF and retained one-line stats.

Final frozen matrix receipt fix110/admission-checks/result.json: nine exit0/no-timeout cases, run.log/cap.ppm per directory. Scheduled releases enter ordinary production input/UI, staged merchant proximity.

| Case | Observed |
|---|---|
| poor-final | gold 0 disabled BUY; attempted release leaves player 4/stock 81/gold 0. |
| equipped-blocked | SELL tab, two action clicks; no transaction/confirmation/error. Saved gold0/items4/equipment4 and stock81 unchanged. Inspected cap.png: equipped glyph and dark disabled SELL. |
| unequip-sell | Frame100 equipped SELL blocked; UNEQUIP120 admitted; SELL140 pays2→gold2/items3/stock82; same-frame duplicate ignored. |
| controls | Cancel unchanged; confirmed ClothArmor03 costs300→9700/player5/stock80; duplicate YES ignored. UNEQUIP140/EQUIP150 Starter; equipped SELL155 blocked; UNEQUIP160 enables SELL170→gold9702/player4/stock81; duplicate SELL170 blocked, next item preserved. |
| buyback | UNEQUIP95 then SELL Starter pays2, exact buyback at list tail repurchased for2 and equipped; final gold10000/player4/equipment4. |
| potions | Maximum120 fills12 at10/unit→gold9880; stock254→242. Ordinary BUY140/170 and maximum150 are blocked; no further native transaction, confirmation or error. Saved quantity12/gold9880. Inspected cap.png: both dark disabled buttons and12/12, no modal. |
| full-bag | 100 diagnostic bag rows+4 equipment; result 2 keeps gold 10000/player 104/stock 81. Background does not dismiss. |
| modal-dismiss | Result2, swallowed background, visible OK235,208 dismisses; no additional transaction. Final cap.png inspected without popup. |
| reload | Actual frontend SinglePlayer→Continue authored controller actions choose copied physical slot; Space opens merchant, SELL contains bought Druid. Exact saved gold 9702/items 4/equipment 3 preserved; new stock 81. |

Physical proof on the same final frozen EXE: fix110/admission-checks/physical/injection.txt (exit0), run.log/cap.ppm. Own960×640 window, no scheduled releases/fixed step. Physical Space30ms, pointer10ms: SELL tab9s, equipped SELL10s produces no transaction, UNEQUIP11s (frame619), SELL12s succeeds (frame678) pays2→10002. BUY cancel15s unchanged; confirm17s (frame977) costs300→9702; Back18s closes; Space19s reopens stock81/gold9702. Final saved inventory boots/gloves/sword+merchant-83-ClothArmor03, equipment3, StartingSuit absent. Continue reload case verifies this same resulting profile state (its copied seed was the prior physical save with identical gold/items/equipment).

Superseded receipts remain preserved: frozen-ok/final-checks and frozen-final/physical allowed equipped sales or ordinary full-capacity BUY confirmation. They are not source-admission passes. Native model result3 and whole-transfer unequip tests remain valid defensive logic; normal UI prevents those calls for the reviewed cases.

Visually inspected original sequence and quiet 1200×720/960×640 captures: orange tabs, categories/row symbols, comparison/paperdoll and original VALUE/TOTAL GOLD chrome; error 2/3 show both lines. Preserved early failed receipts (give-all stock 312, missing preview anchor, newline failure and narrow OK); they are not final passes. Frontend Continue actions are injected controller actions, not physical frontend clicks/natural travel.

## Match statements and limitations

**Matches video:** merchant screen structure/navigation, active tabs, equipment comparison and immediate individual sales. Production interaction now opens/trades/closes; no longer helper-only.

**Matches IDA/authored callers:** bare prices, one unit, stock/buyback distinction, native failure order/capacity, empty-only weighted refill, equipment transfer, BUY confirm/SELL immediate, selected/paired equipped-sale admission, full-capacity ordinary BUY admission, maximum loop, SELL update gate, error admission/newline consumer and player-only save ownership.

**Not verified/inherited gaps:** InventoryItem lacks generated powers; bare stock cannot reproduce purple names/powered prices/stats, exact roster or power RNG consumption. Hard/VeryHard variant conversion is not implemented/tested. Original trophy counters/transaction sounds not added; silent runs do not prove audio. EXE covers Warrior/Swamp only; other classes/merchants, inactive-set-only sale, empty-all-stock production refill, active quest admission and same-level checkpoint retention source/model-audited but not staged. Exact paper doll animation/drag/version/pixel parity unproven. Natural campaign travel and physical frontend Continue untested. NPC reaction/CharacterEvent5 outside B110.

Package: B100 data/3d/interface/quest_marker_merchant.bdae plus B087 blue marker; B102 reviewed 781-model manifest (778 new package targets). Merchant art compiled, uses existing atlas/fonts. Scratch reads expanded fix100 assets; root must verify packaged completeness/integrated preview. Full powered-item content remains follow-up, not completed fidelity.
