#include "../current_level_quest_event_v1.hpp"
#include "../../game-data/quest_gather_loot_receiver_v1.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::level_world::current_level_quest_event_v1;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Context {
    Runtime* runtime = nullptr;
    std::vector<std::uintptr_t> calls;
    std::vector<Event> seen;
    bool mutate_once = false;
    bool mutate_list = false;
    void* attached_context = nullptr;
    Receiver attached_receiver = nullptr;
    std::int32_t result = 0;
};

std::int32_t count_receiver(void* raw, Runtime&, Event& event, std::string&);

std::int32_t receiver(void* raw, Runtime& manager, Event& event, std::string& error) {
    auto& context = *static_cast<Context*>(raw);
    check(&manager == context.runtime, "receiver received a different current-Level manager");
    context.calls.push_back(reinterpret_cast<std::uintptr_t>(&context));
    context.seen.push_back(event);
    if (context.mutate_list) {
        context.mutate_list = false;
        bool changed = false;
        if (context.runtime->detach(event.objective_type, 22, changed, error) != Status::complete)
            return 2;
        check(changed, "callback could not detach next receiver");
        if (context.runtime->attach(event.objective_type, 33, -9,
                                    context.attached_context,
                                    context.attached_receiver,
                                    changed, error) != Status::complete)
            return 2;
        check(changed, "callback could not attach receiver");
        event.flag0 = 1;
        event.subject_id = 42;
    }
    return context.result;
}

struct CountContext {
    std::uintptr_t id = 0;
    std::vector<std::uintptr_t>* calls = nullptr;
    std::vector<Event>* events = nullptr;
    std::int32_t result = 0;
};

std::int32_t count_receiver(void* raw, Runtime&, Event& event, std::string&) {
    auto& context = *static_cast<CountContext*>(raw);
    context.calls->push_back(context.id);
    context.events->push_back(event);
    return context.result;
}

std::int32_t failed_provider(void*, Runtime&, Event&, std::string& error) {
    error = "Objective completion script provider unavailable";
    return 1;
}

struct LootContext { std::int32_t quantity=0, script_calls=0, script_id=-1; };
bool item_quantity(void* raw,std::int32_t item,bool& found,std::int32_t& quantity,std::string& error){
 auto& state=*static_cast<LootContext*>(raw);if(item!=604){error="wrong item";return false;}
 found=true;quantity=state.quantity;error.clear();return true;
}
bool start_script(void* raw,std::int32_t id,std::string& error){
 auto& state=*static_cast<LootContext*>(raw);++state.script_calls;state.script_id=id;error.clear();return true;
}
}

int main() {
    try {
        Runtime runtime;
        Result result{};
        std::string error;
        std::vector<std::uintptr_t> calls;
        std::vector<Event> seen;
        CountContext second{22, &calls, &seen, 0};
        CountContext third{33, &calls, &seen, 0};
        Context first{&runtime, {}, {}, false, true, &third, count_receiver, 0};

        bool changed = false;
        check(runtime.attach(77, 11, 8, &first, receiver, changed, error) == Status::complete && changed,
              "first Attach did not add receiver");
        check(runtime.attach(77, 22, -3, &second, count_receiver, changed, error) == Status::complete && changed,
              "second Attach did not add receiver");
        check(runtime.attach(77, 11, 999, &first, receiver, changed, error) == Status::complete && !changed,
              "duplicate Attach was not ignored");

        Event event{};
        event.objective_type = 77;
        event.character = UINT64_C(0x12345678000000a1);
        event.network_id = -17;
        event.flag0 = 0;
        event.flag1 = 1;
        event.subject_id = -1;
        event.item_id = 604;
        check(runtime.raise(event, result, error) == Status::complete,
              "synchronous Raise failed");
        check(result.delivered == 2 && !result.stopped,
              "Raise did not use the captured receiver list");
        check(first.seen.size() == 1 && seen.size() == 1 && calls.size() == 1 &&
              calls[0] == 22,
              "detaching a receiver mutated the in-flight list");
        check(seen[0].flag0 == 1 && seen[0].subject_id == 42,
              "later receiver did not observe the live mutated event");
        check(event.flag0 == 1 && event.subject_id == 42,
              "receiver mutation was not visible when synchronous Raise returned");
        check(first.seen[0].character == UINT64_C(0x12345678000000a1) &&
              first.seen[0].network_id == -17 && first.seen[0].flag1 == 1 &&
              first.seen[0].item_id == 604,
              "GatherLoot semantic payload fields changed");

        check(runtime.attach(77, 33, -9, &first, receiver, changed, error) == Status::complete && !changed,
              "callback attachment was not published to later raises");
        calls.clear();
        seen.clear();
        Event next{};
        next.objective_type = 77;
        next.character = event.character;
        next.item_id = event.item_id;
        check(runtime.raise(next, result, error) == Status::complete && result.delivered == 2,
              "next Raise did not see the updated listener list");
        check(first.seen.size() == 2 && calls.size() == 1 && calls[0] == 33,
              "receiver order/priority behavior differs from source Attach order");

        bool scheduled = false;
        check(runtime.delayed_detach(77, 33, scheduled, error) == Status::complete && scheduled,
              "DelayedDetach did not queue the receiver");
        calls.clear();
        Event before_flush{};
        before_flush.objective_type = 77;
        check(runtime.raise(before_flush, result, error) == Status::complete && result.delivered == 2,
              "DelayedDetach removed a receiver before Update");
        check(calls.size() == 1 && calls[0] == 33,
              "receiver did not remain live until delayed-detach flush");
        check(runtime.flush_delayed_detaches(result, error) == Status::complete && result.detached == 1,
              "DropDelayedDetach did not remove queued receiver");
        check(runtime.raise(before_flush, result, error) == Status::complete && result.delivered == 1,
              "flushed receiver still received the event");

        CountContext stop{44, &calls, &seen, 1};
        CountContext skipped{55, &calls, &seen, 0};
        check(runtime.attach(88, 44, 0, &stop, count_receiver, changed, error) == Status::complete && changed,
              "stop receiver attach failed");
        check(runtime.attach(88, 55, 0, &skipped, count_receiver, changed, error) == Status::complete && changed,
              "following receiver attach failed");
        Event stop_event{};
        stop_event.objective_type = 88;
        calls.clear();
        check(runtime.raise(stop_event, result, error) == Status::complete && result.stopped &&
              result.delivered == 1 && calls.size() == 1 && calls[0] == 44,
              "source return-1 stop behavior differs");

        check(runtime.attach(89, 66, 0, nullptr, failed_provider, changed, error) ==
                  Status::complete && changed,
              "failed provider receiver attach failed");
        Event failed_event{};
        failed_event.objective_type = 89;
        check(runtime.raise(failed_event, result, error) == Status::receiver_failed &&
                  result.delivered == 1 && !result.stopped &&
                  error == "Objective completion script provider unavailable",
              "provider failure was masked as a legitimate consumed event");

        runtime.clear();
        using Gather=dh2::data::quest_gather_loot_receiver_v1::Binding;
        dh2::data::quest_objective_factory_v1::Record objective(0x1001);
        objective.fields.character_10=UINT64_C(0x12345678000000a1);
        LootContext loot{1,0,-1};
        check(dh2::data::quest_gather_loot_receiver_v1::compile(
              objective,true,604,3,42,true,1,&loot,start_script,error)&&
              objective.compiled_8&&objective.quantity_20==1&&!objective.done_14,
              "GatherLoot Compile did not preserve source initial quantity/compiled prefix");
        Gather gather{&objective,91,604,3,42,&loot,&loot,item_quantity,start_script};
        check(runtime.attach(91,0x1001,0,&gather,Gather::receive,changed,error)==Status::complete&&changed,
              "GatherLoot objective receiver registration failed");
        loot.quantity=2;
        Event pickup{};pickup.objective_type=91;pickup.character=objective.fields.character_10;
        pickup.item_id=604;pickup.subject_id=-1;
        check(runtime.raise(pickup,result,error)==Status::complete&&result.delivered==1,
              "registered GatherLoot receiver did not receive pickup event");
        check(objective.quantity_20==2&&!objective.done_14&&pickup.flag0==1&&pickup.subject_id==2&&loot.script_calls==0,
              "GatherLoot first pickup did not update canonical quantity/event without premature completion");
        loot.quantity=3;Event completing{};completing.objective_type=91;
        completing.character=objective.fields.character_10;completing.item_id=604;completing.subject_id=-1;
        check(runtime.raise(completing,result,error)==Status::complete&&result.delivered==1,
              "GatherLoot completion event delivery failed");
        check(objective.quantity_20==3&&objective.done_14&&completing.flag0==1&&
              completing.subject_id==3&&loot.script_calls==1&&loot.script_id==42,
              "GatherLoot quantity/completion did not update canonical Objective and start source script");
        std::puts("{\"validation\":\"PASS\",\"attach_duplicate\":true,\"raise_snapshot\":true,\"synchronous_payload_mutation\":true,\"priority_does_not_reorder\":true,\"delayed_detach\":true,\"return_one_stops\":true,\"provider_failure_preserved\":true,\"gather_loot_register_pickup_progress_completion\":true}");
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what());
        return 1;
    }
}
