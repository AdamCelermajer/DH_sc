#include "android/asset_manager.h"
#include "model_renderer.hpp"
#include "original_ui_session.hpp"
#include "../game-data/campaign_profile_files_v1.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>

namespace {
struct Options {
    std::filesystem::path assets;
    std::filesystem::path runtime;
    std::filesystem::path capture;
    std::uint64_t frames = 0;
};

Options parse(int argc, char** argv) {
    Options options;
#ifdef DH2_DEFAULT_ASSET_DIR
    options.assets = std::filesystem::u8path(DH2_DEFAULT_ASSET_DIR);
#else
    options.assets = std::filesystem::current_path() / "port/android-native/app/src/main/assets";
#endif
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        auto value = [&](const char* name) -> std::string {
            if (++i >= argc) throw std::runtime_error(std::string("Missing value for ") + name);
            return argv[i];
        };
        if (arg == "--assets") options.assets = value("--assets");
        else if (arg == "--runtime-dir") options.runtime = value("--runtime-dir");
        else if (arg == "--capture") options.capture = value("--capture");
        else if (arg == "--frames") {
            const auto count = value("--frames");
            std::size_t parsed = 0;
            options.frames = std::stoull(count, &parsed);
            if (parsed != count.size() || options.frames == 0)
                throw std::runtime_error("--frames must be a positive integer");
        }
        else if (arg == "--help") {
            std::cout << "DH2 desktop runtime\n"
                         "  --assets DIR       Android app asset tree\n"
                         "  --runtime-dir DIR  save/config output (default per-user AppData)\n"
                         "  --frames N         exit after N rendered gameplay frames\n"
                         "  --capture FILE.ppm save the last framebuffer as a PPM image\n"
                         "Controls: WASD move, Space attack nearest reachable enemy, mouse drag orbit, wheel zoom, Esc quit.\n";
            std::exit(0);
        } else throw std::runtime_error("Unknown argument: " + arg);
    }
    return options;
}

void update_movement(const std::array<bool, 4>& keys) {
    const float x = (keys[3] ? 1.0f : 0.0f) - (keys[2] ? 1.0f : 0.0f);
    const float y = (keys[0] ? 1.0f : 0.0f) - (keys[1] ? 1.0f : 0.0f);
    const float scale = x != 0 && y != 0 ? 0.70710678f : 1.0f;
    model_renderer::move_axis(x * scale, y * scale);
}

void print_vitals() {
    const auto values = model_renderer::player_vitals();
    std::cout << "HP " << values[0] / 256.0f << '/' << values[1] / 256.0f
              << "  MP " << values[2] / 256.0f << '/' << values[3] / 256.0f
              << (values[4] ? "  defeated" : "") << '\n';
}

struct PlayerUiFrame {
    dh2::android_ui::OriginalUiSession* session{};
    int width = 1;
    int height = 1;
};

bool draw_player_ui(void* context, const std::int32_t* sheet, std::size_t count,
                    std::uintptr_t character, std::string& error) {
    auto& frame = *static_cast<PlayerUiFrame*>(context);
    return frame.session->render_player(frame.width, frame.height, sheet, count, character, error);
}

bool dispatch_touch(dh2::android_ui::OriginalUiSession& session, SDL_Window* window,
                   int x, int y, int action, std::string& error) {
    int logical_width = 1, logical_height = 1, drawable_width = 1, drawable_height = 1;
    SDL_GetWindowSize(window, &logical_width, &logical_height);
    SDL_GetWindowSizeInPixels(window, &drawable_width, &drawable_height);
    const float sx = float(drawable_width) / std::max(1, logical_width);
    const float sy = float(drawable_height) / std::max(1, logical_height);
    return session.touch(float(x) * sx, float(y) * sy, action, error);
}

bool desktop_difficulty_count(void* context, std::uint32_t* count, std::string& error) {
    if (!context || !count) {
        error = "Desktop NativeCreateSaveSlot difficulty service is unavailable";
        return false;
    }
    return static_cast<dh2::android_ui::OriginalUiSession*>(context)
        ->game_difficulty_count(*count, error);
}

std::int32_t select_or_create_source_profile(
        AAssetManager* assets, const std::filesystem::path& runtime,
        dh2::android_ui::OriginalUiSession& original_ui) {
    std::string error;
    for (std::uint32_t candidate = 0; candidate < 4; ++candidate) {
        bool exists = false;
        if (!dh2::data::campaign_profile_exists_v1(runtime.string(), candidate, exists, error))
            throw std::runtime_error("Source campaign profile lookup failed: " + error);
        if (!exists) continue;
        if (!model_renderer::assign_menu_save_slot(static_cast<std::int32_t>(candidate), 0, error))
            throw std::runtime_error("Source campaign profile assignment failed: " + error);
        std::cout << "Using existing source campaign slot " << candidate << '\n';
        return static_cast<std::int32_t>(candidate);
    }

    std::int32_t slot = -1;
    bool published = false;
    if (!model_renderer::create_menu_save_slot(
            assets, "Desktop Knight", "KnightPlayerBase", slot, published, error,
            &original_ui, desktop_difficulty_count))
        throw std::runtime_error("Source profile creation failed: " + error);
    if (!published || slot < 0)
        throw std::runtime_error("NativeCreateSaveSlot did not publish a canonical campaign profile");
    if (!model_renderer::assign_menu_save_slot(slot, 0, error))
        throw std::runtime_error("Source campaign profile assignment failed: " + error);
    std::cout << "Created source campaign profile in slot " << slot << '\n';
    return slot;
}

void save_capture(const std::filesystem::path& path, int width, int height) {
    GLint viewport[4]{};
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[0] != 0 || viewport[1] != 0 || viewport[2] != width || viewport[3] != height)
        throw std::runtime_error("GLES capture viewport does not match the framebuffer dimensions");
    const auto prior_error = glGetError();
    if (prior_error != GL_NO_ERROR)
        throw std::runtime_error("GLES reported an error before framebuffer readback: " + std::to_string(prior_error));
    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width) * height * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glFinish();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    const auto read_error = glGetError();
    if (read_error != GL_NO_ERROR)
        throw std::runtime_error("GLES framebuffer readback failed with error " + std::to_string(read_error));
    if (!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    if (!output) throw std::runtime_error("Could not open capture path: " + path.string());
    output << "P6\n" << width << ' ' << height << "\n255\n";
    std::vector<std::uint8_t> rgb(static_cast<std::size_t>(width) * 3);
    for (int y = height - 1; y >= 0; --y) {
        const auto* row = pixels.data() + static_cast<std::size_t>(y) * width * 4;
        for (int x = 0; x < width; ++x) {
            rgb[static_cast<std::size_t>(x) * 3 + 0] = row[static_cast<std::size_t>(x) * 4 + 0];
            rgb[static_cast<std::size_t>(x) * 3 + 1] = row[static_cast<std::size_t>(x) * 4 + 1];
            rgb[static_cast<std::size_t>(x) * 3 + 2] = row[static_cast<std::size_t>(x) * 4 + 2];
        }
        output.write(reinterpret_cast<const char*>(rgb.data()), static_cast<std::streamsize>(rgb.size()));
    }
    if (!output) throw std::runtime_error("Framebuffer capture write failed: " + path.string());
}
}

int main(int argc, char** argv) {
    try {
        auto options = parse(argc, argv);
        options.runtime = std::filesystem::absolute(options.runtime);
        std::filesystem::create_directories(options.runtime);
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
        if (options.runtime.empty()) {
            char* preference = SDL_GetPrefPath("Gameloft", "Dungeon Hunter II");
            if (!preference)
                throw std::runtime_error(std::string("SDL_GetPrefPath: ") + SDL_GetError());
            options.runtime = std::filesystem::u8path(preference);
            SDL_free(preference);
        }
        SDL_Window* window = SDL_CreateWindow("Dungeon Hunter II | shared native runtime",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        if (!window) throw std::runtime_error(std::string("SDL_CreateWindow: ") + SDL_GetError());
        SDL_SysWMinfo window_info{};
        SDL_VERSION(&window_info.version);
        if (!SDL_GetWindowWMInfo(window, &window_info) || window_info.subsystem != SDL_SYSWM_WINDOWS)
            throw std::runtime_error(std::string("SDL Windows window handle: ") + SDL_GetError());
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        EGLint egl_major = 0, egl_minor = 0;
        if (display == EGL_NO_DISPLAY || !eglInitialize(display, &egl_major, &egl_minor))
            throw std::runtime_error("ANGLE eglInitialize failed");
        if (!eglBindAPI(EGL_OPENGL_ES_API)) throw std::runtime_error("ANGLE eglBindAPI failed");
        const EGLint config_attributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
        };
        EGLConfig egl_config{};
        EGLint config_count = 0;
        if (!eglChooseConfig(display, config_attributes, &egl_config, 1, &config_count) || config_count != 1)
            throw std::runtime_error("ANGLE has no compatible RGBA8/GLES2 window config");
        const EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        EGLContext context = eglCreateContext(display, egl_config, EGL_NO_CONTEXT, context_attributes);
        if (context == EGL_NO_CONTEXT) throw std::runtime_error("ANGLE GLES2 context creation failed");
        EGLSurface surface = eglCreateWindowSurface(display, egl_config,
            window_info.info.win.window, nullptr);
        if (surface == EGL_NO_SURFACE) throw std::runtime_error("ANGLE SDL window surface creation failed");
        if (!eglMakeCurrent(display, surface, surface, context))
            throw std::runtime_error("ANGLE eglMakeCurrent failed");
        eglSwapInterval(display, 1);

        std::unique_ptr<AAssetManager, decltype(&AAssetManager_delete)> assets(
            AAssetManager_createFromFilesystem(options.assets.string().c_str()),
            &AAssetManager_delete);
        if (!assets) throw std::runtime_error("Could not open asset directory: " + options.assets.string());

        model_renderer::runtime_directory(options.runtime.string());
        model_renderer::reset_context();
        dh2::android_ui::OriginalUiSession original_ui;
        std::string ui_error;
        if (!original_ui.initialize(assets.get(), ui_error))
            throw std::runtime_error("Shared UI asset services failed: " + ui_error);
        dh2::android_ui::FrontRuntimeServices ui_services{};
        ui_services.context = assets.get();
        ui_services.debug_load = [](void* context, std::string& error) {
            return model_renderer::menu_debug_load(static_cast<AAssetManager*>(context), error);
        };
        ui_services.debug_query = [](void* context, const char* key, std::string& error) {
            return model_renderer::menu_debug_query(static_cast<AAssetManager*>(context), key, error);
        };
        ui_services.save_game = [](void*, std::uintptr_t character, std::string& error) {
            return model_renderer::ui_player_save_game(character, error);
        };
        ui_services.camera_zoom_lock = [](void*, bool locked) {
            model_renderer::camera_zoom_lock(locked);
        };
        original_ui.bind_front_runtime(ui_services);
        const auto design_settings = original_ui.design_settings_table();
        model_renderer::bind_xp_design_settings(design_settings.data, design_settings.size);
        model_renderer::bind_item_text_services(&original_ui,
            [](void* context, const dh2::data::ItemTable& items,
               const dh2::data::CharacterTable& characters) {
                return static_cast<dh2::android_ui::OriginalUiSession*>(context)
                    ->item_text_services(items, characters);
            });
        // The real loading SWF creates the retained Application settings
        // owner used later by attach_player (including its authored HUDStyle).
        // Render it once, then hand its settings owner to gameplay.
        if (!original_ui.load_front_screen(options.runtime.string(), "loading", ui_error))
            throw std::runtime_error("Shared loading UI setup failed: " + ui_error);
        int initial_width = 1, initial_height = 1;
        SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
        if (!original_ui.render(initial_width, initial_height, ui_error))
            throw std::runtime_error("Shared loading UI frame failed: " + ui_error);
        const auto slot = select_or_create_source_profile(assets.get(), options.runtime, original_ui);
        const auto loaded = model_renderer::start_menu_game(slot, assets.get(), 23, false, 0);
        if (loaded.rfind("Crypt |", 0) != 0)
            throw std::runtime_error("Canonical source-menu Crypt launch failed: " + loaded);
        std::cout << loaded << '\n';
        if (!original_ui.attach_player(options.runtime.string(), ui_error))
            throw std::runtime_error("Shared player HUD attach failed: " + ui_error);
        SDL_GetWindowSizeInPixels(window, &initial_width, &initial_height);
        glViewport(0, 0, initial_width, initial_height);
        model_renderer::drawable_target_changed(initial_width, initial_height);
        const auto* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        std::cout << "Renderer: " << (renderer ? renderer : "unknown")
                  << " | GLES: " << (version ? version : "unknown") << '\n';
        std::cout << "WASD move | Space attack nearest reachable enemy | drag orbit | wheel zoom | Esc quit\n";

        std::array<bool, 4> keys{};
        bool running = true, dragging = false;
        std::uint64_t rendered_frames = 0;
        int mouse_x = 0, mouse_y = 0;
        auto last_vitals = SDL_GetTicks64();
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = 1, height = 1;
                    SDL_GetWindowSizeInPixels(window, &width, &height);
                    glViewport(0, 0, width, height);
                    model_renderer::drawable_target_changed(width, height);
                } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    if (dragging && !dispatch_touch(original_ui, window, mouse_x, mouse_y, 1, ui_error))
                        std::cerr << "HUD touch release on focus loss failed: " << ui_error << '\n';
                    dragging = false;
                    keys.fill(false);
                    model_renderer::stop_movement();
                } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
                    if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = true;
                    if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = true;
                    if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = true;
                    if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = true;
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                        std::cout << model_renderer::player_attack() << '\n';
                    update_movement(keys);
                } else if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_W) keys[0] = false;
                    if (event.key.keysym.scancode == SDL_SCANCODE_S) keys[1] = false;
                    if (event.key.keysym.scancode == SDL_SCANCODE_A) keys[2] = false;
                    if (event.key.keysym.scancode == SDL_SCANCODE_D) keys[3] = false;
                    update_movement(keys);
                    if (!keys[0] && !keys[1] && !keys[2] && !keys[3]) model_renderer::stop_movement();
                } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    dragging = true; mouse_x = event.button.x; mouse_y = event.button.y;
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 0, ui_error))
                        std::cerr << "HUD touch down failed: " << ui_error << '\n';
                } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                    if (!dispatch_touch(original_ui, window, event.button.x, event.button.y, 1, ui_error))
                        std::cerr << "HUD touch up failed: " << ui_error << '\n';
                    dragging = false;
                } else if (event.type == SDL_MOUSEMOTION && dragging) {
                    if (!dispatch_touch(original_ui, window, event.motion.x, event.motion.y, 2, ui_error))
                        std::cerr << "HUD touch move failed: " << ui_error << '\n';
                    if (original_ui.camera_pan_allowed())
                        model_renderer::orbit(float(event.motion.x - mouse_x),
                                              float(event.motion.y - mouse_y), 1.0f);
                    mouse_x = event.motion.x; mouse_y = event.motion.y;
                } else if (event.type == SDL_MOUSEWHEEL) {
                    model_renderer::orbit(0.0f, 0.0f, event.wheel.y > 0 ? 0.92f : 1.08f);
                }
            }
            int width = 1, height = 1;
            SDL_GetWindowSizeInPixels(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            model_renderer::draw(width, height);
            if (!model_renderer::active())
                throw std::runtime_error("Shared renderer deactivated during gameplay frame; frame rejected");
            PlayerUiFrame player_ui{&original_ui, width, height};
            if (!model_renderer::with_player_status_sheet(&player_ui, draw_player_ui, ui_error))
                throw std::runtime_error("Shared player HUD frame failed: " + ui_error);
            if (options.frames && rendered_frames + 1 == options.frames && !options.capture.empty())
                save_capture(options.capture, width, height);
            eglSwapBuffers(display, surface);
            ++rendered_frames;
            if (options.frames && rendered_frames >= options.frames) running = false;
            const auto now = SDL_GetTicks64();
            if (now - last_vitals >= 1000) { print_vitals(); last_vitals = now; }
        }

        model_renderer::stop_movement();
        model_renderer::deactivate();
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(display, surface);
        eglDestroyContext(display, context);
        eglTerminate(display);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "DH2 desktop runtime failed: " << error.what() << '\n';
        SDL_Quit();
        return 1;
    }
}
