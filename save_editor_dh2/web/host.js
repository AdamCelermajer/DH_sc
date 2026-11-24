// Platform layer of the save editor UI. The same UI runs
//  * on Android inside the game APK (SaveEditorActivity, a WebView): the Java object window.AndroidHost reads and writes the
//    game's own files directory, keeps backups and refuses to write while the game process runs;
//  * on Windows (dh2-save-editor.exe): a small local web server answers /api/* (folder files, or the phone through adb).
// Every function returns a Promise. Paths are opaque strings handed back by listSaves.
(function () {
    "use strict";
    function b64ToBytes(s) { const bin = atob(s); const u = new Uint8Array(bin.length); for (let i = 0; i < bin.length; i++) u[i] = bin.charCodeAt(i); return u; }
    function bytesToB64(u) { let s = ""; for (let i = 0; i < u.length; i += 0x8000) s += String.fromCharCode.apply(null, u.subarray(i, i + 0x8000)); return btoa(s); }

    let host;
    if (window.AndroidHost) {
        const A = window.AndroidHost;
        const call = (fn) => new Promise((ok, ko) => { try { const r = JSON.parse(fn()); if (r.error) ko(new Error(r.error)); else ok(r); } catch (e) { ko(e); } });
        host = {
            kind: "android",
            note: "Saves of the game on this phone. Close the game before saving (swipe it away from recent apps).",
            locations: () => Promise.resolve([{ id: "game", label: "Game saves on this phone" }]),
            listSaves: () => call(() => A.listSaves()).then((r) => r.files),
            read: (p) => call(() => A.read(p)).then((r) => b64ToBytes(r.data)),
            write: (p, bytes) => call(() => A.write(p, bytesToB64(bytes))),
            launchGame: () => call(() => A.launchGame()),
            gameRunning: () => call(() => A.gameRunning()).then((r) => r.running),
            stopGame: () => call(() => A.stopGame()),
        };
    } else if (location.protocol === "http:" && /^(127\.0\.0\.1|localhost)/.test(location.host)) {
        // dh2-save-editor.exe puts a per-run token in the URL fragment; every API call must carry it.
        const m = /[#&]t=([a-z0-9]+)/.exec(location.hash);
        if (m) sessionStorage.setItem("dh2token", m[1]);
        const token = sessionStorage.getItem("dh2token") || "";
        const api = (path, body) => fetch("/api/" + path, body === undefined ? { headers: { "X-DH2-Token": token } } : { method: "POST", body: body, headers: { "X-DH2-Token": token } })
            .then((r) => r.json()).then((r) => { if (r.error) throw new Error(r.error); return r; });
        const q = encodeURIComponent;
        host = {
            kind: "windows",
            note: "Open a save from a folder on this PC, or from the phone over USB (adb, USB debugging on). Every save keeps a backup first.",
            locations: () => api("locations").then((r) => r.locations),
            listSaves: (loc, arg) => api("list?loc=" + q(loc) + "&arg=" + q(arg || "")).then((r) => r.files),
            read: (p) => api("read?path=" + q(p)).then((r) => b64ToBytes(r.data)),
            write: (p, bytes) => api("write?path=" + q(p), bytesToB64(bytes)),
            browse: () => api("browse").then((r) => r.dir),
        };
    } else {
        // Plain browser: open a file with the file picker, save by download.
        host = {
            kind: "browser",
            note: "Pick a dh2_NNN.savegame file. Saving downloads the edited file; copy it back yourself (keep a copy of the original).",
            locations: () => Promise.resolve([{ id: "pick", label: "Pick a file" }]),
            listSaves: () => new Promise((ok) => {
                const i = document.createElement("input"); i.type = "file";
                i.onchange = () => { const f = i.files[0]; if (!f) return ok([]); f.arrayBuffer().then((b) => { host._picked = new Uint8Array(b); ok([{ name: f.name, path: "picked:" + f.name }]); }); };
                i.click();
            }),
            read: () => Promise.resolve(host._picked),
            write: (p, bytes) => {
                const a = document.createElement("a"); a.href = URL.createObjectURL(new Blob([bytes])); a.download = p.replace(/^picked:/, ""); a.click();
                return Promise.resolve({ ok: true, note: "downloaded" });
            },
        };
    }
    window.DH2Host = host;
})();
