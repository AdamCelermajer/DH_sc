#include "level_manifest.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void require(bool result, const char* message) {
    if (!result) throw std::runtime_error(message);
}
bool decode(const std::string& xml, std::vector<ModulePlacement>& modules, std::string& error) {
    return decode_level_manifest({xml.begin(), xml.end()}, modules, error);
}
}
int main(int argc, char** argv) {
    try {
        std::vector<ModulePlacement> modules;
        std::string error;
        require(decode("<Level><GameObject gametype='Module' name='a' dae='any/model.bdae' xrefobject='piece' position='1,2,3' rotation='0,90,0' scale='2,3,4'/></Level>", modules, error), "Valid placement rejected");
        require(modules.size() == 1 && modules[0].authoredNode == "piece-node", "Node selector incorrect");
        const auto& m = modules[0].placement;
        require(m[12] == 1 && m[13] == 2 && m[14] == 3 && m[15] == 1, "Placement translation incorrect");
        // Export Y rotation is engine X rotation, so +90 sends local Y toward Z.
        require(std::abs(m[0]-2) < 0.0001f && std::abs(m[6]-3) < 0.0001f && std::abs(m[9]+4) < 0.0001f, "Authored rotation/scale conversion incorrect");
        const auto previous = modules[0].assetPath;
        require(!decode("<Level><GameObject gametype='Module' dae='x' xrefobject='y'></Level>", modules, error), "Malformed XML accepted");
        require(modules.size() == 1 && modules[0].assetPath == previous && !error.empty(), "Failed load changed prior result");
        require(!decode("<Level><Module dae='x' xrefobject='y' position='1,2,NaN'/></Level>", modules, error), "Nonfinite transform accepted");
        require(!decode("<!DOCTYPE Level><Level/>", modules, error), "DTD accepted");
        std::string deep;
        for (int i=0; i<130; ++i) deep += "<n>";
        for (int i=0; i<130; ++i) deep += "</n>";
        require(!decode(deep, modules, error), "Depth limit ignored");
        require(decode("<Level><!-- <ignored> --><Module dae='x&amp;y' xrefobject='z' visible='0' activate_cond='quest_ready'/></Level>", modules, error), "Entity/comment parse failed");
        require(!modules[0].visible && modules[0].assetPath == "x&y" && modules[0].activateCondition == "quest_ready", "Metadata lost");
        if (argc > 1) {
            const auto path = std::filesystem::path(argv[1]) / "port/windows-foundation/assets/original-cache/data/scene/001_swamp.mlx";
            require(load_level_manifest(path, modules, error), error.c_str());
            require(modules.size() == 9, "Original XML's nine module declarations were not preserved");
            std::cout << "Original level module placements: " << modules.size() << '\n';
        }
        std::cout << "Level manifest tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
