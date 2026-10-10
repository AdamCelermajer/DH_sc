#include "content_paths.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <stdexcept>

namespace dh::foundation {
namespace {
namespace fs = std::filesystem;
std::string folded(std::string value) {
    std::transform(value.begin(),value.end(),value.begin(),[](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

// B062: directory listings were re-enumerated (every entry folded) for every path component of every candidate on
// every call; FX materials resolved textures this way every frame (4.5 ms per call, 700+ calls per second of combat).
// A listing is cached per directory and validated by the directory's own write time, so files added later are seen.
struct DirectoryListing {
    fs::file_time_type stamp{};
    std::unordered_multimap<std::string,fs::path> byFoldedName;
};
std::shared_ptr<const DirectoryListing> directory_listing(const fs::path& directory) {
    static std::mutex lock;
    static std::map<std::string,std::shared_ptr<const DirectoryListing>> cache;
    std::error_code ec;
    const auto stamp=fs::last_write_time(directory,ec);
    const auto key=directory.generic_string();
    std::lock_guard<std::mutex> guard(lock);
    if(!ec) {
        const auto found=cache.find(key);
        if(found!=cache.end()&&found->second->stamp==stamp) return found->second;
    }
    auto listing=std::make_shared<DirectoryListing>();
    listing->stamp=ec?fs::file_time_type{}:stamp;
    for(const auto& entry:fs::directory_iterator(directory))
        listing->byFoldedName.emplace(folded(entry.path().filename().generic_string()),entry.path());
    // Racy-timestamp guard (as in git): a directory modified within the last two seconds may change again inside the
    // same filesystem timestamp tick, so only settled directories are cached.
    if(!ec&&stamp<fs::file_time_type::clock::now()-std::chrono::seconds(2)) cache[key]=listing;
    return listing;
}

fs::path find_case_path(const fs::path& root, const fs::path& relative) {
    fs::path current=root;
    for(const auto& part:relative) {
        std::error_code ec;
        if(!fs::is_directory(current,ec)) return {};
        const auto wanted=folded(part.generic_string());
        const auto listing=directory_listing(current);
        const auto range=listing->byFoldedName.equal_range(wanted);
        fs::path match;
        for(auto it=range.first;it!=range.second;++it) {
            if(!match.empty()) throw std::runtime_error("Ambiguous resource path case: "+relative.generic_string());
            match=it->second;
        }
        if(match.empty()) return {};
        current=std::move(match);
    }
    return current;
}
} // namespace

std::string normalize_content_uri(const std::string& uri) {
    if(uri.empty()||uri.find('\0')!=std::string::npos)
        throw std::invalid_argument("Empty or NUL-containing content URI");
    std::string value=uri;
    std::replace(value.begin(),value.end(),'\\','/');
    // Original material exports retain an artist virtual drive. Recognize only
    // the resource namespace, never a general Windows absolute filesystem path.
    if(value.size()>8&&std::isalpha(static_cast<unsigned char>(value[0]))&&
       value[1]==':'&&value[2]=='/'&&folded(value.substr(3,5))=="data/")
        value.erase(0,3);
    if(value.front()=='/'||value.find(':')!=std::string::npos||value.find('?')!=std::string::npos)
        throw std::invalid_argument("Content URI must be a relative resource path: "+uri);
    // A COLLADA external reference may name an element after its file URI.
    const auto fragment=value.find('#');
    if(fragment!=std::string::npos) value.resize(fragment);
    if(value.empty()) throw std::invalid_argument("Content URI contains no resource file");
    fs::path result;
    for(const auto& component:fs::path(value)) {
        const auto part=component.generic_string();
        if(part=="..") throw std::invalid_argument("Content URI cannot traverse parents");
        if(part.empty()||part==".") continue;
        const auto alias=folded(part);
        if(alias=="iphone"||alias=="ps3"||alias=="debug"||alias=="old") continue;
        result/=component;
    }
    if(result.empty()) throw std::invalid_argument("Content URI contains no resource file");
    return result.generic_string();
}

std::vector<fs::path> content_path_candidates(const std::string& uri,const fs::path& ownerRelative) {
    auto normalized=normalize_content_uri(uri);
    fs::path owner;
    if(!ownerRelative.empty()) owner=fs::path(normalize_content_uri(ownerRelative.generic_string())).parent_path();
    std::vector<fs::path> result;
    auto append=[&](fs::path path) {
        path=path.lexically_normal();
        if(std::find(result.begin(),result.end(),path)==result.end()) result.push_back(std::move(path));
    };
    fs::path requested(normalized);
    std::vector<fs::path> variants{requested};
    const auto ext=folded(requested.extension().string());
    if(ext==".dae"||ext==".xml") {
        auto compiled=requested;compiled.replace_extension(".bdae");variants.push_back(std::move(compiled));
    }
    // The cache's iPhone PVRTC variant keeps a .tga resource extension but adds
    // pvr2_ to the exported texture name. Exhaust exact-name locations first.
    auto textureNamespace=folded(requested.generic_string());
    if(textureNamespace.rfind("original-cache/",0)==0)
        textureNamespace.erase(0,std::string("original-cache/").size());
    if(ext==".tga"&&(textureNamespace.rfind("data/3d/textures/",0)==0||
                    textureNamespace.rfind("textures/",0)==0)&&
       folded(requested.filename().string()).rfind("pvr2_",0)!=0) {
        variants.push_back(requested.parent_path()/("pvr2_"+requested.filename().string()));
    }
    for(const auto& variant:variants) {
        append(variant);
        if(!owner.empty()) append(owner/variant);
        auto raw=variant;
        const auto rawName=raw.generic_string();
        if(folded(rawName).rfind("original-cache/",0)==0)
            raw=fs::path(rawName.substr(std::string("original-cache/").size()));
        append(raw);append(fs::path("original-cache")/raw);
        // The Android package flattens the Python table cache into data/ even
        // though source owners request data/pydata/<name>. Preserve both
        // exact source/cache candidates above as higher precedence; expose
        // only this one-file .bin alias and let AssetCatalog enforce root
        // confinement when the candidate is resolved.
        if(folded(raw.parent_path().generic_string())=="data/pydata" &&
           folded(raw.extension().string())==".bin")
            append(fs::path("data")/raw.filename());
        // Original compiled Level::LoadFile search folders.
        for(const auto* folder:{"data","data/scene","data/3d/modules"}) {
            if(folded(raw.generic_string()).rfind("data/",0)==0) break;
            append(fs::path(folder)/raw);append(fs::path("original-cache")/folder/raw);
        }
        const auto name=raw.filename();
        const auto extension=folded(raw.extension().string());
        if(extension==".tga"||extension==".png"||extension==".pvr"||extension==".btex") {
            for(const auto* folder:{"data/3d/textures","textures"}) {
                append(fs::path(folder)/name);append(fs::path("original-cache")/folder/name);
            }
        } else if(extension==".bdae"||extension==".dae") {
            for(const auto* folder:{"data/3d/actors","data/3d/models","models","actors","animations","worlds"}) {
                append(fs::path(folder)/name);append(fs::path("original-cache")/folder/name);
            }
        } else if(extension==".dwld"||extension==".dact"||extension==".xml"||extension==".mlx") {
            append(fs::path("worlds")/name);
        }
    }
    return result;
}

fs::path resolve_content_path(const AssetCatalog& assets,const std::string& uri,const fs::path& ownerRelative) {
    for(const auto& candidate:content_path_candidates(uri,ownerRelative)) {
        const auto located=find_case_path(assets.root(),candidate);
        if(located.empty()) continue;
        std::error_code ec;
        if(!fs::is_regular_file(located,ec)) continue;
        // Keep AssetCatalog's canonical containment check authoritative.
        return assets.resolve(located.lexically_relative(assets.root()));
    }
    throw std::runtime_error("Original resource not found: "+uri);
}

std::vector<std::uint8_t> read_content(const AssetCatalog& assets,const std::string& uri,const fs::path& ownerRelative) {
    const auto resolved=resolve_content_path(assets,uri,ownerRelative);
    return assets.read(resolved.lexically_relative(assets.root()));
}
} // namespace dh::foundation
