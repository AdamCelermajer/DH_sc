#include "../native_batch_material_values_v113.hpp"
#include <cassert>

int main(){
 dh2::world::NativeBatchMaterialValuesV113 values;
 dh2::world::record_batch_sampler_binding_v113(values,"Sampler0","diffuse-sampler");
 dh2::world::record_batch_sampler_binding_v113(values,"Sampler1","normal-sampler");
 dh2::world::record_batch_sampler_binding_v113(values,"Sampler0","detail-sampler");
 assert(values.sampler_bindings.size()==2);
 assert(values.sampler_bindings.at("Sampler0")=="detail-sampler");
 assert(values.sampler_bindings.at("Sampler1")=="normal-sampler");
}
