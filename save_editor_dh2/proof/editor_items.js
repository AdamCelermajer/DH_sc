// Item-info check: open a save in the editor UI (served by dh2-save-editor.exe) and screenshot the detail sheet of the
// named inventory items, to compare with the game's own item screens.
// usage: node editor_items.js <devtools port> <folder> <shots dir> <item key,item key,...> [language index]
"use strict";
const path = require("path");
const fs = require("fs");
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

(async () => {
    const [port, folder, shots, keys, lang] = process.argv.slice(2);
    fs.mkdirSync(shots, { recursive: true });
    const c = await connect(+port, "127.0.0.1");
    await c.size(700, 1000, true);
    await sleep(800);
    if (lang) await c.eval(`(() => { const s = document.getElementById('lang'); s.value = ${JSON.stringify(lang)}; s.onchange(); })()`);
    await c.waitFor("document.querySelector('#sources input')");
    await c.eval(`(() => { const inp = document.querySelector('#sources input'); inp.value = ${JSON.stringify(folder)}; document.querySelector('#sources button').click(); })()`);
    await sleep(1200);
    await c.waitFor("document.querySelector('#savelist .item')");
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await sleep(1000);
    await c.shot(path.join(shots, "ed-00-inventory.png"));
    let n = 1;
    for (const k of keys.split(",")) {
        const info = await c.eval(`(() => { const i = DH2App.state.save.gear.items.findIndex(x => x.key === ${JSON.stringify(k)}); if (i < 0) return null;
            document.querySelectorAll('#inv .item')[i].click(); return document.getElementById('sheetbody').innerText; })()`);
        if (info === null) { console.log("missing " + k); continue; }
        await sleep(300);
        const f = "ed-" + String(n++).padStart(2, "0") + "-" + k + ".png";
        await c.shot(path.join(shots, f));
        console.log("=== " + f + "\n" + info.split("\n").slice(0, 14).join(" | "));
        await c.eval(`document.getElementById('close').click()`);
    }
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
