// Test for the rnd::RandomGenerator batch g1-game-app-rnd-8b06ae-0. It links with the batch's own source only, so the
// engine functions that source references are defined here as link-only stubs. The tested paths do not reach the
// stubs that abort. Returns non-zero on failure.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#include "core-platform/memory.hpp"
#include "dh2/LevelConfig.hpp"
#include "dh2/PropertyMap.hpp"
#include "dh2/global/ToLowerCase.hpp"
#include "dh2/rnd/ListRule.hpp"
#include "dh2/rnd/RandomGenerator.hpp"
#include "dh2/rnd/RoomPool.hpp"
#include "dh2/rnd/Tile.hpp"

// ---- link-only stubs (the batch source references these; the tested paths do not call the aborting ones) ----

namespace rnd {
void Tile::Unspawn() { std::abort(); }
void Tile::Print() { std::abort(); }
RootRule::RootRule(RandomGenerator&) {}
RootRule::~RootRule() {}
void RootRule::Unload() {}
RootRule::Impl::Impl(const RootRule&) {}
RootRule::Impl::~Impl() {}
bool RootRule::Impl::Generate(Array2d<Tile*>&) { return false; }   // no placement: Generate reports failure
void RoomPool::LoadFromXml(TiXmlNode*) { std::abort(); }
void ListRule::LoadFromXml(TiXmlNode*) { std::abort(); }
}  // namespace rnd

LevelConfig::LevelConfig(ObjectBase::GO_IDS) {}
void PropertyMap::InitProperties() {}
void PropertyMap::LoadDefaultProperties() {}

bool lstr::operator()(const char* lhs, const char* rhs) const { return std::strcmp(lhs, rhs) < 0; }

std::size_t ToLowerCase(char* str, int from, int to) {
    if (!str) return 0;
    const std::size_t len = std::strlen(str);
    if (to < 0 || static_cast<std::size_t>(to) >= len) to = static_cast<int>(len) - 1;
    for (int i = from; i <= to; ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') str[i] = static_cast<char>(str[i] - 'A' + 'a');
    }
    return len;
}

void* CustomAlloc(unsigned size) { return std::malloc(size); }
void CustomFree(void* p) { std::free(p); }

// ---- test helpers ----

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                                       \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            std::fprintf(stderr, "%s:%d: EXPECT failed: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                                  \
        }                                                                                  \
    } while (0)

// Independent reference for one generator step: next = (279470273 * (state + 1)) mod 0xFFFFFFFB.
std::uint64_t RefStep(std::uint32_t state) {
    return (279470273ull * static_cast<std::uint64_t>(state + 1u)) % 0xFFFFFFFBull;
}

// Reference djb2 variant written as shifts: h = (h << 5) + h + c, from 5381, up to the NUL.
std::uint32_t RefHash(const char* s) {
    std::uint32_t h = 5381u;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(s); *p != 0; ++p) {
        h = (h << 5) + h + *p;
    }
    return h;
}

// Generators are created with new and never destroyed: the destructor runs UnloadRules, which needs the TiXmlDocument
// seam that this test does not model.
rnd::RandomGenerator* MakeGenerator(std::uint32_t state) {
    rnd::RandomGenerator* gen = new rnd::RandomGenerator();
    gen->m_rngState = state;
    return gen;
}

void TestNextInt() {
    rnd::RandomGenerator* gen = MakeGenerator(1);
    std::uint32_t state = 1;
    for (int i = 0; i < 5; ++i) {
        const std::uint64_t expected = RefStep(state);
        const int got = gen->NextInt();
        EXPECT(static_cast<std::uint32_t>(got) == static_cast<std::uint32_t>(expected));
        EXPECT(gen->m_rngState == static_cast<std::uint32_t>(expected));
        state = static_cast<std::uint32_t>(expected);
    }
}

void TestGetInt() {
    rnd::RandomGenerator* gen = MakeGenerator(7);
    const std::uint32_t before = gen->m_rngState;
    EXPECT(gen->GetInt(5, 5) == 5);
    EXPECT(gen->GetInt(9, 3) == 9);
    EXPECT(gen->m_rngState == before);   // no draw when lo >= hi

    // One draw: lo + next % (hi - lo).
    const std::uint64_t expected = RefStep(7u) % 10u;
    EXPECT(gen->GetInt(10, 20) == 10 + static_cast<int>(expected));

    rnd::RandomGenerator* range = MakeGenerator(99);
    for (int i = 0; i < 1000; ++i) {
        const int v = range->GetInt(-4, 4);
        EXPECT(v >= -4 && v < 4);
    }
}

void TestCallOperator() {
    rnd::RandomGenerator* gen = MakeGenerator(3);
    const std::uint64_t next = RefStep(3u);
    EXPECT(gen->operator()(100u) == static_cast<unsigned int>(next % 100u));
}

void TestHash() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    unsigned char empty[] = "";
    unsigned char a[] = "a";
    unsigned char ab[] = "ab";
    unsigned char hello[] = "hello, dungeon";
    EXPECT(gen->Hash(empty) == 5381u);
    EXPECT(gen->Hash(a) == 33u * 5381u + 97u);          // 177670
    EXPECT(gen->Hash(ab) == 33u * (33u * 5381u + 97u) + 98u);
    EXPECT(gen->Hash(hello) == RefHash("hello, dungeon"));
}

void TestListLookup() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    // ListRule objects are plain storage here (never destroyed); the name is the map key.
    void* mem = std::malloc(sizeof(rnd::ListRule));
    rnd::ListRule* forest = new (mem) rnd::ListRule();
    forest->m_name = "forest";
    gen->m_listRules.insert(std::make_pair(forest->m_name.c_str(), forest));

    EXPECT(gen->GetList("forest") == forest);
    EXPECT(gen->GetList("desert") == nullptr);
    EXPECT(gen->ValidList("forest"));
    EXPECT(gen->ValidList("Forest"));                   // lowercased before the lookup
    EXPECT(!gen->ValidList("nope"));

    // The block map is keyed the same way; its values are not dereferenced by ValidBlock.
    gen->m_blocks.insert(std::make_pair("wall_a", static_cast<rnd::Block*>(nullptr)));
    EXPECT(gen->ValidBlock("WALL_A"));
    EXPECT(!gen->ValidBlock("wall_b"));
}

void TestUnloadTilesWithoutRoot() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    gen->m_rootTile = nullptr;
    gen->UnloadTiles();                                 // null root: nothing to unspawn
    EXPECT(gen->m_rootTile == nullptr);
}

void TestUnloadRoomPools() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    rnd::RoomPool* p1 = new (std::malloc(sizeof(rnd::RoomPool))) rnd::RoomPool();
    rnd::RoomPool* p2 = new (std::malloc(sizeof(rnd::RoomPool))) rnd::RoomPool();
    gen->m_roomPools.push_back(p1);
    gen->m_roomPools.push_back(nullptr);                // null entries are skipped, as in the original
    gen->m_roomPools.push_back(p2);
    gen->UnloadRoomPools();
    EXPECT(gen->m_roomPools.empty());
}

void TestUnloadRulesEmptiesListMap() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    void* mem = std::malloc(sizeof(rnd::ListRule));
    rnd::ListRule* rule = new (mem) rnd::ListRule();
    rule->m_name = "cave";
    gen->m_listRules.insert(std::make_pair(rule->m_name.c_str(), rule));
    gen->m_xmlBuffer = nullptr;                         // free(NULL) in UnloadRules
    gen->UnloadRules();                                 // the TiXmlDocument seam is not exercised (null document)
    EXPECT(gen->m_listRules.empty());
    EXPECT(gen->m_xmlBuffer == nullptr);
}

void TestLoadCountsReturnOne() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    // The named-child walk is a seam; with no document the loaders report success without loading anything.
    EXPECT(gen->LoadRoomPools(nullptr) == 1);
    EXPECT(gen->LoadListRules(nullptr) == 1);
    EXPECT(gen->m_roomPools.empty());
    EXPECT(gen->m_listRules.empty());
}

void TestGenerateReportsFailure() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    EXPECT(gen->Generate(42) == 0);                     // the stubbed RootRule::Impl places nothing
    EXPECT(gen->m_seed == 42);
    EXPECT(gen->m_rngState == 42u);
    EXPECT(gen->m_rootTile == nullptr);
}

void TestConstructorDefaults() {
    rnd::RandomGenerator* gen = MakeGenerator(0);
    EXPECT(gen->m_rootTile == nullptr);
    EXPECT(gen->m_xmlDocument == nullptr);
    EXPECT(gen->m_xmlBuffer == nullptr);
    EXPECT(gen->m_sceneDir == "data/scene/");
    EXPECT(gen->m_levelConfig != nullptr);
    EXPECT(gen->m_roomPools.empty());
    EXPECT(gen->m_listRules.empty());
    EXPECT(gen->m_blocks.empty());
}

}  // namespace

int main() {
    TestNextInt();
    TestGetInt();
    TestCallOperator();
    TestHash();
    TestListLookup();
    TestUnloadTilesWithoutRoot();
    TestUnloadRoomPools();
    TestUnloadRulesEmptiesListMap();
    TestLoadCountsReturnOne();
    TestGenerateReportsFailure();
    TestConstructorDefaults();
    if (g_failures != 0) {
        std::fprintf(stderr, "%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("g1-game-app-rnd-8b06ae-0 tests passed\n");
    return 0;
}
