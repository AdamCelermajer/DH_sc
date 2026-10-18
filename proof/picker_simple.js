// Screens of the simple bonus picker: adding an item and editing an item's bonuses (and the Advanced all-tiers toggle).
// usage: node picker_simple.js <devtools port> <url part> <shots dir> <prefix> [folder to open | "android"] [w h]
"use strict";
const path = require("path");
const fs = require("fs");
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
(async () => {
    const [port, urlPart, shots, prefix, folder, w, h] = process.argv.slice(2);
    fs.mkdirSync(shots, { recursive: true });
    const c = await connect(+port, urlPart);
    if (w) await c.size(+w, +h, true);
    await sleep(800);
    const shot = async (n) => { await sleep(500); await c.shot(path.join(shots, prefix + n + ".png")); console.log("shot " + prefix + n); };
    if (folder && folder !== "android") {
        await c.waitFor("document.querySelector('#sources input')");
        await c.eval(`(() => { const row = document.querySelectorAll('#sources .row')[0]; row.querySelector('input').value = ${JSON.stringify(folder)}; row.querySelector('button').click(); })()`);
    }
    await c.waitFor("document.querySelector('#savelist .item')", 30000);
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await c.waitFor("DH2App.state.save");
    // adding: a Marauder armour piece (Plate armour, strongest pool picked by the editor)
    await c.eval(`DH2App.builder(DH2App.data.itemByKey.PlateArmor03)`);
    await sleep(600);
    await c.eval(`document.querySelector('[data-p]').click()`);
    await sleep(400);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("add-1-top");
    await c.eval(`document.querySelector('.pcat:nth-child(3)') && document.querySelector('.pcat:nth-child(3)').scrollIntoView()`);
    await shot("add-2-more");
    await c.eval(`(() => { const s = document.getElementById('psearch'); s.value = 'strength'; s.oninput(); document.getElementById('sheetbody').scrollTop = 0; })()`);
    await shot("add-3-search-strength");
    await c.eval(`document.getElementById('close').click()`);
    // editing: the first equipped item that has bonuses
    await c.eval(`(() => { const i = DH2App.state.save.gear.items.findIndex((e) => (e.slot0 >= 0 || e.slot1 >= 0) && e.powers.length > 1); window.__ei = i; DH2App.builder(DH2App.data.itemByKey[DH2App.state.save.gear.items[i].key], i); })()`);
    await sleep(600);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("edit-1-top");
    await c.eval(`document.querySelector('.alltiers summary').click(); document.getElementById('alltiers').click()`);
    await sleep(500);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("edit-2-advanced-all-tiers");
    console.log(await c.eval(`document.querySelector('.detail .muted').textContent`));
    await c.eval(`document.getElementById('close').click()`);
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
