#include "native_gslevel_runtime_v27.hpp"
namespace dh2::loader {
bool NativeGSLevelRuntimeV27::prepare_loading(std::function<bool(std::uint32_t&,std::string&)> online,std::string& error){
 if(attempted_||loading_||!globals_){error="Required fresh retained GS Loading binding";return false;}
 loading_=std::make_shared<CanonicalLevelLoadingV26>(globals_->borrow(globals_),std::move(online));
 error.clear();return true;
}
bool NativeGSLevelRuntimeV27::construct(LevelSourceRequestV1 request,GSLevelArgumentsV2 arguments,
 LevelConstructorApplicationV4 application,GSLevelServicesV2<CanonicalLevelContextV1> services,
 std::function<bool(std::uint32_t&,std::string&)> online,std::string& error){
 if(attempted_||!globals_||globals_->s_level){error="Required fresh GS instance and sole empty global";return false;}
 if(!loading_&&!prepare_loading(std::move(online),error))return false;
 attempted_=true;fields_.arguments=std::move(arguments);
 gs_=std::make_unique<NativeGSLevelConnectionV26>(std::move(request),std::move(application),fields_,globals_->s_level,std::move(services));
 if(!gs_->construct()){error=gs_->lifecycle().error();return false;}
 return true;
}
bool NativeGSLevelRuntimeV27::destroy(std::string& error){
 if(!gs_){error="Required constructed GS instance for original destruction";return false;}
 if(!gs_->destroy()){error=gs_->lifecycle().error();return false;}return true;
}
bool NativeGSLevelRuntimeV27::abort_unpublished_constructor_v114(std::string& error){
 if(!globals_||fields_.level34||globals_->s_level){
  error="Published GS/Level requires its existing full retirement";return false;
 }
 if(!gs_){error.clear();return true;} // No connection was constructed.
 const auto& level=gs_->level_connection();
 if(level&&!level->abort_unpublished_constructor_v114(error))return false;
 // With actual field34 NULL, original GS Dtor only clears the SAME global.
 // No Level.Unload or full Level D1 is delivered to an incomplete receiver.
 if(!gs_->destroy()){error=gs_->lifecycle().error();return false;}
 error.clear();return true;
}
bool NativeGSLevelRuntimeV27::current(CanonicalCurrentLevelBorrowV1& out,std::string& error)const{
 if(!globals_){error="Required sole native GS global storage";return false;}
 return borrow_current_canonical_level_v1(globals_->borrow(globals_),out,error);
}
bool NativeGSLevelRuntimeV27::frame_fields_v45(GSLevelFieldsV2<CanonicalLevelContextV1>*& out,std::string& error){
 if(!gs_||gs_->lifecycle().stage()!=GSLevelLifecycleStageV2::constructed){error="Required SAME completed GS constructor for frame fields";return false;}
 out=&fields_;error.clear();return true;
}

}
