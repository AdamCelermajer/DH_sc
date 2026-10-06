#include "render_eligibility_v40.hpp"
#include <scene.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
static void require(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
int main(int argc,char** argv){if(argc!=2)return 2;try{
 std::ifstream f(argv[1],std::ios::binary);require(bool(f),"gold unavailable");auto w=[&](){std::uint32_t v;f.read(reinterpret_cast<char*>(&v),4);require(bool(f),"gold truncated");return v;};
 auto text=[&](){std::string s(w(),'\0');f.read(s.data(),s.size());require(bool(f),"gold string truncated");return s;};
 auto registrations=w();for(unsigned i=0;i<registrations;++i){auto flags=w(),culled=w(),expected=w();require(dh2::loader::source_node_registration_gate_v40(flags,culled)==bool(expected),"native registration gate differs from original");}
 auto renders=w();for(unsigned i=0;i<renders;++i){auto byte=w(),expected=w();require(dh2::loader::source_rigid_mesh_render_gate_v40(byte)==bool(expected),"native source byte138 render gate differs from original");}
 auto helpers=w();for(unsigned i=0;i<helpers;++i){dh2::scene::Scene s;auto count=w();for(unsigned j=0;j<count;++j){dh2::scene::Node n;n.parent=std::int32_t(w());n.name=text();s.graph.push_back(n);}auto expected=w();std::uint32_t selected=123;std::string e;require(dh2::loader::find_source_visual_helper_v40(s,selected,e)&&e.empty()&&selected==expected,"native helper finder differs from original first DFS prefix");
  if(!s.graph.empty()){s.graph.front().parent=42;selected=123;require(!dh2::loader::find_source_visual_helper_v40(s,selected,e)&&selected==123,"invalid helper graph corrupted output");}
 }


 auto pass_cases=w();for(unsigned i=0;i<pass_cases;++i){auto mode=w(),technique=w(),flags=w(),primary=w(),secondary=w(),prepare=w();
  const auto actual=dh2::loader::source_mesh_pass_projection_v40(mode,technique,flags);
  require(actual.primary_pass==primary&&actual.secondary_pass==secondary&&actual.prepare_only==bool(prepare),"native material-pass projection differs from whole original onRegister");
 }
 auto lease=std::make_shared<int>(0);std::uint32_t flags=0;dh2::loader::SourceNodeRegistrationResultV40 result;
 std::string hidden_error;require(dh2::loader::deliver_source_node_registration_v40({lease,123,&flags},{},result,hidden_error),"hidden source node demanded unrelated renderer services");
 std::string error;flags=1;result.visit_children=true;
 require(!dh2::loader::deliver_source_node_registration_v40({lease,123,&flags},{},result,error)&&result.visit_children,"missing real camera service defaulted/corrupted output");
 unsigned culls=0,enrolled=0;
 dh2::loader::SourceNodeRegistrationServicesV40 services;services.provider_owner=lease;
 services.is_culled=[&](std::uintptr_t id,bool& culled,std::string&){require(id==123,"foreign registration receiver");++culls;culled=true;return true;};
 require(dh2::loader::deliver_source_node_registration_v40({lease,123,&flags},services,result,error)&&result.culled&&!result.on_register_delivered,"culled source node fabricated enrollment");
 services.is_culled=[&](std::uintptr_t,bool& culled,std::string&){++culls;culled=false;return true;};
 require(!dh2::loader::deliver_source_node_registration_v40({lease,123,&flags},services,result,error),"visible source node defaulted missing material/pass service");
 services.on_register=[&](std::uintptr_t,bool& visit,std::string&){++enrolled;visit=true;return true;};
 require(dh2::loader::deliver_source_node_registration_v40({lease,123,&flags},services,result,error)&&result.on_register_delivered&&result.visit_children&&enrolled==1&&error.empty(),"actual registration continuation was not delivered");
 std::cout<<"PASS original_registration_cases="<<registrations<<" original_render_byte_cases="<<renders<<" original_helper_search_cases="<<helpers<<" original_material_pass_cases="<<pass_cases<<" invalid_graph_preserves_out=1 whole_render_pass_enrollment=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
