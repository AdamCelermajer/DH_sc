#include "save_writer_binding_v1.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::shared_ptr<dh::foundation::campaign_save::SourceSaveWriterBindingV1> binding;
    std::string error;
    if (dh::foundation::campaign_save::SourceSaveWriterBindingV1::bind({}, binding, error))
        throw std::runtime_error("Null candidate unexpectedly received whole-profile Save authority");
    if (binding || error != "Required SAME canonical player candidate for whole-profile save")
        throw std::runtime_error("Missing candidate did not fail explicitly before Save binding");
    std::cout << "{\"validation\":\"PASS\",\"missing_candidate_rejection\":1,\"new_profile_or_codec\":false}\n";
}
