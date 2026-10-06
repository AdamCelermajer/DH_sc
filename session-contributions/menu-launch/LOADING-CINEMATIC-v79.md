v79 loading artwork and launcher resume

The supplied Droid loading SWF's sole shape2 fill mapped to character artwork in the packed splash atlas, rendering a spinning fragment. A separate compatibility movie now maps that shape to the first complete original red/gold ring cell, actual pixel bounds(21,74,793,845) identified in original dqmenus.swf embedded bitmap1. Original cache and texture bytes remain untouched. repair_loading_atlas.py reproduces the movie and verifies every non-shape tag, geometry/action/timeline byte remains unchanged. The original Android packing mapping remains unresolved; this is a documented original-art compatibility correction, not a claim to recover those original coordinates. The original rotating timeline drives the indicator.

The loading overlay is uniformly fitted within the separately fitted original GSInit startup splash rectangle1280x752, so its indicator stays inside the artwork at wide, square and portrait surface sizes. Main menu viewport behavior remains the same. Normal Android launcher reentry now discards a duplicate non-root Main entry and resumes the existing task, avoiding an accidental fresh intro above a live loading screen/cinematic.

Final installed APK tested on separate visible emulator5580/privateADB5038. Three display overrides1080x1920,1968x2184,1080x2400: ring visibly advances and its motion bounds stay inside splash artwork. Same process live resize/Home/launcher return retains loading screen and no intro replay. Screenshots include portrait fitting after launcher return. All campaign/settings hashes preserved. The emulator had closed during verification; process/5580/5581 disappearance was confirmed before reopening its same private AVD with existing userdata. Main emulator5554 and main checkout untouched.

Final APK also ran the full original50.292sec cinematic through natural completion. Normal cold launcher enters original video/audio, Home/resume retains position, one intro entry only, story screenshots at10/20/30/40sec visibly differ, natural completion reaches authored main and title music loop. v78 separately tested video aspectfit/Skip/Back and live cinematic resize at three sizes; those receipts remain tied to its exact earlier APK. No listening-based audio quality claim.

Current loading entry remains an explicit frontend screen. Actual startup/game progress producers and automatic loading-to-game handoff require canonical loader integration; the full goal remains active. Exit confirmation teardown, canonical player/save assignment/serialization/game launch, online services and preview lighting are still unfinished. No new routine status messages were sent to the main session.

Evidence loading-art-v79/runtime and cinematic; source/APK checkpoints front-v79-source/front-v79.apk; generator and compatibility mapping receipts are in the cumulative review patch/files manifest.

Installed APK SHA256 16e6b1b5d2ccbba1177ffc88b2c846127bd227f52ff462e76af4e464688c2f83
