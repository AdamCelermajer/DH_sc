from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
p=b/'canonical_light_point_v53.hpp';s=p.read_text().replace('struct LightPointInitServicesV53 {','''struct LightParameterFieldsV53 {
 std::shared_ptr<void> owner;std::uintptr_t actual_light_identity{};
 std::array<float,4>* ambient4{};std::array<float,4>* diffuse14{};std::array<float,4>* specular24{};
 std::array<float,3>* attenuation34{};float* radius40{};
 std::function<bool(std::string&)> validate_current;
};
struct LightPointInitServicesV53 {''').replace('std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,std::string&)> sync_data;','''std::function<bool(const LightNodeBorrowV53&,LightParameterFieldsV53&,std::string&)> borrow_light_parameters;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;''').replace('bool init_post(std::string&);','bool init_post(std::string&);\n bool sync_data(const LightParameterFieldsV53&,std::string&);');p.write_text(s)
p=b/'canonical_light_point_v53.cpp';s=p.read_text().replace('if(error_.empty())error_=n;','if(error_.empty())error_=n.empty()?"Actual LightPoint service failed":n;').replace(''' if(!services_.sync_data)return fail("Required actual LightPoint SyncData CLight writer",e);
 if(!services_.sync_data(identity_,node_,e))return fail(e,e);''',''' if(!services_.borrow_light_parameters)return fail("Required actual LightPoint CLight parameter storage",e);
 LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e)||!sync_data(parameters,e))return fail(e,e);''')
fn=r'''
bool CanonicalLightPointV53::sync_data(const LightParameterFieldsV53& f,std::string& e){
 if(!strings_.at(0x168).empty()){
  if(!services_.owner||!services_.debug_switch)return needs("DebugSwitch isUsingColladaData",e);
  bool override=false;if(!services_.debug_switch("isUsingColladaData",override,e))return false;
  if(!override){e.clear();return true;} // exact source skip, not driver success
 }
 if(!f.owner||!f.actual_light_identity||!f.ambient4||!f.diffuse14||!f.specular24||!f.attenuation34||!f.radius40||!f.validate_current)return needs("SAME current retained CLight parameter fields",e);
 if(!f.validate_current(e))return false;
 auto* radius=at(floats_,0x130);if(!radius)return needs("produced radius",e);
 if(*radius>0.01f)*f.radius40=*radius; // strict comparison, source preserves near-zero radius
 *f.attenuation34=vectors_.at(0x134);
 for(auto pair:{std::pair<std::array<float,4>*,std::uint32_t>{f.ambient4,0x140},{f.diffuse14,0x14c},{f.specular24,0x158}}){
  const auto& v=vectors_.at(pair.second);*pair.first={v[0],v[1],v[2],1.f};
 }
 if(!f.validate_current(e))return false;e.clear();return true;
}
'''
s=s.rsplit('\n}',1)[0]+fn+'\n}\n';p.write_text(s)
print('Unique actual borrowed CLight scalar writer added')
