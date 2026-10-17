#pragma once
#include "player_info_record_v1.hpp"
#include "player_offline_registration_v1.hpp"
#include "input_manager_v1.hpp"
#include <map>
#include <memory>
#include <vector>
namespace dh2::player_offline_registry_v1 {
// One stable backing store behind the existing public registry projection.
// The source manager+8 record, process factory/counter and input owner are lent
// by the native session. No second PlayerManager or input frame loop is made.
class Owner {
    using Projection=player_manager_host_level::PlayerInfoProjection;
    struct Entry {
        player_info_record_v1::Record record;
        Projection projection;
        explicit Entry(int key):projection{reinterpret_cast<std::uintptr_t>(&record),key,&record.at(0x310)->header}{}
    };
    player_manager_host_level::PlayerRegistry& registry_;
    player_info_record_v1::Factory& factory_;
    player_info_record_v1::Record& fallback_;
    input_manager_v1::Manager& input_;
    std::map<int,std::unique_ptr<Entry>> entries_;
    std::vector<Projection*> published_;
    std::vector<std::unique_ptr<Entry>> retired_;
    Entry* temporary_=nullptr;
    Entry& retain(int key);
    int temporary(Projection**);
    int subscript(int key,Projection**);
    int retire(Projection*);
public:
    Owner(player_manager_host_level::PlayerRegistry& registry,
          player_info_record_v1::Factory& factory,
          player_info_record_v1::Record& fallback,input_manager_v1::Manager& input);
    Owner(const Owner&)=delete;Owner& operator=(const Owner&)=delete;
    ~Owner();
    player_info_record_v1::Record* record(Projection*);
    player_offline_registration_v1::Services services(const player_locality_v1::Services& queries);
    std::size_t retained_lifecycle_storage() const{return retired_.size();}
};
} // namespace dh2::player_offline_registry_v1
