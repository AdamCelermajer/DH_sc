#pragma once
#include "data.hpp"
#include <memory>

namespace dh2::data {
// Scalar projection of Structs::ItemAudioVisual: AudioDrop and AudioPickup
// are signed 32-bit fields; Visual is the source BDAE object identifier.
struct ItemAudioVisualRowV1 {
    std::string identifier;
    std::int32_t audio_drop=0;
    std::int32_t audio_pickup=0;
    std::string visual;
};

// Owns the original loot_audiovisual pyarray/name/schema sections. AudioVisualID
// is the zero-based ItemTable row index; a Borrow resolves that ID without
// creating another item or presentation owner.
class ItemAudioVisualTableV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class ItemAudioVisualTableV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> value):snapshot_(std::move(value)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const std::vector<ItemAudioVisualRowV1>& rows()const;
        const ItemAudioVisualRowV1* get(std::int32_t audio_visual_id)const noexcept;
        std::int32_t id(const std::string& identifier)const noexcept;
    };

    // Atomic replacement: malformed input leaves the previously loaded table
    // available to this owner and all outstanding Borrows.
    bool load(Bytes records,Bytes names,Bytes schema,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
} // namespace dh2::data
