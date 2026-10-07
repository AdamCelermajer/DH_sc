#include "native_resource_budget_v38.hpp"
#include <stdexcept>
#include <sstream>
namespace dh2::android_resources {
std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){
 // Combined authored main-menu/HUD/character-menu plus world textures exceed
 // the initial128MiB engineering texture ceiling in real menu-flow testing.
 // Keep a finite Android profile; portable ledger defaults and byte/count
 // checks are unchanged. This measures requested storage, not driver residency.
 static auto ledger=[](){resources::ResourceBudgetLimitsV37 limits;
  limits.gpu_bytes=512*resources::mib_v37;
  limits.texture_gpu_bytes=256*resources::mib_v37;
  return std::make_shared<resources::ContextResourceBudgetV37>(limits);
 }();return ledger;
}
resources::ContextResourceBudgetV37& budget_v38(){return *budget_lease_v39();}
void begin_context_v38(){std::string error;if(!budget_v38().begin_context(error))throw std::runtime_error("Native resource context: "+error);}
void context_lost_v38()noexcept{budget_v38().context_lost();}
void release_v38(resources::ResourceTokenV37& token){if(!token)return;std::string error;if(!budget_v38().release(token,error))throw std::runtime_error("Native resource release: "+error);}
std::string budget_report_v46(){
 const auto ledger=budget_lease_v39();const auto s=ledger->snapshot();const auto& limits=ledger->limits();
 std::ostringstream out;out<<"{\"admitted_storage_only\":true,\"driver_residency_measured\":false,\"generation\":"<<s.context_generation
  <<",\"limits\":{\"gpu\":"<<limits.gpu_bytes<<",\"textures\":"<<limits.texture_gpu_bytes<<",\"cpu\":"<<limits.cpu_bytes<<"}";
 const auto usage=[&out](const resources::ResourceUsageV37& u){
  out<<"{\"gpu\":"<<u.gpu_bytes<<",\"cpu\":"<<u.cpu_bytes<<",\"textures\":"<<u.texture_gpu_bytes<<",\"fx_buffers\":"<<u.fx_buffer_gpu_bytes;
  out<<",\"objects\":[";for(std::size_t i=0;i<u.objects.size();++i){if(i)out<<',';out<<u.objects[i];}
  out<<"],\"unknown_gpu_storage_objects\":[";
  for(std::size_t i=0;i<u.unknown_gpu_storage_objects.size();++i){if(i)out<<',';out<<u.unknown_gpu_storage_objects[i];}
  out<<"]}";
 };
 out<<",\"live\":";usage(s.live);out<<",\"pending\":";usage(s.pending);out<<",\"peak\":";usage(s.peak);
 out<<",\"records\":"<<s.occupied_records<<",\"rejections\":"<<s.rejections<<",\"scopes\":[";
 for(std::size_t i=0;i<s.live_by_scope.size();++i){if(i)out<<',';out<<"{\"scope\":"<<i<<",\"live\":";usage(s.live_by_scope[i]);out<<",\"peak\":";usage(s.peak_by_scope[i]);out<<'}';}
 out<<"]}";return out.str();
}
}
