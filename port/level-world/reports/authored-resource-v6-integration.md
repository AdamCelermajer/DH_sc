# General authored FX resource V6 integration

The current renderer retains `CharacterAuthoredResourceFactoryV6` in its existing sole factory member. New level-world TUs are `character_authored_resource_v6.cpp` and `authored_fx_mesh_graph_v6.cpp`; new engine-animation TU is `particle_scalar_animation_v6.cpp`. V5 sources and their receipts are preserved. V6 private receiver classes have distinct versioned names, so compiled predecessors can coexist without weak-symbol/type collisions.

The actual45 resource census accepts38 resources over738 CPU frames, including all24 inventoried player effect files. The focused skin/scalar test passes5665 checks: five actual skin resources change per same node/primitive, every output matches the existing source palette/position leaf, and outer placement is applied once. Both original37/38 bind literals map to SpinPhase/SpinPhaseVariation and write the same registered model storage. Existing CFloatEx source arithmetic/cursor paths are retained. This extends native software-skin projection; it does not claim the original hardware-skin technique/driver selection.

Strict newV6 production compiles pass bothABIs (6); retained actor compiles pass bothABIs (2); coherent renderer/panel closure passes18 compiles. The linked fixture APK was 6b0b5408e7bec23ef7c3a570b7b1a1e328cae302733159eb3de986872911d69e. Fixture camera, driver8 and outer matrix are explicit. No live authored Use damage timing or GPU acceptance is inferred.

NPC anchored FX now reads constructor-owned raw81/84 from the retained Character receiver. Raw81=0 is the executed ObjectBaseC1 result in the existing constructor oracle. The existing once-only NPC visual setup publishes the exact Character.InitPost scale calculation into the SAME source120 field before visual transform; the FX borrow requires visual initialization and never substitutes draw appearance or byte8a. Full canonical Character.InitPost lifecycle remains a separate unresolved boundary.

## Remaining required resource bodies

- skill_dh2_monster_dark_queen2ndform_projectile.bdae: Required blood resource initialization
- skill_dh2_monster_dragon_attack_01.bdae: Blood source cloud update -1
- skill_dh2_monster_dragon_attack_02.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_attack_03.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_01.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_02.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_03.bdae: Required authored FX mesh payload

The remaining DarkQueen projector initializer is source SpinAxisType2 (DirectionType1), not an unproven DirectionType0 interpretation. The five Dragon special payloads have actual serialized geometry kind1; Error::geometry_type3 is the parser error code. These are distinct from classic kind0 meshes and remain explicit. Dragon01 reaches an additional force/model branch. No effect substitution or successful empty backend is supplied.
