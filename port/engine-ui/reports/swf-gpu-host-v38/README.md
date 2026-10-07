# Independent V37 audit and bounded host GLES comparison V38

**No actual host GLES or benchmark acceptance is claimed.** The fail-closed
memory preflight refused before starting the compiler or graphics context.
Available host RAM was 6,107,172,864 bytes (about 5.69 GiB); this fixture reserves
1 GiB above the existing 6 GiB safety floor and therefore requires 7 GiB. Commit
headroom was sufficient. Root separately confirmed an active user application
accounts for much of the pressure; no user application or unrelated process was
touched. No emulator, ADB, device, download or production source change occurred.

## Independent invariant review

The review compared the exact frozen V36 and V37 header/CPP payloads, verified
their ZIP manifest hashes and inspected the actual shader pack whose production
SHA256 is `365a4d3c432454c44208ebb484c7a472a3a4534a0c5c9e77a7a90f3b87b1b5c0`.
`frozen-source.diff` and `actual-shader-audit.json` retain the evidence.

- The actual normal fragment shader computes `(texture + DiffuseColor) *
  vColor0`; actual diffuse alpha is zero. Exact final byte alpha zero therefore
  produces fragment alpha zero. The normal RGB factors are SRC_ALPHA and
  ONE_MINUS_SRC_ALPHA; separate coverage alpha uses ONE and ONE_MINUS_SRC_ALPHA.
  Both destination RGB and alpha are preserved mathematically. The guard uses
  the converted final byte, so a source alpha zero raised by cxform still draws.
- CPU texture/span/coordinate/matrix/color/blend/wrap validation precedes the
  no-op. The source primitive counter and command serial advance. Normal passes
  inside masks, every stencil writer, and non-normal bitmap passes still render.
- Transparent lines convert/validate width but do not mutate GL_LINE_WIDTH;
  real lines use the context-cached capability clamp. Geometry cache keys remain
  exact packed bytes plus primitive mode; matrix/color remain per-draw uniforms.
- Warm error polling is deferred, not removed: display/mask entry and exit,
  scene-pane and stencil queries, allocations/uploads and resets are barriers.
  Every error reports its command range. This intentionally changes error
  detection timing from per primitive to the next mandatory barrier; it does
  not establish identical failure timing or source callback termination.
- The owned stencil format remains DEPTH24_STENCIL8 on resize; GL_FRAMEBUFFER
  completeness is still checked for both color targets. Texture/line capability
  values and owned stencil-bit value reset on actual context initialization.
  Retained buffers abandon lost-context names and upload genuine CPU bytes.

No new source correctness blocker was found in these V37 invariants. Existing
scene-pane invalid/empty bounds return false after materialization and rely on
the caller's abort ownership; this behavior predates V37 and is not a newly
accepted recovery guarantee. Driver pixels, actual menu flow and performance
remain pending despite the earlier six strict ABI compiles and sanitized host
GL call-contract PASS1,085.

## Existing host transport and comparison fixture

Read-only discovery found existing WSL Mesa EGL, libGL shared dispatch, LLVM21
and software Vulkan libraries. `libGLESv2.so.2` and EGL/GLES development headers
are absent. Existing portable NDK Khronos headers supply the fixture declarations;
Mesa's shared libGL dispatch is the proposed real ES transport. Whether it serves
an actual ES context is unproven until the bounded fixture runs. No fallback to
a fake renderer or desktop-only GL claim is accepted: the fixture requires an
EGL_OPENGL_ES_API pbuffer, an OpenGL ES version string and llvmpipe renderer.

New files are fixture/report/helpers only. `prepare_swf_gpu_host_pixels_v38.py`
extracts the exact frozen V36/V37 source and verifies hashes; the production
worktree remains untouched. `run_swf_gpu_host_pixels_v38.py` compiles two separate
executables. Its default is compile-only. With a safe admitted host,
`--run-bounded` runs the independent binaries sequentially and compares 31 full
64x64 RGBA hashes. The actual production shader creation, SWF color conversion,
cache, GPU code and GL calls are used. Only disk AAsset transport and a counter
around the real driver's glGetError are explicit fixture adapters.

Cases cover empty/normal zero/visible draws, cxform raising zero alpha, transparent
line, transparent stencil mask/hit query, nested-mask pop, all seven supported
bitmap blend selections at alpha0/128/255, scene pane, actual error injection,
invalid transparent texture and actual EGL context recreation. A synthetic
320x180 workload measures three 100-frame batches after 20 warm frames: eight
visible HUD draws, a separate four-command transparent hurt movie and an empty
movie, finishing real driver work each frame. These timings would be synthetic
submission costs, never app FPS.

Before any child starts, the runner reads actual Windows memory through the
parent's existing read-only guard API. It reserves a 16 GiB commit margin and
6 GiB physical floor plus1 GiB for the fixture. A Linux process-group supervisor
monitors aggregate RSS, duration and a Windows cancellation marker; only its
own verified living process group can be stopped. The actual graphics binary
sets a hard1 GiB address-space limit, hard10 CPU seconds and20-second alarm
before EGL initialization. A seccomp filter rejects fork/vfork/non-thread clone
and forces clone3 to the checked thread-only fallback. Mesa software threads
share the capped address space; worker count is1 and shader disk cache is off.
The supervisor additionally limits runtime RSS to384 MiB and wall time to25
seconds. No Windows Job Object claim is made for the WSL VM itself.

Current status: helper syntax and frozen extraction PASS; **host compilation,
actual GLES context, 31-case pixel comparison and synthetic benchmark PENDING**.
Raw refusal is `headroom-refusal.json`. Do not repeatedly re-probe unchanged
headroom or relax these limits to get an execution result.

## Load-stage bounded-work findings

The read-only audit captured source hashes and exact lines in
`load-stage-source-evidence.json` and `world-load-navigation-proof-block.txt`.

1. **High-impact startup work:** `model_renderer.cpp:2752–2848` unconditionally
   executes development navigation proof loops whenever the floor graph is sewn.
   There are five all-floor-pairs sweeps (search, route, FindPath, motion,
   obstacle registry) and two per-floor proof sweeps (avoidance/producer). This
   is5*F^2 proof iterations plus linear work on every world load, with several
   searches allowing10,000 expansions and repeated route-cache clearing. These
   are diagnostic digest/log probes using synthetic objects, not the live player
   controller path. Move them into a separate bounded QA mode/fixture before
   chapter rollout; retain genuine graph construction and actual controller
   services. No duration or current F count is inferred without runtime evidence.
2. **Unbounded private-save read:** `RendererLevelFilesV25::saved` reads8192-byte
   chunks into an ever-growing vector until EOF. A validated application save
   size cap should reject before accumulating an arbitrary file; do not truncate
   or synthesize successful save restoration. This is distinct from cache ZIP
   member bounds and the16 MiB private-Lua VM policy.
3. **Resource bounds are per member, not whole-level peak:** ZIP mounting bounds
   directory to32 MiB/member to256 MiB and streams compressed input in64KiB
   chunks. The complete archive is not copied into memory. UI loose inputs and
   model loose texture reads have per-file limits, but decoded RGBA, CPU meshes,
   candidate/new+old level ownership and GPU buffers need an aggregate ledger
   and explicit stage budget before larger chapters. A per-file256 MiB bound is
   not a whole-world memory bound. Preserve original levels; fail with a named
   required stage instead of omitting source objects.

These are audit findings and proposed follow-up scope only. The loader session
still owns genuine Level.Init/LoadProcess/Unload and campaign current-Level
publication. The default Crypt C1 path remains intact; no phase38, count,
level-ready flag or menu source state was forced by this work.
