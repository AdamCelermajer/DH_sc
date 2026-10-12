# Character InitPost -> actual model: V41 dependency audit

LoadBaseProperties and Recalc(true) already exist in both the current V5 graph and shared main source. The missing phase bridge is the ordered call chain into them, plus actual template/predicate/model services. Calling these providers during construction would bypass original spawn, debug and waypoint behavior.

| Original step | Current actual provider | Required producer / boundary |
|---|---|---|
| Character InitPost3b4d60 sets called1394 first | CharacterNpcInitPostOwnerV1, actor.init_post_fields, retained record.initialize | Keep one owner through success/failure; never bypass failed prefix with the original called-bit return |
| CheckSpawnProbability38bd64 | Generic game_object_check_spawn_probability_v1 and canonical-base adapter exist; Character invoke delegates | SAME actor inherited cache270/probability274, actual Handle->AsChar->IsPlayer, both shared application channels, actual online/network and failed-spawn lifecycle callbacks |
| DebugSwitches.load337888 then GetSwitch337a88 | Remaining main service; substantial original bodies | Actual application DebugSwitches/file/global state; no blanket empty success |
| Optional starting_waypoint13e8 | Remaining main service | ObjectManager.GetByName34aca0, Handle.GetObject33fdc0, AsGameObject33fee4, controller.Cmd_MoveTo405540; same registry/controller/room |
| SafeGetCharPropsId3b3d38 | Direct name/cache branch in record.invoke; nonempty template delegates | Actual live IsPlayer first after cache miss; V39 template borrow of real13c8/13ca and same application RNG; player Save/class branch stays main-owned |
| LoadBaseProperties3df2a4 at3b4ed0 | ALREADY record.invoke; valid row writes same properties.base, invalid leaves unchanged | Same actual CharacterTable/PropertyState; signed cached halfword is passed as signed ID |
| Recalc(true)3e0810 at3b4edc | ALREADY data::recalc_properties_with_class on same state | Class comes from base[26]; fresh no-buff domain supported; live buffed/adopted state must lend actual PropertyView/groups to existing dh2_class_recalc_base |
| GetCharModelName3a54d4 at3b4ee4 | NPC nonplayer shortcut exists; Faery/player delegates | V38 borrowed live resolved[3], actual GetCharType and whole IsPlayer; Faery real master/save/table providers and player device/local-player providers |
| NonNULL model copy at3b4efc/3b4f00 | CharacterNpcInitPostOwnerV1 writes same model290 | Retain actual dictionary backing until copying; NULL leaves model290 unchanged |
| Source scale before parent visual | Existing dh2_character_visual_scale uses same base12..14 | Preserve source position of this store; do not use constructor scale for initialized model |
| GameObject.InitPost38be5c at3b4f88 | Remaining main-owned whole helper; retained visual field/asset owner exists | SAME actor visual/string/scale/physical/PF/scene fields; no second canonical base or inspection renderer |
| MeetCondition38ab60 after parent visual | Existing game_object_meet_condition_v1 | Original body is mov r0,1; bx lr; this specific GameObject helper is distinct from authored condition systems |
| Remaining Character NPC tail | Existing ordered NPC owner, separate main bodies required | SG_Load2, AI/scripts, FX registration, animation set, sounds, floor/position, mesh box, Revive/vitals, init script/group. A visible mesh does not complete these helpers |

The original call chain is explicit:3b4e74 SafeProps -> signed cached13c8 ->3b4ed0 LoadBase ->3b4edc Recalc(true) ->3b4ee4 GetModelName -> nonNULL CString290 copy -> source scale ->3b4f88 whole GameObject.InitPost ->3b4f90 MeetCondition. Native NPC owner reproduces that order. Existing direct/base/recalc providers are an allocation/property/stat prefix, not a whole activated NPC.

Root and V5 CanonicalCharacterRecordV4 currently contain actor/design/properties/life/view/inventory/init/fields/services. They contain no CanonicalGameObjectBaseOwnerV1 member. RetainedCharacterActorV1 owns inherited fields directly. Search of all retained_character* source found no270/cached_roll/spawn_roll/CheckSpawn storage or transport. Reusing another family's base would duplicate authority.

Original GameObject C2 (38c398, reached by Character C1 GO0) writes probability274=100 at38c494 and cached_roll270=-1 at38c4a8. Three poisoned executions prove the unconditional producer prefix38c488..38c4ac. Its register inputs are source-derived r4=this at38c3a4 and r5=0 at38c3bc; the prefix itself produces r7=-1. This is explicitly a store-prefix proof. A standalone whole GO0 constructor also reaches uninitialized external array/helper services; that larger run is not claimed. Existing original full GameObject constructor capture covers GO7/20. Main root already contains game_object_spawn_probability_v1.cpp; selected V5 has its header but currently lacks that CPP. Parent must include the existing source in the coherent graph before calling the borrowed kernel.

Narrow actual-field proposal: one signed source_cached_spawn270_ member in the existing actor. Borrow it with the existing actual spawn_probability_ after its property producer writes it, and guard by the existing fresh-C1 authority flag and the exact actual record alias. Existing/adopted actors require genuine authority transfer. No second base, local cache, second RNG, per-actor Application or broad member initialization. Main must also supply the actual source network108/online-ownerfc/byte82 and visibility/delete/mark callbacks if the chosen whole spawn kernel reaches them. Cached/player success branches must still preserve the original Handle/IsPlayer-before-cache order.

Whole IsPlayer3a49f0 is not simply AI.type==1. For nonzero GetCharType it returns type==1. For type0 it executes strstr(Character+44 CString,"PlayerCharacter")==that CString pointer: exact case-sensitive prefix match. Original literal8c3170 and function SHAefb4b11a24129fac129ee1ee6460e1a8f9eef7b8648d7f9f70d3d2338eee342a. Current family shortcut misses this branch. NEW stateless borrowed predicate in reference/character-init-model-v41 uses a live archetype and actual GetCharType callback, reads them every call, and requires archetype only on type0. Original body63 cases matched native63 cases plus49 live/missing-provider checks. GetCharType is explicitly fixture-supplied in the oracle; the actual world provider remains main-owned. V40 inspection predicates remain labelled fixtures.

Model/template seams should be installed in CanonicalCharacterRecordV4.remaining_init at reached requests, not scheduled independently. V39 requires shared_ptr<RetainedCharacterActorV1>(record,record->actor.get()), actual existing Application owner, record.services.loot_random==application.channel0 and actual IsPlayer. V38 model binding requires the same record alias, actual initialized PropertyView, pinned current model dictionary, and reached live services. To cover every model branch, main should delegate3a54d4 through the whole adapter before the old AI type shortcut; its current routing delegates only player/Faery types. Loader's role is lending these owners/fields and recording failures. Main owns the services and their effects.

| Subgoal | Status |
|---|---|
| Exact existing base/recalc callbacks and original order | Finished source audit |
| Missing pre-model phase dependencies | Finished source audit; production services pending |
| Live type0/type1 IsPlayer predicate | Finished63 original/native cases; actual main type provider pending |
| Missing actual cache270/C1 producer | Finished search +3 poisoned producer-prefix cases |
| Actual actor270 adoption | Proposed only; no selected graph edits |
| Whole InitPost, actual visual, gameplay/RNG phase proof | Pending main/parent; not claimed |
