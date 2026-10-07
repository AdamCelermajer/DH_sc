//Included after the SAME CharacterMeshFxOwnerV4::Impl and source marker code.
bool CharacterMeshFxOwnerV4::marker_rotation_v70(std::uintptr_t identity,const float source_rotation[3],std::string& e){
 auto& s=*impl_;auto at=s.records.find(identity);
 if(!source_rotation||at==s.records.end()||at->second->pooled){e="Required SAME active AnimatedFX raw rotation34 store";return false;}
 std::copy_n(source_rotation,3,at->second->state.rotation);e.clear();return true;
}
bool CharacterMeshFxOwnerV4::marker_store_anchor_v83(std::uintptr_t identity,std::uintptr_t anchor,std::string& e){
 auto& s=*impl_;auto at=s.records.find(identity);
 if(at==s.records.end()||at->second->pooled){e="Required SAME active AnimatedFX source anchor28";return false;}
 at->second->state.anchor=anchor;e.clear();return true;
}
bool CharacterMeshFxOwnerV4::marker_sync_v83(std::uintptr_t identity,bool reset,std::string& e){
 auto& s=*impl_;s.failure.clear();auto at=s.records.find(identity);
 if(at==s.records.end()||at->second->pooled){e="Required SAME active AnimatedFX SyncIrrData";return false;}
 if(!s.sync(*at->second,reset)){e=s.failure;return false;}e.clear();return true;
}
bool CharacterMeshFxOwnerV4::marker_visual_owner_v76(std::uintptr_t identity,std::uintptr_t owner,std::string& e){
 auto& s=*impl_;auto at=s.records.find(identity);
 if(at==s.records.end()||at->second->pooled){e="Required SAME active AnimatedFX for visual204 store";return false;}
 const auto& r=*at->second;
 if(!r.state.visual){e.clear();return true;} //original optional visual2c branch
 if(!r.resource||r.state.visual!=reinterpret_cast<std::uintptr_t>(r.resource.get())){
  e="AnimatedFX visual204 belongs to a different retained visual resource";return false;
 }
 r.resource->source_owner204=owner; //47d9f8 /47db1c, including genuine NULL clear
 e.clear();return true;
}
bool CharacterMeshFxOwnerV4::marker_loop_v76(std::uintptr_t identity,bool looping,std::string& e){
 auto& s=*impl_;auto at=s.records.find(identity);
 if(at==s.records.end()||at->second->pooled||!at->second->state.visual||!at->second->resource){
  e="Required SAME AnimatedFX.GetAnimator visual/controller for loop suffix";return false;
 }
 auto& r=*at->second;
 //Existing controller samples both the retained mesh and particle composite.
 //SetLooping does not reset play position or emission history.
 if(!r.timeline.library_present||dh2_timeline_loop(&r.timeline,looping?1:0)){
  e="Actual marker animator SetLooping rejected its source timeline";return false;
 }
 e.clear();return true;
}
