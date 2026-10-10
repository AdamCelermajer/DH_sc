#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "frontend_runtime_v1.hpp"
#include "../../platform_win32.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace f = dh::foundation;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
    try {
        f::frontend::FrontendRunConfigV1 config;
        int captureEvery = 6;
        for (int i = 1; i < argc; ++i) {
            const std::string arg = argv[i];
            auto value = [&]() {
                if (++i >= argc) throw std::runtime_error("Missing argument value");
                return std::string(argv[i]);
            };
            if (arg == "--assets") config.asset_root = value();
            else if (arg == "--capture") config.capture_path = value();
            else if (arg == "--frames") config.frames = std::stoi(value());
            else if (arg == "--screen") config.initial_menu = value();
            else if (arg == "--class") config.selected_class = std::stoul(value());
            else if (arg == "--width") config.width = std::stoi(value());
            else if (arg == "--height") config.height = std::stoi(value());
            else if (arg == "--fixed-step") {
                config.fixed_step_seconds = std::stod(value());
                config.fixed_step_enabled = true;
            }
            else if (arg == "--actions") config.action_script = value();
            else if (arg == "--preview") {}
            else if (arg == "--capture-dir") config.capture_directory = value();
            else if (arg == "--capture-every") captureEvery = std::stoi(value());
            else if (arg == "--ui-assets") config.ui_assets = value();
            else if (arg == "--dump-layout") config.dump_layout = true;
            else if (arg == "--verify-native-input") config.verify_native_input = true;
            else if (arg == "--help") {
                std::cout << "Original native frontend: --assets DIR [--screen main|name|class|start --class 0|1|2] [--frames N --fixed-step S --capture IMAGE.ppm] [--actions path|text:NAME|path]\n"
                             "Mouse/touch authored buttons, PC text/Tab/Return, class arrows, Escape Back. New-game staging uses an empty isolated slot; actual save creation/start providers remain required and fail explicitly.\n";
                return 0;
            } else throw std::runtime_error("Unknown option " + arg);
        }
        config.capture_every = captureEvery;
        if (config.asset_root.empty() || config.selected_class > 2 || config.frames < 0 ||
            config.width <= 0 || config.height <= 0 || captureEvery < 1 ||
            (config.fixed_step_enabled &&
             (!std::isfinite(config.fixed_step_seconds) || config.fixed_step_seconds <= 0 ||
              config.fixed_step_seconds > 1)))
            throw std::runtime_error("Invalid native frontend options");

        f::Window window;
        if (!window.open("Dungeon Hunter 2 original frontend", config.width, config.height))
            throw std::runtime_error(window.error());
        f::Renderer renderer;
        if (!renderer.initialize(config.width, config.height))
            throw std::runtime_error("Renderer unavailable");

        f::frontend::FrontendRuntimeServicesV1 services;
        const auto result = f::frontend::run_frontend_v1(window, renderer, config,
                                                        std::move(services));
        if (result.outcome == f::frontend::FrontendRuntimeOutcomeV1::host_failed) {
            std::cerr << result.error << '\n';
            return 1;
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
