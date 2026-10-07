#pragma once
#include "canonical_decor_v15.hpp"
#include "canonical_animated_decor_v1.hpp"

namespace dh2::world {
class CanonicalAnimatedDecorV23;
struct AnimatedDecorServicesV23 {
 std::shared_ptr<void> owner;
 DecorServicesV15 decor;
 // Typed reached engine leaves for local source389128 control on SAME V23 C1.
 // No standalone CanonicalAnimatedDecorV1 receiver is constructed.
 AnimatedDecorServicesV1 source_init;
 // Whole source389128, including qualified Decor InitPost and the actual
 // current visual/controller/RNG/physical/Update operations. Required when
 // reached; this constructor adapter supplies no successful substitute.
 std::function<bool(CanonicalAnimatedDecorV23&,std::string&)> whole_init_post;
};
// Source342600 constructs one GO_ID20 Decor base, then clears source375 and
// constructs the sole string37c. Inheritance reuses that same base/runtime.
class CanonicalAnimatedDecorV23 final:public CanonicalDecorV15 {
 std::string startanim37c_;
 AnimatedDecorServicesV23 services_;
 bool destroyed_{};
 bool source_random_animation(bool,std::string&);
 bool source_mesh_box_and_physical(std::string&);
 bool missing_source(const char*,std::string&)const;
public:
 CanonicalAnimatedDecorV23(std::shared_ptr<void>,actor::RuntimeState&,
                           GameObjectInitializationServicesV1,AnimatedDecorServicesV23);
 CanonicalPropertyActorV1 properties()noexcept{return canonical_family_fields_v15(*this);}
 bool write_string(std::uint32_t,const std::string&,std::string&);
 const std::string& start_animation()const noexcept{return startanim37c_;}
 bool init_post(std::string&);
 bool animation_finished(std::string&);
 bool destroy(std::string&);
 static constexpr bool is_animated()noexcept{return true;}
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalAnimatedDecorV23>,std::shared_ptr<const void>);
};
}
