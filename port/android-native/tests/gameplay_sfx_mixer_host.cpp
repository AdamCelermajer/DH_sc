// Host test for the Android gameplay SFX backend core: WAV decode, mixing,
// fades/volume/pause, Vox priority-bank limits and the shared-queue consumer.
#include "../app/src/main/cpp/gameplay_sfx_engine_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace {
using namespace dh2;
using namespace dh2::gameplay_sfx;
namespace pga = dh2::player_gameplay_audio;

void check(bool ok, const char* message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void put16(std::vector<std::uint8_t>& v, std::uint16_t x) {
    v.push_back(std::uint8_t(x & 0xff));
    v.push_back(std::uint8_t(x >> 8));
}
void put32(std::vector<std::uint8_t>& v, std::uint32_t x) {
    put16(v, std::uint16_t(x & 0xffff));
    put16(v, std::uint16_t(x >> 16));
}
std::vector<std::uint8_t> make_wav(std::uint16_t channels, std::uint32_t rate,
                                   const std::vector<std::int16_t>& samples,
                                   std::uint16_t tag = 1, std::uint16_t bits = 16) {
    std::vector<std::uint8_t> v{'R', 'I', 'F', 'F'};
    put32(v, std::uint32_t(36 + samples.size() * 2));
    for (char c : {'W', 'A', 'V', 'E', 'f', 'm', 't', ' '}) v.push_back(std::uint8_t(c));
    put32(v, 16);
    put16(v, tag);
    put16(v, channels);
    put32(v, rate);
    put32(v, rate * channels * 2);
    put16(v, std::uint16_t(channels * 2));
    put16(v, bits);
    for (char c : {'d', 'a', 't', 'a'}) v.push_back(std::uint8_t(c));
    put32(v, std::uint32_t(samples.size() * 2));
    for (auto s : samples) put16(v, std::uint16_t(s));
    return v;
}
std::shared_ptr<Pcm> constant_pcm(std::int16_t value, std::uint32_t frames,
                                  std::uint32_t rate = 48000, std::uint8_t channels = 1) {
    auto pcm = std::make_shared<Pcm>();
    pcm->samples.assign(std::size_t(frames) * channels, value);
    pcm->frames = frames;
    pcm->channels = channels;
    pcm->rate = rate;
    return pcm;
}
std::vector<std::int16_t> render(Mixer& mixer, std::size_t frames) {
    std::vector<std::int16_t> out(frames * 2, 0x5a5a);
    mixer.render(out.data(), frames);
    return out;
}
bool near(double a, double b, double tolerance) { return std::fabs(a - b) <= tolerance; }

VoiceRequest voice(std::shared_ptr<const Pcm> pcm, std::uint16_t uid, std::uint8_t bank,
                   std::int32_t priority, std::uintptr_t owner = 0) {
    VoiceRequest r;
    r.pcm = std::move(pcm);
    r.uid = uid;
    r.bank = bank;
    r.priority = priority;
    r.owner = owner;
    return r;
}

void test_decode() {
    Pcm pcm;
    std::string error;
    auto mono = make_wav(1, 32000, {1, -2, 3});
    check(decode_wav(mono.data(), mono.size(), pcm, error) && pcm.frames == 3 &&
              pcm.channels == 1 && pcm.rate == 32000 && pcm.samples[1] == -2,
          "mono PCM16 WAV decodes");
    auto stereo = make_wav(2, 32000, {1, 2, 3, 4});
    check(decode_wav(stereo.data(), stereo.size(), pcm, error) && pcm.frames == 2 &&
              pcm.channels == 2 && pcm.samples[3] == 4,
          "stereo PCM16 WAV decodes");
    auto truncated = make_wav(1, 32000, {1, 2, 3, 4});
    truncated.resize(truncated.size() - 3); // data chunk claims more than exists
    check(decode_wav(truncated.data(), truncated.size(), pcm, error) && pcm.frames == 2,
          "a data chunk longer than the file is clamped to whole frames");
    auto adpcm = make_wav(2, 32000, {1, 2}, 17, 4);
    check(!decode_wav(adpcm.data(), adpcm.size(), pcm, error), "IMA ADPCM is not decoded as PCM");
    auto eight = make_wav(1, 32000, {1, 2}, 1, 8);
    check(!decode_wav(eight.data(), eight.size(), pcm, error), "8-bit PCM is rejected");
    const std::uint8_t junk[16] = {};
    check(!decode_wav(junk, sizeof junk, pcm, error), "non-RIFF data is rejected");
}

void test_mixing() {
    std::vector<BankRule> none;
    {  // same-rate unity playback, equal pan, 100% volume
        Mixer mixer(48000, none);
        check(mixer.play(voice(constant_pcm(1000, 100), 1, 0, 0)) == PlayResult::started, "plays");
        auto out = render(mixer, 64);
        check(out[0] == 1000 && out[1] == 1000 && out[126] == 1000, "mono source feeds both channels");
        out = render(mixer, 64);
        check(out[0] == 1000 && out[2 * 35] == 1000 && out[2 * 36] == 0, "voice ends after its last frame");
        check(mixer.active_voices() == 0, "finished voice is released");
    }
    {  // stereo source with different channels and Q14-style gains
        auto pcm = std::make_shared<Pcm>();
        pcm->channels = 2; pcm->rate = 48000; pcm->frames = 8;
        for (int i = 0; i < 8; ++i) { pcm->samples.push_back(2000); pcm->samples.push_back(-4000); }
        Mixer mixer(48000, none);
        auto request = voice(pcm, 1, 0, 0);
        request.left = 0.5f; request.right = 0.25f;
        mixer.play(request);
        auto out = render(mixer, 4);
        check(out[0] == 1000 && out[1] == -1000, "left/right gains scale each channel");
    }
    {  // 32 kHz -> 48 kHz linear interpolation: duration scales by 1.5
        Mixer mixer(48000, none);
        auto pcm = std::make_shared<Pcm>();
        pcm->channels = 1; pcm->rate = 32000; pcm->frames = 300;
        pcm->samples.assign(300, 3000);
        mixer.play(voice(pcm, 1, 0, 0));
        auto out = render(mixer, 440);
        check(out[2 * 100] == 3000 && out[2 * 439] == 3000, "resampled voice is steady while playing");
        out = render(mixer, 40);
        std::size_t audible = 0;
        for (std::size_t i = 0; i < 40; ++i) if (out[i * 2] != 0) ++audible;
        check(audible >= 9 && audible <= 11, "300 frames at 32 kHz last 450 frames at 48 kHz");
        check(mixer.active_voices() == 0, "resampled voice is released at its end");
    }
    {  // loop wraps without gaps and never ends
        Mixer mixer(48000, none);
        auto request = voice(constant_pcm(500, 10), 1, 0, 0);
        request.loop = true;
        mixer.play(request);
        auto out = render(mixer, 1000);
        bool steady = true;
        for (std::size_t i = 0; i < 1000; ++i) steady = steady && out[i * 2] == 500;
        check(steady && mixer.active_voices() == 1, "looping voice repeats indefinitely");
    }
    {  // fade in, fade-out stop, immediate stop declick
        Mixer mixer(48000, none);
        auto request = voice(constant_pcm(10000, 48000 * 2), 7, 0, 0, 0x11);
        request.fade_in_ms = 10;  // 480 frames
        mixer.play(request);
        auto out = render(mixer, 960);
        check(out[0] == 0 && near(out[2 * 240], 5000, 60) && out[2 * 700] == 10000,
              "linear fade-in reaches full gain after the fade time");
        check(mixer.stop(0x11, 7, 20) == 1, "stop finds the owner's voice");
        check(mixer.stop(0x12, 7, 0) == 0, "another owner's StopSound does not match");
        out = render(mixer, 960);
        check(near(out[2 * 480], 5000, 120) && out[2 * 959] <= 20,
              "stop fades linearly to silence");
        out = render(mixer, 64);
        check(mixer.active_voices() == 0, "faded-out voice is released");
        auto loud = voice(constant_pcm(10000, 48000), 8, 0, 0);
        loud.loop = true;
        mixer.play(loud);
        render(mixer, 8);
        mixer.stop_all(0);
        out = render(mixer, 200);
        check(out[0] > 5000 && out[2 * 150] == 0 && mixer.active_voices() == 0,
              "stop_all(0) silences within about 2 ms (declick)");
    }
    {  // master volume ramps over 50 ms and clamps
        Mixer mixer(48000, none);
        auto request = voice(constant_pcm(10000, 48000), 1, 0, 0);
        request.loop = true;
        mixer.play(request);
        mixer.set_master_volume(0.5f);
        auto out = render(mixer, 4800);
        check(out[0] > 9900 && near(out[2 * 1200], 7500, 60) && out[2 * 2399] <= 5010 &&
                  out[2 * 4799] == 5000,
              "FX volume option ramps to its target over 50 ms");
        mixer.set_master_volume(7.0f);
        out = render(mixer, 4800);
        check(out[2 * 4799] == 10000, "volume above 1 clamps to unity");
        mixer.set_master_volume(0.0f);
        out = render(mixer, 4800);
        check(out[2 * 4799] == 0, "zero volume silences every voice");
    }
    {  // pause / resume with fades; new voices are unaffected
        Mixer mixer(48000, none);
        auto request = voice(constant_pcm(10000, 48000 * 4), 1, 0, 0);
        mixer.play(request);
        mixer.pause_all(10);
        auto out = render(mixer, 960);
        check(out[2 * 959] == 0, "pause fades the voice to silence");
        out = render(mixer, 960);
        check(out[0] == 0, "paused voice stays silent");
        mixer.play(voice(constant_pcm(2000, 48000), 2, 0, 0));
        out = render(mixer, 16);
        check(out[0] == 2000, "a voice started while paused plays");
        mixer.resume_all(10);
        out = render(mixer, 960);
        check(out[2 * 959] == 12000, "resume brings the paused voice back to full gain");
    }
    {  // clipping
        Mixer mixer(48000, none);
        mixer.play(voice(constant_pcm(30000, 100), 1, 0, 0));
        mixer.play(voice(constant_pcm(30000, 100), 2, 0, 0));
        mixer.play(voice(constant_pcm(-30000, 100), 3, 0, 0));
        mixer.play(voice(constant_pcm(-30000, 100), 4, 0, 0));
        auto out = render(mixer, 2);
        check(out[0] == 0, "opposite voices cancel");
        mixer.play(voice(constant_pcm(30000, 100), 1, 0, 0));
        mixer.play(voice(constant_pcm(30000, 100), 2, 0, 0));
        out = render(mixer, 2);
        check(out[0] == 32767, "sum saturates at int16");
    }
}

void test_banks() {
    // Real Sounds.xml bank table: 7 banks with maxplaybacks/behaviour.
    check(pga::sfx_banks[2].max_playbacks == 6 && pga::sfx_banks[2].behaviour == 2 &&
              pga::sfx_banks[5].max_playbacks == 3 && pga::sfx_banks[5].behaviour == 3 &&
              pga::sfx_banks[6].behaviour == 0 && pga::sfx_banks[7].max_playbacks == 6,
          "generated bank table matches Sounds.xml");
    std::vector<BankRule> rules(3);
    rules[1] = {std::numeric_limits<std::int32_t>::min() + 1, 2, 0};  // steal oldest
    rules[2] = {std::numeric_limits<std::int32_t>::min() + 1, 2, 3};  // refuse
    {
        Mixer mixer(48000, rules);
        auto pcm = constant_pcm(100, 48000 * 3);
        check(mixer.play(voice(pcm, 1, 1, 1, 1)) == PlayResult::started, "bank 1 voice A");
        check(mixer.play(voice(pcm, 2, 1, 1, 2)) == PlayResult::started, "bank 1 voice B");
        check(mixer.play(voice(pcm, 3, 1, 1, 3)) == PlayResult::started, "bank full: steal oldest");
        check(mixer.bank_voices(1) == 2, "steal-oldest keeps the bank at maxplaybacks");
        check(mixer.stop(1, 1, 0) == 0, "oldest voice was released by the steal");
        check(mixer.stop(2, 2, 0) == 1 && mixer.stop(3, 3, 0) == 1, "newer voices survive");
    }
    {
        Mixer mixer(48000, rules);
        auto pcm = constant_pcm(100, 48000 * 3);
        mixer.play(voice(pcm, 1, 2, 1));
        mixer.play(voice(pcm, 2, 2, 1));
        check(mixer.play(voice(pcm, 3, 2, 3)) == PlayResult::refused_by_bank,
              "'do nothing' refuses new emitters even at higher priority");
        check(mixer.bank_voices(2) == 2, "refused emitter never joins the bank");
        mixer.stop(0, 1, 0);
        render(mixer, 400);
        check(mixer.play(voice(pcm, 4, 2, 1)) == PlayResult::started,
              "a freed slot admits the next emitter");
    }
    {  // steal lowest priority (behaviour 1): strictly higher priority wins
        std::vector<BankRule> r(2);
        r[1] = {std::numeric_limits<std::int32_t>::min() + 1, 3, 1};
        Mixer mixer(48000, r);
        auto pcm = constant_pcm(100, 48000 * 3);
        mixer.play(voice(pcm, 1, 1, 2, 1));
        mixer.play(voice(pcm, 2, 1, 1, 2));  // lowest priority, 2nd oldest
        mixer.play(voice(pcm, 3, 1, 2, 3));
        check(mixer.play(voice(pcm, 4, 1, 1, 4)) == PlayResult::refused_by_bank,
              "equal priority cannot steal under 'steal lowest priority'");
        check(mixer.play(voice(pcm, 5, 1, 3, 5)) == PlayResult::started,
              "higher priority steals");
        check(mixer.stop(2, 2, 0) == 0 && mixer.stop(1, 1, 0) == 1,
              "the lowest-priority voice was the victim");
    }
    {  // steal low prio or oldest at equal priority (behaviour 2)
        std::vector<BankRule> r(2);
        r[1] = {std::numeric_limits<std::int32_t>::min() + 1, 3, 2};
        Mixer mixer(48000, r);
        auto pcm = constant_pcm(100, 48000 * 3);
        mixer.play(voice(pcm, 1, 1, 2, 1));
        mixer.play(voice(pcm, 2, 1, 2, 2));
        mixer.play(voice(pcm, 3, 1, 2, 3));
        check(mixer.play(voice(pcm, 4, 1, 1, 4)) == PlayResult::refused_by_bank,
              "lower priority than every member is refused");
        check(mixer.play(voice(pcm, 5, 1, 2, 5)) == PlayResult::started,
              "equal priority steals the oldest");
        check(mixer.stop(1, 1, 0) == 0 && mixer.stop(2, 2, 0) == 1,
              "the oldest of the lowest-priority voices was the victim");
    }
    {  // a stopping voice leaves its bank immediately
        Mixer mixer(48000, rules);
        auto pcm = constant_pcm(100, 48000 * 3);
        mixer.play(voice(pcm, 1, 2, 1, 1));
        mixer.play(voice(pcm, 2, 2, 1, 2));
        mixer.stop(1, 1, 500);
        check(mixer.play(voice(pcm, 3, 2, 1, 3)) == PlayResult::started,
              "released (fading) voices do not hold a bank slot");
    }
    {  // unmanaged bank id
        Mixer mixer(48000, rules);
        auto pcm = constant_pcm(100, 100);
        for (int i = 0; i < 60; ++i) mixer.play(voice(pcm, 1, 9, 1));
        check(mixer.active_voices() == Mixer::kMaxVoices, "hard voice limit bounds the mixer");
    }
}

// In-memory asset reader serving a generated WAV for every requested path.
struct Assets {
    std::map<std::string, std::vector<std::uint8_t>> files;
    std::set<std::string> requested;
};

Engine make_engine(Assets& assets, std::uint32_t rate = 48000) {
    return Engine(rate, [&assets](const std::string& path, std::vector<std::uint8_t>& bytes,
                                  std::string& error) {
        assets.requested.insert(path);
        auto it = assets.files.find(path);
        if (it == assets.files.end()) {
            error = "missing";
            return false;
        }
        bytes = it->second;
        return true;
    });
}

void drain(Engine& engine, std::vector<Result>& results) {
    std::string cue;
    while (pga::consume(cue)) {
        if (cue.rfind("dh2fx,", 0) == 0) results.push_back(engine.command(cue));
    }
}

void test_engine() {
    Assets assets;
    const auto wav = make_wav(1, 32000, std::vector<std::int16_t>(32000 * 2, 4000));
    auto add = [&](const char* name) { assets.files[std::string("original-media/gameplay/") + name] = wav; };
    add("sfx_skill_mage_thunder_braid_add_2.wav");
    add("sfx_quest_new.wav");
    add("sfx_fs_walk_grass_1.wav");
    add("sfx_fs_walk_grass_2.wav");
    add("sfx_fs_walk_grass_3.wav");
    add("sfx_fs_walk_grass_4.wav");
    add("sfx_cutscene_swamp_intro_faerie.wav");
    add("sfx_magic_portal_loop.wav");
    assets.files["original-media/sfx_menu_back.wav"] = wav;
    {  // clear any queue state from a previous check
        std::string drop;
        while (pga::consume(drop)) {}
    }
    auto engine = make_engine(assets);
    std::vector<Result> results;

    // Lua-style PlaySound: SDD label -> queue -> engine voice with XML attributes.
    const char* chain = "sfx_skill_mage_thunder_braid_add_2";
    check(pga::enqueue(chain, std::strlen(chain), false), "label queued");
    drain(engine, results);
    check(results.size() == 1 && results[0].outcome == Outcome::started && results[0].uid == 118,
          "queued PlaySound becomes a started voice");
    check(engine.mixer().bank_voices(7) == 1, "voice joins its Sounds.xml bank (SKILLS)");
    auto out = render(engine.mixer(), 64);
    check(out[0] == 4000, "voice plays the decoded WAV at unity gain");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);

    // The Sounds.xml loop flag, not the caller's, decides looping.
    results.clear();
    pga::enqueue_play("sfx_magic_portal_loop", 21, 0, false, 0);  // XML loop=yes
    drain(engine, results);
    check(results[0].outcome == Outcome::started, "ambient loop starts");
    render(engine.mixer(), 48000 * 3);
    check(engine.mixer().active_voices() == 1, "XML loop=yes keeps playing despite the caller's flag");
    results.clear();
    pga::enqueue_stop("sfx_magic_portal_loop", 21, 0, 0.0f);
    drain(engine, results);
    check(results[0].outcome == Outcome::stopped, "StopSound is delivered");
    render(engine.mixer(), 200);
    check(engine.mixer().active_voices() == 0, "StopSound releases the looping voice");
    results.clear();
    pga::enqueue_play(chain, std::strlen(chain), 0, true, 0);  // caller says loop, XML says no
    drain(engine, results);
    render(engine.mixer(), 48000 * 3);
    check(engine.mixer().active_voices() == 0, "XML loop=no ignores the caller's loop flag");

    // UI sound through the NativePlaySoundFX path (PlayMenu semantics).
    results.clear();
    check(pga::enqueue_source_menu_sound("QuestNew") == pga::MenuSoundResultV1::queued,
          "NativePlaySoundFX('QuestNew') queues");
    drain(engine, results);
    check(results.size() == 1 && results[0].outcome == Outcome::started && results[0].uid == 10 &&
              engine.mixer().bank_voices(6) == 1,
          "QuestNew plays in the INT bank");
    check(pga::enqueue_source_menu_sound("BalrogAttack") == pga::MenuSoundResultV1::unavailable,
          "a known source sound whose PCM is not in the cache is reported unavailable");
    check(pga::enqueue_source_menu_sound("NotASound") == pga::MenuSoundResultV1::unknown,
          "names outside the SDD table are unknown");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);

    // Positional gains: Play3D through the contract; non-positional groups are centred.
    results.clear();
    engine_audio::spatial_playback_contract_v1::Request spatial{};
    spatial.sound_id = 540;
    spatial.has_sound_id = true;
    spatial.emitter_position = {3.0f, 4.0f, 0.0f};
    spatial.has_emitter_position = true;
    spatial.listener = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}};
    spatial.has_listener = true;
    spatial.emitter_parameters.by_source_id = {50.0f, 1.0f, 1.0f};
    spatial.has_emitter_parameters = true;
    spatial.has_distance_model = true;
    spatial.raw_parameter_0 = 1;
    check(pga::enqueue_spatial_source_sound(540, 0x44, false, 0, spatial), "positional request queued");
    spatial.sound_id = 166;  // QuestNew: Sounds.xml group sfx_int is non-positional
    check(pga::enqueue_spatial_source_sound(166, 0x45, false, 0, spatial), "non-positional queued");
    std::string wire;
    check(pga::consume(wire) && wire == "dh2fx,play,68,118,0,0,sfx_skill_mage_thunder_braid_add_2.wav,1,1465,2930",
          "positional wire format is unchanged");
    results.push_back(engine.command(wire));
    check(pga::consume(wire) && wire == "dh2fx,play,69,10,0,0,sfx_quest_new.wav,1,16384,16384",
          "non-positional sounds skip pan and distance gain");
    results.push_back(engine.command(wire));
    check(results[0].outcome == Outcome::started && results[1].outcome == Outcome::started,
          "both spatial cues start");
    out = render(engine.mixer(), 4);  // 50 ms fade-in at the start of each Play3D emitter
    check(out[0] == 0, "Play3D emitters fade in from silence");
    out = render(engine.mixer(), 4800);
    // Voice 1: 4000 * (1465/16384, 2930/16384); voice 2: 4000 centred, full gain.
    check(near(out[2 * 4000], 4000 * 0.0894 + 4000, 4) && near(out[2 * 4000 + 1], 4000 * 0.1788 + 4000, 4),
          "per-channel gains combine for the two voices");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);

    // Event rows: random footsteps never repeat inside the event's history.
    results.clear();
    pga::EventPicker::instance().seed(7);
    spatial.sound_id = 256;  // env_fs_walk_grass: random [64..67], history 2
    std::vector<std::uint16_t> picks;
    for (int i = 0; i < 40; ++i) {
        check(pga::enqueue_spatial_source_sound(256, 0x46, false, 0, spatial), "event row queued");
        std::string cue;
        check(pga::consume(cue), "event pick queued a cue");
        const auto r = engine.command(cue);
        check(r.outcome == Outcome::started && r.uid >= 64 && r.uid <= 67, "event member plays");
        picks.push_back(r.uid);
        engine.mixer().stop_all(0);
        render(engine.mixer(), 200);
    }
    bool repeats = false;
    for (std::size_t i = 1; i < picks.size(); ++i) repeats = repeats || picks[i] == picks[i - 1];
    check(!repeats, "random events do not repeat the previous pick (history 2)");
    std::set<std::uint16_t> distinct(picks.begin(), picks.end());
    check(distinct.size() == 4, "all four footstep variants are used");
    spatial.sound_id = 244;  // env_assassin_demon_hurt: monster PCM is not in the cache
    check(pga::find_sfx_event_row(244) == nullptr, "events without any staged member are not carried");

    // Bank limits through the real catalog: NPC bank is 3 / 'do nothing'.
    results.clear();
    for (const char* label : {"sfx_cutscene_swamp_intro_faerie"})
        for (int i = 0; i < 5; ++i) pga::enqueue_play(label, std::strlen(label), std::uintptr_t(i + 1), false, 0);
    drain(engine, results);
    int started = 0, refused = 0;
    for (const auto& r : results) {
        started += r.outcome == Outcome::started;
        refused += r.outcome == Outcome::refused_by_bank;
    }
    check(started == 3 && refused == 2, "NPC bank admits 3 emitters and refuses the rest");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);

    // Skill bank: 6 playbacks, steal oldest.
    results.clear();
    for (int i = 0; i < 8; ++i) pga::enqueue_play(chain, std::strlen(chain), std::uintptr_t(100 + i), false, 0);
    drain(engine, results);
    started = 0;
    for (const auto& r : results) started += r.outcome == Outcome::started;
    check(started == 8 && engine.mixer().bank_voices(7) == 6,
          "SKILLS bank keeps 6 voices and steals the oldest for newer ones");
    check(engine.mixer().stop(100, 118, 0) == 0 && engine.mixer().stop(107, 118, 0) == 1,
          "the stolen voices were the oldest");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);

    // Controls and malformed input.
    check(engine.control("volume,100,50").outcome == Outcome::control, "volume control parsed");
    check(engine.control("volume,1").outcome == Outcome::invalid, "malformed volume rejected");
    check(engine.control("pause-music").outcome == Outcome::ignored, "music-only control ignored");
    check(engine.command("dh2fx,play,0,118,0,0,wrong.wav").outcome == Outcome::invalid,
          "filename must match the retained UID");
    check(engine.command("dh2fx,play,0,65000,0,0,sfx_quest_new.wav").outcome == Outcome::invalid,
          "unknown UID is rejected");
    check(engine.command("dh2fx,play,0,118,0,2,sfx_skill_mage_thunder_braid_add_2.wav").outcome ==
              Outcome::invalid, "loop flag must be 0 or 1");
    check(engine.command("dh2fx,stopAll,70000").outcome == Outcome::invalid, "stopAll range checked");
    check(engine.command("dh2fx,menu,sfx_menu_back.wav").outcome == Outcome::started,
          "front-menu effect plays through the INT bank");
    check(engine.command("dh2fx,menu,../etc/passwd").outcome == Outcome::invalid,
          "only the five menu effects are accepted");
    assets.files.erase("original-media/gameplay/sfx_quest_new.wav");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);
    pga::enqueue_source_menu_sound("QuestNew");
    results.clear();
    drain(engine, results);
    check(results.size() == 1 && results[0].outcome == Outcome::started,
          "a decoded sample stays cached after its source disappears");

    // fx volume 0 mutes.
    engine.control("volume,0,0");
    engine.mixer().stop_all(0);
    render(engine.mixer(), 200);
    pga::enqueue_play(chain, std::strlen(chain), 1, false, 0);
    results.clear();
    drain(engine, results);
    render(engine.mixer(), 4800);
    out = render(engine.mixer(), 16);
    check(out[0] == 0, "fx volume 0 silences effects");
}

// Every staged SFX (hand-audited + generated) exists, is a decodable PCM16 WAV
// and is consistent with Sounds.xml-derived defs.
void test_assets(const std::string& root) {
    std::size_t files = 0, bytes = 0;
    std::set<std::string> seen;
    auto verify = [&](const pga::CatalogEntry& entry) {
        const auto* def = pga::find_sfx_def(entry.uid);
        check(def != nullptr, "every catalog UID has Sounds.xml attributes");
        check(def->bank >= 1 && def->bank <= 7 && def->group >= 4 && def->group <= 12,
              "SFX UIDs sit in SFX banks and non-music groups");
        check(pga::find_sfx_group(def->group) != nullptr && pga::find_sfx_bank(def->bank) != nullptr,
              "bank and group rows resolve");
        const std::string path = root + "/original-media/gameplay/" + entry.filename;
        std::ifstream in(path, std::ios::binary);
        check(in.good(), (std::string("staged asset missing: ") + path).c_str());
        std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(in)), {});
        Pcm pcm;
        std::string error;
        check(decode_wav(data.data(), data.size(), pcm, error),
              (std::string("asset does not decode: ") + path + " " + error).c_str());
        check(pcm.rate == 32000, "all SFX are 32 kHz");
        if (seen.insert(entry.filename).second) { ++files; bytes += data.size(); }
    };
    std::set<std::uint16_t> uids;
    for (const auto& entry : pga::catalog) { verify(entry); check(uids.insert(entry.uid).second, "unique UID"); }
    for (const auto& entry : pga::extended_catalog) { verify(entry); check(uids.insert(entry.uid).second, "unique UID"); }
    check(uids.size() == pga::sfx_defs.size(), "defs cover exactly the catalog");
    check(files >= 180, "the Sounds.xml SFX set is staged");
    // Event members resolve to staged sounds or to nothing.
    for (const auto& event : pga::sfx_events) {
        bool any = false;
        for (std::size_t i = 0; i < event.count; ++i) any = any || pga::find_uid(event.uids[i]);
        check(any, "every carried event has a staged member");
    }
    // Table invariants for binary searches.
    for (std::size_t i = 1; i < pga::sfx_defs.size(); ++i)
        check(pga::sfx_defs[i - 1].uid < pga::sfx_defs[i].uid, "defs sorted by UID");
    for (std::size_t i = 1; i < pga::sfx_event_rows.size(); ++i)
        check(pga::sfx_event_rows[i - 1].row < pga::sfx_event_rows[i].row, "event rows sorted");
    std::printf("assets: %zu distinct files, %zu bytes decoded OK\n", files, bytes);
}
}  // namespace

int main(int argc, char** argv) {
    const std::string root = argc > 1 ? argv[1] : "port/android-native/app/src/main/assets";
    test_decode();
    test_mixing();
    test_banks();
    test_engine();
    test_assets(root);
    std::cout << "PASS: gameplay SFX mixer, banks and queue consumer\n";
    return 0;
}
