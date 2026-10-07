#include "native_gslevel_frame_v45.hpp"
namespace dh2::loader {
NativeGSLevelFrameV45::NativeGSLevelFrameV45(std::shared_ptr<NativeGSLevelRuntimeV27> runtime,GSLevelFieldsV2<CanonicalLevelContextV1>* fields,GSApplicationBorrowV44 application,GSLevelUpdateServicesV44<CanonicalLevelContextV1> services):runtime_(std::move(runtime)),fields_(fields){
 services.borrow_level=[](const auto& level,auto& out,std::string& e){return borrow_gslevel_read_v44(level,out,e);};
 services.level_load=[](const auto& level,std::string& e){return actual_level_load_v44(level,e);};
 update_=std::make_unique<GSLevelUpdateV44<CanonicalLevelContextV1>>(runtime_,*fields_,std::move(application),std::move(services));
}
bool NativeGSLevelFrameV45::create(std::shared_ptr<NativeGSLevelRuntimeV27> runtime,GSApplicationBorrowV44 application,GSLevelUpdateServicesV44<CanonicalLevelContextV1> services,std::unique_ptr<NativeGSLevelFrameV45>& out,std::string& e){
 if(!runtime){e="Required retained actual GS runtime";return false;}
 if(services.borrow_level||services.level_load){e="GS frame C1 reads/Level.Load must have one actual provider";return false;}
 GSLevelFieldsV2<CanonicalLevelContextV1>* fields{};
 if(!runtime->frame_fields_v45(fields,e))return false;
 auto next=std::unique_ptr<NativeGSLevelFrameV45>(new NativeGSLevelFrameV45(std::move(runtime),fields,std::move(application),std::move(services)));
 out=std::move(next);e.clear();return true;
}
GSLevelUpdateResultV44 NativeGSLevelFrameV45::tick(){
 if(failed_)return GSLevelUpdateResultV44::failed;
 GSLevelFieldsV2<CanonicalLevelContextV1>* current{};
 if(!runtime_->frame_fields_v45(current,error_)||current!=fields_){failed_=true;required_="same constructed GS runtime fields";return GSLevelUpdateResultV44::failed;}
 const auto result=update_->tick();
 if(result==GSLevelUpdateResultV44::failed){failed_=true;error_=update_->error();required_=update_->required_service();return result;}
 if(!runtime_->frame_fields_v45(current,error_)||current!=fields_){failed_=true;required_="constructed GS lifetime after callback";return GSLevelUpdateResultV44::failed;}
 error_.clear();required_.clear();return result;
}
}
