from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53'
p=b/'canonical_light_point_factory_v53.hpp';s=p.read_text().replace('const auto& records()const noexcept{return records_;}','''const auto& records()const noexcept{return records_;}
 // Call AFTER actual manager unpublication and transport.erased(identity).
 void erased(std::uintptr_t);''');p.write_text(s)
p=b/'canonical_light_point_factory_v53.cpp';s=p.read_text().replace('#include <cstring>','#include <cstring>\n#include <algorithm>');s=s.rsplit('\n}',1)[0]+'''
void CanonicalLightPointFactoryV53::erased(std::uintptr_t identity){
 records_.erase(std::remove_if(records_.begin(),records_.end(),[identity](const auto& record){return record->owner&&record->owner->identity()==identity;}),records_.end());
}
}
''';p.write_text(s)
(r/'stage9-binding-v53.inc.proposed').write_text(r'''// Candidate aggregation owns light factory/file/body as siblings of World.
// This is an integration FRAGMENT; actual engine owner validation is mandatory.
// All names/catalog/conditions/lease providers remain existing source services.

// In existing CanonicalLevelClassDispatchInputsV1.remaining:
if(entry.name&&std::string(entry.name)=="LightPoint"&&entry.original_address==0x34115c)
 return light_factory->construct(entry,source,receiver,error);
return existing_remaining(entry,source,receiver,error);

// After authentic state8 publication, obtain SAME retained config owner whose
// identity equals *candidate.level->config_fields().config38. Do not write38 here.
Stage9ConfigBorrowV53 fields;
if(!borrow_stage9_config_v53(candidate.level,actual_published_config,fields,error))return false;
CanonicalLightsetFileInputsV53 input;
input.archive=actual_mounted_archive;input.level=candidate.level;
input.candidate_owner=actual_candidate_owner;input.manager=&candidate.manager;
input.classes=actual_class_transport.services();input.source=actual_file_source_services;
input.actual_filesystem_owner=actual_source_fs_owner;
input.resolve_filename=actual_v52_filename_resolver;
input.validate_current=actual_level_manager_application_owner_check;
auto file=std::make_shared<CanonicalLightsetFileV53>(std::move(input));
Stage9ServicesV53 services;
services.actual_device_owner=actual_device_owner;
services.is_fixed_pipeline=actual_device_is_fixed_pipeline;
services.validate_current=actual_level_config_device_owner_check;
services.actual_file_loader_owner=file;
services.load_module_file_step=[file](const std::string& name,const char* tag,std::string& e){return file->step(name,tag,e);};
auto body=std::make_shared<Stage9BodyV53>(std::move(fields),std::move(services));
source_loading.external.stage_body[9]=[body](std::string& e){return body->step(e);};
// Keep file (XML/journal)/body/factory until genuine candidate cleanup. Do not
// mark stage10 complete from stage9 registration. Real InitPost must provide
// native CLightSceneNode/current renderer root/auto list/CLight storage/type and
// functional RefreshAttachment, using the same existing LightSetNameOwnerV3.
// Initial assigned stream140 must go through genuine LoadFile stream adapter;
// cache-file adapter rejects it explicitly. No extra file13c update is guessed.
''')
print('Unique cleanup/adoption fragment complete')
