// Screens of the organised editor: inventory by set, catalog by set with numbers and "cannot use" marks, the bonus picker
// by category, and (Android, game running) the stop-the-game dialog. Works on any DevTools target: the Windows app in
// headless Edge, or the editor's WebView on the emulator (adb forward of webview_devtools_remote_<pid>).
// usage: node new_screens.js <devtools port> <url part> <shots dir> <prefix> [folder to open | "android"] [w h]
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
    for (let i = 0; i < 10 && /^chrome-error/.test(await c.eval("location.href")); i++) { await c.send("Page.reload"); await sleep(1500); }   // server not up yet
    const shot = async (n) => { await sleep(500); await c.shot(path.join(shots, prefix + n + ".png")); console.log("shot " + prefix + n); };
    if (folder && folder !== "android") {
        try { await c.waitFor("document.querySelector('#sources input')"); } catch (e) { console.log(await c.eval("document.getElementById('toast').textContent + ' | ' + document.getElementById('sources').innerHTML + ' | ' + location.href")); throw e; }
        await c.eval(`(() => { const row = document.querySelectorAll('#sources .row')[0]; row.querySelector('input').value = ${JSON.stringify(folder)}; row.querySelector('button').click(); })()`);
    }
    await c.waitFor("document.querySelector('#savelist .item')", 30000);
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await c.waitFor("DH2App.state.save");
    await c.eval(`(() => { const g = document.getElementById('group'); g.value = 'set'; g.onchange(); })()`);
    await sleep(500);
    await shot("01-inventory-by-set");
    // open the Marauder group of the inventory and scroll to it
    await c.eval(`(() => { const d = [...document.querySelectorAll('#inv details')].find((x) => /Elementalist/.test(x.textContent)); d.open = true; d.ontoggle(); d.scrollIntoView(); })()`);
    await shot("02-inventory-unusable-greyed");
    // catalog grouped by set; open Soldier, Marauder, Marksman
    await c.eval(`document.getElementById('tab-add').click()`);
    await sleep(800);
    await c.eval(`document.getElementById('tab-add').scrollIntoView()`);
    await shot("03-catalog-sets");
    await c.eval(`(() => { for (const d of document.querySelectorAll('#catalog details')) if (/^(Marauder|Marksman) set/.test(d.querySelector('summary b').textContent)) { d.open = true; d.ontoggle(); }
        [...document.querySelectorAll('#catalog details')].find((d) => d.open).scrollIntoView(); })()`);
    await shot("04-catalog-set-items-numbers");
    // slot grouping, two-hand weapons open
    await c.eval(`(() => { const g = document.getElementById('group'); g.value = 'type'; g.onchange(); const d = [...document.querySelectorAll('#catalog details')].find((x) => /Two-hand/.test(x.textContent)); d.open = true; d.ontoggle(); d.scrollIntoView(); })()`);
    await shot("05-catalog-by-slot-weapons");
    await c.eval(`(() => { const g = document.getElementById('group'); g.value = 'set'; g.onchange(); })()`);
    // the bonus picker of Marauder Armor (PlateArmor03)
    await c.eval(`DH2App.builder(DH2App.data.itemByKey.PlateArmor03)`);
    await sleep(500);
    await c.eval(`(() => { const b = document.querySelector('[data-p]'); b.click(); })()`);
    await sleep(300);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("06-picker-categories");
    await c.eval(`document.querySelector('.pcat:nth-child(3)') && document.querySelector('.pcat:nth-child(3)').scrollIntoView()`);
    await shot("07-picker-more");
    await c.eval(`(() => { const s = document.getElementById('psearch'); s.value = 'fire'; s.oninput(); document.getElementById('sheetbody').scrollTop = 0; })()`);
    await shot("08-picker-search-fire");
    await c.eval(`document.getElementById('close').click()`);
    // potions above 5
    await c.eval(`(() => { const p = document.getElementById('potions'); p.value = '250'; p.onchange(); document.getElementById('tab-inv').click(); window.scrollTo(0, 0); })()`);
    await shot("09-potions-250");
    if (folder === "android") {
        await c.eval(`document.getElementById('save').click()`);
        await sleep(1200);
        await shot("10-save-dialog");
    }
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
