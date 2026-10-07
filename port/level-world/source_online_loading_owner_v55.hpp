#pragma once
#include <cstdint>

namespace dh2::application {
// Projection of the actual COnline fields reached by source loading. Original
// GetOnline7fd794 lazily calls COnline.GetInstance7fd744, which constructs
// COnlineImpl825000 -> COnline7fd838. The base C1 writes byte4/5=0; the
// derived C1 only replaces the vptr. Transport queues/mutexes are not claimed.
// One Application-retained native facet supplies these SAME source cells to
// C1, GS, factories and front-menu providers. This is not a copied menu flag.
class SourceOnlineLoadingOwnerV55 final {
 std::uint8_t byte4_{},byte5_{};
public:
 SourceOnlineLoadingOwnerV55()=default;
 SourceOnlineLoadingOwnerV55(const SourceOnlineLoadingOwnerV55&)=delete;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::uint8_t byte5()const noexcept{return byte5_;}
 // Whole COnline.SetIsOnlineGame7fd50c is strb r1,[r0,#5]; bx lr.
 void set_is_online_game(std::uint8_t value)noexcept{byte5_=value;}
 std::uint8_t byte4()const noexcept{return byte4_;}
};
}
