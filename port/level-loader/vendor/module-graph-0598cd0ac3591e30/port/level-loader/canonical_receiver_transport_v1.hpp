#pragma once
#include "canonical_class_receiver_bindings_v1.hpp"

namespace dh2::loader {
struct CanonicalReceiverTransportServicesV1 {
    // Pins the actual constructor/provider context and PropertyMap. Keep this
    // owner and the transport as siblings; the owner must not own the transport.
    std::shared_ptr<void> owner;
    void* context{};
    bool(*construct)(void*,const world::CanonicalFactoryEntryV1&,
        const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&){};
    bool(*unknown_type_debug)(void*,const char*,std::string&){};
};
// Catalog-wide source transport. The caller supplies real class constructors
// and SAME-receiver continuations. This owns no manager, schema, Level, class
// implementation, conditions, renderer or current-world publication policy.
class CanonicalReceiverTransportV1 final {
    world::CanonicalPropertyMapV1& properties_;
    CanonicalReceiverTransportServicesV1 source_;
    std::map<std::uintptr_t,world::CanonicalClassReceiverV1> receivers_;
    world::CanonicalClassReceiverV1* find(const world::CanonicalObjectBorrowV1&,std::string&);
    static bool construct(void*,const world::CanonicalFactoryEntryV1&,
        const world::CanonicalSourceObjectRequestV1&,world::CanonicalObjectBorrowV1&,std::string&);
    static bool init_properties(void*,const world::CanonicalObjectBorrowV1&,std::string&);
    static bool set_template(void*,const world::CanonicalObjectBorrowV1&,const char*,std::string&);
    static bool defaults(void*,const world::CanonicalObjectBorrowV1&,std::string&);
    static bool overrides(void*,const world::CanonicalObjectBorrowV1&,const world::CanonicalSourceObjectRequestV1&,std::string&);
    static bool init_post(void*,const world::CanonicalObjectBorrowV1&,std::string&);
    static bool is_game_object(void*,const world::CanonicalObjectBorrowV1&,bool&,std::string&);
    static bool position(void*,const world::CanonicalObjectBorrowV1&,std::array<float,3>&,std::string&);
    static bool set_position(void*,const world::CanonicalObjectBorrowV1&,const std::array<float,3>&,bool,std::string&);
    static bool unknown(void*,const char*,std::string&);
public:
    CanonicalReceiverTransportV1(world::CanonicalPropertyMapV1&,CanonicalReceiverTransportServicesV1);
    CanonicalReceiverTransportV1(const CanonicalReceiverTransportV1&)=delete;
    CanonicalReceiverTransportV1& operator=(const CanonicalReceiverTransportV1&)=delete;
    // The canonical candidate lease must pin this transport through factory
    // calls. The returned existing ABI has a borrowed context, not ownership.
    world::CanonicalClassServicesV1 services()noexcept;
    // Actual removal/deleting destructor calls this after unpublication.
    // Duplicate Add erases only the newly constructed duplicate identity.
    void erased(std::uintptr_t identity){receivers_.erase(identity);}
    std::size_t retained_count()const noexcept{return receivers_.size();}
    // Borrow same retained dispatch, including a constructor's failed prefix.
    // Caller pins its actual object/candidate until this synchronous use ends.
    bool receiver(const world::CanonicalObjectBorrowV1&,
        const world::CanonicalClassReceiverV1*& out,std::string&);
};
}
