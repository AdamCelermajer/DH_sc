#pragma once
#include "../level-loader/native_root_source_binding_v65.hpp"
#include "../asset-payloads/zip_asset_pack_v1.hpp"
#include <cstring>
#include <functional>
namespace model_renderer::source_campaign_detail_v61 {
// Native IStream backend over the SAME mounted ZIP authority. Open creates a
// genuine sequential receiver; Size reads its entry, Read acquires actual bytes
// and copies into the loader's final StreamBuffer, Close releases its backing.
// It is not a generated stream, source cache clone or synthetic filename token.
class CacheIStreamV65 {
 dh2::assets::ZipAssetPackV1 archive_;
 dh2::assets::ZipEntryV1 entry_;
 std::uint32_t position_{};bool closed_{};
public:
 CacheIStreamV65(dh2::assets::ZipAssetPackV1 archive,dh2::assets::ZipEntryV1 entry):archive_(std::move(archive)),entry_(std::move(entry)){}
 bool size(std::uint64_t& value,std::string& e)const{
  if(closed_||!archive_.mounted()){e="Source cache IStream already closed";return false;}
  value=entry_.bytes;e.clear();return true;
 }
 bool read(void* destination,std::uint32_t count,std::string& e){
  if(closed_||!archive_.mounted()||position_>entry_.bytes||count>entry_.bytes-position_||(!destination&&count)){
   e="Source cache IStream Read outside live owned entry";return false;
  }
  // The existing native XML receiver accepts at most32MiB. Reject before
  // the source Grow is admitted through Size, never allocate arbitrary input.
  std::vector<std::uint8_t> bytes;bool found{};
  if(!archive_.read(entry_.uri,found,bytes,e))return false;
  if(!found||bytes.size()!=entry_.bytes){e="Actual source cache entry changed/absent during Read";return false;}
  if(count)std::memcpy(destination,bytes.data()+position_,count);
  position_+=count;e.clear();return true;
 }
 bool close(std::string& e){
  if(closed_){e="Source cache IStream closed twice";return false;}
  archive_={};entry_={};closed_=true;e.clear();return true;
 }
};
struct CacheStreamAuthorityV65 {std::weak_ptr<CacheIStreamV65> produced;};
inline std::shared_ptr<CacheIStreamV65> checked_cache_stream_v65(const CacheStreamAuthorityV65& authority,
 const dh2::loader::SourceIStreamBorrowV65& source,std::string& e){
 const auto actual=authority.produced.lock();
 if(!actual||source.actual_owner.get()!=actual.get()||source.actual_owner.owner_before(actual)||actual.owner_before(source.actual_owner)||
    source.identity!=reinterpret_cast<std::uintptr_t>(actual.get())){
  e="Required SAME actual cache IStream receiver/lease";return {};
 }
 return actual;
}
inline bool bind_cache_filename_source_v65(const dh2::assets::ZipAssetPackV1& mounted,
 const std::shared_ptr<void>& files,
 std::function<bool(const std::string&,bool&,std::string&)> uncompiled,
 dh2::loader::AssignedReadLeavesV65 parser,
 dh2::loader::SourceLoadingInputsV43& out,std::string& error){
 using namespace dh2::loader;
 if(!mounted.mounted()||!files||!uncompiled||!parser.owner||!parser.parse_result||out.native_filename_source||out.assigned_source.owner){
  error="Required sole mounted native filename/LoadFileData source binding";return false;
 }
 const auto authority=std::make_shared<CacheStreamAuthorityV65>();
 AssignedCopyLeavesV65 copy;copy.owner=files;
 copy.size=[authority](const SourceIStreamBorrowV65& source,std::uint64_t& value,std::string& e){
  auto stream=checked_cache_stream_v65(*authority,source,e);if(!stream||!stream->size(value,e))return false;
  if(value>32u*1024u*1024u){e="Source XML IStream exceeds existing native32MiB parse domain";return false;}
  return true;
 };
 copy.read=[authority](const SourceIStreamBorrowV65& source,void* destination,std::uint32_t count,std::string& e){
  auto stream=checked_cache_stream_v65(*authority,source,e);return stream&&stream->read(destination,count,e);
 };
 FilenameSourceLeavesV65 filename;filename.owner=files;filename.is_using_uncompiled_data=std::move(uncompiled);filename.copy=std::move(copy);
 filename.open_resource=[mounted,authority](const std::string& name,bool& found,std::string& uri,SourceIStreamBorrowV65& source,std::string& e){
  if(!authority->produced.expired()){e="Actual source IStream must close before another open";return false;}
  dh2::assets::ZipEntryV1 entry;if(!mounted.entry(name,found,entry,e))return false;
  if(!found){source={};uri.clear();e.clear();return true;}
  uri=entry.uri;auto stream=std::make_shared<CacheIStreamV65>(mounted,std::move(entry));
  authority->produced=stream;source={stream,reinterpret_cast<std::uintptr_t>(stream.get())};e.clear();return true;
 };
 filename.close_source=[authority](SourceIStreamBorrowV65& source,std::string& e){
  auto stream=checked_cache_stream_v65(*authority,source,e);
  if(!stream||!stream->close(e))return LifecycleStepV36::failed;
  source={};authority->produced.reset();return LifecycleStepV36::complete;
 };
 // Root and subsequent MGP/MVP occurrences borrow the same CFS/stream leaves.
 // The loader creates each occurrence's assigned receiver and parser on the
 // one Level140 slot; completing the root releases that slot before modules.
 NativeRootSourceLeavesV65 leaves;
 leaves.filename=std::move(filename);leaves.read=std::move(parser);
 // Generated bytes require their own genuine generator IStream receiver.
 // Do not bind this cache-stream cast as the procedural assign provider.
 return bind_native_root_source_v65(std::move(leaves),out,error);
}
}
