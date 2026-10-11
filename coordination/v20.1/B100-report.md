# B100 report: cannot talk to the merchant, no icon above his head (shot-18)

Branch fix/b100 (from v20.1/integrate), build `DH_wt/build-fix100` exit 0, ctest 167/168 (only `session_skill_binding` fails, allowed).

## Evidence
### Video (P1, z_Zky7qQdYs), observed
- t~585 s (`.local-inputs/claude-v20.1/fix100/keep/video-t585.png`): the shirtless Bogwalker merchant at the merchant camp has a gold money-bag icon floating above his head, a green target ring under him, and the Space/action button shows the speech bubble.
- t~587 s onward (`keep/video-t587-shop.png`, frames t=587-596): the "MERCHANT" menu (BUY / SELL tabs, slot category header, item list with red X for unusable, detail pane with the hero paper doll, VALUE, TOTAL GOLD, back arrow) covers the screen.
- Shot-18 (user): the same merchant stands behind the hero, no icon, no ring-driven interaction.
### IDA (pseudocode-all.c), observed
- `Character::IsMerchant` 0x3a30c4 = GetCharType()==7 (AI row type 7).
- Marker: `Character::Update`, line ~126689: `IsMerchant && Character+138` (enabled; raw SetEnable0x33ddc8 confirmed in B088 re-review; visible is +128 via SetVisible0x33dcf8): if `+762` (registered quest NPC) -> drop the FX in `+5252`; else grab `VisualFXManager::GrabAnimFX(139, this)`, loop its animator. Effect 139 = `quest_marker_merchant` (effects list order 137 quest_marker, 138 quest_marker_blue, 139 quest_marker_merchant; asset `data/3D/interface/quest_marker_merchant.bdae`). Full IsInteractive/dead/disabled/quest-suppression branches remain qualified fidelity gaps rather than verified production claims.
- Candidate: `TargetList::_IsCharacterValid` 0x4a1ab8 mask bit 0x10 = IsMerchant; the OOI mask is 0x59, so a merchant is an object-of-interest candidate (GetInteractionType = 3 for a friendly NPC).
- Interact: `Character::Interact` 0x3a4d78 (line ~121600): raises the TalkToNPC quest event, sets interact state, and for a merchant with loot (and `+762==0`) fills the merchant ItemInventory from `MerchantTable[GetLoot()]` and calls the Flash callbacks `openMerchantMenu` and `AdditionnalInfosForMerchant`. Menu `menu_Merchant` (MenuMerchant, Natives `NativeMerchantGetItemsListForSlot / BuyItem / SellItem`, sell price from MerchantTable).

## Root cause
The merchant fact existed (map marker code, AI type 7) but the merchant was neither registered as a context-button candidate (the B088 NPC loop only knows TalkToNPC quest NPCs) nor given the marker, and Space had no merchant branch.

## Change (main.cpp, small hunks marked B100)
- `merchantActors` set built where the map facts are built (AI type 7 per placed character).
- Merchant OOI candidate (type 3, always eligible while enabled/alive).
- Marker: `quest_marker_merchant.bdae` drawn over each merchant unless a TalkToNPC objective of it is registered (+762 rule), same draw path as the B087 "!"; marker array extended to 3.
- Space on the merchant: `merchantInteract` raises the TalkToNPC event for its CharacterTable row (as `Character::Interact`) and logs `Context merchant interact ... (shop UI not ported)`.

## Verification (real EXE, quiet, silent, isolated save; scratch `.local-inputs/claude-v20.1/fix100/`)
Swamp merchant camp, hero 250 units from the merchant (row 294), Space at frame 60: `Merchant marker drawn`, `Action icon icon=3 type=3` (speech bubble), `Context merchant interact id=... row=294`. Frame `keep/after-merchant.png`: money bag above the merchant, green ring, speech-bubble button; matches the video frame t~585.

## Match statement
Video: icon, ring and speech-bubble button match (observed side by side). IDA: candidate/marker/interact rules implemented as read; interaction event matches. Shop UI: NOT matching, see below.

## Shop UI: what exists / what does not (precise)
- Does NOT exist in the Windows port: no `MenuMerchant`, no BUY/SELL pages, no merchant inventory (`MerchantTable` rows are only partly borrowed in `features/interactions/npc_interact.*` as a source-shape adapter, unused by main.cpp), no buy/sell price logic. `features/character_menu` has only Tab {stats,equipment,skills,faery,quest,map}. The original menu is a Flash movie `menu_Merchant` (present in `port/level-world/reference/ui-layout/swf-inventory.json`, with `menu_MerchantBuy/Sell` labels) that reuses the equipment page layout plus the paper doll.
- So Space currently raises the event and logs only: the user still sees nothing open. This needs its own session (menu page, merchant stock from MerchantTable + `ItemInventory::AddLoot`, buy/sell with gold, price formulas from `NativeMerchantBuyItem 0x4408d4 / SellItem 0x44058c`).

## Not verified
- Marker animation (static mesh, as B087); marker hiding in cutscenes/dialogue; hiding while the merchant is a registered quest NPC (rule coded, no such merchant staged); other merchants (only 1 in the Swamp); Character event 5 / interact state (NPC reaction) not ported.
- No new unit test: the candidate path is the already tested type-3 character registration; the merchant specifics live in main.cpp.

## Package files required
`data/3d/interface/quest_marker_merchant.bdae` (21,972 bytes) from `.local-inputs/assets-extra/android/data/3d/interface/` (also `quest_marker_blue.bdae` from B087). Without it the merchant has no icon (logged "unavailable").
