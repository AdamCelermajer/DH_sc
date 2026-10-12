# B119–B122 independent audio review

Reviewer: `/root/batch_audio_review`. Read-only shared source review; root owns integration and frozen builds. Final status: **B119/B120/B121 READY for source-faithful route/output repair**. **B122 reviewed output/loader audit and reachable merchant/page/Skills/Faery/Equipment cue repairs READY on their named freezes; B133 audio/visible modal review READY on276C; bounded combined B119�122 sanity PASS; root release acceptance remains separate**. No shared behavior file edited; no acoustic/ear-parity result claimed.

## Independent source findings

- Read `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt` directly. Merchant BUY/SELL mode tabs call MenuSelect at 0xbf92/0xc03a. Action BUY branch opens confirmation and calls MenuConfirm at 0xc162; ordinary SELL branch calls doMerchant at 0xc1bd then PickupGold at 0xc1bf–0xc1cb. Cue admission is reaching the authored release branch, not native success. The native buy result/error path inside doMerchant does not own the later caller cue. Normal source SELL is direct; a shared PC confirmation adaptation must be distinguished from original source behavior.
- Dexterity increment handler directly shows NativeStatsAssignPoint at 0x14a4f–0x14a67 followed by MenuSpending at 0x14a69–0x14a75. Native return is discarded; do not insert an invented success-only cue condition. UI disabled/release admission and every other stat handler remain B121 owner's proof responsibility.
- Checked ledger: PickupGold generated ordinal155 → XML UID155 → `sfx_pickup_gold.wav`; ledger marks cached sample absent. MenuSpending146 → UID0 → `sfx_menu_spending.wav`. Old ledger absence does not prove absence from newer selected package roots. Verify exact selected root/file hash and provenance before final available/missing claim.

## Runtime sink and proof boundaries

- `main.cpp:3600` constructs RuntimeSessionAudioV1 only when `options.runtimeAudio`; parsing `--audio` at585. Initialization failure logs and gameplay continues. Campaign sound callback with no host is a silent no-op (3631 onward). Therefore identify launch args/root and initialization, not merely callsite existence.
- RuntimeSessionAudioV1 `play_named_ui_sound`392 delegates same host typed request. RuntimeAudioHostV1 `submit_named_ui_sound` resolves source name/ordinal, verifies plain PCM/event0, loads exact XML UID, checks host/device readiness, submits zero-delay fresh emitter, and requires own token/UID. Output-not-ready, missing exact asset, unknown name, and command failure are distinct.
- Plain `play_named_plain`380 uses the same retained host with source Play semantics; missing-original error is logged and cleared as successful silent no-op. Do not infer a queued voice from its bool return. Potion original plain Play should not be silently reclassified as PlayMenu solely for diagnostic convenience.
- Host owns one AudioNativeSessionV42; retained runtime mixer/clock/context is shared. Window activity publishes focused/minimized and pauses output; source world cues additionally require current listener/world epoch and source authority. Source/sample admission differs from a user's ear hearing it.
- `quiet_run.ps1:62` defaults DH_AUDIO_SILENT=1. `winmm_output.cpp:78` converts every mixed sample to zero under that variable, then calls waveOutWrite79. Real WinMM started receipts and device progression in this mode prove event→voice/output lifecycle, not nonzero device PCM or audibility. `-AllowSound` alone may inherit a parent's DH_AUDIO_SILENT, so inspect actual environment for audible tests.

## Evidence and verification review requirements

B120 source/visual note correctly identifies original Part1 585–600s merchant sequence and insufficient-gold BUY; those frames cannot prove a successful BUY or its sound. Existing audio reports similarly establish UI/quest sequence context and source IDs; no original audio match was independently auditioned in this review. Do not label source-caller identity as acoustic verification.

Before READY: freeze executable SHA and source state; use own isolated save/window and exact assets; exercise affordable buy, confirmation open/No/Yes/duplicate, normal disabled buy, defensively refused admitted buy, sale and no-selection/duplicate, all four stat increments, no points, cancellation semantics. Preserve visual sequences and native mutation/log plus exact cue ordinal/UID/token/started receipt. Source refused dispatch can still request cue; refused UI admission cannot. Focus/output/missing-asset failures must not veto transaction/stat gameplay. Broader B122 paths need concrete integration/repair and production coverage, not only census or fixture pass.

## Review coordination

Sent merchant versus stat cue correction and unconditional caller-order requirement to B122/root. Requested B119–B121 concrete patch/API and branch evidence. B120 corrected an initially contradictory statement about SELL cue; requested exact normal direct-SELL versus defensive confirmation distinction. B122 matrix corrected merchant identity.

## Current completion boundary

Investigation/static review only. No new executable run or frozen production path available to reviewer yet. B119/B120/B121 stay implementation-owner/root integration pending. B122 broad audit is useful but not a completed repair; exact absent samples remain explicit gaps with authentic recovery ownership. Await freeze/test handoff for independent runtime review.

## Accepted asset/root check and isolated host verification

User confirmed v21.1 via root. Actual accepted `startup.args` contains `--audio`, `--audio-assets audio-assets`, `--audio-table audio-assets/sounds_pyarray.bin`; Play.cmd invokes startup config. Both expected assets exist in that selected package tree, contrary to old cached ledger absence:

- `.local-inputs/windows-source-clock-v21.1/audio-assets/data/sounds/sfx_pickup_gold.wav`: 66936 bytes, SHA256 `1F6B70C1EE674BF894E50A736327E946D79307AA1D3F3E7AFF2A72707A60A7A4`.
- `.local-inputs/windows-source-clock-v21.1/audio-assets/data/sounds/sfx_menu_spending.wav`: 36426 bytes, SHA256 `8535FF7019E1B23CFE356C215457D5FF126DB3FE67E4AF3CD6376444028B2C7B`.

Paths/hash independently inspected. B120 reports the PickupGold hash also matches the ios source path; this review has not independently rehashed that original tree yet. Root and all four owners informed, and B122 asked to audit other missing claims against the actual selected accepted root.

Ran retained `.local-inputs/windows-toolchain/runtime_audio_host_v1_tests.exe`, SHA256 `9C76C0F7E8EC4606670C74E188768CD8BD17722E2F4E7EAD6D61295C23F4043B`, with feature audio assets and `.local-inputs/combat-sound-v2/sounds_pyarray.bin`, `DH_AUDIO_SILENT=1`. PASS: existing fixture single V42/WinMM source232 voice, QPC/TIME_SAMPLES progression, disabled/online-muted/inactive gates, exact missing Troll328 diagnostic, focus pause/resume, listener fail-closed, checked shutdown. This retained executable is not identified as current-source build and this is fixture output mechanics only, not B119–B122 production caller coverage or audible parity.

## B121 proposal review concern

`.local-inputs/v21.3/B121/main-callback-proposal.patch` precomputes only Tab::stats plus geometric hit. Independent `source_composition.hpp` read shows real release first rejects hidden presenter/invalid dimensions and processes presenter hit precedence, then stat resolver/provider. Geometry alone can emit a cue when no authored callback executed. Requested actual callback-occurrence/suffix placement (native return may be ignored, actual caller must be reached). Zero-points original deactivated-button gate remains unproven. Existing callback currently rejects zero source_stat_points; deliberately bypassing that with raw geometry is not established source fidelity.

## Selected-root census follow-up

Independently compared accepted `audio-assets/data/sounds/sounds.xml` against feature catalog: both SHA256 `4BC1DE654D65470D49E48CEE354D5D5CDD45006C18D65420DDC07ACF5B400A6E`. Accepted `audio-assets/sounds_pyarray.bin` matches retained combat-sound-v2 source table SHA256 `1A0F9D325D3F2D10EC1CB15394EC709B6DA991B3818EB33C6CABDA95BEE25AB8`.

Full old-ledger exact-path probe against accepted root saved only in bounded `.local-inputs/v21.3/audio-review/audio-selected-root-census.json`: 722 asset references, 497 distinct filenames; 639 references present, 83 references / 44 distinct filenames absent. This is URI existence, not full decode/source-consumer verification; multiple event references repeat assets. Catalog itself contains malformed/extensionless names (`sfx_impactl_metal_1_22.wav`, `sfx_skill_mage_thunder_braid_add_2,wav`, extensionless cultist chant/specter explosion). Asked B122 to investigate original selected bytes/catalog/source-version before proposing aliases; filenames alone cannot justify guessed substitutes.

B122 independently reports accepted source-authentic bytes for PotionDrink, Lizard hurt/death, TrollReturn and PickupWeapon as well as merchant/stat clips, hash-matched against supplied ios originals. Earlier missing claims for these rows are stale for accepted v21.1. B122 remains open for concrete consumer repairs and production checks.

Clarified numeric domains: AnimationTables Lizard row374 Died and377 Injured carry SoundAutoGen IDs476/477; ledger source IDs476→UID285 die and477→UID284 hurt independently confirmed. Animation row377 is not source sound ordinal377. Preserve the domain labels in final reports.

## Continued integration review ownership

Root explicitly retained this review through actual frozen tests; initial HOLD was a status, not feature completion. Lead `batch_integration` owns shared WT main/core/CMake changes. Trace-only frozen commit63d7e97f SHA `EDFB187F2B172BED8DD79152232FF681A4EA59C7D0BB991D3D83CA5D1F26DF6E` predates audio caller integration; not used as a post-fix result. Own four fresh replay arg directories prepared under `.local-inputs/v21.3/audio-review/cases/` for sell/sell-disabled/sell-duplicate and stats. No accepted save/package modified.

B119 full report reviewed: success/disabled/duplicate baseline branches reproduced by owner, no named cue. Correct native per-frame guard separates duplicate mutation from source caller continuation; each admitted callback requests PickupGold even if duplicate native mutation refused. No command/disabled UI means no cue. Lead informed.

B120 distinguishes direct source SELL versus confirmation BUY and includes modal-open MenuConfirm, accepted PickupGold once after buy or max-potion loop, normal insufficient UI disabled, defensive native refusal source follow-through, cancelled/stale accepted modal no duplicate cue. Actual production freeze still pending.

B121 revised proposal installs `stat_training_post_release` inside SourceCompositionV1 after actual registered callback invocation. This addresses initial geometric bypass concern. Source caller native no-op versus deactivated button release uncertainty remains explicit; do not assert artwork proves original zero-point release rejection. Confirm/cancel code does not contain MenuSpending; test against actual private UI controls once frozen. Owner baseline real C key + client60,310 at 2x viewport stages Strength10→11 / points2→1 with no cue.

B122 output diagnostic proposed by helper is actual WinMM refill premix versus post-silent conversion snapshot; offline sample metrics alone cannot prove final voice/output. Requested lead integrate reusable stats and test real frozen caller token/started receipt together with output state. Current QuestNew startup not_ready retries same frame and becomes token1 in owner baseline; do not falsely diagnose that transient alone as missing cue. Also asked B122 to audit main LevelUp call's named-ui helper versus original Script_PlaySound plain Play flags.

## Independent frozen caller tests completed

SELL: tested frozen c84b93d2 SHA `D9E0FEF887F39959D3D86621431CC6C0358E68B3EF61244B8B1F7770DECBF9C5` on three own fresh hidden/silent runs. All exited0. Exact args/logs/captures under `.local-inputs/v21.3/audio-review/cases/{sell,sell-disabled,sell-duplicate}/`.

- Success: frame140 one StartingSuit transfer, gold0→2, player items4→3/stock81→82, PickupGold155 UID155 token3 submitted then matching started receipt. Viewed final SELL UI with gold2 and empty Torso list.
- Equipped refusal: no Merchant transaction or named sound; viewed equipped Ceremonial Garb Armor2, gold0, dark SELL.
- Duplicate: frame140 three admitted callbacks yield tokens3/4/5, matching started receipts, one sale and two native duplicate blocks. This preserves source caller continuation rather than success-only audio.

Stats: tested f9ed76a9 SHA `8DED82833D817693698988432D2570D23CDCA0D54744E845CCDF167FE2A8C34A` with real Win32 C key/clicks in three own fresh hidden/silent windows, explicit960x640, isolated `--test-stat-points4` setup. Exact args/logs under `cases/stat-{success,yes,no}/`; all exited0.

- Four + callbacks: stat0/1/2/3 points4→3→2→1→0; exact MenuSpending146 UID0 tokens3/4/5/6, each started and finished. Additional zero-point + emits token7 with no mutation, matching current admitted callback continuation; original deactivated-button admission remains unknown. Final UI visually shows points0 and each test stat increment. Debug fresh fixture Strength/Dex began0, so do not claim authored normal starting values from this fixture.
- Yes: one allocation token3; opens confirmation, commits at frame384 points3; no further MenuSpending.
- No: one allocation token3; opens confirmation, rollback at frame385 points4; no further MenuSpending.

Initial own stat run lacked copied point seed and used default1201x720. It established three admitted zero-point callbacks; one intended Dexterity click missed due viewport scale. It is not positive-point coverage. Corrected explicit-size positive tests above resolve that setup gap.

These are production callback→exact token→mixer lifecycle proofs with DH_AUDIO_SILENT=1, not nonzero device PCM or a heard sound. B119 owner additionally reports non-silent f9ed76a9 equivalent branches via -AllowSound with env unset/healthy refills; no device-bound PCM snapshot or loopback is exposed yet. B120 purchase fixtures and B122 actual output stats remain pending. Lead advised last refill after cue finish can be zero; metrics must capture event interval/cumulative signal rather than treating final active-voice absence as failure.

## B120 extended independent branches

Replayed retry owner's retained-profile fixtures on f9ed76a9 in own hidden/silent windows with only copied character saves and absolute accepted assets. Explicit invalid transition-entry9999 is documented test setup to retain inventory and stage merchant proximity; this does not certify normal transition behavior.

- `.local-inputs/v21.3/audio-review/cases/purchase-bag-full-profile/`: exactly100 runtime items at cap; accepted confirmation reaches TradeResult2, price300, gold10000 unchanged, stock81 unchanged, changed0. Native refusal still follows source caller's PickupGold155 UID155 token3 and matching started receipt; MenuConfirm token2 precedes it. Viewed final authentic Inventory Full / Cannot buy more equipment popup.
- `purchase-potion-full-profile/`:12/12 full capacity; ordinary action disabled, no merchant named cue or transaction.
- `purchase-purchase-reopen-profile/`: reloads prior successful saved profile with gold9700; persistence result agrees with purchase mutation.
- `purchase-potion-max/`: MenuConfirm141 UID2 token2 at110; accepted kind6 Potion0 loop at150 debits120 total (12×10), gold10000→9880. Exactly one PickupGold155 UID155 token3, started and finished, despite repeated Yes. No per-item cue introduced.

Owner's preliminary80-extra/104-item seeds were corrected to final exact100-item gate coverage before this independent replay. These final cases all exit0. Purchase audio source/admission/normal UI/result/duplicate/cancel/refusal/persistence branches pass review; nonzero current-output measurements remain distinct from these silent token/voice results.

## Evidence preservation and disk

Root alone removed27 current-turn raw PPM duplicates after pixel-equivalence verification against retained lossless PNGs (67.8MB). Manifest `.local-inputs/v21.3/audio-review/raw-capture-equivalence.json` records original paths/PNG/byte sizes. Source, args, logs, save fixtures and PNG visual evidence preserved. No accepted package/user save or old evidence was deleted by this reviewer. Root reports39.9GB free and owns further raw-capture cleanup/final commit/worktree cleanup.

Output diagnostic owner corrected PCM reset on successful new WinMM open before rendering; single-output frontend shutdown/gameplay handoff makes the reset scope valid. Strict silent close/reopen test passes and checks zero counters/device position at reopen before rendering. Final integrated PCM-enabled freeze still awaited.


## Final frozen route/output verdict

Frozen executable `.local-inputs/v21.3/integration/frozen-4c60f3d5.exe`, independently verified SHA256 `4C60F3D538C6F26C81E7FBB27ACBB6AE4EBFC191A37E9C817B1585D9C41ADEAD`. All8 own source-gated seller/buyer regressions on this freeze exit0; own final args/jobs/save/logs under `.local-inputs/v21.3/audio-review/final-4c60f3d5/`.

- SELL normal: exact155/UID155/token3; one gold0→2 transfer. Disabled equipped/no-command branch has no sale cue. Three admitted same-frame releases queue tokens3/4/5 while only one sale occurs.
- BUY normal: MenuConfirm141/UID2/token2, then PickupGold155/UID155/token3, one300-cost purchase, gold10000→9700. Repeated Yes has no extra mutation/cue. Cancel onlyConfirm; disabled zero-gold neither cue.
- Native full-bag refusal: result2,100 items, gold10000 and stock81 unchanged; PickupGold token3 still follows admitted source handler. Buy Maximum loops12 at10 each with one post-loopPickupGold token3 and one120 debit.
- Each admitted cue token has its matching mixer-start receipt. Final full-root silent runs show nonzero actual mix, positive refill/device counters and zero write failures, with submitted PCM exactly0 under the intended silent flag. Ambient audio contributes to those totals, so they do not isolate a particular cue.

Independent **sole-cue normal output** on this exact freeze: own `endpoint-pickupgold/` uses the helper's bounded original metadata/catalog root with authentic MenuConfirm/PickupGold only; noConfirm action occurs in SELL, and no other voice is submitted/started. Gameplay/content root remains acceptedv21.1. DH_AUDIO_SILENT was explicitly removed and `quiet_run -AllowSound` used for the private165-frame (<3 device-seconds) session, with metrics enabled. PickupGold155/UID155/token1 starts at outputFrame126976. `WinMM PCM: silent=0 refills=287 writeOk=287 writeFailed=0 submittedFrames=146944 deviceFrames=143357 mixNonzero=39902 mixPeak=0.218048 submittedNonzero=39802 submittedPeak=7144 lastSignalFirstFrame=146432`. This directly proves the correct original cue contributes nonzero int16 samples to successful current WinMM submissions; it does not claim a person heard it or acoustic parity with a video-only reference.

B120 owner's independent pure-cue sequential test on this freeze isolates complete Confirm and PickupGold: Confirm185313 mixed nonzero samples; combined284281 = Confirm185313 + PickupGold98968. Disabled cue-only run has0 voices/0 mixer signal. These values are consistent with source/caller/PCM boundaries. The normal sole-PickupGold endpoint proof above supplies the shared final conversion/device evidence.

**B119 READY**, **B120 READY** for their requested caller/output fixes. **B121 READY** for allocation callback/output repair: all4 real Win32 + routes, commit/rollback and zero-point continuation already verified on f9ed with unchanged callback semantics; B121 owner also verified non-silent route, exactUID0 WinMM consumer test independently verifies nonzero sample/rendering, and final emitter/conversion path passes above. Preserve the original zero-point Flash dispatch caveat: retained deactivated hitzones and handler strengthen admission inference, but footage does not test that click. No repeated tests absent a relevant source delta, per root direction.

**B122 output routing/diagnostic repair passes review**: exact caller identities, source-sensitive refused outcomes, typed plain result, selected authentic assets, same retained output, per-open PCM reset, actual current nonzero device submission are established. Broader B122 verdict still needs its owner's final44 exact-loader URI/kind/current-active consumer classification. Unused/unknown catalog entries are caveats, not invented current production failures; confirmed active missing consumers remain implementation gates with ownership. B130 typed potion exercise is owned by its new retry session. Final combined-release regression/acceptance remains root-owned.

## B122 final census and staged merchant review

The completed exact-loader/current-consumer classification passes review. The 44 absent unique catalog basenames are not 44 demonstrated production failures. Original LoadSound3699fc prepends data/sounds/ to the exact selected sounds.xml filename, with optional archive lookup and no near-name/suffix fallback. Current bounded swamp music selects present UIDs467/505/468; absent combat-bank labels540–542 are not selected by that scene. The live current lizard combat trace selects present source120/121 and marker476/477. Conditional ArmoredHuman impact117/118, CriticalHit1 and checkpoint catalog entries remain explicitly unproved current callers, rather than globally unused. No fabricated asset or alias repair is warranted by this census.

Staged merchant select_mode/close callers in DH_wt/v201int pass independent static review. Source BUY/SELL releases request MenuSelect143/UID3 before mode mutation, including same-tab; the appended command enum preserves existing values. Source Back0xCDBB requests MenuBack140/UID1 before leaving/resetting. Current caller preserves that order and does not let sound failure prevent the visible transition. Outside none and confirmation/error popup swallowing remain in the model; focused private tests pass. Frozen normal production tab/same-tab/outside/Back voice receipts are still pending, so this implementation gate remains open.

Merchant arrow releases and category focus-in are distinct source events; focus must not be guessed from a raw hover or click without an admitted event/API or explicit PC adaptation. Further character page/equipment/skill/Faery cue omissions are documented in B122-CHARACTER-UI-AUDIO.md and assigned fresh owners by root. Those confirmed caller gaps remain their named implementation gates. B119–121 READY does not close these new scopes or constitute accepted-release approval.

B130 retry reports final4B42 typed PotionDrink160/UID147 token2 at consumed key5 frame146 with matching start/end, held-key suppression and fresh full-vital refusal, exhausted/signed-count refusal coverage. Its current non-silent endpoint has positive submitted PCM, healthy writes and device progress; full-root ambient totals do not isolate the potion sample. Gameplay reviewer/root retain B130 acceptance.

## Newly assigned UI cue proposals reviewed

B122-PAGE-CUES.md: MenuTab147/UID4 admitted open/top-tab callback, same-tab included, diagnostic direct select silent. Reviewer requested correction of after-close recipe wording: authored MenuBack140/UID1 precedes PopAll/BackToHud; request immediately before actual accepted close, after modal/admission decision. Pending stat/skill confirmation branches must not emit Back. PlayerCanBackToHud parity is unestablished and C/Escape adaptation is explicit.

B122-EQUIPMENT-CUES.md: exact source mapping/order passes: PickupArmor154/UID152, PickupPotion156/UID153, PickupWeapon157/UID154; Equip post-attempt/list refresh, Unequip pre-attempt, Details AutoEquip MenuConfirm141/UID2 pre-operation. No cue is established for ALL AutoEquip. Native refusal cannot be converted into a success-only cue gate; UI rejection is distinct from admitted callback/native no-op. Slot9 currently unadmitted canonical equipment branch remains unresolved without bypassing it for a test.

B133-report.md: exact decoded statements independently checked. Transmute open MenuSelect143/UID3 at0x1D747 precedes warning/push; Drop open MenuConfirm141/UID2 at0x1D85E follows parsed text. Yes pops menu then calls doTransmute and PickupGold155/UID155 at0x34628, or doDrop and MenuConfirm141/UID2 at0x34667. No native-success condition intervenes, and inspected No handler has no explicit cue. Pending admission/one-shot validation remains menu reviewer responsibility; actual frozen modal route/voice checks are pending.

## B4B5 merchant tab/Back frozen PASS

Independently hashed executable integration/dh-foundation-integration-b4b5f74dad9b02e69c2b3a3646752a99316541eeddb74c067c462401004d1c7f.exe matches SHA256 B4B5F74DAD9B02E69C2B3A3646752A99316541EEDDB74C067C462401004D1C7F. Own quiet fresh production replay under audio-review/merchant-navigation-pending exits0. Scheduled SemanticInput down/up releases, not OS hardware clicks: SELL90, same SELL100, BUY110 emit exact MenuSelect143/UID3 tokens3/4/5 with matching start/end; outside120 emits no cue. Back140 submits MenuBack140/UID1 token6 before Merchant closed log, with matching voice start. Capture cap.png independently viewed and confirms HUD after close. Silent metrics mix312315 / submitted0 / writes313 failure0 / device156758 preserve configured silence.

Owner B122 modal-suppression actual logs and f110.png reviewed: normal10,000-gold retained profile opens authentic960-gold Conscript Armor confirmation; SELL110 swallowed, No120 no extra cue, SELL130 emits token4, Back140 token5; both exact voice lifecycle receipts present. Non-silent submitted572917 peak26475, writes570 failure0/device288162 establish healthy endpoint output. Full-root aggregate includes ambient audio and does not isolate the individual menu cues; unchanged exact samples, exact tokens and shared endpoint proof preserve that distinction.

B122 merchant tab/Back repair passes runtime review. Merchant left/right arrow release MenuSelect remains a separate confirmed caller gap pending implementation and frozen receipt; category focus-in remains distinct event/API uncertainty. Fresh page/equipment/skill/Faery and item-modal owners retain their own integration gates. Do not interpret this bounded PASS as closure of the expanded B122 scope or accepted release.

## B4B5 character page cue review READY

Reviewed integrated Presenter::close_with_before and composition callback: close_guard evaluates once; refusal returns before cue; accepted callback precedes open_=false. Main C/Escape use that boundary; admitted top-page releases emit MenuTab and diagnostic provider selection remains separate. Owner real Win32 all-pages logs show exact147/UID4 tokens3–9 for open/six tabs, accepted Escape140/UID1token10, later C open/close tokens11/12, each started. Same-tab/outside, diagnostics and Back pointer branches are evidenced. Staged-point guard-refusal log/capture independently inspected: Strength10→11, confirmation opens, no MenuBack, Escape dismisses modal and menu remains open.

Exact sole-original MenuTab non-silent output log independently checked: token1 kind0 START113664, kind1 FINISH158282 (owner requested to correct initially mislabeled start field); mix87676/peak.241191, submitted83972/peak7903,393 successful writes/failure0/device197581. Source ordering, actual admission, visual and final endpoint evidence pass within stated C/Escape PC adaptation, source/footage version difference and unestablished PlayerCanBackToHud parity. Page cue repair READY; further expanded scopes retain owners.

## A433 Skills/Faery bounded caller repair READY

Independent hash A433637AA8FE512AB8F303BCD5D82FCA280934F130E564FDF3B0B5479FB37526 matches frozen owner executable. Reviewed actual source hooks: Skills source availability probe precedes train, postlude cue follows attempted train even false; Faery known/list/unlock gate precedes activate_slot, post callback follows attempted activation even false. Native refusal is distinct from rejected admission.

Normal physical-input logs independently checked: genuine level2/one-point source seed trains row4 rank0→1, point1→0 then MenuSpending146/UID0token4 at539, voice447488→474775. Faery0 and fresh same0 release follow committed callback then MenuSelect143/UID3tokens6/7 at718/779 with full voices591360→595388 and640512→644540. Repeat no-points Upgrade and locked1/outside no extra cue. Runtime3 captures f540.png and f780.png independently viewed: rank1/points0/Upgrade absent; Celeste0 selected and other slots locked. Normal WinMM0 underrun, healthy refills/gap counters establishes route, not cue-attributed PCM; common renderer endpoint proof retained from prior exact-source output cases.

READY within explicit limits: another unlocked Faery is unavailable in the genuine seed, alternate/refused callbacks covered only by source contract tests; no original Faery visual sequence or acoustic parity was recovered. No synthetic unlock/point grant was used for credited runtime.

## 02621 merchant arrow cue repair READY

Independent freeze hash02621BB87ADF4E88B1F9AAFE5D0F35195A161455ADD13355FF01E86FF41BBC4F and own audio-review/merchant-arrows-02621 run exit0. Appended select_navigation comes after accepted Details previous/next selection and other-pane refresh; caller emits exact MenuSelect via retained typed UI route, then continues. Full source ClassChangeUp/Down wraps0→9/9→0, resolving delta-guard concern for valid categories. Owner selected-label captures show Torso→Potions→Torso→Right Hand→Torso; f95.png independently viewed Potions selected. Modal owner log has Confirm at90, no arrow110/no cue on No120, arrow130 only after dismissal.

Own four admitted arrow releases at90/100/110/120 emit143/UID3tokens3–6, each matching start/end; outside130 none, Back140token7 start. Silent metrics mix307196/submitted0/writes310 failure0/device155202. Owner normal full-root endpoint submitted580444 with575 writes/failure0/device290526; aggregate includes ambient and is not isolated arrow energy. Source-authentic cue/event/visual/output route proof passes. Category TypeN focus-in is still a separate unknown/API question, no unconditional click/hover cue invented. Merchant arrow repair READY; expanded equipment/item-confirmation scopes retain their implementation owners.

## 02621 reachable Equipment cue repair READY

Reviewed coherent order-tests.log PASS and actual MainPage post-attempt Equip/pre-native Unequip hook, partial binding/current slot admission, correct native-false continuation. Normal Continue Equipment logs show exact Armor154/UID152, Weapon157/UID154 and DetailsAuto MenuConfirm141/UID2 with normal slot mutation/repeat receipts; sword-absent f95.png independently viewed. Ring state/effect, ALL absence, empty LeftHand/no selected control and redraw limits documented by owner. No slot9 bypass or manufactured production native-Auto refusal.

Independently read sole-family PCM runtime.log files: Armor token1 START50688/FINISH87897, final int16 nonzero67170 peak10944; Weapon token1 START50688/FINISH86073, nonzero69020 peak6730; Confirm token1 START49152, nonzero168166 peak17903. Each silent0, zero write failures, deviceFrames130816/130871/129867. Exactly one genuine family WAV/voice isolates each signal. Together with source-order/refusal seams and ordinary runtime/UI proof, reachable equipment repair READY. Slot9 and original acoustics remain explicit uncertainties. Menu reviewer/root retain gameplay/preview acceptance.

## Root acceptance and retained B133 follow-up

Root accepted scoped B119–B122 READY. These verdicts do not approve the combined release. Root explicitly retained this reviewer for B133 with fresh owner /root/b133_confirmation_retry until immutable committed combined freeze proof. Earlier DBD popup physical No/Yes center attempts are not credited; source/version admission issue remains possible. Current modal stable pointer capture key is present in main surface hit provider; lead must bind the tested candidate to that committed source before retries. No additional coordinate retry on old DBD.

## B133 provisional 5C8 cue route review

Fresh owner source-bound frozen5C8D77BB6BAB630FAC9EFEDE016403CAFF755EB89D7B07F0E28BE30ABD0F902A now includes stable modal pointer capture key absent DBD. Credited admitted pointer fixture logs independently read: Transmute open MenuSelect143/UID3tokens3/4; actual Yes PickupGold155/UID155token5, all start/end. Drop opens MenuConfirm141/UID2tokens3/4; actual Yes native world drop source64/UID150token5 precedes authored MenuConfirm141/UID2token6; both voices start/end and one published item. Owner cancellation/outside/duplicate branches have no added authored cue. Initial cinematic1 force-open attempts excluded.

Provisional cue route/order PASS only. Original shared modal dims underlying page, while current solids draw order leaves Equipment page/avatar too bright; overlay correction and final immutable combined visible/input repeat remain B133 gates. Root retained this reviewer with fresh owner until that proof; no READY yet.

## Final committed combined 276C B119–B122 sanity PASS

Root commit51c4ac5c469b01969b39d6af1be45a68e5eb2c57 candidate independently SHA-verified276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A. Own combined-276c seller/buyer/arrows jobs and real Win32 Stats clicks use fresh own saves/windows, accepted assets; all final jobs exit0.

SELL price2 one transfer/gold0→2 exactPickupGold155UID155token4; BUY price960 one purchase/gold10000→9040 Confirm141UID2token2 thenPickup155UID155token3. Four arrow releases exact143UID3tokens3–6 matching full voice lifecycles; outside none/Back correct. Actual C+four stat releases at202/261/321/382 stage4→0 points and exact146UID0tokens4–7 each start/end. Stats final screenshot independently viewed points0 and increments; bought menu screenshot viewed gold9040. This fresh diagnostic stat fixture starts Strength/Dex0, not claimed original starting values. Positive actual mix/zero submitted PCM under intended silence, healthy writes/failure0/device advancement in all cases. Endpoint implementation unchanged and earlier sole authentic cue positive normal PCM proof retained.

First stat launch erroneously passed a comment line from saved args to direct CLI and exited before input; log retained as invalid setup. Filtered comments and reran final-run.log successfully. No runtime defect inferred from that launch. All runner sessions finished; no active test/audio window.

Root's177/177 CTest pass is integration-suite evidence, distinct from these normal frozen consumers. B133 combined modal proof still pending fresh owner/reviewer; root acceptance/release separate.

## Final 276C B133 audio/visible cue review READY

Reviewed exact matching276C owner runtime logs and pointer-up/semantic-click/outcome diagnostics for final276-physical-transmute/drop. Transmute outside outcome0 retains modal/no cue, No outcome1 dismisses/no cue; two admitted opens143/UID3tokens3/4, actual Yes outcome2 action1 bag-0-Ring01 then PickupGold155/UID155token5, matching complete voices. Drop No outcome1 no extra; two opens141/UID2tokens3/4, actual Yes action2 publishes one native world item source64/UID150 then authored MenuConfirm141/UID2token6; both exact voice lifecycles. Duplicate after acceptance adds no transaction/authored cue. Current Yes source call remains unconditional after native attempt and one-shot pending admission consumes action first.

Owner open.png and yes.png independently viewed: original ornamental WarningBox/prompt above genuinely dimmed Equipment/avatar, exact source191/255 shade recovered rather than guessed; Transmute Broken Ring into27 gold; accepted result ring absent, gold27, modal dismissed. These final artifacts supersede DBD/input-miss, cinematic1 forced-open and5C8 old bright-overlay attempts. Source preflight/localization precedes the current Transmute cue but ordinary observable cue-before-visible-modal ordering preserved; no original broken-localization failure sequence established.

B133 audio/visible cue review READY. Menu reviewer owns snapshot/source-lease/gameplay admission verdict; B136 source-camera restore/cancellation retest remains separate and unproved here. No ear/loopback parity claim. Root final combined acceptance/packages/commit cleanup separate. All independent tester processes closed.
