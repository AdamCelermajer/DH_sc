// Save editor UI (shared by Android and Windows). Rules and formats live in dh2save.js; this file only shows and edits.
(function () {
    "use strict";
    const $ = (id) => document.getElementById(id);
    const esc = (s) => String(s == null ? "" : s).replace(/[&<>"]/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[c]));
    const text = (s) => esc(s).replace(/\n/g, "<br>");
    const host = window.DH2Host;
    const data = new DH2.ItemData(window.DH2_ITEMDATA);
    const st = { save: null, path: null, name: null, dirty: false, cap: 5 };

    // ------------------------------------------------------------ language
    const langSel = $("lang");
    data.languages().forEach((l) => { const o = document.createElement("option"); o.value = l.index; o.textContent = l.key + " - " + l.sample; langSel.appendChild(o); });
    langSel.value = localStorage.getItem("dh2lang") || "0";
    data.lang = +langSel.value;
    langSel.onchange = () => { data.lang = +langSel.value; localStorage.setItem("dh2lang", langSel.value); render(); };

    function toast(msg, ms) { const t = $("toast"); t.innerHTML = msg; t.classList.remove("hidden"); clearTimeout(toast.t); toast.t = setTimeout(() => t.classList.add("hidden"), ms || 3500); }
    function fail(e) { console.error(e); toast('<span class="err">' + esc(e.message || e) + "</span>", 6000); }

    // ------------------------------------------------------------ open
    function showFiles() {
        $("files").classList.remove("hidden"); $("editor").classList.add("hidden");
        $("hostnote").textContent = host.note;
        host.locations().then((locs) => {
            const box = $("sources"); box.innerHTML = "";
            locs.forEach((l) => {
                const row = document.createElement("div"); row.className = "row"; row.style.margin = "6px 0";
                const b = document.createElement("button"); b.textContent = l.label;
                let input = null;
                if (l.needsPath) {
                    input = document.createElement("input"); input.style.flex = "1"; input.value = localStorage.getItem("dh2arg-" + l.id) || l.default || ""; input.placeholder = l.placeholder || "";
                    row.appendChild(input);
                    if (host.browse && l.browse) { const br = document.createElement("button"); br.textContent = "Browse..."; br.onclick = () => host.browse().then((d) => { if (d) input.value = d; }).catch(fail); row.appendChild(br); }
                }
                b.onclick = () => {
                    const arg = input ? input.value : "";
                    if (input) localStorage.setItem("dh2arg-" + l.id, arg);
                    $("savelist").innerHTML = '<div class="muted">reading...</div>';
                    host.listSaves(l.id, arg).then(listSaves).catch((e) => { $("savelist").innerHTML = ""; fail(e); });
                };
                row.insertBefore(b, row.firstChild);
                box.appendChild(row);
            });
            if (host.kind === "android") {
                host.listSaves("game").then(listSaves).catch(fail);
                host.gameRunning().then((on) => { if (on) $("hostnote").innerHTML = '<span class="err">The game is running. You can look at the save, but saving needs the game closed.</span> ' + esc(host.note); }).catch(() => {});
            }
        }).catch(fail);
    }
    function listSaves(files) {
        const box = $("savelist");
        if (!files.length) { box.innerHTML = '<div class="muted">No dh2_NNN.savegame here.</div>'; return; }
        box.innerHTML = "";
        files.forEach((f) => {
            const d = document.createElement("div"); d.className = "item savefile";
            d.innerHTML = '<div class="grow"><div class="nm">' + esc(f.name) + '</div><div class="sub">' + esc(f.info || "") + "</div></div><button>Open</button>";
            d.onclick = () => open(f);
            box.appendChild(d);
        });
        if (files.length === 1 && host.kind === "browser") open(files[0]);
    }
    function open(f) {
        host.read(f.path).then((bytes) => {
            st.save = new DH2.Save(bytes); st.path = f.path; st.name = f.name; st.dirty = false; st.checkpoints = f.checkpoints || [];
            st.cap = DH2.potionCap(st.save, data);
            $("files").classList.add("hidden"); $("editor").classList.remove("hidden");
            $("fileinfo").textContent = f.name + (f.where ? " (" + f.where + ")" : "");
            render();
        }).catch(fail);
    }

    // ------------------------------------------------------------ editor
    function setDirty(v) { st.dirty = v; $("dirty").textContent = v ? "unsaved changes" : "no changes"; $("save").disabled = !v; $("revert").disabled = !v; }
    function render() {
        if (!st.save) return;
        const g = st.save.gear, info = st.save.info;
        $("hero").innerHTML = "Hero <b>" + esc(info.name || "?") + "</b> &middot; " + esc((info.cls || "").replace("PlayerBase", "").replace(/^_/, "")) +
            " &middot; level " + esc(info.level) + " &middot; " + g.items.length + " items (the game holds 100)";
        $("gold").value = g.gold;
        $("goldnote").textContent = "0 .. " + DH2.MAX_GOLD.toLocaleString() + " (the game's gold limit)";
        const pi = DH2.potionIndex(st.save, data);
        $("potions").value = pi >= 0 ? g.items[pi].qty : 0;
        $("potions").max = st.cap;
        $("potionnote").textContent = "0 .. " + st.cap + " (" + data.text(data.itemByKey.Potion0)[0] + "; a new hero starts with " + DH2.DEFAULT_POTION_CAP + ")";
        renderInventory();
        if (!$("add").classList.contains("hidden")) renderCatalog();
        setDirty(st.dirty);
    }

    function rarityDot(r) { return '<span class="dot" style="background:' + r.rgb + '" title="' + esc(r.name) + '"></span>'; }
    function itemRow(desc, extra, badges) {
        return rarityDot(desc.rarity) + '<div class="grow"><div class="nm" style="color:' + desc.rarity.rgb + '">' + esc(desc.name || desc.key) + "</div>" +
            '<div class="sub">' + esc(desc.type) + (desc.level ? " &middot; lvl " + desc.level : "") + " &middot; " + esc(desc.rarity.name) +
            (desc.powers.length ? " &middot; " + desc.powers.length + " bonus" + (desc.powers.length > 1 ? "es" : "") : "") + (extra || "") + "</div></div>" + (badges || "");
    }
    function slotLabel(s) { return s < 0 ? "" : (data.slotName[s] || ("slot " + s)); }
    function entryDesc(it) {
        const id = data.itemByKey[it.key];
        if (id === undefined) return null;
        return data.describe(id, it.powers.map((p) => data.powerByKey[p]), it.value);
    }

    function renderInventory() {
        const box = $("inv"); box.innerHTML = ""; const list = document.createElement("div"); list.className = "list";
        st.save.gear.items.forEach((it, idx) => {
            const d = entryDesc(it);
            const el = document.createElement("div"); el.className = "item";
            if (!d) { el.innerHTML = '<div class="grow err">unknown item ' + esc(it.key) + "</div>"; list.appendChild(el); return; }
            let badges = "";
            if (it.slot0 >= 0) badges += '<span class="badge eq">set 1: ' + esc(slotLabel(it.slot0)) + "</span>";
            if (it.slot1 >= 0) badges += '<span class="badge eq">set 2: ' + esc(slotLabel(it.slot1)) + "</span>";
            if (d.stackable || it.qty !== 1) badges += '<span class="badge">x' + it.qty + "</span>";
            el.innerHTML = itemRow(d, "", badges);
            el.onclick = () => showItem(idx);
            list.appendChild(el);
        });
        box.appendChild(list);
    }

    function detailTable(d, it) {
        const rows = [];
        const r = (k, v) => { if (v !== "" && v != null) rows.push("<tr><td>" + esc(k) + "</td><td>" + v + "</td></tr>"); };
        r("Type", esc(d.type) + (d.slotting && d.slotting !== "none" ? " (" + esc(d.slotting) + ")" : ""));
        r("Rarity", rarityDot(d.rarity) + " " + esc(d.rarity.name) + ' <span class="muted">(the game colours names by the number of bonuses: ' + d.rarity.powers + ")</span>");
        if (d.level) r("Level requirement", String(d.level));
        r("Stats", text(d.stats));
        r("Requirements", text(d.reqs));
        r("Difficulty row", esc(d.difficulty));
        if (d.set) r("Armour set", esc(d.set.name) + ' <span class="muted">(all five pieces: trophy ' + esc(d.set.trophy) + ")</span>");
        r("Value", String(d.value));
        r("Sell price", d.prices.sellMin === d.prices.sellMax ? String(d.prices.sellMin) : d.prices.sellMin + " - " + d.prices.sellMax +
            ' <span class="muted">(merchants buy at ' + d.prices.buyPctMin + "-" + d.prices.buyPctMax + "% of value)</span>");
        r("Reforge gold", d.prices.transmute + ' <span class="muted">(base; the game adds the hero's transmute bonus)</span>');
        if (it) {
            if (it.slot0 >= 0 || it.slot1 >= 0) r("Equipped", [it.slot0 >= 0 ? "set 1: " + slotLabel(it.slot0) : "", it.slot1 >= 0 ? "set 2: " + slotLabel(it.slot1) : ""].filter(Boolean).join(", "));
            r("Quantity", String(it.qty));
        }
        r("Internal name", '<span class="muted">' + esc(d.key) + "</span>");
        return "<table>" + rows.join("") + "</table>";
    }
    function powersHtml(d) {
        if (!d.powers.length) return '<div class="muted">No bonuses.</div>';
        return d.powers.map((p) => '<div class="power" style="color:' + d.rarity.rgb + '">' + text(p.text) +
            ' <div class="k">' + esc(p.key) + (p.attrs.length ? " &middot; " + p.attrs.map((a) => esc(a.attr) + " " + a.bonus).join(", ") : "") + "</div></div>").join("");
    }

    function showItem(idx) {
        const it = st.save.gear.items[idx]; const d = entryDesc(it);
        let h = '<div class="detail"><div class="nm" style="color:' + d.rarity.rgb + '">' + esc(d.name) + "</div>" + detailTable(d, it) + "<h2>Bonuses</h2>" + powersHtml(d);
        if (d.stackable && data.items[d.id].type !== 14) {
            h += '<div class="box"><label>Quantity</label><div class="row"><input id="qty" type="number" min="1" max="' + data.maxQty + '" value="' + it.qty + '"><button id="setqty">Set</button></div></div>';
        }
        if (data.items[d.id].type === 14) h += '<div class="muted">Potions are changed with the Potions box above.</div>';
        h += '<div class="row" style="margin-top:12px"><button id="close">Close</button>';
        if (data.items[d.id].type !== 14) h += '<button id="remove" class="danger">Remove from inventory</button>';
        h += "</div>";
        if (it.slot0 >= 0 || it.slot1 >= 0) h += '<div class="muted">This item is equipped. Removing it leaves the slot empty.</div>';
        h += "</div>";
        openSheet(h);
        $("close").onclick = closeSheet;
        if ($("remove")) $("remove").onclick = () => { st.save.gear.items.splice(idx, 1); closeSheet(); setDirty(true); render(); };
        if ($("setqty")) $("setqty").onclick = () => {
            const n = Math.trunc(+$("qty").value);
            if (!(n >= 1 && n <= data.maxQty)) return fail(new Error("quantity 1.." + data.maxQty));
            it.qty = n; closeSheet(); setDirty(true); render();
        };
    }
    function openSheet(h) { $("sheetbody").innerHTML = h; $("sheet").classList.remove("hidden"); $("sheetbody").scrollTop = 0; }
    function closeSheet() { $("sheet").classList.add("hidden"); }
    $("sheet").onclick = (e) => { if (e.target === $("sheet")) closeSheet(); };

    // ------------------------------------------------------------ gold / potions
    function numInput(id, apply) {
        $(id).onchange = () => { try { apply(+$(id).value); setDirty(true); render(); } catch (e) { fail(e); render(); } };
    }
    numInput("gold", (n) => DH2.setGold(st.save, n));
    numInput("potions", (n) => DH2.setPotions(st.save, data, n, st.cap));
    document.querySelectorAll("[data-add]").forEach((b) => b.onclick = () => {
        try { DH2.setGold(st.save, Math.min(DH2.MAX_GOLD, st.save.gear.gold + +b.dataset.add)); setDirty(true); render(); } catch (e) { fail(e); }
    });

    // ------------------------------------------------------------ catalog
    const types = {}; data.items.forEach((it) => { if (data.canAdd(it.id)) types[it.type] = data.typeName[it.type]; });
    Object.keys(types).sort((a, b) => a - b).forEach((t) => { const o = document.createElement("option"); o.value = t; o.textContent = types[t]; $("ftype").appendChild(o); });
    data.d.powerColors.slice(0, 5).forEach((c) => { const r = data.rarity(c.powers); const o = document.createElement("option"); o.value = c.powers; o.textContent = r.name + " (" + c.powers + " bonus" + (c.powers === 1 ? "" : "es") + ")"; $("frar").appendChild(o); });
    function maxCount(it) { let m = 0; it.sources.forEach((s) => { const o = data.sourceOptions(s); o.counts.forEach((c) => { m = Math.max(m, Math.min(c, o.powers.length)); }); }); return m; }
    function canHave(it, n) { return it.sources.some((s) => { const o = data.sourceOptions(s); return o.counts.some((c) => Math.min(c, o.powers.length) === n || (c >= o.powers.length && o.powers.length === n)) || (n === 0 && o.counts.indexOf(0) >= 0); }); }
    let catLimit = 60;
    function renderCatalog() {
        const q = $("q").value.trim().toLowerCase(), ft = $("ftype").value, fr = $("frar").value, fmin = $("fmin").value, fmax = $("fmax").value, fd = $("fdiff").value;
        const out = [];
        for (const it of data.items) {
            if (!data.canAdd(it.id)) continue;
            if (ft !== "" && String(it.type) !== ft) continue;
            if (fmin !== "" && it.levelReq < +fmin) continue;
            if (fmax !== "" && it.levelReq > +fmax) continue;
            const d = data.describe(it.id, [], data.lootValue(it.id, []));
            if (fd && d.difficulty !== fd) continue;
            if (q && (d.name || "").toLowerCase().indexOf(q) < 0 && it.key.toLowerCase().indexOf(q) < 0) continue;
            if (fr !== "" && !canHave(it, +fr)) continue;
            out.push(d);
        }
        const box = $("catalog"); box.innerHTML = '<div class="muted">' + out.length + " items the game can drop</div>";
        const list = document.createElement("div"); list.className = "list";
        out.slice(0, catLimit).forEach((d) => {
            const el = document.createElement("div"); el.className = "item";
            el.innerHTML = itemRow(d, " &middot; up to " + maxCount(data.items[d.id]) + " bonuses &middot; " + esc(d.difficulty), "");
            el.onclick = () => builder(d.id);
            list.appendChild(el);
        });
        box.appendChild(list);
        if (out.length > catLimit) { const m = document.createElement("div"); m.className = "more"; m.innerHTML = "<button>Show more</button>"; m.onclick = () => { catLimit += 100; renderCatalog(); }; box.appendChild(m); }
    }
    ["q", "ftype", "frar", "fmin", "fmax", "fdiff"].forEach((id) => { $(id).oninput = () => { catLimit = 60; renderCatalog(); }; });
    $("tab-inv").onclick = () => { $("tab-inv").classList.add("on"); $("tab-add").classList.remove("on"); $("inv").classList.remove("hidden"); $("add").classList.add("hidden"); };
    $("tab-add").onclick = () => { $("tab-add").classList.add("on"); $("tab-inv").classList.remove("on"); $("add").classList.remove("hidden"); $("inv").classList.add("hidden"); renderCatalog(); };

    // Item builder: pick one loot source (power list + number table + difficulty), then the bonuses.
    function builder(itemId) {
        const it = data.items[itemId];
        const opts = it.sources.map((s) => data.sourceOptions(s));
        // group identical (powers, counts) choices
        const seen = {}; const choices = [];
        opts.forEach((o) => { const k = o.powers.join(",") + "|" + o.counts.join(","); if (!seen[k]) { seen[k] = 1; choices.push(o); } });
        choices.sort((a, b) => Math.max.apply(null, b.counts) - Math.max.apply(null, a.counts) || b.powers.length - a.powers.length);
        const b = { src: 0, picked: [] };
        function draw() {
            const o = choices[b.src];
            const maxN = Math.max.apply(null, o.counts.map((c) => Math.min(c, o.powers.length)));
            const value = data.lootValue(itemId, b.picked);
            const d = data.describe(itemId, data.orderPowers(b.picked), value);
            const err = data.checkPowers(itemId, b.picked);
            let h = '<div class="detail"><div class="nm" style="color:' + d.rarity.rgb + '">' + esc(d.name) + "</div>";
            if (choices.length > 1) {
                h += '<label class="muted">Loot rule (power list / count table) - the game picks bonuses from one list per drop</label><select id="srcsel" style="width:100%">' +
                    choices.map((c, i) => '<option value="' + i + '"' + (i === b.src ? " selected" : "") + ">" + esc((c.listName || "no bonuses") + " / " + (c.numProbName || "-")) +
                        " : " + c.counts.join(", ") + " bonuses from " + c.powers.length + "</option>").join("") + "</select>";
            }
            h += '<div class="muted" style="margin:6px 0">Allowed bonus counts: ' + o.counts.map((c) => Math.min(c, o.powers.length)).join(", ") + ". Picked " + b.picked.length + ".</div>";
            h += o.powers.map((p) => {
                const on = b.picked.indexOf(p) >= 0;
                const blocked = !on && (b.picked.length >= maxN || b.picked.some((q) => data.clashes(p, q)));
                return '<label class="pw' + (blocked ? " off" : "") + '"><input type="checkbox" data-p="' + p + '"' + (on ? " checked" : "") + (blocked ? " disabled" : "") + ">" +
                    "<span>" + text(data.powerText(p)) + ' <span class="k muted">' + esc(data.powers[p].key) + "</span></span></label>";
            }).join("");
            h += detailTable(d, null) + "<h2>Bonuses</h2>" + powersHtml(d);
            h += '<div class="' + (err ? "err" : "ok") + '">' + (err ? esc(err) : "The original game can drop exactly this item.") + "</div>";
            const full = st.save.gear.items.length >= DH2.MAX_ITEMS;
            if (full) h += '<div class="err">The inventory is full (100 items).</div>';
            h += '<div class="row" style="margin-top:12px"><button id="close">Cancel</button><button id="addit" class="primary"' + (err || full ? " disabled" : "") + ">Add to inventory</button></div></div>";
            openSheet(h);
            $("close").onclick = closeSheet;
            if ($("srcsel")) $("srcsel").onchange = () => { b.src = +$("srcsel").value; b.picked = []; draw(); };
            document.querySelectorAll("[data-p]").forEach((c) => c.onchange = () => {
                const p = +c.dataset.p;
                if (c.checked) b.picked.push(p); else b.picked = b.picked.filter((x) => x !== p);
                const top = $("sheetbody").scrollTop; draw(); $("sheetbody").scrollTop = top;
            });
            $("addit").onclick = () => {
                st.save.gear.items.push(data.newEntry(itemId, b.picked, 1));
                closeSheet(); setDirty(true); render();
                toast("Added " + esc(d.name) + ". Press Save to write the file.");
            };
        }
        draw();
    }

    // ------------------------------------------------------------ save
    $("save").onclick = () => {
        const bytes = st.save.toBytes();
        // self-check: what we write must parse back to what we show
        const back = new DH2.Save(bytes);
        if (back.gear.gold !== st.save.gear.gold || back.gear.items.length !== st.save.gear.items.length) return fail(new Error("internal check failed, nothing written"));
        $("save").disabled = true;
        const gear = back.gearBytes();
        let synced = 0;
        host.write(st.path, bytes).then((r) => {
            // then the checkpoint copies of the same hero get the same GEAR (each write keeps its own backups)
            return st.checkpoints.reduce((p, cp) => p.then(() => host.read(cp).then((cb) => host.write(cp, DH2.replaceGear(cb, gear))).then(() => { synced++; })), Promise.resolve()).then(() => r);
        }).then((r) => {
            st.save = back; setDirty(false); render();
            toast('<span class="ok">Saved.</span> ' + esc(r.note || "") + (synced ? " (+ " + synced + " checkpoint copy)" : "") + (r.backup ? "<br>Backup: " + esc(r.backup) : ""), 7000);
            if (host.launchGame) {
                const b2 = document.createElement("button"); b2.textContent = "Start the game"; b2.style.marginLeft = "8px";
                b2.onclick = () => host.launchGame().catch(fail);
                $("toast").appendChild(b2);
            }
        }).catch((e) => {
            $("save").disabled = false; fail(e);
            if (host.stopGame && /running/.test(e.message)) {
                const b3 = document.createElement("button"); b3.textContent = "Stop the game"; b3.style.marginLeft = "8px";
                b3.onclick = () => host.stopGame().then(() => toast("The game was stopped. Press Save again.")).catch(fail);
                $("toast").appendChild(b3);
            }
        });
    };
    $("revert").onclick = () => { st.save = new DH2.Save(st.save.original); st.cap = DH2.potionCap(st.save, data); setDirty(false); render(); };
    $("fileinfo").onclick = () => { if (!st.dirty || confirm("Discard unsaved changes?")) { st.save = null; $("fileinfo").textContent = "no save open"; showFiles(); } };

    window.DH2App = { state: st, data: data, open: open };   // used by the automated proof scripts
    showFiles();
})();
