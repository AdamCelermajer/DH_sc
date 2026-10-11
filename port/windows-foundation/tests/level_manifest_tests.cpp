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
        // Source getMatrix_transposed: export +Y sends local Y toward -Z.
        require(std::abs(m[0]-2) < 0.0001f && std::abs(m[6]+3) < 0.0001f && std::abs(m[9]-4) < 0.0001f, "Authored rotation/scale conversion incorrect");
        require(decode("<Level><Module dae='x' xrefobject='y' rotation='0,0,90' scale='2,3,4'/></Level>", modules, error), "Z rotation rejected");
        require(std::abs(modules[0].placement[1]-2)<0.0001f && std::abs(modules[0].placement[4]+3)<0.0001f, "Authored +Z must send +X toward +Y");
        require(decode("<Level><Module dae='x' xrefobject='y' rotation='90,0,0' scale='2,3,4'/></Level>", modules, error), "X rotation rejected");
        require(std::abs(modules[0].placement[2]+2)<0.0001f && std::abs(modules[0].placement[8]-4)<0.0001f, "Authored +X must send +X toward -Z");
        const std::string decorXml="<Module><GameObject name='arch' gametype='AnimatedDecor' dae='arch' xrefobject='arch' position='1,2,3' rotation='0,0,45'/></Module>";
        std::vector<ModulePlacement> decor;
        const Mat4 parent{1,0,0,0,0,1,0,0,0,0,1,0,10,20,30,1};
        require(decode_module_decor({decorXml.begin(),decorXml.end()},parent,decor,error), "Decor placement rejected");
        require(decor.size()==1 && std::abs(decor[0].placement[1]-std::sqrt(0.5f))<0.0001f && decor[0].placement[12]==11 && decor[0].placement[13]==22, "Decor rotation or parent translation incorrect");
        require(decode("<Level><Module dae='x' xrefobject='y' rotation='0,0,90' matrix='1 0 0 0 0 1 0 0 0 0 1 0 4 5 6 1'/></Level>",modules,error) && modules[0].placement[0]==1 && modules[0].placement[1]==0 && modules[0].placement[12]==4, "Explicit matrix precedence changed");
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
