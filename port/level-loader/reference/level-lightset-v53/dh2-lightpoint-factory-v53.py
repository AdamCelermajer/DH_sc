from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
(b/'canonical_light_point_factory_v53.hpp').write_text(r'''#pragma once
#include "canonical_light_point_v53.hpp"
#include <canonical_class_receiver_bindings_v1.hpp>
namespace dh2::loader {
struct CanonicalLightPointRecordV53 {
 std::unique_ptr<world::CanonicalLightPointV53> owner;
 std::shared_ptr<const void> declaration;
};
struct CanonicalLightPointFactoryInputsV53 {
 std::shared_ptr<void> world;
 std::function<bool(const world::CanonicalSourceObjectRequestV1&,const std::shared_ptr<CanonicalLightPointRecordV53>&,world::LightPointInitServicesV53&,std::string&)> services;
};
// Bind this to the existing remaining canonical factory dispatch before the
// unknown-class failure. SAME catalog factory34115c, no extra registry.
class CanonicalLightPointFactoryV53 final {
 CanonicalLightPointFactoryInputsV53 input_;
 std::vector<std::shared_ptr<CanonicalLightPointRecordV53>> records_;
public:
 explicit CanonicalLightPointFactoryV53(CanonicalLightPointFactoryInputsV53 i):input_(std::move(i)){}
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
 const auto& records()const noexcept{return records_;}
};
}
''')
(b/'canonical_light_point_factory_v53.cpp').write_text(r'''#include "canonical_light_point_factory_v53.hpp"
#include <cstring>
namespace dh2::loader {
bool CanonicalLightPointFactoryV53::construct(const world::CanonicalFactoryEntryV1& entry,const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 if(!entry.name||std::strcmp(entry.name,"LightPoint")||entry.original_address!=0x34115c){e="Require actual LightPoint catalog factory34115c";return false;}
 if(!input_.world||!source.source_lease){e="Required SAME LightPoint World/source declaration owner";return false;}
 auto record=std::make_shared<CanonicalLightPointRecordV53>();record->declaration=source.source_lease;
 world::LightPointInitServicesV53 s;s.owner=input_.world;
 if(input_.services&&!input_.services(source,record,s,e))return false;
 if(!s.owner||s.owner.owner_before(input_.world)||input_.world.owner_before(s.owner)||record->owner){e="LightPoint provider replaced World lease/replayed source constructor";return false;}
 record->owner=std::make_unique<world::CanonicalLightPointV53>(input_.world,std::move(s));
 auto receiver=std::shared_ptr<world::CanonicalLightPointV53>(record,record->owner.get());
 auto candidate=world::canonical_class_receiver_v1(receiver);candidate.source_lease=source.source_lease;
 candidate.init_post=[receiver](std::string& error){return receiver->init_post(error);};
 candidate.is_game_object=[](bool& value,std::string& error){value=false;error.clear();return true;};
 records_.push_back(std::move(record));out=std::move(candidate);e.clear();return true;
}
}
''')
# Add canonical native light identity matching to parameter borrow; test direct
# SyncData storage fixtures are separate and do not claim a real renderer node.
p=b/'canonical_light_point_v53.hpp';s=p.read_text().replace('std::uintptr_t identity{};};','std::uintptr_t identity{},actual_light_identity{};};');p.write_text(s)
p=b/'canonical_light_point_v53.cpp';s=p.read_text().replace('if(!node_.identity||!node_.owner)','if(!node_.identity||!node_.owner||!node_.actual_light_identity)').replace('LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e)||!sync_data(parameters,e))return fail(e,e);','''LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e))return fail(e,e);
 if(parameters.actual_light_identity!=node_.actual_light_identity)return fail("Foreign CLight parameter owner",e);
 if(!sync_data(parameters,e))return fail(e,e);''');p.write_text(s)
print('Unique actual GO19 catalog factory adapter written')
