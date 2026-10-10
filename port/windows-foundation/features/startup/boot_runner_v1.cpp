#include "boot_runner_v1.hpp"

#include "asset_catalog.hpp"
#include "../../../engine-audio/audio_mixer_v34.hpp"
#include "content_paths.hpp"
#include "hud_glyphs.hpp"
#include "intro_movie_v2.hpp"
#include "intro_soundtrack_v2.hpp"
#include "overlay_renderer.hpp"
#include "platform_key_codes.hpp"
#include "platform_sleep.hpp"
#include "platform_win32.hpp"
#include "renderer.hpp"
#include "texture_loader.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iterator>

namespace dh::foundation::startup {
namespace {

// Rising edge of any press/tap source (touch and mouse map to mouse_left).
class PressEdge {
public:
    bool update(const Window& window) {
        const bool down = window.key_down(platform_key::mouse_left) || window.key_down(platform_key::space) ||
                          window.key_down(platform_key::enter) || window.key_down(platform_key::escape);
        const bool edge = down && !wasDown_;
        wasDown_ = down;
        return edge;
    }
private:
    bool wasDown_ = false;
};

// Fits (w,h) into the window keeping aspect ratio.
OverlaySprite fit_sprite(int windowW, int windowH, int w, int h, float alpha, std::uint32_t texture) {
    OverlaySprite s;
    const float scale = std::min(float(windowW) / float(w), float(windowH) / float(h));
    s.width = float(w) * scale;
    s.height = float(h) * scale;
    s.x = (float(windowW) - s.width) * 0.5f;
    s.y = (float(windowH) - s.height) * 0.5f;
    s.texture = texture;
    s.color = {1, 1, 1, alpha};
    return s;
}

void draw_black(OverlayRenderer& overlay, int w, int h) {
    const float fw = float(w), fh = float(h);
    const std::array<OverlayTriangleVertex, 6> quad{{{0, 0, 0, 0}, {fw, 0, 0, 0}, {fw, fh, 0, 0},
                                                     {0, 0, 0, 0}, {fw, fh, 0, 0}, {0, fh, 0, 0}}};
    overlay.drawTriangles(quad.data(), quad.size(), 0, {0, 0, 0, 1});
}

// Authored labels from original_art_data.cpp: MENU_TOUCH_TO_CONTINUE ("Touch  the  screen  to  continue",
// double spaces kept) and MENU_SKIP ("SKIP"). Placement of the skip label is a port choice (see report).
constexpr const char* kTouchToContinue = "Touch  the  screen  to  continue";
constexpr const char* kSkipLabel = "SKIP";

// A rasterised text line: one texture per non-blank glyph, built once.
struct TextLabel {
    std::vector<OverlaySprite> sprites;
    std::vector<std::uint32_t> textures;
    bool built = false;
    std::string error;
};

enum class LabelAnchor { center, right_bottom };

// Font = original Fontin SmallCaps file, the HUD font the port already uses;
// the menu's exact typography is not verified.
void build_text_label(Renderer& renderer, const AssetCatalog& assets, const char* text, int size, LabelAnchor anchor,
                      int w, int h, TextLabel& label) {
    label.built = true;
    HudGlyphFont font;
    std::string error;
    if (!font.load(resolve_content_path(assets, "data/Fontin SmallCaps.ttf"), error)) {
        label.error = std::string(text) + " font: " + error;
        return;
    }
    HudGlyphRun run;
    if (!font.raster(text, size, 1.0f, run, error)) {
        label.error = std::string(text) + " raster: " + error;
        return;
    }
    const bool right = anchor == LabelAnchor::right_bottom;
    const float baseline = right ? float(h) * 0.92f : float(h) * 0.86f;
    const float x0 = right ? float(w) * 0.96f - run.advance : (float(w) - run.advance) * 0.5f;
    for (const auto& g : run.glyphs) {
        if (g.image.rgba.empty()) continue;  // blank glyphs (spaces) draw nothing
        const std::uint32_t texture = renderer.createTexture(int(g.image.width), int(g.image.height), g.image.rgba.data());
        label.textures.push_back(texture);
        OverlaySprite s;
        s.x = x0 + g.x;
        s.y = baseline + g.y;
        s.width = g.width;
        s.height = g.height;
        s.u1 = g.u1;
        s.v1 = g.v1;
        s.texture = texture;
        label.sprites.push_back(s);
    }
}

bool read_file(const std::filesystem::path& path, std::vector<std::uint8_t>& bytes) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    bytes.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return true;
}

// Stops the soundtrack voice and keeps pumping until the mixer has released it (it reads the sample),
// bounded so a stalled output cannot hang the boot.
void release_soundtrack(IntroSoundtrackV2& soundtrack, dh2::audio::AudioMixerV34& mixer, const std::function<void()>& pump) {
    if (!soundtrack.started()) return;
    soundtrack.stop(mixer);
    const double until = Window::seconds() + 0.5;
    while (!soundtrack.released(mixer) && Window::seconds() < until) {
        if (pump) pump();
        soundtrack.drain_receipts(mixer);
        platform_sleep_milliseconds(2);
    }
}

} // namespace

BootRunResult run_boot_v1(Window& window, Renderer& renderer, const BootRunConfig& config) {
    BootRunResult result;
    if (!config.assets) {
        result.error = "boot: asset catalog unavailable";
        return result;
    }
    std::string error;
    TextureImage splashImage;
    // Splash variant chosen as the original GSInit::Update case 7 did (language 0 assumed).
    const char* splashUri = config.window_width == 854   ? "data/3d/textures/splash_final_droid.tga"
                            : config.window_width == 800 ? "data/3d/textures/splash_final_i9000.tga"
                                                         : "data/3d/textures/splash_final.tga";
    if (!load_texture(resolve_content_path(*config.assets, splashUri), splashImage, error)) {
        result.error = "boot splash: " + error;
        return result;
    }
    const std::uint32_t splashTexture = renderer.createTexture(int(splashImage.width), int(splashImage.height), splashImage.rgba.data());

    // Movie: an unusable file skips the movie with the reason recorded (never silent).
    IntroMovieV2 movie;
    bool movieAvailable = false;
    if (config.intro_movie.empty()) {
        result.movie_status = "skipped: no intro movie configured";
    } else {
        std::vector<std::uint8_t> bytes;
        if (!read_file(config.intro_movie, bytes)) {
            result.movie_status = "skipped: intro movie not found: " + config.intro_movie.string();
        } else if (!movie.open(std::move(bytes), error)) {
            result.movie_status = "skipped: " + error;
        } else {
            movieAvailable = true;
        }
    }

    // Soundtrack: decoded once, then one mixer voice. Its audible position is the movie clock.
    std::vector<std::int16_t> pcm;
    IntroSoundtrackV2 soundtrack;
    bool soundtrackStarted = false;
    if (movieAvailable && config.audio_mixer) {
        if (!movie.decode_soundtrack(pcm, error)) {
            result.error = "soundtrack: " + error;
        } else {
            result.soundtrack_duration = double(pcm.size() / 2) / double(movie.info().sample_rate);
            const std::uint64_t lead = movie.info().sample_rate / 10;  // start 100 ms after the first pump
            if (!soundtrack.start(*config.audio_mixer, pcm, movie.info().sample_rate, lead, config.audio_latency_frames, error))
                result.error = "soundtrack: " + error;
            else
                soundtrackStarted = true;
        }
    }
    result.movie_clock = soundtrackStarted ? "audio" : "wall";

    BootFlowV1 flow(false);
    PressEdge press;
    OverlayRenderer overlay;
    std::vector<std::uint8_t> frameRgba;
    std::uint32_t frameTexture = 0;
    TextLabel title, skip;
    std::vector<double> presses = config.scripted_presses;
    std::sort(presses.begin(), presses.end());
    std::size_t nextPress = 0;
    auto captures = config.captures;
    std::sort(captures.begin(), captures.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::size_t nextCapture = 0;
    const double start = Window::seconds();
    double movieStart = -1.0;     // boot time when the movie phase began
    bool useWallClock = !soundtrackStarted;
    bool movieEnded = false;      // picture reached its last frame and the clock passed it
    bool movieSkippedByUser = false;
    const double fps = movieAvailable ? movie.info().fps : 1.0;

    while (flow.phase() != BootPhase::complete && flow.phase() != BootPhase::quit) {
        window.poll();
        if (window.should_close()) {
            flow.request_quit();
            break;
        }
        const double now = Window::seconds() - start;
        if (config.max_seconds > 0.0 && now >= config.max_seconds) {
            flow.request_quit();
            break;
        }
        if (config.audio_pump) config.audio_pump();
        if (soundtrackStarted) soundtrack.drain_receipts(*config.audio_mixer);
        bool pressed = press.update(window);
        if (nextPress < presses.size() && now >= presses[nextPress]) {
            pressed = true;  // scripted verification press (same abstract edge as real input)
            ++nextPress;
        }
        const BootPhase before = flow.phase();
        flow.update(now, pressed);
        if (before == BootPhase::movie && flow.phase() == BootPhase::title && pressed) movieSkippedByUser = true;

        if (flow.phase() == BootPhase::movie && movieStart < 0.0) {
            movieStart = now;
            if (!movieAvailable) flow.movie_finished(now);  // no movie: continue at once
        }
        if (flow.phase() == BootPhase::movie && movieAvailable) {
            double clock = now - movieStart;
            if (!useWallClock) {
                const double audio = soundtrack.seconds(*config.audio_mixer);
                if (audio >= 0.0) {
                    clock = audio;  // master clock: the mixer's output frames
                } else {
                    clock = 0.0;    // soundtrack not audible yet: hold the first frame
                    if (now - movieStart > 3.0) {  // output never pumped (no device): wall clock, said so in the log
                        useWallClock = true;
                        result.movie_clock = "wall (audio not audible after 3 s)";
                        clock = now - movieStart;
                    }
                }
            }
            bool updated = false;
            if (!movie.frame_at(clock, frameRgba, updated, error)) {
                result.movie_status = "skipped: " + error;
                movieAvailable = false;
                flow.movie_finished(now);
            } else if (updated) {
                if (frameTexture) renderer.destroyTexture(frameTexture);
                frameTexture = renderer.createTexture(int(movie.info().width), int(movie.info().height), frameRgba.data());
                ++result.movie_frames_shown;
            }
            // The last frame stays up for its own duration: the movie ends at its end time.
            if (movieAvailable && movie.at_end() && clock >= double(movie.frames_decoded()) / fps) {
                movieEnded = true;
                result.movie_status = "played";
                flow.movie_finished(now);
            }
        }

        const int w = window.width(), h = window.height();
        renderer.resize(w, h);
        renderer.beginFrame(Camera{});
        overlay.begin(w, h);
        draw_black(overlay, w, h);
        if (flow.phase() == BootPhase::movie) {
            if (frameTexture) overlay.drawSprite(fit_sprite(w, h, int(movie.info().width), int(movie.info().height), 1.0f, frameTexture));
            // Skip overlay: any press (touch, mouse, Enter, Space, Escape) skips the movie.
            if (!skip.built) build_text_label(renderer, *config.assets, kSkipLabel, 22, LabelAnchor::right_bottom, w, h, skip);
            for (const auto& glyph : skip.sprites) overlay.drawSprite(glyph);
        } else if (flow.phase() == BootPhase::title) {
            overlay.drawSprite(fit_sprite(w, h, int(splashImage.width), int(splashImage.height), 1.0f, splashTexture));
            if (!title.built) build_text_label(renderer, *config.assets, kTouchToContinue, 28, LabelAnchor::center, w, h, title);
            for (const auto& glyph : title.sprites) overlay.drawSprite(glyph);
        }
        overlay.end();
        renderer.endFrame();
        while (config.capture && nextCapture < captures.size() && now >= captures[nextCapture].first) {
            config.capture(captures[nextCapture].second, w, h);  // host reads the back buffer
            ++nextCapture;
        }
        window.swap();
        platform_sleep_milliseconds(4);
    }

    if (soundtrackStarted) {
        result.soundtrack_seconds = std::max(0.0, soundtrack.seconds(*config.audio_mixer));
        release_soundtrack(soundtrack, *config.audio_mixer, config.audio_pump);
    }
    if (frameTexture) renderer.destroyTexture(frameTexture);
    renderer.destroyTexture(splashTexture);
    for (auto texture : title.textures) renderer.destroyTexture(texture);
    for (auto texture : skip.textures) renderer.destroyTexture(texture);
    if (!title.error.empty()) result.error = title.error;
    if (!skip.error.empty()) result.error = skip.error;

    result.seconds = Window::seconds() - start;
    result.outcome = flow.phase() == BootPhase::quit ? BootRunOutcome::quit : BootRunOutcome::complete;
    if (movieEnded) {
        // status already "played"
    } else if (movieSkippedByUser) {
        result.movie_status = "skipped: user";
    } else if (movieAvailable && result.outcome == BootRunOutcome::quit) {
        result.movie_status = "stopped before movie end";
    }
    return result;
}

} // namespace dh::foundation::startup
