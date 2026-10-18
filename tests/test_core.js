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

for (const f of process.argv.slice(3)) {
    const bytes = new Uint8Array(fs.readFileSync(f));
    let c;
    try { c = DH2.parseContainer(bytes); } catch (e) { console.log("not a section file (" + e.message + "): " + path.basename(f)); continue; }
    const same = (a, b) => a.length === b.length && a.every((x, i) => x === b[i]);
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
}
console.log(fails ? fails + " failure(s)" : "all checks passed");
process.exit(fails ? 1 : 0);
