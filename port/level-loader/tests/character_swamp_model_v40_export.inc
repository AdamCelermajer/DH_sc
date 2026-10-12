// Inspection only. Existing source records are OBSERVED without changing their
// caches, master, sheets or application RNG. Original Init/RNG order unverified.
 auto escape_xml=[](const std::string& in){std::string out;for(char c:in){switch(c){case '&':out+="&amp;";break;case '<':out+="&lt;";break;case '>':out+="&gt;";break;case '"':out+="&quot;";break;default:out+=c;}}return out;};
 std::ofstream export_file(argv[4]);require(bool(export_file),"V40 export unavailable");
 export_file<<"<ObjectPreview identity=\"SWAMP\" scope=\"actual constructor records observed; isolated fresh inspection actors; original whole Init RNG order UNVERIFIED\">\n";
 CharacterTemplateAssetsV35 template_assets;
 {bool exists{};std::vector<std::uint8_t> rows,names,schema;require(archive.read("data/pydata/character_templates_pyarray.bin",exists,rows,e)&&exists,e);require(archive.read("data/pydata/character_templates_pyarraynames.bin",exists,names,e)&&exists,e);require(archive.read("data/pydata/character_templates_pystructnames.bin",exists,schema,e)&&exists,e);require(template_assets.load({rows.data(),rows.size()},{names.data(),names.size()},{schema.data(),schema.size()},e),e);}
 auto inspection_application=std::make_shared<ApplicationSpawnRandomOwnerV4>();
 const auto production_rng_before=application->channel(0);const auto production_rng1_before=application->channel(1);
 auto models_alias=std::shared_ptr<const void>(character_tables_owner,&character_tables.models);
 character::CharacterModelNamesV38 models{models_alias,&character_tables.models.values};
 data::GameObjectDictionaryV11 dictionary;
 {bool exists{};std::vector<std::uint8_t> rows,names;require(archive.read("data/pydata/game_objects_dictionary_pyarray.bin",exists,rows,e)&&exists,e);require(archive.read("data/pydata/game_objects_dictionary_pyarraynames.bin",exists,names,e)&&exists,e);require(dictionary.load(rows.data(),rows.size(),names.data(),names.size(),e),e);}
 unsigned accounted_characters=0,accounted_chests=0,exported_characters=0,exported_chests=0,nonvisual=0,blocked=0,faery_cases=0,templates=0;
 for(const auto* attempt:attempts){
  auto* factory=attempt->factory_attempt();if(!factory||factory->stage()!=CanonicalFactoryStageV1::complete)continue;
  auto* object=fixture.manager.object(factory->handle().key);auto* original=canonical_source_entry_v1(attempt->source().request());if(!object||!original)continue;
  auto kind=original->source().attribute("gametype");if(!kind||(*kind!="Character"&&*kind!="OpenableContainer"))continue;
  const auto& source=attempt->source().request();const auto* name_ptr=original->source().attribute("name");require(name_ptr,"V40 authored name missing");const std::string name=*name_ptr;
  std::string asset,reason,status="asset";std::array<float,3> position{},scale{{1.f,1.f,1.f}};bool derived_scale=false;std::int32_t selected=-1,model_id=-1;std::string description,templ,model_branch;
  if(*kind=="Character"){
   ++accounted_characters;auto record=character_factory->find(object->identity);require(record&&record->actor,"V40 actual Character lost");require(record->actor->source_position(position,e),e);
   auto actual_alias=std::shared_ptr<character::RetainedCharacterActorV1>(record,record->actor.get());character::CharacterLoaderFieldsV38 actual_fields;require(actual_alias->loader_fields_v38(actual_alias,actual_fields,e),e);
   const auto actual_props=*actual_fields.properties13c8,actual_template=*actual_fields.template13ca;const auto actual_master=*actual_fields.master418;
   const auto actual_sheets=*record->properties;
   export_file<<"<ActualState name=\""<<escape_xml(name)<<"\" properties13c8=\""<<actual_props<<"\" template13ca=\""<<actual_template<<"\" master418=\""<<actual_master<<"\" resolved_model=\""<<record->view.resolved[3]<<"\" class_row_phase=\"constructor rule defaults; later InitPost LoadBaseProperties pending\"/>\n";
   // Genuine canonical factory, real actor caches/view, isolated record and RNG.
   // No source loading callback is claimed: property/class projection below is
   // an explicit host inspection fixture over original tables.
   auto isolated_services=record->services;isolated_services.loot_random=&inspection_application->channel(0);isolated_services.state_services={};isolated_services.remaining_init={};isolated_services.set_position={};
   CanonicalCharacterFamilyFactoryV4 isolated_factory(isolated_services);CanonicalClassReceiverV1 isolated_receiver;
   require(isolated_factory.construct({"Character",0x340800},source,isolated_receiver,e),e);auto isolated=isolated_factory.find(isolated_receiver.object.identity);require(isolated&&isolated->actor,"V40 isolated actual factory record absent");
   auto property_actor=isolated->actor->properties();require(properties.init_properties(property_actor,e),e);auto template_name=source.attribute(source.source_context,source.element,"template");if(template_name)require(properties.set_template(property_actor,template_name,e),e);require(properties.load_defaults(property_actor,e)&&properties.load_overrides(property_actor,source,e),e);
   const auto* desc=isolated->actor->source_string(0x13b0);const auto* template_source=isolated->actor->source_string(0x1398);require(desc&&template_source,"V40 actual authored description/template producer missing");description=*desc;templ=*template_source;
   auto alias=std::shared_ptr<character::RetainedCharacterActorV1>(isolated,isolated->actor.get());character::CharacterLoaderFieldsV38 fields;require(alias->loader_fields_v38(alias,fields,e),e);
   if(!templ.empty()){
    CharacterTemplateFrameV39 frame;
    // Explicit nonplayer predicate fixture for authored Character placements;
    // production IsPlayer and original Init scheduling remain main-owned.
    require(CharacterTemplateFrameV39::bind(alias,inspection_application,isolated->services.loot_random,template_assets.borrow(),[](auto,bool& player,std::string&){player=false;return true;},frame,e),e);
    require(frame.select_nonplayer_template(selected,e),e);++templates;
   }else{const auto& names=isolated->design.characters()->names;auto at=std::find(names.begin(),names.end(),description);selected=at==names.end()?-1:static_cast<std::int32_t>(at-names.begin());const auto low=static_cast<std::uint16_t>(selected);std::memcpy(fields.properties13c8,&low,2);}
   if(selected<0||static_cast<std::size_t>(selected)>=isolated->design.characters()->rows.size()){status="unsupported";reason="Original direct/template Character row unavailable";}
   else{
    isolated->properties->base=isolated->design.characters()->rows[selected];require(data::recalc_properties_with_class(*isolated->design.classes(),*isolated->design.rules(),*isolated->properties,e),e);
    character::CharacterModelFieldsV38 model_fields;require(alias->model_name_fields_v38(alias,model_fields,e),e);require(model_fields.properties==&isolated->view&&model_fields.properties->resolved==isolated->properties->resolved.data(),"V40 model borrow detached from actual fixture record");
    auto actual_type=[isolated](std::uintptr_t identity,std::int32_t& type,std::string& error){if(identity!=isolated->actor->shared_handle().cached){error="V40 model predicate receiver mismatch";return false;}auto index=isolated->view.resolved[1];const auto* ai=isolated->design.ai();if(index<0||static_cast<std::size_t>(index)>=ai->rows.size())index=8;if(static_cast<std::size_t>(index)>=ai->rows.size()){error="V40 original AI row/fallback8 unavailable";return false;}type=ai->rows[index].type;return true;};
    character::CharacterModelServicesV38 services;services.character_type=actual_type;services.is_player=[actual_type](auto identity,bool& player,std::string& error){std::int32_t type{};if(!actual_type(identity,type,error))return false;player=type==1;return true;};
    character::CharacterModelResultV38 model;
    if(!character::character_model_name_v38(model_fields,models,services,model,e)){status="service_pending";reason=e;}
    else if(!model.file){status="nonvisual";reason="Original GetCharModelId selected no valid model";}
    else{asset=*model.file;model_id=model.model_id;model_branch=model.branch==character::CharacterModelBranchV38::faery_null_master?"fresh-produced NULL-master Faery":"original nonplayer branch";}
    require(dh2_character_visual_scale(scale.data(),isolated->view.base+12)==0,"V40 source visual scale failed");derived_scale=true;
    if(description=="DefaultFairy"){
     require(selected==97&&isolated->view.resolved[3]==34&&*fields.master418==0&&model.branch==character::CharacterModelBranchV38::faery_null_master&&model_id==34&&asset.find("faeries_02_celeste")!=std::string::npos,"V40 exact DefaultFairy/Celeste fixture failed");
     // Only the isolated fixture is changed. A nonNULL master must reach real
     // save/Faery providers; never retain a stale C1 fallback.
     *fields.master418=0x40;character::CharacterModelResultV38 preserved=model;require(!character::character_model_name_v38(model_fields,models,services,preserved,e)&&e.find("Faery")!=std::string::npos&&preserved.file==model.file&&preserved.model_id==34,"V40 later master lacked required actual provider boundary");export_file<<"<FaeryBoundary name=\""<<escape_xml(name)<<"\" fixture_nonnull_master=\"true\" actual_master_unchanged=\"true\" missing_provider_failure=\""<<escape_xml(e)<<"\" unchanged_output_on_failure=\"true\"/>\n";*fields.master418=0;++faery_cases;
    }
   }
   require(*actual_fields.properties13c8==actual_props&&*actual_fields.template13ca==actual_template&&*actual_fields.master418==actual_master&&record->properties->base==actual_sheets.base&&record->properties->resolved==actual_sheets.resolved&&record->properties->saved==actual_sheets.saved&&record->properties->gear==actual_sheets.gear,"V40 inspection mutated existing Character state");
  }else{
   ++accounted_chests;auto chest=std::find_if(containers.begin(),containers.end(),[&](const auto& value){return value->receiver().base().identity()==object->identity;});require(chest!=containers.end(),"V40 chest lost");std::copy_n((*chest)->receiver().base().vector3(0x160),3,position.begin());OpenableContainerRowV1 row;std::int32_t id{};
   if(!chest_data->resolve_row((*chest)->receiver().fields().data_desc,id,row,e)){status="unsupported";reason=e;}else{const std::string* file{};if(!dictionary.file(row.visual,file,e)||!file){status="unsupported";reason=e;}else asset=*file;}
  }
  if(!asset.empty()){bool exists{};std::vector<std::uint8_t> bytes;require(archive.read(asset,exists,bytes,e),e);if(!exists){status="missing_asset";reason="Original selected model absent from original cache";asset.clear();}}
  export_file<<'<'<<(asset.empty()?"Blocked":"Entity")<<" name=\""<<escape_xml(name)<<"\" kind=\""<<*kind<<"\" status=\""<<status<<"\" reason=\""<<escape_xml(reason)<<"\" scope=\"isolated fresh Character inspection; chest authored table reference; whole Init RNG order unverified\" module_occurrence=\""<<source.module_occurrence<<"\" runtime_module_id=\""<<source.runtime_module_id<<"\" element=\""<<source.element<<"\" charpropsname=\""<<escape_xml(description)<<"\" char_template=\""<<escape_xml(templ)<<"\" properties_row=\""<<selected<<"\" model_id=\""<<model_id<<"\" model_branch=\""<<escape_xml(model_branch)<<"\" asset=\""<<escape_xml(asset)<<"\" position=\""<<std::setprecision(9)<<position[0]<<','<<position[1]<<','<<position[2]<<"\"";
  for(const char* key:{"rotation","scale"}){auto value=original->source().attribute(key);export_file<<' '<<key<<"=\"";if(std::string(key)=="scale"&&derived_scale)export_file<<scale[0]<<','<<scale[1]<<','<<scale[2];else export_file<<escape_xml(value?*value:(std::string(key)=="scale"?"1,1,1":"0,0,0"));export_file<<'"';}export_file<<"/>\n";
  if(status=="nonvisual")++nonvisual;else if(asset.empty())++blocked;else if(*kind=="Character")++exported_characters;else ++exported_chests;
 }
 require(accounted_characters==50&&accounted_chests==5&&faery_cases==1,"V40 all original SWAMP characters/chests/DefaultFairy not accounted");
 require(application->channel(0).seed==production_rng_before.seed&&application->channel(0).calls==production_rng_before.calls&&application->channel(1).seed==production_rng1_before.seed&&application->channel(1).calls==production_rng1_before.calls,"V40 inspection consumed production application RNG");
 export_file<<"<Coverage accounted_characters=\""<<accounted_characters<<"\" accounted_chests=\""<<accounted_chests<<"\" characters=\""<<exported_characters<<"\" chests=\""<<exported_chests<<"\" nonvisual=\""<<nonvisual<<"\" blocked_visuals=\""<<blocked<<"\" default_faery_cases=\""<<faery_cases<<"\" selected_templates=\""<<templates<<"\" inspection_rng_calls=\""<<inspection_application->channel(0).calls<<"\" production_rng_unchanged=\"true\" whole_init_rng_order_verified=\"false\" rendered=\"false\" playable=\"false\"/>\n</ObjectPreview>\n";
 export_file.close();require(bool(export_file),"V40 export write failed");
