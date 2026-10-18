// dh2_itemdump: the item data of the DH2 save editor (save_editor_dh2), produced by the game's own code. Not part of the game.
//
//   dh2_itemdump --data <game files dir> --dump <out.json> [--out <log dir>]
//
// Boots the reconstructed game headless (the host runner's boot: appInit, GSInit loads every pydata table and the text
// packs), runs frames until Arrays::ItemTable and the StringManager are ready, then writes one JSON file with:
//  * the raw rows the editor's rules need (Arrays::ItemTable / ItemPowerTable / ItemPowerList / ItemBonusAttrMonopoly /
//    ItemList / LootTable / NumProbArray / MerchantTable / FontPalette, read by the original pydata readers);
//  * for every language pack, the texts the game itself shows: ItemInstance::_UpdateName/_UpdateStats/_UpdateReqs (the
//    name, stats line and requirements line of the item-detail screen, NativeGetItemInfo) for every item row, and
//    ItemInstance::AddPower's description (PowerInfo::m_desc, "ItemPowers%dDesc") for every power row;
//  * the PyData constants groups the editor labels with (ItemType, EquipmentSlots, ...), ItemPowerColor and the
//    ItemInstance::GetColor colour of 0..4 powers.
// Nothing here changes game state that is saved: the tool never calls SG_Save and exits after the dump.
#include "host_runner.hpp"

#include <windows.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/ItemInstance.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/global/charai_ai.hpp"     // ApplicationGetPyDataConstants
#include "dh2/global/item_instance.hpp" // ApplicationGetStringManager
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_arrays27.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "pydata-accessors/w7_pydata12.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

namespace {

template <class Row>
const Row* TableRow(const std::uint8_t* members, int index) {
    return reinterpret_cast<const Row*>(members) + index;
}

// JSON string: UTF-8 bytes pass through, quote/backslash/control bytes are escaped.
std::string J(const char* s) {
    std::string o = "\"";
    if (s != nullptr) {
        for (const unsigned char* p = reinterpret_cast<const unsigned char*>(s); *p != 0; ++p) {
            const unsigned char c = *p;
            if (c == '"') o += "\\\"";
            else if (c == '\\') o += "\\\\";
            else if (c == '\n') o += "\\n";
            else if (c == '\r') o += "\\r";
            else if (c == '\t') o += "\\t";
            else if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
            else o += static_cast<char>(c);
        }
    }
    return o + "\"";
}
std::string J(const std::string& s) { return J(s.c_str()); }
std::string N(long long v) { return std::to_string(v); }

std::string NameOf(const pydata::NameTable& t, int i) {
    return (i >= 0 && i < t.size && t.names != nullptr) ? J(t.names[i]) : std::string("null");
}

// Every constant of a PyDataConstants class: getConstantName over a value range (the map itself is private).
std::string ConstGroup(const char* cls, int lo, int hi) {
    PyDataConstants* c = ApplicationGetPyDataConstants();
    std::string o = "{";
    bool first = true;
    for (int v = lo; v <= hi; ++v) {
        const char* n = c->getConstantName(cls, v);
        if (n == nullptr || *n == 0) continue;
        if (!first) o += ",";
        first = false;
        o += J(n) + ":" + N(v);
    }
    return o + "}";
}

std::string LootEntryJson(const Structs::LootEntry& e) {
    return "{\"itemList\":" + N(e.ItemListID) + ",\"powerList\":" + N(e.ItemPowerList) + ",\"numPowerProbs\":" + N(e.NumPowerProbs) +
           ",\"pct\":" + N(e.Pct) + ",\"prob\":" + N(e.Prob) + ",\"mProb\":" + N(e.MProb) + ",\"rProb\":" + N(e.RProb) +
           ",\"wProb\":" + N(e.WProb) + "}";
}

bool Ready() {
    StringManager* s = ApplicationGetStringManager();
    return Arrays::ItemTable::size > 0 && Arrays::ItemPowerTable::size > 0 && s != nullptr && s->m_currentPack >= 0 &&
           ApplicationGetPyDataConstants() != nullptr;
}

void Dump(std::FILE* f) {
    StringManager* strings = ApplicationGetStringManager();
    const int itemCount = static_cast<int>(Arrays::ItemTable::size);
    const int powerCount = static_cast<int>(Arrays::ItemPowerTable::size);
    std::fprintf(f, "{\"source\":\"dh2_itemdump (save_editor_dh2 itemdump/tool_itemdump.cpp): original pydata readers + ItemInstance text code\",\n");

    // ---- constants ----
    std::fprintf(f, "\"constants\":{");
    const char* groups[] = {"ItemType", "EquipmentSlots", "EquipmentSlottingTypes", "ClassRequirements", "ItemBonusAttr", "PickUpType",
                            "DamageTypes", "ItemBaseProp", "ItemDistType", "RollType", "DropRate", "AttrBonusEntryFlags", "ItemPowerColor"};
    for (std::size_t g = 0; g < sizeof groups / sizeof groups[0]; ++g) {
        std::fprintf(f, "%s%s:%s", g ? ",\n" : "\n", J(groups[g]).c_str(), ConstGroup(groups[g], -16, 300).c_str());
    }
    PyDataConstants* c = ApplicationGetPyDataConstants();
    std::fprintf(f, ",\n\"InventoryDesign\":{\"MaxItemInstanceQuantity\":%d,\"MinPlayerPotionItemInstances\":%d,\"InfiniteQuantity\":%d,\"TransmuteFactor\":%d}",
                 c->getConstant("InventoryDesign", "MaxItemInstanceQuantity"), c->getConstant("InventoryDesign", "MinPlayerPotionItemInstances"),
                 c->getConstant("InventoryDesign", "InfiniteQuantity"), c->getConstant("InventoryDesign", "TransmuteFactor"));
    std::fprintf(f, ",\n\"CharacterDesign\":{\"TransmuteMultiplier\":%d}", c->getConstant("CharacterDesign", "TransmuteMultiplier"));
    std::fprintf(f, "},\n");

    // ---- ItemInstance::GetFontDef / GetColor for 0..5 powers ----
    std::fprintf(f, "\"powerColors\":[");
    for (int n = 0; n <= 5; ++n) {
        const int def = ItemInstance::GetFontDef(n);
        const Structs::FontColorDef* row = TableRow<Structs::FontColorDef>(Arrays::FontPalette::members, def);
        std::fprintf(f, "%s{\"powers\":%d,\"fontDef\":%d,\"fontDefName\":%s,\"color\":%u,\"glow\":%u}", n ? "," : "", n, def,
                     NameOf(Arrays::FontPalette::m_memberNames, def).c_str(), static_cast<unsigned>(ItemInstance::GetColor(n)),
                     static_cast<unsigned>(row->glowcolor));
    }
    std::fprintf(f, "],\n");

    // ---- items ----
    std::fprintf(f, "\"items\":[\n");
    for (int i = 0; i < itemCount; ++i) {
        const Structs::Item* r = TableRow<Structs::Item>(Arrays::ItemTable::members, i);
        std::fprintf(f, "%s[%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s]", i ? ",\n" : "",
                     NameOf(Arrays::ItemTable::m_memberNames, i).c_str(), r->Type, r->EquipmentSlottingType, r->PickUpType, r->DistType,
                     r->Stackable ? 1 : 0, r->Value, r->GoldValueMultiplier, r->LevelRequirement, r->StrengthRequirement, r->DexterityRequirement,
                     r->EnduranceRequirement, r->EnergyRequirement, r->ClassRequirement, r->Param1, r->Param2, r->Param3, r->Param4, r->Param5,
                     r->Param6, r->BaseStat, r->BaseProp, r->BaseElement, r->Name, r->Material, J(static_cast<const char*>(r->IconName)).c_str(),
                     J(static_cast<const char*>(r->ModularModule)).c_str());
    }
    std::fprintf(f, "],\n\"itemFields\":[\"key\",\"type\",\"slotting\",\"pickUpType\",\"distType\",\"stackable\",\"value\",\"goldValueMultiplier\",\"levelReq\",\"strReq\",\"dexReq\",\"endReq\",\"eneReq\",\"classReq\",\"param1\",\"param2\",\"param3\",\"param4\",\"param5\",\"param6\",\"baseStat\",\"baseProp\",\"baseElement\",\"nameStr\",\"materialStr\",\"icon\",\"modular\"],\n");

    // ---- powers ----
    std::fprintf(f, "\"powers\":[\n");
    for (int p = 0; p < powerCount; ++p) {
        const Structs::ItemPowerRef* r = TableRow<Structs::ItemPowerRef>(Arrays::ItemPowerTable::members, p);
        std::string attrs = "[";
        const auto* a = static_cast<const Structs::AttrBonusEntry*>(r->AttrBonusList);
        for (std::uint32_t k = 0; k < r->AttrBonusListSize; ++k) {
            attrs += (k ? ",[" : "[") + N(a[k].Attr) + "," + N(a[k].Bonus) + "," + N(a[k].Flags) + "]";
        }
        attrs += "]";
        std::fprintf(f, "%s[%s,%d,%d,%s,%d,%d,%d,%d,%d]", p ? ",\n" : "", NameOf(Arrays::ItemPowerTable::m_memberNames, p).c_str(), r->Palette,
                     r->SpecialEffect, attrs.c_str(), r->Description, r->GoldBonusMultiplier, r->SortingOrder, r->GoldBonus, r->AttrMonopoly);
    }
    std::fprintf(f, "],\n\"powerFields\":[\"key\",\"palette\",\"specialEffect\",\"attrBonus\",\"descStr\",\"goldBonusMultiplier\",\"sortingOrder\",\"goldBonus\",\"attrMonopoly\"],\n");

    // ---- power lists, monopoly lists ----
    std::fprintf(f, "\"powerLists\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::ItemPowerList::size); ++l) {
        const auto* r = TableRow<Structs::ItemPowerEntryList>(Arrays::ItemPowerList::members, l);
        const auto* e = static_cast<const Structs::ItemPowerEntry*>(r->list_entries);
        std::string s = "[";
        for (std::uint32_t k = 0; k < r->list_entriesSize; ++k) s += (k ? ",[" : "[") + N(e[k].Power) + "," + N(e[k].Prob) + "]";
        std::fprintf(f, "%s[%s,%s]]", l ? ",\n" : "", NameOf(Arrays::ItemPowerList::m_memberNames, l).c_str(), s.c_str());
    }
    std::fprintf(f, "],\n\"monopoly\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::ItemBonusAttrMonopoly::size); ++l) {
        const auto* r = TableRow<Structs::ItemBonusAttrList>(Arrays::ItemBonusAttrMonopoly::members, l);
        const auto* e = static_cast<const std::int32_t*>(r->list_entries);
        std::string s = "[";
        for (std::uint32_t k = 0; k < r->list_entriesSize; ++k) s += (k ? "," : "") + N(e[k]);
        std::fprintf(f, "%s[%s,%s]]", l ? ",\n" : "", NameOf(Arrays::ItemBonusAttrMonopoly::m_memberNames, l).c_str(), s.c_str());
    }

    // ---- item lists, number tables, loot tables, merchants ----
    std::fprintf(f, "],\n\"itemLists\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::ItemList::size); ++l) {
        const auto* r = TableRow<Structs::ItemListEntryList>(Arrays::ItemList::members, l);
        const auto* e = static_cast<const Structs::ItemListEntry*>(r->list_entries);
        std::string s = "[";
        for (std::uint32_t k = 0; k < r->list_entriesSize; ++k)
            s += (k ? ",[" : "[") + N(e[k].ItemID) + "," + N(e[k].Prob) + "," + N(e[k].Qty) + "]";
        std::fprintf(f, "%s[%s,%s]]", l ? ",\n" : "", NameOf(Arrays::ItemList::m_memberNames, l).c_str(), s.c_str());
    }
    std::fprintf(f, "],\n\"numProbs\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::NumProbArray::size); ++l) {
        const auto* r = TableRow<Structs::NumProbList>(Arrays::NumProbArray::members, l);
        const auto* e = static_cast<const Structs::NumProb*>(r->list_entries);
        std::string s = "[";
        for (std::uint32_t k = 0; k < r->list_entriesSize; ++k) s += (k ? ",[" : "[") + N(e[k].Num) + "," + N(e[k].Prob) + "]";
        std::fprintf(f, "%s[%s,%s]]", l ? ",\n" : "", NameOf(Arrays::NumProbArray::m_memberNames, l).c_str(), s.c_str());
    }
    std::fprintf(f, "],\n\"lootTables\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::LootTable::size); ++l) {
        const Structs::Loot& r = reinterpret_cast<const Structs::Loot*>(Arrays::LootTable::members)[l];
        std::string rnd = "[", fix = "[", sub = "[";
        const auto* re = static_cast<const Structs::LootEntry*>(r.RandomLootEntries);
        for (std::uint32_t k = 0; k < r.RandomLootEntriesSize; ++k) rnd += (k ? "," : "") + LootEntryJson(re[k]);
        const auto* fe = static_cast<const Structs::LootEntry*>(r.FixedLootEntries);
        for (std::uint32_t k = 0; k < r.FixedLootEntriesSize; ++k) fix += (k ? "," : "") + LootEntryJson(fe[k]);
        const auto* se = static_cast<const std::int32_t*>(r.SubLoots);
        for (std::uint32_t k = 0; k < r.SubLootsSize; ++k) sub += (k ? "," : "") + N(se[k]);
        std::fprintf(f, "%s{\"key\":%s,\"rollType\":%d,\"numRandom\":%d,\"random\":%s],\"fixed\":%s],\"sub\":%s]}", l ? ",\n" : "",
                     NameOf(Arrays::LootTable::m_memberNames, l).c_str(), r.RollType, r.NumRandomItemProbs, rnd.c_str(), fix.c_str(), sub.c_str());
    }
    std::fprintf(f, "],\n\"merchants\":[\n");
    for (int l = 0; l < static_cast<int>(Arrays::MerchantTable::size); ++l) {
        const auto* r = reinterpret_cast<const Structs::Merchant*>(Arrays::MerchantTable::members) + l;
        const auto* e = static_cast<const Structs::MerchandiseListEntry*>(r->MerchandiseList);
        std::string s = "[";
        for (std::uint32_t k = 0; k < r->MerchandiseListSize; ++k) s += (k ? ",[" : "[") + N(e[k].ConditionId) + "," + N(e[k].Merchandise) + "]";
        std::fprintf(f, "%s{\"key\":%s,\"buy\":%d,\"sell\":%d,\"stock\":%s]}", l ? ",\n" : "", NameOf(Arrays::MerchantTable::m_memberNames, l).c_str(),
                     r->BuyMultiplier, r->SellMultiplier, s.c_str());
    }
    std::fprintf(f, "],\n\"characterPropertyNames\":[");
    for (int i = 0; i < Structs::CharacterProperties::m_dataNames.size; ++i) {
        std::fprintf(f, "%s%s", i ? "," : "", J(Structs::CharacterProperties::m_dataNames.names[i]).c_str());
    }
    std::fprintf(f, "],\n");

    // ---- player class sheets: Special_Loot_Max_Potions (property 194), the potion cap Character::OnInitFinal copies into
    // ItemInventory::m_maxPotions from PROPS_GetInt(194). The class row word is the base value before skills.
    std::fprintf(f, "\"playerClasses\":[");
    bool firstClass = true;
    for (int i = 0; i < static_cast<int>(Arrays::CharacterTable::size); ++i) {
        const char* name = Arrays::CharacterTable::m_memberNames.names[i];
        const Structs::CharacterProperties* row = TableRow<Structs::CharacterProperties>(Arrays::CharacterTable::members, i);
        const std::int32_t* words = &row->AIFaction;
        if (std::strstr(name, "PlayerBase") == nullptr && words[194] == 0) continue;
        // words 19 (Level) and 149..156 (Stat_Strength/Dexterity/Endurance/Energy, Prereq_*): the class's starting sheet; the
        // save's PROP section holds the hero's changes to it (PlayerSavegame::__SaveProperties), IsEquippableBy compares the sum.
        std::fprintf(f, "%s{\"row\":%d,\"key\":%s,\"maxPotions\":%d,\"transmuteBonus\":%d,\"classString\":%d,\"level\":%d,\"stats\":[%d,%d,%d,%d,%d,%d,%d,%d]}",
                     firstClass ? "" : ",", i, J(name).c_str(), words[194], words[197], row->ClassString, words[19], words[149], words[150], words[151],
                     words[152], words[153], words[154], words[155], words[156]);
        firstClass = false;
    }
    std::fprintf(f, "],\n");

    // ---- texts per language pack, made by the game's own ItemInstance code ----
    const int originalPack = strings->m_currentPack;
    const auto* packs = static_cast<const Structs::LangSheetList*>(static_cast<const void*>(Arrays::StrID_Languages::members));
    std::fprintf(f, "\"languages\":[\n");
    bool firstLang = true;
    const int packCount = static_cast<int>(Arrays::StrID_Languages::size);
    for (int pack = 0; pack < packCount && pack < 8; ++pack) {
        if (!strings->preloadPack(static_cast<unsigned>(pack))) {
            std::fprintf(stderr, "[itemdump] pack %d did not load\n", pack);
        }
        strings->switchPack(static_cast<unsigned>(pack), false);
        const auto* sheets = static_cast<const Structs::LangSheet*>(packs[pack].list);
        std::fprintf(f, "%s{\"pack\":%d,\"key\":%s,\"file\":%s,\n\"items\":[\n", firstLang ? "" : ",\n", pack,
                     NameOf(Arrays::StrID_Languages::m_memberNames, pack).c_str(), J(sheets[0].filename).c_str());
        firstLang = false;
        for (int i = 0; i < itemCount; ++i) {
            std::fprintf(stderr, "[itemdump] pack %d item %d\n", pack, i);
            std::fflush(stderr);
            ItemInstance item(i, 1u);
            const Structs::Item* r = item.GetItem();
            if (r->Type == 13) {
                item.SetValue(r->Value);  // gold: the name formats the amount; the low end of CalcLootItemValue's range
            } else {
                item.SetValue(static_cast<int>(static_cast<std::uint32_t>(r->Value) * static_cast<std::uint32_t>(r->GoldValueMultiplier)));
            }
            item.UpdateLocalization();
            std::fprintf(f, "%s[%s,%s,%s]", i ? ",\n" : "", J(item.m_name).c_str(), J(item.m_stats).c_str(), J(item.m_requirements).c_str());
        }
        std::fprintf(f, "],\n\"powers\":[\n");
        for (int p = 0; p < powerCount; ++p) {
            // AddPower assigns StringManager::getString(Description): a row whose text id has no string in this pack would be
            // a null assign in the game too, so it is written as null (the editor never offers such a power).
            const Structs::ItemPowerRef* pr = TableRow<Structs::ItemPowerRef>(Arrays::ItemPowerTable::members, p);
            if (strings->getString(pr->Description) == nullptr) {
                std::fprintf(f, "%snull", p ? ",\n" : "");
                continue;
            }
            ItemInstance item(0, 1u);
            item.AddPower(p, -1);
            std::fprintf(f, "%s%s", p ? ",\n" : "", J(item.GetPowerDesc(0)).c_str());
        }
        std::fprintf(f, "]}");
    }
    std::fprintf(f, "]}\n");
    if (originalPack >= 0) strings->switchPack(static_cast<unsigned>(originalPack), false);
}

}  // namespace

int main(int argc, char** argv) {
    std::string dumpPath;
    std::vector<char*> rest;
    rest.push_back(argv[0]);
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--dump") == 0 && i + 1 < argc) dumpPath = argv[++i];
        else rest.push_back(argv[i]);
    }
    dh2host::Options opt;
    if (dumpPath.empty() || !dh2host::parseArgs(static_cast<int>(rest.size()), rest.data(), opt)) {
        std::fprintf(stderr, "usage: %s --data <game files dir> --dump <out.json> [--out <log dir>]\n", argv[0]);
        return 1;
    }
    char full[MAX_PATH];
    if (GetFullPathNameA(dumpPath.c_str(), MAX_PATH, full, nullptr) != 0) dumpPath = full;
    dh2host::g_runnerOut = dh2host::splitConsole(opt);
    dh2host::installNullGL();
    if (!dh2host::bootGame(opt)) {
        std::fprintf(dh2host::g_runnerOut, "boot failed\n");
        return 3;
    }
    int f = 0;
    for (; f < 4000 && !Ready(); ++f) {
        dh2host::clockAdvance(opt.dtMs);
        dh2host::runFrame();
    }
    if (!Ready()) {
        std::fprintf(dh2host::g_runnerOut, "[itemdump] data not ready after %d frames\n", f);
        return 4;
    }
    std::fprintf(dh2host::g_runnerOut, "[itemdump] ready at frame %d: %u items, %u powers\n", f, Arrays::ItemTable::size, Arrays::ItemPowerTable::size);
    std::FILE* out = std::fopen(dumpPath.c_str(), "wb");
    if (out == nullptr) {
        std::fprintf(dh2host::g_runnerOut, "[itemdump] cannot write %s\n", dumpPath.c_str());
        return 5;
    }
    Dump(out);
    std::fclose(out);
    std::fprintf(dh2host::g_runnerOut, "[itemdump] wrote %s\n", dumpPath.c_str());
    std::fflush(dh2host::g_runnerOut);
    std::_Exit(0);  // no static destructors: the game was never shut down
}
