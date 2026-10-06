#include "generic_renderer_root_consumer_v42.hpp.proposed"
#include <type_traits>
static_assert(std::is_same_v<decltype(model_renderer::GenericNativeRenderFrameBorrowV42{}.actual_level_identity),std::uintptr_t>);
void check_required_callback_types(const model_renderer::GenericRootRenderServicesV42& source,
 const dh2::loader::ModuleDrawFrameV1& module,const dh2::loader::ActorDrawPrimitiveReadV38& actor){
 model_renderer::GenericNativeRenderFrameBorrowV42 frame;std::string error;
 if(source.borrow_frame&&source.consume_module&&source.consume_actor_primitive&&source.finish_frame){
  if(source.borrow_frame(123,frame,error)){source.consume_module(frame,module,error);source.consume_actor_primitive(frame,actor,error);source.finish_frame(frame,error);}
 }
}
