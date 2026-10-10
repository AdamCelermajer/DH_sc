The executor borrows `CharacterMeshFxOwnerV4`; the manager remains the authority
for authored selection, redirects, random selection, resource pools, source
timelines, particles, completion callbacks and warm reuse. Construct it with the
campaign's `VisualFxManagerLibrariesV63`, same live actor scene, actual resource
reader, source debug/anchor/floor services and the retained resource factory.
`CharacterAuthoredResourceFactoryV32` supports the recovered authored resource
domains used by the host test. Keep every borrowed provider alive until the
manager is quiescent and destroyed.

Feed detached retained marker batches through `marker` using the real actor,
clip identity and `(generation, cycle, index)` token. It routes only the original
`fx_` event prefix; the manager performs the original ordered table-name lookup.
It commits the token before callbacks, including failed source prefixes. Other
marker names remain available for audio/gameplay consumers. Reentry therefore
cannot replay the same effect. `release_actor` removes that actor's delivery
tokens and detaches its actual actor/socket anchors while allowing original
effects to finish with their last submitted transform.

Feed the actual selected `AnimationStart.step` through `phase` with its complete
source path and retained phase occurrence. This calls the recovered
`character_animation_step_fx_v2` kernel. Swoosh rows require the whole original
equipment gate; it must not be guessed from a weapon model.
The source Swoosh prefix can itself play equipment sound and FX and returns
both fallback decisions. Run it once at the retained phase; this callback may
return its already computed FX decision while audio consumes its sound decision.
Explicit original
skill, weapon, fairy or result producers can call `play_set` with their selected
numeric set and actual attachment policy. A missing socket rejects. The adapter
does not derive effect IDs from actor names or maps.

Call `scene_phase(source_absolute_ms, source_app_dt)` and then
`manager_phase(source_app_dt)` at their original phases. `draw_sources` returns
retained mesh/particle borrows without advancing anything. The renderer must
apply the actual SceneManager order and source materials/texture transforms.
Do not replace them with generated quads, colors or particle sprites.

Run `run_tests.py` with the local Python runtime. It SHA-checks original asset
receipts, builds current source kernels with ASan/UBSan through WSL, and writes
`reports/feature-effects.json`. Anchor and camera services in this test are
declared fixtures; the report does not establish live Windows rendering.

`EffectsRenderBridge` converts retained source mesh/skin and particle billboard
streams into root `Mesh`/`Mat4` packets. UVs are already baked by the original
resource receiver and are never transformed again. Its material callback receives
the real BRES and animated material; its submit callback receives a shared frame
loan. Keep that loan through queue flush and drain it before destroying/reloading
the source Scene or manager. The frame pins decoded geometry/material snapshots;
live Scene/texture-matrix pointers still borrow their original owner.

`OriginalEffectMaterialBinding` resolves actual selected COMMON unlit pass state,
loads original texture pixels with the existing decoder and uses root upload/
release callbacks. Unsupported shaders, lighting owners, separate alpha maps and
unsupported richer pass state reject. `EffectsAnchorBinding` reads actual actor/
socket fields each query and forwards release to the same manager. Interruption
does not invent a clip-local effect stop; death/unload follows source lifetimes.

`RetainedEffectsAdapter` consumes the same retained audiovisual marker batch
after actor event0x28 and original forwarding. Its token retains actor, clip,
slot, generation, wall timestamp, lag and batch occurrence ordinal; subsequent
loop/frame events in the same generation are not accidentally suppressed.

Faery glow content is embedded in original `faeries_02_celeste.bdae`: the rigid
eight-vertex/twelve-index glow instance uses `fx_particles_additive` and
`atlas_fx_particles_001.tga`. The native scene-borrow test proves that enabling
the original rigid instances uses the same `CharacterVisual` pose/Scene. Three
embedded cloud nodes remain a required same-actor particle runtime continuation.
No separate EffectsTables `FaeryGlow` set has been established. Instantiating the
whole actor as a separate FX resource would create a second scene/pose and is not
an acceptable continuation.

`run_tests.py --render` validates source packet/UV/skin/texture/pass conversion
and emits a diagnostic binary fixture for `effects_native_renderer_tests.cpp`.
That native WGL test exercises root Renderer draw/upload and framebuffer changes
on the actual source streams, with diagnostic camera framing. It does not prove
campaign integration or original scene-wide draw ordering. The native
`effects_scene_borrow_tests.cpp` exercises the real retained actor scene, posed
hand attachment, clock isolation, release and original rigid faery glow.
