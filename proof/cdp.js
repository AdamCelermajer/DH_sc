// Minimal Chrome DevTools Protocol client (node >= 22: global WebSocket, fetch) used by the save editor's automated UI checks.
// usage in a script: const cdp = await require("./cdp").connect(port); await cdp.eval("1+1"); await cdp.shot("x.png");
"use strict";
const fs = require("fs");

async function connect(port, urlPart) {
    let targets = [];
    for (let i = 0; i < 50; i++) {
        try { targets = await (await fetch("http://127.0.0.1:" + port + "/json")).json(); } catch (e) { targets = []; }
        const t = targets.find((x) => x.type === "page" && (!urlPart || x.url.indexOf(urlPart) >= 0));
        if (t) return open(t.webSocketDebuggerUrl);
        await new Promise((r) => setTimeout(r, 300));
    }
    throw new Error("no page target on port " + port);
}

function open(wsUrl) {
    return new Promise((resolve, reject) => {
        const ws = new WebSocket(wsUrl);
        let id = 0;
        const pending = new Map();
        ws.onmessage = (m) => {
            const msg = JSON.parse(m.data);
            if (msg.id && pending.has(msg.id)) { const p = pending.get(msg.id); pending.delete(msg.id); msg.error ? p.ko(new Error(msg.error.message)) : p.ok(msg.result); }
        };
        ws.onerror = reject;
        ws.onopen = () => {
            const send = (method, params) => new Promise((ok, ko) => { id++; pending.set(id, { ok, ko }); ws.send(JSON.stringify({ id, method, params: params || {} })); });
            const api = {
                send,
                eval: async (expr) => {
                    const r = await send("Runtime.evaluate", { expression: expr, awaitPromise: true, returnByValue: true });
                    if (r.exceptionDetails) throw new Error(r.exceptionDetails.exception ? r.exceptionDetails.exception.description : r.exceptionDetails.text);
                    return r.result.value;
                },
                shot: async (file) => { const r = await send("Page.captureScreenshot", { format: "png", captureBeyondViewport: false }); fs.writeFileSync(file, Buffer.from(r.data, "base64")); },
                size: (w, h, mobile) => send("Emulation.setDeviceMetricsOverride", { width: w, height: h, deviceScaleFactor: 1, mobile: !!mobile }),
                close: () => ws.close(),
            };
            resolve(api);
        };
    });
}
module.exports = { connect };
