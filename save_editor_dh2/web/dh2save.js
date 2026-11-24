// Dungeon Hunter 2 save editor: the save format and the item rules. One file for every platform (the Android WebView in the
// game APK, the Windows app, node tests). No DOM use here.
//
// Spec = the reconstructed original code (port/decomp-reconstruction/game-items-save/src):
//  * Savegame container (Savegame.cpp): Savegame::saveAll (0x315fb8) writes a uint32 section count, then per section a uint32
//    data size, a 4-byte name and the data; the reader is the section table walk of Savegame::_cacheFile (0x315ad0). A first
//    word of -1 marks a file being written (Savegame::UpdateJobs 0x314734); the game then falls back to "<name>.bak".
//  * Strings: IStreamBase::writeAs(std::string) (0x461668) = int32 length including the NUL, the bytes, the NUL;
//    IStreamBase::readAs(std::string) (0x461da8).
//  * GEAR section: PlayerSavegame::__SaveInventory (0x46a0b4) / __LoadInventory (0x46a3a0): uint32 gold, uint32 current
//    equipment set, uint32 item count, then per item: string ItemTable name, int32 equipment slot in set 0, int32 slot in set 1
//    (-1 = not equipped), int32 quantity, int32 value, uint8 pick-up flag (ItemInstance +0x68), uint32 power count, power names
//    (ItemPowerTable names).
//  * Gold: ItemInventory::SetGold (0x3fdfd8) clamps to m_maxGold, 999999999 for a player (Character::LoadGearsProperties path,
//    game-character Character-ee9946-9.cpp).
//  * Potions: the only potion row is "Potion0" (ItemInventory::SetPotionQty 0x3ffc40); ItemInventory::_AddItemInstance keeps the
//    first type-14 item as the potion stack.
//  * Rarity: ItemInstance::GetColor / GetFontDef (0x3fa6dc / 0x3fa5cc): the colour is chosen by the number of powers only
//    (PyData constants ItemPowerColor zero..four -> FontPalette rows WhiteBlackOutline, GreenBlackOutline, ...).
//  * Value of a new item: ItemInventory::CalcLootItemValue (0x4020b4): Value * GoldValueMultiplier + sum over powers of
//    GoldBonusMultiplier * GoldBonus (32-bit wrap).
//  * Power order: ItemInstance::AddPower (0x3fbc60) inserts by SortingOrder with the original's swap (ids move, values stay).
//  * Powers allowed on an item: ItemInventory::_AddLootItemPowers (0x403310), see allowedPowerCounts / checkPowers.
(function (root, factory) {
    if (typeof module === "object" && module.exports) module.exports = factory();
    else root.DH2 = factory();
})(typeof self !== "undefined" ? self : this, function () {
    "use strict";

    const MAX_GOLD = 999999999;            // ItemInventory::m_maxGold of a player
    const DEFAULT_POTION_CAP = 5;          // a new hero's starting stack (BasePlayerInventory), shown as information only
    const MAX_POTIONS = 2147483647;        // the stack count is an int32 in GEAR (__SaveInventory); the editor allows 0..2^31-1

    // ---------------------------------------------------------------- bytes
    function Reader(bytes, pos, end) {
        this.b = bytes; this.p = pos || 0; this.end = end === undefined ? bytes.length : end;
        this.dv = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
    }
    Reader.prototype.need = function (n) { if (this.p + n > this.end) throw new Error("truncated data at byte " + this.p); };
    Reader.prototype.u32 = function () { this.need(4); const v = this.dv.getUint32(this.p, true); this.p += 4; return v; };
    Reader.prototype.i32 = function () { this.need(4); const v = this.dv.getInt32(this.p, true); this.p += 4; return v; };
    Reader.prototype.u8 = function () { this.need(1); return this.b[this.p++]; };
    Reader.prototype.str = function () {   // IStreamBase::readAs(std::string)
        const len = this.i32();
        if (len <= 0 || len > 4096) throw new Error("bad string length " + len + " at byte " + (this.p - 4));
        this.need(len);
        const s = latin1(this.b.subarray(this.p, this.p + len - 1));
        if (this.b[this.p + len - 1] !== 0) throw new Error("string without NUL at byte " + this.p);
        this.p += len;
        return s;
    };

    function Writer() { this.chunks = []; this.n = 0; }
    Writer.prototype.bytes = function (u8) { this.chunks.push(u8); this.n += u8.length; };
    Writer.prototype.u32 = function (v) { const b = new Uint8Array(4); new DataView(b.buffer).setUint32(0, v >>> 0, true); this.bytes(b); };
    Writer.prototype.i32 = function (v) { const b = new Uint8Array(4); new DataView(b.buffer).setInt32(0, v | 0, true); this.bytes(b); };
    Writer.prototype.u8 = function (v) { this.bytes(Uint8Array.of(v & 0xff)); };
    Writer.prototype.str = function (s) {  // IStreamBase::writeAs(std::string)
        const b = new Uint8Array(s.length + 1);
        for (let i = 0; i < s.length; i++) b[i] = s.charCodeAt(i) & 0xff;
        this.i32(b.length); this.bytes(b);
    };
    Writer.prototype.out = function () {
        const r = new Uint8Array(this.n); let o = 0;
        for (const c of this.chunks) { r.set(c, o); o += c.length; }
        return r;
    };
    function latin1(u8) { let s = ""; for (let i = 0; i < u8.length; i++) s += String.fromCharCode(u8[i]); return s; }

    // ---------------------------------------------------------------- container
    // Savegame::_cacheFile / ReadSectionTable: count, then (size, name, data) until the count or the end of the file.
    function parseContainer(bytes) {
        if (!(bytes instanceof Uint8Array)) bytes = new Uint8Array(bytes);
        const r = new Reader(bytes);
        if (bytes.length < 4) throw new Error("file too small to be a savegame");
        const count = r.u32();
        if (count === 0xFFFFFFFF) throw new Error("this file was being written when the game stopped (first word -1); the game uses the .bak copy");
        const sections = [];
        for (let i = 0; i < count && r.p < bytes.length; i++) {
            const size = r.u32();
            r.need(4);
            const name = latin1(bytes.subarray(r.p, r.p + 4)); r.p += 4;
            r.need(size);
            sections.push({ name: name, data: bytes.subarray(r.p, r.p + size) });
            r.p += size;
        }
        return { count: count, sections: sections, tail: bytes.subarray(r.p) };
    }
    function writeContainer(c) {
        const w = new Writer();
        w.u32(c.count);
        for (const s of c.sections) {
            w.u32(s.data.length);
            const n = new Uint8Array(4); for (let i = 0; i < 4; i++) n[i] = s.name.charCodeAt(i) & 0xff;
            w.bytes(n); w.bytes(s.data);
        }
        w.bytes(c.tail);
        return w.out();
    }
    function section(c, name) { return c.sections.find(function (s) { return s.name === name; }) || null; }

    // ---------------------------------------------------------------- GEAR
    function parseGear(data) {
        const r = new Reader(data);
        const g = { gold: r.u32(), equipSet: r.u32(), items: [] };
        const n = r.u32();
        for (let i = 0; i < n; i++) {
            const it = { key: r.str(), slot0: r.i32(), slot1: r.i32(), qty: r.i32(), value: r.i32(), flag: r.u8(), powers: [] };
            const pc = r.u32();
            if (pc > 64) throw new Error("bad power count " + pc);
            for (let k = 0; k < pc; k++) it.powers.push(r.str());
            g.items.push(it);
        }
        g.tail = data.subarray(r.p);   // __LoadInventory reads nothing after the items; kept as is
        return g;
    }
    function writeGear(g) {
        const w = new Writer();
        w.u32(g.gold); w.u32(g.equipSet); w.u32(g.items.length);
        for (const it of g.items) {
            w.str(it.key); w.i32(it.slot0); w.i32(it.slot1); w.i32(it.qty); w.i32(it.value); w.u8(it.flag);
            w.u32(it.powers.length);
            for (const p of it.powers) w.str(p);
        }
        w.bytes(g.tail || new Uint8Array(0));
        return w.out();
    }

    // Small read-only sections shown in the header.
    function parseInfo(c) {
        const info = {};
        try { const s = section(c, "PNAM"); if (s) info.name = new Reader(s.data).str(); } catch (e) { /* not shown */ }
        try { const s = section(c, "PLVL"); if (s) info.level = new Reader(s.data).i32(); } catch (e) { }
        try { const s = section(c, "PCLS"); if (s) info.cls = new Reader(s.data).str(); } catch (e) { }
        try { const s = section(c, "LNAM"); if (s) info.levelName = new Reader(s.data).str(); } catch (e) { }
        return info;
    }

    // PROP (PlayerSavegame::__SaveProperties 0x4689d8): int32 count (224), then property i as int32 word i (x256 fixed point
    // for numbers), then one byte. These are the hero's changes to the class row's sheet (CharProperties::m_savedProps).
    function parseProps(c) {
        const s = section(c, "PROP");
        if (!s || s.data.length < 4) return null;
        const r = new Reader(s.data);
        const n = r.i32();
        if (n <= 0 || n > 4096 || 4 + 4 * n > s.data.length) return null;
        const w = new Int32Array(n);
        for (let i = 0; i < n; i++) w[i] = r.i32();
        return w;
    }

    // ---------------------------------------------------------------- the save object
    function Save(bytes) {
        this.original = new Uint8Array(bytes);
        this.container = parseContainer(this.original);
        const gear = section(this.container, "GEAR");
        if (!gear) throw new Error("no GEAR section: this is not a player savegame (dh2_NNN.savegame)");
        this.gear = parseGear(gear.data);
        this.info = parseInfo(this.container);
    }
    // Bytes of the edited file. Without edits the GEAR bytes are rewritten from the parsed values and must equal the original.
    Save.prototype.toBytes = function () {
        const c = this.container;
        const sections = c.sections.map(function (s) { return { name: s.name, data: s.data }; });
        const g = sections.find(function (s) { return s.name === "GEAR"; });
        g.data = writeGear(this.gear);
        return writeContainer({ count: c.count, sections: sections, tail: c.tail });
    };

    // The hero's checkpoint copy (dh2_NNN_single.checkpoint / _multi.checkpoint, PlayerSavegame::SG_SaveCheckpoint) holds the
    // same sections. The game reads only QEST from it (Level -> Character::SG_LoadCheckpoint(16)), but the editor keeps its
    // GEAR equal to the savegame's so both copies of the hero agree. Returns the checkpoint bytes with the GEAR data replaced.
    function replaceGear(bytes, gearData) {
        const c = parseContainer(new Uint8Array(bytes));
        const g = section(c, "GEAR");
        if (!g) throw new Error("no GEAR section in the checkpoint");
        g.data = gearData;
        return writeContainer(c);
    }
    Save.prototype.gearBytes = function () { return writeGear(this.gear); };

    // ---------------------------------------------------------------- item data (itemdata.json)
    function ItemData(json) {
        const d = typeof json === "string" ? JSON.parse(json) : json;
        this.d = d;
        const fi = {}; d.itemFields.forEach(function (k, i) { fi[k] = i; });
        const pf = {}; d.powerFields.forEach(function (k, i) { pf[k] = i; });
        this.items = d.items.map(function (r, i) {
            const o = { id: i };
            for (const k in fi) o[k] = r[fi[k]];
            return o;
        });
        this.powers = d.powers.map(function (r, i) {
            const o = { id: i };
            for (const k in pf) o[k] = r[pf[k]];
            return o;
        });
        this.itemByKey = {}; this.items.forEach((it) => { if (!(it.key in this.itemByKey)) this.itemByKey[it.key] = it.id; });
        this.powerByKey = {}; this.powers.forEach((p) => { if (!(p.key in this.powerByKey)) this.powerByKey[p.key] = p.id; });
        this.lang = 0;
        const C = d.constants;
        this.typeName = invert(C.ItemType); this.slotName = invert(C.EquipmentSlots); this.slottingName = invert(C.EquipmentSlottingTypes);
        this.className = invert(C.ClassRequirements);
        this.maxQty = (C.InventoryDesign && C.InventoryDesign.MaxItemInstanceQuantity) || 99;
    }
    function invert(o) { const r = {}; for (const k in o) if (!(o[k] in r)) r[o[k]] = k; return r; }

    ItemData.prototype.text = function (id) { const L = this.d.languages[this.lang]; return L.items[id] || ["", "", ""]; };
    ItemData.prototype.powerText = function (id) { const L = this.d.languages[this.lang]; return L.powers[id]; };
    ItemData.prototype.languages = function () {
        return this.d.languages.map(function (L, i) { return { index: i, key: L.key, sample: (L.items[5] || [""])[0] }; });
    };

    // ItemInstance::GetColor(powerCount): colour and the game's palette name (WhiteBlackOutline -> "White").
    ItemData.prototype.rarity = function (powerCount) {
        const c = this.d.powerColors.find(function (x) { return x.powers === powerCount; }) || this.d.powerColors[this.d.powerColors.length - 1];
        const name = String(c.fontDefName || "").replace(/BlackOutline$/, "").replace(/^DefaultFont$/, "Default");
        return { name: name, palette: c.fontDefName, rgb: "#" + ("000000" + (c.color >>> 0).toString(16)).slice(-6).toUpperCase(), powers: powerCount };
    };

    // ItemInventory::CalcLootItemValue for a non-gold item.
    ItemData.prototype.lootValue = function (itemId, powerIds) {
        const it = this.items[itemId];
        let v = Math.imul(it.value, it.goldValueMultiplier);
        for (const p of powerIds) { const pw = this.powers[p]; v = (v + Math.imul(pw.goldBonusMultiplier, pw.goldBonus)) | 0; }
        return v;
    };

    // NativeGetItemInfo (game-app global-f216b6-13.cpp): sell price = value * merchant BuyMultiplier / 100 (at least 1);
    // transmute value with no Special_Transmute_Efficiency bonus.
    ItemData.prototype.prices = function (value) {
        const buys = this.d.merchants.map(function (m) { return m.buy; });
        const lo = Math.min.apply(null, buys), hi = Math.max.apply(null, buys);
        const sell = function (pct) { return Math.max(1, Math.trunc(value * (pct / 100))); };
        const tm = (this.d.constants.CharacterDesign && this.d.constants.CharacterDesign.TransmuteMultiplier) || 0;
        const product = Math.imul(Math.imul(value, 256), 256);
        const transmute = Math.max(1, Math.imul(tm, product >> 8) >> 16);
        return { sellMin: sell(lo), sellMax: sell(hi), buyPctMin: lo, buyPctMax: hi, transmute: transmute };
    };

    // Everything the item-detail screen shows (NativeGetItemInfo: name, ItemStatsDesc, ItemReqsDesc, ItemPowers%dDesc,
    // ItemValue, the name colour) for a row and a power list.
    ItemData.prototype.describe = function (itemId, powerIds, value) {
        const it = this.items[itemId];
        const t = this.text(itemId);
        const powers = powerIds.map((p) => ({ id: p, key: this.powers[p] ? this.powers[p].key : "?", text: this.powers[p] ? this.powerText(p) : null,
            attrs: this.powers[p] ? this.powers[p].attrBonus.map((a) => ({ attr: invert(this.d.constants.ItemBonusAttr)[a[0]] || a[0], bonus: a[1] / 256 })) : [] }));
        return {
            id: itemId, key: it.key, name: t[0], stats: t[1], reqs: t[2], type: this.typeName[it.type] || String(it.type),
            slotting: this.slottingName[it.slotting] || String(it.slotting), level: it.type === 14 ? 0 : it.levelReq,
            classReq: this.className[it.classReq] || "", rarity: this.rarity(powerIds.length), powers: powers,
            value: value, prices: this.prices(value), stackable: !!it.stackable, set: it.set ? { name: it.set[0], trophy: it.set[1] } : null,
            difficulty: /_VeryHard$/.test(it.key) ? "Very Hard" : /_Hard$/.test(it.key) ? "Hard" : "Normal",
            armor: [7, 8, 9, 10, 12, 6].indexOf(it.type) >= 0 ? (it.param1 >> 8) : null,
            block: it.type === 6 ? (it.param2 >> 8) : null,
            damage: it.type >= 0 && it.type <= 5 ? [it.param1 >> 8, it.param2 >> 8] : null,
        };
    };

    // ---------------------------------------------------------------- the hero and what he can use
    // ItemInstance::IsEquippableBy (0x3fa330): Level >= LevelRequirement, Prereq_X + Stat_X >= XRequirement for the four
    // attributes (all x256), and the class requirement 1..9 names one CharacterTable row that must be the hero's class.
    const CLASS_ROWS = [null, "KnightPlayerBase", "KnightPlayerBase_Berserker", "KnightPlayerBase_Paladin", "RoguePlayerBase",
        "RoguePlayerBase_Assassin", "RoguePlayerBase_Archer", "MagePlayerBase", "MagePlayerBase_Necromancer", "MagePlayerBase_Illusionist"];
    const ATTRS = ["str", "dex", "end", "ene"];
    // Starting Strength/Dexterity/Endurance/Energy of each class. They are in no table (the CharacterTable player rows hold
    // 0; the class setup scripts put them on the base sheet), so they were read off the game's own Stats screen: the
    // reconstructed game (host runner) showing a real level 6 and level 12 Warrior save, and the same save with only its
    // PCLS class changed, minus the save's spent points and the equipped item bonuses. Subclasses keep their class's values.
    const CLASS_START = { Knight: [10, 5, 8, 3], Rogue: [5, 10, 6, 5], Mage: [4, 6, 6, 10] };
    function classStart(cls) { const m = /^(Knight|Rogue|Mage)PlayerBase/.exec(cls || ""); return m ? CLASS_START[m[1]] : null; }
    // "KnightPlayerBase_Berserker" -> "Knight / Berserker"; the game calls the Knight class "Warrior".
    ItemData.prototype.heroClassLabel = function (cls) {
        const m = /^(Knight|Rogue|Mage)PlayerBase(?:_(\w+))?$/.exec(cls || "");
        if (!m) return cls || "?";
        return (m[1] === "Knight" ? "Warrior" : m[1]) + (m[2] ? " / " + m[2] : "");
    };
    // The hero's sheet as far as the files tell it: the class row (CharacterTable) + the save's PROP changes + the bonuses
    // of the items equipped in the active set (AllStats counts for all four). Skill and buff bonuses are not in any file.
    ItemData.prototype.hero = function (save) {
        const info = save.info, props = parseProps(save.container), P = this.d.propIndex || {};
        const row = (this.d.playerClasses || []).find(function (c) { return c.key === info.cls; }) || null;
        const start = classStart(info.cls);
        const base = start ? start.slice() : [0, 0, 0, 0], spent = [0, 0, 0, 0], items = [0, 0, 0, 0];
        const names = ["Strength", "Dexterity", "Endurance", "Energy"];
        names.forEach(function (n, i) {
            if (row && row.stats) base[i] += (row.stats[i] + row.stats[i + 4]) / 256;
            if (props && P["Stat_" + n] !== undefined) spent[i] = (props[P["Stat_" + n]] + props[P["Prereq_" + n]]) / 256;
        });
        const set = save.gear.equipSet;
        for (const it of save.gear.items) {
            if ((set === 1 ? it.slot1 : it.slot0) < 0) continue;
            for (const pk of it.powers) {
                const p = this.powers[this.powerByKey[pk]];
                if (!p) continue;
                for (const a of p.attrBonus) {
                    if (a[0] === 0) for (let i = 0; i < 4; i++) items[i] += a[1] / 256;
                    else if (a[0] >= 1 && a[0] <= 4) items[a[0] - 1] += a[1] / 256;
                }
            }
        }
        const total = base.map(function (b, i) { return Math.floor(b + spent[i] + items[i]); });
        const level = info.level !== undefined ? info.level : (row && props ? (row.level + props[P.Level]) >> 8 : 0);
        return { level: level, cls: info.cls || "", classKnown: !!row, startKnown: !!start, attrs: total, base: base, spent: spent, items: items, fromSave: !!props };
    };
    // Reasons why this hero cannot equip the row (empty array = he can). hero = ItemData.hero(save).
    ItemData.prototype.cannotUse = function (itemId, hero) {
        const it = this.items[itemId], out = [];
        if (!hero || it.type > 12 || it.type < 0) return out;
        if (it.levelReq > hero.level) out.push("level " + it.levelReq + " (hero " + hero.level + ")");
        const req = [it.strReq, it.dexReq, it.endReq, it.eneReq];
        for (let i = 0; i < 4; i++) if (req[i] > hero.attrs[i]) out.push(req[i] + " " + ATTRS[i] + " (hero " + hero.attrs[i] + ")");
        if (it.classReq >= 1 && it.classReq <= 9 && CLASS_ROWS[it.classReq] !== hero.cls) {
            const parts = String(this.text(itemId)[2] || "").split(/[,，、]\s*/);
            out.push("class: " + (it.classReq && parts.length > 1 ? parts[parts.length - 1].trim() : (this.className[it.classReq] || "")));
        }
        return out;
    };

    // ---------------------------------------------------------------- sets and groups
    // Armour sets: the game's own sets (it.set: the trophy sets of ItemInventory::TrophyCheckArmorSet), else the armour
    // family the item rows share (<Class_>Plate|Leather|Cloth<Piece>NN, one name per family in the game's text, e.g.
    // "Soldier Armor/Belt/Boots/..."), which has no set bonus.
    const FAMILY = /^(?:([A-Za-z]+)_)?(Plate|Leather|Cloth)(Armor|Belt|Boots|Gloves|Helm)(\d+)(?:_Hard|_VeryHard)?$/;
    ItemData.prototype.setOf = function (itemId) {
        const it = this.items[itemId];
        if (it.set) return { key: "set:" + it.set[0], name: it.set[0], trophy: it.set[1], game: true };
        const m = FAMILY.exec(it.key);
        if (!m) return null;
        const key = "fam:" + (m[1] || "") + m[2] + m[4];
        return { key: key, name: this.familyName(key), material: m[2], game: false };
    };
    // The family's name in the current language: the words all its pieces' names share (Soldier Armor + Soldier Belt ->
    // "Soldier"); the internal name when they share none.
    ItemData.prototype.familyName = function (key) {
        this._fam = this._fam || {};
        const ck = this.lang + key;
        if (ck in this._fam) return this._fam[ck];
        let common = null;
        for (const it of this.items) {
            const m = FAMILY.exec(it.key);
            if (!m || it.set || "fam:" + (m[1] || "") + m[2] + m[4] !== key || /_(Very)?Hard$/.test(it.key)) continue;
            const w = String(this.text(it.id)[0] || "").split(/\s+/);
            if (common === null) common = w;
            else { let k = 0; while (k < common.length && k < w.length && common[k] === w[k]) k++; common = common.slice(0, k); }
        }
        const name = common && common.join(" ") ? common.join(" ") : key.slice(4);
        this._fam[ck] = name;
        return name;
    };
    ItemData.prototype.pieceOrder = function (itemId) { return [10, 7, 8, 12, 9, 6, 0, 1, 2, 3, 4, 5, 11].indexOf(this.items[itemId].type); };
    const TYPE_LABEL = { 0: "One-hand weapons", 1: "Off-hand weapons", 2: "Two-hand weapons", 3: "Daggers and claws (either hand)", 4: "Bows",
        5: "Staffs and wands", 6: "Shields", 7: "Body armour", 8: "Gloves and bracers", 9: "Boots", 10: "Helms", 11: "Rings", 12: "Belts",
        13: "Gold", 14: "Potions", 15: "Valuables", 16: "Quest items" };
    ItemData.prototype.typeLabel = function (type) { return TYPE_LABEL[type] || this.typeName[type] || String(type); };

    // ---------------------------------------------------------------- bonus categories and tiers
    // From the ItemBonusAttr ids of a power's first attribute bonus (ItemPowerRef.AttrBonusList).
    const CATEGORIES = [
        ["Attributes", [0, 1, 2, 3, 4]],
        ["Life and mana", [5, 6, 20, 21, 22, 23, 37, 38]],
        ["Elemental damage", [7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 49, 50, 51]],
        ["Attack: damage, critical, attack rating", [24, 28, 29, 52, 53, 54, 55, 56, 57, 58]],
        ["Defence: armour, block, dodge, defence rating", [25, 26, 27, 30]],
        ["Resistances", [31, 32, 33, 34, 35, 36]],
        ["Spells", [39, 40, 41, 42, 43, 44, 45, 46]],
        ["Loot: gold and treasure find", [47, 48]],
    ];
    const FAMILY_LABEL = { AR: "Attack rating", DR: "Defence rating", HP: "Life (HP)", MP: "Mana (MP)", HPRegen: "Life regeneration",
        MPRegen: "Mana regeneration", ItemPowerSpellCritical: "Spell critical", ItemPowerSpellToHit: "Spell hit chance", ElectricalDamage: "Lightning damage",
        ResistElectrical: "Resist lightning", AllStats: "All attributes", LootGoldBonus: "Gold find", LootTreasureBonus: "Treasure find", Critical: "Critical chance",
        Damage: "Damage %" };
    ItemData.prototype.categories = function () { return CATEGORIES.map(function (c) { return c[0]; }).concat(["Other"]); };
    // {category, family, label, tier (1..8 or 0), difficulty ("" | "Hard" | "Very Hard")}
    ItemData.prototype.powerInfo = function (powerId) {
        const p = this.powers[powerId];
        const m = /^(.*?)(?:_(\d+))?(?:_(Hard|VeryHard))?$/.exec(p.key);
        const fam = m[1];
        const a = p.attrBonus.length ? p.attrBonus[0][0] : -1;
        const cat = CATEGORIES.find(function (c) { return c[1].indexOf(a) >= 0; });
        const label = FAMILY_LABEL[fam] || fam.replace(/^SpellBonusDamage(\w+)$/, "$1 spell damage").replace(/^Resist(\w+)$/, "Resist $1")
            .replace(/([a-z])([A-Z])/g, "$1 $2").replace(/^(\w)/, function (x) { return x.toUpperCase(); });
        return { category: cat ? cat[0] : "Other", family: fam, label: label, tier: m[2] ? +m[2] : 0,
                 difficulty: m[3] === "VeryHard" ? "Very Hard" : (m[3] || "") };
    };

    // ---------------------------------------------------------------- rules for new items
    // Item rows the editor may add: loot can create them (sources), and they are not gold (type 13: _AddItemInstance turns
    // a gold item into gold on load), not quest items (16, quest state) and not the potion (edited as a count).
    ItemData.prototype.canAdd = function (itemId) {
        const it = this.items[itemId];
        return it && it.sources.length > 0 && [13, 14, 16, -1].indexOf(it.type) < 0;
    };

    // AddPower's difficulty variant: power id + 1 / + 2 when its name is "<name>_Hard" / "<name>_VeryHard".
    ItemData.prototype.powerVariant = function (powerId, difficulty) {
        if (difficulty === 1 || difficulty === 2) {
            const v = this.powers[powerId + difficulty];
            if (v && v.key === this.powers[powerId].key + (difficulty === 1 ? "_Hard" : "_VeryHard")) return powerId + difficulty;
        }
        return powerId;
    };

    // One "source" of a row = one way the loot code makes it: (ItemPowerList, NumProbArray row, difficulty).
    // Returns {counts: [...], powers: [ids after the difficulty variant], all: bool}.
    // Memoised per (list, numProb, difficulty): the result is shared, callers must not change it.
    ItemData.prototype.sourceOptions = function (src) {
        const ck = src[0] + "," + src[1] + "," + src[2];
        this._so = this._so || {};
        if (!(ck in this._so)) this._so[ck] = this._sourceOptions(src);
        return this._so[ck];
    };
    ItemData.prototype._sourceOptions = function (src) {
        const list = src[0], numProb = src[1], diff = src[2];
        if (list < 0 || list >= this.d.powerLists.length) return { counts: [0], powers: [], difficulty: diff, list: list };
        const entries = this.d.powerLists[list];
        const counts = [];
        const np = this.d.numProbs[numProb];
        if (np) for (const e of np) if (e[1] !== 0 && counts.indexOf(e[0]) < 0) counts.push(e[0]);
        if (!np) counts.push(0);   // _GetProbQuantity asserts and returns 0
        const powers = [];
        for (const e of entries) {
            const id = this.powerVariant(e[0], diff);
            if (e[0] >= 0 && e[0] < this.powers.length && powers.indexOf(id) < 0) powers.push(id);
        }
        return { counts: counts.sort(function (a, b) { return a - b; }), powers: powers, difficulty: diff, list: list,
                 listName: this.d.powerListNames[list], numProbName: this.d.numProbNames[numProb] };
    };

    // Simple mode of the editor: the strongest loot source of a row and the strongest tier of every bonus type.
    // Rank of a source = [power level of its pool (ArmorPowerLevel_08 -> 8, class lists 0), largest bonus count it rolls,
    // sum of the values of its bonuses]; higher wins in that order. Memoised per row; callers must not change the result.
    ItemData.prototype.powerValue = function (powerId) {
        return this.powers[powerId].attrBonus.reduce(function (s, a) { return s + Math.abs(a[1]); }, 0);
    };
    ItemData.prototype.sourceRank = function (o) {
        const m = /PowerLevel_(\d+)$/.exec(o.listName || "");
        const self = this;
        return [m ? +m[1] : 0, o.counts.reduce(function (mx, c) { return Math.max(mx, Math.min(c, o.powers.length)); }, 0),
                o.powers.reduce(function (s, p) { return s + self.powerValue(p); }, 0)];
    };
    // {choices: distinct sources, strongest first (each with .rank), strongest: power ids = best tier of every bonus type}
    ItemData.prototype.bestOf = function (itemId) {
        this._bo = this._bo || {};
        if (this._bo[itemId]) return this._bo[itemId];
        const seen = {}, choices = [];
        for (const src of this.items[itemId].sources) {
            const o = this.sourceOptions(src);
            const k = o.powers.join(",") + "|" + o.counts.join(",");
            if (seen[k]) continue;
            seen[k] = 1; choices.push(Object.assign({ rank: this.sourceRank(o) }, o));
        }
        choices.sort(function (a, b) { for (let i = 0; i < 3; i++) if (a.rank[i] !== b.rank[i]) return b.rank[i] - a.rank[i]; return 0; });
        const best = {};
        for (const c of choices) for (const p of c.powers) {
            const i = this.powerInfo(p), k = i.family + "|" + i.difficulty, cur = best[k];
            if (!cur || i.tier > cur.tier || (i.tier === cur.tier && this.powerValue(p) > this.powerValue(cur.p))) best[k] = { p: p, tier: i.tier };
        }
        return (this._bo[itemId] = { choices: choices, strongest: Object.keys(best).map(function (k) { return best[k].p; }) });
    };
    // The strongest tier of the same bonus type as this power on this row (the power itself when it already is).
    ItemData.prototype.strongestOf = function (itemId, powerId) {
        const i = this.powerInfo(powerId);
        for (const p of this.bestOf(itemId).strongest) { const j = this.powerInfo(p); if (j.family === i.family && j.difficulty === i.difficulty) return p; }
        return powerId;
    };

    // Monopoly test of _AddLootItemPowers: a power clashes when one of its ItemBonusAttrMonopoly ids is an attribute of a
    // power added before it. Checked in both directions, so any insertion order of an accepted set is one the game makes.
    ItemData.prototype.clashes = function (a, b) {
        const pa = this.powers[a], pb = this.powers[b];
        const mono = (p) => this.d.monopoly[p.attrMonopoly] || [];
        const attrs = (p) => p.attrBonus.map(function (x) { return x[0]; });
        const ma = mono(pa), mb = mono(pb), aa = attrs(pa), ab = attrs(pb);
        return ma.some(function (x) { return ab.indexOf(x) >= 0; }) || mb.some(function (x) { return aa.indexOf(x) >= 0; });
    };

    // Is this power list (ids, in any order) one the original loot code can put on the row? Returns null or an error text.
    ItemData.prototype.checkPowers = function (itemId, powerIds) {
        const it = this.items[itemId];
        if (!it) return "unknown item";
        if (!this.canAdd(itemId)) return "the game never creates this item from loot";
        const n = powerIds.length;
        if (new Set(powerIds).size !== n) return "the same bonus twice";
        for (const src of it.sources) {
            const o = this.sourceOptions(src);
            const all = o.powers.length;
            if (!powerIds.every(function (p) { return o.powers.indexOf(p) >= 0; })) continue;
            // probQuantity >= list size: the whole list is added (no monopoly test).
            if (o.counts.some(function (c) { return c >= all; }) && n === all) return null;
            if (o.counts.indexOf(n) < 0 || n >= all) continue;
            let ok = true;
            for (let i = 0; i < n && ok; i++) for (let j = i + 1; j < n && ok; j++) if (this.clashes(powerIds[i], powerIds[j])) ok = false;
            if (ok) return null;
        }
        return "this combination of " + n + " bonus(es) cannot drop on this item in the original game";
    };

    // Class preferences for the bonuses pre-ticked when an item is added (a suggestion the owner can change).
    // Bonus families (power key without tier/difficulty), most wanted first; every other family follows by its value.
    const CLASS_PREFS = {
        Knight: ["Strength", "Endurance", "HP", "Damage", "Armor", "DR", "AllStats", "Block", "ResistAll", "LifeLeech"],
        Rogue: ["Dexterity", "Critical", "Damage", "AR", "FireDamage", "AirDamage", "ElectricalDamage", "WaterDamage", "EarthDamage", "Dodge", "AllStats"],
        Mage: ["Energy", "MP", "MPRegen", "FireDamage", "AirDamage", "ElectricalDamage", "WaterDamage", "EarthDamage", "Critical", "SpellBonusDamageAll", "ItemPowerSpellCritical"],
    };
    ItemData.CLASS_PREFS = CLASS_PREFS;
    ItemData.prototype.classKind = function (cls) { const m = /^(Knight|Rogue|Mage)PlayerBase/.exec(cls || ""); return m ? m[1] : null; };
    // Bonuses for a new item of this hero class: the strongest tier of each bonus, as many as the item's loot rules allow
    // (largest valid count over the loot sources), taken in the class's preference order, then by value. Always a set that
    // checkPowers accepts (or [] when the item has no bonuses).
    ItemData.prototype.classPick = function (itemId, cls) {
        const prefs = CLASS_PREFS[this.classKind(cls)] || [];
        const self = this, best = this.bestOf(itemId);
        const rank = (p) => { const i = prefs.indexOf(self.powerInfo(p).family); return i < 0 ? prefs.length : i; };
        const order = best.strongest.slice().sort((a, b) => rank(a) - rank(b) || self.powerValue(b) - self.powerValue(a) || a - b);
        let out = [];
        for (const c of best.choices) {
            const max = Math.max.apply(null, c.counts.map((n) => Math.min(n, c.powers.length)));
            if (max <= out.length) continue;
            const cand = order.filter((p) => c.powers.indexOf(p) >= 0);
            const pick = [];
            for (const p of cand) if (pick.length < max && pick.every((q) => !self.clashes(p, q))) pick.push(p);
            for (let n = pick.length; n > out.length; n--) {
                const set = pick.slice(0, n);
                if (this.checkPowers(itemId, set) === null) { out = set; break; }
            }
        }
        return out;
    };

    // ItemInstance::AddPower insertion order (ids swap, the SortingOrder values stay where they were pushed).
    ItemData.prototype.orderPowers = function (powerIds) {
        const ids = [], vals = [];
        for (const p of powerIds) {
            ids.push(p); vals.push(this.powers[p].sortingOrder);
            const count = ids.length;
            if (count !== 1 && vals[count - 2] > vals[count - 1]) {
                let k = count - 1;
                for (;;) {
                    const t = ids[k]; ids[k] = ids[k - 1]; ids[k - 1] = t;
                    if (k - 1 === 0) break;
                    --k;
                    if (!(vals[k - 1] > vals[k])) break;
                }
            }
        }
        return ids;
    };

    // A new inventory entry exactly as AddLoot + __SaveInventory would write it (not equipped, flag 1 from the constructor).
    ItemData.prototype.newEntry = function (itemId, powerIds, qty) {
        const ordered = this.orderPowers(powerIds);
        return { key: this.items[itemId].key, slot0: -1, slot1: -1, qty: qty || 1, value: this.lootValue(itemId, ordered), flag: 1,
                 powers: ordered.map((p) => this.powers[p].key) };
    };

    // An existing inventory entry with new bonuses: the same item row, slots (equipped state), quantity and pick-up flag;
    // the powers in AddPower order and the value the loot code would store for that row + powers (CalcLootItemValue).
    // Throws when the combination is not one the game's loot code can make on this row.
    ItemData.prototype.editEntry = function (entry, powerIds) {
        const id = this.itemByKey[entry.key];
        if (id === undefined) throw new Error("unknown item " + entry.key);
        const err = this.checkPowers(id, powerIds);
        if (err) throw new Error(err);
        // an unchanged bonus set keeps the order the game stored (AddPower's order depends on the drop's insertion order)
        const same = powerIds.length === entry.powers.length && powerIds.every((p) => entry.powers.indexOf(this.powers[p].key) >= 0);
        const ordered = same ? entry.powers.map((k) => this.powerByKey[k]) : this.orderPowers(powerIds);
        return { key: entry.key, slot0: entry.slot0, slot1: entry.slot1, qty: entry.qty, value: this.lootValue(id, ordered), flag: entry.flag,
                 powers: ordered.map((p) => this.powers[p].key) };
    };

    // ---------------------------------------------------------------- edits on a Save
    // The potion stack: the first item of type 14 (ItemInventory::_AddItemInstance keeps it as m_potion).
    function potionIndex(save, data) {
        return save.gear.items.findIndex(function (it) { const id = data.itemByKey[it.key]; return id !== undefined && data.items[id].type === 14; });
    }
    // Highest potion count the editor writes: whatever the save's int32 quantity field holds (the owner's choice: no game
    // cap). The game's own pick-up cap is Special_Loot_Max_Potions (property 194), computed at run time from skills.
    function potionCap() { return MAX_POTIONS; }
    function setPotions(save, data, n, cap) {
        if (cap === undefined) cap = MAX_POTIONS;
        n = Math.trunc(Number(n));
        if (!(n >= 0 && n <= cap)) throw new Error("potions must be a whole number 0.." + cap);
        const i = potionIndex(save, data);
        if (i >= 0) {
            if (n === 0) save.gear.items.splice(i, 1);   // SetPotionQty(0) deletes the stack
            else save.gear.items[i].qty = n;
        } else if (n > 0) {
            const id = data.itemByKey.Potion0;           // SetPotionQty: a new "Potion0" instance, value from CalcLootItemValue
            save.gear.items.push({ key: "Potion0", slot0: -1, slot1: -1, qty: n, value: data.lootValue(id, []), flag: 1, powers: [] });
        }
    }
    function setGold(save, n) {
        n = Math.trunc(n);
        if (!(n >= 0 && n <= MAX_GOLD)) throw new Error("gold must be 0.." + MAX_GOLD);
        save.gear.gold = n;
    }

    return {
        MAX_GOLD: MAX_GOLD, DEFAULT_POTION_CAP: DEFAULT_POTION_CAP, MAX_POTIONS: MAX_POTIONS, MAX_ITEMS: 100, parseProps: parseProps,
        parseContainer: parseContainer, writeContainer: writeContainer, parseGear: parseGear, writeGear: writeGear, replaceGear: replaceGear,
        Save: Save, ItemData: ItemData, potionIndex: potionIndex, potionCap: potionCap, setPotions: setPotions, setGold: setGold,
    };
});
