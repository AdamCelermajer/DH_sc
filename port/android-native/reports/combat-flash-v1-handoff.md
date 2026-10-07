# Retained scrolling combat text integration

`character_combat_text_v1` implements whole original Character selector `3af77c`, preserving source follower suppression, miss/dodge/block/fear/slow branches, actual attacker property queries, dual-hand style masks, raw damage `>>8`, original localized words and GameDesign colors. Required producers remain failures; no text is generated from substitute combat values.

`CombatFlashSwfV1` retains the same loaded droid HUD graph and its original font platform. `scan()` finds original `anim_` styles in graph preorder, retains clips/text fields, and hides original style clips. Each style has eight real instances; clones use source `_clone_%d`. `play()` uses the actual camera projection callback and source inverse pixel scale. Its twelve-context queue preserves original full/drop and instance-seven reuse behavior. `update()` executes original strict timer comparison and finite frame completion; `draw()` uses native text/color/timeline/display callbacks with temporary visibility, position restoration and source buffering toggles.

## Root integration

Add `character_combat_text_v1.cpp`, `combat_flash_queue_v1.cpp`, `combat_flash_swf_v1.cpp` to the existing native target. Retain one `CombatFlashSwfV1` inside the same `OriginalUiSession` owner, after loading its actual HUD/font platform. Destroy it before that movie/platform is replaced. Call `scan()` once after source HUD graph initialization.

Include `engine-ui/swf_movie_combat_flash_v1.inc` inside namespace `dh2::ui` after the actual `SwfMovie::Impl`. Add its public declaration `bool source_display_rectangle(float rectangle[4], std::int32_t viewport[4], std::string&);`. Its callback is read before the bridge enters the graph scope, avoiding nested scopes. This supplies the existing source viewport connection's exact rectangle and submitted pixel viewport, not a fixed surface ratio.

Construct the bridge with `CombatFlashProjectionV1`: `world_to_screen` borrows the submitted camera/world projection; `display_rectangle` calls that same movie's `source_display_rectangle`. Return required-provider failure if either source producer is unavailable.

Install the source `CombatTextServicesV1` on the actual combat application backend. Borrow follower, world position, actual bounds height, attacker properties, actual dual wield/IsPlayer, same retained GameDesign and StringManager. Its `enqueue` callback calls `flash.play(q->style,q->position,q->text,q->number,q->color,q->numeric,error)` synchronously on the owning render thread and returns `0` on success. String/style pointers are borrowed only for this call; if root uses a transport queue, copy them and keep original order. Do not replay the combat selector from a HUD snapshot or subtract HP in the presentation layer.

Call `flash.update(actual_application_dt, actual_level_load_phase,error)` once per source MenuManager tick; do not update it additionally through `SwfMovie::advance`. Original Level+130 is `_LoadProcess` phase, initialized0 and incremented by the original loader to38; it is NOT a GameDesign level kind. Phases2..26 use33ms; other phases use `int(1000/movieFPS)`. The current native loader has no original phase progression; borrowing its retained constructor0 prefix is faithful, while setting38 merely because the world is ready would invent a producer. Draw once per source submitted frame, supplying the actual `IsDisablingFlashAnimation` Debug switch from the same World Debug owner. Preserve native/provider diagnostics and reached queue entries on failure.

## Evidence and bounds

`character-combat-text-v1-original.json`: original ARM selector versus optimized ARM64, 2560 cases, 11352 ordered source calls, zero mismatches. `combat-flash-timer-v1-original.json`: whole original FlashAnimManager Update versus optimized ARM64 queue, 675 cases, zero mismatches across all twelve contexts. Source input/service fixtures are explicit.

`combat-flash-v1-host.json`: ASAN/UBSAN actual droid/shared SWF and genuine original TTF/native edit text/glyph renderer. Finds twenty authored `anim_` styles, draws source numeric text with actual ARGB transform, creates a real style clone, advances finite clips and verifies expiry/reuse. Camera projection/external bitmap delivery are explicit host fixtures. This is positive native render-sink proof, not a GPU or live combat claim.

The host library is the existing source edit-text overlay build, preserving that overlay ABI. Stock GameSWF virtual `goto_frame`/STOP is the recovered RenderFX wrapper's dispatch; broad original sprite timeline parity outside the authored tested combat clips is not claimed. Root still owns live gameplay service binding, camera projection and live screenshot proof.
