// Include after EquipmentPlatform and its sole current equipment_platform
// declaration. Headers: gameplay_camera_application_v23.hpp and <map>.
// This type directory only validates the native opaque integration slot;
// App's slot owns the one context, and actual World owns the one session.
struct CameraServicesDirectoryRecordV20 {
 std::weak_ptr<dh2::camera::GameplayCameraApplicationV23> owner;
 std::string initialization_failure;
};
static std::map<std::uintptr_t,CameraServicesDirectoryRecordV20>& camera_services_directory_v20(){
 static std::map<std::uintptr_t,CameraServicesDirectoryRecordV20> actual_types;return actual_types;
}
static bool read_gameplay_camera_resource_v20(const std::string& name,std::vector<std::uint8_t>&bytes,std::string&error){
 if(!equipment_platform){error="Required current actual OriginalCacheAsset provider for camera resource";return false;}
 bool found=false;if(!equipment_platform->cache.read(name,found,bytes,error))return false;
 if(!found){error="Original camera resource absent: "+name;return false;}return true;
}
static bool borrow_gameplay_camera_services_v20(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&app,
 dh2::camera::CameraApplicationBindingsV23 bindings,
 std::shared_ptr<dh2::camera::GameplayCameraApplicationV23>&out,std::string&error){
 if(!app){error="Required actual retained native Application camera owner";return false;}
 auto& directory=camera_services_directory_v20();auto existing=app->native_camera_services_v20();
 if(existing){auto known=directory.find(app->identity());if(known==directory.end()){error="Unrecognized Application camera integration slot producer";return false;}auto typed=known->second.owner.lock();if(!typed||typed.get()!=existing.get()){error="Application camera slot identity changed outside its typed producer";return false;}out=std::move(typed);if(!out->ready()){error=known->second.initialization_failure.empty()?"Required initialized Application camera service prefix":known->second.initialization_failure;return false;}return true;}
 bindings.application=app;bindings.read=read_gameplay_camera_resource_v20;
 auto context=std::make_shared<dh2::camera::GameplayCameraApplicationV23>();
 if(!app->publish_native_camera_services_v20(context,error))return false;
 auto& record=directory[app->identity()];record.owner=context;out=context;
 if(!context->initialize(std::move(bindings),error)){record.initialization_failure=error;return false;}
 return true;
}
