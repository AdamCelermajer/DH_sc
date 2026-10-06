// Include inside renderer namespace after real selected-world/manager helpers.
// Caller retains owner on SAME WorldScriptContext until camera/scene teardown.
bool prepare_gameplay_camera_config_v20(
 dh2::world::SelectedWorldCameraConfigV20& owner,const std::string& selected_mlx_uri,
 std::vector<std::uint8_t> actual_selected_mlx,
 dh2::world::CanonicalPropertyMapV1& same_properties,
 dh2::world::CanonicalObjectManagerV1& same_manager,
 dh2::world::SelectedWorldCameraServicesV20 services,
 dh2::camera::CameraLevelConfigV19& output,std::string& error){
 return owner.load(selected_mlx_uri,std::move(actual_selected_mlx),same_properties,same_manager,std::move(services),error)&&owner.camera(output,error);
}
