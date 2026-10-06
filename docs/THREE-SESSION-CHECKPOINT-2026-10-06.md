# Three-session checkpoint — 2026-10-06

Compared with 791e961, the last published snapshot. All saved source is preserved; this is an ongoing reconstruction checkpoint, not complete-game acceptance.

## Gameplay and rendering

Main-menu entry to the Crypt demo, original portrait/statistics flow, target markers and a targeted skill were exercised. Native builds now use optimization with debug symbols. Retained skinning/effect buffers, geometry culling and reduced HUD/GPU submission improved a controlled software-renderer comparison from roughly 29 to 39 FPS. Sustained 60 FPS and general coverage remain unverified.

Normal attacks and skills now share target position; ranged close-range checks, out-of-sight cleanup and frontal-priority acquisition were added. Three skill assets across eight headings and two swords in both equipment slots passed source alignment checks; this does not establish every live visual case.

Native audio is linked, 638 original sound/event bindings recovered, and the authored sfx event-name provider connected. Missing clips and audible gameplay acceptance remain open. GPU resource controls cover encoded/decoded textures, GPU storage, effect/HUD buffers and cleanup on failure/context loss. Exhaustive navigation diagnostics are being removed from normal loading into bounded optional QA.

Both ARM64 and x86_64 builds passed. Crypt rendered on an Android 16 ARM64 phone, but FPS was not measured. A killing skill exposed a missing shared kill handler that stops the native frame; complete loot/XP/quest/death integration remains open.

Emulator protection includes a process-tree hard cap, watchdog, admission and timeout checks. The cap was subsequently raised to 15 GB at the user's request and paging permitted while retaining system commit reserve. Emulator startup, launcher/watchdog behavior and memory exhaustion remain documented issues; no universal stability claim is made.

## Menu contribution

Local menu flows now include character creation/deletion, options, keyboard, class/starting-equipment preview, intro and splash. Gameplay loading artwork and tips render, with completion/host-wait callbacks compared against 972 original-code cases and checked at three resolutions with resize/resume.

Latest handoff is v88 build-only. Saved inventory/equipment restoration and actual Start Game into the genuine world remain unfinished. The menu session is paused and its existing integration handoff was delivered to main. Current source and saved evidence are preserved separately; v88 is not described as a fully runtime-verified release.

## Loader contribution

All 195 SWAMP objects construct, including 50 characters and five chests. Earlier preview rendered 27 characters and five chests. The 21 template-based visual gaps were resolved with 371 original-code comparisons; newer host checks account for all 50 character and five chest asset references. Those selections are not fresh on-screen proof.

Added generic renderer/actor inputs, exact material slots and original visibility rules. Chest draw checks emit two visible skinned primitives while retaining the hidden collision helper. All 41 trap declarations pass transport checks. Missing inherited Character cache/master/spawn fields are being adopted through the existing actor.

Loading now uses retained real Level counters and the original root/module InitPost order. Authentic stage-7/10 source-loading probes traverse all nine module pairs before geometry/floor preparation, then stop at missing chest MeetCondition service. Conflicting providers are rejected; cancellation cleanup retains resources until successful teardown and passed ASan/UBSan checks. Abort cleanup is distinguished from normal unload to avoid saving incomplete transitions.

Genuine activation, GS frame envelope, gameplay conditions/timelines, graphics integration, transitions/restoration and fresh visible SWAMP acceptance remain unfinished.

## Capture scope

Gameplay files are at repository root. Isolated menu and loader sources remain under session-contributions, with prior evidence preserved. The file manifest binds exact saved bytes; active sessions can continue afterward. Ignored build outputs, APKs and machine-local inputs are excluded. Existing RIGHTS.md applies. No new emulator launch or gameplay mutation was performed for this publication checkpoint.
