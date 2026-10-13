// Batch g1-game-app-rnd-8b06ae-6 (game-app, owner rnd, part 7 of 8): the rule walk and filter functions (Rule::Impl,
// Path::Impl, ForceBlock::Impl), the root placement (RootRule::Impl::Generate), the ListRule and BlockSearch helpers,
// RoomPool lookup and size computation, and the rule XML loaders. Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so.
//
// Status summary (the ledger rows carry the detail):
//   reconstructed: Rule::GetBlock, ListElem copy ctor, Rule::Impl::FilterExits, RootRule::Impl::Generate, ListRule::Replace,
//                  Path::Impl::AddExit, Path::Impl::FilterExits, BlockSearch copy ctor, ListRule::Find, RoomPool::Find(string),
//                  RoomPool::ComputeSizeOfRules, ForceBlock::LoadFromXml (the discarded "connectFrom" read is omitted),
//                  RootRule::LoadFromXml (thunk to Rule::LoadFromXml).
//   partial:       ListRule::LoadFromXml, Rule::LoadFromXml, Path::LoadFromXml (TinyXML element access not declared),
//                  ForceBlock::Impl::FilterExits (rnd::Direction not declared), Rule::Impl::Step and Path::Impl::OneStep
//                  (rnd::gDistributions and the Direction tables), Rule::Impl::OneStep (side link table not declared).
//
// What blocks the partial rows (not declared in the tree yet):
//   - rnd::Direction: the 16-byte records behind Exit::m_direction (the first int is the id), sDirections (north, east, south,
//     west), DIRECTION_OPPOSITES. Block.hpp and Exit.hpp only forward-declare it, as in the other rnd batches.
//   - rnd::gDistributions: the 6-byte distribution rows indexed by 726 * child count + 4356 * candidate count.
//   - Block::Side link table: the link count is unk_28 (+0x28) and the Exit* links start at +0x2c. Block.hpp keeps that
//     range as pad_2c[0x100], so the links cannot be read by name.
//   - TinyXML element access (ToElement, Attribute, QueryIntAttribute, FirstChild and NextSibling with a name). The game
//     subset in tinyxml-and-c-libs/tixml_dh.hpp does not declare them.
//   - ForceBlock +0x8c: read by ForceBlock::Impl::FilterExits, written by no recovered code (declared as unk_8c in this batch).
//
// Signature note: the original LoadFromXml family returns an int (1 on success, 0 on failure; Rule::LoadFromXml ANDs the
// results of its children). The headers declare void, as the other rnd rows do, so the flag is not carried. The batch -5
// test defines these functions with a void signature, so the headers stay as they are.
//
// Other deviations from the original, both at points the original does not define:
//   - Path::Impl::FilterExits with a fixed list index past the end (or below -1) reads outside the element array in the
//     original; here such an index adds nothing.
//   - RoomPool::ComputeSizeOfRules with no element left to grow divides by zero in the original; here it returns 0. The
//     sizes FillSizes produces always leave room for the requested total, so this is not reached.
//
// Shared helpers: lrand48 is dh2::rand48_next() (the POSIX generator, which is the libc import on the other hosts). The
// original's std::random_shuffle over a RandomGenerator is written out as ShuffleWithGenerator (the same loop, without the
// deprecated call), using RandomGenerator::operator().
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include "core-platform/lc_api.hpp"
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/BlockSearch.hpp"
#include "dh2/rnd/Exit.hpp"
#include "dh2/rnd/ForceBlock.hpp"
#include "dh2/rnd/ListElem.hpp"
#include "dh2/rnd/ListRule.hpp"
#include "dh2/rnd/Path.hpp"
#include "dh2/rnd/RPElem.hpp"
#include "dh2/rnd/RandomGenerator.hpp"
#include "dh2/rnd/RoomPool.hpp"
#include "dh2/rnd/RootRule.hpp"
#include "dh2/rnd/Rule.hpp"
#include "dh2/rnd/Tile.hpp"
#include "vendor-lua/dh2_rand48.hpp"

// Engine assert level. core-platform/memory.hpp declares it too; it is not included here, as in the other game-app sources.
extern int gAssertLevel;

namespace rnd {

namespace {

// Source path printed by the original's ASSERT lines in rndRule.cpp.
constexpr const char kRndRuleCppPath[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\random\\rndRule.cpp";

// A Block side record has the rnd::Exit fields in its first 0x2c bytes, with the same members in the same order, and the
// original passes a side wherever an Exit* is expected. Block::Side is not derived from Exit in the tree, so the record is
// viewed as an Exit through its address.
const Exit* SideAsExit(const Block::Side& side) {
    return reinterpret_cast<const Exit*>(&side);
}

// The name test of Rule::Impl::FilterExits: the block name is longer than five characters and contains "_start". The original
// scans for an underscore followed by the five letters of "start".
bool NameHasStartMarker(const std::string& name) {
    return name.size() > 5 && name.find("_start") != std::string::npos;
}

// PARTIAL. The direction branch of ForceBlock::Impl::FilterExits compares the force-block's direction word (+0x88) with the
// first word of the exit's Direction, and its +0x8c word with the exit's flags. rnd::Direction is not declared in the tree,
// so the id cannot be read: this stub accepts no exit through that branch.
bool ForceDirectionBranch(const Exit& exit, int forcedDirection, int forcedFlags) {
    (void)exit;
    (void)forcedDirection;
    (void)forcedFlags;
    return false;
}

// The acceptance test of ForceBlock::Impl::FilterExits: any direction (+0x88 == 4), or the direction branch.
bool AcceptsForcedExit(const ForceBlock& forced, const Exit& exit) {
    return forced.unk_88 == 4 || ForceDirectionBranch(exit, forced.unk_88, forced.unk_8c);
}

// The original's std::random_shuffle(first, last, generator): element i (counting from 1) swaps with the element at
// generator(i + 1). The standard call is deprecated, so the same loop is written out here.
template <typename Iterator>
void ShuffleWithGenerator(Iterator first, Iterator last, RandomGenerator& generator) {
    if (first == last) {
        return;
    }
    for (Iterator i = first + 1; i != last; ++i) {
        std::iter_swap(i, first + static_cast<std::ptrdiff_t>(generator(static_cast<unsigned int>(i - first) + 1)));
    }
}

}  // namespace

// 0x48e274 rnd::Rule::GetBlock(char const*) const
// Looks the name up in the generator behind the root rule (RootRule +0x8c, reached through the rule's root at +0x04).
Block* Rule::GetBlock(const char* name) const {
    return m_root->m_generator->GetBlock(name);
}

// 0x48e280 rnd::ListElem::ListElem(rnd::ListElem const&)
// Copies the owner (+0x00), the name (+0x04), the gameplay (+0x1c), the visual (+0x34) and the chance count (+0x4c).
ListElem::ListElem(const ListElem& other)
    : m_owner(other.m_owner),
      m_name(other.m_name),
      m_gameplay(other.m_gameplay),
      m_visual(other.m_visual),
      m_chances(other.m_chances) {}

// 0x48e690 rnd::Rule::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>>&, rnd::ListRule*&)
// Removes the first candidate whose block name contains "_start", then shuffles the candidates with the generator behind the
// rule. The list argument is not read by the original.
void Rule::Impl::FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates, ListRule*& list) {
    (void)list;
    for (auto it = candidates.begin(); it != candidates.end(); ++it) {
        if (NameHasStartMarker(it->first->m_block->m_name)) {
            candidates.erase(it);
            break;
        }
    }
    RandomGenerator& app = *m_rule->GetApp();
    ShuffleWithGenerator(candidates.begin(), candidates.end(), app);
}

// 0x48e8ac rnd::RootRule::Impl::Generate(Array2d<rnd::Tile*>&)
// Places the root tile of the level and returns it, or null. The root rule's list (+0x68) and its index (+0x6c) pick the
// candidates:
//   - no list: the block names of the root rule (m_names, copied and shuffled) are tried in order with a default element;
//   - a list and an index that is -1 or past the end: a copy of the list is shuffled and tried in order;
//   - a list and a valid index: that element alone is tried.
// A candidate is tried when its block is found by name, through PlaceRootTile. The grid is not used here (the original's
// argument is not read; RandomGenerator::Generate keeps the returned tile).
Tile* RootRule::Impl::Generate(Array2d<Tile*>& grid) {
    (void)grid;
    const RootRule& root = *m_rootRule;
    ListRule* list = root.m_list;
    if (list != nullptr) {
        const std::uint32_t index = static_cast<std::uint32_t>(root.m_listIndex);
        if (root.m_listIndex == -1 || index >= list->m_elems.size()) {
            std::vector<ListElem> elems(list->m_elems);
            ShuffleWithGenerator(elems.begin(), elems.end(), *m_rule->GetApp());
            for (const ListElem& candidate : elems) {
                Block* block = m_rule->GetBlock(candidate.m_name.c_str());
                if (block != nullptr) {
                    if (Tile* placed = PlaceRootTile(block, candidate)) {
                        return placed;
                    }
                }
            }
            return nullptr;
        }
        const ListElem element = list->m_elems[index];
        Block* block = m_rule->GetBlock(element.m_name.c_str());
        return block != nullptr ? PlaceRootTile(block, element) : nullptr;
    }

    std::vector<const char*> names(m_names);
    ShuffleWithGenerator(names.begin(), names.end(), *m_rule->GetApp());
    for (const char* name : names) {
        Block* block = m_rule->GetBlock(name);
        if (block != nullptr) {
            if (Tile* placed = PlaceRootTile(block, ListElem())) {
                return placed;
            }
        }
    }
    return nullptr;
}

// 0x48ec74 rnd::ListRule::Replace(rnd::ListElem&)
// Only a list whose replacement flag is clear takes elements this way: the element is appended. The owner of the element must
// be this list; the original asserts otherwise (level 2 stores through the null flag, which is abort here; level 1 prints the
// ASSERT line for rndRule.cpp line 127) and appends the element anyway.
void ListRule::Replace(ListElem& elem) {
    if (!m_replacement) {
        if (elem.m_owner != this) {
            if (gAssertLevel == 2) {
                std::abort();
            } else if (gAssertLevel == 1) {
                std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "elem.mOwner == this", kRndRuleCppPath, 127);
            }
        }
        m_elems.push_back(elem);
    }
}

// 0x48ed2c rnd::ListRule::LoadFromXml(TiXmlNode *)
// PARTIAL. Not rebuilt. The original reads the node's element, its "name" attribute (lowercased into m_name) and its
// "replacement" attribute (absent or "true" sets m_replacement), then loads one ListElem for each "elem" child (ListElem::LoadFromXml,
// partial) and checks its block with RandomGenerator::ValidBlock before appending it. The TinyXML element access (ToElement,
// Attribute, FirstChild and NextSibling with a name) is not declared in the tree, so nothing is read here. The original's
// return value is always null (the last sibling); the header keeps void.
void ListRule::LoadFromXml(TiXmlNode* node) {
    (void)node;
}

// 0x48f05c rnd::Path::Impl::AddExit(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>>&, rnd::Exit const*, int)
// Pushes the source exit, paired with a copy of the index-th element of the rule's list, when the source block is 1 x 1 and
// suits the path: a dead end needs a block with one side, and a block with two sides needs either the path's unk_8d flag clear or
// a straight block. Nothing is pushed otherwise.
void Path::Impl::AddExit(std::vector<std::pair<const Exit*, ListElem>>& candidates, const Exit* source, int index) {
    const Exit* chosen = nullptr;
    ListElem element;
    const Block* block = source->m_block;
    if (block->m_blockWidth == 1 && block->m_blockHeight == 1) {
        const int sideCount = block->m_sideCount;
        if (IsDeadEnd()) {
            if (sideCount == 1) {
                element = m_rule->m_list->m_elems[static_cast<std::size_t>(index)];
                chosen = source;
            }
        } else if (sideCount == 2) {
            // The original tests unk_8d first and calls IsStraight only when it is set.
            if (!m_path->unk_8d || block->IsStraight()) {
                element = m_rule->m_list->m_elems[static_cast<std::size_t>(index)];
                chosen = source;
            }
        }
    }
    if (chosen != nullptr) {
        candidates.emplace_back(chosen, element);
    }
}

// 0x48f25c rnd::Path::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>>&, rnd::ListRule*&)
// The candidates are rebuilt through AddExit: each candidate exit whose block name equals the name of the rule's list element
// (every element when the list index is -1, else only the element at the index) is passed on. With no list at all, no exit
// survives. The filtered set replaces the candidates, and the base FilterExits then removes a "_start" exit and shuffles.
void Path::Impl::FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates, ListRule*& list) {
    std::vector<std::pair<const Exit*, ListElem>> filtered;
    ListRule* ruleList = m_rule->m_list;
    if (ruleList != nullptr) {
        for (const auto& candidate : candidates) {
            const Exit* exit = candidate.first;
            if (exit == nullptr || exit->m_block == nullptr) {
                continue;
            }
            const std::string& blockName = exit->m_block->m_name;
            const int index = m_rule->m_listIndex;
            if (index == -1) {
                for (std::size_t i = 0; i < ruleList->m_elems.size(); ++i) {
                    if (blockName == ruleList->m_elems[i].m_name) {
                        AddExit(filtered, exit, static_cast<int>(i));
                    }
                }
            } else if (static_cast<std::size_t>(index) < ruleList->m_elems.size() &&
                       blockName == ruleList->m_elems[static_cast<std::size_t>(index)].m_name) {
                AddExit(filtered, exit, index);
            }
        }
    }
    candidates.swap(filtered);
    Rule::Impl::FilterExits(candidates, list);
}

// 0x48f47c rnd::ForceBlock::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*, rnd::ListElem>>&, rnd::ListRule*&)
// The list output is set to the force-block's list (+0x68). Candidates whose block name matches are kept, paired with the list
// element they match (with no list: with a default element, for each name in m_names, compared case-insensitively). The
// acceptance test is AcceptsForcedExit. The filtered set then replaces the candidates and the base FilterExits runs.
// PARTIAL: the direction branch of the acceptance test needs rnd::Direction (see ForceDirectionBranch). The "any direction"
// branch (+0x88 == 4) is rebuilt in full.
void ForceBlock::Impl::FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates, ListRule*& list) {
    const ForceBlock& forced = *m_forceBlock;
    list = forced.m_list;
    std::vector<std::pair<const Exit*, ListElem>> filtered;
    if (forced.m_list != nullptr) {
        const std::vector<ListElem>& elems = forced.m_list->m_elems;
        const std::uint32_t index = static_cast<std::uint32_t>(forced.m_listIndex);
        const bool hasIndex = index != 0xFFFFFFFFu && index < elems.size();
        for (const auto& candidate : candidates) {
            const Exit* exit = candidate.first;
            if (exit == nullptr || exit->m_block == nullptr) {
                continue;
            }
            const std::string& blockName = exit->m_block->m_name;
            if (hasIndex) {
                const ListElem& element = elems[index];
                if (blockName == element.m_name && AcceptsForcedExit(forced, *exit)) {
                    filtered.emplace_back(exit, element);
                }
            } else {
                for (const ListElem& element : elems) {
                    if (blockName == element.m_name && AcceptsForcedExit(forced, *exit)) {
                        filtered.emplace_back(exit, element);
                    }
                }
            }
        }
    } else {
        for (const auto& candidate : candidates) {
            const Exit* exit = candidate.first;
            if (exit == nullptr || exit->m_block == nullptr) {
                continue;
            }
            for (const char* name : m_names) {
                if (LC_API_STRCASECMP(exit->m_block->m_name.c_str(), name) == 0 && AcceptsForcedExit(forced, *exit)) {
                    filtered.emplace_back(exit, ListElem());
                }
            }
        }
    }
    candidates.swap(filtered);
    Rule::Impl::FilterExits(candidates, list);
}

// 0x48f954 rnd::Rule::Impl::Step(rnd::Tile*, rnd::Exit const*)
// The candidates are the sides of the tile's block other than the exit the step came from, paired with default elements, and
// they are shuffled with the rule's generator. A rule without children, or no candidates, succeeds at once (true).
// PARTIAL: the choice among the children (the 6-byte rows of rnd::gDistributions, indexed by 726 * child count + 4356 * candidate
// count, then the child NewImpl and OneStep calls with the direction test) is not rebuilt, because the table and rnd::Direction
// are not declared. This stub returns false after the shuffle.
bool Rule::Impl::Step(Tile* tile, const Exit* from) {
    const Block& block = *tile->m_block;
    std::vector<std::pair<const Exit*, ListElem>> candidates;
    for (int i = 0; i < block.m_sideCount; ++i) {
        const Exit* side = SideAsExit(block.m_sides[i]);
        if (side != from) {
            candidates.emplace_back(side, ListElem());
        }
    }
    RandomGenerator& app = *m_rule->GetApp();
    ShuffleWithGenerator(candidates.begin(), candidates.end(), app);
    if (m_rule->m_childCount == 0 || candidates.empty()) {
        return true;
    }
    return false;
}

// 0x48fd64 rnd::Path::Impl::OneStep(rnd::Tile*, rnd::Exit const*, rnd::Exit const*)
// The depth limit branch is rebuilt: once m_depth reaches m_limit, the step goes on from the block's other side (the side that
// is not "from" when the block has two sides, nothing otherwise) through Rule::Impl::Step.
// PARTIAL: the rest of the step needs rnd::Direction (sDirections, the first word of each direction, DIRECTION_OPPOSITES, and the
// packed direction word stored in unk_48) and the side link table (count at +0x28, pointers at +0x2c, not declared). This stub
// returns 0 for that branch.
int Path::Impl::OneStep(Tile* tile, const Exit* from, const Exit* to) {
    if (m_depth >= m_limit) {
        const Block& block = *tile->m_block;
        const Exit* other = nullptr;
        if (block.m_sideCount == 2) {
            other = (from == SideAsExit(block.m_sides[0])) ? SideAsExit(block.m_sides[1]) : SideAsExit(block.m_sides[0]);
        }
        return Rule::Impl::Step(tile, other) ? 1 : 0;
    }
    (void)to;
    return 0;
}

// 0x490304 rnd::Rule::Impl::OneStep(rnd::Tile*, rnd::Exit const*, rnd::Exit const*)
// PARTIAL. Not rebuilt: the candidates are the links of the "from" side record (count at unk_28, pointers at +0x2c, not
// declared). The original builds the candidate list, calls the virtual FilterExits, tries each candidate with Tile::TrySpawn
// (the link as the offset, "from" as the parent exit), keeps the first tile that the virtual Step accepts, and unspawns the
// others. This stub spawns nothing and returns 0.
int Rule::Impl::OneStep(Tile* tile, const Exit* from, const Exit* to) {
    (void)tile;
    (void)from;
    (void)to;
    return 0;
}

// 0x490520 rnd::BlockSearch::BlockSearch(rnd::BlockSearch const&)
// Copies the name (+0x00), the gameplay (+0x18) and the visual (+0x30).
BlockSearch::BlockSearch(const BlockSearch& other)
    : m_name(other.m_name), m_gameplay(other.m_gameplay), m_visual(other.m_visual) {}

// 0x4905d0 rnd::ListRule::Find(char const*, char const*, char const*, bool)
// Returns a copy of the first element whose name, gameplay and visual match the arguments, or a default element when none does.
// With allowReplacement set and a list whose replacement flag is clear, the found element is also removed from the list.
ListElem ListRule::Find(const char* name, const char* gameplay, const char* visual, bool allowReplacement) {
    ListElem found;
    const BlockSearch search(name, gameplay, visual);
    const auto it = std::find_if(m_elems.begin(), m_elems.end(), search);
    if (it != m_elems.end()) {
        found = *it;
        if (allowReplacement && !m_replacement) {
            m_elems.erase(it);
        }
    }
    return found;
}

// 0x4906bc rnd::RoomPool::Find(std::string)
// Hashes the key with the pool's generator and looks the hash up with RoomPool::Find(unsigned long) (0x48bc20).
RPElem* RoomPool::Find(std::string key) {
    const unsigned int hash = m_generator->Hash(reinterpret_cast<unsigned char*>(key.data()));
    return Find(static_cast<unsigned long>(hash));
}

// 0x490904 rnd::RoomPool::ComputeSizeOfRules(void)
// Chooses the size of the pool's rules. Each element has a size range: unk_0c is the lowest and unk_10 the highest, and unk_14 is
// the current size (FillSizes sets it to the lowest). The pool's size m_size must lie between the sums of the lowest and of the
// highest sizes, or the call returns 0. The remaining amount is spread one unit at a time over the elements that can still
// grow, each chosen at random (dh2::rand48_next, the lrand48 of the original); an element that reaches its highest size leaves the
// set. Returns 1 when the sizes are set.
int RoomPool::ComputeSizeOfRules() {
    int minSum = 0;
    int maxSum = 0;
    for (const RPElem& elem : m_elems) {
        maxSum += elem.unk_10;
        minSum += elem.unk_0c;
    }
    if (m_size < minSum || m_size > maxSum) {
        return 0;
    }
    int remaining = m_size - minSum;
    if (remaining <= 0) {
        return 1;
    }

    // Indices of the elements whose current size is below their highest size.
    std::vector<std::size_t> open;
    for (std::size_t i = 0; i < m_elems.size(); ++i) {
        if (m_elems[i].unk_14 < m_elems[i].unk_10) {
            open.push_back(i);
        }
    }
    while (remaining > 0) {
        for (;;) {
            if (open.empty()) {
                // The original divides by zero here. The sizes FillSizes sets always leave room for the total, so this is not reached.
                return 0;
            }
            const std::size_t pick = static_cast<std::size_t>(dh2::rand48_next()) % open.size();
            RPElem& elem = m_elems[open[pick]];
            ++elem.unk_14;
            if (elem.unk_14 == elem.unk_10) {
                open.erase(open.begin() + static_cast<std::ptrdiff_t>(pick));
                --remaining;
                break;
            }
            if (--remaining == 0) {
                return 1;
            }
        }
    }
    return 1;
}

// 0x490bdc rnd::Rule::LoadFromXml(TiXmlNode *)
// PARTIAL. Not rebuilt past the null check. The original reads the node's "name" attribute (a "#list" or "#list[N]" name selects a
// list through RandomGenerator::GetList, checked with ValidList; any other text is a comma separated list of block names, each
// checked with ValidBlock and kept in m_blockNames), the "exit" attribute (m_exit), the "id" attribute (hashed into m_id), and the
// child elements, each built with NewRule and loaded with its LoadFromXml (the results are ANDed). Then it sizes the room pools
// the generator holds for this rule (RPElem::FillSizes with unk_80 and unk_84). All of that reads TinyXML elements, whose access is
// not declared in the tree, so the body stops here. The original returns 1 on success and 0 when a child fails; the header keeps
// void.
void Rule::LoadFromXml(TiXmlNode* node) {
    if (node == nullptr) {
        return;
    }
}

// 0x4912d0 rnd::ForceBlock::LoadFromXml(TiXmlNode *)
// The original reads the "connectFrom" attribute of the node and drops the result, which has no effect, then runs Rule::LoadFromXml.
// The read is omitted here.
void ForceBlock::LoadFromXml(TiXmlNode* node) {
    Rule::LoadFromXml(node);
}

// 0x49130c rnd::Path::LoadFromXml(TiXmlNode *)
// PARTIAL. The original reads "dontGoBack" (QueryIntAttribute; stored as the flag unk_8c, set when non-zero) and "length" (one
// number sets unk_80 and unk_84, "a,b" sets unk_80 to a and unk_84 to b) before Rule::LoadFromXml runs. TinyXML attribute access
// is not declared in the tree, so the attributes are not read here. A null node returns at once, as in the original.
void Path::LoadFromXml(TiXmlNode* node) {
    if (node == nullptr) {
        return;
    }
    Rule::LoadFromXml(node);
}

// 0x4913cc rnd::RootRule::LoadFromXml(TiXmlNode *)
// Thunk: forwards to rnd::Rule::LoadFromXml (0x490bdc), which is partial. This forwarder is exact.
void RootRule::LoadFromXml(TiXmlNode* node) {
    Rule::LoadFromXml(node);
}

}  // namespace rnd
