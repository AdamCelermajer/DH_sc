// Save editor UI (shared by Android and Windows). Rules and formats live in dh2save.js; this file only shows and edits.
// Rendering, so that a phone's WebView stays fast: a list is one HTML string set once per group (no per-row DOM building, no
// per-row listeners: clicks and group toggles are delegated), the rows of a group are built when the group opens, item
// descriptions / can-use checks / catalog rows are cached per language and hero, the catalog search is debounced.
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
    // caches: cat = catalog rows (per language), cant = can-use reasons (per hero + language), desc = inventory entry
    // descriptions (per entry object), grp = group of a row (per mode + language), pc = bonus category class per power
    const memo = { lang: -1, cat: null, cant: new Map(), heroKey: "", desc: new WeakMap(), grp: new Map(), pc: new Map() };
    function langChanged() { memo.cant.clear(); memo.grp.clear(); }

    // ------------------------------------------------------------ language
    const langSel = $("lang");
    data.languages().forEach((l) => { const o = document.createElement("option"); o.value = l.index; o.textContent = l.key + " - " + l.sample; langSel.appendChild(o); });
    langSel.value = localStorage.getItem("dh2lang") || "0";
    data.lang = +langSel.value;
    langSel.onchange = () => { data.lang = +langSel.value; localStorage.setItem("dh2lang", langSel.value); langChanged(); render(); };

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
    function setDirty(v) { st.dirty = v; if (window.AndroidHost && window.AndroidHost.setDirty) window.AndroidHost.setDirty(!!v); $("dirty").textContent = v ? "unsaved changes" : "no changes"; $("save").disabled = !v; $("revert").disabled = !v; }
    const ATTR = ["Str", "Dex", "End", "Ene"];
    function render() {
        if (!st.save) return;
        const g = st.save.gear, info = st.save.info;
        st.hero = data.hero(st.save);
        const h = st.hero;
        const hk = h.cls + "|" + h.level + "|" + h.attrs.join(",");
        if (hk !== memo.heroKey) { memo.heroKey = hk; memo.cant.clear(); }
        $("hero").innerHTML = '<div class="hname"><b>' + esc(info.name || "?") + '</b><span class="hcls">' + esc(data.heroClassLabel(h.cls)) + '</span><span class="tag lvl">Level ' + esc(h.level) + "</span></div>" +
            '<div class="hstats">' + ATTR.map((a, i) => '<span class="chip st">' + a + " <b>" + h.attrs[i] + "</b></span>").join("") +
            '<span class="chip bag">' + g.items.length + " / " + DH2.MAX_ITEMS + " items</span></div>" +
            '<div class="muted">Attributes = ' + (h.startKnown ? "the class's starting values" : "<span class=err>class starting values unknown (counted as 0)</span>") +
            " + points spent + bonuses of the items equipped in set " + (g.equipSet + 1) + " (as the game's Stats screen; skill and potion buffs are not in the save).</div>";
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

    // ------------------------------------------------------------ rows
    const SLOT_LABEL = { 0: "Torso", 1: "Right hand", 2: "Left hand", 3: "Feet", 4: "Hands", 5: "Right ring", 6: "Left ring", 7: "Waist", 8: "Head" };
    const SLOT_ORDER = [8, 0, 4, 7, 3, 1, 2, 5, 6];
    const TYPE_SHORT = { 0: "1H weapon", 1: "Off-hand", 2: "2H weapon", 3: "Dagger / claw", 4: "Bow", 5: "Staff / wand", 6: "Shield", 7: "Armour", 8: "Gloves", 9: "Boots",
        10: "Helm", 11: "Ring", 12: "Belt", 13: "Gold", 14: "Potion", 15: "Valuable", 16: "Quest item" };
    const CAT_CLASS = { "Attributes": "attr", "Life and mana": "life", "Elemental damage": "elem", "Attack: damage, critical, attack rating": "atk",
        "Defence: armour, block, dodge, defence rating": "def", "Resistances": "res", "Spells": "spell", "Loot: gold and treasure find": "loot" };
    function slotLabel(s) { return s < 0 ? "" : (SLOT_LABEL[s] || data.slotName[s] || ("slot " + s)); }
    function typeShort(id) { const t = data.items[id].type; return TYPE_SHORT[t] || data.typeName[t] || String(t); }
    function powerClass(pid) {
        let c = memo.pc.get(pid);
        if (c === undefined) { c = "pc pc-" + (CAT_CLASS[data.powerInfo(pid).category] || "other"); memo.pc.set(pid, c); }
        return c;
    }
    function cannotUse(id) {
        let v = memo.cant.get(id);
        if (v === undefined) { v = data.cannotUse(id, st.hero); memo.cant.set(id, v); }
        return v;
    }
    function entryDesc(it) {
        const c = memo.desc.get(it);
        if (c && c.lang === data.lang && c.powers === it.powers && c.value === it.value) return c.d;
        const id = data.itemByKey[it.key];
        const d = id === undefined ? null : data.describe(id, it.powers.map((p) => data.powerByKey[p]), it.value);
        memo.desc.set(it, { lang: data.lang, powers: it.powers, value: it.value, d: d });
        return d;
    }
    const chip = (cls, inner) => '<span class="chip ' + cls + '">' + inner + "</span>";
    // the item's own numbers (the same values as the game's ItemStatsDesc text) as coloured chips
    function statChips(d) {
        let h = "";
        if (d.damage) h += chip("dmg", "Damage " + d.damage[0] + "&ndash;" + d.damage[1]);
        if (d.armor != null) h += chip("arm", "Armour " + d.armor);
        if (d.block != null) h += chip("blk", "Block " + d.block + "%");
        return h;
    }
    function reqChips(d, bad) {
        const cls = bad ? "req bad" : "req";
        let h = d.level ? chip(cls, "Lv " + d.level) : "";
        const r = String(d.reqs || "").replace(/^\s*Req:\s*/i, "").trim();
        if (r) h += chip(cls, oneLine(r));
        return h;
    }
    const powerChips = (d) => d.powers.map((p) => '<span class="' + powerClass(p.id) + '">' + oneLine(p.text) + "</span>").join("");
    // One list row as HTML. d = describe(); opt.attr = extra attributes (data-i / data-id for the delegated click), opt.tags.
    function rowHtml(d, opt) {
        opt = opt || {};
        const why = cannotUse(d.id), bad = why.length > 0;
        let h = '<div class="item' + (bad ? " cant" : "") + '"' + (opt.attr || "") + ' style="--rc:' + d.rarity.rgb + '"><div class="main"><div class="l1"><span class="nm">' +
            esc(d.name || d.key) + "</span>" + (opt.tags || "") + "</div>";
        const chips = statChips(d) + reqChips(d, bad);
        if (chips) h += '<div class="chips">' + chips + "</div>";
        if (d.powers.length) h += '<div class="pws">' + powerChips(d) + "</div>";
        if (bad) h += '<div class="why">Hero cannot use: ' + esc(why.join(", ")) + "</div>";
        return h + '</div><div class="side"><span class="rar">' + esc(d.rarity.name) + '</span><span class="ty">' + esc(typeShort(d.id)) + "</span></div></div>";
    }

    // ------------------------------------------------------------ groups (sets / slot)
    // The group of an item row: its armour set (the game's sets, or the family of same-name pieces) or its slot type.
    function groupOf(id, mode) {
        const ck = mode + "|" + data.lang + "|" + id;
        let g = memo.grp.get(ck);
        if (g) return g;
        const it = data.items[id];
        if (mode === "set") {
            const s = data.setOf(id);
            if (s) g = { key: s.key, label: s.name + " set", order: 0, cls: s.game ? "gset" : "",
                sub: s.game ? "game armour set: all five pieces give the trophy &laquo;" + esc(s.trophy) + "&raquo;" : "same-name " + s.material.toLowerCase() + " armour, no set bonus" };
        }
        if (!g) g = mode === "none" ? { key: "all", label: "All items", order: 0, sub: "" } :
            { key: "type:" + it.type, label: data.typeLabel(it.type), order: 1 + data.pieceOrder(id), sub: esc(data.typeName[it.type] || "") };
        memo.grp.set(ck, g);
        return g;
    }
    function groupHtml(G, k, open) {
        let cant = 0;
        for (const e of G.items) if (cannotUse(e.id).length) cant++;
        return '<details class="grp' + (G.cls ? " " + G.cls : "") + '" data-k="' + esc(k) + '" data-o="' + (open ? 1 : 0) + '"' + (open ? " open" : "") + '><summary><span class="gl">' + esc(G.label) +
            '</span><span class="cnt">' + G.items.length + "</span>" + (cant ? '<span class="bad">' + cant + " not usable</span>" : "") + (G.sub ? '<span class="sub">' + G.sub + "</span>" : "") +
            '</summary><div class="list">' + (open ? G.items.map((e) => e.html()).join("") : "") + "</div></details>";
    }
    // entries: [{id, html: () => row html, group?: fixed group}] -> <details> blocks appended to box; the rows of a group are
    // built when it is open (now, or later in the delegated toggle handler).
    function renderGroups(box, entries, mode, scope, openAll) {
        const groups = new Map();
        for (const e of entries) {
            const g = e.group || groupOf(e.id, mode);
            let G = groups.get(g.key);
            if (!G) { G = Object.assign({ items: [], req: 1e9 }, g); groups.set(g.key, G); }
            const it = data.items[e.id];
            G.items.push(e);
            G.req = Math.min(G.req, it.levelReq * 1000 + it.strReq + it.dexReq + it.endReq + it.eneReq);
        }
        const list = [...groups.values()].sort((a, b) => a.order - b.order || a.req - b.req || a.label.localeCompare(b.label));
        let h = "";
        for (const G of list) {
            if (!G.sorted) G.items.sort((a, b) => data.pieceOrder(a.id) - data.pieceOrder(b.id) || a.id - b.id);
            const k = scope + mode + G.key;
            const open = st.open[k] !== undefined ? st.open[k] : (openAll || !!G.openDefault);
            box._groups[k] = G;
            h += groupHtml(G, k, open);
        }
        box.insertAdjacentHTML("beforeend", h);
    }
    function fillGroup(det) {
        const box = det.parentNode, G = box._groups && box._groups[det.dataset.k], list = det.lastElementChild;
        if (G && !list.firstElementChild) list.innerHTML = G.items.map((e) => e.html()).join("");
    }
    function groupsBox(box, onRow) {
        box._groups = {};
        box.addEventListener("toggle", (ev) => {   // toggle does not bubble: capture
            const det = ev.target;
            if (!det.classList || !det.classList.contains("grp")) return;
            if (det.open !== (det.dataset.o === "1")) { st.open[det.dataset.k] = det.open; det.dataset.o = det.open ? "1" : "0"; }
            if (det.open) fillGroup(det);
        }, true);
        box.addEventListener("click", (ev) => {
            const row = ev.target.closest(".item");
            if (row && box.contains(row)) onRow(row, ev);
        });
    }
    const groupSel = $("group");
    groupSel.value = localStorage.getItem("dh2group") || "set";
    groupSel.onchange = () => { localStorage.setItem("dh2group", groupSel.value); render(); };

    // ------------------------------------------------------------ inventory: equipped first, then the bag
    function renderInventory() {
        const box = $("inv"); box.innerHTML = ""; box._groups = {};
        const g = st.save.gear, set = g.equipSet;
        const bag = [], eq = [];
        let unknown = "";
        g.items.forEach((it, idx) => {
            const d = entryDesc(it);
            if (!d) { unknown += '<div class="item unk" data-i="' + idx + '"><div class="main err">unknown item ' + esc(it.key) + "</div></div>"; return; }
            const slot = set === 1 ? it.slot1 : it.slot0, other = set === 1 ? it.slot0 : it.slot1;
            const tags = (slot >= 0 ? '<span class="tag eq">' + esc(slotLabel(slot)) + "</span>" : "") +
                (other >= 0 ? '<span class="tag eq2">set ' + (set === 1 ? 1 : 2) + ": " + esc(slotLabel(other)) + "</span>" : "") +
                (d.stackable || it.qty !== 1 ? '<span class="tag qty">&times;' + it.qty + "</span>" : "");
            const e = { id: d.id, slot: slot >= 0 ? slot : other, html: () => rowHtml(d, { attr: ' data-i="' + idx + '"', tags: tags }) };
            (e.slot >= 0 ? eq : bag).push(e);
        });
        eq.sort((a, b) => SLOT_ORDER.indexOf(a.slot) - SLOT_ORDER.indexOf(b.slot) || a.id - b.id);
        const EQ = { key: "equipped", label: "Equipped", order: -1, cls: "eq", sorted: true, openDefault: true, sub: "set " + (set + 1) + " &middot; what the hero wears" };
        const entries = eq.map((e) => Object.assign(e, { group: EQ }));
        const mode = groupSel.value;
        if (mode === "none") { const BAG = { key: "bag", label: "Bag", order: 0, sorted: true, openDefault: true, sub: "in the order of the save" }; for (const e of bag) { e.group = BAG; entries.push(e); } }
        else for (const e of bag) entries.push(e);
        if (!eq.length) box.innerHTML = '<div class="count">Nothing equipped.</div>';
        renderGroups(box, entries, mode, "inv", bag.length <= 40);
        if (unknown) box.insertAdjacentHTML("beforeend", '<div class="list">' + unknown + "</div>");
    }
    groupsBox($("inv"), (row) => { if (row.dataset.i !== undefined) showItem(+row.dataset.i); });

    function detailTable(d, it) {
        const rows = [];
        const r = (k, v) => { if (v !== "" && v != null) rows.push("<tr><td>" + esc(k) + "</td><td>" + v + "</td></tr>"); };
        const why = cannotUse(d.id);
        r("Type", esc(typeShort(d.id)) + ' <span class="muted">' + esc(d.type) + (d.slotting && d.slotting !== "none" ? " (" + esc(d.slotting) + ")" : "") + "</span>");
        r("Rarity", '<span class="rar" style="--rc:' + d.rarity.rgb + '">' + esc(d.rarity.name) + '</span> <span class="muted">(the game colours names by the number of bonuses: ' + d.rarity.powers + ")</span>");
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
        return d.powers.map((p) => '<div class="power"><span class="' + powerClass(p.id) + '">' + oneLine(p.text) + "</span>" +
            ' <div class="k">' + esc(p.key) + (p.attrs.length ? " &middot; " + p.attrs.map((a) => esc(a.attr) + " " + a.bonus).join(", ") : "") + "</div></div>").join("");
    }

    function showItem(idx) {
        const it = st.save.gear.items[idx]; const d = entryDesc(it);
        if (!d) return;
        let h = '<div class="detail"><div class="nm" style="--rc:' + d.rarity.rgb + '">' + esc(d.name) + "</div>" + detailTable(d, it) + "<h2>Bonuses</h2>" + powersHtml(d);
        if (d.stackable && data.items[d.id].type !== 14) {
            h += '<div class="box"><label>Quantity</label><div class="row"><input id="qty" type="number" min="1" max="' + data.maxQty + '" value="' + it.qty + '"><button id="setqty">Set</button></div></div>';
        }
        if (data.items[d.id].type === 14) h += '<div class="muted">Potions are changed with the Potions box above.</div>';
        h += '<div class="row" style="margin-top:12px"><button id="close">Close</button>';
        if (data.canAdd(d.id)) h += '<button id="editp" class="primary">Edit bonuses</button>';
        if (data.items[d.id].type !== 14) h += '<button id="remove" class="danger">Remove from inventory</button>';
        h += "</div>";
        if (it.slot0 >= 0 || it.slot1 >= 0) h += '<div class="muted">This item is equipped. Removing it leaves the slot empty.</div>';
        h += "</div>";
        openSheet(h);
        $("close").onclick = closeSheet;
        if ($("editp")) $("editp").onclick = () => builder(d.id, idx);
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
    // every row the game can drop, described once per language: description, search text, the bonus counts its loot sources roll
    function catalogRows() {
        if (memo.cat && memo.lang === data.lang) return memo.cat;
        const rows = [];
        for (const it of data.items) {
            if (!data.canAdd(it.id)) continue;
            const d = data.describe(it.id, [], data.lootValue(it.id, []));
            const s = data.setOf(it.id);
            const counts = new Set(); let max = 0;
            for (const src of it.sources) { const o = data.sourceOptions(src); for (const c of o.counts) { const n = Math.min(c, o.powers.length); counts.add(n); if (n > max) max = n; } }
            rows.push({ id: it.id, d: d, text: ((d.name || "") + "\n" + it.key + "\n" + (s ? s.name : "")).toLowerCase(), counts: counts, max: max, lvl: it.levelReq, type: String(it.type), diff: d.difficulty });
        }
        memo.cat = rows; memo.lang = data.lang;
        return rows;
    }
    let catLimit = 60;
    function renderCatalog() {
        const q = $("q").value.trim().toLowerCase(), ft = $("ftype").value, fr = $("frar").value, fmin = $("fmin").value, fmax = $("fmax").value, fd = $("fdiff").value, fu = $("fuse").checked;
        const out = [];
        for (const r of catalogRows()) {
            if (ft !== "" && r.type !== ft) continue;
            if (fmin !== "" && r.lvl < +fmin) continue;
            if (fmax !== "" && r.lvl > +fmax) continue;
            if (fd && r.diff !== fd) continue;
            if (q && r.text.indexOf(q) < 0) continue;
            if (fr !== "" && !r.counts.has(+fr)) continue;
            if (fu && cannotUse(r.id).length) continue;
            out.push(r);
        }
        const box = $("catalog"); box._groups = {};
        box.innerHTML = '<div class="count">' + out.length + " items the game can drop" + (fu ? " that the hero can use" : "") + "</div>";
        const mk = (r) => () => rowHtml(r.d, { attr: ' data-id="' + r.id + '"', tags: '<span class="tag">up to ' + r.max + " bonus" + (r.max === 1 ? "" : "es") + "</span>" +
            (r.diff !== "Normal" ? '<span class="tag d' + (r.diff === "Hard" ? 1 : 2) + '">' + r.diff + "</span>" : "") });
        if (groupSel.value !== "none") { renderGroups(box, out.map((r) => ({ id: r.id, html: mk(r) })), groupSel.value, "cat", out.length <= 30); return; }
        let h = '<div class="list">';
        for (const r of out.slice(0, catLimit)) h += mk(r)();
        h += "</div>";
        if (out.length > catLimit) h += '<div class="more"><button data-more>Show more (' + (out.length - catLimit) + " left)</button></div>";
        box.insertAdjacentHTML("beforeend", h);
    }
    groupsBox($("catalog"), (row) => { if (row.dataset.id !== undefined) builder(+row.dataset.id); });
    $("catalog").addEventListener("click", (ev) => { if (ev.target.hasAttribute("data-more")) { catLimit += 100; renderCatalog(); } });
    let qTimer = 0;
    const refilter = () => { clearTimeout(qTimer); qTimer = 0; catLimit = 60; renderCatalog(); };
    const refilterSoon = () => { clearTimeout(qTimer); qTimer = setTimeout(refilter, 120); };   // typing stays smooth
    ["q", "fmin", "fmax"].forEach((id) => { $(id).oninput = refilterSoon; $(id).onchange = refilter; });
    ["ftype", "frar", "fdiff", "fuse"].forEach((id) => { $(id).onchange = refilter; });
    $("tab-inv").onclick = () => { $("tab-inv").classList.add("on"); $("tab-add").classList.remove("on"); $("inv").classList.remove("hidden"); $("add").classList.add("hidden"); };
    $("tab-add").onclick = () => { $("tab-add").classList.add("on"); $("tab-inv").classList.remove("on"); $("add").classList.remove("hidden"); $("inv").classList.add("hidden"); renderCatalog(); };

    // Before / after table of an edited item (what the game will show and store).
    function compareHtml(a, z, entry) {
        const pw = (d) => d.powers.length ? d.powers.map((p) => '<span class="' + powerClass(p.id) + '">' + oneLine(p.text) + "</span>").join(" ") : '<span class="muted">none</span>';
        const price = (d) => d.prices.sellMin === d.prices.sellMax ? String(d.prices.sellMin) : d.prices.sellMin + " - " + d.prices.sellMax;
        const row = (k, x, y) => "<tr><td>" + esc(k) + "</td><td>" + x + "</td><td" + (x !== y ? ' class="chg"' : "") + ">" + y + "</td></tr>";
        const nm = (d) => '<span class="rar" style="--rc:' + d.rarity.rgb + '">' + esc(d.rarity.name) + "</span>";
        return '<table class="cmp"><tr><th></th><th>Before</th><th>After</th></tr>' + row("Rarity", nm(a), nm(z)) + row("Bonuses", pw(a), pw(z)) +
            row("Stats", oneLine(a.stats), oneLine(z.stats)) + row("Item value", String(a.value), String(z.value)) + row("Sell price", price(a), price(z)) +
            row("Equipped", entry.slot0 >= 0 || entry.slot1 >= 0 ? "yes" : "no", entry.slot0 >= 0 || entry.slot1 >= 0 ? "yes" : "no") +
            row("Quantity", String(entry.qty), String(entry.qty)) + "</table>";
    }

    // ------------------------------------------------------------ item builder
    // The strongest loot source of the row is used automatically; the picker shows the strongest tier of every bonus type, grouped by category.
    // editIdx: index of an inventory entry whose bonuses are edited (same row, slots, quantity; new powers and value).
    function builder(itemId, editIdx) {
        const editing = editIdx !== undefined ? st.save.gear.items[editIdx] : null;
        const best = data.bestOf(itemId);
        const choices = best.choices;   // strongest source first (power level, bonus count, values)
        const maxOf = (c) => Math.max.apply(null, c.counts.map((n) => Math.min(n, c.powers.length)));
        // sources that can hold all of these bonuses, strongest first
        const fitting = (set) => choices.filter((c) => set.length <= maxOf(c) && set.every((p) => c.powers.indexOf(p) >= 0));
        const orig = editing ? editing.powers.map((k) => data.powerByKey[k]).filter((p) => p !== undefined) : [];
        const b = { picked: orig, q: "", all: false, note: "", extra: [] };
        if (editing && orig.length) {
            const up = orig.map((p) => data.strongestOf(itemId, p));
            if (up.some((p, i) => p !== orig[i])) {
                if (new Set(up).size === up.length && data.checkPowers(itemId, up) === null) { b.picked = up; b.note = "Bonuses raised to their strongest tier."; }
                else { b.extra = orig.filter((p) => best.strongest.indexOf(p) < 0); b.note = "Kept your current bonuses: their strongest tiers cannot drop together on this item."; }
            }
        }
        const groupsMemo = {};
        function groupsFor(all, extra) {
            const key = all ? "all" : "top" + extra.join(",");
            if (groupsMemo[key]) return groupsMemo[key];
            const pool = all ? [].concat.apply([], choices.map((c) => c.powers)) : best.strongest.concat(extra);
            const cats = {}, seenP = {};
            for (const p of pool) {
                if (seenP[p]) continue; seenP[p] = 1;
                const pi = data.powerInfo(p);
                const c = cats[pi.category] = cats[pi.category] || {};
                (c[pi.label] = c[pi.label] || []).push({ p: p, pi: pi });
            }
            for (const cat in cats) for (const l in cats[cat]) cats[cat][l].sort((x, y) => x.pi.tier - y.pi.tier);
            return (groupsMemo[key] = cats);
        }
        function pickerHtml() {
            const cats = groupsFor(b.all, b.extra);
            let h = "";
            for (const cat of data.categories()) {
                if (!cats[cat]) continue;
                h += '<div class="pcat pc-' + (CAT_CLASS[cat] || "other") + '"><h3>' + esc(cat) + "</h3>";
                for (const label of Object.keys(cats[cat])) {
                    const rows = cats[cat][label];
                    const all = (label + " " + rows.map((r) => data.powerText(r.p) + " " + data.powers[r.p].key).join(" ")).toLowerCase();
                    h += '<div class="pfam" data-text="' + esc(all) + '"><div class="plabel">' + esc(label) + "</div><div class=\"chips\">";
                    for (const r of rows) {
                        const p = r.p, on = b.picked.indexOf(p) >= 0;
                        const clash = b.picked.find((q) => q !== p && data.clashes(p, q));
                        const blocked = !on && (clash !== undefined || !fitting(b.picked.concat([p])).length);
                        const why = !blocked ? "" : clash !== undefined ? "cannot be combined with " + data.powerText(clash) : "the game never gives this together with your picks";
                        h += '<label class="pick' + (on ? " on" : "") + (blocked ? " off" : "") + '" title="' + esc(why || data.powers[p].key) + '"><input type="checkbox" data-p="' + p + '"' +
                            (on ? " checked" : "") + (blocked ? " disabled" : "") + ">" + (b.all && r.pi.tier ? '<span class="tier">L' + String(r.pi.tier).padStart(2, "0") + (r.pi.difficulty ? " " + esc(r.pi.difficulty) : "") + "</span>" : "") +
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
            const o = fit.length ? fit[0] : choices[0];
            const value = data.lootValue(itemId, b.picked);
            const d = data.describe(itemId, data.orderPowers(b.picked), value);
            const err = data.checkPowers(itemId, b.picked);
            let h = '<div class="detail">' + (editing ? '<h2>Edit bonuses</h2>' : "") + '<div class="preview">' + rowHtml(d, {}) + "</div>";
            h += '<div class="muted">Strongest bonuses of this item, up to ' + maxOf(o) + " at once. Greyed ones cannot go with what you picked." + (b.note ? " " + esc(b.note) : "") + "</div>";
            h += '<div class="row" style="margin:8px 0"><input id="psearch" type="search" placeholder="search bonuses (e.g. strength, fire, crit)" style="flex:1" value="' + esc(b.q) + '">' +
                '<span class="muted">picked ' + b.picked.length + "</span></div>";
            h += '<details class="alltiers"><summary class="muted">Advanced</summary><label class="muted"><input type="checkbox" id="alltiers"' + (b.all ? " checked" : "") + "> Show all tiers (weaker versions too)</label></details>";
            h += '<div id="picker">' + pickerHtml() + "</div>";
            if (editing) h += "<h2>Before and after</h2>" + compareHtml(entryDesc(editing), d, editing);
            else h += "<h2>Result</h2>" + detailTable(d, null) + "<h2>Bonuses</h2>" + powersHtml(d);
            h += '<div class="' + (err ? "err" : "ok") + '">' + (err ? esc(err) : "The original game can drop exactly this item.") + "</div>";
            const full = !editing && st.save.gear.items.length >= DH2.MAX_ITEMS;
            if (full) h += '<div class="err">The inventory is full (100 items).</div>';
            h += '<div class="row sticky"><button id="close">Cancel</button><button id="addit" class="primary"' + (err || full ? " disabled" : "") + ">" + (editing ? "Apply changes" : "Add to inventory") + "</button></div></div>";
            openSheet(h);
            $("close").onclick = closeSheet;
            $("alltiers").onchange = () => { b.all = $("alltiers").checked; const top = $("sheetbody").scrollTop; draw(); $("alltiers").closest("details").open = true; $("sheetbody").scrollTop = top; };
            $("psearch").oninput = () => { b.q = $("psearch").value; applySearch(); };
            applySearch();
            $("picker").onchange = (ev) => {
                const c = ev.target;
                if (!c.dataset || c.dataset.p === undefined) return;
                const p = +c.dataset.p;
                if (c.checked) b.picked.push(p); else b.picked = b.picked.filter((x) => x !== p);
                const top = $("sheetbody").scrollTop; draw(); $("sheetbody").scrollTop = top;
            };
            $("addit").onclick = () => {
                if (editing) {
                    try { st.save.gear.items[editIdx] = data.editEntry(editing, b.picked); } catch (e) { return fail(e); }
                    closeSheet(); setDirty(true); render();
                    return toast("Changed " + esc(d.name) + ". Press Save to write the file.");
                }
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

    // used by the automated proof scripts
    window.DH2App = { state: st, data: data, open: open, render: render, builder: builder, showItem: showItem, renderCatalog: renderCatalog,
        openGroup: (det) => { det.open = true; fillGroup(det); } };
    showFiles();
})();
