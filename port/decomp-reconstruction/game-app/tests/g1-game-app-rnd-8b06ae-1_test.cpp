// Test for the rnd batch g1-game-app-rnd-8b06ae-1. It links with the batch's own source and defines the few symbols that
// source refers to but this batch does not rebuild (the block constructor and the MgxBlock map hooks, which only exist
// so a test Block can be made and the MgxBlock vtable can link). Returns non-zero on failure.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/Point3D.hpp"
#include "dh2/lstr.hpp"
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/Exit.hpp"
#include "dh2/rnd/MgxBlock.hpp"

// Declared in the batch source (not in a header yet). TryIsZero is declared in dh2/rnd/Exit.hpp.
namespace rnd {
void SplitFileListText(std::string text, std::vector<std::string>& files);
}  // namespace rnd

// Symbols outside this batch that the batch source's vtables and constructors refer to.
bool lstr::operator()(const char* lhs, const char* rhs) const { return std::strcmp(lhs, rhs) < 0; }

namespace rnd {
Block::Block() {}
MgxBlock* MgxBlock::FromFilename(const char*, const char*, const char*) { return nullptr; }
bool MgxBlock::PlaceInMap(Array2d<Tile*>&, Tile*, int, int) { return true; }
bool MgxBlock::RemoveFromMap(Array2d<Tile*>&, Tile*, int, int) { return true; }
bool MgxBlock::FitsInMap(Array2d<Tile*>&, int, int) { return true; }
}  // namespace rnd

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                                       \
    do {                                                                                   \
        if (!(cond)) {                                                                     \
            std::fprintf(stderr, "%s:%d: EXPECT failed: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                                  \
        }                                                                                  \
    } while (0)

// A concrete block for the tests. Block itself is abstract.
class TestBlock : public rnd::Block {
public:
    bool PlaceInMap(Array2d<rnd::Tile*>& map, rnd::Tile* tile, int x, int y) override {
        (void)map;
        (void)tile;
        (void)x;
        (void)y;
        return true;
    }
    bool RemoveFromMap(Array2d<rnd::Tile*>& map, rnd::Tile* tile, int x, int y) override {
        (void)map;
        (void)tile;
        (void)x;
        (void)y;
        return true;
    }
    bool FitsInMap(Array2d<rnd::Tile*>& map, int x, int y) override {
        (void)map;
        (void)x;
        (void)y;
        return true;
    }
};

// The Exit constructor only keeps the address of its Direction, so a reference to an object of that type is enough here.
const rnd::Direction& TestDirection() {
    static const int anchor = 0;
    return *static_cast<const rnd::Direction*>(static_cast<const void*>(&anchor));
}

void TestTryIsZero() {
    EXPECT(rnd::TryIsZero("0"));
    EXPECT(rnd::TryIsZero(""));
    EXPECT(!rnd::TryIsZero("00"));
    EXPECT(!rnd::TryIsZero("a"));
    EXPECT(!rnd::TryIsZero("0 "));
}

void TestSplitFileListText() {
    std::vector<std::string> files;
    rnd::SplitFileListText("alpha.mgx\r\nbeta.mgx\r\n", files);
    EXPECT(files.size() == 2);
    if (files.size() == 2) {
        EXPECT(files[0] == "alpha.mgx");
        EXPECT(files[1] == "beta.mgx");
    }

    // Without the '\r' the original still drops the character before the newline.
    files.clear();
    rnd::SplitFileListText("one\n", files);
    EXPECT(files.size() == 1);
    if (files.size() == 1) {
        EXPECT(files[0] == "on");
    }

    // Text after the last newline is not a line.
    files.clear();
    rnd::SplitFileListText("a\r\nb", files);
    EXPECT(files.size() == 1);
    if (files.size() == 1) {
        EXPECT(files[0] == "a");
    }

    files.clear();
    rnd::SplitFileListText("", files);
    EXPECT(files.empty());
}

void TestGetBlockUnitPosition() {
    TestBlock block;
    block.m_blockWidth = 2;     // +0x54
    block.m_blockHeight = 3;    // +0x58
    block.m_unitWidth = 10.0f;  // unit width, +0x4c
    block.m_unitHeight = 10.0f; // unit height, +0x50

    std::vector<std::string> names;
    rnd::Exit exit(TestDirection(), &block, 0, 0, 0.0f, 0, names);

    struct Case {
        float x;
        float y;
        int expectX;
        int expectY;
    };
    const Case cases[] = {
        {0.0f, 0.0f, 0, 1},
        {5.0f, 12.0f, 1, 0},
        {-100.0f, 100.0f, 0, 0},
        {25.0f, -20.0f, 3, 3},
    };
    for (const Case& c : cases) {
        Point3D<float> position;
        position.X = c.x;
        position.Y = c.y;
        exit.GetBlockUnitPosition(position, &block);
        EXPECT(exit.m_x == c.expectX);
        EXPECT(exit.m_y == c.expectY);
    }
}

void TestExitConstructor() {
    TestBlock block;
    block.m_name = "blk";
    std::vector<std::string> names = {"north", "south"};
    rnd::Exit exit(TestDirection(), &block, 3, 4, 1.5f, 7, names);

    EXPECT(exit.m_block == &block);
    EXPECT(exit.m_x == 3);
    EXPECT(exit.m_y == 4);
    EXPECT(exit.m_weight == 1.5f);
    EXPECT(exit.m_direction == &TestDirection());
    EXPECT(exit.m_flags == 7);
    EXPECT(exit.unk_28 == 0);
    EXPECT(exit.m_names == names);
    EXPECT(exit.m_blockName != nullptr && std::strcmp(exit.m_blockName, "blk") == 0);
}

}  // namespace

int main() {
    TestTryIsZero();
    TestSplitFileListText();
    TestGetBlockUnitPosition();
    TestExitConstructor();
    if (g_failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", g_failures);
        return 1;
    }
    std::printf("g1-game-app-rnd-8b06ae-1 tests passed\n");
    return 0;
}
