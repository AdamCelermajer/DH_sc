#include "../character_current_equipped_faery_v1.hpp"
#include "../../game-data/player_savegame_v1.hpp"

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace e = dh2::character_current_equipped_faery_v1;
namespace s = dh2::character_current_spell_v1;
namespace d = dh2::data;
constexpr std::uintptr_t CHARACTER = 0x100000001ull;

void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

std::uint32_t word(const char* text) {
    return static_cast<std::uint32_t>(std::stoull(text));
}

std::int32_t signed_word(std::uint32_t value) {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

struct Fake {
    std::int32_t selected{};
    std::int32_t level{};
    int fail_at{-1};
    bool throws{};
    std::uint32_t calls{};
    std::vector<std::array<std::int64_t, 3>> trace;

    static int invoke(void* context, const s::Request* request, s::Response* response) {
        auto& self = *static_cast<Fake*>(context);
        require(request->character == CHARACTER, "captured Character changed");
        require(request->difficulty == -1, "source difficulty selector changed");
        const auto ordinal = self.calls++;
        self.trace.push_back({static_cast<std::int64_t>(request->operation),
                              static_cast<std::int64_t>(request->id), request->difficulty});
        if (request->operation == s::Operation::selected_faery) {
            require(request->id == 0, "selection service received an invented ID");
            response->value = self.selected;
        } else if (request->operation == s::Operation::saved_level) {
            require(request->id == static_cast<std::uint32_t>(self.selected),
                    "level read did not use the selected ID");
            response->value = self.level;
        } else {
            throw std::runtime_error("unexpected source service operation");
        }
        if (self.fail_at == static_cast<int>(ordinal)) {
            if (self.throws) throw std::runtime_error("source provider failed after effects");
            return -1;
        }
        return 0;
    }

    s::Services services() { return {this, invoke}; }
};

void print_oracle(const e::Result& result, e::Status status, const Fake& fake) {
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"faery_id\":" << result.faery_id
              << ",\"level\":" << result.level
              << ",\"calls\":" << result.calls
              << ",\"complete\":" << result.complete << ",\"trace\":[";
    for (std::size_t i = 0; i < fake.trace.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << '[' << fake.trace[i][0] << ',' << fake.trace[i][1] << ','
                  << fake.trace[i][2] << ']';
    }
    std::cout << "]}\n";
}

struct SavedFixture {
    d::PlayerSavegameV1 save;
    d::PlayerSavegameV1 foreign;
    const d::PlayerSavegameV1* selected{&save};
    std::int32_t difficulty{};
    s::SavedBindings saved_bindings{};
    s::Services services{};

    SavedFixture() {
        save.set_character(CHARACTER);
        foreign.set_character(CHARACTER + 2);
        save.initialize_faeries();
        foreign.initialize_faeries();
        std::string error;
        for (std::uint32_t diff = 0; diff < 3; ++diff) {
            for (std::uint32_t id = 0; id < 5; ++id) {
                require(save.set_faery_level(id, static_cast<std::int32_t>(1000 * diff + 10 * id + 3),
                                             diff, error), error.c_str());
            }
        }
        saved_bindings = {CHARACTER, &selected, &difficulty, nullptr, nullptr, nullptr};
        services = s::saved_services(&saved_bindings);
    }

    void select(std::uint32_t id) {
        const std::array<std::uint32_t, 3> ids{id, id, id};
        std::size_t used{};
        std::string error;
        require(save.load_current_faery(
                    {reinterpret_cast<const std::uint8_t*>(ids.data()), sizeof(ids)}, used, error) &&
                    used == sizeof(ids), error.c_str());
    }
};

struct ChangingDifficulty {
    s::Services saved_services{};
    std::int32_t* difficulty{};
    std::int32_t after_selection{};
    std::uint32_t selections{};

    static int invoke(void* context, const s::Request* request, s::Response* response) {
        auto& self = *static_cast<ChangingDifficulty*>(context);
        const int status = self.saved_services.invoke(
            self.saved_services.context, request, response);
        if (!status && request->operation == s::Operation::selected_faery) {
            ++self.selections;
            *self.difficulty = self.after_selection;
        }
        return status;
    }
};

struct CallbackFixture {
    Fake fake;
    e::Bindings bindings{CHARACTER, fake.services()};
};

struct Observation {
    std::uint32_t count{};
    float number{};
};

int main(int argc, char** argv) {
    try {
        if (argc == 5 && std::string(argv[1]) == "--oracle") {
            Fake fake;
            fake.selected = signed_word(word(argv[3]));
            fake.level = signed_word(word(argv[4]));
            const auto services = fake.services();
            e::Result result{};
            const auto status = std::string(argv[2]) == "id"
                ? e::current_equipped_faery_id(CHARACTER, &services, &result)
                : e::current_equipped_faery_level(CHARACTER, &services, &result);
            print_oracle(result, status, fake);
            return 0;
        }

        std::uint32_t checks = 0;
        for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
            SavedFixture fixture;
            fixture.difficulty = static_cast<std::int32_t>(difficulty);
            for (std::uint32_t id = 0; id < 5; ++id) {
                fixture.select(id);
                e::Result result{};
                require(e::current_equipped_faery_id(CHARACTER, &fixture.services, &result) ==
                            e::Status::complete && result.faery_id == static_cast<std::int32_t>(id) &&
                            result.calls == 1 && result.complete == 1,
                        "selected faery ID did not use the sole current-save read");
                ++checks;
                result = {};
                require(e::current_equipped_faery_level(CHARACTER, &fixture.services, &result) ==
                            e::Status::complete && result.faery_id == static_cast<std::int32_t>(id) &&
                            result.level == static_cast<std::int32_t>(1000 * difficulty + 10 * id + 3) &&
                            result.calls == 2 && result.complete == 1,
                        "selected faery level did not use ID then saved level");
                ++checks;
            }
        }

        {
            SavedFixture fixture;
            fixture.saved_bindings.difficulty = nullptr;
            fixture.selected = nullptr;
            e::Result id{};
            require(e::current_equipped_faery_id(CHARACTER, &fixture.services, &id) ==
                        e::Status::complete && id.faery_id == 0 && id.calls == 1,
                    "null save did not preserve source ID zero without reading difficulty");
            ++checks;
            e::Result level{};
            require(e::current_equipped_faery_level(CHARACTER, &fixture.services, &level) ==
                        e::Status::complete && level.faery_id == 0 && level.level == -1 &&
                        level.calls == 2,
                    "null save did not preserve source level -1 without reading difficulty");
            ++checks;
        }

        {
            SavedFixture fixture;
            fixture.select(2);
            fixture.difficulty = 0;
            ChangingDifficulty changing{fixture.services, &fixture.difficulty, 2, 0};
            const s::Services wrapped{&changing, ChangingDifficulty::invoke};
            e::Result result{};
            require(e::current_equipped_faery_level(CHARACTER, &wrapped, &result) ==
                        e::Status::complete && result.faery_id == 2 && result.level == 2023 &&
                        changing.selections == 1,
                    "level read did not observe source difficulty fresh on its second call");
            ++checks;
        }

        {
            Fake fake;
            const auto services = fake.services();
            e::Result result{};
            result.faery_id = 91;
            const auto original = result;
            require(e::current_equipped_faery_id(0, &services, &result) ==
                        e::Status::invalid_argument && !std::memcmp(&result, &original, sizeof(result)) &&
                        fake.calls == 0,
                    "invalid Character changed query output or called a provider");
            ++checks;
            require(e::current_equipped_faery_level(CHARACTER, nullptr, &result) ==
                        e::Status::invalid_argument && !std::memcmp(&result, &original, sizeof(result)) &&
                        fake.calls == 0,
                    "missing Services changed query output or called a provider");
            ++checks;
            require(e::current_equipped_faery_id(
                        CHARACTER, &services,
                        reinterpret_cast<e::Result*>(reinterpret_cast<std::uintptr_t>(&result) + 1)) ==
                        e::Status::invalid_argument && fake.calls == 0,
                    "misaligned result accepted");
            ++checks;
            require(e::current_equipped_faery_id(
                        CHARACTER, &services, reinterpret_cast<e::Result*>(
                            const_cast<s::Services*>(&services))) == e::Status::invalid_argument &&
                        fake.calls == 0,
                    "Services/result alias accepted");
            ++checks;
        }

        {
            SavedFixture fixture;
            e::Result result{};
            fixture.selected = &fixture.foreign;
            require(e::current_equipped_faery_id(CHARACTER, &fixture.services, &result) ==
                        e::Status::provider_failed && result.calls == 1 && !result.complete,
                    "foreign saved owner accepted");
            ++checks;
            fixture.selected = &fixture.save;
            fixture.difficulty = 3;
            result = {};
            require(e::current_equipped_faery_id(CHARACTER, &fixture.services, &result) ==
                        e::Status::provider_failed && result.calls == 1,
                    "unsupported difficulty accepted");
            ++checks;
            fixture.difficulty = 0;
            d::PlayerSavegameV1 fresh;
            fresh.set_character(CHARACTER);
            fixture.selected = &fresh;
            result = {};
            require(e::current_equipped_faery_id(CHARACTER, &fixture.services, &result) ==
                        e::Status::complete && result.faery_id == 0,
                    "ID query incorrectly requires faery level backing");
            ++checks;
            result = {};
            require(e::current_equipped_faery_level(CHARACTER, &fixture.services, &result) ==
                        e::Status::provider_failed && result.calls == 2,
                    "level query fabricated uninitialized faery rows");
            ++checks;
        }

        for (int failure = 0; failure < 2; ++failure) {
            for (bool throws : {false, true}) {
                Fake fake;
                fake.selected = 4;
                fake.level = 19;
                fake.fail_at = failure;
                fake.throws = throws;
                const auto services = fake.services();
                e::Result result{};
                const auto status = e::current_equipped_faery_level(CHARACTER, &services, &result);
                require(status == e::Status::provider_failed && result.calls ==
                            static_cast<std::uint32_t>(failure + 1) && !result.complete,
                        "failed source provider did not preserve the completed call prefix");
                if (failure == 1) require(result.faery_id == 4, "second-call error lost captured ID");
                ++checks;
            }
        }

        {
            CallbackFixture fixture;
            fixture.fake.selected = 3;
            fixture.fake.level = 47;
            dh2_script_value output{};
            std::uint32_t returned = 99;
            char error[128]{};
            require(e::current_equipped_faery_id_v1(
                        &fixture.bindings, reinterpret_cast<const dh2_script_value*>(1), UINT32_MAX,
                        &output, 1, &returned, error, sizeof(error)) == 0 && returned == 1 &&
                        output.type == DH2_SCRIPT_NUMBER && output.number == 3.0f,
                    "ID Lua callback read Arguments or changed source integer return");
            ++checks;
            output = {};
            returned = 99;
            require(e::current_equipped_faery_level_v1(
                        &fixture.bindings, nullptr, 0, &output, 1, &returned,
                        error, sizeof(error)) == 0 && returned == 1 &&
                        output.type == DH2_SCRIPT_NUMBER && output.number == 47.0f,
                    "level Lua callback returned the wrong integer");
            ++checks;

            const auto unchanged = output;
            returned = 99;
            fixture.fake.calls = 0;
            fixture.fake.fail_at = 0;
            require(e::current_equipped_faery_level_v1(
                        &fixture.bindings, nullptr, 0, &output, 1, &returned,
                        error, sizeof(error)) == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE &&
                        returned == 0 && !std::memcmp(&output, &unchanged, sizeof(output)),
                    "failed callback wrote an unreturned result");
            ++checks;

            fixture.fake.fail_at = -1;
            fixture.fake.calls = 0;
            returned = 99;
            require(e::current_equipped_faery_id_v1(
                        &fixture.bindings, nullptr, 0,
                        reinterpret_cast<dh2_script_value*>(&fixture.bindings), 1,
                        &returned, error, sizeof(error)) == DH2_SCRIPT_REQUIRED_SERVICE_FAILURE &&
                        returned == 99 && fixture.fake.calls == 0,
                    "binding/output alias was not rejected before service calls");
            ++checks;
        }

        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"native_wired\":false}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
