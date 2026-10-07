#pragma once
#include <canonical_object_factory_v1.hpp>
#include <canonical_class_receiver_bindings_v1.hpp>
#include <canonical_level_context_v1.hpp>
#include <map>
#include <functional>
#include <vector>
namespace dh2::loader {
// Scoped actual owners, borrowed from Main's produced source candidate. The
// retained catalog never stores the complete containing World or Level receipt.
struct ScopeV67 {
 std::weak_ptr<void> actual_world;
 std::weak_ptr<CanonicalLevelContextV1> level;
 std::weak_ptr<world::CanonicalObjectManagerV1> objects;
 world::CanonicalPropertyMapV1* properties{};
 // Existing independent provider authority. It must not own actual World,
 // Level or the ObjectManager through direct aliases or hidden captures.
 std::shared_ptr<void> services_owner;
};
using ClassFactoryV67=std::function<bool(const world::CanonicalFactoryEntryV1&,
 const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&)>;
struct PartV67 {
 std::shared_ptr<void> owner; // actual child factory, not a substitute World
 std::vector<world::CanonicalFactoryEntryV1> entries;
 ClassFactoryV67 construct; // invokes that child's genuine canonical C1s
};
class ProductionNonCharacterCatalogV67 final:
 public std::enable_shared_from_this<ProductionNonCharacterCatalogV67> {
 struct Route {std::size_t part{};const world::CanonicalFactoryEntryV1* original{};};
 ScopeV67 scope_;std::vector<PartV67> parts_;std::map<std::string,Route> routes_;
 ProductionNonCharacterCatalogV67()=default;
public:
 static bool create(ScopeV67,std::vector<PartV67>,
  std::shared_ptr<ProductionNonCharacterCatalogV67>&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,
  const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
 ClassFactoryV67 factory();
 bool supports(const std::string&)const noexcept;
 // Explicit acceptance requirement, not filtering. Missing required entries
 // are reported before source XML construction; no declaration is skipped.
 bool require_entries(const std::vector<std::string>&,std::string&)const;
};
}
