namespace dh2::world {
bool RetainedModuleVisualV3::borrow_scene_node_v109(std::uintptr_t id,RetainedSceneNodeBorrowV109& out,std::string& e){
 out={};if(!ready_||!root_present_||!root_)return missing("live actual Module visual root",e);
 const auto weak=weak_from_this();const auto expected=std::weak_ptr<void>(root_);
 auto current=[weak,expected](std::string& e){auto visual=weak.lock();auto root=expected.lock();
  if(!visual||!visual->ready_||!visual->root_present_||!root||visual->root_.get()!=root.get()||
     visual->root_.owner_before(root)||root.owner_before(visual->root_)){
   e="Retired or replaced actual Module visual root";return false;
  }return true;
 };
 return root_scene().borrow_scene_node_v109(root_,std::move(current),id,out,e);
}
}
