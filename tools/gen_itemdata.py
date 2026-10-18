#!/usr/bin/env python3
"""Build-time generator of the save editor's item data (itemdata.json) from the dump the game itself writes.

usage: py -3 -I tools/gen_itemdata.py <itemdata-raw.json from dh2_itemdump> <out itemdata.json | itemdata.js>

The raw dump comes from itemdump/tool_itemdump.cpp (dh2_itemdump, linked against the dh_noam host runner), which runs the original
pydata readers and the original ItemInstance text code. This script only reshapes it and precomputes, per item row, the
ways the original loot code can create that row with powers ("sources"), following these functions of the reconstruction
(port/decomp-reconstruction/game-items-save/src):
  * ItemInventory::AddLoot (0x40407c, ItemInventory-cf6f6b-4.cpp): the item list row of a loot entry, swapped for its
    "<name>_Hard" (id + 1) / "<name>_VeryHard" (id + 2) sibling at difficulty 1 / 2 when that sibling exists; the quantity is
    the ItemListEntry Qty (-2 means 99); powers come from _AddLootItemPowers with c = the difficulty.
  * ItemInventory::_AddLootItems (0x402dfc): every entry of the loot entry's ItemList can be the item.
  * ItemInventory::_AddLootTable (0x4039a0): fixed, random and sub-table entries all end up as loot entries.
  * ItemInventory::DBG_DropAllLoots (0x404658) is the only user of loot table 5 (AllItemsLootTable): a debug path, excluded.
  * ItemInventory::_AddLootItemPowers (0x403310) and _GetProbQuantity (0x401b90): the power count is a Num of the entry's
    NumProbArray row whose Prob is not 0; the powers come from the entry's ItemPowerList; AddPower (0x3fbc60) swaps a power
    for its _Hard/_VeryHard sibling at difficulty 1/2.
The editor (web/dh2save.js) applies the same rules when it checks a power selection.
"""
import json, sys

DEBUG_LOOT_TABLES = {"AllItemsLootTable"}  # ItemInventory::DBG_DropAllLoots -> AddLoot(5, ...)

# Armour-set trophies: ItemInventory::TrophyCheckArmorSet (0x3fcff8) checks equipment slots 3, 4, 8, 0, 7.
# Id lists: game-app/src/r2a-game-app-global-f216b6-3.cpp; class sets: game-app/src/g1-game-app-global-f216b6-4.cpp.
SET_BY_IDS = {
    "Archmage": ("gear_mage", [289, 290, 291, 325, 326, 327, 361, 362, 363, 217, 218, 219, 253, 254, 255]),   # 0x3fcaec
    "Jester": ("gear_rogue", [583, 584, 585, 619, 620, 621, 655, 656, 657, 511, 512, 513, 547, 548, 549]),    # 0x3fcc98
    "Lordly": ("gear_warrior", [844, 845, 846, 880, 881, 882, 916, 917, 918, 772, 773, 774, 808, 809, 810]),  # 0x3fce48
}
SET_BY_CLASS = {  # Structs::Item ClassRequirement of all five pieces
    8: ("Decay", "gear_necromancer"), 9: ("Phantasm", "gear_illusionist"), 5: ("Shadow", "gear_assassin"),  # 0x3fc6e8 0x3fc790 0x3fc83c
    6: ("Marksman", "gear_archer"), 3: ("Angelic", "gear_paladin"), 2: ("Rage", "gear_berserker"),          # 0x3fc8e8 0x3fc994 0x3fca40
}
SET_SLOTTING = {0, 3, 4, 7, 8}  # EquipmentSlottingTypes Torso, Feet, HandArmor, Waist, Head = the slots the check reads


def variant_row(names, item_id, difficulty):
    """ItemInventory::AddLoot: the row the loot code creates for item_id at this difficulty (flag false)."""
    variant = item_id + 2
    if variant < len(names):
        if difficulty == 1 and names[item_id + 1] == names[item_id] + "_Hard":
            return item_id + 1
        if difficulty == 2 and names[variant] == names[item_id] + "_VeryHard":
            return variant
    return item_id


def main():
    raw = json.load(open(sys.argv[1], encoding="utf-8"))
    F = {k: i for i, k in enumerate(raw["itemFields"])}
    P = {k: i for i, k in enumerate(raw["powerFields"])}
    items, powers = raw["items"], raw["powers"]
    names = [it[F["key"]] for it in items]

    sources = {}   # row -> set of (powerList, numProbs, difficulty)
    qtys = {}      # row -> set of quantities the loot code gives
    for table in raw["lootTables"]:
        if table["key"] in DEBUG_LOOT_TABLES:
            continue
        for entry in table["random"] + table["fixed"]:
            lst = entry["itemList"]
            if lst < 0 or lst >= len(raw["itemLists"]):
                continue
            for item_id, prob, qty in raw["itemLists"][lst][1]:
                if item_id < 0 or item_id >= len(items):
                    continue
                for d in (0, 1, 2):
                    row = variant_row(names, item_id, d)
                    sources.setdefault(row, set()).add((entry["powerList"], entry["numPowerProbs"], d))
                    qtys.setdefault(row, set()).add(99 if qty == -2 else qty)

    sets = {}
    for name, (trophy, ids) in SET_BY_IDS.items():
        for i in ids:
            sets[i] = [name, trophy]
    for i, it in enumerate(items):
        if it[F["slotting"]] in SET_SLOTTING and it[F["classReq"]] in SET_BY_CLASS and i not in sets:
            sets[i] = list(SET_BY_CLASS[it[F["classReq"]]])

    keep = ["key", "type", "slotting", "pickUpType", "stackable", "value", "goldValueMultiplier", "levelReq", "strReq", "dexReq",
            "endReq", "eneReq", "classReq", "param1", "param2", "param3", "baseProp", "baseElement", "icon"]
    out_items = []
    for i, it in enumerate(items):
        row = [it[F[k]] for k in keep]
        src = sorted(sources.get(i, ()))
        row.append([list(s) for s in src])
        row.append(sorted(qtys.get(i, ())))
        row.append(sets.get(i))
        out_items.append(row)

    pkeep = ["key", "sortingOrder", "goldBonusMultiplier", "goldBonus", "attrMonopoly", "attrBonus"]
    out = {
        "format": 1,
        "source": "generated by tools/gen_itemdata.py from " + raw["source"],
        "itemFields": keep + ["sources", "lootQty", "set"],
        "items": out_items,
        "powerFields": pkeep,
        "powers": [[p[P[k]] for k in pkeep] for p in powers],
        "powerLists": [pl[1] for pl in raw["powerLists"]],
        "powerListNames": [pl[0] for pl in raw["powerLists"]],
        "monopoly": [m[1] for m in raw["monopoly"]],
        "numProbs": [n[1] for n in raw["numProbs"]],
        "numProbNames": [n[0] for n in raw["numProbs"]],
        "merchants": [{"key": m["key"], "buy": m["buy"], "sell": m["sell"]} for m in raw["merchants"]],
        "constants": raw["constants"],
        "powerColors": raw["powerColors"],
        # player class rows of CharacterTable: starting Level and Stat_*/Prereq_* words (x256), and where those words sit in
        # the save's PROP section (PlayerSavegame::__SaveProperties writes property i as word i after the count)
        "playerClasses": [{"key": c["key"], "level": c.get("level"), "stats": c.get("stats")} for c in raw["playerClasses"] if "PlayerBase" in c["key"]],
        "propIndex": {k: raw["characterPropertyNames"].index(k) for k in ("Level", "Stat_Strength", "Stat_Dexterity", "Stat_Endurance", "Stat_Energy",
                                                                       "Prereq_Strength", "Prereq_Dexterity", "Prereq_Endurance", "Prereq_Energy")},
        "languages": [{"pack": L["pack"], "key": L["key"], "file": L["file"], "items": L["items"], "powers": L["powers"]}
                      for L in raw["languages"]],
    }
    text = json.dumps(out, ensure_ascii=False, separators=(",", ":"))
    with open(sys.argv[2], "w", encoding="utf-8", newline="\n") as f:
        # .js: loaded with a <script> tag (the Android WebView cannot fetch file:// assets); .json: for node and tools
        f.write("window.DH2_ITEMDATA=" + text + ";\n" if sys.argv[2].endswith(".js") else text)
    with_src = sum(1 for r in out_items if r[-3])
    print("items %d (%d with loot sources), powers %d, languages %d -> %s" % (len(out_items), with_src, len(powers), len(out["languages"]), sys.argv[2]))


if __name__ == "__main__":
    main()
