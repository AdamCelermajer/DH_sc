// Intro movie tests (Preview 15). Always run: header/garbage rejection, WAV image layout.
// With a movie path argument (converted intro_v1.mpg) they also check the decoded picture
// and soundtrack: size, frame rate, due-time cursor, soundtrack length. Pixel comparison
// against ffmpeg frames is done by the verifier script (port/windows-foundation/tools).

#include "intro_movie_v2.hpp"
#include "pcm_wav_v2.hpp"

#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using namespace dh::foundation::startup;

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

static std::uint32_t le32(const std::vector<std::uint8_t>& b, std::size_t at) {
    return b[at] | (b[at + 1] << 8) | (b[at + 2] << 16) | (std::uint32_t(b[at + 3]) << 24);
}

int main(int argc, char** argv) {
    // Garbage and empty input are rejected with a reason.
    {
        IntroMovieV2 movie;
        std::string error;
        CHECK(!movie.open({}, error));
        CHECK(!error.empty());
        IntroMovieV2 junk;
        std::vector<std::uint8_t> bytes(4096, 0x5a);
        CHECK(!junk.open(bytes, error));
        CHECK(!junk.valid());
        CHECK(junk.at_end());
    }
    // WAV image: RIFF/WAVE, PCM16 stereo at the given rate, sizes consistent.
    {
        std::vector<std::int16_t> pcm = {1, -1, 2, -2, 3, -3};  // three stereo frames
        const auto wav = make_pcm16_wav(pcm, 48000, 2);
        CHECK(wav.size() == 44 + pcm.size() * 2);
        CHECK(std::memcmp(wav.data(), "RIFF", 4) == 0);
        CHECK(std::memcmp(wav.data() + 8, "WAVE", 4) == 0);
        CHECK(le32(wav, 4) == wav.size() - 8);
        CHECK(std::memcmp(wav.data() + 12, "fmt ", 4) == 0);
        CHECK(le32(wav, 24) == 48000);          // sample rate
        CHECK(le32(wav, 28) == 48000 * 4);      // byte rate
        CHECK(wav[22] == 2 && wav[34] == 16);   // channels, bits
        CHECK(std::memcmp(wav.data() + 36, "data", 4) == 0);
        CHECK(le32(wav, 40) == pcm.size() * 2);
        CHECK(wav[44] == 1 && wav[45] == 0 && wav[46] == 0xff && wav[47] == 0xff);
    }
    // Optional real movie: picture cursor and soundtrack length.
    if (argc > 1) {
        std::ifstream f(argv[1], std::ios::binary);
        std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        CHECK(!bytes.empty());
        IntroMovieV2 movie;
        std::string error;
        CHECK(movie.open(std::move(bytes), error));
        CHECK(error.empty());
        if (movie.valid()) {
            const auto& info = movie.info();
            CHECK(info.width == 1024 && info.height == 576);
            CHECK(std::fabs(info.fps - 24.0) < 1e-6);
            CHECK(info.sample_rate == 48000 && info.channels == 2);

            std::vector<std::uint8_t> rgba;
            bool updated = false;
            // First frame is due at t = 0.
            CHECK(movie.frame_at(0.0, rgba, updated, error));
            CHECK(updated);
            CHECK(rgba.size() == std::size_t(info.width) * info.height * 4);
            CHECK(rgba[3] == 255);
            // Frame cursor: 1 s of playback shows the frame at 1.0 s and no more frames are due before 1/24 s.
            bool any = false;
            std::vector<std::uint8_t> frame24;
            for (int i = 1; i <= 24; ++i) {
                bool u = false;
                CHECK(movie.frame_at(double(i) / 24.0, rgba, u, error));
                any = any || u;
                if (i == 24) frame24 = rgba;  // frame 24 is presented at exactly 1.0 s
            }
            CHECK(any);
            // Optional: ffmpeg rgb24 dump of frame 24 (ffmpeg -i intro_v1.mpg -vf select=eq(n\,24) -vframes 1 -f rawvideo -pix_fmt rgb24).
            if (argc > 2) {
                std::ifstream r(argv[2], std::ios::binary);
                std::vector<std::uint8_t> ref((std::istreambuf_iterator<char>(r)), std::istreambuf_iterator<char>());
                const std::size_t pixels = std::size_t(info.width) * info.height;
                CHECK(ref.size() == pixels * 3);
                if (ref.size() == pixels * 3 && frame24.size() == pixels * 4) {
                    double squared = 0.0;
                    for (std::size_t p = 0; p < pixels; ++p)
                        for (std::size_t c = 0; c < 3; ++c) {
                            const double d = double(ref[p * 3 + c]) - double(frame24[p * 4 + c]);
                            squared += d * d;
                        }
                    const double mse = squared / double(pixels * 3);
                    const double psnr = mse > 0.0 ? 10.0 * std::log10(255.0 * 255.0 / mse) : 99.0;
                    std::printf("movie: frame 24 vs ffmpeg PSNR=%.2f dB\n", psnr);
                    CHECK(psnr > 30.0);
                }
            }
            CHECK(movie.frames_decoded() >= 25);
            bool u = true;
            CHECK(movie.frame_at(1.0, rgba, u, error));
            CHECK(!u);  // nothing new is due at the same time
            // The whole soundtrack: 50.2 s of stereo PCM at 48 kHz (MP2 frames plus the ffmpeg end padding).
            std::vector<std::int16_t> pcm;
            CHECK(movie.decode_soundtrack(pcm, error));
            const double seconds = double(pcm.size() / 2) / 48000.0;
            std::printf("movie: %ux%u fps=%.3f soundtrack=%.3f s (%zu stereo frames)\n", info.width, info.height, info.fps,
                        seconds, pcm.size() / 2);
            CHECK(seconds > 50.0 && seconds < 50.6);
            // Play to the end: the cursor must reach the end and report it.
            int guard = 0;
            while (!movie.at_end() && guard++ < 100000) {
                bool uu = false;
                CHECK(movie.frame_at(1000.0, rgba, uu, error));
            }
            CHECK(movie.at_end());
            std::printf("movie: frames=%llu\n", static_cast<unsigned long long>(movie.frames_decoded()));
            CHECK(movie.frames_decoded() == 1207);
        }
    }
    if (failures == 0) std::puts("intro_movie_v2 tests: all passed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
