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
    const DEFAULT_POTION_CAP = 5;          // a new hero's starting stack (BasePlayerInventory); see potionCap()

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
    ItemData.prototype.sourceOptions = function (src) {
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

    // ---------------------------------------------------------------- edits on a Save
    // The potion stack: the first item of type 14 (ItemInventory::_AddItemInstance keeps it as m_potion).
    function potionIndex(save, data) {
        return save.gear.items.findIndex(function (it) { const id = data.itemByKey[it.key]; return id !== undefined && data.items[id].type === 14; });
    }
    // Highest potion count the editor writes: a fresh hero's stack (5) or the count the save already has, whichever is
    // larger. The real cap is Special_Loot_Max_Potions (property 194), computed at run time from skills by Lua scripts.
    function potionCap(save, data) {
        const i = potionIndex(save, data);
        return Math.max(DEFAULT_POTION_CAP, i >= 0 ? save.gear.items[i].qty : 0);
    }
    function setPotions(save, data, n, cap) {
        n = Math.trunc(n);
        if (!(n >= 0 && n <= cap)) throw new Error("potions must be 0.." + cap);
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
        MAX_GOLD: MAX_GOLD, DEFAULT_POTION_CAP: DEFAULT_POTION_CAP, MAX_ITEMS: 100,
        parseContainer: parseContainer, writeContainer: writeContainer, parseGear: parseGear, writeGear: writeGear, replaceGear: replaceGear,
        Save: Save, ItemData: ItemData, potionIndex: potionIndex, potionCap: potionCap, setPotions: setPotions, setGold: setGold,
    };
});
