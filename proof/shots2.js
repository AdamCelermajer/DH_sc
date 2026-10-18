// Screens of the editor UI (equipped-first inventory per slot and per set, the catalog, an item sheet, the bonus picker) on a
// DevTools target: headless Edge on the Windows app, or the editor's WebView on the emulator.
// usage: node shots2.js <devtools port> <url part> <shots dir> <prefix> [folder to open | "android"] [w h]
"use strict";
const path = require("path");
const fs = require("fs");
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

(async () => {
    const [port, urlPart, shots, prefix, folder, w, h] = process.argv.slice(2);
    fs.mkdirSync(shots, { recursive: true });
    const c = await connect(+port, urlPart);
    if (w) await c.size(+w, +h, false);
    await sleep(600);
    for (let i = 0; i < 10 && /^chrome-error/.test(await c.eval("location.href")); i++) { await c.send("Page.reload"); await sleep(1500); }
    const shot = async (n) => { await sleep(450); await c.shot(path.join(shots, prefix + n + ".png")); console.log("shot " + prefix + n); };
    if (folder && folder !== "android") {
        await c.waitFor("document.querySelector('#sources input')");
        await c.eval(`(() => { const row = document.querySelectorAll('#sources .row')[0]; row.querySelector('input').value = ${JSON.stringify(folder)}; row.querySelector('button').click(); })()`);
    }
    await c.waitFor("document.querySelector('#savelist .item')", 30000);
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await c.waitFor("DH2App.state.save");
    const setGroup = (v) => c.eval(`(() => { const g = document.getElementById('group'); g.value = '${v}'; g.onchange(); window.scrollTo(0, 0); })()`);
    await c.eval(`document.getElementById('tab-inv').click()`);
    await setGroup("type");
    await shot("01-inventory-by-slot-equipped-first");
    await c.eval(`(() => { const ds = [...document.querySelectorAll('#inv details')]; const d = ds.find((x) => /Helms/.test(x.textContent)); DH2App.openGroup(d); d.scrollIntoView(); })()`);
    await shot("02-inventory-by-slot-helms-open");
    await setGroup("set");
    await shot("03-inventory-by-set");
    await setGroup("none");
    await shot("04-inventory-no-groups-equipped-then-bag");
    await c.eval(`(() => { const its = DH2App.state.save.gear.items; DH2App.showItem(its.findIndex((x) => x.slot0 === 0)); })()`);
    await shot("05-item-sheet-equipped-armour");
    await c.eval(`document.getElementById('close').click()`);
    await setGroup("type");
    await c.eval(`document.getElementById('tab-add').click()`);
    await c.eval(`window.scrollTo(0, 0)`);
    await shot("06-catalog-by-slot");
    await c.eval(`(() => { const d = [...document.querySelectorAll('#catalog details')].find((x) => /Two-hand/.test(x.textContent)); DH2App.openGroup(d); d.scrollIntoView(); })()`);
    await shot("07-catalog-two-hand-weapons-open");
    await c.eval(`(() => { const i = document.getElementById('q'); i.value = 'marauder'; i.onchange(); window.scrollTo(0, 0); })()`);
    await setGroup("set");
    await shot("08-catalog-search-marauder-by-set");
    await c.eval(`(() => { const i = document.getElementById('q'); i.value = ''; i.onchange(); })()`);
    await c.eval(`DH2App.builder(DH2App.data.itemByKey.PlateArmor03)`);
    await sleep(300);
    await c.eval(`(() => { const b = document.querySelector('[data-p]'); b.click(); })()`);
    await sleep(300);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("09-bonus-picker");
    await c.eval(`document.getElementById('close').click()`);
    await c.eval(`document.getElementById('tab-inv').click()`);
    await setGroup("type");
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
