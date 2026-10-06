// Inspection export of the actual retained constructor journal. Detached
// property recalculation resolves original model references without claiming
// Character InitPost/activation success or mutating its canonical receiver.
 auto escape_xml=[](const std::string& in){std::string out;for(char c:in){switch(c){case '&':out+="&amp;";break;case '<':out+="&lt;";break;case '>':out+="&gt;";break;case '"':out+="&quot;";break;default:out+=c;}}return out;};
 std::ofstream export_file(argv[4]);require(bool(export_file),"Preview export file unavailable");
 export_file<<"<ObjectPreview identity=\"SWAMP\" scope=\"canonical constructor journal; authored inspection; whole activation ordering pending\">\n";
 unsigned exported_characters=0,exported_chests=0,blocked_visuals=0;
 CharacterTemplateAssetsV35 template_assets;
 {bool exists{};std::vector<std::uint8_t> rows,names,schema;require(archive.read("data/pydata/character_templates_pyarray.bin",exists,rows,e)&&exists,e);require(archive.read("data/pydata/character_templates_pyarraynames.bin",exists,names,e)&&exists,e);require(archive.read("data/pydata/character_templates_pystructnames.bin",exists,schema,e)&&exists,e);require(template_assets.load({rows.data(),rows.size()},{names.data(),names.size()},{schema.data(),schema.size()},e),e);}
 auto template_snapshot=template_assets.borrow();unsigned selected_templates=0;const auto template_rng_before=application->channel(0).calls;

 data::GameObjectDictionaryV11 preview_dictionary;
 {bool exists{};std::vector<std::uint8_t> rows,names;require(archive.read("data/pydata/game_objects_dictionary_pyarray.bin",exists,rows,e)&&exists,e);require(archive.read("data/pydata/game_objects_dictionary_pyarraynames.bin",exists,names,e)&&exists,e);require(preview_dictionary.load(rows.data(),rows.size(),names.data(),names.size(),e),e);}
 for(const auto* attempt:attempts){
  auto* factory=attempt->factory_attempt();if(!factory||factory->stage()!=CanonicalFactoryStageV1::complete)continue;
  auto* object=fixture.manager.object(factory->handle().key);auto* original=canonical_source_entry_v1(attempt->source().request());if(!object||!original)continue;
  auto kind=original->source().attribute("gametype");if(!kind||(*kind!="Character"&&*kind!="OpenableContainer"))continue;
  std::string asset,reason,name=*original->source().attribute("name");std::array<float,3> position{},visual_scale{{1.f,1.f,1.f}};bool derived_scale=false;std::int32_t selected_properties=-1;
  if(*kind=="Character"){
   auto record=character_factory->find(object->identity);require(record&&record->actor,"Preview lost canonical Character");require(record->actor->source_position(position,e),e);
   const auto& description=*record->actor->source_string(0x13b0);const auto& templ=*record->actor->source_string(0x1398);
   auto& design=record->design;
   if(!templ.empty()){
    // Inspection borrows the source-proved template/RNG branch. These local
    // caches do not activate or mutate the canonical Character receiver;
    // production callers must lend its actual13c8/13ca fields in Init order.
    std::int16_t props=-1,template_id=-1;require(template_snapshot.select(templ,props,template_id,application->channel(0),e),e);selected_properties=props;++selected_templates;
   }else{const auto& names=design.characters()->names;auto row=std::find(names.begin(),names.end(),description);if(row!=names.end())selected_properties=static_cast<std::int32_t>(row-names.begin());}
   if(selected_properties<0||static_cast<std::size_t>(selected_properties)>=design.characters()->rows.size())reason="Original Character direct/template row absent";
   else{data::PropertyState detached;data::reset_properties(*design.rules(),detached,&design.characters()->rows[selected_properties]);require(data::recalc_properties_with_class(*design.classes(),*design.rules(),detached,e),e);auto view=data::property_view(*design.rules(),detached);require(dh2_character_visual_scale(visual_scale.data(),view.base+12)==0,"Original Character visual scale rejected");derived_scale=true;
    auto ai_index=detached.resolved[1];if(ai_index<0||static_cast<std::size_t>(ai_index)>=design.ai()->rows.size())ai_index=8;require(static_cast<std::size_t>(ai_index)<design.ai()->rows.size(),"Actual Character AI fallback absent");auto ai_type=design.ai()->rows[ai_index].type;auto model=detached.resolved[3];if(ai_type==1||ai_type==3)reason="Actual player/faery model-name branch pending";else if(model<0||static_cast<std::size_t>(model)>=character_tables.models.values.size())reason="Actual Character model dictionary entry absent";else asset=character_tables.models.values[model];
   }
  }else{
   auto chest=std::find_if(containers.begin(),containers.end(),[&](const auto& c){return c->receiver().base().identity()==object->identity;});require(chest!=containers.end(),"Preview lost canonical chest");
   std::copy_n((*chest)->receiver().base().vector3(0x160),3,position.begin());OpenableContainerRowV1 row;std::int32_t id{};
   if(!chest_data->resolve_row((*chest)->receiver().fields().data_desc,id,row,e))reason=e;
   else {const std::string* file{};if(!preview_dictionary.file(row.visual,file,e)||!file)reason=e;else asset=*file;}
  }
  if(!reason.empty()||asset.empty()){++blocked_visuals;export_file<<"<Blocked name=\""<<escape_xml(name)<<"\" kind=\""<<*kind<<"\" reason=\""<<escape_xml(reason)<<"\"/>\n";continue;}
  export_file<<"<Entity name=\""<<escape_xml(name)<<"\" kind=\""<<*kind<<"\" asset=\""<<escape_xml(asset)<<"\" position=\""<<std::setprecision(9)<<position[0]<<','<<position[1]<<','<<position[2]<<"\"";
  if(selected_properties>=0)export_file<<" properties_row=\""<<selected_properties<<"\"";
  for(const char* key:{"rotation","scale"}){auto value=original->source().attribute(key);export_file<<' '<<key<<"=\"";if(std::string(key)=="scale"&&derived_scale)export_file<<std::setprecision(9)<<visual_scale[0]<<','<<visual_scale[1]<<','<<visual_scale[2];else export_file<<escape_xml(value?*value:(std::string(key)=="scale"?"1,1,1":"0,0,0"));export_file<<"\"";}
  export_file<<"/>\n";if(*kind=="Character")++exported_characters;else ++exported_chests;
 }
 export_file<<"<Coverage characters=\""<<exported_characters<<"\" chests=\""<<exported_chests<<"\" blocked_visuals=\""<<blocked_visuals<<"\" constructed=\""<<supported_objects<<"\" next_constructor=\""<<escape_xml(stopped_type)<<"\"/>\n</ObjectPreview>\n";
 export_file<<"<!-- template branch selections="<<selected_templates<<" application_rng_calls_before="<<template_rng_before<<" after="<<application->channel(0).calls<<"; inspection ordering, whole Character activation pending -->\n";
 export_file.close();require(bool(export_file),"Preview export write failed");
