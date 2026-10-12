{
 // Insert after V45 positive pin/lifetime tests but BEFORE callback release.
 // Include byte_free_observer_v46.hpp at file scope and link its .cpp in this
 // isolated HOST target only. Existing V45 admission endpoints remain fixtures.
 struct V46Events {bool watch{},filesystem_released{},admission_released{};};
 struct V46FilesystemLease {
  std::shared_ptr<void> actual_archive;
  std::shared_ptr<V46Events> events;
  ~V46FilesystemLease(){if(events->watch){require(test_v46::final_byte_delete_observed_v46(),"actual V45 bytes were not freed before filesystem lease release");actual_archive.reset();events->filesystem_released=true;}}
 };
 struct V46AdmissionLease {
  std::shared_ptr<void> actual_admission;
  std::shared_ptr<V46Events> events;
  ~V46AdmissionLease(){if(events->watch){require(test_v46::final_byte_delete_observed_v46()&&events->filesystem_released,"actual V45 admission released before bytes/filesystem");actual_admission.reset();events->admission_released=true;}}
 };
 auto v46_events=std::make_shared<V46Events>();auto v46_services=v45_services;
 v46_services.borrow_resources=[&](const ActorVisualReadV37&,ActorImageOwnerBorrowV45& out,std::string& e){auto images=v45_images;
  auto fs=std::make_shared<V46FilesystemLease>();fs->actual_archive=v45_archive;fs->events=v46_events;images.owner=fs;
  return actor_image_owner_v45(containers.front(),std::move(images),out,e);
 };
 v46_services.admit_before_read=[&](const MaterialImageIdentityV43& id,const std::string& uri,std::shared_ptr<void>& pin,std::string& e){std::shared_ptr<void> actual;
  if(!v45_services.admit_before_read(id,uri,actual,e))return false;
  auto observe=std::make_shared<V46AdmissionLease>();observe->actual_admission=std::move(actual);observe->events=v46_events;pin=std::move(observe);return true;
 };
 std::shared_ptr<const std::vector<std::uint8_t>> v46_bytes;ActorImageSubmissionResultV45 v46_result;
 require(submit_generic_actor_images_v45(helper_input,helper_key,v46_services,[&](const ActorPrimitiveImagesReadV45& read,std::string&){
  if(!v46_bytes)for(const auto& image:read.images)if(image.image.bytes){v46_bytes=image.image.bytes;break;}return true;
 },v46_result,e),e);
 require(v46_bytes&&!v46_bytes->empty(),"actual V45 byte allocation was not delivered");
 // Only the saved allocation's last aliases remain. Mark AFTER the stream,
 // so other actual allocations/leases legitimately released inside it are not
 // mistaken for the observed resource's final lifetime event.
 test_v46::watch_final_byte_delete_v46(v46_bytes->data());v46_events->watch=true;
 require(!test_v46::final_byte_delete_observed_v46()&&!v46_events->filesystem_released&&!v46_events->admission_released,"byte-only copy lost actual lifetime");
 v46_bytes.reset();require(test_v46::final_byte_delete_observed_v46()&&v46_events->filesystem_released&&v46_events->admission_released,"actual V45 final deletion/admission ordering differs");
 test_v46::clear_final_byte_delete_watch_v46();
 std::cerr<<"ACTUAL_V45_BYTE_DELETE_V46 PASS byte_only_copy=1 bytes_before_filesystem_before_admission=1 host_admission_fixture=1 production_budget_GPU_claim=0\n";
}
