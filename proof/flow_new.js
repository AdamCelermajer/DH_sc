// Screens of the simplified editor: home, inventory, add-item flow (search -> tap -> added toast), edit bonuses.
// usage: node flow_new.js <devtools port> <url part> <shots dir> <prefix> [folder to open | "android"] [w h] [adb serial: screenshots via adb screencap]
"use strict";
const path = require("path");
const fs = require("fs");
const { execFileSync } = require("child_process");
const { connect } = require("./cdp");
const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
(async () => {
    const [port, urlPart, shots, prefix, folder, w, h, serial] = process.argv.slice(2);
    fs.mkdirSync(shots, { recursive: true });
    const c = await connect(+port, urlPart);
    if (w && +w) await c.size(+w, +h, +w < 700);
    await sleep(800);
    const adb = process.env.ADB || "C:/Users/adamc/AppData/Local/Android/Sdk/platform-tools/adb.exe";
    const shot = async (n) => {
        await sleep(700);
        const f = path.join(shots, prefix + n + ".png");
        if (serial) { if (serial !== "emulator-5554") throw new Error("only emulator-5554"); fs.writeFileSync(f, execFileSync(adb, ["-s", serial, "exec-out", "screencap", "-p"], { maxBuffer: 1 << 28 })); }
        else await c.shot(f);
        console.log("shot " + prefix + n);
    };
    if (folder && folder !== "android") {
        await c.waitFor("document.querySelector('#sources input')");
        await c.eval(`(() => { const row = document.querySelectorAll('#sources .row')[0]; row.querySelector('input').value = ${JSON.stringify(folder)}; row.querySelector('button').click(); })()`);
    }
    await c.waitFor("document.querySelector('#savelist .item')", 30000);
    await c.eval(`document.querySelector('#savelist .item').click()`);
    await c.waitFor("DH2App.state.save");
    await sleep(600);
    await shot("1-home");
    await c.eval(`window.scrollTo(0, 420)`);
    await shot("2-inventory");
    // add flow: search -> tap -> added toast
    await c.eval(`document.getElementById('tab-add').click()`);
    await c.eval(`(() => { const q = document.getElementById('q'); q.value = 'PlateArmor03'; q.oninput(); })()`);
    await sleep(500);
    await c.eval(`window.scrollTo(0, 0)`);
    await shot("3-add-search");
    await c.eval(`document.querySelector('#catalog .item').click()`);
    await shot("4-added-toast");
    console.log(await c.eval(`document.getElementById('toast').textContent + ' | items ' + DH2App.state.save.gear.items.length + ' | last ' + JSON.stringify(DH2App.state.save.gear.items.slice(-1)[0])`));
    await c.eval(`document.getElementById('toastedit').click()`);
    await sleep(500);
    await c.eval(`document.getElementById('sheetbody').scrollTop = 0`);
    await shot("5-edit-bonuses");
    await c.eval(`document.getElementById('sheetbody').scrollTop = 600`);
    await shot("6-edit-bonuses-more");
    await c.eval(`document.getElementById('close').click()`);
    c.close();
})().catch((e) => { console.error(e); process.exit(1); });
