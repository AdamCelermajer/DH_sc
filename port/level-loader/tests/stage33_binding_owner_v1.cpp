#include "stage_loader_save_restore_v1.hpp"
#include <iostream>

using namespace dh2::loader;

int main(){
 std::string error;
 std::shared_ptr<CanonicalLevelContextV1> level;
 LevelSourceRequestV1 request;request.identity="stage33-binding";request.definition="test.mlx";
 auto level_services=std::make_shared<int>(1);
 if(!CanonicalLevelContextV1::create(request,level_services,level,error))return 1;
 auto application=std::make_shared<int>(2);

 Stage33RestoreServicesV1 services;services.actual_application=application;
 services.owner=std::shared_ptr<void>(level,level.get());
 LifecycleServicesV36 loading;
 if(bind_stage33_restore_v1(loading,level,services,error))return 2;
 if(error.find("independent restore authority")==std::string::npos)return 3;

 services.owner=std::make_shared<int>(3);
 services.actual_application=std::shared_ptr<void>(services.owner,services.owner.get());
 error.clear();
 if(bind_stage33_restore_v1(loading,level,services,error))return 4;
 if(error.find("independent restore authority")==std::string::npos)return 5;

 services.owner=std::make_shared<int>(4);services.actual_application=application;
 error.clear();
 if(!bind_stage33_restore_v1(loading,level,services,error))return 6;
 if(!loading.stage_body[33])return 7;
 std::cout<<"Stage33 binding rejects Level/Application alias owners and accepts an independent provider\n";
 return 0;
}
