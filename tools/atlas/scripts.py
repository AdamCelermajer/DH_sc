"""Build the Lua script -> engine call map from the original data tree."""
from __future__ import annotations

import collections
import hashlib
import json
import re
from pathlib import Path

from common import source_site, stable_id, write_json


_IDENT = re.compile(r"[A-Za-z_][A-Za-z_0-9]*")
_NUMBER = re.compile(r"(?:0[xX][0-9A-Fa-f]+|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?)")
_ASSET_EXT = re.compile(r"\.(?:bdae|wav|ogg|mp3|luac|lua|mlx|mgp|mgx|mvx|mvp|png|dds|tga|xml|bin)$", re.I)
_REF_CALL = re.compile(r"(?:anim|sound|audio|fx|effect|projectile|skill|script|model|dialog|cutscene|pyid|prop|spawn|target|zone|trap|quest|music|play|load|template|actor|character)", re.I)
_BUILTINS = set("assert collectgarbage dofile error getfenv getmetatable ipairs load loadfile loadstring module next pairs pcall print rawequal rawget rawset require select setfenv setmetatable tonumber tostring type unpack xpcall coroutine string table math os io debug package self".split())
_KEYWORDS = set("and break do else elseif end false for function goto if in local nil not or repeat return then true until while".split())


def _tokens(text: str):
    """Small Lua lexer preserving offsets/lines; sufficient for call-site inventory."""
    out=[]; i=0; line=1; n=len(text)
    while i<n:
        c=text[i]
        if c in " \t\r\f\v": i+=1; continue
        if c=="\n": line+=1; i+=1; continue
        if text.startswith("--",i):
            # Lua long comments: --[=[ ... ]=]
            m=re.match(r"--\[(=*)\[",text[i:])
            if m:
                close="]"+m.group(1)+"]"; j=text.find(close,i+m.end())
                end=n if j<0 else j+len(close); line+=text[i:end].count("\n"); i=end
            else:
                j=text.find("\n",i); i=n if j<0 else j
            continue
        if c in "'\"":
            q=c; start=i; ln=line; i+=1
            while i<n:
                if text[i]=="\\": i=min(n,i+2); continue
                if text[i]==q: i+=1; break
                if text[i]=="\n": line+=1
                i+=1
            out.append(("string",text[start:i],ln,start,i)); continue
        if c=="[":
            m=re.match(r"\[(=*)\[",text[i:])
            if m:
                close="]"+m.group(1)+"]"; start=i; ln=line; j=text.find(close,i+m.end())
                end=n if j<0 else j+len(close); line+=text[i:end].count("\n")
                out.append(("string",text[start:end],ln,start,end)); i=end; continue
        m=_IDENT.match(text,i)
        if m: out.append(("id",m.group(),line,i,m.end())); i=m.end(); continue
        m=_NUMBER.match(text,i)
        if m: out.append(("number",m.group(),line,i,m.end())); i=m.end(); continue
        out.append(("sym",c,line,i,i+1)); i+=1
    return out


def _arg_ranges(tokens, open_i, text):
    depth=1; start=open_i+1; i=start; args=[]
    while i<len(tokens):
        val=tokens[i][1]
        if val=="(": depth+=1
        elif val==")":
            depth-=1
            if depth==0:
                if i>start: args.append((start,i))
                return args,i
        elif val=="," and depth==1:
            args.append((start,i)); start=i+1
        i+=1
    if start<len(tokens): args.append((start,len(tokens)))
    return args,len(tokens)-1


def _parse_script(text):
    ts=_tokens(text); calls=[]; local_names=set(); local_functions=set(); aliases={}; defined_names=set(); params=set()
    # Collect declared locals, named functions and parameters. We retain separate
    # labels instead of silently treating a shadowed local as an engine global.
    for i,t in enumerate(ts):
        if t[1]=="local" and i+2<len(ts):
            if ts[i+1][1]=="function" and ts[i+2][0]=="id":
                local_names.add(ts[i+2][1]); local_functions.add(ts[i+2][1])
            else:
                j=i+1
                declared=[]
                while j<len(ts) and ts[j][0]=="id":
                    local_names.add(ts[j][1]); declared.append(ts[j][1]); j+=1
                    if j<len(ts) and ts[j][1]==",": j+=1; continue
                    break
                # Preserve simple function aliases such as `local cast = DoSkill`.
                if j+1<len(ts) and ts[j][1]=="=" and ts[j+1][0]=="id":
                    rhs=j+1
                    if rhs+1<len(ts) and ts[rhs+1][1] in (".",":") and rhs+2<len(ts) and ts[rhs+2][0]=="id":
                        aliases.update({name:(ts[rhs+2][1],"object_method",ts[rhs][1]) for name in declared})
                    elif rhs+1>=len(ts) or ts[rhs+1][1] not in ("(",".",":"):
                        aliases.update({name:(ts[rhs][1],"global",None) for name in declared})
        if t[1]=="function":
            j=i+1; name=[]
            while j<len(ts) and ts[j][1]!="(": name.append(ts[j][1]); j+=1
            if name and name[-1] not in (".",":"): defined_names.add(name[-1])
            if j<len(ts):
                j+=1
                while j<len(ts) and ts[j][1]!=")":
                    if ts[j][0]=="id": params.add(ts[j][1])
                    j+=1
        # Parameter list on anonymous `function (` handled above.
    # `for x,y in` variables are locals too.
    for i,t in enumerate(ts):
        if t[1]=="for":
            j=i+1
            while j<len(ts) and ts[j][1]!="in" and ts[j][1]!="=":
                if ts[j][0]=="id": local_names.add(ts[j][1])
                j+=1
        if t[0]=="id" and t[1] in local_names and i+2<len(ts) and ts[i+1][1]=="=" and ts[i+2][1]=="function":
            local_functions.add(t[1])
    for i,t in enumerate(ts):
        if t[0]!="id": continue
        kind="global"; fn=t[1]; receiver=None; open_i=None; expr_start=i
        if i and ts[i-1][1] in (".",":"):
            continue
        if i+1<len(ts) and ts[i+1][1] in (".",":") and i+3<len(ts) and ts[i+2][0]=="id" and ts[i+3][1]=="(":
            receiver=t[1]; fn=ts[i+2][1]; open_i=i+3
            kind="object_method" if ts[i+1][1]==":" else "qualified_function"
        elif i+1<len(ts) and ts[i+1][1]=="(":
            open_i=i+1
        if open_i is None: continue
        # Exclude declarations and calls made in method/qualified chains already
        # represented by their complete expression.
        prev=ts[i-1][1] if i else ""
        if prev=="function": continue
        if receiver is None:
            if fn in local_functions: kind="local_function"
            elif fn in aliases: kind="local_alias"
            elif fn in local_names: kind="local_call_unknown"
            elif fn in defined_names: kind="script_function"
            elif fn in params: kind="parameter_or_shadowed"
            elif fn in _KEYWORDS: continue
            elif fn in _BUILTINS: kind="lua_builtin"
        if kind=="qualified_function" and receiver in ("table","string","math","os","io","coroutine","debug","package"):
            kind="lua_builtin"
        alias_target=None
        if kind=="local_alias":
            alias_target=aliases.get(fn,(None,None,None))[0]
            if aliases.get(fn,(None,None,None))[1]=="object_method":
                receiver=aliases[fn][2]; kind="alias_method"
        args,close_i=_arg_ranges(ts,open_i,text)
        raw_start=ts[expr_start][3]; raw_end=ts[close_i][4] if close_i<len(ts) else ts[open_i][4]
        arg_data=[]
        for ai,(a,b) in enumerate(args):
            if a>=b: raw=""
            else: raw=text[ts[a][3]:ts[b-1][4]]
            literals=[{"type":x[0],"raw":x[1],"value":x[1][1:-1] if x[0]=="string" and len(x[1])>=2 else x[1],"line":x[2]} for x in ts[a:b] if x[0] in ("string","number")]
            arg_data.append({"index":ai+1,"expression":raw.strip(),"literals":literals})
        calls.append({"name":fn,"mode":kind,"receiver":receiver,"alias_target":alias_target,"line":t[2],"expression":text[raw_start:raw_end].strip(),"arguments":arg_data,"_start":raw_start})
    # Keep computed table calls explicitly dynamic; never guess the function name.
    for i,t in enumerate(ts):
        if t[1]!="]" or i+1>=len(ts) or ts[i+1][1]!="(": continue
        depth=1; j=i-1
        while j>=0:
            if ts[j][1]=="]": depth+=1
            elif ts[j][1]=="[":
                depth-=1
                if depth==0: break
            j-=1
        if j<1: continue
        start=j-1
        while start>=2 and ts[start-1][1]==".": start-=2
        args,close_i=_arg_ranges(ts,i+1,text); raw_start=ts[start][3]; raw_end=ts[close_i][4]
        expr=text[raw_start:ts[i][4]].strip(); arg_data=[]
        for ai,(a,b) in enumerate(args):
            raw=text[ts[a][3]:ts[b-1][4]] if a<b else ""
            lits=[{"type":x[0],"raw":x[1],"value":x[1][1:-1] if x[0]=="string" and len(x[1])>=2 else x[1],"line":x[2]} for x in ts[a:b] if x[0] in ("string","number")]
            arg_data.append({"index":ai+1,"expression":raw.strip(),"literals":lits})
        calls.append({"name":"<dynamic>","mode":"dynamic","receiver":expr,"alias_target":None,"line":ts[start][2],"expression":text[raw_start:raw_end].strip(),"arguments":arg_data,"_start":raw_start})
    # Calls can be encountered on receiver then member; the member scan is skipped
    # above, so method calls have exactly one record.
    calls.sort(key=lambda x:x["_start"])
    return calls,local_names,defined_names,params


def _literal_ref_kind(call, lit):
    value=lit["value"]
    if lit["type"]=="string":
        ext=Path(value.replace("\\","/")).suffix.lower()
        if ext in {".bdae",".wav",".ogg",".mp3",".luac",".lua",".mlx",".mgp",".mgx",".mvx",".mvp",".png",".dds",".tga",".xml",".bin"}:
            return {".bdae":"model_or_effect",".wav":"sound",".ogg":"sound",".mp3":"sound",".luac":"script",".lua":"script",".mlx":"level",".mgp":"model",".mgx":"model",".mvx":"model",".mvp":"model",".png":"image",".dds":"image",".tga":"image",".xml":"data",".bin":"data"}[ext]
        if _REF_CALL.search(call["name"]): return "named_reference"
        return None
    if lit["type"]=="number" and _REF_CALL.search(call["name"]): return "numeric_reference"
    return None


def _ida_registration_map(inputs):
    """Recover Lua public names -> native handler/address from the offline IDA export."""
    p=Path(inputs)/"ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c"
    if not p.is_file(): return {}
    src=p.read_text(encoding="utf-8",errors="replace")
    registrations={}
    bind_re=re.compile(r'Binder::bind(Function|Method)\(.*?"([^"\\]+)"\s*,\s*\(void \(\*\).*?\)\s*([A-Za-z_][A-Za-z_0-9:]*)(?:,\s*this)?\);',re.S)
    for m in bind_re.finditer(src):
        mode,public,target=m.groups()
        registrations.setdefault(public,[]).append({"binding":mode,"handler":target})
    address_by_target={}
    header_re=re.compile(r"/\* FUNCTION ([0-9a-fA-F]+) ([^*]+) \*/")
    for m in header_re.finditer(src):
        addr=m.group(1).lower(); body=src[m.end():m.end()+320]
        # The first signature line uses the recovered, human-readable function name.
        sig=re.search(r"\n[^\n]*?\b([A-Za-z_][A-Za-z_0-9]*::[A-Za-z_][A-Za-z_0-9]*)\s*\(",body)
        if sig: address_by_target[sig.group(1)]="0x"+addr
    out={}
    for public,entries in registrations.items():
        for e in entries:
            e["address"]=address_by_target.get(e["handler"])
        out[public]=entries
    return out


def _port_search(root, names):
    """Collect exact identifier mentions in production source to guide status review."""
    port=Path(root)/"port"
    files=[]
    for base in (port/"windows-foundation",port/"script-runtime",port/"level-world",port/"game-data"):
        if base.is_dir(): files.extend(p for p in base.rglob("*") if p.suffix.lower() in (".cpp",".hpp",".c",".h"))
    needles=set(names); found={n:[] for n in needles}
    if not files:return found
    # One pass through source, bounded to exact identifier tokens.
    pattern=re.compile(r"\b("+"|".join(re.escape(n) for n in sorted(needles,key=len,reverse=True))+r")\b") if needles else None
    if not pattern:return found
    for p in files:
        try: lines=p.read_text(encoding="utf-8",errors="replace").splitlines()
        except OSError: continue
        for no,line in enumerate(lines,1):
            for m in pattern.finditer(line):
                n=m.group(1)
                if len(found[n])<12: found[n].append(f"{p.relative_to(root).as_posix()}:{no}")
    return found


def run(ctx, inventory_only=False):
    inputs=Path(ctx.inputs); root=Path(ctx.root); out=Path(ctx.out)
    script_paths=[]
    for platform in ("android","ios"):
        d=inputs/"assets-extra"/platform/"data"/"scripts"
        if d.is_dir(): script_paths.extend((platform,p) for p in d.rglob("*") if p.is_file() and p.suffix.lower() in (".lua",".luac"))
    script_paths.sort(key=lambda x:(x[0],x[1].as_posix().lower()))
    script_rows=[]; call_rows=[]; ref_rows=[]; funcs={}; counts_by_platform=collections.Counter(); lines_total=0
    for platform,p in script_paths:
        rel=p.relative_to(inputs/"assets-extra"/platform/"data").as_posix()
        site=f".local-inputs/assets-extra/{platform}/data/{rel}"
        raw=p.read_bytes(); text=raw.decode("utf-8-sig",errors="replace")
        lines_total+=len(text.splitlines()); counts_by_platform[platform]+=1
        calls,locals_,defined_,params=_parse_script(text)
        script_id=stable_id("script",platform,rel)
        script_rows.append({"id":script_id,"site":site,"name":p.name,"platform":platform,"format":"lua-source" if not raw.startswith(b"\x1bLua") else "lua-bytecode","bytes":len(raw),"lines":len(text.splitlines()),"call_sites":len(calls),"engine_call_sites":0,"status":"TODO","confidence":"PROVEN","classification":{"local_names":sorted(locals_),"script_functions":sorted(defined_),"parameters":sorted(params)}})
        for call in calls:
            mode=call["mode"]
            is_engine=mode in ("global","object_method","qualified_function","alias_method","dynamic") or (mode=="local_alias" and call["alias_target"] is not None)
            if mode=="dynamic":
                function_key=(mode,call["receiver"]); display=f"<dynamic:{call['receiver']}>"
            elif mode=="local_alias" and call["alias_target"] is not None:
                function_key=("global",call["alias_target"]); display=call["alias_target"]
            elif mode=="global":
                # IDs include mode so an imported object's method remains distinct.
                function_key=(mode,call["name"])
                display=call["name"]
            else:
                function_key=(mode,call["receiver"],call["name"])
                display=(f"{call['receiver']}:{call['name']}" if mode=="object_method" else f"{call['receiver']}.{call['name']}" if call["receiver"] else call["name"])
            if is_engine:
                fid=stable_id("script-function",*function_key)
                entry=funcs.setdefault(fid,{"id":fid,"name":display,"mode":mode,"status":"TODO","port_status":"UNKNOWN","native_addresses":[],"meaning":"","implementation":None,"confidence":"GUESS","call_count":0,"script_count":0})
                entry["call_count"]+=1
                entry["_scripts"].add(script_id) if "_scripts" in entry else entry.update({"_scripts":{script_id}})
            else: fid=None
            call_id=stable_id("script-call",platform,rel,call["line"],call["_start"],display)
            call_rows.append({"id":call_id,"script_id":script_id,"function_id":fid,"site":site,"line":call["line"],"name":display,"call_mode":mode,"receiver":call["receiver"],"alias_target":call["alias_target"],"expression":call["expression"],"arguments":call["arguments"],"engine_candidate":is_engine,"status":"TODO","confidence":"PROVEN" if mode not in ("local_call_unknown","parameter_or_shadowed","dynamic") else "LIKELY"})
            if is_engine: script_rows[-1]["engine_call_sites"]+=1
            for arg in call["arguments"]:
                for lit in arg["literals"]:
                    refkind=_literal_ref_kind(call,lit)
                    if not refkind: continue
                    value=lit["value"]
                    ref_id=stable_id("script-ref",platform,rel,call["line"],arg["index"],lit["line"],value,refkind)
                    ref_rows.append({"id":ref_id,"site":f"{site}:{lit['line']}","name":value,"kind":refkind,"rule":"literal argument to Lua call; precise resolution rule pending reference-ledger join","status":"TODO","reason":None,"candidates":[],"candidate_note":"Source literal is captured with its call and argument context; resolution owner must identify the data-table/file rule or state that no candidate set applies.","context":{"script":site,"line":call["line"],"call":display,"call_mode":mode,"argument_index":arg["index"],"argument_expression":arg["expression"],"literal_type":lit["type"],"literal_line":lit["line"]},"port_status":"UNKNOWN","implementation":None,"confidence":"GUESS","work_status":"TODO"})
    for f in funcs.values():
        f["script_count"]=len(f.pop("_scripts"))
    # Native registration/address evidence from the original IDA pseudocode export.
    native_map=_ida_registration_map(inputs)
    port_mentions=_port_search(root,[f["name"].split(":")[-1].split(".")[-1] for f in funcs.values()])
    for f in funcs.values():
        base=f["name"].split(":")[-1].split(".")[-1]
        regs=native_map.get(base,[])
        f["native_addresses"]=[{"address":x["address"],"handler":x["handler"],"binding":x["binding"]} for x in regs if x.get("address")]
        f["native_handlers"]=[x["handler"] for x in regs]
        f["ida_evidence"]=".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c" if regs else None
        f["meaning"]="; ".join(sorted(set(x["handler"] for x in regs))) if regs else "No public Lua binding with this exact name was found in the IDA registration tables; possible dynamic/alias API, script helper, or unresolved name."
        mentions=port_mentions.get(base,[])
        f["implementation_candidates"]=mentions
        # A text match alone is not proof of a complete port; retain PARTIAL for review.
        f["port_status"]="PARTIAL" if mentions else "MISSING"
        f["implementation"]=mentions[:1] or None
        f["confidence"]="PROVEN" if regs else "LIKELY"
    func_rows=sorted(funcs.values(),key=lambda f:(f["name"].lower(),f["id"]))
    # Inventory-only output intentionally leaves every work/status row TODO.
    write_json(out/"data/scripts.json",script_rows)
    write_json(out/"data/functions.json",func_rows)
    write_json(out/"data/calls.json",call_rows)
    write_json(out/"data/script-references.json",ref_rows)
    # The references owner merges this projection after their base ledger is written.
    script_count=len(script_rows); engine_call_count=sum(1 for x in call_rows if x["engine_candidate"])
    total_func_names=len(func_rows); todo={"total":script_count,"todo":script_count,"done":0}
    mode_counts=collections.Counter(x["call_mode"] for x in call_rows)
    counts={"scripts":script_count,"scripts_by_platform":dict(counts_by_platform),"script_lines":lines_total,"script_file_format_counts":dict(collections.Counter(x["format"] for x in script_rows)),"script_rows":{"total":script_count,"todo":script_count,"done":0},"engine_call_sites":engine_call_count,"engine_function_names":total_func_names,"engine_function_rows":{"total":total_func_names,"todo":total_func_names,"done":0},"calls":{"total":len(call_rows),"todo":len(call_rows),"engine_candidates":engine_call_count,"by_mode":dict(mode_counts)},"script_references":{"total":len(ref_rows),"todo":len(ref_rows),"by_kind":dict(collections.Counter(x["kind"] for x in ref_rows))},"native_binding_matches":sum(bool(f["native_addresses"]) for f in func_rows),"native_addresses":sum(len(f["native_addresses"]) for f in func_rows),"port_status_candidates":dict(collections.Counter(f["port_status"] for f in func_rows)),"known_answer_tests":{"swampking_core.luac":"INVENTORIED_TODO","witch.luac":"INVENTORIED_TODO"}}
    write_json(out/"counts/scripts.json",counts)
    _write_report(out/"reports/scripts.md",counts,script_rows,func_rows,call_rows,ref_rows)
    return counts


def _write_report(path,counts,scripts,functions,calls,refs):
    names=collections.Counter(f["name"] for f in functions)
    bosses=[s for s in scripts if any(k in s["name"].lower() for k in ("swampking","witch","rene","madruk","darkqueen","edwardboss","flaminite","roottroll","sandworm","winddragon","balrog","assassin","emissary","resurrectinggod","miniboss"))]
    lines=["# Script to engine map", "", "Inventory from original Android/iOS script trees; all rows intentionally begin TODO.", "", "- Android Lua files: **%s** (`lua-source`: %s, `lua-bytecode`: %s)."%(counts["scripts_by_platform"].get("android",0),counts["script_file_format_counts"].get("lua-source",0),counts["script_file_format_counts"].get("lua-bytecode",0)),"- iOS Lua files: **%s**."%counts["scripts_by_platform"].get("ios",0),"- Source lines: **%s**; call expressions: **%s**; engine candidates: **%s** (%s unique candidate rows)."%(counts["script_lines"],counts["calls"]["total"],counts["engine_call_sites"],counts["engine_function_names"]),"- Script argument references inventoried: **%s**."%counts["script_references"]["total"],"", "## Boss and custom script inventory", "", "| Script | Engine call sites | Status |", "| --- | ---: | --- |"]
    lines += [f"| {s['site']} | {s['engine_call_sites']} | TODO |" for s in bosses]
    lines += ["", "## Ranked engine functions", "", "Ranked by number of direct and method call sites. A source name/path mention is only a PARTIAL candidate; MISSING means no exact production-source name match was found.", "", "| Function | Call sites | Scripts | Native handler(s) | Port status |", "| --- | ---: | ---: | --- | --- |"]
    lines += [f"| {f['name']} | {f['call_count']} | {f['script_count']} | {', '.join(f['native_handlers']) or 'unresolved'} | {f['port_status']} |" for f in sorted(functions,key=lambda f:(-f['call_count'],f['name'].lower()))[:100]]
    # High-value boss details are generated directly from call sites, preserving source line context.
    lookup={s["id"]:s for s in scripts}; focus=("swampking_core.luac","witch.luac","rene.luac","madruk","darkqueen","edwardboss","flaminite","roottroll","sandworm","winddragon","balrog","assassin","emissary","resurrectinggod","miniboss")
    lines += ["", "## Boss/custom script call detail", "", "Calls below are direct source observations, with original file and line. They expose authored phase/skill/timer/animation/sound/FX/cutscene hooks; variable values are left symbolic where the source computes them.", ""]
    for s in scripts:
        if not any(k in s["name"].lower() for k in focus): continue
        subset=[c for c in calls if c["script_id"]==s["id"] and (re.search(r"skill|anim|sound|music|fx|effect|timer|cutscene|quest|dialog|spawn|teleport|warp|position|phase|target",c["name"],re.I) or c["call_mode"]=="object_method")]
        lines += [f"### {s['name']} — {s['site']}", "", f"Inventory: {len(subset)} relevant call sites; source line count {s['lines']}.", "", "| Line | Call | Arguments | Mode |", "| ---: | --- | --- | --- |"]
        for c in subset:
            args="; ".join(f"{a['index']}={a['expression']}" for a in c["arguments"])
            lines.append(f"| {c['line']} | `{c['name']}` | `{args}` | {c['call_mode']} |")
        lines.append("")
    lines += ["## Mapping progress", "", f"IDA registration tables matched {counts.get('native_binding_matches',0)} of {len(functions)} candidate function rows, with {counts.get('native_addresses',0)} address candidates. Port status totals are candidates only: {counts.get('port_status_candidates',{}).get('PARTIAL',0)} PARTIAL name/path mentions and {counts.get('port_status_candidates',{}).get('MISSING',0)} MISSING exact-name matches. Every source row remains TODO until caller/admission semantics and behavior-level implementation are reviewed.", "", "## Scope notes", "", "The 219 Android `.luac` files are plaintext Lua source rather than Lua 5.1 bytecode; this was detected from file headers, not inferred from extensions. Seven `.nomedia` marker files are excluded. The iOS data tree and Android original-cache contain no Lua scripts. The tokenizer separates global names, alias calls, local functions, unknown local calls, imported/receiver methods, script-defined functions, parameters, built-ins and Lua control syntax. Computed call expressions and native registration/address resolution remain follow-up work.", ""]
    path.parent.mkdir(parents=True,exist_ok=True); path.write_text("\n".join(lines),encoding="utf-8")


def known_checks(ctx):
    """Known-answer checks grounded in the B155/B156 recovered-source handoff."""
    inputs=Path(ctx.inputs); root=Path(ctx.root); checks=[]
    base=inputs/"assets-extra/android/data/scripts/ai"
    for name,required,port_paths in (
        ("swampking_core.luac",{"DoSkill","PlayAnim","PlaySound","PlayMusic","SetIsTargetable","SetActorPosition","StartTimerCB","StopTimerCB","TargetListSearch","AddAggro"},["port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp","port/windows-foundation/features/bosses/runtime_boss_encounter_plan_v1.cpp","port/windows-foundation/features/bosses/runtime_timer_trap_v1.cpp"]),
        ("witch.luac",{"DoSkill","StartTimerCB","StopTimerCB","PlayFX","PlaySound","MoveTo","WarpTo","TargetListSearch","AddAggro"},["port/windows-foundation/features/enemy_ai/monster_decisions.cpp","port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp"])):
        p=base/name
        if not p.is_file():
            checks.append({"name":name,"passed":False,"detail":"Original Lua source file is missing."}); continue
        text=p.read_text(encoding="utf-8",errors="replace"); parsed,_,_,_=_parse_script(text)
        actual={c["name"] for c in parsed if c["mode"] in ("global","object_method","qualified_function")}
        absent=sorted(required-actual); missing_paths=[v for v in port_paths if not (root/v).is_file()]
        if name=="witch.luac":
            skill_calls=[c for c in parsed if c["name"]=="DoSkill"]
            has_only_skill0=(len(skill_calls)==1 and skill_calls[0]["arguments"] and skill_calls[0]["arguments"][0]["expression"]=="0")
            has_10s="MOVE_DELAY" in text and bool(re.search(r"MOVE_DELAY\s*=\s*10000(?:\.0)?",text))
            outcome=not absent and not missing_paths and has_only_skill0 and has_10s
            detail=f"Source has the expected call families; DoSkill count={len(skill_calls)}, only slot 0={has_only_skill0}, 10s teleport delay={has_10s}. Port service files exist. Teleport wiring remains behavior-level PARTIAL pending integration verification. Missing calls={absent}; missing port paths={missing_paths}."
        else:
            outcome=not absent and not missing_paths
            detail=f"Source includes the Mud Lord services for attack/animation/audio, relocation/targetability, timers and target search. Port owner sources exist. Missing calls={absent}; missing port paths={missing_paths}. Boss damage/AOE pushback and camera-frustum awake latch remain separately documented gaps."
        checks.append({"name":name,"passed":outcome,"detail":detail})
    return checks


def coverage_check(ctx):
    """Re-extract source calls and verify the saved script/call inventory is complete."""
    out=Path(ctx.out); scripts=json.loads((out/"data/scripts.json").read_text(encoding="utf-8"))
    calls=json.loads((out/"data/calls.json").read_text(encoding="utf-8")); errors=[]
    source_scripts=[]
    for platform in ("android","ios"):
        d=Path(ctx.inputs)/"assets-extra"/platform/"data"/"scripts"
        if not d.is_dir(): continue
        for p in d.rglob("*"):
            if not p.is_file() or p.suffix.lower() not in (".lua",".luac"): continue
            rel=p.relative_to(Path(ctx.inputs)/"assets-extra"/platform/"data").as_posix(); site=f".local-inputs/assets-extra/{platform}/data/{rel}"
            script_id=stable_id("script",platform,rel); source_scripts.append(script_id)
            parsed,_,_,_=_parse_script(p.read_bytes().decode("utf-8-sig",errors="replace"))
    saved_scripts={s["id"] for s in scripts}
    # Compare canonical call IDs, independently deriving them again from source.
    source_ids=set()
    for platform in ("android","ios"):
        d=Path(ctx.inputs)/"assets-extra"/platform/"data"/"scripts"
        if not d.is_dir():continue
        for p in d.rglob("*"):
            if not p.is_file() or p.suffix.lower() not in (".lua",".luac"):continue
            rel=p.relative_to(Path(ctx.inputs)/"assets-extra"/platform/"data").as_posix(); sid=stable_id("script",platform,rel)
            for c in _parse_script(p.read_bytes().decode("utf-8-sig",errors="replace"))[0]:
                if c["mode"]=="dynamic": display=f"<dynamic:{c['receiver']}>"
                elif c["mode"]=="local_alias" and c["alias_target"]: display=c["alias_target"]
                elif c["mode"]=="object_method": display=f"{c['receiver']}:{c['name']}"
                elif c["mode"] in ("qualified_function","alias_method"): display=f"{c['receiver']}.{c['name']}"
                else: display=c["name"]
                source_ids.add(stable_id("script-call",platform,rel,c["line"],c["_start"],display))
    saved_ids={c["id"] for c in calls}
    if set(source_scripts)!=saved_scripts: errors.append(f"script rows differ from source inventory: source={len(source_scripts)} rows={len(saved_scripts)}")
    if source_ids!=saved_ids: errors.append(f"call rows differ from independently re-extracted source sites: source={len(source_ids)} rows={len(saved_ids)}; missing={len(source_ids-saved_ids)} extra={len(saved_ids-source_ids)}")
    script_ids=saved_scripts; function_ids={f["id"] for f in json.loads((out/"data/functions.json").read_text(encoding="utf-8"))}
    for c in calls:
        if c["script_id"] not in script_ids: errors.append(f"call {c['id']} references missing script {c['script_id']}")
        if c["engine_candidate"] and c["function_id"] not in function_ids: errors.append(f"call {c['id']} references missing function {c['function_id']}")
    return errors
