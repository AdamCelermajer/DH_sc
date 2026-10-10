// Native test for B040 level ambience assets. Usage: level_music_v1_tests <features/audio/assets dir>
// Checks the staged original VXN files: manifest sizes/presence, sounds.xml UID rows,
// VXN segmented decode (non-silent first block) and the fresh native state used by
// RuntimeAudioHostV1::play_level_music. Start/stop/focus rules need the live session
// and are not covered here.
#include "../../../engine-audio/audio_sample_v34.hpp"
#include "level_music_v1.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {
int failures = 0;

void check(bool ok, const std::string& what) {
    std::cout << (ok ? "PASS " : "FAIL ") << what << '\n';
    if (!ok) ++failures;
}

std::string read_text(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

std::shared_ptr<const std::vector<std::uint8_t>> read_bytes(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    return std::make_shared<const std::vector<std::uint8_t>>(
        std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

// Each row of sounds.xml is one <sound .../> element. Returns the row for uid.
std::string sound_row(const std::string& xml, int uid) {
    const std::string needle = "uid=\"" + std::to_string(uid) + "\"";
    const auto at = xml.find(needle);
    if (at == std::string::npos) return {};
    const auto begin = xml.rfind("<sound ", at);
    const auto end = xml.find("/>", at);
    if (begin == std::string::npos || end == std::string::npos) return {};
    return xml.substr(begin, end - begin + 2);
}
}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: level_music_v1_tests <features/audio/assets>\n";
        return 2;
    }
    const fs::path assets = argv[1];
    const fs::path sounds = assets / "data" / "sounds";
    const std::string xml = read_text(sounds / "sounds.xml");
    const std::string manifest = read_text(assets.parent_path() / "source-subset-manifest.json");

    struct Expected { int uid; const char* file; const char* label; };
    const Expected rows[] = {
        {467, "m_level_swamp_sfx_swamp.vxn", "SwampHubAmbientMusic"},
        {468, "m_level_swamp_sfx_witch.vxn", "SwampWitchCaveAmbientMusic"},
        {470, "m_level_swamp_sfx_water.vxn", "WaterTempleCaveAmbientMusic"},
    };

    for (const auto& expected : rows) {
        const std::string row = sound_row(xml, expected.uid);
        const std::string label = std::string("label=\"") + expected.label + "\"";
        const std::string file = std::string("filename=\"") + expected.file + "\"";
        check(row.find(file) != std::string::npos && row.find(label) != std::string::npos &&
                  row.find("format=\"vxn\"") != std::string::npos && row.find("loop=\"yes\"") != std::string::npos &&
                  row.find("group=\"2\"") != std::string::npos,
              std::string("sounds.xml uid ") + std::to_string(expected.uid) + " -> " + expected.file + " vxn loop group 2");

        const fs::path path = sounds / expected.file;
        const auto size = fs::exists(path) ? fs::file_size(path) : 0;
        check(size == 12278272, std::string("staged file size ") + expected.file);
        check(manifest.find(std::string("\"data/sounds/") + expected.file + "\"") != std::string::npos,
              std::string("manifest entry ") + expected.file);

        // Real VXN container decode: the native segmented family must open and
        // expose states, and the first block of its first segment must be non-silent.
        dh2::audio::AudioSampleV34 sample;
        std::string error;
        const bool opened = size != 0 && dh2::audio::audio_sample_open_v34(read_bytes(path), sample, error);
        check(opened, std::string("VXN opens: ") + expected.file + (opened ? "" : " error=" + error));
        check(opened && sample.native && !sample.states.empty(), std::string("native states present: ") + expected.file);
        check(opened && !sample.states.empty() && sample.states[0].playlist < sample.playlists.size(),
              std::string("fresh native state 0 resolves a playlist: ") + expected.file);
        if (opened && !sample.segments.empty() && sample.block_align && sample.channels) {
            const auto& segment = sample.segments[0];
            const auto* data = sample.bytes->data() + sample.data_offset + segment.byte_offset;
            std::vector<std::int16_t> pcm(sample.block_align * 4 + 64, 0);
            const unsigned frames = dh2::audio::audio_ima_block_v34(data, sample.block_align, sample.channels,
                                                                    pcm.data(), unsigned(pcm.size()));
            bool non_silent = false;
            for (unsigned i = 0; i < frames * sample.channels && i < pcm.size(); ++i) non_silent |= pcm[i] != 0;
            check(frames > 0 && non_silent, std::string("first decoded VXN block is non-silent: ") + expected.file);
        } else {
            check(false, std::string("VXN has a decodable segment: ") + expected.file);
        }
    }

    // B040 transition policy (pure gate and log lines used by RuntimeSessionAudioV1).
    // The live start/stop/pause/revive effects need the integrated session; see the report.
    using namespace dh::foundation::audio;
    const int kNone = -1;
    check(!level_music_start_due_v1(false, true, 3, kNone), "no configured track: no start");
    check(!level_music_start_due_v1(true, false, 3, kNone), "unfocused or minimised output: no start (retry later)");
    check(!level_music_start_due_v1(true, true, kNone, kNone), "missing source row: no start");
    check(level_music_start_due_v1(true, true, 3, kNone), "focused, row 3, nothing owned: start");
    check(!level_music_start_due_v1(true, true, 3, 3), "same track already owned: no restart (Level::Update once)");
    check(level_music_start_due_v1(true, true, 3, 5), "different owned track: switch");
    check(level_music_start_due_v1(true, true, 3, kNone), "after revive-stop owned=-1: restart is due");
    check(kLevelMusicStartFadeMs == 2000 && kLevelMusicReviveStopMs == 2 && kLevelMusicReviveFadeMs == 1000 &&
              kLevelMusicReturnStopFadeMs == 1000 && kLevelMusicResumeMs == 50,
          "original fades: start 2000, revive stop 2 then restart 1000, return stop 1000, same-id resume 50");
    const auto start = level_music_transition_line_v1("start", "SwampHubAmbientMusic", kLevelMusicStartFadeMs, "ordinal=3");
    check(start == "Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000 ordinal=3",
          "start log line: " + start);
    const auto pause = level_music_transition_line_v1("output-pause", "SwampHubAmbientMusic", 0, "focused=0 minimized=0");
    check(pause.find("kind=output-pause") != std::string::npos && pause.find("focused=0") != std::string::npos,
          "output-pause log line names focus state");
    check(level_music_transition_line_v1("return-stop", "", 1000, "").find("track=none") != std::string::npos,
          "empty track is printed as none, never blank");

    std::cout << (failures == 0 ? "ALL PASS" : "FAILURES") << " (" << failures << ")\n";
    return failures == 0 ? 0 : 1;
}
