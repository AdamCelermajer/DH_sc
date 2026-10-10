#include "skill_ui.hpp"
#include "original_skill_art.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::skill_ui;
using Raw=std::vector<std::uint8_t>;
Raw read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("missing actual skill input: "+p);return {std::istreambuf_iterator<char>(f),{}};}
void check_at(bool b,int line){if(!b)throw std::runtime_error("skill UI test failed line "+std::to_string(line));}
#define check(x) check_at((x),__LINE__)
int main(int argc,char** argv){try{check(argc==2);std::string root=argv[1],error;auto data=read(root+"/skills_pyarray.bin"),names=read(root+"/skills_pyarraynames.bin"),schema=read(root+"/skills_pystructnames.bin");dh2::data::SkillTables owner;check(owner.load({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));auto tables=owner.borrow();check(tables.skills().size()==127&&tables.lists().size()==36);
 auto char_data=read(root+"/character_properties_pyarray.bin"),char_names=read(root+"/character_properties_pyarraynames.bin"),char_schema=read(root+"/character_properties_pystructnames.bin");dh2::data::CharacterTable characters;check(dh2::data::load_characters({char_data.data(),char_data.size()},{char_names.data(),char_names.size()},{char_schema.data(),char_schema.size()},characters,error));unsigned source_rows=0;int mismatch_row=-1,mismatch_list=-1;
 auto skill_tree=std::find(characters.fields.begin(),characters.fields.end(),"SkillTree");check(skill_tree!=characters.fields.end());auto skill_tree_column=std::size_t(skill_tree-characters.fields.begin());
 for(std::size_t row=0;row<characters.rows.size();++row){int list=-1;if(!source_skill_tree_metadata(characters,int(row),tables,list,error))continue;check(list==characters.rows[row][skill_tree_column]&&std::size_t(list)<tables.lists().size());++source_rows;if(mismatch_row<0){for(std::size_t other=0;other<tables.lists().size();++other)if(int(other)!=list){mismatch_row=int(row);mismatch_list=int(other);break;}}}
 check(source_rows>0&&mismatch_row>=0);int selected_native_list=mismatch_list;check(std::size_t(selected_native_list)<tables.lists().size()); // live Character list remains authoritative even when it differs from row metadata
 unsigned source_frame=99;check(original_class_frame_for_row(263,source_frame,error)&&source_frame==0);check(original_class_frame_for_row(325,source_frame,error)&&source_frame==1);check(original_class_frame_for_row(290,source_frame,error)&&source_frame==2);source_frame=99;check(!original_class_frame_for_row(264,source_frame,error)&&source_frame==99&&error.find("no class frame")!=std::string::npos);
 for(const char* name:{"Knight","KnightBerserker","KnightPaladin","Mage","MageIllusionist","MageNecromancer","Rogue","RogueArcher","RogueAssassin"})check(tables.list_index(name)>=0);
 // Service spies are interface fixtures, not reconstructed progression policy.
 int list=0,points=7,mutation_count=0,last_position=-1,last_slot=-1,probe_calls=0;bool allowed=true;Services s;
 s.character=[&](int& l,int& p,std::string&){l=list;p=points;return true;};s.progress=[&](int p,Progress& out,std::string&){out={p+1,true,allowed,allowed};return true;};s.slots=[](std::vector<int>& out,std::string&){out={-1,-1};return true;};
 s.increment=[&](int p,bool,std::string&){++mutation_count;last_position=p;return true;};s.probe_increment=[&](int p,bool& accepted,std::string&){++probe_calls;last_position=p;accepted=allowed;return true;};s.assign=[&](int slot,int p,std::string&){++mutation_count;last_slot=slot;last_position=p;return true;};
 Presenter ui(tables,s);View view;unsigned checked_lists=0,checked_rows=0;
 list=selected_native_list;check(ui.view(view,error)&&view.list_id==selected_native_list); // source Character skill-list value is authoritative over saved-row metadata
 for(list=0;list<36;++list){check(ui.view(view,error));check(view.rows.size()==tables.lists()[list].size());for(std::size_t i=0;i<view.rows.size();++i){const auto& r=view.rows[i];check(r.position==int(i)&&r.table_id==tables.lists()[list][i]&&r.source_icon==tables.skills()[r.table_id].icon&&r.source_name==tables.skill_names()[r.table_id]);++checked_rows;}++checked_lists;}
 list=0;while(tables.lists()[list].empty())++list;check(ui.view(view,error)&&!view.rows.empty());check(ui.select(0,error));bool probe_result=false;check(ui.probe_training(probe_result,error)&&probe_result&&probe_calls==1);check(ui.train(true,error)&&mutation_count==0&&probe_calls==2);check(ui.train(false,error)&&mutation_count==1&&last_position==0);check(ui.assign(1,error)&&mutation_count==2&&last_slot==1&&last_position==0);allowed=false;probe_result=true;check(ui.probe_training(probe_result,error)&&!probe_result&&probe_calls==3&&error.empty());check(!ui.train(true,error)&&!ui.assign(0,error)&&mutation_count==2);check(!ui.assign(-1,error));check(!ui.select(-1,error));check(!ui.select(int(view.rows.size()),error));
 list=-1;check(ui.view(view,error)&&view.list_id==3);list=999;check(ui.view(view,error)&&view.list_id==3);allowed=true;list=tables.list_index("Knight");check(!ui.train(false,error)&&mutation_count==2);
 // The live menu supplies static localized labels; this presenter replaces
 // only skill-owned fields and must leave native details blank until selected.
 Services connected_services=s;connected_services.source_class_frame=[](unsigned& cf,std::string&){cf=0;return true;};int detail_calls=0;connected_services.selected_texts=[&](int position,std::vector<std::pair<std::string,std::string>>& fields,std::string&){++detail_calls;for(const auto& f:dh::foundation::character_menu::original_menu_art(dh::foundation::character_menu::Tab::skills).text_fields)if(f.path.find("/SKILL_NAME/")!=std::string::npos||f.path.find("/skill_description/")!=std::string::npos||f.path.find("/current_skill_description/")!=std::string::npos||f.path.find("/next_skill_description/")!=std::string::npos)fields.emplace_back(f.path,f.path+":"+std::to_string(position));return true;};
 Presenter connected(tables,connected_services);
 dh::foundation::character_menu::Frame frame;
 auto& static_fields=dh::foundation::character_menu::original_menu_art(dh::foundation::character_menu::Tab::skills).text_fields;
 auto label=std::find_if(static_fields.begin(),static_fields.end(),[](const auto& f){return f.path.find("GAMEPLAYMENUS_SKILL_POINTS")!=std::string::npos;});
 check(label!=static_fields.end());frame.text.push_back({*label,"localized static label"});
 auto name_field=std::find_if(static_fields.begin(),static_fields.end(),[](const auto& f){return f.path.find("/SKILL_NAME/")!=std::string::npos;});
 check(name_field!=static_fields.end());frame.text.push_back({*name_field,"stale detail"});
 if(!connected.append(frame,error))throw std::runtime_error(error);
 check(detail_calls==0);check(std::any_of(frame.text.begin(),frame.text.end(),[](const auto& f){return f.value=="localized static label";}));check(std::none_of(frame.text.begin(),frame.text.end(),[](const auto& f){return f.field.path.find("/SKILL_NAME/")!=std::string::npos;}));check(std::none_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& b){return b.role.find("menu_SkillTreeSheetNew/btn_add/")==0;}));
 const auto& zones=original_skill_hit_zones(0);auto selected_zone=std::find_if(zones.begin(),zones.end(),[](const auto& z){return z.kind==HitKind::select;});check(selected_zone!=zones.end());
 const auto&a=selected_zone->triangles[0];const auto&b=selected_zone->triangles[1];const auto&c=selected_zone->triangles[2];check(connected.release((a.x+b.x+c.x)/3,(a.y+b.y+c.y)/3,error));check(connected.selected()&&*connected.selected()==selected_zone->position);
 const auto probes_before_selected_append=probe_calls;frame.text.clear();check(connected.append(frame,error)&&detail_calls==1&&probe_calls==probes_before_selected_append+1);
 const auto selected_suffix=":"+std::to_string(selected_zone->position);check(std::any_of(frame.text.begin(),frame.text.end(),[&](const auto& f){return f.field.path.find("/SKILL_NAME/")!=std::string::npos&&f.value.find(selected_suffix)!=std::string::npos;}));
 Presenter missing(tables,{});check(!missing.view(view,error)&&error.find("GetCharSkillListId")!=std::string::npos);Presenter empty({},s);check(!empty.view(view,error));s.slots=[](std::vector<int>& out,std::string&){out={999};return true;};Presenter invalid(tables,s);auto before=view.rows.size();check(!invalid.view(view,error)&&view.rows.size()==before);dh::foundation::character_menu::Frame missing_text_frame;check(!ui.append(missing_text_frame,error)&&error.find("localization")!=std::string::npos);
 std::cout<<"{\"validation\":\"PASS\",\"actual_source_class_rows\":"<<source_rows<<",\"actual_class_lists\":"<<checked_lists<<",\"actual_skill_references\":"<<checked_rows<<",\"mutation_spy_calls\":"<<mutation_count<<",\"source_probe_spy_calls\":"<<probe_calls<<",\"progression_policy_recovered\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}


