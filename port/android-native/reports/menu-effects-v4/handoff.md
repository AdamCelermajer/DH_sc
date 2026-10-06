# Authored character menu sound V4

The existing exact name-to-ID catalog is retained. NativePlaySoundFX argument/name misses remain source no-ops. A present packaged catalog WAV is queued through the existing UI-thread FrontAudio receiver; an absent asset logs `Original menu sound not ready` and returns without audio or interrupting the ActionScript menu operation.

Original source chain: NativePlaySoundFX43ae10 -> PlayMenu36bc34 -> Play36b80c. LoadSound3699fc retains the DataHandle wrapper. StreamCFile C1/Init888f18/888a60 initializes size+4 to zero; missing filesystem open leaves it zero. CreateNewCursor888aec returns NULL for size<=0; LoadDataSource86b144 cleans up and constructs an invalid (-1,-1) handle. Play checks IsReady8624f8 at36b908 and returns at36b90c without creating/playing an emitter. The old adapter's fatal backend check contradicted this branch.

`missing-file-original-proof.json` runs whole original ARM Init and CreateNewCursor with an explicit filesystem-NULL boundary (2 PASS cases). The remainder of the missing path is original-body inspection, not a complete Vox differential. Captures: `port/engine-ui/reference/menu-sound-v4/original.asm`.

`source-assets.json` records all183 original sound rows, actual filenames, format/volume/repeat, per-file hashes, and missing records. Both provided cache ZIPs have identical sound members (`cache-comparison.json`). PickupWeapon109 / sfx_pickup_weapon.wav and the other Pickup, Drop, MenuPotion/PotionDrink WAVs are genuinely absent; no replacement sound was created. All actual catalog WAVs have been copied byte-for-byte into original-media; the materializer verifies each source SHA and refuses to overwrite differing existing assets.

FrontAudio keeps actual audio focus, resumed/focused gating, effect volume, preparation errors, completion/error release, pause/stop cleanup. Its former five-name filter now accepts safe catalog WAV basenames; native exact catalog lookup and asset presence remain the publication boundary. WAVs are uncompressed by the existing Gradle noCompress rule, so the retained openFd transport is valid. No new JNI method is required.

Production edits are restricted to FrontUiSession native sound dispatch and FrontAudio.effect validation. No app build/install/live sound validation was performed in this lane; root must build and test the item/equipment operation and present menu cues. Missing resources are diagnostic non-playback, not claimed audible support.
