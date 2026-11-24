// Timing of the editor UI on a DevTools target (headless Edge on the Windows app, or the editor WebView on the emulator):
// inventory grouping modes, opening every slot group, the catalog, a group of the catalog, typing in the catalog search.
// Each number = wall time from the action to the next painted frame (double requestAnimationFrame), ms, median of N runs.
// usage: node perf.js <devtools port> <url part> [folder to open | "android"] [label]
"use strict";
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

(async () => {
    const [port, urlPart, folder, label] = process.argv.slice(2);
    const c = await connect(+port, urlPart);
    if (folder !== "android") await c.size(1000, 1100, false);
    await sleep(500);
    for (let i = 0; i < 10 && /^chrome-error/.test(await c.eval("location.href")); i++) { await c.send("Page.reload"); await sleep(1500); }
    if (folder && folder !== "android") {
        await c.waitFor("document.querySelector('#sources input')");
        await c.eval(`(() => { const row = document.querySelectorAll('#sources .row')[0]; row.querySelector('input').value = ${JSON.stringify(folder)}; row.querySelector('button').click(); })()`);
    }
    await c.waitFor("document.querySelector('#savelist .item')", 30000);
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await c.waitFor("DH2App.state.save");
    await sleep(500);
    const ua = await c.eval("navigator.userAgent + ' | ' + innerWidth + 'x' + innerHeight + ' dpr ' + devicePixelRatio");
    console.log((label || "") + " " + ua);
    const nav = await c.eval(`(() => { const n = performance.getEntriesByType('navigation')[0]; return n ? Math.round(n.domContentLoadedEventEnd) + ' ms to DOMContentLoaded (scripts parsed + first render), ' + Math.round(n.loadEventEnd) + ' ms to load' : 'n/a'; })()`);
    console.log("  page load: " + nav);
    // run `code` and wait for the next painted frame; returns ms
    // [ms to the next painted frame, ms of script + style + layout (forced layout right after the action)]
    const timed = (code) => c.eval(`new Promise((done) => { const t0 = performance.now(); (() => { ${code} })(); void document.body.offsetHeight; const cpu = performance.now() - t0;
        requestAnimationFrame(() => requestAnimationFrame(() => done([Math.round((performance.now() - t0) * 10) / 10, Math.round(cpu * 10) / 10]))); })`);
    const med = async (name, code, n, prep) => {
        const ts = [];
        for (let i = 0; i < (n || 5); i++) { if (prep) await c.eval("(() => { " + prep + " })()"); await sleep(60); ts.push(await timed(code)); }
        const fr = ts.map((t) => t[0]).sort((a, b) => a - b), cpu = ts.map((t) => t[1]).sort((a, b) => a - b);
        const m = [fr[Math.floor(fr.length / 2)], cpu[Math.floor(cpu.length / 2)]];
        console.log("  " + name.padEnd(44) + ("frame " + m[0]).padStart(12) + ("  cpu " + m[1]).padStart(11) + " ms   (cpu runs " + ts.map((t) => t[1]).join(" ") + ")");
        return m;
    };
    const setGroup = (v) => `const g = document.getElementById('group'); g.value = '${v}'; g.onchange();`;
    const closeAll = `document.querySelectorAll('details').forEach((d) => { d.open = false; });`;
    const res = {};
    await c.eval(`document.getElementById('tab-inv').click()`);
    res.invSet = await med("inventory grouped by set", setGroup("set"), 5, setGroup("none"));
    res.invType = await med("inventory grouped by slot / type", setGroup("type"), 5, setGroup("none"));
    res.invNone = await med("inventory, no groups", setGroup("none"), 5, setGroup("type"));
    // open every slot group one after the other (as the user does), total time
    await c.eval("(() => { " + setGroup("type") + " })()");
    res.invOpenAll = await med("inventory by slot: open every group", `document.querySelectorAll('#inv details').forEach((d) => { if (DH2App.openGroup) DH2App.openGroup(d); else { d.open = true; if (d.ontoggle) d.ontoggle(); } });`, 5, "DH2App.state.open = {}; " + setGroup("type"));
    await c.eval("(() => { " + setGroup("type") + " })()");
    const groups = await c.eval(`[...document.querySelectorAll('#inv details')].map((d) => d.querySelector('summary').textContent.trim().slice(0, 40))`);
    console.log("  inventory groups: " + groups.join(" | "));
    res.invOpenOne = await med("inventory by slot: open the biggest group", `const ds = [...document.querySelectorAll('#inv details')]; const d = ds.sort((a, b) => b.querySelectorAll('.item').length - a.querySelectorAll('.item').length)[0] || ds[0]; if (DH2App.openGroup) DH2App.openGroup(d); else { d.open = true; if (d.ontoggle) d.ontoggle(); }`, 5, "DH2App.state.open = {}; " + setGroup("type"));
    // catalog
    await c.eval(`(() => { document.getElementById("tab-inv").click(); ${setGroup("type")} })()`);
    res.catOpenType = await med("catalog tab open (by slot / type)", `document.getElementById('tab-add').click();`, 5, `document.getElementById('tab-inv').click();`);
    res.catSet = await med("catalog grouped by set", setGroup("set"), 5, setGroup("type"));
    res.catType = await med("catalog grouped by slot / type", setGroup("type"), 5, setGroup("set"));
    await c.eval("(() => { " + setGroup("type") + " })()");
    res.catOpenBig = await med("catalog by slot: open 'One-hand weapons'", `const d = [...document.querySelectorAll('#catalog details')].find((x) => /One-hand/.test(x.textContent)); if (DH2App.openGroup) DH2App.openGroup(d); else { d.open = true; if (d.ontoggle) d.ontoggle(); }`, 5, "DH2App.state.open = {}; " + setGroup("type"));
    res.catNone = await med("catalog, no groups (first page)", setGroup("none"), 5, setGroup("type"));
    await c.eval("(() => { " + setGroup("type") + " })()");
    const typeKey = (q) => `const i = document.getElementById('q'); i.value = ${JSON.stringify(q)}; i.oninput();`;
    const keys = [];
    for (const q of ["s", "so", "sol", "sold", "soldi"]) { await sleep(80); keys.push(await timed(typeKey(q))); }
    console.log("  search keystrokes s,so,sol,sold,soldi (cpu)".padEnd(48) + keys.map((k) => String(k[1])).join(" ") + " ms");
    res.keyMax = Math.max.apply(null, keys.map((k) => k[1]));
    // the whole word typed quickly, time until the list is up to date
    await c.eval("(() => { " + typeKey("") + " })()");
    await sleep(400);
    res.typeWord = await c.eval(`new Promise((done) => { const t0 = performance.now(); const i = document.getElementById('q'); let k = 0; const word = 'marauder';
        const step = () => { k++; i.value = word.slice(0, k); i.oninput(); if (k < word.length) setTimeout(step, 30); else {
            const wait = () => { if (document.querySelectorAll('#catalog .item, #catalog details').length && !document.querySelector('#catalog .count, #catalog .muted').textContent.startsWith('0')) requestAnimationFrame(() => done(Math.round(performance.now() - t0))); else requestAnimationFrame(wait); }; wait(); } }; step(); })`);
    console.log("  typing 'marauder' (8 keys, 30 ms apart) -> list ready".padEnd(48) + String(res.typeWord).padStart(8) + " ms");
    await c.eval("(() => { " + typeKey("") + " })()");
    res.sheet = await med("open an item sheet (equipped armour)", `const its = DH2App.state.save.gear.items; const i = its.findIndex((x) => x.slot0 >= 0); if (DH2App.showItem) DH2App.showItem(i); else { document.getElementById('tab-inv').click(); const g = document.getElementById('group'); g.value = 'none'; g.onchange(); document.querySelectorAll('#inv .item')[i].click(); }`, 3, `const s = document.getElementById('sheet'); s.classList.add('hidden');`);
    await c.eval(`document.getElementById('sheet').classList.add('hidden')`);
    res.builder = await med("open the bonus picker (Marauder Armor)", `DH2App.builder(DH2App.data.itemByKey.PlateArmor03);`, 3, `document.getElementById('sheet').classList.add('hidden');`);
    await c.eval(`document.getElementById('sheet').classList.add('hidden')`);
    console.log("RESULT " + JSON.stringify(res));
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
