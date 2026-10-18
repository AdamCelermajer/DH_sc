// Save editor UI (shared by Android and Windows). Rules and formats live in dh2save.js; this file only shows and edits.
(function () {
    "use strict";
    const $ = (id) => document.getElementById(id);
    const esc = (s) => String(s == null ? "" : s).replace(/[&<>"]/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[c]));
    const text = (s) => esc(s).replace(/\n/g, "<br>");
    const oneLine = (s) => esc(String(s || "").trim()).replace(/\s*\n\s*/g, " &middot; ");
    const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
    const host = window.DH2Host;
    const data = new DH2.ItemData(window.DH2_ITEMDATA);
    const st = { save: null, path: null, name: null, dirty: false, hero: null, open: {} };

    // ------------------------------------------------------------ language
    const langSel = $("lang");
    data.languages().forEach((l) => { const o = document.createElement("option"); o.value = l.index; o.textContent = l.key + " - " + l.sample; langSel.appendChild(o); });
    langSel.value = localStorage.getItem("dh2lang") || "0";
    data.lang = +langSel.value;
    langSel.onchange = () => { data.lang = +langSel.value; localStorage.setItem("dh2lang", langSel.value); render(); };

    // ms = 0: stays until tapped
    function toast(msg, ms) {
        const t = $("toast"); t.innerHTML = msg; t.classList.remove("hidden"); clearTimeout(toast.t);
        if (ms !== 0) toast.t = setTimeout(() => t.classList.add("hidden"), ms || 3500);
    }
    $("toast").onclick = (e) => { if (e.target.tagName !== "BUTTON") $("toast").classList.add("hidden"); };
    function fail(e) { console.error(e); toast('<span class="err">' + esc(e.message || e) + "</span>", 0); }

    // A modal question. buttons: [[label, value, css class]]; resolves with the value of the pressed button.
    function ask(html, buttons) {
        return new Promise((resolve) => {
            $("dlgbody").innerHTML = html + '<div class="row dlgbtns">' + buttons.map((b, i) => '<button data-i="' + i + '" class="' + (b[2] || "") + '">' + esc(b[0]) + "</button>").join("") + "</div>";
            $("dlg").classList.remove("hidden");
            $("dlgbody").querySelectorAll("[data-i]").forEach((el) => el.onclick = () => { $("dlg").classList.add("hidden"); resolve(buttons[+el.dataset.i][1]); });
        });
    }
    if (host.launchGame) {
        $("play").classList.remove("hidden");
        $("play").onclick = () => {
            if (!st.dirty) return host.launchGame().catch(fail);
            ask("<h2>Start the game?</h2><p>Your changes are not saved yet. Starting the game now leaves the save file as it is.</p>",
                [["Back to the editor", ""], ["Start without saving", "go", "danger"]]).then((v) => { if (v) host.launchGame().catch(fail); });
        };
    }

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
                host.gameRunning().then((on) => {
                    if (on) $("hostnote").innerHTML = '<span class="err">The game is running.</span> You can look at and change the save; when you press Save the editor asks you before it stops the game.';
                }).catch(() => {});
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
        return host.read(f.path).then((bytes) => {
            st.save = new DH2.Save(bytes); st.path = f.path; st.name = f.name; st.dirty = false; st.checkpoints = f.checkpoints || [];
            $("files").classList.add("hidden"); $("editor").classList.remove("hidden");
            $("fileinfo").textContent = f.name + (f.where ? " (" + f.where + ")" : "");
            render();
        }).catch(fail);
    }

    // ------------------------------------------------------------ editor
    function setDirty(v) { st.dirty = v; $("dirty").textContent = v ? "unsaved changes" : "no changes"; $("save").disabled = !v; $("revert").disabled = !v; }
    const ATTR = ["Str", "Dex", "End", "Ene"];
    function render() {
        if (!st.save) return;
        const g = st.save.gear, info = st.save.info;
        st.hero = data.hero(st.save);
        const h = st.hero;
        $("hero").innerHTML = "Hero <b>" + esc(info.name || "?") + "</b> &middot; " + esc(data.heroClassLabel(h.cls)) + " &middot; level " + esc(h.level) +
            " &middot; " + ATTR.map((a, i) => a + " <b>" + h.attrs[i] + "</b>").join(" ") + " &middot; " + g.items.length + " items (the game holds 100)" +
            '<div class="muted">Attributes = ' + (h.startKnown ? "the class's starting values" : "<span class=err>class starting values unknown (counted as 0)</span>") +
            " + points spent (from the save) + bonuses of the items equipped in set " + (g.equipSet + 1) +
            ". Skill and potion buffs are not in the save. Items the hero cannot equip are greyed with the reason.</div>";
        $("gold").value = g.gold;
        $("goldnote").textContent = "0 .. " + DH2.MAX_GOLD.toLocaleString() + " (the game's gold limit)";
        const pi = DH2.potionIndex(st.save, data);
        $("potions").value = pi >= 0 ? g.items[pi].qty : 0;
        $("potionnote").textContent = "Any whole number 0 .. " + DH2.MAX_POTIONS.toLocaleString() + " (the save keeps the count as a 32-bit number). " +
            data.text(data.itemByKey.Potion0)[0] + "; a new hero starts with " + DH2.DEFAULT_POTION_CAP + ".";
        renderInventory();
        if (!$("add").classList.contains("hidden")) renderCatalog();
        setDirty(st.dirty);
    }

    function rarityDot(r) { return '<span class="dot" style="background:' + r.rgb + '" title="' + esc(r.name) + '"></span>'; }
    // One list row: name, the game's stats and requirements texts, and why the hero cannot use it.
    function itemRow(d, opt) {
        opt = opt || {};
        const why = data.cannotUse(d.id, st.hero);
        let h = rarityDot(d.rarity) + '<div class="grow"><div class="nm" style="color:' + d.rarity.rgb + '">' + esc(d.name || d.key) + "</div>";
        if (d.stats) h += '<div class="stat">' + oneLine(d.stats) + "</div>";
        h += '<div class="sub">' + (d.reqs ? '<span class="' + (why.length ? "err" : "req") + '">' + oneLine(d.reqs) + "</span> &middot; " : "") + esc(d.type) +
            " &middot; " + esc(d.rarity.name) + (d.powers.length ? " &middot; " + d.powers.length + " bonus" + (d.powers.length > 1 ? "es" : "") : "") + (opt.extra || "") + "</div>";
        if (opt.powers && d.powers.length) h += '<div class="pws" style="color:' + d.rarity.rgb + '">' + d.powers.map((p) => oneLine(p.text)).join(" &middot; ") + "</div>";
        if (why.length) h += '<div class="why">Hero cannot use: ' + esc(why.join(", ")) + "</div>";
        h += "</div>" + (opt.badges || "");
        return { html: h, cant: why.length > 0 };
    }
    function slotLabel(s) { return s < 0 ? "" : (data.slotName[s] || ("slot " + s)); }
    function entryDesc(it) {
        const id = data.itemByKey[it.key];
        if (id === undefined) return null;
        return data.describe(id, it.powers.map((p) => data.powerByKey[p]), it.value);
    }

    // ------------------------------------------------------------ groups (sets / slot)
    // The group of an item row: its armour set (the game's sets, or the family of same-name pieces) or its slot type.
    function groupOf(id, mode) {
        const it = data.items[id];
        if (mode === "set") {
            const s = data.setOf(id);
            if (s) return { key: s.key, label: s.name + " set", order: 0,
                sub: s.game ? "game armour set: all five pieces give the trophy &laquo;" + esc(s.trophy) + "&raquo;" : "same-name " + s.material.toLowerCase() + " armour, no set bonus" };
        }
        if (mode === "none") return { key: "all", label: "All items", order: 0, sub: "" };
        return { key: "type:" + it.type, label: data.typeLabel(it.type), order: 1 + data.pieceOrder(id), sub: esc(data.typeName[it.type] || "") };
    }
    // entries: [{id, html builder, el}] -> grouped <details> blocks; contents are built when a group opens.
    function renderGroups(box, entries, mode, scope, openAll) {
        const groups = new Map();
        for (const e of entries) {
            const g = groupOf(e.id, mode);
            if (!groups.has(g.key)) groups.set(g.key, Object.assign({ items: [], req: 1e9 }, g));
            const G = groups.get(g.key), it = data.items[e.id];
            G.items.push(e);
            G.req = Math.min(G.req, it.levelReq * 1000 + it.strReq + it.dexReq + it.endReq + it.eneReq);
        }
        const list = [...groups.values()].sort((a, b) => a.order - b.order || a.req - b.req || a.label.localeCompare(b.label));
        for (const G of list) {
            G.items.sort((a, b) => data.pieceOrder(a.id) - data.pieceOrder(b.id) || a.id - b.id);
            const det = document.createElement("details"); det.className = "grp";
            const cant = G.items.filter((e) => data.cannotUse(e.id, st.hero).length).length;
            det.innerHTML = "<summary><b>" + esc(G.label) + '</b> <span class="muted">' + G.items.length + (cant ? ", " + cant + " not usable" : "") +
                (G.sub ? " &middot; " + G.sub : "") + "</span></summary>";
            const inner = document.createElement("div"); inner.className = "list"; det.appendChild(inner);
            const fill = () => { if (!inner.childElementCount) G.items.forEach((e) => inner.appendChild(e.make())); };
            const k = scope + mode + G.key;
            det.ontoggle = () => { st.open[k] = det.open; if (det.open) fill(); };
            if (openAll || st.open[k]) { det.open = true; fill(); }
            box.appendChild(det);
        }
    }
    const groupSel = $("group");
    groupSel.value = localStorage.getItem("dh2group") || "set";
    groupSel.onchange = () => { localStorage.setItem("dh2group", groupSel.value); render(); };

    function renderInventory() {
        const box = $("inv"); box.innerHTML = "";
        const entries = [];
        st.save.gear.items.forEach((it, idx) => {
            const d = entryDesc(it);
            const id = d ? d.id : -1;
            const make = () => {
                const el = document.createElement("div"); el.className = "item";
                if (!d) { el.innerHTML = '<div class="grow err">unknown item ' + esc(it.key) + "</div>"; return el; }
                let badges = "";
                if (it.slot0 >= 0) badges += '<span class="badge eq">set 1: ' + esc(slotLabel(it.slot0)) + "</span>";
                if (it.slot1 >= 0) badges += '<span class="badge eq">set 2: ' + esc(slotLabel(it.slot1)) + "</span>";
                if (d.stackable || it.qty !== 1) badges += '<span class="badge">x' + it.qty + "</span>";
                const r = itemRow(d, { badges: badges, powers: true });
                el.innerHTML = r.html; if (r.cant) el.classList.add("cant");
                el.onclick = () => showItem(idx);
                return el;
            };
            if (id < 0) { box.appendChild(make()); return; }
            entries.push({ id: id, make: make });
        });
        if (groupSel.value === "none") { const l = document.createElement("div"); l.className = "list"; entries.forEach((e) => l.appendChild(e.make())); box.appendChild(l); }
        else renderGroups(box, entries, groupSel.value, "inv", entries.length <= 40);
    }

    function detailTable(d, it) {
        const rows = [];
        const r = (k, v) => { if (v !== "" && v != null) rows.push("<tr><td>" + esc(k) + "</td><td>" + v + "</td></tr>"); };
        const why = data.cannotUse(d.id, st.hero);
        r("Type", esc(d.type) + (d.slotting && d.slotting !== "none" ? " (" + esc(d.slotting) + ")" : ""));
        r("Rarity", rarityDot(d.rarity) + " " + esc(d.rarity.name) + ' <span class="muted">(the game colours names by the number of bonuses: ' + d.rarity.powers + ")</span>");
        if (d.level) r("Level requirement", String(d.level));
        r("Stats", text(d.stats));
        r("Requirements", text(d.reqs));
        if (why.length) r("Hero", '<span class="err">cannot use it: ' + esc(why.join(", ")) + "</span>");
        else if (d.reqs) r("Hero", '<span class="ok">meets the requirements</span>');
        r("Difficulty row", esc(d.difficulty));
        const s = data.setOf(d.id);
        if (s) r(s.game ? "Armour set" : "Armour family", esc(s.name) + ' <span class="muted">(' + (s.game ? "all five pieces: trophy " + esc(s.trophy) : "same-name pieces, no set bonus") + ")</span>");
        r("Item value", d.value + ' <span class="muted">(the base of all prices; the game&#39;s Transmute box shows the gold for this hero)</span>');
        r("Sell price", d.prices.sellMin === d.prices.sellMax ? String(d.prices.sellMin) : d.prices.sellMin + " - " + d.prices.sellMax +
            ' <span class="muted">(merchants buy at ' + d.prices.buyPctMin + "-" + d.prices.buyPctMax + "% of value)</span>");
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
        $(id).onchange = () => { try { apply($(id).value); setDirty(true); render(); } catch (e) { fail(e); render(); } };
    }
    numInput("gold", (v) => DH2.setGold(st.save, +v));
    numInput("potions", (v) => {
        if (!/^\s*\d+\s*$/.test(v)) throw new Error("potions: a whole number 0 .. " + DH2.MAX_POTIONS);
        DH2.setPotions(st.save, data, +v);
    });
    document.querySelectorAll("[data-add]").forEach((b) => b.onclick = () => {
        try { DH2.setGold(st.save, Math.min(DH2.MAX_GOLD, st.save.gear.gold + +b.dataset.add)); setDirty(true); render(); } catch (e) { fail(e); }
    });

    // ------------------------------------------------------------ catalog
    const types = {}; data.items.forEach((it) => { if (data.canAdd(it.id)) types[it.type] = data.typeLabel(it.type); });
    Object.keys(types).sort((a, b) => a - b).forEach((t) => { const o = document.createElement("option"); o.value = t; o.textContent = types[t]; $("ftype").appendChild(o); });
    data.d.powerColors.slice(0, 5).forEach((c) => { const r = data.rarity(c.powers); const o = document.createElement("option"); o.value = c.powers; o.textContent = r.name + " (" + c.powers + " bonus" + (c.powers === 1 ? "" : "es") + ")"; $("frar").appendChild(o); });
    function maxCount(it) { let m = 0; it.sources.forEach((s) => { const o = data.sourceOptions(s); o.counts.forEach((c) => { m = Math.max(m, Math.min(c, o.powers.length)); }); }); return m; }
    function canHave(it, n) { return it.sources.some((s) => { const o = data.sourceOptions(s); return o.counts.some((c) => Math.min(c, o.powers.length) === n || (c >= o.powers.length && o.powers.length === n)) || (n === 0 && o.counts.indexOf(0) >= 0); }); }
    let catLimit = 60;
    function renderCatalog() {
        const q = $("q").value.trim().toLowerCase(), ft = $("ftype").value, fr = $("frar").value, fmin = $("fmin").value, fmax = $("fmax").value, fd = $("fdiff").value, fu = $("fuse").checked;
        const out = [];
        for (const it of data.items) {
            if (!data.canAdd(it.id)) continue;
            if (ft !== "" && String(it.type) !== ft) continue;
            if (fmin !== "" && it.levelReq < +fmin) continue;
            if (fmax !== "" && it.levelReq > +fmax) continue;
            const d = data.describe(it.id, [], data.lootValue(it.id, []));
            if (fd && d.difficulty !== fd) continue;
            const s = data.setOf(it.id);
            if (q && (d.name || "").toLowerCase().indexOf(q) < 0 && it.key.toLowerCase().indexOf(q) < 0 && !(s && s.name.toLowerCase().indexOf(q) >= 0)) continue;
            if (fr !== "" && !canHave(it, +fr)) continue;
            if (fu && data.cannotUse(it.id, st.hero).length) continue;
            out.push(d);
        }
        const box = $("catalog"); box.innerHTML = '<div class="muted">' + out.length + " items the game can drop" + (fu ? " that the hero can use" : "") + "</div>";
        const make = (d) => () => {
            const el = document.createElement("div"); el.className = "item";
            const r = itemRow(d, { extra: " &middot; up to " + maxCount(data.items[d.id]) + " bonuses &middot; " + esc(d.difficulty) });
            el.innerHTML = r.html; if (r.cant) el.classList.add("cant");
            el.onclick = () => builder(d.id);
            return el;
        };
        if (groupSel.value !== "none") {
            renderGroups(box, out.map((d) => ({ id: d.id, make: make(d) })), groupSel.value, "cat", out.length <= 30);
            return;
        }
        const list = document.createElement("div"); list.className = "list";
        out.slice(0, catLimit).forEach((d) => list.appendChild(make(d)()));
        box.appendChild(list);
        if (out.length > catLimit) { const m = document.createElement("div"); m.className = "more"; m.innerHTML = "<button>Show more</button>"; m.onclick = () => { catLimit += 100; renderCatalog(); }; box.appendChild(m); }
    }
    ["q", "ftype", "frar", "fmin", "fmax", "fdiff", "fuse"].forEach((id) => { $(id).oninput = $(id).onchange = () => { catLimit = 60; renderCatalog(); }; });
    $("tab-inv").onclick = () => { $("tab-inv").classList.add("on"); $("tab-add").classList.remove("on"); $("inv").classList.remove("hidden"); $("add").classList.add("hidden"); };
    $("tab-add").onclick = () => { $("tab-add").classList.add("on"); $("tab-inv").classList.remove("on"); $("add").classList.remove("hidden"); $("inv").classList.add("hidden"); renderCatalog(); };

    // ------------------------------------------------------------ item builder
    // "ArmorPowerLevel_03" -> "Armor bonuses, power level 03"; other pool names are the game's special lists.
    function poolLabel(name) {
        const m = /^(\w*?)PowerLevel_(\d+)$/.exec(name || "");
        if (m) return (m[1] ? m[1].replace(/([a-z])([A-Z])/g, "$1 $2") : "All") + " bonuses, power level " + m[2];
        return name ? name.replace(/_/g, " ") + " (special list)" : "no bonuses";
    }
    // Pick one loot source (bonus pool + number table + difficulty), then the bonuses, grouped by category and kind.
    function builder(itemId) {
        const it = data.items[itemId];
        const opts = it.sources.map((s) => data.sourceOptions(s));
        const seen = {}; const choices = [];
        opts.forEach((o) => { const k = o.powers.join(",") + "|" + o.counts.join(","); if (!seen[k]) { seen[k] = 1; choices.push(o); } });
        choices.sort((a, b) => (a.listName || "").localeCompare(b.listName || "") || Math.max.apply(null, b.counts) - Math.max.apply(null, a.counts));
        const maxOf = (c) => Math.max.apply(null, c.counts.map((n) => Math.min(n, c.powers.length)));
        // pools that can hold all of these bonuses (src -1 = every pool of the item, picked side by side by power level)
        const fitting = (set) => choices.filter((c, i) => (b.src < 0 || b.src === i) && set.length <= maxOf(c) && set.every((p) => c.powers.indexOf(p) >= 0));
        const b = { src: -1, picked: [], q: "" };
        function pickerHtml() {
            const cats = {}, seenP = {};
            const pool = b.src < 0 ? [].concat.apply([], choices.map((c) => c.powers)) : choices[b.src].powers;
            for (const p of pool) {
                if (seenP[p]) continue; seenP[p] = 1;
                const pi = data.powerInfo(p);
                const c = cats[pi.category] = cats[pi.category] || {};
                (c[pi.label] = c[pi.label] || []).push({ p: p, pi: pi });
            }
            let h = "";
            for (const cat of data.categories()) {
                if (!cats[cat]) continue;
                h += '<div class="pcat"><h3>' + esc(cat) + "</h3>";
                for (const label of Object.keys(cats[cat])) {
                    const rows = cats[cat][label].sort((x, y) => x.pi.tier - y.pi.tier);
                    const all = (label + " " + rows.map((r) => data.powerText(r.p) + " " + data.powers[r.p].key).join(" ")).toLowerCase();
                    h += '<div class="pfam" data-text="' + esc(all) + '"><div class="plabel">' + esc(label) + "</div><div class=\"chips\">";
                    for (const r of rows) {
                        const p = r.p, on = b.picked.indexOf(p) >= 0;
                        const clash = b.picked.find((q) => q !== p && data.clashes(p, q));
                        const blocked = !on && (clash !== undefined || !fitting(b.picked.concat([p])).length);
                        const why = !blocked ? "" : clash !== undefined ? "cannot be combined with " + data.powerText(clash) : "no bonus pool of this item gives this together with your picks";
                        h += '<label class="chip' + (on ? " on" : "") + (blocked ? " off" : "") + '" title="' + esc(why || data.powers[p].key) + '"><input type="checkbox" data-p="' + p + '"' +
                            (on ? " checked" : "") + (blocked ? " disabled" : "") + ">" + (r.pi.tier ? '<span class="tier">L' + String(r.pi.tier).padStart(2, "0") + (r.pi.difficulty ? " " + esc(r.pi.difficulty) : "") + "</span>" : "") +
                            esc(data.powerText(p)) + "</label>";
                    }
                    h += "</div></div>";
                }
                h += "</div>";
            }
            return h;
        }
        function applySearch() {
            const q = b.q.trim().toLowerCase();
            document.querySelectorAll(".pfam").forEach((el) => { el.style.display = !q || el.dataset.text.indexOf(q) >= 0 ? "" : "none"; });
            document.querySelectorAll(".pcat").forEach((el) => { el.style.display = [...el.querySelectorAll(".pfam")].some((f) => f.style.display !== "none") ? "" : "none"; });
        }
        function draw() {
            const fit = fitting(b.picked);
            const o = fit.length ? fit.sort((x, y) => maxOf(y) - maxOf(x))[0] : choices[Math.max(0, b.src)];
            const value = data.lootValue(itemId, b.picked);
            const d = data.describe(itemId, data.orderPowers(b.picked), value);
            const err = data.checkPowers(itemId, b.picked);
            const r = itemRow(d, {});
            let h = '<div class="detail"><div class="item' + (r.cant ? " cant" : "") + '" style="cursor:default">' + r.html + "</div>";
            h += '<div class="explain"><b>Bonus pool</b> = the list the game draws this item\'s bonuses from (one pool per drop). ' +
                "Pools called <i>... power level 01 .. 08</i> hold weaker .. stronger versions of the same bonuses (L01 = weakest, L08 = strongest). " +
                "Hard / Very Hard item rows get the Hard / Very Hard versions with bigger numbers. Greyed bonuses cannot go together with what you picked (the game never combines them).</div>";
            if (choices.length > 1) {
                h += '<label class="muted">Bonus pool (and how many bonuses it gives)</label><select id="srcsel" style="width:100%">' +
                    '<option value="-1"' + (b.src < 0 ? " selected" : "") + ">All bonus pools of this item (power levels side by side)</option>" + choices.map((c, i) => '<option value="' + i + '"' + (i === b.src ? " selected" : "") + ">" + esc(poolLabel(c.listName)) + " : " +
                        [...new Set(c.counts.map((n) => Math.min(n, c.powers.length)))].join(", ") + " of " + c.powers.length + " bonuses" + "</option>").join("") + "</select>";
            } else h += '<div class="muted">Bonus pool: ' + esc(poolLabel(o.listName)) + "</div>";
            if (b.src < 0 && b.picked.length) h += '<div class="muted">Your picks drop from: ' + esc(poolLabel(o.listName)) + "</div>";
            h += '<div class="row" style="margin:8px 0"><input id="psearch" type="search" placeholder="search bonuses (e.g. strength, fire, crit)" style="flex:1" value="' + esc(b.q) + '">' +
                '<span class="muted">allowed: ' + [...new Set(o.counts.map((c) => Math.min(c, o.powers.length)))].join(", ") + " bonuses &middot; picked " + b.picked.length + "</span></div>";
            h += '<div id="picker">' + pickerHtml() + "</div>";
            h += "<h2>Result</h2>" + detailTable(d, null) + "<h2>Bonuses</h2>" + powersHtml(d);
            h += '<div class="' + (err ? "err" : "ok") + '">' + (err ? esc(err) : "The original game can drop exactly this item.") + "</div>";
            const full = st.save.gear.items.length >= DH2.MAX_ITEMS;
            if (full) h += '<div class="err">The inventory is full (100 items).</div>';
            h += '<div class="row sticky"><button id="close">Cancel</button><button id="addit" class="primary"' + (err || full ? " disabled" : "") + ">Add to inventory</button></div></div>";
            openSheet(h);
            $("close").onclick = closeSheet;
            if ($("srcsel")) $("srcsel").onchange = () => { b.src = +$("srcsel").value; b.picked = []; draw(); };
            $("psearch").oninput = () => { b.q = $("psearch").value; applySearch(); };
            applySearch();
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
    // Android: the game keeps the hero in memory and writes it back, so the file can only be replaced while the game's
    // process is gone. The editor never stops the game without asking.
    async function stopGameIfRunning() {
        if (!host.gameRunning) return true;
        let running = false;
        try { running = await host.gameRunning(); } catch (e) { running = false; }
        if (!running) return true;
        const v = await ask("<h2>The game is running</h2><p>The game is running. Unsaved progress in the game will be lost. Stop the game and save?</p>" +
            '<p class="muted">The game keeps your hero in memory and would write over the edited save. Stopping it is the same as swiping it away from the recent apps: ' +
            "what you did since the game last saved is lost. After saving you can start the game again.</p>",
            [["Cancel", ""], ["Stop the game and save", "stop", "danger"]]);
        if (v !== "stop") return false;
        await host.stopGame();
        for (let i = 0; i < 40; i++) { if (!(await host.gameRunning())) return true; await sleep(250); }
        throw new Error("The game did not stop. Close it from the recent apps, then press Save again.");
    }
    async function doSave() {
        const bytes = st.save.toBytes();
        // self-check: what we write must parse back to what we show
        const back = new DH2.Save(bytes);
        if (back.gear.gold !== st.save.gear.gold || back.gear.items.length !== st.save.gear.items.length) throw new Error("internal check failed, nothing written");
        const gear = back.gearBytes();
        const r = await host.write(st.path, bytes);
        let synced = 0;
        for (const cp of st.checkpoints) { const cb = await host.read(cp); await host.write(cp, DH2.replaceGear(cb, gear)); synced++; }   // the hero's checkpoint copies get the same GEAR
        st.save = back; setDirty(false); render();
        const where = r.backup ? "Backup: " + esc(String(r.backup).split(/[\\/]/).slice(-2).join("/")) : "";
        if (host.launchGame) {
            const v = await ask('<h2 class="ok">Saved</h2><p>' + esc(r.note || "") + (synced ? " (+ " + synced + " checkpoint copy)" : "") + '</p><p class="muted">' + where + "</p>",
                [["Stay in the editor", ""], ["Start the game", "start", "primary"]]);
            if (v === "start") await host.launchGame();
        } else toast('<span class="ok">Saved.</span> ' + esc(r.note || "") + (synced ? " (+ " + synced + " checkpoint copy)" : "") + (where ? "<br>" + where : ""), 7000);
    }
    $("save").onclick = async () => {
        $("save").disabled = true;
        try {
            if (!(await stopGameIfRunning())) { $("save").disabled = !st.dirty; return; }
            await doSave();
        } catch (e) {
            $("save").disabled = !st.dirty; fail(e);
        }
    };
    $("revert").onclick = () => { st.save = new DH2.Save(st.save.original); setDirty(false); render(); };
    $("fileinfo").onclick = () => {
        const go = () => { st.save = null; $("fileinfo").textContent = "no save open"; showFiles(); };
        if (!st.dirty) return go();
        ask("<h2>Discard the changes?</h2><p>The unsaved changes of this save are lost.</p>", [["Keep editing", ""], ["Discard", "go", "danger"]]).then((v) => { if (v) go(); });
    };

    window.DH2App = { state: st, data: data, open: open, render: render, builder: builder };   // used by the automated proof scripts
    showFiles();
})();
