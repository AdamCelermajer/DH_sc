// Included by each actual owner implementation; does not create an owner.
namespace dh2::character::skills {
int DH2_SKILL_OWNER_V105::source_destroy_instances_v105(std::uint32_t kind){
 if(kind>1)return -1;auto& t=*impl_;
 const auto end=t.slots[kind].size();
 for(std::size_t i=0;i<end;++i){
  const auto* instance=t.slots[kind][i];if(!instance)continue;
  const auto owned=std::find_if(t.instances.begin(),t.instances.end(),[instance](const auto& p){return &p->value==instance;});
  if(owned==t.instances.end()){t.error="AIUnLoadScriptProcess deleting destructor received an unowned instance";return -2;}
  t.instances.erase(owned); // D0 precedes the source slot NULL store.
  t.slots[kind][i]=nullptr;t.sync();
 }
 t.slots[kind].clear();t.sync(); // end=begin; allocation/capacity retained.
 return 1;
}
}
