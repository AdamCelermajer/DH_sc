# B105 Celeste embedded effects — fresh session, 2026-10-11

Owner: /root/b105_celeste. Worktree `DH_wt/fix105`, branch `fix/b105`. Shared emitter owner: /root/b101_b105_effects. Independent reviewer pending root dispatch.

## Investigation before new behavior edits

Visual evidence: inspected user shot-22 (v20): flat white body/wing streaks, separate yellow floor-like glow. Reused and inspected original P1 `z_Zky7qQdYs` 288–293 s six-frame sheet `DH_wt/fixfx_work/ref/celeste-zoom-288-293.png`: warm white-gold glow follows the flying figure; separate small gold/white stars change location between frames. P2 `cIAW38IfLxY` inspected six-frame sequences at 350–352, 400–403, 440–443, 450–453, 470–473 and 510–513 s in `DH_wt/fix105_work/ref/`: first shows fire cinematic, later merchant/peasant conversations and map. Faery is mostly clipped out at the top in merchant views; these do not prove her continuous glow. P1 supplies the direct continuity evidence. No recolor inferred from HP, NPC name, or a still.

Logic evidence reused: root `B105-IDA-particle-emitter-system.md`, and shared `B101-B105-report.md`. `constructNode` 0x61b2f4 case9 -> `constructEmitter` 0x61a888 / 0x60e79c -> factory 0x634c64 -> `CParticleSystemSceneNode::initParticleSystem` 0x656450. The emitter is an identity child of its authored owning node. `onAnimate` 0x64dac0 consumes parent absolute transform; world-space particles retain old positions as the parent moves. `forceBind` 0x667f18 targets emitter UID, birthrate types28/47 and other emitter parameters. `init` 0x64fd60 binds material type86 color to per-particle track; `initPColor` 0x64bf00 / `applyPColor` 0x64e830 map particle age to material track. Visibility of containing node gates children in `ISceneNode::onAnimate` 0x596d6c. No emitter-name special case.

Actual Celeste asset: three PCloud emitters, rates .75/.45/1.2 per second, lives .0667/.0333/.0667 seconds. `bolt2` single-key RGBA218,197,101,255. Body includes normal diffuse/additive geometry and a glow mesh; emitters alone have not yet been proven to fix the separated white-body look. Core checkpoint 6dd26c6d fixes source box axis mapping and per-particle color tracks. Camera checkpoint 52700217 converts GL camera row2 to source +forward, avoiding backface loss. Attachment API `include_meshes=false` refuses emitter-free actor assets and emits particles only.

Expected behavior/minimal implementation: keep actor CharacterVisual animation and mesh submission exactly once. Attach authored actor emitters once per current factory/actor, advance on absolute gameplay effect time while sampling clip parameters at clip time, use same live actor world placement as mesh. Invisible/disabled actors do not submit effects. Keep actor and scene-object identity domains separate (stable IDs are arbitrary 64-bit hashes; OR-ing a reserved bit is not collision-free).

Uncertainties: original video v1.0.3 vs recovered assets/library version may differ; exact glow mesh/billboard binding and attachment to external animation clip poses still require audit. P2 inspected samples provide no full faery sequence. No claim of visual equivalence until real EXE captures.

Pre-code tests: emitter-free actor refusal, duplicate idempotence, particles-only submitted sources, disabled/absent actor no frame contribution, transform follows live actor, pause and clip loop preserve correct particle lifetime. Quiet real EXE with fresh scratch saves: dense faery idle/motion captures and ordinary actor pose regression. Freeze executable/hash and record ctest. Accepted packages and user saves untouched.

## Preserved inheritance

Existing dirty actor_population.* / main.cpp draft preserved as 7eda1010 unchanged and honestly unverified. Merged shared 6dd26c6d (6c265e87), then 52700217. Draft still contains actor mesh suppression and requires removal. No inherited work deleted/reset.
