# Original root viewport and MenuFlash2DCamera

This separately owned native module ports the coupled original root bounds,
viewport, screen conversion and camera Update arithmetic. It provides the
actual renderer rectangle produced at the start of `root::begin_display`.
The original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The adjacent manifest captures ten complete routines, including:

| Original | Address | Native entry |
|---|---:|---|
| MenuFlash2DCamera::Update | `0x42cd84` | `dh2_ui_flash_camera_update` |
| root::set_display_viewport | `0x775d38` | `dh2_ui_set_viewport` |
| root::set_display_bounds | `0x7755f4` | `dh2_ui_set_bounds` |
| root::screen_to_logical | `0x773dc0` | `dh2_ui_screen_to_logical` |
| root::logical_to_screen | `0x773f50` | `dh2_ui_logical_to_screen` |
| root::begin_display coordinate prefix | `0x7751bc..0x775208` | `dh2_ui_display_rectangle` |

`ViewportState64` borrows the source movie rectangle in twips (xMin,xMax,yMin,
yMax). Viewport and bounds are signed x/y/width/height. The receiver is a
**live player** projected from the original weak-player reference, not a
guessed movie clip or an event listener. This module does not own or emulate
the legacy weak pointer. The caller retains the projected player and service
owners across the synchronous call.

The original driver orientation comes from renderer virtual `+0xac`. Bounds
query it once if the first value is zero, twice otherwise; the second result
equal to two selects the unrotated width/height pairing. Screen/renderer point
functions query once and treat zero and two identically. Other values select
the other axis pairing. This is an explicit provider; Android orientation or
surface width alone is not proved as its producer.

Let movie pixel extents be `W=(xMax-xMin)/20`, `H=(yMax-yMin)/20` and let bounds
be `(bx,by,bw,bh)`, viewport `(vx,vy,vw,vh)`. For orientation zero/two:

* `screen_to_logical`: `x=(x-bx)/(bw/W)`, `y=(y-by)/(bh/H)`.
* `logical_to_screen`: `x=x*(vw/bw)-(bx*20)/(bw/W)` and
  `y=y*(vh/bh)-(by*20)/(bh/H)`.

For other orientations, screen x uses by/bh/W, screen y uses bx/bw/H;
logical-to-screen x uses vh/bh and by/bh/W, y uses vw/bw and bx/bw/H.
The operation order and float32 intermediates are retained. The latter entry
is used on movie **twip** corners for renderer bounds; it is not a drop-in
inverse for pixel-valued touch coordinates. `dh2_ui_display_rectangle` returns
the source-projected `[xMin,xMax,yMin,yMax]` for the GPU begin command, without
mutating authored scene matrices. It executes the two orientation queries in
the source order. The rest of original begin_display's render-owner/state
callbacks is outside this helper.

Mode zero retains exact requested bounds. For modes one/two, the ratio is
`(height/movieHeightPixels)/(width/movieWidthPixels)`, with width/height movie
pairing selected above. Mode one selects width expansion when ratio>=1 and
height expansion otherwise; mode two does the opposite. Float-to-int truncation,
signed-half centering and wrap are preserved. Other mode words pass through.
Stored pixel scale uses the source comparison `sx<sy ? sy : sx`, including
its ordered nonfinite selection. A changed viewport writes all four viewport
fields and calls bounds with those exact arguments and mode zero. An unchanged
viewport/bounds does not redo the corresponding effects.

## Player-global publication, not an ActionScript event

The bounds tail constructs `gameswf::as_object` (`0x76b820`), sets literal
members `xMin`, `yMin`, `xMax`, `yMax` to the two screen-to-logical corner
results, adds the reference (`0x759c64`), then writes player-global member
**`Viewport`** through virtual `+0x1c`. The actual literal is at `0x8e3298`.
The corners are `(0,0)` and `(float(vx)+float(vw),float(vy)+float(vh))`.
The required `publish_viewport` service receives `[xMin,yMin,xMax,yMax]` and
must implement owned global-object publication in the SWF owner. No permissive
event/no-op implementation is supplied. Original AS allocation, ref-counting,
member setters and weak-reference expiry are an explicit boundary in this
proof. Earlier discovery prose describing a bounds-change notification should
be read as a description of this global publication, not proof of an event.

## Camera and integration order

The camera eases signed current offsets toward desired offsets once per Update:
`1+trunc(wrappedDelta/10)`; it uses no elapsed-dt factor. It then reads actual
driver dimensions, optionally clamps against the captured clip rectangle,
calls SetViewport(0,0,w,h), then SetBounds(currentX,currentY,w,h,mode0).
Small limit rectangles can cause the later max-bound clamp to supersede the
earlier min-bound clamp; no invented centering policy replaces it.
SetLimitClip (`0x42cf30`) stores the borrowed clip identity and captures its
rect only when nonnull through MenuFX::GetClipBounds `0x416a7c`. The caller
must supply that captured pixel rectangle; dynamic clip rect recomputation
is not part of camera Update. Constructor `0x42ccd0` initializes current and
desired offsets to zero, and stores half source design globals in separate
fields that Update does not read. Constructor/global/clip owner recovery is
captured, not claimed as the native owner here.

The Android UI owner can now retain one ViewportState, provide true driver
dimensions/orientation, compose both camera callbacks with root setters, and
feed the source display rectangle to GPU projection while using the separate
screen-to-logical entry for input. Publish the actual global Viewport object
when a live player is present. Required provider failure returns -2, malformed
projection returns -1, success is zero. Mutated prefixes survive provider
failure; no rollback or fabricated acceptance is performed.

## Proof and reproduction

`reports/viewport-arm64-differential.json` compares 5,200 actual original
instruction executions against O2 ARM64 (6 operations, 12,683 ordered services),
with exact finite words and NaN classification. It covers source orientation
queries, all three modes and other mode words, zero/negative dimensions,
nonfinite rectangles, signed camera wrap, equality and stateful resizing.
Renderer/driver projections and post-conversion global publication are fixtures.
The real original coordinate prefix of begin_display executes; later render
services are not claimed.

`reports/viewport-host-audit.json` replays the same gold under ASan/UBSan/LSan,
with twelve provider-failure, partial-prefix, malformed-overlap and synchronous
publication reentry guards. Source/binary/gold hashes and exact isolated compiler
command are bound by `tools/viewport_host.py --build`.

ARM64: `tools/build_viewport_oracle.ps1`, then
`tests/viewport_differential.py --engine <ELF> --library <oracle.so>
--report reports/viewport-arm64-differential.json
--gold reference/viewport/viewport-gold.bin`.

Host: `tools/viewport_host.py --build`. The central target can compile
`viewport.cpp`, and an audit target `tests/viewport.cpp` links it and runs with
the gold path as its sole argument. No core/vendor/GPU/shared CMake changes
were made for this module. This is not a full UI/input/ActionScript/GPU parity
claim.
