unsigned classes=0,checks_completed=0,pre_failures=0,use_failures=0,positive_native_results=0,native_hp_prefixes=0;
std::ostringstream prefixes;
const char* names[]{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"};
const char* scripts[]{"prince_warrior_bashdown","prince_mage_coldray","prince_rogue_jump_kick"};
for(unsigned which=0;which<3;++which){
 Unit unit;auto identity=std::uintptr_t(0xabcdef0600000001ULL+which*2);
 unit.fsm_state.current=3;unit.fsm={&unit.fsm_state,identity,1,0};
 unit.event_owner={identity,identity+1,reinterpret_cast<std::uintptr_t>(&unit.fsm_state),identity+2,0,0,0,0};unit.ai_keys[0x90/4]=0x3d0ca0;unit.ais_keys[0x90/4]=0x3dcc80;unit.events={identity+3,&unit.event_owner,unit.ai_keys.data(),0,unit.ais_keys.data(),0,0,0,0,0,0};
 const auto at=std::find(d.characters()->names.begin(),d.characters()->names.end(),names[which]);check(at!=d.characters()->names.end());
 CharacterScriptSessionInputV3 in;in.identity=identity;in.name="Player";in.source_is_character=1;in.state_machine=&unit.fsm;
 in.properties=std::make_shared<data::PropertyState>();in.combat=std::make_shared<data::CombatActorState>();in.temporary=std::make_shared<data::PropertySheet>();
 data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[at-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error),error);
 unit.saved=std::make_shared<data::PlayerSavegameV1>();unit.saved->set_character(identity);const auto list=in.properties->resolved[28];check(list>=0&&std::size_t(list)<sb.lists().size());check(unit.saved->initialize_skills(sb.lists()[list],error),error);unit.saved->initialize_faeries();in.savegame=unit.saved;
 unit.scripts=full_cache;in.common=full_cache.common();in.cached_file_context=&unit;in.cached_file=Unit::cached;
 MenuPlatform platform;platform.root=root;platform.assets=assets;platform.identity=identity;platform.debug=debug;platform.files=files;platform.design=design.borrow();platform.text.assets=assets;
 auto constants=file(assets+"/original-cache/data/pydata/common_text_pycst.bin");dh2_script_constants_reload receipt{};check(!dh2_script_constants_load(platform.text.constants,constants.data(),constants.size(),&receipt));
 HudTextEnvironmentV1 env{{&platform,MenuPlatform::text_open,MenuPlatform::text_close,MenuPlatform::text_debug,MenuPlatform::constant,nullptr,nullptr}};
 auto scene=resources.borrow().factory_scene();data::LootRandom8V2 random{1,0};PlayerEquipmentRenderInputsV1 gear_input;gear_input.design=design.borrow();gear_input.properties=in.properties;gear_input.random=&random;gear_input.character=identity;gear_input.potion_capacity=12;gear_input.language_pack=0;gear_input.resources=resources.borrow();gear_input.live_scene=&scene;gear_input.assets={&platform,MenuPlatform::asset};gear_input.debug=debug;gear_input.debug_files=files;gear_input.text_environment=env;gear_input.world={&platform,MenuPlatform::world};
 PlayerEquipmentRenderOwnerV1 gear(std::move(gear_input));check(gear.initialize(error),error);check(gear.inventory()->equipment()[0][1],"Actual initial-grant equipped main hand");
 PopulatedWorldV6 world;world.design=design.borrow();world.gear=&gear;
 world.enemy_properties=std::make_shared<data::PropertyState>();bool enemy_found=false;
 for(std::size_t j=0;j<d.characters()->rows.size();++j){data::reset_properties(*d.rules(),*world.enemy_properties,&d.characters()->rows[j]);if(!data::recalc_properties_with_class(*d.classes(),*d.rules(),*world.enemy_properties,error))continue;
  auto* ai=data::ai_props(*d.ai(),world.enemy_properties->resolved[1]);if(ai&&ai->type==4&&data::ai_enemy(*d.ai(),in.properties->resolved[0],world.enemy_properties->resolved[0],true,false)){enemy_found=true;break;}}
 check(enemy_found,"Actual hostile monster character row");world.enemy_view=data::property_view(*d.rules(),*world.enemy_properties);
 for(const auto& ai:d.ai()->rows)world.types.push_back(ai.type);world.ai_types={world.types.data(),std::uint32_t(world.types.size()),0};
 for(unsigned i=0;i<2;++i){world.objects[i].identity=identity+i;world.objects[i].visible=1;world.characters[i]={identity+i,i?world.enemy_view.resolved:in.properties->resolved.data(),i?"NativeEnemy":"Player",0x2000,0,0,1,1};world.handles[i]={std::int32_t(i+1),0,0};world.records[i]={std::int32_t(i+1),0,identity+i};}
 world.objects[1].position[1]=-100;world.entry={&world.entry_sentinel,&world.objects[1]};world.entry_sentinel={&world.entry,nullptr};world.room={&world.sentinel,&world.entry_sentinel};world.sentinel={&world.room,nullptr};world.search_registry={&world.sentinel};world.handles_registry={world.records,2,2,77,0};
 NativeDebug native_debug{debug,&files};world.debug=&native_debug;sk::SkillManaServicesV5 mana_services{&native_debug,NativeDebug::mana};sk::SkillAttackNativeServicesV6 attack_services{&native_debug,attack_debug};
 TargetOwner16 target_owner{identity,0,0,0};TargetState48 target{identity+4,&target_owner,0,0,0,0,0,0,0,0};TargetBindings48 target_bindings{&target,{&native_debug,NativeDebug::target},nullptr,{0,0}};in.target=&target_bindings;
 dh2_script_object_services objects{&world,PopulatedWorldV6::type,PopulatedWorldV6::methods,PopulatedWorldV6::method};in.objects=&objects;
 sk::SkillNativeWorldV5 live{&world.objects[0],&world.search_registry,{&world,PopulatedWorldV6::search},&world,nullptr,PopulatedWorldV6::resolve,&objects};
 sk::CharacterSkillNativeReadOnlyBindingsV6 readonly(*gear.inventory(),unit.fsm,mana_services,live);
 sk::SkillCombatWorldV6 combat_world{&world,PopulatedWorldV6::handle,{&world,PopulatedWorldV6::query},PopulatedWorldV6::actor,nullptr};
 DotCombatContext32 combat_context{};data::CombatRandom combat_random{1,0};sk::SkillApplyServicesV6 applications{&world,PopulatedWorldV6::application,&attack_services};
 sk::CharacterSkillNativeBindingsV6 native(readonly,*gear.inventory(),unit.fsm,sb,combat_world,combat_context,combat_random,attack_services,applications);
 in.gameplay_context=&native;in.gameplay_binding=sk::CharacterSkillNativeBindingsV6::binding;
 auto player_init=init;player_init.gameplay.ai=&unit.events;unit.skill_owner={identity,0,0};player_init.gameplay.skill_owner=&unit.skill_owner;player_init.gameplay.difficulty_context=&unit;player_init.gameplay.difficulty=Unit::selected;
 unit.player=sk::CharacterPlayerSkillsV6::create(design.borrow(),in,sb,faeries.borrow(),debug_services,unit.fsm,player_init,error);check(bool(unit.player),error);world.player=unit.player.get();
 check(!native.attach(unit.player->session()),native.error());check(unit.player->initialize(1)==1,unit.player->error());check(!readonly.install_object_binding(),readonly.error());world.refresh();
 check(!sk::dh2_character_skill_regen_v6(&world.enemy_view,0,-1,&attack_services),"Native retained enemy spawn HP refresh");check(world.enemy_view.resolved[36]>0,"Actual hostile row maximum HP");
 auto& player=*unit.player;check(player.session().properties()==gear.properties()&&player.native_buffs(),"Same V6/equipment/property/buff owner");
 auto& rows=player.state().skills;auto found=std::find_if(rows.items,rows.items+rows.count,[&](auto* row){return row&&!std::strcmp(row->script,scripts[which]);});check(found!=rows.items+rows.count);auto index=unsigned(found-rows.items);
 check(unit.saved->set_skill_level(index,1,error),error);check(player.update()==1,player.error());check(!dh2_property_set(&player.session().property_view(),41,100000));
 std::uint32_t answer=0;check(!player.callback(index,sk::skill_check_usable_v3,&answer)&&answer==1,player.error());++checks_completed;
 const auto mana_before=in.properties->resolved[41];auto pre=player.callback(index,sk::skill_pre_v3,&answer);check(pre!=0,"Populated Pre must fail at genuine missing Cmd_LookAt");check(readonly.targets().count==1&&readonly.targets().heap[0].identity==world.objects[1].identity,"Retained native populated target prefix: "+std::to_string(readonly.targets().count)+"; "+readonly.error()+"; "+player.error());check(in.properties->resolved[41]==mana_before,"LookAt failure precedes source UseMana");++pre_failures;
 // The source has already cached target / search backup before LookAt failed.
 // Separate actual Use delivery proves native calculation and required effect
 // failure; this is not a claim of successful whole Pre→Use gameplay.
 auto use=player.callback(index,sk::skill_use_v3,&answer);check(use!=0,"Required native target application effect failure");check(world.required_application!=0&&world.latest.mask!=0&&combat_context.attacker==identity,"Source authored Use reached actual V6 calculation/application");check(combat_random.calls>0,"Actual shared combat RNG advanced");++use_failures;
 // Independently calculate the authored current row over the exact same graph.
 const auto source_row=sb.lists()[list][index];auto& skill=sb.skills()[source_row];world.refresh();data::CombatResult calculated{};
 check(!sk::dh2_character_skill_attack_calculate_v6(&calculated,&combat_context,&combat_random,&world.attacks[0],&world.attacks[1],gear.inventory(),skill.scalar.words[7],std::int32_t(skill.scalar.words[6]),&attack_services),"Actual equipped same-owner native calculation");
 check(calculated.amount>0,"Positive authored native result");++positive_native_results;
 // Retained encounter relation is an explicit initial storage fixture, not
 // an accepted acquisition/PlayerAIS OnAggro callback. Subsequent AddAggro and
 // reciprocal relation changes execute the ordered native V6 owner operation.
 const float previous_threat=2.0f;std::uint32_t threat_bits;std::memcpy(&threat_bits,&previous_threat,4);
 world.enemy_outgoing.entries[0]={world.objects[0].identity,threat_bits,0};world.enemy_outgoing.count=1;
 world.player_incoming.entries[0]={world.objects[1].identity,threat_bits,0};world.player_incoming.count=1;world.retained_encounter=true;
 const auto hp_before=world.enemy_view.resolved[36];bool native_hp=false;
 for(unsigned attempt=0;attempt<16&&!native_hp;++attempt){check(player.callback(index,sk::skill_use_v3,&answer)!=0,"Required application tail remains unavailable");native_hp=world.enemy_view.resolved[36]<hp_before;}
 check(native_hp,"Actual authored Use/AddAggro/HitFor HP effect prefix");++native_hp_prefixes;
 check(world.enemy_outgoing.entries[0].threat_bits==world.player_incoming.entries[0].threat_bits,"Same native reciprocal relation value");
 if(which)prefixes<<',';prefixes<<"{\"script\":\""<<scripts[which]<<"\",\"hp_before\":"<<hp_before<<",\"hp_after\":"<<world.enemy_view.resolved[36]<<",\"required_hit_service\":"<<world.required_hit<<",\"required_application_service\":"<<world.required_application<<'}';
 check(player.session().property_view().resolved==gear.property_view()->resolved&&world.attacks[1].properties->resolved==world.enemy_properties->resolved.data(),"No scratch property copies");
 check(native_debug.strings.empty(),"All actual owned debug strings released");
 SameSaveReloadProofV6 save_proof{unit.saved.get(),&player,sb};sk::SkillSaveReloadServicesV6 reload_services{&save_proof,SameSaveReloadProofV6::selected,SameSaveReloadProofV6::load};sk::SkillSaveReloadOutputV6 reload{};
 data::SavedSkillUpdateServicesV1 update_service{&player,[](void* p,std::uintptr_t id,std::string&){auto& p1=*static_cast<sk::CharacterPlayerSkillsV6*>(p);return p1.session().timers().owner==id&&p1.update()==1;}};
 check(unit.saved->set_skill_in_slot(0,index,update_service,error),error);check(unit.saved->has_skill_slots());
 check(sk::dh2_character_skill_save_reload_v6(&reload,unit.saved.get(),&reload_services)==-2&&reload.phase==2&&reload.deleted,"Required selected-list failure retains delete/null prefix");
 check(unit.saved->has_skill_slots(),"Source map clear not reached on list selection failure");
 save_proof.reject_selection=false;
 check(sk::dh2_character_skill_save_reload_v6(&reload,unit.saved.get(),&reload_services)==-2&&reload.phase==6&&!reload.deleted,"Required actual outer Save.Load remains explicit");
 check(save_proof.selections==2&&save_proof.loads==1&&unit.saved->skills().size()==sb.lists()[list].size()&&!unit.saved->has_skill_slots(),"Same Save native reinitialization and source slot clear prefix");
 check(player.native_savegame()==unit.saved.get()&&in.savegame==unit.saved,"Retained original Session/current skill save authority unchanged");
 const auto spell_count=player.state().spells.count;
 save_proof.reject_selection=true;
 check(player.native_reload_skills(&reload_services)!=1,"Whole same-owner reload retains required selection failure");
 check(player.state().skills.count==0&&player.state().spells.count==spell_count,"Actual skill destruction/reset precedes same Save selection; spells retained");
 check(player.native_savegame()==unit.saved.get()&&save_proof.selections==3,"Whole reload retains original Save identity");
 data::PlayerSaveLoadOwnerV1 actual_load(unit.saved);save_proof.load_owner=&actual_load;save_proof.reject_selection=false;
 check(&actual_load.save()==player.native_savegame()&&!actual_load.profile().identity&&unit.saved->slot()==-1,"Genuine same Save constructor-null profile and fresh slot");
 check(player.native_reload_skills(&reload_services)==1,player.error());
 check(actual_load.delivered_calls()==0&&!actual_load.profile().identity&&player.state().skills.count==sb.lists()[list].size(),"Whole native reload succeeds through genuine mask8 null-profile guard and configure/update");
 check(player.state().spells.count==spell_count&&player.native_savegame()==unit.saved.get(),"Whole positive reload retains spells and original Save authority");
 auto retained_profile=std::make_shared<int>(1);check(actual_load.publish_profile({identity+0x100,retained_profile},error),error);
 check(player.native_reload_skills(&reload_services)!=1,"Nonnull retained profile requires actual section backend despite slot -1");
 check(actual_load.profile().identity==identity+0x100&&actual_load.delivered_calls()==1&&player.state().skills.count==0,"Required nonnull section failure preserves real reload prefix before configure");
 check(&actual_load.save()==player.native_savegame()&&unit.saved->skills_initialized(),"Nonnull failure retains SAME Save reinitialization");
 player.session().close();++classes;
}
std::cout<<"{\"validation\":\"PASS\",\"same_owner_classes\":"<<classes<<",\"actual_starter_checks\":"<<checks_completed<<",\"populated_look_at_failure_prefixes\":"<<pre_failures<<",\"actual_use_required_application_failures\":"<<use_failures<<",\"positive_same_owner_native_results\":"<<positive_native_results<<",\"native_hp_effect_prefixes\":"<<native_hp_prefixes<<",\"required_prefixes\":["<<prefixes.str()<<"],\"no_alias_fixture\":true,\"scratch_property_copies\":false,\"explicit_offline_world_geometry_input\":true,\"explicit_retained_encounter_relation_input\":true,\"full_target_application\":false,\"full_campaign\":false,\"checks\":"<<checks<<"}\n";return 0;
