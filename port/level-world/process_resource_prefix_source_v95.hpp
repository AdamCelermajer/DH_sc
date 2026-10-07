#pragma once
#include <array>
#include <cstring>
#include <exception>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
namespace dh2::application {
// Actual original Java GLMediaPlayer.getSDFolder (DEX method695): one
// const-string + return-object. Not SUtils preferences, a profile save path,
// Main's private files_directory, a GLES choice or a mount-directory default.
inline constexpr char original_gl_media_player_sd_folder_v95[]=
 "/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/";
struct ProcessResourcePrefixBorrowV95 {
 std::shared_ptr<const std::array<char,1024>> owner;
 const char* data{};std::size_t length{};
 explicit operator bool()const noexcept{return owner&&data==owner->data();}
};
// Native RES_PATH global's independently allocated byte owner. The SAME
// existing process App pins this adapter; it contains no App/World/CFS owner.
class ProcessResourcePrefixSourceV95 final {
 const std::thread::id producer_{std::this_thread::get_id()};
 mutable std::mutex publication_mutex_;
 std::shared_ptr<std::array<char,1024>> bytes_;
 std::size_t length_{};bool attempted_{},published_{};std::string failure_;
public:
 bool initialize_gl_media_player_source(std::string& e){
  std::lock_guard<std::mutex> publication(publication_mutex_);
  // Completed immutable source publication survives GL thread recreation.
  // First actual appInit remains on the owner-construction producer thread.
  if(attempted_){if(published_){e.clear();return true;}e=failure_;return false;}
  if(std::this_thread::get_id()!=producer_){e="Required actual appInit resource-prefix producer thread";return false;}
  attempted_=true;
  try{
   // nativeGetSdFolderPath531b64 allocates1024, strcpy from actual Java UTF
   // bytes, releases Java UTF lease, returns this SAME independent buffer.
   // Only reached string+NUL bytes are produced; opaque padding is untouched.
   auto next=std::shared_ptr<std::array<char,1024>>(new std::array<char,1024>);
   constexpr auto size=sizeof(original_gl_media_player_sd_folder_v95);
   static_assert(size<=1024);
   std::memcpy(next->data(),original_gl_media_player_sd_folder_v95,size);
   bytes_=std::move(next);length_=size-1;published_=true;e.clear();return true;
  }catch(const std::exception& ex){failure_=ex.what();e=failure_;return false;}
 }
 bool borrow(ProcessResourcePrefixBorrowV95& out,std::string& e)const{
  std::lock_guard<std::mutex> publication(publication_mutex_);
  if(!published_||!bytes_){e="Required completed SAME original appInit RES_PATH publication";return false;}
  out={bytes_,bytes_->data(),length_};e.clear();return true;
 }
 bool published()const{std::lock_guard<std::mutex> publication(publication_mutex_);return published_;}
};
}
