# SAME Module local visibility V94 — source only

Candidate: C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/module-local-visibility-v94-working-20261007

`module-local-visibility-v94.patch` contains narrow additions against current Root Module graph/retained visual plus the coherent pending V93 static-node field owner. Apply that delta preserving current unrelated Module/graph/cleanup changes; do not replace whole stale Root sources. No new TU, target, World, Scene, C1/visual, decoder, clock or animation provider is introduced.

Exact typed Main callback:

`CanonicalModuleGraphV3::source_sync_visibility_v94(uintptr_t actual_actor_identity, string& error)`

Call this from the SAME source-world's existing Module graph for Module seven-byte OBJS restore/native SyncVisibility. It pins the actual weak record throughout delivery, rereads its SAME base2d8, resolves only the existing retained Module-owned visual, and calls its genuine SyncVisibility. A genuinely NULL2d8 takes the original successful return. Other positive/unpublished receivers fail explicitly. It never queries V69's non-Character generic visual directory or creates another receiver.

Source bodies implemented:

- `RetainedModuleVisualV3::sync_visibility_v94`: original VisualObject.SyncVisibility4713d0 reads actual produced base.visible80. False calls local SetVisible(false). True calls actual Module vtablec4, which is GameObject.IsZonable3883b8 (byte2ed XOR1), then reads only reached zoning2ee and entered2f0. A zonable+zoning+not-entered object hides; other reached branches request true. No guessed static/IsBreakable policy or default field value.
- `RetainedModuleVisualV3::set_root_local_visibility_v94`: original VisualObject.SetVisible471368 returns for real NULL root, otherwise compares requested bool to effective root.flags11c bit1. On a difference it calls SAME actual SceneManager.ForceRegister BEFORE original Root virtual48. Parent visibility is not substituted.
- `ModuleStaticSceneV2::set_root_local_visibility_v94`: original Root.SetVisible35c26c stores209 on every call. True returns with a queued local request. False invokes qualified ISceneNode.SetVisible596ec4 against SAME local120/flags and propagates effective visibility through actual attached children.
- `commit_root_local_visibility_v94`: original Root.onAnimate35d168 prefix reads209 before hidden400/onAnimate200 early-outs. For queued true+currently hidden it writes qualified local120/effective flag, calls actual Scene.notifyVisibilityChanged5890a8, THEN clears209. Already visible or no request keeps original early-return fields; it does not invent a209 clear. Failed native notification retains a sticky failure prefix and the source request/pins.
- The existing registered Module `scene_phase_v69` callback invokes this source prefix BEFORE its existing hidden/200 checks. It uses the same live root and weak SAME Scene owner. Existing counter/cache/static animator controls remain after that prefix. It adds no extra sample/frame/OnAnimate call.

Storage: root actual120/121 +209 have separate local/request/parent meanings. Source selected-node loader populates local120 and parent121 from actual SNode visibility words rather than deriving local visibility from a flattened effective mask. Module node/mesh propagation writes those SAME V91/V93 cells and current effective masks, honoring actual detach membership so mapped minimap resources are never re-hidden by an old Module root. Original C1 root local/parent defaults1 and request209=0 are preserved.

Integration dependencies: coherent V91/V93 scene field ownership and ModuleStaticScene source pair must accompany this additive patch. No CMake TU addition is needed. Main consumes the public typed graph method in its Module saved-object/native callback; Main's actual Scene owner delivers the existing qualified root onAnimate path. Optimized Scene virtual18 is not substituted for original Root.onAnimate14 or supplemented with an invented second visibility clock.

No tests, compilation, syntax/build/apply-checks, source audit jobs, hashes, fixture changes, emulator or native jobs ran. Source adoption and Main's BIG integrated milestone remain pending.


Parent integration additions: module-visibility-public-export-v94.patch adds the actual typed runtime helper to source_campaign_module_rooms_v91.hpp; module-visibility-runtime-export-v94.inc supplies its body beside the existing actual ModuleGraph borrowers (already appended in the V91 candidate INC). It validates/pins the SAME current actual Module record and invokes the existing actual graph.source_sync_visibility_v94, without exposing a second graph or forwarding through V69 visuals. module-visibility-save-invoke-v94.patch connects the real native4713d0 request in current NonCharacterSaveDispatch to that export for Module/Block only, preserving incoming genuine overrides and all other classes. Apply after Module seven-byte/typed metadata patches and the V94 owner additions. Source-only; no runtime acceptance claim.
