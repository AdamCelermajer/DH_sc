#include "procedural_process_scratch_v69.hpp"
#include "procedural_xml_emission_v66.hpp"
#include <cstdlib>
#include <exception>
#include <memory>
namespace dh2::loader {
namespace {
// Process heap authority only. No containing Level/World/cache/parser owns
// this object, and it owns none of them. Its retained allocations have exactly
// the original leak lifetime: the OS/process heap eventually reclaims them.
// Avoid a per-allocation vector/node/control-block allocation between the
// original256 calls; no scratch contents are read or written by this arena.
struct ProcessPropertyScratchArenaV69 {};
const std::shared_ptr<ProcessPropertyScratchArenaV69>& process_arena(){
 // Deliberately retained through all global/app teardown, including service
 // callback disposal. Individual aliases must never free source scratch.
 static const auto* retained=new std::shared_ptr<ProcessPropertyScratchArenaV69>(
  std::make_shared<ProcessPropertyScratchArenaV69>());
 return *retained;
}
}
bool allocate_process_property_scratch256_v69(PropertyScratchV66& out,std::string& e){
 try {
  const auto& arena=process_arena();
  // Original3f3870/389998 call CustomAlloc256.310454 first calls malloc;
  // new(size,MemoryHintState)310570 tailbranches there without using hint.
  // Calling calloc/value-initializing storage would change the BX-LR output.
  auto* bytes=static_cast<std::uint8_t*>(std::malloc(256));
  if(!bytes){
   // Original NULL branch invokes allocation-failure logging/assertion. The
   // checked loader cannot fabricate a successful allocation/empty string.
   e="Original CustomAlloc256 libc malloc failed; native allocation diagnostic branch required";return false;
  }
  // Aliasing does not allocate a new control block or change native heap call
  // order. It never owns a free(bytes) deleter. Overwritten/dropped aliases
  // leave the exact original malloc allocation live for process lifetime.
  PropertyScratchV66 next;next.owner=std::shared_ptr<void>(arena,bytes);
  next.bytes=bytes;next.size=256;out=std::move(next);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool bind_process_property_scratch_v69(ProceduralXmlServicesV66& services,std::string& e){
 if(services.allocate_property_scratch256){e.clear();return true;}
 try {
  (void)process_arena(); // one setup allocation, outside descriptor traversal
  services.allocate_property_scratch256=allocate_process_property_scratch256_v69;
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
}
