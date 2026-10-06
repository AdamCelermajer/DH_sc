struct EffectGpuTextureV4 {
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> resource_budget_v40;
 dh2::resources::CpuAdmissionV40 decoded_cpu_v40;
 dh2::resources::ResourceTokenV37 texture_budget_v40;
 std::uint64_t texture_generation_v40{};
 GLuint texture{};dh2::textures::GeneralFxTextureImageOwnerV2 image;
 dh2::textures::TextureParameterOwnerV1& owner(){return image.parameters();}
};
