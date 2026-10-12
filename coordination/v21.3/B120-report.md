# B120 — missing merchant buy sound

Status: READY — integrated source-faithful BUY caller/output verified on frozen 4C60F3D5; owner and independent reviewer PASS. Root release acceptance remains root-owned. Reviewer: `batch_audio_review`.

## Investigation before implementation

### Visual evidence

Inspected the supplied current-game screenshot `.local-inputs/v21.3/user-shots/codex-clipboard-802efbb4-fc30-4b30-b28c-b370f3c7242f.png`: selected (2H) Useless Cleaver costs 240 while Total Gold is 202; the BUY button is dark and no confirmation or completed transfer is visible. This is a refused/disabled case, not success evidence. Reused the original v1.0.3 Part 1 merchant sequence already inspected for B110: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`, contact sheet `.local-inputs/claude-v20.1/fix110/vid/p1sheet.png`, sampled over 585–600 s (merchant opens about 587 s; B110 report records the observed interval). I inspected the sheet: its BUY view shows selected Mystic Armor value 540 with only 34 total gold, so this is visibly a disabled/insufficient-funds case, not evidence of a successful purchase. The subsequent sequence is SELL: selected armor and equipment rows, gold rises 34→56→76→126→164 across individual item sales. Those frames show merchant UI context and currency changes but do not establish a buy animation or sound. B120's supplied 202 gold / 240 cost screenshot is likewise refusal evidence only. No continuous audio listening/matched waveform was done; no success-specific visual/audio onset is claimed.

### Original logic and caller gates

- `MenuMerchant` sprite 251 source bytecode is in `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`. Its `btn_Action.onRelease` handler at 0xC0F8–0xC1CB: BUY mode opens `menu_confirm2`, calls `NativePlaySoundFX("MenuConfirm")`, and formats the confirmation. SELL mode dispatches `doMerchant` directly at 0xC1A7–0xC1CB and then plays `PickupGold`; BUY mode opens the confirmation path described below.
- The confirmation Yes handler is sprite 785, 0x3467A–0x346B2: for `WarningType == Merchant`, it calls `menu_Merchant.doMerchant()` and then unconditionally calls `NativePlaySoundFX("PickupGold")`. The same sequence therefore requests this named cue after an accepted confirmation, including when the subsequent native result opens an error popup. Cancel/No does not enter this branch and does not request PickupGold. This is the actual source ordering; no success-only condition surrounds the cue.
- `doMerchant` at 0xC1D3–0xC2F0 reads `merchantMode`; BUY calls `NativeMerchantBuyItem(merchant id, name, item index, player)` at 0xC1F2–0xC222. A nonzero NativeResult displays `MerchantErrorPopup`; it does not guard the later caller cue. Sell calls its separate native and refreshes gold.
- IDA `NativeMerchantBuyItem` at `0x4408d4` (`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0044/004408d4.c`) gates valid player/merchant/item identity/index first; calculates stock vs buyback price using the merchant table and minimum 1; returns 1 for price>gold, then 2 for full non-potion inventory, then 3 for potion-capacity full. Only admitted transactions debit gold, clear buyback flag, transfer one item and increment spend stat. There is no item-class admission check in this native. Class restrictions affect later equip admission, not purchase. The UI caller `displaySelectedItemInfos` at 0xB4C7–0xB6FC disables the action for insufficient cash; potion maximum is disabled on full price/cash or no remaining potion capacity. The ShopUi implementation preserves selected-row/gold/potion UI gates and source result ordering; its confirmation token is cleared on first Yes/No release, preventing duplicate accept dispatch.
- Original sound chain is `NativePlaySoundFX` `0x43ae10` → `VoxSoundManager::PlayMenu` `0x36bc34` → plain `Play` `0x36b80c`. B122's shared mapping confirms the port consumer is `RuntimeSessionAudioV1::play_named_ui_sound("PickupGold", error)`, which resolves the generated source ordinal and submits through the current mixer/output session.

### Cue identity and asset

The BUY button's separate confirmation cue is source ordinal **141**, `MenuConfirm` → XML UID **2** → `sfx_menu_confirm.wav` (priority 2, non-looping, group 4, bank 6). The post-confirm transaction cue is generated `Arrays::Sounds` source ordinal **155**, exact name `PickupGold`, maps to XML UID **155**, exact PCM asset `data/sounds/sfx_pickup_gold.wav`; row metadata is priority 1, non-looping, group 7, bank 3 (`port/engine-audio/reference/source-bindings-v38/ledger.json`, source binding notes). Do not use the pickup-world-item 3D route or substitute another sound. The original cache ledger marks the exact file absent, but the user-confirmed v21.1 runtime audio root (`.local-inputs/windows-source-clock-v21.1/audio-assets`) contains the exact file; v21.2 and `.local-inputs/assets-extra/ios` also contain the same bytes, SHA256 `1F6B70C1EE674BF894E50A736327E946D79307AA1D3F3E7AFF2A72707A60A7A4` (66,936 bytes); the exact file is present in the selected root. Missing-asset behavior is a source-compatible silent no-op, not permission to fabricate a cue.

## Expected behavior and minimal patch proposal

The transaction cue `PickupGold` is in the confirmation-Yes caller after `doMerchant`, not in the NativeMerchantBuyItem transaction model; the earlier BUY button release separately requests `MenuConfirm` after opening the popup. The Buy Maximum button also opens this popup and requests `MenuConfirm`. Root should bind the existing named UI cue facade to both authored points: `MenuConfirm` once when an affordable BUY action opens the confirmation, and `PickupGold` once for accepted Yes, and preserve the source ordering even if the defensive native result returns insufficient gold/inventory/potion capacity. Cancel, unavailable selection/no confirmation, and a duplicated/stale Yes must not produce a transaction or cue. Class incompatibility does not reject purchase in NativeMerchantBuyItem; preserve purchase then let the authored equip rule reject equipping if relevant. SELL is a separate direct `btn_Action` release at sprite 251, 0xC1A7–0xC1CB: it calls doMerchant and then PickupGold without a confirmation. B119 owns that direct seller path. B120 proposes wiring only the BUY confirmation path.

Production owner is root/main: `ShopUi::release` returns a one-shot `Command::buy` or `Command::buy_potions` only on accepted confirmation; the production consumer is `port/windows-foundation/main.cpp:5916–5947`, which executes `Shop::buy`, updates errors/save/refresh/gold. Root can call `runtimeAudio->play_named_ui_sound("PickupGold", ...)` once after executing either accepted buy command, including once after the full `buy_potions` loop; preserve the original follow-up even if a defensive native failure result is returned. The prior modal-opening `MenuConfirm` is a separate one-shot signal detectable by `confirmation_visible()` changing false→true around `ShopUi::release`. Keep the cue call adjacent to the existing original call ordering, not inside shared Shop data logic. Root owns `main`/CMake and `RuntimeSessionAudioV1`; this report proposes the API/call site and does not edit those files.

## Focused verification defined before wiring

On an isolated own save/window with a frozen executable and explicit audio root:

1. Affordable non-potion BUY: first click opens confirmation and requests one MenuConfirm source ordinal141 / UID2; Cancel leaves gold/stock/inventory unchanged and adds no PickupGold token; Yes transfers exactly one, debits exact price, and submits one source ordinal155 / UID155 named cue through the same host/mixer. Inspect before/after UI frames and save state.
2. Successful potion purchase / maximum if supported by this caller: verify one cue per accepted confirmation, and no repeated cue on repeated Yes.
3. Insufficient-gold refusal: normal UI action is disabled, so ordinary click cannot open confirmation or request MenuConfirm/PickupGold. A defensive stale accepted-confirmation callback shows native result1/error and, by original ActionScript ordering, still requests the single PickupGold cue. Full bag and full potion capacity should preserve the exact result2/result3/error paths and caller cue ordering if an already-open confirmation is accepted after state changes.
4. Cancel and duplicate/stale confirmation: no mutation and no extra PickupGold; opening the modal emits MenuConfirm once; class-locked item remains buyable but fails equip gate after purchase.
5. Exact sample case: the user-confirmed v21.1 root contains `PickupGold` at the hash above; assert source ordinal, UID and started mixer voice. A zeroed/silent-device run proves routing/voice lifecycle, not audibility. A normal output/listener or human ear check is still required to claim audible sound.
6. Verify with video frames around confirmation/results and actual runtime log/voice receipts against the frozen EXE SHA; don't use the supplied 202/240 shot as success proof.

## Remaining uncertainties

- Part 1 v1.0.3 footage inspected for B110 has no confirmed successful BUY sequence or sound identity from a matched isolated cue; this report's exact identity comes from authored source bytecode and the original generated sound mapping, not from a guessed acoustic resemblance.
- No audio-root uncertainty remains: the selected user-confirmed v21.1 root contains the exact file with matching hash.
- Actual nonzero device output/audibility and a decoded original mixed-video waveform comparison have not been tested.

## Implementation/runtime status

Not implemented by this worker: shared root-owned caller binding and integrated frozen executable proof remain pending. B122 is investigating shared event routing and is coordinating the exact facade. No claim of resolution from this audit alone.








## Resumed implementation ownership and integrated production proof — 2026-10-12

Owner `/root/b120_buy_sound_retry` resumed B120 by root instruction; reviewer remains `/root/batch_audio_review`. The earlier evidence-only implementation status above is superseded. Shared caller patch was applied by integration owner to frozen source `f9ed76a9`; candidate `.local-inputs/v21.3/integration/f9ed76a9/dh-foundation.exe`, independently rehashed SHA256 `8DED82833D817693698988432D2570D23CDCA0D54744E845CCDF167FE2A8C34A`. Existing original footage/source evidence above was reused; no unknown successful original BUY footage/audio is invented.

### Implementation

The integrated caller observes confirmation false→true around actual ShopUi release and requests MenuConfirm141 / XML UID2 once. Accepted `buy`/`buy_potions` executes native-model transaction, then requests PickupGold155 / UID155 once, including native refusal; the max-potion cue is outside its repeated purchase loop. Modal pending token clears on first Yes/No. All shared source/build work belongs to integration lead; this owner supplied existing scoped proposal and completed isolated executable verification, with no root behavior edits.

### Actual frozen executable branch results

All final cases were real hidden own executable windows, explicit accepted v21.1 visual/audio roots, `DH_AUDIO_SILENT=1`, own saves and paths. `.local-inputs/v21.3/B120/retry/{jobs,profile-jobs}.json` preserve exact launch args. Final summaries all exit0.

- `success`: ordinary BUY100 opens confirmation, MenuConfirm141/UID2 token2 starts; Yes140 buys `shop-48-ClothArmor03` (display Druid Armor, red X / Req17ENG with Knight player) for300; gold10000→9700, runtime inventory4→5, stock81→80. PickupGold155/UID155 token3 starts and finishes. Repeated Yes140 and150 produce no additional transaction or cue. Parsed saved inventory contains exactly one `merchant-82-ClothArmor03`; gold9700. This verifies purchase admission independent of later equip eligibility; no equip permission claim.
- `cancel`: BUY100 token2 starts, No140 closes modal; repeated stale Yes140/150 does nothing. No PickupGold or merchant transaction, saved gold10000 and seed inventory unchanged.
- `disabled`: zero-gold ordinary BUY100/Yes140/150 has no modal cue, no transaction cue and no transaction; saved gold0 unchanged.
- `potion-max`: slot9 selected90; BuyMaximum110 gives MenuConfirm token2. Yes150 purchases12 Potion0 for10 each, gold10000→9880, persisted one potion instance quantity12, stock81; PickupGold token3 exactly once for entire loop and duplicateYes. Viewed f160 showing YourPotions12/12 and dark BUY.
- `bag-full-profile`: retained own profile exactly100 runtime inventory entries; BUY100 opens confirmation/token2, Yes140 returns native result2 InventoryFull, gold10000→10000, stock81 unchanged, changed0. Caller still requests PickupGold155/UID155 token3 with matching started receipt. Viewed f150 InventoryFull popup and unchanged currency.
- `potion-full-profile`: retained12 Potion0 at capacity12; ordinary slot9 BUY100 is disabled. No merchant cue or mutation; viewed f150 12/12 dark BUY, gold10000.
- `purchase-reopen-profile`: separate process loads copied successful saved inventory, opens normal merchant with gold9700. Saved ClothArmor03 remains. This is retained-profile purchase persistence proof, not full Continue/world-save coverage.

Visual sequence inspected: success f80 (DruidArmor300, gold10000, requirements/redX), f110 confirmation, f150 result/gold9700; potion-max f16012/12; bag-full-profile f150 error; potion-full-profile f150 disabled. Captures are adjacent PPM/PNG files. Current source content/runtime is established by these UI sequences; original success sound identity is established by authored source, not a guessed waveform match.

### Fixture boundaries and rejected attempts

`--fresh-player` intentionally clears loaded gear/inventory at main1314 and reconstructs equipped items; it preserves gold. Thus initial `bag-full` / `potion-full` cases did not test refusal despite hand-edited seed inventory: explicitly excluded. Initial80-extra retained bag was84 items and below source port capacity100: excluded as refusal. Final retained-profile seed uses96 extras +3 seed items + starter suit binding =100.

`profile_cases.py` uses a private `dh2.transition.entry.v1` component with test entry9999 and removes fresh-player. Normal existing transition loading retains the profile; no matching entry keeps supplied merchant-proximity position. This is explicit test setup, not original gameplay content. Own gsave-checksum attempts with frame0/deferred40 restore failed `Source camera target: Missing current camera-target anchor provider`; no refusal/persistence proof is credited to those failed restored cases. Integration lead was informed. No user save/window/package altered.

### Review/output completion boundary

Reviewer independently repeated success/cancel/disabled on f9ed and reported PASS. Final max-potion/full-bag/potion-full/reopen fixtures supplied for independent review. All named tokens→matching mixer voice lifecycle checks pass; silent WinMM delivery does not establish nonzero device PCM or heard audio. Await shared B122 output-stats freeze for interval evidence before B120 READY. Defensive stale-gold/result1 and stale-potion/result3 after an already-open modal require state-change injection; not forced through an invented UI rule. Actual admitted result2 refusal covers unconditional caller continuation. Source version/footage gaps above remain explicit.

Additional focused integrated branches: `single-potion` ordinary BUY100 / Yes140 debits10 (10000→9990), transfers exactly one Potion0, emits MenuConfirmtoken2 and PickupGoldtoken3 with matching started receipts; duplicateYes emits nothing. `audio-disabled` same ordinary armor purchase with `--audio` absent still succeeds gold10000→9700 and item transfer; playback availability does not gate gameplay. Both frozen f9ed hidden isolated runs exit0, exact args in `additional-jobs.json`. Reviewer final independent profile replays PASS: exact100-item result2/refused-caller voice, full potion disabled no cue, retained successful purchase gold9700. Shared output metric freeze still pending.

## Final PCM-enabled freeze verification

Candidate `.local-inputs/v21.3/integration/frozen-4c60f3d5.exe`, SHA256 `4C60F3D538C6F26C81E7FBB27ACBB6AE4EBFC191A37E9C817B1585D9C41ADEAD`, independently rehashed. Shared PCM patch now integrated. Own quiet tests set `DH_AUDIO_PCM_METRICS=1`, silent=1; exact jobs/logs under `retry/pcm-4c60-*`, all exit0.

Controlled exact-asset root `retry/thin-audio` contains unchanged authentic MenuConfirm/PickupGold samples, original sounds.xml/table and original settings pyarray+names. Other samples deliberately absent, so no quest/music voice competes. This is an isolated output attribution fixture alongside prior accepted-full-root production coverage. Initial thin-baseline lacked required settings and failed audio initialization; excluded. Corrected thin-baseline-v2 validates both exact cue lifecycles.

- Cancel: only MenuConfirm141/UID2 token1 starts/finishes; mixNonzero185313, peak0.546387. No trade. Disabled: no voices, mixNonzero0/peak0. Both healthy writes, no failure.
- Sequential success: BUY100 token1 finishes at outputFrame183137 before Yes260. PickupGold155/UID155 token2 starts218624 and finishes268793, gold10000→9700. mixNonzero284281 = cancel185313 +98968 distinct extra mixed samples from the completed PickupGold cue. No other sample/voice is admitted. Healthy output writes677, writeFailed0, submittedFrames346624, deviceFrames343044. Silent gate yields submittedNonzero0/peak0 exactly as required. This verifies buy cue reaches nonzero real mixer PCM and progressing WinMM delivery, while explicitly excluding hearing/nonzero device samples in this silent run.
- Normal overlapping success also passed exact tokens, mutation and writes; do not attribute aggregate peak/count to one overlapping voice.
- Newest full-root reruns `final-4c60-bag-full-profile` and `final-4c60-potion-max` exit0: exact100-item result2/refusal unchanged gold10000/stock81 but PickupGoldtoken3; maxPotion12 total120 gold10000→9880 with one transaction token3. Full-root mixer totals contain other audio and are not per-cue attribution.

Shared output owner reports separate sole-PickupGold normal non-silent test on this exact freeze: positive post-quantization submitted samples/peak, healthy writes/device progress. That owner evidence is not an ear check by B120. Reviewer final source/normal branches PASS was received; PCM numeric review requested. READY recommendation is scoped to the integrated original BUY caller repair and verified original sample/output chain, pending reviewer confirmation/root acceptance. No original successful-BUY acoustic matching or human audible listening claimed; stale result1/result3 injection remains source-only, with actual result2 refusal proving caller independence.



### Independent final acceptance

`batch_audio_review` confirmed final numeric review and recommends B120 READY for source-faithful route/output. Reviewer independently tested sole PickupGold on exact4C60F3D5: ordinal155/UID155 token1 started, only voice, silent0, mixed39902/peak0.218048, submitted39802/int16peak7144, writes287 failed0, device143357 frames. Reviewer confirmed B120 sequential185313+98968=284281 is consistent and silent submitted0 expected, and independently passed eight final full-root branches including native refused bag and max loop. **B120 READY** for root acceptance. This includes real nonzero device-bound PCM evidence; no human ear check or original successful-BUY acoustic parity claim is added.
