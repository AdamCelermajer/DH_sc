// Windows flow check: the editor UI served by dh2-save-editor.exe, driven through Edge's DevTools port like a user would:
// open a folder, open the save, look at items, change gold and potions, add an item with bonuses, save. Screenshots go to <shots>.
// usage: node windows_flow.js <devtools port> <folder with dh2_000.savegame> <shots dir> [itemKey powerKey,powerKey,...]
"use strict";
const path = require("path");
const fs = require("fs");
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

(async () => {
    const [port, folder, shots, itemKey, powerKeys] = process.argv.slice(2);
    fs.mkdirSync(shots, { recursive: true });
    const c = await connect(+port, "127.0.0.1");
    await c.size(1000, 1100, false);
    await sleep(800);
    const shot = async (n) => { await sleep(400); await c.shot(path.join(shots, n)); console.log("shot " + n); };
    await shot("win-01-start.png");
    // folder location: type the path and press "Open folder"
    await c.eval(`(() => { const inp = document.querySelector('#sources input'); inp.value = ${JSON.stringify(folder)}; document.querySelector('#sources button').click(); })()`);
    await sleep(1200);
    await shot("win-02-list.png");
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await sleep(1000);
    await shot("win-03-opened.png");
    const before = await c.eval(`({gold: DH2App.state.save.gear.gold, items: DH2App.state.save.gear.items.length})`);
    console.log("opened", JSON.stringify(before));
    // details of the first item with bonuses (or the first item)
    await c.eval(`(() => { const its = DH2App.state.save.gear.items; let i = its.findIndex(x => x.powers.length); if (i < 0) i = 0; document.querySelectorAll('#inv .item')[i].click(); })()`);
    await shot("win-04-item-detail.png");
    await c.eval(`document.getElementById('close').click()`);
    // gold +5000 with the button, potions to 3
    await c.eval(`document.querySelector('[data-add]').click()`);
    await c.eval(`(() => { const p = document.getElementById('potions'); p.value = 3; p.onchange(); })()`);
    // add an item through the catalog + builder
    await c.eval(`document.getElementById('tab-add').click()`);
    if (itemKey) {
        await c.eval(`(() => { const q = document.getElementById('q'); q.value = ${JSON.stringify(itemKey)}; q.oninput(); })()`);
        await sleep(300);
        await shot("win-05-catalog.png");
        await c.eval(`document.querySelector('#catalog .item').click()`);
        await sleep(300);
        // pick the loot rule that contains all wanted bonuses, then tick them
        const want = (powerKeys || "").split(",").filter(Boolean);
        await c.eval(`(() => {
            const want = ${JSON.stringify(want)}.map(k => DH2App.data.powerByKey[k]);
            const sel = document.getElementById('srcsel');
            if (sel) { for (let i = 0; i < sel.options.length; i++) { sel.value = i; sel.onchange();
                const ids = [...document.querySelectorAll('[data-p]')].map(x => +x.dataset.p);
                if (want.every(w => ids.includes(w))) break; } }
            for (const w of want) { const cb = document.querySelector('[data-p="' + w + '"]'); cb.checked = true; cb.onchange(); }
        })()`);
        await sleep(300);
        await shot("win-06-builder.png");
        const ok = await c.eval(`!document.getElementById('addit').disabled`);
        if (!ok) throw new Error("builder refused the item: " + await c.eval(`document.querySelector('.err') && document.querySelector('.err').textContent`));
        await c.eval(`document.getElementById('addit').click()`);
    }
    await c.eval(`document.getElementById('tab-inv').click()`);
    await shot("win-07-edited.png");
    await c.eval(`document.getElementById('save').click()`);
    await sleep(1500);
    await shot("win-08-saved.png");
    const after = await c.eval(`({gold: DH2App.state.save.gear.gold, items: DH2App.state.save.gear.items.length, dirty: DH2App.state.dirty, toast: document.getElementById('toast').textContent})`);
    console.log("saved", JSON.stringify(after));
    c.close();
    process.exit(after.dirty ? 1 : 0);
})().catch((e) => { console.error(e); process.exit(1); });
