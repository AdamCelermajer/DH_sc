//Native process Arrays service, after original_ui is defined. No array load,
//singleton update, parsing or source stage advancement occurs in this borrower.
namespace model_renderer {
bool borrow_process_spawn_arrays_v108(std::shared_ptr<dh2::android_ui::SourceProcessArraysV101>& out,std::string& e){
 return original_ui.process_arrays_borrow_v101(out,e);
}
}
