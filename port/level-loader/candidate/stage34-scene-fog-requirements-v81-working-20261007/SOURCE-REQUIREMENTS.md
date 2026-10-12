# Stage34 original Scene update and App58 producer

Source-only source recovery and current-API comparison. No execution, testing, compiler, hashes or proof packets. V80 is adopted into active private loader headers, selected controller and private renderer inc. Shared Root remains unchanged. The private checkout has no current source_campaign_runtime_v61.hpp/.cpp; its integration.patch must be applied by Main to the real current Root pair, preserving parent float-field changes.

## Scene virtual60: exact receiver/method

Level._LoadProcess3f6bcc..3f6bf0 loads actual Application10.device1c.SceneManager, sets r1=0 and r2=0, and invokes virtual60 before cached Camera128.virtual10.

SceneManager C1 352cbc..352cc8 stores `_ZTV12SceneManager` (95cd78) +1c as the address point. Therefore virtual60 is symbol+7c, not symbol+60: exact target58b9f0 is `glitch::scene::CSceneManager::update(float,bool)`. Arguments are 0.0f,false. Neither drawInit nor setupCamera is this call.

Original58b9f0:

1. Compare dt against -123456.0f (sentinel c7f12000), using __aeabi_fcmpeq. For ordinary0, add to SAME float time254 using __aeabi_fadd, store254 BEFORE subsequent callbacks, then __fixunssfsi gives timer. Sentinel branch obtains original Timer.getTime, __aeabi_ui2f stores254, then converts; not reached by Stage34.
2. Fresh hierarchy_dirty288; if true call collectAllNodes58b7ac before choosing mode.
3. false mode loads SAME root node4 and delivers its virtual14, exact ISceneNode.onAnimate596d6c, with timer. True mode instead traverses actual collected vector27c/280 and calls each virtual18/onUpdateTime.

CSceneManager C2 58de64 sets r8=0, 58dff8 stores that actual float254; 58de88 sets sl=1, 58dffc initializes dirty288=1. Original root onAnimate honors flags400/1 and200, delivers actual animator listfc then virtualb8 updateAbsolutePosition(false), recursively follows actual child listf4 using virtual14, then clears own flag20. It does not deduplicate by native draw epoch.

Current Root GameObjectSceneRootRegistryV1 provides `source_scene_phase_v69(epoch,timer)` with a flat collected-root/epoch journal. Current Root `advance_source_scene_v69` additionally requires loading_complete and adds FX scene delivery. It has no time254/source update(float,bool)/same manager root4 method. These methods are not a faithful Stage34 substitution. Do not call gameplay frame advancement, registration, drawInit, rendering or Camera update to fill this leaf.

Main requirement: expose the recovered update body on SAME existing manager/root graph and float254 source storage, reusing its genuine onAnimate traversal/animators/cache primitives. Stage34's current `scene_virtual60` callback receives zero argument words; its real provider must deliver update(0.0f,false) to this actual receiver and retain source failure prefixes. No second Scene/root or fake success. The existing V80 scene1c borrower already returns SAME registered roots, so no new registry is needed.

## App58 is first PlayerLightTweaker, not SceneManager fog storage

Application.PostInit32f90c..32f9a8 loops FOUR times. Each iteration allocates0x144, constructs PlayerLightTweaker C1 32f2b4, publishes at actual App58/5c/60/64, and assigns the corresponding genuine tweaker name into its stringf8. GamepadTweaker is separately constructed and published at68 afterward.

PlayerLightTweaker32f2b4 uses CTweakableC2 on SAME actual App10.device, registers actual variables, and loads sprintf `%s/data/tweaker/player_light.tweaker_xml` through CTweakable.loadXML32b6b8. This constructor/debug/table production must not be replaced by a copied Stage34 fog struct.

The exact first PlayerLightTweaker mutable fields consumed by Stage34 are:

- four contiguous color bytes f4: registered32f628 as `m_fogColor` (SColor).
- float110: registered32f63c as `m_fogStart`.
- float114: registered32f650 as `m_fogEnd`.
- float11c/120/124: registered32f664/678/68c as `m_fogDirectionMask.X/Y/Z`.

Direction stores actual0 at32f350/354/358. Colorf4 and start110/end114 have no scalar zero/default stores in this C1; their actual variable production remains authoritative; V82 recovered that constructor loadXML only walks group metadata, without assigning registered-cell values. Do not fabricate default initialized values. Stage34 overwrites these consumed fields in its existing source order after its camera delta stores. The already typed Stage34FogParamsV1 lends exactly color_f4, start110, end114, direction11c plus real owner/identity.

Current Root level-world has no PlayerLightTweaker owner/accessor; ApplicationServicesOwnerV5 has no App58 typed owner. Its SAME scene's SceneManagerFrameFieldsV67 fog432/material fog_color/fog_start_end and fog_transform458 are distinct actual source cells. Do not alias them as App58. Main requirement: retain first actual PlayerLightTweaker through the existing Application lifetime and expose its exact field lease to incoming Stage34.fog_params58; preserve actual constructor/registration/XML group metadata and native teardown; XML values are not constructor defaults. This is a reached resource facet, not a whole-stage or rendering algorithm callback.

Parent's paired constructor19c/1a0/1a4 float correction and update_camera_fog direct float consumption remain the separate already owned dependency. This source finding changes neither camera nor rendering algorithms.
