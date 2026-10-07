#pragma once
#include "player_add_character_owner_v5.hpp"
#include "player_manager_update_owner_v70.hpp"
#include <map>
namespace dh2::player {
// App-retained manager continuation. Each source AddCharacter attempt retains
// its failure prefix on its SAME PlayerInfo receiver; retries cannot construct
// another Character or replay save/equipment grants.
class PlayerManagerCharacterDeliveryV70 {
 PlayerManagerOwnerV1& manager_;
 PlayerAddServicesV5 add_services_;
 std::map<PlayerInfoFieldsV1*,std::unique_ptr<PlayerAddCharacterOwnerV5>> attempts_;
 bool delivering_{};
public:
 PlayerManagerCharacterDeliveryV70(PlayerManagerOwnerV1& m,PlayerAddServicesV5 services):manager_(m),add_services_(std::move(services)){}
 bool deliver(const PlayerManagerRequestV1&,PlayerManagerResponseV1&,std::string&);
 // Call after original PM removes the record and unpublishes Character660,
 // before native storage may reuse that PlayerInfo address.
 bool forget_removed(PlayerInfoFieldsV1&,std::string&);
};
}
