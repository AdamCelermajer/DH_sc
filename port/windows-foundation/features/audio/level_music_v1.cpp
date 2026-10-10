#include "level_music_v1.hpp"
#include "../../content_paths.hpp"
#include "../../../level-loader/xml_document_v1.hpp"
#include <exception>

namespace dh::foundation::audio {

bool read_level_music_names_v1(const AssetCatalog& assets, const std::string& levelUri,
    LevelMusicNamesV1& names, std::string& error) {
    names = {};
    try {
        dh2::loader::XmlDocumentV1 document;
        if(!document.capture_level_buffer(levelUri, read_content(assets, levelUri), error))return false;
        const auto source = document.borrow();
        if(!source.parsed()) {
            error = source.diagnostic().message;
            return false;
        }
        for(const auto& element : source.elements()) {
            const auto* type = element.attribute("gametype");
            if(!type || *type != "LevelConfig")continue;
            if(const auto* music = element.attribute("music"))names.music = *music;
            if(const auto* safezone = element.attribute("safezone_music"))names.safezone = *safezone;
            error.clear();
            return true;
        }
        error = "Level scene has no LevelConfig element: " + levelUri;
        return false;
    } catch(const std::exception& failure) {
        error = std::string("Level music scene read failed: ") + failure.what();
        return false;
    }
}

} // namespace dh::foundation::audio
