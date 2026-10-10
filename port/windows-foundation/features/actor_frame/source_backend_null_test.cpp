#include "source_campaign_backend_v1.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_character_interaction_v114.hpp"
#include <cstdio>
int main(){
 std::string error="stale";std::uintptr_t output=42;
 if(!model_renderer::source_campaign_object_as_character_v114({},0,output,error)||output||!error.empty())return 1;
 if(model_renderer::source_campaign_object_as_character_v114({},1,output,error)||output||error.find("Required registered SourceCampaignBackendContextV1")==std::string::npos)return 2;
 std::puts("PASS actual exported IsCharacter null and missing-context branches");
}
