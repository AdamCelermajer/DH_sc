# Source viewport connection

This additive adapter connects the already audited original viewport/camera arithmetic to a retained genuine GameSWF root, player and ActionScript graph. It does not replace the existing SwfMovie facade, drive a frame, select Android orientation, or claim whole-frame/GPU parity.

## Exact graph ownership and scope

`SwfViewportLease.owner` must retain the exact implementation graph that owns `root`, its weak player and ActionScript heap. Retaining the outer SwfMovie is insufficient: loading another movie replaces that implementation. The facade can change its private implementation ownership to `shared_ptr<Impl>` and return `{impl, impl->root.get_ptr()}`. Each operation and display must retain a local copy through its existing `Impl::Scope`. AS publication synchronously invokes arbitrary watchers; the adapter retains the graph across them and has no extra once-only guard. Releasing the connection during a watcher is tested. Destroying the borrowed connection receiver during its own callback is outside the native caller contract.

On reload, scoped display must compare the lease root with the current implementation root and reject a stale graph. The old lease remains valid until released. Do not silently move its state to a newly loaded movie. Root mouse notification writes buttons, X, Y only, as original 0x774128 does. Stock upstream notification additionally emits immediate MOUSE_MOVE and is not that original primitive.

## Practical original initialization

Original root C1 0x775da0 initializes viewport `(0,0,1,1)`, pixel scale 1, background opaque black and weak player; it queries definition width/height then invokes the actual SetViewport 0x775d38. It does not initialize every bounds word before that call. For the actual authored rectangle `(0,9600,0,6400)` twips and width/height 480/320 pixels, the resulting source projection is:

```
movie_rect = {0,9600,0,6400} // xmin,xmax,ymin,ymax
viewport   = {0,0,480,320}  // x,y,width,height
bounds     = {0,0,480,320}
pixel_scale = 1
reserved = 0
```

`bind` validates the rectangle against the real definition and reloads the real weak player. This is a postconstructor projection for this authored input, not a claim that allocator bytes for arbitrary roots were zero. General setup should derive the seed from actual authored rectangle and source constructor/setter evidence.

FlashCamera C1 0x42ccd0 stores null limit-clip identity at +8 and current/desired offsets zero at +24/+28/+2c/+30. Its half-design-dimension fields +1c/+20 are not read by recovered Update 0x42cd84. Thus zero `FlashCamera40` gives the complete initially read source fields; limit rectangle is unread while identity is null. Source SetLimitClip captures clip bounds only when nonnull; no arbitrary per-frame recapture.

Call `camera_update` with genuine renderer orientation and current driver width/height services. The original callback composition invokes SetViewport `(0,0,width,height)` then SetBounds `(camera.currentX,camera.currentY,width,height,mode0)`. It neither aspect-fits nor rescales the authored stage to an invented logical rectangle. Camera easing is integer offset arithmetic with no dt factor. The adapter then supplies `display_rectangle` from the exact source coordinate prefix. Screen-to-logical input consumes pixels; logical-to-screen display corners consume twips, so these APIs must not be casually treated as a pixel-to-pixel inverse pair.

RenderFX Initialize(driver) 0x7a98f8 constructs its source initialization arguments, sets the driver, null callbacks/allocator fields, a true byte and speed 1; CreateContext 0x7a9708 constructs genuine player-context providers. MenuManager::LoadSWFFile 0x42d290 and MultiMenuManager counterpart 0x437d68 load a MenuFX then construct its FlashCamera. Driver orientation comes from renderer virtual +0xa8 through RenderFX.SetOrientation 0x7a7eb4; Android surface aspect is not evidence of that enum.

## Overlay background and display contract

Original `render_handler_glitch::begin_display` 0x7d73d0 stores its incoming packed background color at 0x7d73dc (`sp+0xc`) and never reads that slot in the complete 992-byte routine. It saves driver transforms, viewport and scissor; installs the display viewport/projection; resets batching and source render state. There is no background-color draw or color-buffer clear. Root background itself remains authored/source state. A modern world-HUD sink must preserve the world color at begin-display rather than clear it because root background is opaque. Do not manufacture source transparency by changing the movie's alpha.

Minimal scoped facade addition: acquire the exact graph lease; enter its existing Scope; check graph identity; call connection.display_rectangle; invoke the existing sink begin_display with root background, the connection viewport, and the returned twip rectangle; draw the requested actual clip; end_display even on required cleanup paths. Do not call stock `root::set_display_viewport` here: it bypasses the original bounds/publication policy and overwrites pixel-scale. Preserve source background in the draw record while the modern backend follows the original no-background-draw behavior. Display world first, then HUD; frame/touch/Application ordering remains caller-owned until connected with its source producers.

## Proof and limits

`connection-probe.json`/gold execute original source setters and point mapping; 160 camera cases execute actual original camera and root setters together. Only MenuFX receiver routing and observation before ActionScript allocation are services in this original probe. Original root constructor, raw mouse setter/getter and setup/display routines are captured completely in the manifests and ASM directories.

Host v2 executes the actual privately copied UI DSO: 4,684 gold comparisons, 1,583 fresh ActionScript publications in original order, 10,752 driver callbacks and eight ownership/failure guards; ASan/UBSan/LSan have zero findings. A genuine AS watcher verifies every rectangle and exercises reentry/release. This is a composed bridge proof, not a new original-full-frame or Android-visible claim. Existing viewport ARM64 differential remains the optimized original arithmetic evidence. The complete original GL begin routine is captured; full original driver/GPU state implementation is not reconstructed here.

Reproduction from WSL repository root:

```
PYTHONDONTWRITEBYTECODE=1 python3 port/engine-ui/tools/swf_viewport_connection_host.py --ui-library .local-inputs/swf-viewport-connection-host/private/libdh2_engine_ui.so --build-directory .local-inputs/swf-viewport-connection-host --report port/engine-ui/reports/swf-viewport-connection-host-audit-NEW.json
```

For central CMake, compile swf_viewport_connection.cpp into the genuine UI DSO and compile only tests/swf_viewport_connection.cpp into its audit target, linking dh2_engine_ui with the same sanitizer/options and gameswf include/configuration as swf_movie_audit. Test argument is `reference/swf-viewport-connection/connection-gold.bin`. Preserve reports tied to previous actual DSO bytes.
