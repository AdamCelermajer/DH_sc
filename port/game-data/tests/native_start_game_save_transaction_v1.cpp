#include "../native_start_game_save_transaction_v1.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::data;
namespace {
struct FakeMetadata {
    std::uintptr_t identity = 0x680;
    NativeStartGameSaveViewV1 save;
    std::vector<std::string> calls;
    std::vector<std::uintptr_t> identities;
    bool fail_numeric = false;
    bool fail_spawn = false;
    bool mutate_level_row_on_numeric_save = false;
    std::int32_t saved_difficulty = -1;
};
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
bool read_save(void* raw, std::uintptr_t identity,
               NativeStartGameSaveViewV1& out, std::string& error) {
    auto& owner = *static_cast<FakeMetadata*>(raw);
    owner.calls.emplace_back("read"); owner.identities.push_back(identity);
    if (identity != owner.identity) { error = "wrong metadata Save"; return false; }
    out = owner.save; return true;
}
bool save_numeric(void* raw, std::uintptr_t identity,
                  std::int32_t& difficulty, std::string& error) {
    auto& owner = *static_cast<FakeMetadata*>(raw);
    owner.calls.emplace_back("numeric-save"); owner.identities.push_back(identity);
    if (identity != owner.identity) { error = "wrong metadata Save"; return false; }
    if (owner.fail_numeric) { error = "numeric save failed"; return false; }
    owner.saved_difficulty = difficulty;
    if (owner.mutate_level_row_on_numeric_save)
        owner.save.level_rows[2] = 41;
    return true;
}
bool save_spawn(void* raw, std::uintptr_t identity, std::size_t row,
                std::int32_t&, std::string& error) {
    auto& owner = *static_cast<FakeMetadata*>(raw);
    owner.calls.emplace_back("spawn-save:" + std::to_string(row));
    owner.identities.push_back(identity);
    if (identity != owner.identity) { error = "wrong metadata Save"; return false; }
    if (owner.fail_spawn) { error = "spawn save failed"; return false; }
    owner.save.use_spawn_points[row] = 0; return true;
}
NativeStartGameMetadataOwnerV1 provider(FakeMetadata& owner) {
    return {&owner, owner.identity, read_save, save_numeric, save_spawn};
}
LevelTables levels() {
    LevelTables value;
    value.levels.resize(42);
    value.levels[23].name = "GOTHICUS_CRYPT_01";
    value.levels[23].level_file = "007_crypt_01.rule.xml";
    value.levels[41].name = "SWAMP";
    value.levels[41].level_file = "001_swamp.mlx";
    return value;
}
}

int main() {
    try {
        auto table = levels();
        FakeMetadata metadata;
        metadata.save.slot = 3;
        metadata.save.unlocked_difficulty = 2;
        metadata.save.level_rows = {41, 23, 23};
        metadata.save.entry_points = {0, 2, 6};
        metadata.save.use_spawn_points = {1, 1, 1};
        metadata.mutate_level_row_on_numeric_save = true;
        NativeStartGameRequestV1 request;
        request.current_difficulty = 0;
        request.has_numeric_difficulty = true;
        request.requested_difficulty = 2;
        NativeStartGameSaveTransactionV1 transaction;
        std::int32_t current_difficulty = 0;
        std::string error;
        auto owner = provider(metadata);
        require(begin_native_start_game_save_transaction_v1(owner, table, request,
            transaction, current_difficulty, error), error.c_str());
        require(transaction.phase == NativeStartGameSavePhaseV1::planned &&
                transaction.save_identity == metadata.identity &&
                transaction.plan.slot == 3 && transaction.plan.level_row == 41 &&
                transaction.plan.entry_point == 6 &&
                transaction.plan.difficulty_before_request == 0 && current_difficulty == 2 &&
                metadata.saved_difficulty == 2,
                "plan must use the selected Save after numeric SG_Save side effects");
        require(metadata.calls == std::vector<std::string>{"read", "numeric-save", "read"},
                "numeric SG_Save must precede the post-save source-field read");

        require(save_spawn_flag_before_native_load_v1(owner, transaction,
            current_difficulty, error), error.c_str());
        require(transaction.phase == NativeStartGameSavePhaseV1::spawn_flag_saved &&
                metadata.save.use_spawn_points[2] == 0 &&
                metadata.calls.back() == "spawn-save:2",
                "selected spawn flag must be saved before load through same owner");
        for (const auto identity : metadata.identities)
            require(identity == metadata.identity,
                    "transaction callbacks crossed metadata Save identities");
        require(!save_spawn_flag_before_native_load_v1(owner, transaction,
            current_difficulty, error), "spawn save must not replay");

        auto replaced = provider(metadata); replaced.save_identity++;
        NativeStartGameSaveTransactionV1 mismatch;
        require(begin_native_start_game_save_transaction_v1(owner, table, request,
            mismatch, current_difficulty, error), error.c_str());
        require(!save_spawn_flag_before_native_load_v1(replaced, mismatch,
            current_difficulty, error), "owner replacement between plan and load must fail closed");

        metadata.fail_numeric = true;
        NativeStartGameSaveTransactionV1 failed;
        require(!begin_native_start_game_save_transaction_v1(owner, table, request,
            failed, current_difficulty, error) &&
                failed.phase == NativeStartGameSavePhaseV1::empty,
                "failed source numeric SG_Save must not publish a transaction");

        std::cout << "PASS NativeStartGame metadata transaction | same +680 identity | ordered SG_Save | owner swap/replay fail-closed\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "FAIL NativeStartGame metadata transaction: " << failure.what() << '\n';
        return 1;
    }
}
