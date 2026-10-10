#include "audio_source_cue_submission_v1.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>

using namespace dh::foundation::audio;
using namespace dh2::audio;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}

struct Fixture {
    std::string workspace;
    unsigned output_checks{};
    unsigned command_builds{};
};

std::shared_ptr<const std::vector<std::uint8_t>> read_bytes(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return {};
    return std::make_shared<const std::vector<std::uint8_t>>(
        std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

bool exact_asset(void* raw, const char* uri,
    std::shared_ptr<const std::vector<std::uint8_t>>& bytes, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    const std::string name(uri ? uri : "");
    std::string path;
    if (name.rfind("data/pydata/", 0) == 0) {
        path = fixture.workspace + "/port/engine-audio/reference/source-bindings-v38/" +
            name.substr(std::string("data/pydata/").size());
    } else if (name.rfind("data/sounds/", 0) == 0) {
        path = fixture.workspace + "/.local-inputs/audio-v34/cache/" +
            name.substr(std::string("data/sounds/").size());
    } else {
        error = "Unrecognized source audio URI: " + name;
        return false;
    }
    bytes = read_bytes(path);
    if (!bytes) {
        error = "Missing authentic source asset: " + name;
        return false;
    }
    error.clear();
    return true;
}

bool output_unavailable(void* raw, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.output_checks;
    error = "Focused source output unavailable (no device opened by this test)";
    return false;
}

bool command_would_build(void* raw, const AudioSoundV34&, const AudioGroupV34&,
                         AudioCommandV34& command, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.command_builds;
    command.left = command.right = 1.f;
    error.clear();
    return true;
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "usage: audio_source_cue_submission_v1_tests <workspace-root>");
        Fixture fixture;
        fixture.workspace = argv[1];
        AudioGameplaySourcesV40 sources;
        sources.context = &fixture;
        sources.exact_assets = {&fixture, exact_asset, nullptr};
        sources.output_ready = output_unavailable;
        sources.source_command = [](void*, const dh2::character::CombatSoundPlayV1&,
            const AudioSoundV34&, const AudioGroupV34&, AudioCommandV34&, std::string& error) {
            error = "Unexpected World Play3D source-command path";
            return false;
        };
        auto runtime = std::make_unique<AudioGameplayRuntimeV42>(0x534f55524345u, sources);
        std::string error;
        check(runtime->initialize_exact_source(error), "Source audio initialization failed: " + error);

        const auto ordinal = runtime->bindings().source_id("MenuSelect");
        check(ordinal == 143, "Recovered MenuSelect ordinal differs from source binding evidence");
        constexpr std::int64_t caller_event_ns = 1810000000000LL;
        AudioSourceCueSubmissionV1 cue;
        check(resolve_audio_source_cue_v1(runtime->bindings(), runtime->catalog(), ordinal,
            caller_event_ns, cue, error), "MenuSelect source cue resolution failed: " + error);
        check(cue.source_id == 143 && cue.xml_sound_uid == 3 &&
              cue.source_name == "MenuSelect" &&
              cue.exact_resource_uri == "data/sounds/sfx_menu_select.wav" &&
              cue.event_monotonic_ns == caller_event_ns,
              "Source ID→XML UID/resource/caller timestamp record changed");

        auto sample = runtime->load_sample_actual_xml_uid(cue.xml_sound_uid, error);
        check(bool(sample), "Authentic MenuSelect WAV failed decoder: " + error);
        check(sample->format == 1 && sample->channels == 2 && sample->rate == 32000 &&
              sample->bits == 16 && sample->segments.size() == 1 &&
              sample->segments[0].frames > 0,
              "MenuSelect source PCM format or segment metadata mismatch");
        AudioSampleCursorV34 cursor;
        check(cursor.bind(sample.get(), 0), "MenuSelect PCM decoder cursor rejected exact source sample");
        float left{}, right{};
        check(cursor.frame(0, left, right) && std::isfinite(left) &&
              std::isfinite(right),
              "MenuSelect first decoded stereo source frame invalid");

        const auto command = [&fixture](const AudioSoundV34& sound,
            const AudioGroupV34& group, AudioCommandV34& out, std::string& detail) {
            return command_would_build(&fixture, sound, group, out, detail);
        };
        check(!submit_audio_source_cue_v1(cue, *runtime, command, error) &&
              error.find("Focused source output unavailable") != std::string::npos,
              "No-device path did not fail at exact output-readiness gate");
        check(fixture.output_checks == 1 && fixture.command_builds == 0 &&
              runtime->last_token() == 0,
              "No-device cue unexpectedly built or posted a play command");

        struct MissingCue { int source_id; int uid; const char* name; const char* uri; };
        for (const auto& missing : {
            MissingCue{474, 282, "sfx_lizardman_attack_1", "data/sounds/sfx_lizardman_attack_1.wav"},
            MissingCue{477, 284, "sfx_lizardman_hurt", "data/sounds/sfx_lizardman_hurt.wav"},
            MissingCue{476, 285, "sfx_lizardman_die", "data/sounds/sfx_lizardman_die.wav"}}) {
            AudioSourceCueSubmissionV1 absent;
            check(resolve_audio_source_cue_v1(runtime->bindings(), runtime->catalog(),
                missing.source_id, caller_event_ns, absent, error),
                std::string("Missing Lizard source row did not resolve: ") + error);
            check(absent.xml_sound_uid == missing.uid && absent.source_name == missing.name &&
                  absent.exact_resource_uri == missing.uri,
                  "Lizard source diagnostic mapping was substituted");
            const auto absent_sample = runtime->load_sample_actual_xml_uid(missing.uid, error);
            check(!absent_sample && error.find(missing.uri) != std::string::npos,
                  "Missing original Lizard sample was substituted or lost its precise diagnostic");
        }

        AudioSourceCueSubmissionV1 unchanged = cue;
        check(!resolve_audio_source_cue_v1(runtime->bindings(), runtime->catalog(),
              ordinal, 0, unchanged, error) && unchanged.event_monotonic_ns == caller_event_ns,
              "Invalid synthesized/zero caller time modified resolved output");
        std::cout << "PASS MenuSelect source ordinal 143→UID3, exact 32 kHz PCM resource decoded without opening output; typed plain-source submit refuses absent device; lizard 474/477/476 retain missing original-resource diagnostics\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
