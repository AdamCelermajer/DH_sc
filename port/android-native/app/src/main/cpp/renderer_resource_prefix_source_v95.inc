// Process bootstrap/borrow seams; no global replacement App or cache mount.
// Include in model_renderer TU after ApplicationServicesOwnerV5 declaration.
bool initialize_source_resource_prefix_v95(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& expected,std::string& e){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> actual;
 if(!borrow_actual_application_services_v5(actual,e)||!expected||actual!=expected){if(e.empty())e="Required SAME actual process App for native appInit prefix";return false;}
 return actual->initialize_source_resource_prefix_v95(e);
}
bool borrow_source_resource_prefix_v95(std::string& out,std::string& e){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> actual;
 if(!borrow_actual_application_services_v5(actual,e)||!actual)return false;
 dh2::application::ProcessResourcePrefixBorrowV95 prefix;
 if(!actual->borrow_source_resource_prefix_v95(prefix,e)||!prefix)return false;
 // Native SetLevelRouting's own stack strcpy is a string copy. This callback
 // pins the SAME original byte owner until that copy completes, not a World.
 out.assign(prefix.data);e.clear();return true;
}
