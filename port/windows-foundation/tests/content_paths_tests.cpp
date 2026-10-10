#include "../content_paths.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace dh::foundation;
namespace {
void check(bool pass, const char* why) { if (!pass) throw std::runtime_error(why); }
template<class F> void fails(F action, const char* why) {
    bool failed=false; try { action(); } catch (const std::exception&) { failed=true; } check(failed,why);
}
struct Fixture {
    fs::path root=fs::temp_directory_path()/("dh-content-paths-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    Fixture(){ fs::create_directories(root); }
    ~Fixture(){ std::error_code ec; fs::remove_all(root,ec); }
};
void put(const fs::path& file,const std::string& text) {
    fs::create_directories(file.parent_path()); std::ofstream out(file,std::ios::binary); out<<text;
    check(out.good(),"Cannot create fixture resource");
}
void fixtures() {
    Fixture fixture; AssetCatalog catalog(fixture.root);
    check(normalize_content_uri("data\\iphone\\3d\\models\\hero.dae#mesh")=="data/3d/models/hero.dae","Authoring URI normalization failed");
    check(normalize_content_uri("./data/PS3/Debug/old/Hero.bdae")=="data/Hero.bdae","Platform aliases not normalized");
    for(const std::string uri:{"", "../hero.bdae", "data/../../hero.bdae", "C:\\hero.bdae", "/hero.bdae", "//host/hero.bdae", "https://host/hero.bdae", "hero.bdae?query"})
        fails([&]{ resolve_content_path(catalog,uri); },"Unsafe/missing resource path was accepted");
    fails([&]{ normalize_content_uri(std::string("safe\0bad",8)); },"NUL resource URI accepted");
    fails([&]{ content_path_candidates("hero.bdae","../outside.xml"); },"Unsafe owner path accepted");
    put(fixture.root/"textures/Hero.tga","flat");
    put(fixture.root/"original-cache/data/3d/textures/Hero.tga","original");
    const auto original=resolve_content_path(catalog,"data/iphone/3d/textures/hero.tga");
    check(fs::equivalent(original,fixture.root/"original-cache/data/3d/textures/Hero.tga"),"Original compiled resource lookup failed");
    put(fixture.root/"Hero.tga","exact");
    check(fs::equivalent(resolve_content_path(catalog,"Hero.tga"),fixture.root/"Hero.tga"),"Exact resource did not outrank fallback");
    put(fixture.root/"levels/local.bdae","owner");
    put(fixture.root/"models/local.bdae","flat");
    check(fs::equivalent(resolve_content_path(catalog,"local.bdae","levels/scene.xml"),fixture.root/"levels/local.bdae"),"Owner-relative resource did not outrank flat fallback");
    put(fixture.root/"models/compiled.bdae","compiled");
    check(fs::equivalent(resolve_content_path(catalog,"compiled.dae"),fixture.root/"models/compiled.bdae"),"DAE compiled fallback failed");
    check(read_content(catalog,"compiled.dae")==std::vector<std::uint8_t>({'c','o','m','p','i','l','e','d'}),"Content binary read changed payload");
    const fs::path authored_table_uri="data/pydata/source-table.bin";
    const auto exact_table=fixture.root/authored_table_uri;
    const auto cached_table=fixture.root/"original-cache/data/pydata/source-table.bin";
    const auto packaged_table=fixture.root/"data/source-table.bin";
    put(exact_table,"authored"); put(cached_table,"cached"); put(packaged_table,"packaged");
    check(fs::equivalent(resolve_content_path(catalog,authored_table_uri.generic_string()),exact_table),
          "Authored data/pydata path did not outrank cache and Android alias");
    std::error_code remove_error;
    fs::remove(exact_table,remove_error); check(!remove_error,"Could not remove exact precedence fixture");
    check(fs::equivalent(resolve_content_path(catalog,authored_table_uri.generic_string()),cached_table),
          "Original-cache data/pydata path did not outrank Android alias");
    fs::remove(cached_table,remove_error); check(!remove_error,"Could not remove cache precedence fixture");
    check(fs::equivalent(resolve_content_path(catalog,authored_table_uri.generic_string()),packaged_table),
          "Exact Android data/pydata basename alias did not resolve");
    check(read_content(catalog,authored_table_uri.generic_string())==
          std::vector<std::uint8_t>({'p','a','c','k','a','g','e','d'}),
          "Android data/pydata alias changed the source bytes");
    put(fixture.root/"data/unaliased.txt","no table");
    fails([&]{resolve_content_path(catalog,"data/pydata/unaliased.txt");},
          "Android alias accepted non-bin extension");
    put(fixture.root/"data/unsafe.bin","unsafe");
    fails([&]{resolve_content_path(catalog,"data/pydata/../unsafe.bin");},
          "Android alias bypassed URI traversal rejection");
    const auto candidates=content_path_candidates("data/iphone/3d/models/compiled.dae","levels/scene.xml");
    check(candidates==content_path_candidates("data/iphone/3d/models/compiled.dae","levels/scene.xml"),"Candidate ordering changed across identical calls");
    std::set<std::string> unique;
    for(const auto& path:candidates) check(unique.insert(path.generic_string()).second,"Duplicate fallback candidate");
    // Prove unsafe input cannot be converted to a safe basename fallback.
    fails([&]{resolve_content_path(catalog,"../../compiled.dae");},"Traversal bypassed through flat fallback");
    put(fixture.root/"ambiguous/Mixed.bin","first");
    put(fixture.root/"ambiguous/mixed.bin","second");
    if(!fs::equivalent(fixture.root/"ambiguous/Mixed.bin",fixture.root/"ambiguous/mixed.bin")) {
        fails([&]{resolve_content_path(catalog,"ambiguous/MIXED.bin");},"Case-ambiguous resource accepted");
        std::cout<<"Case ambiguity rejection exercised\n";
    } else std::cout<<"Case ambiguity fixture unavailable on this case-insensitive filesystem; check skipped\n";
    std::cout<<"Content path fixture checks passed\n";
}
void actual(const fs::path& assets) {
    AssetCatalog catalog(assets);
    const auto model_candidates=content_path_candidates("data/iphone/3d/characters/swampking/swampking.dae");
    const bool model_in_catalog=std::any_of(model_candidates.begin(),model_candidates.end(),[&](const fs::path& path) {
        return fs::is_regular_file(catalog.root()/path);
    });
    if(model_in_catalog) {
        const auto model=resolve_content_path(catalog,"data/iphone/3d/characters/swampking/swampking.dae");
        check(model.extension()==".bdae" && fs::file_size(model)>0,"Original model authoring URI did not resolve to compiled resource");
    }
    const auto table=resolve_content_path(catalog,"data/pydata/character_properties_pyarraynames.bin");
    const fs::path authored_table="data/pydata/character_properties_pyarraynames.bin";
    const auto expected=fs::exists(catalog.root()/authored_table)
        ? catalog.root()/authored_table
        : fs::exists(catalog.root()/"original-cache"/authored_table)
            ? catalog.root()/"original-cache"/authored_table
            : catalog.root()/"data/character_properties_pyarraynames.bin";
    check(fs::equivalent(table,expected),"Source table URI resolved outside exact packaged/cache candidate paths");
    const auto bank=read_content(catalog,"data/pydata/character_properties_pyarraynames.bin");
    check(!bank.empty(),"Actual character-property table is empty");
    std::cout<<"Actual original model and character-property paths resolved at "<<assets.string()<<'\n';
}
}
int main(int argc,char** argv) {
    try {
        fixtures();
        const fs::path repository=argc>1?fs::path(argv[1]):fs::current_path();
        actual(argc>2?fs::path(argv[2]):repository/".local-inputs/windows-population-assets");
        return 0;
    } catch(const std::exception& error) {std::cerr<<"Content path test failure: "<<error.what()<<'\n';return 1;}
}
