# Version 1 retained HUD sprite scheduling

This connects the five authored dqhud status timelines to the recovered native status producer. It is a source scheduling proof plus a bounded native GameSWF-core connection. It does not claim original ActionScript/tag interpreter parity, a complete HUD manager, or live GPU rendering.

Original authority: libDungeonHunter2.so SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Complete routines and addresses are in original-functions.json and reference/original-functions.asm. The original ELF executes only in desktop instruction oracles.

## Original scheduling

`sprite_instance::goto_frame(int)` at 0x781a3c obtains frame count first. An invalid or equal signed frame stores STOP (1) and returns without dirty notification. A changed frame replaces the previous goto batch with the current pending batch and clears pending. Backward seeks execute reverse tags down through target+1; forward intermediate frames execute state-only tags. It clears intermediate pending actions, executes the target normally, then stores the signed-16-bit target and STOP, appends target pending actions, clears pending and notifies advancement. STOP is committed after target traversal. Services can synchronously mutate the live state and reenter; the oracle includes 80 actual original/native recursive goto cases.

`set_play_state` at 0x77fe10 obtains the genuine engine sound handler (0x77cba0). A delivered null handler is source absence, distinct from missing lookup. A nonnull handler and nonnegative stream ID require pause delivery using OLD play state == 0. It then stores the low signed byte and always notifies advancement. Source RenderFX goto wrapper applies `play XOR 1`; a request to stop therefore supplies 1, including after an invalid MP/XP frame. A caller must not skip set-play-state because goto returned false.

`notify_need_advance` at 0x7750e8 marks each live node before examining its weak parent. For a dead parent proxy, it decrements the 32-bit reference count with wrapping arithmetic, requires deletion when the result is zero, then clears the captured node's weak parent/proxy slots after that callback. The pure field kernel preserves those effects and deletion reentry. Coherent borrowed topology is required; source-invalid cycles and exhausted native bounds reject.

## Native connection and ownership

`hud_sprite_timeline.hpp/.cpp` implements source scheduling over explicit action buffers/services. Capacity failure remains explicit; generic source allocator ownership is not reconstructed.

`hud_sprite_core.hpp/.cpp` binds the actual retained loaded dqhud SHA256 `a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238`, exact definition/root identity, and only these source clip IDs/frame counts:

| Clip | ID | Frames |
|---|---:|---:|
| HealthBars.player.bar_hp | 90 | 100 |
| HealthBars.player.bar_mp | 147 | 100 |
| HealthBars.player.bar_xp | 152 | 101 |
| HealthBars.btn_potion.DistressGlow | 113 | 103 |
| _root.HurtCorners | 31 | 103 |

It uses actual upstream retained frame-tag/reverse operations, not hand-drawn bar geometry. Bindings are borrowed only within the retaining movie's scope. The clip is strongly pinned during synchronous traversal. Unexpected pending/goto action history, frame-generated ActionScript batches, stream-sound goto paths, missing sound/dirty providers, and changed binding identities fail with the executed prefix retained. Supported real clips have no such action/stream paths after initial advance(0). Generic queued-action consumption and unsupported clips are deliberately not silently accepted.

`HudAdvanceOwner` provides the concrete required notify callback. It walks actual retained core weak parents and owns a sidecar needs-advance field per weak character identity. It never strongly retains the movie/characters, sweeps dead references, and handles pointer reuse by validating the weak identity. This is the storage projection needed because the upstream character lacks the original dirty byte. `reset_after_advance` is an explicit modern consumer operation; no original dirty-clearing producer or complete original root-advance algorithm is claimed. The pure `hud_advance` field kernel independently proves the original weak-control arithmetic and callback ordering; upstream weak-control ABI is not presented as bit-identical.

Root integration can call `advance_owner.notify(sprite,error)` as the required notify provider. It must supply a genuine engine sound lookup and mandatory stream pause if reached. Bind/find/goto/play belong within the movie's protected scope and retained root/graph lease. Actual viewport publication should precede an authored reflow advance if that script reads Viewport; complete original Application/MenuManager input/advance ordering remains a separate caller boundary.

## Evidence and reproduction

- scheduling-gold.bin: 1,730 complete original/O2 ARM64 comparisons, 27,402 ordered services, 80 recursive goto cases; host replay adds 16 guards, ASan/UBSan/LSan zero.
- advance-gold.bin: 450 original/O2 ARM64 comparisons, 72 weak-control deletion callbacks, signed wrapping/deletion reentry; host adds four guards, sanitizers zero.
- `reports/hud-sprite-core-host-audit.json` is the preserved historical bridge receipt. The final `reports/hud-sprite-core-owner-host-audit.json` binds the coherent extended facade and viewport dependencies separately: 1,014 actual retained clip traversals, 602 authored child-matrix comparisons, eight frozen status-producer cases/160 services, 2,089 dirty deliveries, 1,055 genuine upstream sound queries, 11 weak nodes, three ownership guards and seven failure guards. Movie destruction releases all weak entries. ASan/UBSan (excluding upstream vptr)/LSan zero.

From repository root, run `python port/engine-ui/tests/hud_sprite_timeline_host.py`, `python port/engine-ui/tests/hud_advance_host.py`, and `python port/engine-ui/tests/hud_sprite_core_host.py --core-library /home/adampalace/dh2-gameswf-asan/libgameswf_core.a`. Each records actual source/binary/gold hashes. The core runner creates an audit-only scoped visitor in scratch; it edits no production facade/vendor file. The final core runner links the actual viewport dependencies added by root during integration and checks every captured source hash before/after.

The real core uses frozen native upstream GameSWF 1714 plus separately documented compatibility/teardown corrections. Texture images, localization and stencil remain fixtures in this dedicated core test. Source scheduling parity does not convert those fixtures into proven original GPU/material/AS/font ownership. The final receipt's full-original-tag/AS-parity and live-GL flags are false.

## Remaining HUD subsystem

The frozen status producer covers HP/MP/XP/distress/hurt only, using genuine resolved-property words (HP 36/max38, MP41/max43, XP33/max34) and source signed arithmetic. Full NativeGetPlayerHUDInfos, potion text/counts, skill/spell cooldown/visibility, equipment/property148, active player/debug and manager lifetime are separate recovered containing-method boundaries. Those services must be connected before claiming the full original HUD; `_xscale` and invented percentages are not substitutes.
