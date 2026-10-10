#pragma once
#include "../level-world/floors.hpp"
#include "../level-world/navigation_objects.hpp"
#include <limits>
#include <stdexcept>

namespace dh::foundation {
// Retains the caller's actual sewn floor world and this world's sole obstacle
// storage. It is an in-house PF lifetime capsule, not a complete source Level.
// Native and presentation actors borrow the same registry; never clone it for
// a second actor graph. Size before publishing any pointers into the storage.
struct SourceNavigationWorldStorage final {
    std::shared_ptr<dh2::floors::World> floors;
    std::vector<dh2::navigation::ObstacleEntry> entries;
    std::vector<std::uint32_t> floorKeys;
    dh2::navigation::ObstacleRegistry registry{};
    SourceNavigationWorldStorage(std::shared_ptr<dh2::floors::World> actualFloors,
                                 std::size_t actorCapacity):floors(std::move(actualFloors)){
        if(!floors||!floors->sewn)throw std::invalid_argument("Source navigation storage requires the actual sewn floor world");
        if(actorCapacity>std::numeric_limits<unsigned>::max()||floors->records.size()>=std::numeric_limits<unsigned>::max())
            throw std::length_error("Source navigation storage exceeds native registry bounds");
        entries.resize(actorCapacity);floorKeys.resize(floors->records.size()+1);
        registry={entries.data(),0,unsigned(entries.size()),floorKeys.data(),0,unsigned(floorKeys.size())};
    }
    SourceNavigationWorldStorage(const SourceNavigationWorldStorage&)=delete;
    SourceNavigationWorldStorage& operator=(const SourceNavigationWorldStorage&)=delete;
    SourceNavigationWorldStorage(SourceNavigationWorldStorage&&)=delete;
    SourceNavigationWorldStorage& operator=(SourceNavigationWorldStorage&&)=delete;
};
}
