#include "../creation/creation_adapter.hpp"
#include <iostream>
#include <stdexcept>

namespace f = dh::foundation;
namespace c = f::frontend::creation;

int main() {
    unsigned calls = 0;
    const c::CreationRequest valid{"verification-player", "Verifier", "KnightPlayerBase"};
    // Synthetic values exercise adapter boundaries only, never campaign defaults.
    auto provider = [&](const c::CreationRequest& request, const c::ClassChoice&,
                        std::string&) -> std::optional<f::CharacterState> {
        ++calls;
        f::CharacterState value;
        value.id = request.character_id;
        value.name = request.player_name;
        value.class_id = request.class_token;
        return value;
    };
    auto require = [](bool condition, const char* message) {
        if (!condition) throw std::runtime_error(message);
    };
    try {
        const auto unavailable = c::stage_creation(valid);
        require(unavailable.status == c::CreationStatus::unavailable_service && !unavailable.character,
                "Unavailable source service exposed a character");
        for (const c::CreationRequest& invalid : {
             c::CreationRequest{"", "Verifier", "KnightPlayerBase"},
             c::CreationRequest{"verification-player", "Bad\nName", "KnightPlayerBase"},
             c::CreationRequest{"verification-player", "Verifier", "invented-class"}}) {
            const auto result = c::stage_creation(invalid, provider);
            require(result.status == c::CreationStatus::invalid_request && !result.character,
                    "Invalid request exposed a character");
        }
        require(calls == 0, "Invalid request reached initialization service");
        const auto mismatch = c::stage_creation(valid, [&](const auto& request, const auto& choice,
                                                           std::string& error) {
            auto result = provider(request, choice, error);
            result->id = "wrong-player";
            return result;
        });
        require(mismatch.status == c::CreationStatus::invalid_projection && !mismatch.character,
                "Mismatched initialization identity was staged");
        const auto exception = c::stage_creation(valid, [](const auto&, const auto&,
                std::string&) -> std::optional<f::CharacterState> {
            throw std::runtime_error("source-service-failure");
        });
        require(exception.status == c::CreationStatus::service_failure && !exception.character,
                "Source service exception escaped staging boundary");
        const auto warned = c::stage_creation(valid, [&](const auto& request, const auto& choice,
                                                        std::string& error) {
            auto result = provider(request, choice, error);
            error = "projection-incomplete";
            return result;
        });
        require(warned.status == c::CreationStatus::service_failure && !warned.character,
                "Incomplete projection with error was staged");
        std::cout << "Independent creation rejection boundary checks passed\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
