// Checks of the save editor core (web/dh2save.js) on real files. usage: node tests/test_core.js <itemdata.json> <save files...>
//  1. open + save without changes = byte-identical file (container for every file, GEAR re-encoded for player saves);
//  2. every item and power name of the save resolves in the game's tables;
//  3. a sample edit (gold, potions, one item with powers from its loot rules) parses back to the same values.
"use strict";
const fs = require("fs");
const path = require("path");
const DH2 = require(path.join(__dirname, "..", "web", "dh2save.js"));

const data = new DH2.ItemData(fs.readFileSync(process.argv[2], "utf8"));
let fails = 0;
const fail = (m) => { console.log("FAIL " + m); fails++; };
const same = (a, b) => a.length === b.length && a.every((x, i) => x === b[i]);

for (const f of process.argv.slice(3)) {
    const bytes = new Uint8Array(fs.readFileSync(f));
    let c;
    try { c = DH2.parseContainer(bytes); } catch (e) { console.log("not a section file (" + e.message + "): " + path.basename(f)); continue; }

    if (!same(DH2.writeContainer(c), bytes)) fail(f + ": container round trip differs");
    let save;
    try { save = new DH2.Save(bytes); } catch (e) { console.log("container only (" + e.message.split(":")[0] + "): " + path.basename(f) + " ok, " + c.sections.map((s) => s.name).join(",")); continue; }
    const out = save.toBytes();
    if (!same(out, bytes)) fail(f + ": GEAR round trip differs"); else console.log("round trip identical: " + path.basename(f) + " (" + bytes.length + " bytes)");
    for (const it of save.gear.items) {
        const id = data.itemByKey[it.key];
        if (id === undefined) { fail("unknown item " + it.key); continue; }
        const pids = it.powers.map((p) => data.powerByKey[p]);
        if (pids.some((p) => p === undefined)) fail("unknown power in " + it.powers);
        const d = data.describe(id, pids, it.value);
        console.log("  " + [d.name, d.type, "lvl " + d.level, d.rarity.name, d.stats, d.reqs, d.powers.map((p) => p.text).join(" | "), "value " + it.value, "qty " + it.qty, "slots " + it.slot0 + "/" + it.slot1].join(" ; "));
    }
}

// sample edit on the first player save
const first = process.argv.slice(3).find((f) => { try { new DH2.Save(new Uint8Array(fs.readFileSync(f))); return true; } catch (e) { return false; } });
if (first) {
    const save = new DH2.Save(new Uint8Array(fs.readFileSync(first)));
    DH2.setGold(save, save.gear.gold + 5000);
    DH2.setPotions(save, data, 3, DH2.potionCap(save, data));
    // first addable armour row with a source that allows 3 powers
    let added = null;
    for (const it of data.items) {
        if (!data.canAdd(it.id) || it.type !== 7) continue;
        for (const src of it.sources) {
            const o = data.sourceOptions(src);
            if (o.counts.indexOf(3) < 0 || o.powers.length <= 3) continue;
            const pick = [];
            for (const p of o.powers) { if (pick.every((q) => !data.clashes(p, q))) pick.push(p); if (pick.length === 3) break; }
            if (pick.length === 3 && data.checkPowers(it.id, pick) === null) { added = data.newEntry(it.id, pick, 1); break; }
        }
        if (added) break;
    }
    if (!added) fail("no item with 3 powers found"); else save.gear.items.push(added);
    const back = new DH2.Save(save.toBytes());
    if (back.gear.gold !== save.gear.gold) fail("gold");
    if (back.gear.items.length !== save.gear.items.length) fail("item count");
    console.log("edit ok: gold " + back.gear.gold + ", potions " + back.gear.items[DH2.potionIndex(back, data)].qty + ", added " + JSON.stringify(back.gear.items[back.gear.items.length - 1]));
    if (data.checkPowers(data.itemByKey.Longsword01, [0, 1, 2, 3, 4]) === null) fail("an impossible 5-power sword was accepted");

    // potions: any non-negative int32 (no 0..5 cap), round trip through the bytes
    for (const n of [6, 250, 99999, DH2.MAX_POTIONS, 0, 7]) {
        const s2 = new DH2.Save(new Uint8Array(fs.readFileSync(first)));
        DH2.setPotions(s2, data, n);
        const b2 = new DH2.Save(s2.toBytes());
        const pi = DH2.potionIndex(b2, data);
        const got = pi >= 0 ? b2.gear.items[pi].qty : 0;
        if (got !== n) fail("potions " + n + " read back as " + got); else console.log("potions " + n + " round trip ok");
        if (n === 7 && !same(new DH2.Save(b2.toBytes()).toBytes(), b2.toBytes())) fail("potions re-save not identical");
    }
    for (const bad of [-1, DH2.MAX_POTIONS + 1, NaN]) {
        let threw = false; try { DH2.setPotions(save, data, bad); } catch (e) { threw = true; }
        if (!threw) fail("potions " + bad + " accepted");
    }

    // hero attributes and equip rules (compare with the game's Stats screen when the save is the owner's Warrior)
    const h = data.hero(save);
    console.log("hero " + data.heroClassLabel(h.cls) + " level " + h.level + " str/dex/end/ene " + h.attrs.join("/"));
    for (const it of save.gear.items) {
        const id = data.itemByKey[it.key];
        if ((it.slot0 >= 0 || it.slot1 >= 0) && data.cannotUse(id, h).length) fail("equipped " + it.key + " marked unusable: " + data.cannotUse(id, h));
    }
    if (h.cls === "KnightPlayerBase" && !data.cannotUse(data.itemByKey.Archer_LeatherArmor01, h).some((r) => /^class/.test(r))) fail("archer armour usable by a warrior");

    // sets and bonus categories
    const fam = data.setOf(data.itemByKey.PlateArmor02), mar = data.setOf(data.itemByKey.PlateBoots03), mk = data.setOf(data.itemByKey.Archer_LeatherArmor01);
    if (!fam || fam.name !== "Soldier" || !mar || mar.name !== "Marauder" || !mk || mk.name !== "Marksman" || !mk.game) fail("set names " + JSON.stringify([fam, mar, mk]));
    const other = data.powers.filter((p) => p.id > 0 && data.powerInfo(p.id).category === "Other").map((p) => p.key);
    if (other.length) fail("powers without a category: " + other.slice(0, 5));
    const pi = data.powerInfo(data.powerByKey.Strength_03_Hard);
    if (pi.category !== "Attributes" || pi.tier !== 3 || pi.difficulty !== "Hard") fail("powerInfo " + JSON.stringify(pi));
}
console.log(fails ? fails + " failure(s)" : "all checks passed");
process.exit(fails ? 1 : 0);
