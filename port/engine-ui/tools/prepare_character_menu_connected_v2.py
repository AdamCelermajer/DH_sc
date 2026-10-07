"""Snapshot expanded gameplay fixture and replace BOTH AS sinks with real AS.

Preserves v1 sources and receipt. V2 lists allocate real arrays and rows; field
values are read back from those objects after the original typed callback.
"""
from pathlib import Path
import hashlib, json, re
ROOT=Path(__file__).resolve().parents[3]
HERE=ROOT/'port/engine-ui/tests'
source=HERE/'character_menu_composite_v1.cpp'
text=source.read_text()
for marker in ('struct ASHost {','struct ASListHost {'):
 begin=text.index(marker);end=text.index('\n};',begin)+3
 text=text[:begin]+text[end:]
text=text.replace('#include <cctype>','#include <cctype>\n#include "character_menu_connected_as_host_v2.hpp"')
needle='CharacterMenuQueriesOwnerV1 queries(qg);ASHost as;'
assert text.count(needle)==1
text=text.replace(needle,'CharacterMenuQueriesOwnerV1 queries(qg);ConnectedMenuASHostV2 as(queries,retained,assets+"/original-cache/data/menus");')
text=text.replace('queries.dispatch(', 'as.dispatch(')
text=text.replace('inventory_as.dispatch(', 'as.dispatch(inventory_queries,')
text=text.replace('missing_slot.dispatch(', 'as.dispatch(missing_slot,')
text=text.replace('missing.dispatch(', 'as.dispatch(missing,')
text=text.replace('ASListHost list_host;', 'ConnectedMenuListHostV2 list_host(as);')
needle='check(as.dispatch(inventory_queries,"NativeInvGetItemsListForSlot",call,error),error);'
assert text.count(needle)==1
text=text.replace(needle,needle+'list_host.observe();')
text=text.replace('AS_sink_and_offline_world_boundary_fixtures','startup_GPU_and_offline_world_boundary_fixtures')
text=text.replace('\\"complete_original_scripts\\":219,','\\"complete_original_scripts\\":219,\\"real_retained_AS_transport\\":true,\\"actual_AS_inventory_lists\\":true,\\"authored_character_menu_flow\\":false,')
out=HERE/'character_menu_connected_v2.cpp'
out.write_text('// Snapshot; both AS fixtures replaced with actual retained AS objects.\n'+text)
host=(HERE/'character_menu_connected_as_host_v1.hpp').read_text().replace('ConnectedMenuASHostV1','ConnectedMenuASHostV2')
host=host.replace('#include <algorithm>','#include <algorithm>\n#include <map>')
host=host.replace('if(identity==host->object)host->members.emplace_back(member,value);','if(identity==host->object)host->members.emplace_back(member,value);\n     else if(host->list_open)host->row_members[identity].emplace_back(member,value);')
host=host.replace(' unsigned callbacks=0;',' bool list_open=false;\n std::map<std::uintptr_t,std::vector<std::pair<std::string,dh2::ui::CharacterMenuValueV1>>> row_members;\n unsigned callbacks=0;')
host=host.replace('"NativeInvUnequipItem","NativeInvEquipItem","NativeSwapEquipment"','"NativeInvUnequipItem","NativeInvEquipItem","NativeSwapEquipment",\n   "NativeInvGetItemsListForSlot","NativeInvAutoEquipSlot","NativeHUDGetIsFaeryUnlocked"')
host=host.replace(' dh2::ui::CharacterMenuCallV1 call(',''' void reset_array(){
  std::string error;
  check(movie.action_script(this,[](void* p,dh2::ui::SwfAsGraph& graph,std::string& failure){
   auto& host=*static_cast<ConnectedMenuASHostV2*>(p);gameswf::as_object* root=nullptr;
   if(!graph.borrow_object(host.root_value,root,failure))return false;
   gameswf::gc_ptr<gameswf::as_array> rows=new gameswf::as_array(root->get_player());
   rows->push(gameswf::as_value(99));
   if(!graph.retain_object(rows.get_ptr(),host.array_value,failure))return false;
   host.array=host.array_value.identity();host.elements.clear();return true;
  },error),error);
 }
 dh2::ui::CharacterMenuCallV1 call(''')
host+='''
struct ConnectedMenuListHostV2 {
 ConnectedMenuASHostV2& host;
 std::uintptr_t array;
 std::map<std::uintptr_t,std::vector<std::pair<std::string,dh2::ui::CharacterMenuValueV1>>> rows;
 std::vector<dh2::ui::CharacterMenuValueV1> elements;
 explicit ConnectedMenuListHostV2(ConnectedMenuASHostV2& h):host(h){
  host.reset_array();array=host.array;host.row_members.clear();host.list_open=true;
 }
 ~ConnectedMenuListHostV2(){host.list_open=false;host.row_members.clear();host.reset_array();}
 dh2::ui::CharacterMenuCallV1 call(unsigned slot){
  using V=dh2::ui::CharacterMenuValueV1;
  return host.call({V::numeric(slot),V::reference(array),V::numeric(0)});
 }
 void observe(){
  elements=host.elements;std::string error;
  check(host.movie.action_script(this,[](void* p,dh2::ui::SwfAsGraph& graph,std::string& failure){
   auto& list=*static_cast<ConnectedMenuListHostV2*>(p);
   gameswf::as_object* object=nullptr;
   if(!graph.borrow_object(list.host.array_value,object,failure))return false;
   auto* array=static_cast<gameswf::as_array*>(object);
   for(std::size_t i=1;i<array->m_array.size();++i){
    const auto& entry=array->m_array[i];check(entry.is_object());
    auto* row=entry.to_object();auto identity=reinterpret_cast<std::uintptr_t>(row);
    dh2::ui::SwfAsValue retained;
    if(!graph.retain_object(row,retained,failure))return false;
    auto found=list.host.row_members.find(identity);check(found!=list.host.row_members.end());
    auto& values=list.rows[identity];
    for(const auto& written:found->second){
     dh2::ui::SwfAsValue actual;bool exists=false;dh2::ui::CharacterMenuValueV1 value;
     if(!graph.get_member(retained,written.first.c_str(),actual,exists,failure)||!exists||
        !ConnectedMenuASHostV2::observe(graph,actual,value,failure))return false;
     values.emplace_back(written.first,std::move(value));
    }
   }
   return true;
  },error),error);
 }
};
'''
host_out=HERE/'character_menu_connected_as_host_v2.hpp'
host_out.write_text(host)
assert 'ASListHost' not in text and 'ASHost as;' not in text
receipt=ROOT/'.local-inputs/character-menu-connected-v2-host/fixture-source.json'
receipt.parent.mkdir(parents=True,exist_ok=True)
receipt.write_text(json.dumps({'source':str(source.relative_to(ROOT)),
 'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
 'generated':{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (out,host_out)},
 'base_transport_sha256':hashlib.sha256((HERE/'character_menu_connected_as_host_v1.hpp').read_bytes()).hexdigest(),
 'limits':'Actual AS fields/lists; authored screen flow and live Android not exercised'},indent=2)+'\n')
print(json.dumps({'validation':'PASS','fixture':str(out),'AS_fixtures_replaced':2}))
