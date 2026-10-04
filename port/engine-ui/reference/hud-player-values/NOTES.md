# Original player HUD status frames

`hud_player_values.hpp/.cpp` reconstructs **only** `InfoHUDManager::FastUpdate`'s status region `0x41e0f4..0x41e208`, plus the complete `RenderFX::GotoFrame` wrapper `0x7a7d34..0x7a7d90`. Full containing methods are captured in `original-functions.json`/`reference/original-functions.asm`; the bounded region byte hash and source string/asset bindings are in `bindings.json`. Original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

The visible connection is an authored **timeline frame**, not `_xscale`. `initCachedChars` resolves `_root.menu_HUD_%d`, where `%d` comes from the genuine `HUDStyle` design query. Under that base, the five status target paths are returned by `hud_value_clip_path`. HurtCorners is absolute and uses a null base. The cached clip provider must preserve that base/path distinction and same retained movie identity.

| Order | Resolved sheet indices | Source frame | Target |
|---|---|---|---|
| 1 | HP36 / MaxHP38 | clamp(q-1,0,99) | HealthBars.player.bar_hp |
| 2 | MP41 / MaxMP43 | min(q-1,99), no lower clamp | HealthBars.player.bar_mp |
| 3 | XP33 / MaxXP34 | min(q,99), no subtract/lower clamp | HealthBars.player.bar_xp |
| 4 | Reuses HP frame | Same cached HP frame | HealthBars.btn_potion.DistressGlow |
| 5 | Reuses HP frame | Same cached HP frame | _root.HurtCorners |

Each quotient is signed division of a **wrapped 32-bit multiplication** `100*current` by the signed maximum. Integer truncation is toward zero. Subtractions also wrap. The original calls imported `__aeabi_idiv` with no maximum-zero branch. This module requires `divide_zero` delivery for a zero divisor; its result is an explicit unresolved runtime service, not an invented zero result or full original libc/runtime reconstruction. Nonzero division, including INT_MIN/-1 wrapping, is native arithmetic without signed-overflow UB. The proof labels the 313 cases that supply runtime-zero results separately.

The source reads HP, then MP, then XP before resolving clips. These are cached **resolved** property words at Character payload `+0xff8` (HP `+1088`, MaxHP `+1090`, MP `+109c`, MaxMP `+10a4`, XP `+107c`, MaxXP `+1080`). It does not recompute base/class/gear/buff properties, test death, or query a supplied health Boolean in this region. A live caller should borrow its real resolved property sheet and preserve its lifetime during the synchronous call. The API checks alignment/count/reserved fields; allocation validity remains a borrowed-storage contract.

For each clip, the source captures the current manager RenderFX pointer **before** cached clip resolution, then uses that captured value for the wrapper. A callback may synchronously mutate the live owner; the next clip reloads RenderFX. Clip identity stays captured across sprite-type/goto/pause callbacks. `RenderFX::GotoFrame` skips null and non-sprite type2 objects. Otherwise it always invokes sprite virtual goto-frame then virtual set-play-state with `play XOR 1`; source `play=false` gives STOP1. The goto virtual's return is ignored, including invalid/negative MP or XP frames. Backend providers must deliver genuine sprite operations; unsupported operations return required failure rather than successful no-op. Negative frame requests must reach the actual backend unchanged.

## Proof and integration

`tests/hud_player_values_differential.py` executes the actual original bounded instructions, actual complete RenderFX wrapper and O2 ARM64 native instructions. Cached clip resolution and sprite virtual endpoints are caller fixtures. The imported signed division is explicitly modeled. The oracle starts at the status region with source r4/r6 already projected; it does **not** execute or replay the omitted potion prefix. It stops before the property148 visibility/skill tail. Gold: `status-frames-gold.bin`, 1,562 comparisons / 13,721 ordered services / zero mismatches. It includes signed overflow, strict frame boundaries, negative/max-zero inputs, missing/non-sprite clips, arbitrary original play-word XOR and synchronous lookup mutation. `tests/hud_player_values.cpp` replays the same original gold under ASan/UBSan/LSan and adds 15 failure/malformed/reentry guards; zero findings.

Standalone commands from repo root:

```powershell
& port/engine-ui/tools/build_hud_player_values_oracle.ps1
# Python needs the existing Unicorn/ELF tooling environment.
python port/engine-ui/tests/hud_player_values_differential.py --engine .local-inputs/libDungeonHunter2.so --library .local-inputs/hud-player-values/hud_player_values_oracle.so --gold port/engine-ui/reference/hud-player-values/status-frames-gold.bin --report port/engine-ui/reports/hud-player-values-arm64-differential.json
python port/engine-ui/tests/hud_player_values_host.py
```

Central host target: compile `tests/hud_player_values.cpp`, include this module's header directory, link the actual engine-ui DSO; CLI is a single gold path. No central CMake, renderer, frozen core/viewport or APK changes were made here.

## Sprite timeline boundary and upstream fork difference

The actual dqhud resource has HP90/MP147 with100 frames, XP152 with101, Distress113/Hurt31 with103. The five definitions and recursively referenced sprite placements have no DoAction/DoInitAction/PlaceObject clip-action tags, per `bindings.json`. Authored placement/geometry still must execute; this observation does not replace them with custom scales.

Captured original sprite `goto_frame(int)` `0x781a3c` has an important difference from upstream GameSWF r1714: it copies the **current pending action list** into its goto batch before traversing, then appends target-frame queued actions. Upstream clears current pending actions before target execution and replaces the goto batch with target actions. Original STOP is stored after valid traversal; upstream stores STOP at entry. Both invalid/equal requests stop immediately, but original returns0/1 whereas upstream returns void. Full arbitrary ActionScript timeline goto parity is therefore unclaimed. `goto-frame/` preserves the original complete function and set-play-state body for a future exact adapter. A narrowly scoped HUD implementation may reuse upstream frame-tag operations only after accounting for pending action history, not merely the direct no-action tag inventory; externally scheduled actions or mutable callbacks are still possible. The source set-play-state also owns sound notification/state-refresh services, which are not reconstructed by this status module.

## Explicit remaining source manager boundaries

Before this region, FastUpdate obtains the local player/Character, returns on null, queries equipment potion count and updates the authored potion count text. After this region it reads property148 and writes visibility, then handles skills/cooldowns and additional target/dead/online/debug paths. Those remain mandatory separate producers for full HUD behavior. `NativeGetPlayerHUDInfos` (`0x44e5cc`) is a separate ActionScript callback with 2/3-argument coercion, player-active guards, actual skill services, LEVEL/HP_PCT/HP_LOWPCT/MP_PCT/XP_PCT and potion outputs; only its captured source facts are documented here. Its HP/MP/XP properties are **raw percent quotients**, not these frame adjustments.

`InfoHUDManager::Update` (`0x41ec00`) owns current-level eligibility, RenderFX presence, cache initialization/applyOneTime, a500ms signed-wrap slow timer using actual Application.GetDt and FastUpdate. This module is not an alternative full manager, full skill UI, scene/session owner or live GPU verification.
