namespace dh2::character {
int DH2_SCRIPT_OWNER_V105::source_release_active_v105(){
 auto& t=*impl_;const auto id=t.state.active;
 if(id){const auto found=t.sessions.find(id);
  if(found==t.sessions.end()){t.message="Required retained active AIS deleting destructor";return -2;}
  if(found->second->executing||dh2_script_vm_stack_size(found->second->vm)<0){t.message="Cannot release active AIS during its scoped VM callback";return -1;}
  t.sessions.erase(found); // Source D0 occurs before the active=NULL store.
  t.state.active=0;
 }
 // Exact 3ccad8..3ccaec resets; timer33/34 and delayed24 are untouched.
 t.state.external_name=0;t.state.active=0;t.state.pending=0;
 t.state.load_step=0;t.state.scripted=0;
 return 1;
}
}
