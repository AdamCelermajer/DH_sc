#pragma once
#include "material_image_identity_v43.hpp"
#include <admitted_cpu_bytes_v40.hpp>
namespace dh2::loader {
// Bind exactly OriginalCacheAssetsV1::read_admitted. A mounted cache copy can
// share its existing private State/descriptor without retaining an actor/world.
using MainOriginalAdmittedReadV46=std::function<bool(const std::string&,bool&,
 std::vector<std::uint8_t>&,const std::function<bool(std::uint32_t,std::string&)>&,std::string&)>;
struct MainEncodedImageBudgetInputsV46 {
 std::shared_ptr<void> filesystem_owner;
 std::weak_ptr<void> actual_renderer_owner;
 std::shared_ptr<resources::ContextResourceBudgetV37> actual_main_budget;
 resources::ResourceScopeV37 scope{resources::ResourceScopeV37::actor};
 MainOriginalAdmittedReadV46 original_read_admitted;
};
// Narrow source API-binding proposal, not selected/integrated. Uses Main's
// CpuAdmission/ledger; has no budget counters, limits, GL names or actor state.
// Owning renderer thread only. A weak pending read ticket prevents cancellation
// before read from retaining a reservation; actual bytes keep the final ticket.
class MainEncodedImageBudgetProviderV46 : public std::enable_shared_from_this<MainEncodedImageBudgetProviderV46> {
 struct Ticket;
 MainEncodedImageBudgetInputsV46 input_;
 std::weak_ptr<Ticket> pending_;
public:
 explicit MainEncodedImageBudgetProviderV46(MainEncodedImageBudgetInputsV46 input):input_(std::move(input)){}
 bool admit_before_read(const MaterialImageIdentityV43&,const std::string&,
                        std::shared_ptr<void>& actual_pin,std::string&);
 bool read(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
 bool image_services(const std::function<bool(const MaterialImageIdentityV43&,std::string&,std::string&)>& actual_resolver,
                     MaterialImageServicesV43& out,std::string&);
};
}
