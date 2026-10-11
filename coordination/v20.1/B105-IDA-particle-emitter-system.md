# B101/B105 — IDA reverse engineering of the original particle (PCloud) emitter system (helper report, saved by root)

Evidence: `pseudocode-all.c`, quoted `addr name`. **[R]** = read in code, **[I]** = inferred. The helper edited no repo files; scratch dumps and `vt.py` (vtable decoder) are in `C:/Users/adamc/Desktop/workspace/DH_wt/fixfx_work/ida/`. See also `B101-IDA-door-fire-mechanism.md` (door class + fire door model decode) — the two reports agree.

## 0. Answer to the box-axis question
- Shape block -> parameters [R] (`00656450 CParticleSystemSceneNode::initParticleSystem`, EmitterType case 0): `RadiusLength = shape[0]`, `Width = shape[1]`, `Height = shape[2]`.
- Parameters -> box [R] (`0064e750 PEmitterModel::initPEmitterModel` case 0): `PDBox(Width, Height, RadiusLength)`.
- Box constructor [R] (`0069b21c PDBox::PDBox(float a2,a3,a4)`): sizes at +44/+48/+52; corners min=(-a2/2,-a3/2,-a4/2), max=(+a2/2,+a3/2,+a4/2); edge vectors (a2,0,0),(0,a3,0),(0,0,a4).
- **Mapping: local X = Width, local Y = Height, local Z = RadiusLength; centred on the node origin.** For GL_PCloud01 (RadiusLength 1, Width 1, Height 862.5) the box is 1 x 862.5 x 1: a LINE 862.5 long along the emitter node's local Y.
- `0069efac PDBox::transform` [R]: every frame rebuilds min/max and edge vectors from the stored sizes, then multiplies by the FULL 4x4 world matrix (rotation, scale, translation): node scale stretches the box. `0069b534 PDBox::generate` [R]: pos = min + r1*edgeX + r2*edgeY + r3*edgeZ, r in [0,1): uniform in the transformed box.
- Which matrix [R]: `0064dac0 CParticleSystemSceneNode::onAnimate` l.39 sets the system's world matrix (ps+12) from `this[59]->vfunc56()`; `this[59]` is the scene-node parent (`00597290 ISceneNode::getParent`); vfunc56 = `getAbsoluteTransformation` (vtable +0x54 -> `0035a8e0`, returns `this+36`). In `0061b2f4 CColladaDatabase::constructNode`, an instance_emitter (case 9) is added via `addChild` (vfunc92) under the node that owns it, which receives the node's position/rotation quaternion/scale (vfuncs 164/156/148). The particle node itself has an identity transform (`00667a98` ctor: position 0, unit quaternion, scale 1). Effective matrix = absolute transform of GL_PCloud01-node = root * `_bone_fx_doorfire-node` (t=-435.38,0,0) * GL_PCloud01-node (t=324.2,34.05,252.82, identity rotation), including any scale up the chain.
- `CParticleBoxEmitter` / `createBoxEmitter` (`006c22dc`) are Irrlicht's own and NOT on the COLLADA path.

## 1. Resource records and construction
Call chain [R]: `0061b2f4 constructNode` (case 9) -> `0061a888 constructEmitter(SInstanceEmitter*)` (resolves URL `instance[1]+1`, force names at `instance+24`, materials from `instance[4]`, stride 60, material ref at +8) -> `0060e79c constructEmitter(SEmitter*)` -> factory vfunc104 = `00634c64 CColladaFactory::createParticleSystem` (`new(0x198)`, `0064ed94 CParticleSystemSceneNode` C1 stores `this[99]=SEmitter*`, then vfunc244 = `initParticleSystem(driver, billboard)`). Back in constructNode: vfunc212 (setRenderingLayer), vfunc260 = `0064fd60 init`, `addChild`.
Record lookup [R]: `0060e468 getEmitter` = `lib+124 + 144*i` (stride 0x90); `0060e484 getGNPSEmitter` = `lib+132 + 232*i` (stride 0xe8); `0061a7f4` matches by string compare on +0.

**SEmitter (0x90)** [R unless noted] (read in `initParticleSystem 00656450` "IPS" and factory `00634c64`):
| Off | Type | Meaning | Read at |
|---|---|---|---|
| +0x00 | char* | id (e.g. `GL_PCloud01-emitter`), returned by `getUID` (`0064bb78`) | C1, getUID |
| +0x04 | ? | not read on this path | - |
| +0x08 | int | EmitterType: 0 box, 1 sphere, 2 cylinder | IPS l.118 |
| +0x0C | float* | shape block: box [RadiusLength, Width, Height]; sphere [Radius]; cylinder [Radius, Height] | IPS l.125-154 |
| +0x10,+0x14 | ? | not read | - |
| +0x18 | int | MaxParticles (0 = no cap) | l.160 |
| +0x1C | ? | not read (GNPS has a variation field here) | - |
| +0x20 | float | BirthRate (/s) | l.164 |
| +0x24 | ? | not read | - |
| +0x28 | float | Life (s) | l.168 |
| +0x2C | float | LifeVariation (ABSOLUTE seconds) | l.172 |
| +0x30 | float | TargetSize (full quad edge) | l.176 |
| +0x34 | float | SizeVariation (relative) | l.180 |
| +0x38 | float | SizeGrowthTime | l.184 |
| +0x3C | float | SizeFadeTime | l.188 |
| +0x40 | float | Speed | l.192 |
| +0x44 | float | SpeedVariation (relative) | l.196 |
| +0x48 | int | direction mode: 0 Direction=(0,0,0); 1 dir block = vec3 + variation at [3]; 2 only DirectionVariation = block[1] | l.199-232 |
| +0x4C | float* | direction block | same |
| +0x50 | int | render kind: 0 primitive (see +0x54), 2 instance geometry | factory |
| +0x54 | int* | render block | factory |
| +0x58 | ? | not read | - |
| +0x5C | float | AnimOffset | l.278 |
| +0x60 | float | AnimOffsetVariation (relative) | l.282 |
| +0x64 | int | AnimKeyMappingType (0 or 1) | l.233-275 |
| +0x68 | float | AnimLength | l.286 |
| +0x6C | float | AnimLengthVariation (relative) | l.290 |
| +0x70 | float | AnimScaleMultiplier | l.294 |
| +0x74 | float | AnimScaleMultiplierVariation (stored, unused by update) | l.298 |
| +0x78 | float | SpinTime (s per revolution) | l.302 |
| +0x7C | float | SpinVariation (relative) | l.306 |
| +0x80 | float | SpinPhase (radians) | l.310 |
| +0x84 | float | SpinPhaseVariation (relative) | l.314 |
| +0x88 | int | SpinAxisType: 0 random, 1 fixed, 2 = velocity | l.318 |
| +0x8C | float* | spin-axis block (type 1): vec3 + variation [3] | l.321-347 |

Render block at +0x54 when +0x50==0 [R] (`00634c64`): word [0] = shape: 1 unit cube mesh; 2 three crossed unit quads (24 vertices); **3 or 4 camera-facing billboards** (`initParticleSystem(driver,true)`); 7 sphere mesh. Billboard sub-fields: [1] flags, [2] mode, [3..5] axis0, [6..8] axis1; mode 1: flag 2 -> `lockAxis(0,axis0)`, flag 4 -> `lockAxis(1,axis1)`; mode 2: `lockAxis(1,axis1)`; mode 3: context bytes [5]=1,[4]=0,[32]=1 -> velocity-aligned. `006309ac lockAxis` sets byte `ctx+4+i`, stores 0.5*normalize(axis). When +0x50==2 geometry comes from URL `*(*(blk+4)+4)+1`: particles become mesh instances. **GL_PCloud01 must have [0]=3 or 4 to render as billboards** (if it uses the `PSGeneric*` bakers everything in section 5 changes): CHECK IN THE DATA.

**SGNPSEmitter (0xe8)** [R] (`00642e58 CGlitchNewParticleSystemSceneNode::initParticleSystem`): +8 EmitterType, +12 shape block, +16 BirthRate, +20 BirthRateVariation, +24 MaxParticles, +28 MaxParticlesVariation, +32/+36 Life/LifeVariation, +48/+52 TargetSize/SizeVariation, +56/+60(byte) SizeScale/OverLife, +64/+68 Speed/SpeedVariation, +72/+76(byte) SpeedScale/OverLife, +80..88 Direction, +92..100 DirectionVariation, +104/+105 (bytes) StartAngleType/SpinKeepAlignedToMotion, +108/+112 SpinPhase/Variation, +116/+120 SpinTime/Variation, +124/+128(byte) SpinScale/OverLife, +132(byte) SpinAxisType, +136..144 SpinAxis, +148..156 SpinAxisVariation, +164..172 SpinPivotPoint, +180/+184 MotionRandom/Variation, +188/+192(byte) MotionRandomScale/OverLife, +196/+198 TextureSubDiviseH and V (decompiler shows +196 for both; V likely +197) / TextureSubDiviseRandomPick. Only GNPS has atlas subdivision; the PCloud SEmitter path has NO atlas.

**Particle system object** [R]: `00655314 PSManager::createPCloudSystem(bool billboard)` builds a Mixin: billboards = `PRenderDataBillboardModel` with `PSBillboard*` bakers; otherwise `PSGeneric*` bakers. Constructor `00654a74`: models at Mixin +16 Generation, +36 Size, +56 Color, +96 Emitter, +120 Motion, +148 Forces, +168 Spin, +208 Life, +220 RenderData; shared context at +384; RNG state +4 and seed +8 both start at **123456789**. Parameter slots: Generation (`006545d0`): BirthRate +4, MaxParticles +8, dt +12, accumulator +16. Size (`006543f8`): TargetSize +4, SizeVariation +8, SizeGrowthTime +12, SizeFadeTime +16. Color (`00654690`): TextureTransformTrack +4, DiffuseColorTrack +8, AnimKeyMappingType +12, AnimOffset +16, AnimOffsetVariation +20, AnimLength +24, AnimLengthVariation +28, AnimScaleMultiplier +32, AnimScaleMultiplierVariation +36. Emitter (`006548fc`): domain +4, EmitterType +8, RadiusLength +12, Width +16, Height +20. Motion (`006540ec`): Direction +4, DirectionVariation +16, Speed +20, SpeedVariation +24. Spin (`00654218`): SpinTime +4, SpinVariation +8, SpinPhase +12, SpinPhaseVariation +16, SpinAxis +20, SpinAxisVariation +32, SpinAxisType +36. Life (`0065452c`): Life +4, LifeVariation +8. Context (`0064d1d0`): +36/+40 particle vector, +48 parameter map, +72 time, +76 previous time, +80 dt, **+84 local-space flag (0)**, +88 AnimationDatabase.
**SParticle (100 bytes)** [R]: +0 position, +12 velocity, +24 SColor, +28..48 texture 2x3 matrix, +52 anim offset, +56 anim length, +60 age, +64 life, +68 current size, +72 base size, +76 spin time, +80 spin angle, +84..92 spin axis, +96 squared distance to camera (sort). Defaults from the resize template in `006532cc`.

## 2. Emission
Per-frame update [R] (`006532cc IParticleSystem<SParticle>::update(t, colour)`; vtable 0x97fc80): (1) if `t < previous` call `init()` (full reset, l.39-44), then `dt = t - previous` (l.45-48); (2) `generateParticles`; (3) on NEW particles only, in order: initPLife, initPColor, initPSize, initPPosition, initPForces, initPMotion, initPSpin, initPRenderData (empty); (4) `applyPLife` on ALL particles incl. new; (5) partition out the dead (alive = 0 <= age < life, l.95-106), resize; (6) on survivors: applyPColor(colour), applyPSize, applyPForces, applyPMotion, applyPSpin, applyPRenderData.
Count [R] (`0065317c generateParticles`): `x = dt*BirthRate + accumulator; n = (int)x; accumulator = x - n`; new total = `min(old + n, MaxParticles)` (MaxParticles==0: no cap); the accumulator keeps advancing when the cap is hit, so excess births are lost.
Position [R] (`0064c08c initPPosition`): if context+84==0 and a world matrix exists, `domain->transform(world)` once per update; each new particle `pos = domain->generate(rng)`. Sphere (`0069c6ec`): direction by rejection inside a cube, normalised; radius = inner + rand*(outer-inner), inner 0 (not volume-uniform); `0069bdec PDSphere::transform` = translation only. Cylinder (`0069e1bc`, `PDCylinder(Height,Radius)`): base + rand*axis(0,H,0) + r*(cos t*u + sin t*v), r linear in rand; its transform (`0069e69c`) not dumped.
Velocity [R] (`00653bb8 initPMotion`): copies the world matrix (identity if none); `speed = Speed + Speed*SpeedVar*(rand-0.5)`; Direction==0: dir = normalize(randvec-0.5) (not transformed); else if DirectionVariation>0 with `s = DirVar*180`: `rotateXYBy`, `rotateYZBy`, `rotateXZBy`, each by `U(-s/2, s/2)` (degrees per Irrlicht API [I]); dir then multiplied by the world 3x3 (`x*M0 + y*M4 + z*M8`...) and normalised; velocity = speed * dir_world. Node scale does not change speed; rotation does. GL_PCloud01: velocity 225-375 along +Z of the parent chain.
Life [R] (`0064c2fc initPLife`): `life = Life + LifeVar*(rand-0.5)` (absolute), `age = 0`; if `life < dt` and `dt < 4*life` then `life = 1.5*dt`.
Size [R] (`0064bd1c initPSize`): `base = TargetSize*(1 + SizeVar*(rand-0.5))`; if SizeGrowthTime>0 current size starts 0 else base.
Spin [R] (`0064e3c8 initPSpin`): `spinTime = SpinTime*(1 + SpinVar*(rand-0.5))`; `angle0 = SpinPhase*(1 + SpinPhaseVar*(rand-0.5))`; axis by type: 0 normalize(randvec-0.5); 1 SpinAxis jittered with the same x180 XY/YZ/XZ scheme; 2 particle velocity.
RNG [R] (`0062fe78 PSRandom::Rand`): Park-Miller (48271, Schrage 44488/3399), returns `s / 2147483650.0` in [0,1). Every system starts from seed 123456789 and `init()` resets it (`0064bbe0`: `a1[1] = a1[2]`): all emitters produce identical random sequences. A draw is SKIPPED whenever its variation is 0 (matters for bit-exact reproduction).

## 3. Per-particle update
| Step | Function | Formula [R] |
|---|---|---|
| Age | `0064c3f4 applyPLife` | `age += dt` (newborns aged by dt in their first frame) |
| Death | `006532cc` l.95 | removed when `age >= life` or `age < 0` |
| Position | `0064c190 applyPMotion` | `pos += vel*dt` (explicit Euler, constant velocity) |
| Forces | `0064d948 applyPForces` | only forces bound by `0064f6f8 attach()` |
| Size | `0064be08 applyPSize` | below |
| Spin | `0064c25c applyPSpin` | `angle += (2*pi / spinTime)*dt`; 0 if spinTime is 0 |
Forces: `attach()` looks up nodes by UID from the force-name list (node+336, from SInstanceEmitter+24): Gravity, Wind, Deflector (`006338d8`, `00633360`, `00634518`). With no force list there is NO gravity.
Size formula: `size = base; if growth>0 and age<growth: size = base*age/growth; if fade>0 and (life-age)<fade: size = base*(life-age)/fade` (fade overrides growth). GL_PCloud01 (growth 0.067, fade 1.267, life 1.667): 0 -> 500 by age 0.067, holds 500 until age 0.4, then linear to 0 at 1.667.
Colour and AnimLength (`0064e830 applyPColor`): no track [R]: particle colour = the colour argument; `onAnimate` (l.42-58) reads the material colour only if `driver->getDriverType() & 7` is nonzero; the only driver in the binary is `COpenGLES2Driver::getDriverType` (`005aefa4`) = 8, and 8&7 = 0 => **vertex colour is white 0xFFFFFFFF**; the material colour stays in the material. Track selection [R] (`0064fd60 init`): per material the first colour parameter (`getParameterID(renderer,6,0)`): DiffuseColorTrack = `getAnimation(db, matId, 86, paramName)`; fallback 1 `getAnimation(db, matId, 25, 255)`; fallback 2 `getAnimation(db, matId, 86, "__irrlicht_Diffuse_color")`; TextureTransformTrack = `getAnimation(db, matId, 256, 255)`; the animation database is the block for clip 0 (or default clip) from `CAnimationStreamingManager`; lookup only when a header field (`*(*(*(res+36)+32)+4)`) is zero (meaning unknown). Per-particle anim values [R] (`0064bf00 initPColor`): `animOffset_p = AnimOffset*(1 + AnimOffsetVar*(rand-0.5))`; `animLength_p = AnimLength*(1 + AnimLengthVar*(rand-0.5))`. Track time in ms [R]: MappingType==1: `t = (age*animLength_p / life * AnimScaleMultiplier + animOffset_p)*1000`; else `t = (animOffset_p + age)*1000`. Sampling [R]: `getValue(..., true)` -> `006e2c40 CAnimationTrackEx::getValue`; `0066a1f0 findKeyFrameNoEx` binary search, clamps to first/last key (colour holds after track end), interpolated. **AnimLength is NOT atlas animation**: it is the time span of the material's type-86 colour track (and the texture-transform track) mapped over each particle's age. TextureTransformTrack [R] is sampled step-wise (flag 0) into the particle's 2x3 matrix, but the billboard UV baker is empty (`006c01d8 PSBillboardTexCoordsBaker::getPerParticleTexCoords`): no visible effect on PCloud billboards.

## 4. World vs local space
**World space [R]**: context+84 starts at 0 (`0064d1d0`); no writer found anywhere. With +84==0: positions are generated in world space via the parent's absolute matrix (section 0), then integrate independently (particles trail behind a moving emitter); `render` sets WORLD = identity (`006572b8`: `setTransform(1, IdentityMatrix)`); bounding-box translation skipped (`00651eec`). Translation, rotation and scale all apply to the emission box; rotation applies to velocity direction; scale does not affect speed (normalised); **sizes are never scaled by the node** (corners = size * camera vectors). (+84==1 would use translation-only world matrices: unused.)

## 5. Rendering
View matrix [R]: `render` copies `camera->vfunc324 + 132` into the context ViewMatrix (slot 324 = `getViewFrustum` `00582138` returns `this+360`; +132 -> `this+492` = `getViewMatrix` `00581f58`).
Billboard vectors [R] (`0064dcd8 PSBillboardPositionBaker::getPerParticleSystemPosition`): up = (V1,V5,V9); n = -(V2,V6,V10); default (no axis locked): `R = 0.5*normalize(up x n)`, `U = 0.5*normalize(up)`; lock rules use `W = U x R`: axis0 locked -> R = axis0, U = 0.5*normalize(R x W); axis1 locked -> U = axis1, R = 0.5*normalize(W x U); if context byte +33 is set the locked axes are first multiplied by the world 3x3.
Per particle [R] (`006c0238 getPerParticlePosition`): velocity-aligned mode (+32): U = 0.5*normalize(velocity), R = -0.5*normalize(n x U); if spin angle != 0 rotate R and U by `quaternion::fromAngleAxis(angle, +/-normalize(U x R))`; the axis is NEGATED when the particle's spin-axis x > 0 (random axes: about half spin each way).
Corners [R]: v0 = U - R, UV (0,0); v1 = -R - U, (0,1); v2 = R - U, (1,1); v3 = R + U, (1,0). Vertex = pos + Pivot (static 0) + size*corner (`006559b0 CParticleSystemBaker::bake`). R,U are half-vectors, so **the quad edge equals `size` (TargetSize is the full width)**. UVs = static table (`TexCoords` 0,0 / 0,1 / 1,1 / 1,0; shared quad mesh same, IPS l.414-425). Normal = n for all four vertices. Vertex colour = particle SColor (4 bytes copied, stream 18).
Indices [R]: static array at `0x99b85c` = {0,1,2,0,2,3}; shared `OutBillboardIndexBuffer` repeats it as `4k + {...}` up to the largest MaxParticles of any system (IPS l.437-503). Triangle list [I].
Sort [R]: `applyPRenderData` computes `d^2 = |pos - (V12,V13,V14)|^2` and sorts with `AlphaSort` descending (`00650618`) = far to near; note (V12,V13,V14) is the view matrix's translation column, not the eye position (engine quirk); the bounding box is built from particle centres only.
Material [R]: from instance bind_material via `00619820 addMaterial` -> `CRootSceneNode::addMaterial` (shared model material); `render` first calls `hasColorAnimationTrack`; if true, sets the material colour parameter to white; then `setMaterial(mat, mat->getTechnique())`. **Blend, z-write and cull are not touched by the particle code**: they come from the model's material technique/renderer [I] (for the fire door: additive ONE,ONE, depth write off, see the door report).
Pass [R]: `0064da1c onRegisterSceneNode` registers only when particle count > 0, as `registerNodeForRendering(this, mat, 0, pass 8, null, 0x7FFFFFFF)`; pass 8 in `0058f348` goes to the TRANSPARENT list (`STransparentNodeEntry`, distance-sorted) unless `sceneManager+650` is set. Culling uses `getTransformedBoundingBox` = world bbox of particle centres (`0064bbc0`), auto culling mode 2.

## 6. Animation channels and time
Binding [R] (`00667f18 ISceneNodeAnimator::forceBind`): channel types 0x1C (28) and 0x2F both target `getParticleSystemParameter("BirthRate")` (vfunc252 = `00651924`), a raw pointer to the Generation model's BirthRate (+4); the animator writes the float directly; the next `generateParticles` uses it. Node found via `CRootSceneNode::getParticleSystem(target)` (`0065b4e4`), case-insensitive compare against getUID (emitter id string). Other channel types: 0x1D TargetSize, 0x1E SizeVariation, 0x1F Speed, 0x20 SpeedVariation, 0x21 Direction, 0x22 DirectionVariation, 0x23 SpinTime, 0x24 SpinVariation, 0x25 SpinPhase, 0x26 SpinPhaseVariation, 0x27/0x28 SpinAxis/SpinAxisVariation, 0x29-0x2E AnimOffset, AnimOffsetVariation, AnimLength, AnimLengthVariation, AnimScaleMultiplier, AnimScaleMultiplierVariation; 0x30-0x44 (BirthRateVariation, Life, Size/Speed/SpinScale, RadiusLength/Width/Height, SpinPivotPoint...) look GNPS-oriented.
Two clocks: [R] the simulation runs on absolute scene time in ms: `onAnimate(t)` -> `update(t/1000)`, dt from the previous call. [I] BirthRate and other emitter parameters follow the owning model's animation clip time through that model's clip animator. Consequences: particles keep living/moving through clip loops, only the emission rate follows the clip; per-particle colour follows the particle's own age, not clip time; a reset happens only if the time passed to `onAnimate` goes backwards.

## 7. Other fidelity points
- No pre-warm or enable flag [R]: `init` zeroes the clocks (`0064bbe0`); the first `update(T)` has dt = T: it emits `min(T*rate, Max)` particles, all aged T, which die at once if T >= 4*life. Emission is gated only by BirthRate.
- Visibility [R]: `constructNode` calls `setVisible(SNode[13] != 0)` (vfunc72) on the containing node; `ISceneNode::onAnimate` (`00596d6c`) skips animators and children when the node's flags (+284) say invisible.
- The `GL_PCloud` name has no meaning in code beyond being the id used for animation targets (case-insensitive).
- NOT verified: the SColor channel order inside type-86 keys; the cylinder transform; whether the GL_PCloud01 render block is 3/4 (billboard) and its AnimKeyMappingType (+0x64) value: both must be read from the data.
