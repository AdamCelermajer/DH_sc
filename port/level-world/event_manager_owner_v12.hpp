#pragma once
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace dh2::events {
class EventManagerOwnerV12;
struct InputEventProjectionV120 {
 std::int16_t x8{},y_a{};std::int32_t index_c{},button8{},value_c{},value10{};std::uint8_t pressed10{};
};
// Borrow the actual payload. Raise/ RaiseAsync do not copy it in this binary;
// callbacks can observe the same mutable source event fields.
struct EventBorrowV12 {
 std::uintptr_t identity{};void* context{};
 bool(*get_type)(void*,std::int32_t&,std::string&){};
 std::shared_ptr<void> lifetime;
 //Only the real input producer supplies this typed live projection. Context
 //is never interpreted as a foreign ARM event image by receivers.
 bool(*input_v120)(void*,InputEventProjectionV120&,std::string&){};
};
struct EventReceiverV12 {
 std::uintptr_t identity{};void* context{};
 bool(*on_event)(void*,const EventBorrowV12&,EventManagerOwnerV12&,std::int32_t&,std::string&){};
 std::shared_ptr<void> lifetime;
};
struct PendingEventV12 {
 // event.lifetime pins a containing native arena/World, not a second owning
 // deleter for the event itself: deleting_destructor is its source delete site.
 EventBorrowV12 event;
 void* destroy_context{};
 bool(*deleting_destructor)(void*,std::uintptr_t,std::string&){};
};
struct EventQueueProducerV12 {
 void* context{};
 // Required external actual queue producer: neither Raise nor RaiseAsync is
 // one. Source storage transfer only; no guessed clone or memcpy operation.
 bool(*produce)(void*,PendingEventV12&,std::string&){};
};
struct ReceiverObservationV12 {std::int32_t type{};std::uintptr_t identity{};std::int32_t priority{};std::uint8_t byte10{};};
class EventManagerOwnerV12 {
 struct ReceiverInfo {EventReceiverV12 receiver;std::int32_t priority{};std::uint8_t byte10{};std::uint64_t token{};};
 struct DelayedInfo {std::int32_t type{};std::uint64_t token{};};
 const std::uintptr_t identity_;
 std::map<std::int32_t,std::list<ReceiverInfo>> receivers_;
 std::list<PendingEventV12> queue20_;
 std::list<DelayedInfo> delayed28_;
 std::uint64_t next_token_{1}; // host identity for stable native registration nodes
 unsigned dispatch_depth_{};bool updating_{},failed_{};std::string failed_reason_;
 bool ready(std::string&)const;
 void latch_failure(const std::string&,const char* fallback);
public:
 explicit EventManagerOwnerV12(std::uintptr_t actual_base_identity);
 ~EventManagerOwnerV12();
 EventManagerOwnerV12(const EventManagerOwnerV12&)=delete;
 EventManagerOwnerV12& operator=(const EventManagerOwnerV12&)=delete;
 std::uintptr_t identity()const noexcept{return identity_;}
 bool attach(std::int32_t,const EventReceiverV12&,std::int32_t priority,bool& inserted,std::string&);
 bool detach(std::int32_t,std::uintptr_t receiver,bool& removed,std::string&);
 bool delayed_detach(std::int32_t,std::uintptr_t receiver,bool& scheduled,std::string&);
 bool drop_delayed_detach(std::string&);
 //Native lifetime retirement of one exact observer before borrowed storage
 //free. Not Objective.Unregister: no gameplay callbacks/source field stores.
 //Allowed after failed delivery but never during an active snapshot/update.
 bool retire_receiver_alias_v88(std::uintptr_t receiver,std::string&);
 bool raise(const EventBorrowV12&,std::string&);
 bool raise_async(const EventBorrowV12& e,std::string& error){return raise(e,error);} // literal339090 b338ebc
 bool update(double source_dt,std::string&);
 bool flush(std::string&);
 // Explicit source-storage adapter, not a recovered enqueue/clone method.
 bool import_pending_from_required_producer_v12(const EventQueueProducerV12&,std::string&);
 std::size_t receiver_count(std::int32_t)const noexcept;
 std::vector<ReceiverObservationV12> receiver_records_v12()const;
 std::size_t pending_count()const noexcept{return queue20_.size();}
 std::size_t delayed_count()const noexcept{return delayed28_.size();}
 bool source_delivery_busy_v115()const noexcept{return dispatch_depth_||updating_;}
 bool failed()const noexcept{return failed_;}
};
}
