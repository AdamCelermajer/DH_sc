# B101 (wall of fire) + B105 (Celeste glow): model-embedded particle emitters

Branch `fix/b101-b105` (worktree `DH_wt/fixfx`, from `v20.1/integrate` dfc67d8d). Not pushed. Written incrementally.

## 0. Session state carried over
- `DH_wt/fixfx`: no earlier commits; scratch in `DH_wt/fixfx_work` (BRES decoders `bres.py`, `dump_bdae.py`, `dump_nodes.py`; IDA function dumper `fn.py`; reference frames `ref/`; baseline runs `runs/base*`).
- `DH_wt/fix105` uncommitted diff (actor model path in `PopulationActor`, actors offered to `EmbeddedSceneFxV1`, actor mesh not drawn twice): useful, carried into this branch.
- `DH_wt/fix101`: clean; scratch `DH_wt/fix101-scratch` (P1/P2 reference sheets).

## 1. Visual evidence (observed)
- User shot-19 (our v20): the red door fire is a smooth flat red/orange translucent band across the floor (diagonal), no flames, no motion texture, no particles visible.
- Our baseline (`fixfx_work/runs/base3/f240.png`, hero at -12137,10444): identical flat band, bottom left.
- Reference P1 612-623 s (`fix101-scratch/p1sheet.png`, `p1dense.png`, 1 s and 0.25 s spacing) and P2 5:40-6:16 (`p2sheet.png`): a thick band of billowing orange flame puffs along the corridor line, bright yellow-orange cores, dark-orange edges, small bright sparks; the puffs drift/billow from frame to frame (motion visible at 0.25 s spacing); the band has visible vertical thickness (rises above the floor), it is not a flat decal.
- User shot-22 (our v20): Celeste is a flat white figure with white wing streaks and one yellow glow sprite.
- Reference P1 288-293 s (`fixfx_work/ref/celeste-zoom-288-293.png`): golden-white glow around the faery, small golden star sparkles appear around/behind her, warm yellow light, not a flat white body.

## 2. Data (decoded from the shipped BDAE, `fixfx_work/dump_bdae.py`, `dump_nodes.py`)
### go_door_fire_red.bdae (`_prim_Swamp2Door_FIRE`)
- One emitter `GL_PCloud01-emitter` (record +0x8 = 0: box shape). Shape block: RadiusLength 1, Width 1, Height 862.5. Direction (0,0,1), DirectionVariation 0. BirthRate 15, Life 1.667, LifeVariation 0, TargetSize 500, SizeVariation 0, SizeGrowthTime 0.0667, SizeFadeTime 1.267, Speed 300, SpeedVariation 0.5, AnimKeyMappingType 1, AnimLength 0.667, AnimScaleMultiplier 1, SpinTime 11.67.
- Emitter node `GL_PCloud01-node` t=(324.2,34.05,252.82) identity rotation, under `_bone_fx_doorfire-node` t=(-435.38,0,0) under `fx_doorfire-node`.
- Materials: `ColorMaterial_00E59BB8` (emitter, FX_smoke_03.tga), `Standard_20` (flame mesh, fx_doorfire.tga), `Standard_202`.
### faeries_02_celeste.bdae (`DefaultFairy`, original-cache)
- Three emitters under `root_character-node` (t z=270), node scale 0.609: `GL_PCloud04` (box 150x10x10, TargetSize 120, Life 0.0667), `GL_PCloud06` (+0x8 = 1: sphere, radius 40, TargetSize 145, Life 0.0333), `GL_PCloud07` (box 60x20x20, TargetSize 95, Life 0.0667). SpinPhase 1.571, SpinPhaseVariation 1.
- Materials `additive`, `alpha`, `bolt2`, `diffuse`, `fx_particles_additive` (atlas_fx_particles_001.tga), images incl. `fx_spell_lightning_sparks.tga`.

## 3. IDA (in progress)
