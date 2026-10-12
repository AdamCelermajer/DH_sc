from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');p=b/'canonical_light_point_v53.hpp';s=p.read_text().replace('bool fail(const std::string&,std::string&);','bool fail(const std::string&,std::string&);\n bool write_light_parameters(const LightParameterFieldsV53&,std::string&);');p.write_text(s)
p=b/'canonical_light_point_v53.cpp';s=p.read_text();old=''' if(!services_.borrow_light_parameters)return fail("Required actual LightPoint CLight parameter storage",e);
 LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e))return fail(e,e);
 if(parameters.actual_light_identity!=node_.actual_light_identity)return fail("Foreign CLight parameter owner",e);
 if(!sync_data(parameters,e))return fail(e,e);''';new=''' bool write_parameters=true;
 if(!strings_.at(0x168).empty()){
  if(!services_.debug_switch)return fail("Required actual LightPoint TestMultiPlayerLight switch",e);
  if(!services_.debug_switch("TestMultiPlayerLight",write_parameters,e))return fail(e,e);
 }
 if(write_parameters){
  if(!services_.borrow_light_parameters)return fail("Required actual LightPoint CLight parameter storage",e);
  LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e))return fail(e,e);
  if(parameters.actual_light_identity!=node_.actual_light_identity)return fail("Foreign CLight parameter owner",e);
  if(!write_light_parameters(parameters,e))return fail(e,e);
 }''';assert old in s;s=s.replace(old,new)
s=s.replace(''' if(!f.owner||!f.actual_light_identity''',''' return write_light_parameters(f,e);
}
bool CanonicalLightPointV53::write_light_parameters(const LightParameterFieldsV53& f,std::string& e){
 if(!f.owner||!f.actual_light_identity''');p.write_text(s);print('Exact DAE/TestMultiPlayerLight parameter skip before borrowing storage preserved')
