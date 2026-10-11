# B106 - E at "Travel to" does nothing

## Root cause
P17 TRANSITIONS2 moved the travel confirm to the Space context button and only when no object of interest (e.g. NPC Rene beside the exit) is present. The prompt still printed "[E]", and the E key (semantic `interact`, binding 'E') was never read for travel. In shot-23 the priest is the object of interest, so Space talked and E did nothing.

## Fix (main.cpp, one hunk at the P17 TRANSITIONS2 confirm + key feed)
- E (`uiInput.actions.interact`) now confirms the armed prompt, regardless of an NPC nearby. Space still confirms when no object of interest exists. Prompt text stays "[E]" (consistent).
- `--transition-key N` test hook now presses the real E key through the semantic input (instead of a private confirm path, which is disabled), so the test exercises the production binding.

## Evidence
- Video: P1 reference does not show the prompt (TRANSITIONS2 report: not located in footage). Prompt art / confirm button not verified against video. Not matched to video.
- IDA: `NativeGoToZone` (0x004422b8): called by the Flash HUD travel button, independent of the action/context button; QuickSave + SG_SaveAllPlayer, then `DisplayFastTravel` callback and `Application::LoadLevel(level from Arrays::LevelList by name, entry, slot, ...)`. So a dedicated travel button (PC: E) is consistent with the source; Space as an additional PC alias is kept.
- Run (hidden, isolated save, build-fix106, logs in .local-inputs/claude-v20.1/fix106/run2.log): `Transition prompt shown ... target=SWAMP_CAVE_WITCH_A display="Bogwitch Cavern" frame=41`; `Transition confirmed key=E ... frame=60`; world saved; `Procedural level generated rule=data/scene/002_swamp_witchcave.rule.xml tiles=7 modules=7`; `Transition arrival entry=0 spawn=_prim_EntryPoint position=203.6,557.5,37.1`. Frames r2-swamp-f55/r2-cave-f150/r2-cave-f250.png.
- Destination frames (not fixed, B109): cave arrives with hero standing on a water/bog cobble floor, red blood decals, dark blue-green rock walls on the right/bottom, Rene and the faery follow; a "Travel to: The Boglands [E]" prompt is shown immediately on arrival (arrival point lies inside the return exit zone; same touch rule as the Darkwood case). Rock walls look stretched/dark; not judged against the video.

## Not verified
Physical keyboard E in a visible window (E injected through the same semantic key path by the test hook). No new unit test (logic is in main.cpp); full ctest not run.
