#include "../source_authored_module_bundle_v1.hpp"

#include <array>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace bundle = dh2::world::source_authored_module_bundle_v1;
namespace ledger = dh2::world::source_handle_ledger_v1;

namespace {

constexpr char mlx[] =
    "<Level>"
    "<GameObject name=\"config\" gametype=\"LevelConfig\"/>"
    "<GameObject name=\"room_a\" gametype=\"Module\" "
    "dae=\"data/3d/modules/test.bdae\" "
    "mgp=\"data/iphone/3d/modules/a.mgp\" "
    "mvp=\"data/3d/modules/a.mvp\" "
    "xrefobject=\"_room_a\" position=\"0,0,0\" "
    "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"room_b\" gametype=\"Module\" "
    "dae=\"data/3d/modules/test.bdae\" "
    "mgp=\"data/3d/modules/b.mgp\" "
    "mvp=\"data/3d/modules/b.mvp\" "
    "xrefobject=\"_room_b\" position=\"10,0,0\" "
    "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Level>";
constexpr char empty_module[] = "<Module/>";

struct Cache {
    std::vector<std::string> reads;
    bool fail_second_module = false;
};

bool read_cache(void* raw, const char* path, std::vector<std::uint8_t>& bytes,
                std::string& error) {
    auto& cache = *static_cast<Cache*>(raw);
    cache.reads.emplace_back(path ? path : "");
    if (cache.fail_second_module && cache.reads.back() == "data/3d/modules/b.mgp") {
        error = "injected cache miss";
        return false;
    }
    if (cache.reads.back() != "data/3d/modules/a.mgp" &&
        cache.reads.back() != "data/3d/modules/a.mvp" &&
        cache.reads.back() != "data/3d/modules/b.mgp" &&
        cache.reads.back() != "data/3d/modules/b.mvp") {
        error = "unexpected or non-normalized module cache path";
        return false;
    }
    bytes.assign(empty_module, empty_module + sizeof(empty_module) - 1);
    error.clear();
    return true;
}

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

void check_exact_module_order_and_ledger() {
    Cache cache;
    const bundle::Services services{&cache, read_cache};
    bundle::Bundle loaded;
    std::string error;
    require(bundle::load(reinterpret_cast<const std::uint8_t*>(mlx), sizeof(mlx) - 1,
                "TEST", "data/scene/test.mlx", services, 0, 3,
                loaded, error) == bundle::Status::complete,
            error.c_str());
    require(cache.reads == std::vector<std::string>{
                "data/3d/modules/a.mgp", "data/3d/modules/a.mvp",
                "data/3d/modules/b.mgp", "data/3d/modules/b.mvp"},
            "MLX module cache paths were not read in exact MGP/MVP source order");
    require(loaded.modules.size() == 2 && loaded.mgp_views.size() == 2 &&
                loaded.mvp_views.size() == 2 && loaded.ordered_files.size() == 4,
            "Authored Module bundle did not retain every module and typed view");
    require(loaded.handle_ledger.occurrences.size() == 3 &&
                loaded.handle_ledger.entries.size() == 3 &&
                loaded.handle_ledger.initial_top_level_handle_offset == 3,
            "Authored Module bundle did not use the canonical root/ObjectManager ledger");
    require(loaded.mgp_views[0].data == loaded.modules[0].mgp_bytes.data() &&
                loaded.mvp_views[1].data == loaded.modules[1].mvp_bytes.data() &&
                loaded.ordered_files[2].kind == ledger::ModuleFileKind::mgp &&
                loaded.ordered_files[3].kind == ledger::ModuleFileKind::mvp,
            "Borrowed views do not refer to the retained ordered cache bytes");
}

void check_failure_does_not_publish_partial_bundle() {
    Cache cache;
    cache.fail_second_module = true;
    const bundle::Services services{&cache, read_cache};
    bundle::Bundle output;
    output.modules.emplace_back();
    std::string error;
    require(bundle::load(reinterpret_cast<const std::uint8_t*>(mlx), sizeof(mlx) - 1,
                "TEST", "data/scene/test.mlx", services, 0, 3,
                output, error) == bundle::Status::cache_read_failed,
            "A missing authored MGP cache input was not rejected");
    require(output.modules.empty() && output.mgp_views.empty() &&
                output.handle_ledger.entries.empty() && !error.empty(),
            "A failed authored Module read published a partial bundle");
}

}  // namespace

int main() {
    try {
        check_exact_module_order_and_ledger();
        check_failure_does_not_publish_partial_bundle();
        std::cout << "source authored Module bundle regression passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
