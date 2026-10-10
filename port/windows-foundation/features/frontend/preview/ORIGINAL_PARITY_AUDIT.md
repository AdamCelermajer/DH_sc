# Original reference and capture audit

Viewed the supplied original screenshots at the exact Temp paths
codex-clipboard-cd4abf24-31d5-45e1-843f-57fc5947883a.png (main) and
codex-clipboard-1428e82b-433b-4258-95e0-363d0a1cc049.png (class). The class
reference is original footage timestamp1:20: Rogue in a low foreground showcase
pose, Knight left, Mage right, seated statue centered, all three actors visible.
The reconstructed Android image is not the acceptance reference.

Original IDA source used directly from the existing original ELF export:

- Show428f38: reset previous class=-1; start lol_1_idle; camera local(0,0,-200);
  create all three Player_CS actors; assign MenuIdle; copy each dummy's position,
  rotation and scale after mesh/property/equipment initialization.
- Update428498: first selection and each changed selection assign selected
  CharAnimTable+96(MenuOnSelect), flag1; other actors +92(MenuIdle), flag0;
  use destination-specific scene transitions. Same selection after scene clip
  completes selects the corresponding authored lol_*_idle.
- SM_SetAnimState3c1aa8 stores state/flags and enters actual CSAnim14.
- RenderClassSelectPane428b74 sets viewport to the authored class_select SWF
  absolute bounds converted with invPixelScaleX/Y, submits SceneManager, restores
  the prior viewport. Host UI/viewport integration must follow this boundary.
- Collada camera constructor6e5550 converts authored horizontal FOV using
  serialized aspect. Perspective retains the base camera's literal4/3 aspect;
  it does not use viewport width/height or assign serialized aspect as a
  perspective ratio. Orthographic has a separate setAspectRatio branch.
  Show overrides up/near/far/local position. See CAMERA_ASPECT_PROOF.md.

Actual cache CLASS_SELECTION scene has no dummy_gameobject_* nodes. The seated
statue and trees are inline geometry, so this exact optimized asset does not
require inventing external AnimatedDecor owners. Actual dummy scales are1,1,1.
Positions: Knight(-354.095,-404.289,0), Rogue(3.2527,-616.974,0),
Mage(364.563,-413.023,0). This confirms class actor submission must not multiply
property-derived .9/.9/1 scales after the dummy transform.

Actual animation resources and ranges (milliseconds):

| Class | MenuIdle | MenuOnSelect | Showcase duration |
| --- | --- | --- | --- |
| Knight | prince_menu_idle_knight.bdae0..3000 | prince_menu_idle_knight_02.bdae0..5000 | 5000 |
| Rogue | prince_menu_idle_rogue.bdae0..3000 | prince_menu_idle_rogue_02.bdae0..5000 | 5000 |
| Mage | prince_menu_idle_mage.bdae0..1500 | prince_menu_idle_mage_02.bdae0..5366 | 5366 |

All live under original data/3D/characters/prince/animations/, resolved through
actual animation dictionary/state records. Scene ranges:1to2=0..1299,
2to3=1333..2633,3to2=2666..4000,2to1=4033..5333,
idle1=6000..6033,idle2=6333..6366,idle3=6666..6699.

Capture each class at elapsed0,250,650,1300,1600,2500,4000,5000,5366,6000ms, using the same
dt for scene and body. Use source selected_clip()/sampled_milliseconds() and body
animation_name()/animation_elapsed_seconds() in capture metadata. Scene source
samples cursor before increment; to capture a requested exact elapsed point,
advance with dt then sample zero delta. timing_probe.cpp records these actual
poses and resources for all three classes. The original scene centers Rogue
after its camera transition: eye(0,-1354.82,175.21), target(0,-1255.07,182.185).
Compare original footage1:20 to the Rogue1600/2500/4000ms captures to identify
the same body-animation pose rather than comparing an arbitrary idle frame.
Verify statue center, Knight left/Mage right, foreground body/weapon pose and
the complete authored scene. Use the same physical viewport/aspect as the
original reference, and compare UI overlay registration separately.

Material-owner boundary recovered from original source:

- Main skybox/solid ranges bind embedded COMMON unlit, color1,1,1,1, pass blend
  disabled(ONE,ZERO), depth test/write true, LEQUAL515, BACK1029, CCW2305.
- Main foliage Material__11805 binds GL_Diffuse_L1_VC_iPhone.bdae,
  L1_Vc_Al_----_----_----_----, env_darkwoods_alpha.tga and
  pvr2_env_darkwoods_alpha_alpha.tga; source alphaRef0, color1,1,1,1.
- Original GL_Diffuse_L1_iPhone_VS.glsl computes
  (Light0Ambient.rgb+Light0Diffuse.rgb*nDotVP*attenuation)*vertexRGB,
  with output vertex alpha1. FS replaces diffuse alpha with AlphaSampler.z
  for AL/AT and AT discards alpha<.8. No artistic brightness multiplier exists.
- Original commitLightParameter5b187c performs the light-semantic upload ONLY
  when the actual CLight pointer is nonnull. A null pointer makes no uniform
  writes, preserving prior program uniform state. NULL is not a white-light
  or zero-light substitution. Actual program/material/light lifetime and prior
  uniforms must be connected before L1 appearance can be claimed exact.

Current helper repairs COMMON pass/unlit admission; external L1 uniform owner,
original lighting, and GPU/reference image acceptance remain unverified.
The speculative post-showcase MenuIdle fallback has been removed. Original
CSAnim event34 with flag1 invokes SM_SetIdleState, whose actual equipment-dependent
ordinary Idle owner must be supplied through set_idle_transition_provider.
Without that provider, each actor holds its exact source OnSelect endpoint:
Knight5000ms, Rogue5000ms, Mage5366ms. idle_transition_required() is true and
idle_transition_status() reports source-showcase-end-held-idle-owner-required.
Body clock and geometry/sockets stay held while other scene/UI owners can update.
A source selection change explicitly restarts the new showcase and assigns
the other actors source MenuIdle, clearing the old pending boundary.
Provider failure propagates its error and preserves the required boundary;
successful delivery is recorded as native-idle-provider-delivered. The recording
provider fixtures test the transport boundary, not native ordinary Idle behavior.
No original pixel parity or complete native FSM claim follows from asset tests.
