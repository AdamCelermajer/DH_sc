# Character loader fields and model-name adapter v38

Original factory/C1 and model selection are proved; retained actor changes remain proposed and unapplied. The new adapter owns no cache, master, RNG, save, or model table: it reads borrowed live fields and invokes reached main-owned services.

| Source field | Original constructor producer | Current selected retained actor | Proposed ownership |
|---|---|---|---|
| Character13c8 signed16 properties cache | C1 3aa34c = -1 | Existing char_properties_id13c8_ | Reuse same field |
| Character13ca signed16 template cache | C1 3aa354 = -1 | Missing | Add char_template_id13ca_ to actual retained actor |
| Character418 / CharAI50 master | CharAI C1 3cec70 = NULL; subobject starts Character3c8 | Missing; group34 exists separately | Add source_ai_master50 to actual actor and route main master callbacks to it |

The proposal patch is illustrative and deliberately unapplied. Fresh canonical construction produces these values; existing-actor adoption must transfer real existing field authority before enabling borrows. It cannot silently replay C1 defaults. Borrows carry the receiver lease and pointers to the actual fields. Template adapter V35 must receive the SAME13c8/13ca references and actual application RNG; it must preserve original lookup order and cached fast paths.

Actual producers:
- SafeGetCharPropsTemplateId3b36ec writes13ca for a nonempty actual template; empty name returns existing cache. SafeGetCharPropsId3b3d38 returns cached13c8 before sampling; player path uses actual SG_Load/class. InitSpawned3b379c changes13c8 and leaves13ca intact.
- AI_SetMaster3d4d80 stores master at3d4d8c before main AI behavior. NULL branch proved whole; nonNULL store prefix proved separately, stopping before unported AI effects. Character Lua _SetMaster3b9084 and Level PlaceFaeryAndFollowers3f0898 route through it.
- ObjectManager.InitPost is load state10, _LoadPlayer state14, and PlaceFaeryAndFollowers state37 after real NativeEndLoading advances36->37. Every model query must read the live field: a phase number does not prove no earlier script/master producer changed a particular actor.

Model-name adapter files: `reference/character-loader-fields-v38/character_model_name_v38.hpp/.cpp`. Exact source selection:
1. GetCharModelId3a31e8 reads Character1004 and bounds-checks the actual model dictionary. Its source projection is CharProperties560 + resolved-sheet a94 + sheet header4 + ModelFile index3*4. Adapter uses the actual live PropertyView.resolved[3], not a guessed model or appearance.
2. IsFaerie3a3094 calls GetCharType3a3054 and tests type3. A produced master=NULL selects the authored base model. A nonNULL master invokes actual SG_GetCurrentFaerieId(-1), then actual GetCharFaery(id)->Model+c. Missing master storage/provider fails; it never becomes NULL or a fabricated current save.
3. Non-Faerie always calls actual IsPlayer virtual28. Players require Device.IsHighPerformance; low-performance players require actual PlayerManager.IsLocalPlayer. Remote classes use original fixed model rows75/76/77; unknown classes require original Debug policy. It never infers IsPlayer from an AI-type shortcut or omits the player branch.

SWAMP inspection finding: exact original CharacterProperties row97 `DefaultFairy` has AI20, AnimTable23, ModelFile34. Model dictionary row34 is `faeries_02_celeste`, selecting `data/3D/characters/faeries/faeries_02_celeste.bdae`. The raw table is a224-int row prefix with later sections; consumed offsets and whole-file hashes are retained. Parent's actual detached/native property computation must remain authoritative if it changes the resolved Model field.

A constructor-state inspection export can close the remaining Faery asset reference when the receiver is freshly produced by actual C1, actual property/class initialization has completed, and no master producer has run. It must label that inspection accordingly. This is neither saved Faery selection nor activated gameplay. At state37 and whenever a master is present, use the current actual master/save/table callbacks; never reuse a prior constructor snapshot.

Validation: three poisoned whole Character factory/C1-body executions include actual CharAI C1; unrelated base/subobject/global collection/string services are explicit fixtures. Thirteen original GetCharModelName cases execute actual GetCharModelId, IsFaerie and actual SG current-ID lookup against explicit save storage, with remaining predicates/Faery rows as domain fixtures. Native stateless adapter matches all13 and observes master changes through one borrowed pointer; absent source fields/services fail. No retained actor/overlay edits, no V4 archive linking after parent's V5 ABI change, no emulator launch.

Remaining integration: parent adopts real actor members/guards/borrows into the selected coherent source graph, wires native source/property/model services, and updates inspection export. Main retains master AI behavior, player/save/current-difficulty/Faery table providers and complete activation. The standalone adapter tests do not prove those services.
