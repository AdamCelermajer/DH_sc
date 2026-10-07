#pragma once
#include "visual_fx_preload.hpp"
#include "../game-data/effects_tables.hpp"
#include <functional>
#include <list>
#include <memory>
#include <vector>
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::fx {
// Actual pre-player source library members. Paths/rows point into the SAME
// immutable Arrays owner; no scene, player, resource instance or GL is created.
struct AnimatedFxInfoV63 {
 const std::string* file0{};
 std::vector<std::shared_ptr<void>> free4;
 std::list<std::shared_ptr<void>> active10;
};
struct AnimFxStepV63 {std::uint8_t redirect0{};std::int32_t file_or_set4{-1};};
struct AnimFxSetInfoV63 {
 const data::EffectSet* authored0{};
 std::vector<std::unique_ptr<AnimFxStepV63>> steps4;
 std::list<std::shared_ptr<void>> active10;
};
struct VisualFxLibraryDebugV63 {
 std::weak_ptr<application::ApplicationServicesOwnerV5> application;
 std::function<bool(std::uint32_t,const char*,std::uint32_t&,std::string&)> invoke;
};
struct VisualFxLibraryReleaseV88 {
 std::shared_ptr<void> owner;
 std::function<bool(std::shared_ptr<void>&,std::string&)> drop_finished,animated_d0,set_data_d1;
};
class VisualFxManagerLibrariesV63 final {
 data::EffectsTables::Borrow tables_;
 VisualFxLibraryDebugV63 debug_;
 std::list<std::shared_ptr<void>> finished8_v88_; //actual C1-empty list8
 bool flush_failed_v88_{};std::string flush_failure_v88_;
 std::uint8_t byte4_{}; // exact C1 49328c/4932f0 zero
 std::vector<std::int32_t> queue_storage_;
 PreloadQueue16 pending10_{};
 std::vector<std::vector<PreloadStep8>> registration_steps_;
 std::vector<PreloadSet16> registration_sets_;
 PreloadTable16 registration_table_{};
 std::vector<std::unique_ptr<AnimFxSetInfoV63>> sets1c_;
 std::vector<std::unique_ptr<AnimatedFxInfoV63>> dictionary28_;
 // Native failed-prefix retention; not a source readiness/member byte.
 std::unique_ptr<AnimFxSetInfoV63> pending_set_;
 bool busy_{},failed_{};std::string failure_,leaf_error_;
 static int preload(void*,std::uint32_t,const char*,std::uint32_t*);
 bool query(std::uint32_t,const char*,std::uint32_t&,std::string&);
 bool module(bool&,std::string&);
 bool reject(const std::string&,std::string&);
 bool register_impl(std::int32_t,bool,std::string&);
public:
 VisualFxManagerLibrariesV63(data::EffectsTables::Borrow,VisualFxLibraryDebugV63);
 VisualFxManagerLibrariesV63(const VisualFxManagerLibrariesV63&)=delete;
 bool build_libraries(std::string&);
 bool register_effect_to_load(std::int32_t,std::string&);
 bool register_set_to_load(std::int32_t,std::string&);
 bool precache_libraries(std::string&);
 //Actual library pool members, borrowed by the SAME native instance backend.
 AnimatedFxInfoV63* animated_info_v88(std::int32_t i)noexcept{return i>=0&&std::size_t(i)<dictionary28_.size()?dictionary28_[i].get():nullptr;}
 AnimFxSetInfoV63* set_info_v88(std::int32_t i)noexcept{return i>=0&&std::size_t(i)<sets1c_.size()?sets1c_[i].get():nullptr;}
 std::list<std::shared_ptr<void>>& finished8_v88()noexcept{return finished8_v88_;}
 //Whole4950a0 tail→494e3c. Flush preserves registration VectorSet10,
 //destroys real pools/set data, erases declarations, then resets byte4.
 bool flush_libraries_v88(const VisualFxLibraryReleaseV88&,std::string&);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const data::EffectsTables::Borrow& source_tables()const noexcept{return tables_;}
 std::uint8_t precache_byte4()const noexcept{return byte4_;}
 const auto& dictionary()const noexcept{return dictionary28_;}
 const auto& sets()const noexcept{return sets1c_;}
 const auto* pending_files()const noexcept{return pending10_.ids;}
 std::uint32_t pending_file_count()const noexcept{return pending10_.count;}
 // Existing proved Character/Lua preload kernels borrow this SAME VectorSet
 // storage/count; they must not create a per-World/per-skill pending queue.
 PreloadQueue16& pending_queue_v63()noexcept{return pending10_;}
 //Exact transport over this owner's existing registration views/Debug leaf;
 //borrowing it does not run BuildLibraries or produce the pool vectors.
 const PreloadTable16& registration_table_v122()const noexcept{return registration_table_;}
 PreloadServices16 registration_services_v122()noexcept{return {this,preload};}
 const auto* failed_set_prefix()const noexcept{return pending_set_.get();}
 bool belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>&)const noexcept;
 std::uintptr_t application_identity()const noexcept;
 const std::string& error()const noexcept{return failure_;}
};
}
