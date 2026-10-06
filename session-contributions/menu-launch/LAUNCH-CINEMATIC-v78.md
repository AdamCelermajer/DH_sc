v78 normal-launch original intro

Normal MainActivity launch now opens CinematicActivity with the recovered original intro.mp4, then reaches the authored main menu. Previously the cinematic was opt-in only. Explicit front_screen/model/texture/world/original_hud/developer entries retain their direct engine inspection routes, and an explicit cinematic boolean can override the default. The cinematic completion intent explicitly requests main, preventing a replay loop. Activity recreation with saved state does not redirect back to the intro. This is the requested Android launch flow, not a claim that the complete original native GS startup policy has been recovered.

Visible isolated emulator5580 runtime verification: normal launch at landscape1920x1080,2184x1968,2400x1080; byte-identical original video visibly advances; native video dimensions1280x720 and aspect-fit bounds verified; actual Skip button and Android Back reach main/title loop; Home/resume retains video position; changing resolution during the same live video retains playback; full natural completion reaches main/title loop. Explicit loading entry bypasses intro. AudioFlinger evidence collected, no listening quality claim. All campaign/settings hashes preserved. Separate main emulator5554 and main checkout untouched.

The original authored loading splash/overlay remains available through front_screen loading. Its automatic transition/progress from actual game loading is pending canonical loader ownership. Start/New Game still uses explicit pending feedback; canonical Player assignment/serialization/loading, multiplayer/achievements and final preview material lighting remain unfinished. Exit confirmation-to-platform teardown remains pending. Goal remains active.

Evidence: launch-cinematic-v78/validation.json, screenshots, per-size and lifecycle logs, before-private-files.tar; source/APK checkpoints front-v78-source and front-v78.apk; cumulative patch and169 receipts retained.

Installed APK SHA256 dbe6fb161af26cd127e0476c0393f4e40e35e2eb47e55f80705268aa9039e2ce
