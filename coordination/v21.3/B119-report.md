# B119 — selling-item sound

Status: original cue and admission order established; root-owned call-site integration and frozen-runtime verification are pending. Reviewer: `batch_audio_review`. The worker did not edit shared production sources.

## Investigation before implementation

### Original visual evidence

I inspected the supplied original gameplay video, Dungeon Hunter 2 v1.0.3 Part 1, at 585, 587, 590, 594, and 600 seconds. It is at `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`; extracted frames are under `.local-inputs/v21.3/B119/original/`. At 585s the hero stands by the merchant before opening the menu. At 587s the SELL tab is open with an item selected and total gold 34. At 590s the selected row and sale value remain visible. At 594s the Right Hand category is selected and gold is 56. By 600s the Feet category is selected and gold is 126. This shows the merchant screen and successive sale currency changes, not the sound itself. The MP4 has a video stream only and no audio stream, so it cannot establish original audibility, cue onset, or refusal acoustics. The original SWF source establishes cue identity below.

### Recovered source and admission

The original ActionScript is `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`:

- `MenuMerchant` / `btn_Action.onRelease` at offsets `0xC0F8–0xC1CB` branches on `merchantMode`. SELL directly calls `menu_Merchant.doMerchant()` at `0xC1A7`, then calls `NativePlaySoundFX("PickupGold", 1)` at `0xC1BF`. There is no confirmation step and no result check before the cue.
- `doMerchant` at `0xC1D3–0xC2F0` calls `NativeMerchantSellItem` at `0xC2B5`; it ignores the returned value, destroys the selected button, and refreshes the displayed player gold. The caller's PickupGold request follows when `doMerchant` returns.
- The UI disables SELL for an empty selection and selected items equipped in the active slot or paired hand/ring slot (`displaySelectedItemInfos`, offsets `0xB7BE–0xB8A6`; see the corrected caller audit in `coordination/v20.1/B110-report.md`). Thus a disabled/no-selection click does not enter the sale handler. This is the ordinary refusal path; no separate refusal sound is authored.
- IDA `NativeMerchantSellItem` at `0x44058c` first admits only an existing `menu_Merchant` whose per-frame `+196` guard is clear, a valid player and merchant identity, and an in-range inventory index. On success it computes the merchant BuyMultiplier price with minimum 1, adds gold, marks the item sold-by-player at byte `+105`, transfers one item into merchant inventory, updates player skin, and increments stat 220 (with the >99 trophy check). Gate failures silently skip those side effects. The native returns `false` in all cases; `doMerchant` does not branch on it.
- The per-frame `+196` guard limits duplicate native mutations, but ActionScript still reaches the following PickupGold call after `doMerchant`. Therefore a duplicate native no-op or another native-level refusal after an admitted `btn_Action` callback requests the same PickupGold cue. No distinct refused-sale cue was found. The port should keep action admission separate from transaction outcome: an admitted SELL action follows the original cue ordering even if the model/native gate makes no mutation; a disabled button or absent command does not.
- Original sound routing is `NativePlaySoundFX` `0x43ae10` → `VoxSoundManager::PlayMenu` `0x36bc34` → plain `Play` `0x36b80c`.

### Cue identity and source asset

`port/engine-audio/reference/source-bindings-v38/ledger.json` maps `PickupGold` source ordinal 155 to XML UID 155, bank 3, group 7, priority 1, non-looping, exact PCM file `data/sounds/sfx_pickup_gold.wav`. The generated ledger's old `exists:false` reflects its sparse cache. The selected accepted v21.1 audio root contains the exact file at `.local-inputs/windows-source-clock-v21.1/audio-assets/data/sounds/sfx_pickup_gold.wav`; it is 66,936 bytes with SHA256 `1F6B70C1EE674BF894E50A736327E946D79307AA1D3F3E7AFF2A72707A60A7A4`. The v21, v21.2, and extracted iOS copies have the same hash. Do not substitute a 3D world-pickup path or another cue.

### Current production path and integration proposal

The current production SELL consumer is `port/windows-foundation/main.cpp:5911–5947`. `ShopUi::release` produces a sell command for an admitted click; the caller enforces its once-per-frame transaction guard, invokes `Shop::sell`, saves and refreshes on success, and logs the result. It currently submits no named sale sound. Root owns `main.cpp` and integration.

Use `RuntimeSessionAudioV1::play_named_ui_sound("PickupGold", error)` (`features/audio/runtime_session_audio_v1.hpp:150`), which returns `NamedUiCueResultV1` with exact ordinal, UID, output token, and typed status. Place the call after the admitted SELL action has run through the transaction path, but do not condition it on `TradeResult::ok`: the original ActionScript calls it after `doMerchant` regardless of the native's silent result. The current main guard short-circuits a same-frame duplicate before calling `Shop::sell`; to preserve the authored order, that duplicate branch should still submit PickupGold before continuing, without mutating or saving another item. Do not call it for disabled/no-selection clicks or when `ShopUi::release` yields no SELL command. Log name, ordinal, UID, token, status and frame so the user can distinguish a submitted voice from an absent asset or unavailable output. Do not add the cue inside shared `Shop` model code.

## Focused verification defined before integration

On a fresh isolated v21.1 save copy and the hidden test window, using the frozen executable and the accepted v21.1 audio root:

1. Select an ordinary sellable item and click SELL. Verify one item transfers to merchant stock, gold changes by exact sale price, and the same frame records one `PickupGold` request (ordinal 155, UID 155) with a nonzero token.
2. Click a disabled equipped item and click with no sellable selection. Verify no SELL command, no transaction, no PickupGold token, and the corresponding disabled/no-selection UI frame.
3. Submit a SELL release, then two more admitted SELL releases in that frame. Verify the source-equivalent once-per-frame gate allows only the first mutation while all three admitted callbacks follow the authored PickupGold order; expect three cues and one sale. The prepared duplicate replay is `.local-inputs/v21.3/B119/after/sell-duplicate/a.args` (it uses an extra bag copy so the next selection remains available). A disabled/stale selection that never passes UI command admission should produce neither action nor cue; an admitted command whose transaction is rejected by a defensive native/model gate follows the original post-call cue path.
4. Confirm the runtime's `Audio voice ... token=<token>` started receipt consumes the same token and output frames advance in the same session. Use the normal output path, not `DH_AUDIO_SILENT=1`; a silent run only proves routing and mixer lifecycle, not audible device output. Record the frozen EXE SHA and retain the log/capture.

## Remaining gaps

No separate refused SELL cue or error-prompt sound is present in the recovered sale callback. The original video copy has no audio stream; direct listening or matched waveform confirmation is unavailable from that reference, so cue identity rests on the authored ActionScript and generated source binding. The current source/frozen EXE has not yet been tested after root integration. Runtime proof must use the selected v21.1/v21.2 asset root, since the v20.1 sparse tree omits the authentic WAV.

## Implementation and runtime status

No behavior source was changed by this worker. I reproduced the current defect on the frozen B118 executable `3D61A74EC3451011DADB70219CF97D83F11FE80AF5B783CF2C5897566433F306` at `.local-inputs/b118/root/dh-foundation.exe`, using the accepted v21.1 assets/audio root and the B110 staged-proximity menu-release fixture. The successful isolated run is retained at `.local-inputs/v21.3/B119/before/`. It opens the merchant through staged map interaction and records `Merchant transaction frame=140 kind=3 instance=equipped/StartingSuit result=0 price=2 gold=0->2 ... changed=1`, but no `PickupGold` request; audio summary is `dispatched=0 startedVoices=2`. The final capture `.local-inputs/v21.3/B119/before/cap.png` shows the SELL page with gold 2 after the sale.

Two additional hidden baseline jobs are retained under `.local-inputs/v21.3/B119/after/`. The equipped-selection fixture logs no merchant transaction and still has `dispatched=0`. The duplicate fixture logs one successful frame-140 sale followed by two `Merchant duplicate sell blocked frame=140` entries; no second item or gold mutation occurs, and `Audio final dispatched=0 startedVoices=2`. Together these prove the ordinary production click path, disabled-selection refusal, and reachable same-frame duplicate guard. All baseline runs used the brief's silent hidden desktop; the absent named-event submission is observable in the logs, while acoustic output is reserved for the post-integration normal-output test.

Root owns the shared caller integration/build. Actual post-fix request, duplicate/refusal branches, exact token-to-mixer voice receipt, and normal output-path verification remain open until root supplies the integrated frozen executable.

## Post-integration frozen runtime verification

Ran the three pre-staged cases against integrated candidate `.local-inputs/v21.3/integration/f9ed76a9/dh-foundation.exe` (SHA256 `8DED82833D817693698988432D2570D23CDCA0D54744E845CCDF167FE2A8C34A`). The nearby `BUILD.md` identifies commit `f9ed76a9` including the state-trace/audio integration. Each case used its own hidden desktop process and fresh output save paths under `.local-inputs/v21.3/B119/post-f9ed76a9/`; `DH_AUDIO_SILENT` was unset (`quiet_run.ps1 -AllowSound`). The accepted v21.1 assets and audio corpus were used.

- Normal staged SELL completed: one `StartingSuit` sold for 2 gold (`gold=0->2`, player items 4→3, stock 81→82). It logged `Merchant sound name=PickupGold ordinal=155 uid=155 token=3 status=submitted frame=140`; the exact token then logged `Audio voice kind=0 token=3 outputFrame=128512`. The final screenshot in `normal-sell/cap.png` shows the SELL panel and total gold 2.
- Equipped-item refusal produced no `Merchant transaction` or `Merchant sound` line and no PickupGold token. The hidden session still advanced its WinMM/device clock; final audio summary had two unrelated voices from the fixture's entry/menu cues.
- Duplicate case: one transaction mutation was followed by two `Merchant duplicate sell blocked frame=140` entries. There were exactly three PickupGold submissions (tokens 3, 4, 5; ordinal/UID 155) for the three admitted callback attempts, and matching `Audio voice` receipts for all three at output frame 123904. Only the first attempt mutated inventory/gold.

All three processes exited 0. Their output logs end with healthy WinMM refill/device-clock activity (normal: 345 refills, 0 underruns; equipped refusal: 342, 0; duplicate: 336, 0), and no hidden-run silent-output environment variable was set. Thus this verifies the actual WinMM route accepted non-silent output-path buffers and consumed the same cue tokens, alongside source-faithful success/refusal/duplicate semantics. The candidate does not expose mixer/submitted PCM metrics or a loopback recording, so these checks do not establish that a person at the machine acoustically heard the cue; B122's offline exact-UID render establishes that PickupGold itself produces nonzero mixer PCM. Acoustic endpoint audibility remains a narrower limitation.

The fixture's frame-120 action is an unrelated merchant setup/refresh (`kind=5`, zero price); its frame-140 SELL is the actual tested sale. Full logs, copied `.args`, captures, and exit summaries are retained under the post-integration directory above. The integrated source remains root-owned; this worker changed only the B119 evidence report and private test outputs.
