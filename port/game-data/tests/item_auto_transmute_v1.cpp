#include "../item_auto_transmute_v1.hpp"

#include <cstdlib>
#include <iostream>

using namespace dh2::data;

namespace {
struct Fixture {
    unsigned transfers{};
    unsigned consumes{};
    unsigned gold_awards{};
    std::size_t world_index{};
    std::uintptr_t world_identity{};
    std::uintptr_t inventory_identity{0xcafe};
    std::uintptr_t consumed_identity{};
    std::int32_t item_id{-1};
    std::int32_t inventory_index{7};
    std::int32_t payout{};
    AutoTransmuteProviderResultV1 transfer_result{AutoTransmuteProviderResultV1::committed};
    AutoTransmuteProviderResultV1 consume_result{AutoTransmuteProviderResultV1::committed};
};

AutoTransmuteProviderResultV1 transfer(void* context, std::size_t world_index,
                                       std::uintptr_t identity, std::int32_t item_id,
                                       std::int32_t& inventory_index,
                                       std::uintptr_t& inventory_item_identity,
                                       std::string& error) {
    auto& f = *static_cast<Fixture*>(context);
    ++f.transfers; f.world_index = world_index; f.world_identity = identity;
    f.item_id = item_id; inventory_index = f.inventory_index;
    inventory_item_identity = f.inventory_identity;
    error.clear(); return f.transfer_result;
}

AutoTransmuteProviderResultV1 consume(void* context, std::int32_t inventory_index,
                                      std::uintptr_t identity, std::int32_t item_id,
                                      std::int32_t payout, std::string& error) {
    auto& f = *static_cast<Fixture*>(context);
    ++f.consumes;
    if (inventory_index != f.inventory_index || identity != f.inventory_identity || item_id != f.item_id) {
        error = "wrong transferred Item identity";
        return AutoTransmuteProviderResultV1::indeterminate;
    }
    f.consumed_identity = identity;
    f.payout = payout;
    // Model an effect that may have happened before the provider lost certainty.
    if (f.consume_result != AutoTransmuteProviderResultV1::not_applied) ++f.gold_awards;
    error.clear(); return f.consume_result;
}

AutoTransmuteFactsV1 eligible_facts() {
    AutoTransmuteFactsV1 facts{};
    facts.ready = true; facts.item_id = 42; facts.item_type = 3;
    facts.base_property = 9; facts.power_count = 1; facts.saved_option = 2;
    facts.item_value = 10; facts.transmute_bonus = 0;
    facts.transmute_multiplier = 65536;
    return facts;
}

void require(bool condition, const char* message, unsigned& cases) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
    ++cases;
}
}

int main() {
    unsigned cases = 0;
    std::string error;

    require(auto_transmute_value_v1(10, 0, 65536) == 2560,
            "source payout fixed-point formula", cases);
    require(auto_transmute_value_v1(10, 256, 65536) == 5120,
            "property 197 scales payout", cases);
    require(auto_transmute_value_v1(-1, 0, 0) == 1,
            "source minimum payout clamp", cases);

    // Disabled/equal threshold, potion and BaseProp sentinel stay on ordinary
    // pickup without touching either provider.
    for (unsigned mode = 0; mode != 3; ++mode) {
        auto facts = eligible_facts();
        if (mode == 0) facts.power_count = facts.saved_option;
        if (mode == 1) facts.item_type = 14;
        if (mode == 2) facts.base_property = -1;
        Fixture fixture; AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        require(auto_transmute_pickup_v1(2, 0x1234, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::normal_pickup,
                "ineligible item selects unchanged normal pickup", cases);
        require(fixture.transfers == 0 && fixture.consumes == 0 &&
                continuation.phase == AutoTransmutePhaseV1::pending,
                "normal pickup has no transmute side effects", cases);
    }

    // Strict less-than boundary, exact identity, and payout are carried through
    // one transfer and one consume. Repeated contacts cannot award twice.
    {
        Fixture fixture; AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(5, 0xabc, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::completed,
                "eligible item transmute completes", cases);
        require(fixture.transfers == 1 && fixture.consumes == 1 &&
                fixture.gold_awards == 1 && fixture.world_index == 5 &&
                fixture.world_identity == 0xabc &&
                fixture.consumed_identity == fixture.inventory_identity &&
                fixture.consumed_identity != fixture.world_identity && fixture.item_id == 42 &&
                fixture.payout == 2560,
                "transfer returns destination identity and consume targets merged/canonical Item", cases);
        require(auto_transmute_pickup_v1(5, 0xabc, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::already_completed &&
                fixture.transfers == 1 && fixture.consumes == 1 && fixture.gold_awards == 1,
                "completed contact retry cannot award twice", cases);
    }

    // A known non-application can resume at consume without repeating transfer;
    // an indeterminate result becomes terminal because it may have awarded gold.
    {
        Fixture fixture; fixture.consume_result = AutoTransmuteProviderResultV1::not_applied;
        AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(1, 0xdef, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::consume_not_applied,
                "known consume failure remains resumable", cases);
        fixture.consume_result = AutoTransmuteProviderResultV1::committed;
        require(auto_transmute_pickup_v1(1, 0xdef, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::completed &&
                fixture.transfers == 1 && fixture.consumes == 2 && fixture.gold_awards == 1,
                "consume retry reuses transfer and awards once", cases);
    }
    {
        Fixture fixture; fixture.consume_result = AutoTransmuteProviderResultV1::indeterminate;
        AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(1, 0x987, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::terminal_failure &&
                fixture.gold_awards == 1,
                "indeterminate consume is reported terminal", cases);
        fixture.consume_result = AutoTransmuteProviderResultV1::committed;
        require(auto_transmute_pickup_v1(1, 0x987, facts.item_id, facts, services,
                                        continuation, error) == AutoTransmuteStatusV1::terminal_failure &&
                fixture.transfers == 1 && fixture.consumes == 1 && fixture.gold_awards == 1,
                "terminal failure cannot duplicate uncertain award", cases);
    }

    {
        Fixture fixture; fixture.transfer_result = AutoTransmuteProviderResultV1::not_applied;
        AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(3, 0x777, facts.item_id, facts,
                                        services, continuation, error) ==
                    AutoTransmuteStatusV1::transfer_not_applied &&
                fixture.transfers == 1 && fixture.consumes == 0 &&
                continuation.phase == AutoTransmutePhaseV1::pending,
                "failed transfer cannot consume or award the Item", cases);
        fixture.transfer_result = AutoTransmuteProviderResultV1::committed;
        require(auto_transmute_pickup_v1(3, 0x777, facts.item_id, facts,
                                        services, continuation, error) ==
                    AutoTransmuteStatusV1::completed &&
                fixture.transfers == 2 && fixture.consumes == 1 &&
                fixture.gold_awards == 1,
                "transfer retry consumes only after commit", cases);
    }

    // A committed transfer must identify the retained destination Item. The
    // original may have been destroyed while AddItemInstance merged its stack.
    {
        Fixture fixture; fixture.inventory_identity = 0;
        AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 services{&fixture, transfer, consume};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(3, 0x777, facts.item_id, facts,
                                        services, continuation, error) ==
                    AutoTransmuteStatusV1::terminal_failure &&
                fixture.transfers == 1 && fixture.consumes == 0 &&
                continuation.phase == AutoTransmutePhaseV1::terminal_failure,
                "committed transfer without canonical destination identity cannot consume", cases);
    }

    {
        Fixture fixture; AutoTransmuteContinuationV1 continuation;
        AutoTransmuteServicesV1 transfer_only{&fixture, transfer, nullptr};
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(4, 0x888, facts.item_id, facts,
                                        transfer_only, continuation, error) ==
                    AutoTransmuteStatusV1::awaiting_provider &&
                fixture.transfers == 1 && fixture.consumes == 0 &&
                continuation.phase == AutoTransmutePhaseV1::transferred,
                "missing consume provider preserves transferred continuation", cases);
        AutoTransmuteServicesV1 complete{&fixture, transfer, consume};
        const AutoTransmuteFactsV1 unavailable_facts{};
        require(auto_transmute_pickup_v1(4, 0x888, facts.item_id, unavailable_facts,
                                        complete, continuation, error) ==
                    AutoTransmuteStatusV1::completed &&
                fixture.transfers == 1 && fixture.consumes == 1 &&
                fixture.gold_awards == 1,
                "provider recovery resumes at consume without another transfer", cases);
    }

    // No canonical V4 provider means fail closed; the kernel cannot invent a
    // second inventory or property store to make the branch appear complete.
    {
        AutoTransmuteContinuationV1 continuation;
        const auto facts = eligible_facts();
        require(auto_transmute_pickup_v1(0, 0x555, facts.item_id, facts, {}, continuation, error) ==
                    AutoTransmuteStatusV1::awaiting_provider &&
                continuation.phase == AutoTransmutePhaseV1::pending,
                "missing V4 provider leaves Item untouched and fails closed", cases);
    }

    std::cout << "PASS AutoTransmute kernel: " << cases
              << " assertions; canonical V4 callbacks required\n";
    return 0;
}
