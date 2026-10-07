#pragma once
#include "resource_budget_v37.hpp"
#include <vector>
namespace dh2::resources {
// Reserve before an existing producer allocates its bytes; publish only after
// the producer succeeds. Declare this BEFORE its owned buffers so their actual
// storage is destroyed before the accounting lease releases.
class CpuAdmissionV40 {
 std::shared_ptr<ContextResourceBudgetV37> budget_;
 ResourceTokenV37 token_{};
 ResourceReservationV37 pending_;
public:
 CpuAdmissionV40()=default;
 ~CpuAdmissionV40();
 CpuAdmissionV40(const CpuAdmissionV40&)=delete;
 CpuAdmissionV40& operator=(const CpuAdmissionV40&)=delete;
 CpuAdmissionV40(CpuAdmissionV40&&)noexcept;
 CpuAdmissionV40& operator=(CpuAdmissionV40&&)noexcept;
 bool reserve(std::shared_ptr<ContextResourceBudgetV37>,ResourceScopeV37,std::uint64_t,std::string&);
 bool commit(std::string&);
 void reset()noexcept;
};
struct AdmittedVectorV40 {
 CpuAdmissionV40 admission;
 std::vector<std::uint8_t> bytes;
 // Alias shares the exact buffer AND its accounting lease, without copying.
 static std::shared_ptr<const std::vector<std::uint8_t>> borrow(const std::shared_ptr<AdmittedVectorV40>& owner){return {owner,&owner->bytes};}
};
}
