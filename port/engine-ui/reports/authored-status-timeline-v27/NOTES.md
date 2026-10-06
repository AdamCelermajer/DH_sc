# Original status-message animation V27

The same persistent `MenuStatusMessagesV26` queue now drives the original selected HUD's `HUDelements.itemname_text` sprite 166. `OriginalUiSession::render_player` advances only this 48-frame subtree using the actual application tick, elapsed milliseconds, and retained HUD movie cadence. Its authored `scroll` label, transforms, fade and final ActionScript remain the authority. The real `onAnimationEnd` stops and hides the clip, then calls `NativeStopMessage("status", 0)`, which removes the current message and starts the next same-queue message synchronously. No manufactured expiry timer or whole-root advance is used.

The adapter is reset when the actual movie is released or changed. The source queue belongs to the persistent UI/application service and is not replaced by this animation adapter.

## Verification

`host-receipt.json` records a 96-check ASan/UBSan run against the actual Android HUD/shared SWFs, original text localization and fonts, the coherent current source-facade UI library, and actual HUD selection/onPush. Two queued statuses complete through the authored callback at ticks 39 and 78; only the first enqueue starts immediately, the second starts on first completion, both are removed, and the final clip is hidden. Paused and hidden status clips do not advance. Texture, renderer and unrelated platform callback providers are declared test fixtures. This does not prove live monster-death XP delivery.

`android-compile.json` records strict ARM64 and x86_64 compilation of the timeline module and its `OriginalUiSession` connection. No APK was built or installed by this task. The manual host regression must use the same `overlays/edit-text-v1` header include path as the coherent UI library; omitting it produces invalid C++ layout observations.

## Remaining XP production connections

- The actual `PlayerManager+6c4` initialized-character count, rather than map count `+6a0`, supplies XP recipients. The V25 player-manager boot preserves the real initial zero count. A real AddCharacter/InitAll continuation must publish the initialized player before XP can reach one recipient.
- The live progression actor must borrow the same property state, Save, character identity, source local/remote flags, current Level policy `+150` and difficulty. Level C1's genuine zero policy allows XP; no fabricated policy or recipient count should be introduced.
- Level-up needs original actor metadata `+13c8`, class cache reset/recalculation, HP/MP regeneration, and actual SG_Save delivery. The root player metadata constructor default remains `-1` until the real InitPre producer runs.
- Full level presentation still requires the original localized `MENU_LEVEL_UP` status enqueue, `VisualFXManager.PlayAnimFXSet(0x87)`, source `PlayerLevelUp` script notification and reached tutorial/campaign/trophy branches. EventManager event 87 is not an equivalent FX call.
- XP floating text must use the existing native source formatter, original XP color/style and actual victim/world anchor. The formatter is already available through the root combat text sink; the kill-to-progression dispatch still needs composition with the real victim, contributors and current Level.
- Modern GL/movie recreation while a status is pending needs explicit same-head presentation restoration; this adapter does not silently dequeue or fabricate replay semantics.

The existing skill-buffer V26 and player-manager V25 code was preserved unchanged.
