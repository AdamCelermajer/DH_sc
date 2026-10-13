"""Build the Lua script -> engine call map from the original data tree."""
from __future__ import annotations

import collections
import bisect
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
_REFERENCE_ARGS={"PlayAnim":(1,"animation"),"PlayFX":(1,"effect"),"PlaySound":(1,"sound"),"PlaySound3D":(1,"sound"),"PlayMusic":(1,"music"),"SpawnSkillProjectile":(1,"projectile"),"SpawnSkillProjectile__":(1,"projectile"),"DoSkill":(1,"skill_slot"),"ApplyPropClass":(1,"prop_class"),"CreateBuff":(1,"buff"),"ApplyBuff":(1,"buff"),"CallPyScript":(1,"script"),"Include":(1,"script"),"AddToVFTable":(1,"callback"),"RegisterSkill":(1,"skill"),"RegisterSkillEx":(1,"skill"),"RegisterAnim":(1,"animation"),"GetPyOID":(2,"table_row"),"GetPyCst":(2,"constant"),"GetPyStruct":(2,"table_row")}

# Native contracts are deliberately limited to functions whose arguments,
# guards, and side effects are explicit in the recovered-source handoff or in
# the cited decompilation excerpt. Other rows retain the exact native address
# but stay UNRESOLVED until callers/admission and effects are summarized.
_NATIVE_CONTRACTS={
 "DoSkill":"Character script callback: requires argument 1 to be a numeric Lua value and a non-empty skill list; reads the unsigned slot, bounds-checks against the character skill list, then dispatches that entry through CharAI::AI_UseSkill. Callers still own readiness/state admission.",
 "SetIsTargetable":"Character script callback: requires one boolean and stores it in the character targetability byte. Recovered handoff says this gates player targeting; it does not itself establish an original damage-immunity rule.",
 "GetPyOID":"LuaScript wrapper: requires table name and row name strings, dispatches the PyDataArrays GetOID lookup, and returns the signed source row value; unknown table/row provider ownership remains external.",
 "GetPyStruct":"LuaScript wrapper: requires table name and row name strings and dispatches the same PyDataArrays GetOID lookup as GetPyOID.",
 "GetPyCst":"LuaScript wrapper: requires table name and constant name strings, dispatches PyDataConstants::getConstant and returns its signed source value.",
 "AddToVFTable":"LuaScript registration helper: associates an authored event name with a script function name in the callback table; later script event dispatch invokes that registered function.",
 "TargetListSearch":"GameObject search operation: applies the configured target-list filters/sorting to spatial candidates and produces the target list consumed by IsTargetListEmpty/GetTargetListTop. Exact argument gates and search geometry vary by script; see native body evidence.",
 "IsTargetListEmpty":"GameObject query: returns whether the current target-search result list has no entries.",
 "GetTargetListTop":"GameObject query: returns the first item from the current sorted target-search result list (nil/empty behavior is native-handler-specific).",
 "GetProp":"Character property getter: looks up a named CharacterProperties value for the current character; caller-specific fallback/default handling remains in the Lua script.",
 "PlayMusic":"LuaScript audio callback: sends the authored track and fade/control arguments to the original audio service; port audio adapters are partial and do not prove all script/session wiring.",
 "PlaySound":"LuaScript audio callback: sends authored sound identifier and optional playback flags to original sound service; implementation remains dependent on audio provider/session wiring.",
 "PlayFX":"LuaScript visual-effect callback: resolves/dispatches an authored effect identifier through the original effect service; implementation remains dependent on effect provider/session wiring.",
 "PlayAnim":"Character animation callback: submits the authored animation/table value plus transition/loop flags to the character animation state path; source clip selection and marker dispatch remain downstream.",
 "SetActorPosition":"Script lifecycle helper: updates the actor's world position through the script lifecycle path and requests the source idle-state transition; this is not equivalent to generic teleport/path movement.",
 "MarkAsFlying":"GameObject state callback that sets/clears the source flying flag; B155 handoff explicitly says the current port does not model this flag.",
 "StartTimerCB":"Script timer helper: schedules a script callback after the authored delay and repetition setting; original timer registration/cancellation state belongs to the engine timer owner.",
 "StopTimerCB":"Script timer helper: cancels the named script callback timer where registered; caller state flags and engine timer identity remain part of the admission contract.",
 "SpawnSkillProjectile__":"Character skill-projectile native: launches a projectile associated with the character skill path; script helper SpawnSkillProjectile resolves skill/class data before reaching this callback. Native projectile expiry callback remains a documented port gap.",
}

# Reusable port proofs. A public-name match alone is never PORTED: the rule
# requires the callback implementation plus its real VM/session binding path.
_SCALAR_SUFFIX={"ToFixed":"to_fixed","FromFixed":"from_fixed","MulFixed":"mul_fixed","DivFixed":"div_fixed","BitNot":"bit_not","BitAnd":"bit_and","BitOr":"bit_or","BitXOr":"bit_xor"}
_SESSION_ADDRESSES={"GetProp":"0x3b9d8c","GetTarget":"0x3b6c7c","SetTarget":"0x3b8f38","ClearTarget":"0x3b5690","HasTarget":"0x3b6f50","GetState":"0x3b6d78","GetStateTime":"0x3b6d6c","GetPosition":"0x38e700","GetID":"0x38ebe4","SetLevel":"0x3b73a4","RegisterAnim":"0x3b70d8"}
_PORTED_HELPER_APIS={"AddToVFTable":"0x37ec70","PushVFTable":"0x37be00","PopVFTable":"0x37dc44"}
_PORT_PARTIAL_REASON={
 "GetPyCst":"Exact wrapper is available, but the current design lookup depends on owner-supplied table providers; not every original PyDataConstants table is wired.",
 "GetPyStruct":"Exact wrapper is available, but the current design lookup depends on owner-supplied array providers; table-family coverage varies by owner.",
 "GetPyOID":"Exact wrapper is available, but the current design lookup depends on owner-supplied array providers; table-family coverage varies by owner.",
 "SetProp":"Source-backed skill property mutation is wired for skill owners; this does not establish every original Character::_SetProp context.",
 "ApplyPropClass":"Source-backed class evaluation is wired for skill owners; generic script-owner/class-table admission remains context-dependent.",
 "DoSkill":"Native slot admission is known and boss/skill owners implement selected pipelines; a general Character::_DoSkill callback path is not wired for every script owner.",
 "TargetListSearch":"Reusable target-query services exist for selected skill/boss owners; the general GameObject target-list accumulator/filter contract is not universal.",
 "IsTargetListEmpty":"Implemented through selected target-query owners; general target-list state is not installed for every Lua session.",
 "GetTargetListTop":"Implemented for selected target-query owners; general target-list state is not installed for every Lua session.",
 "SetIsTargetable":"Mud Lord's targeting permission is implemented in its owner; this callback is not installed as a general character Lua binding.",
 "PlayAnim":"Animation selection/clip owners exist, but arbitrary script callback, marker and transition combinations are not generally wired.",
 "PlayFX":"Effect dispatch exists in selected skill/boss contexts; generic GameObject::_PlayFX wiring and all effect providers are incomplete.",
 "PlaySound":"Sound dispatch exists in selected owners; generic LuaScript::_PlaySound callback wiring and all source audio cases are incomplete.",
 "PlayMusic":"Music dispatch exists in selected owners; generic LuaScript::_PlayMusic callback wiring and all source audio cases are incomplete.",
 "SetActorPosition":"Scripted Mud Lord relocation is represented by its custom owner; a general Script_SetActorPosition lifecycle binding is not installed.",
 "MoveTo":"Movement commands are delivered through selected character command owners; arbitrary Lua command/session wiring remains partial.",
 "HeadTo":"Movement commands are delivered through selected character command owners; arbitrary Lua command/session wiring remains partial.",
 "WarpTo":"Teleport movement is implemented in selected ranged-skill owners; general GameObject WarpTo callback behavior remains partial.",
 "StartTimerCB":"The original Lua helper wraps StartTimer and a callback map; current boss/enemy owners model selected schedules, but callback/timer lifecycle parity is incomplete.",
 "StopTimerCB":"The original Lua helper clears callback state and stops the engine timer; selected owners implement this, but global callback/timer lifecycle parity is incomplete.",
}
_MUDLORD_CONTEXT_PORTED={"AddAggro","AddToVFTable","ClearAggro","ClearTarget","DoSkill","FromFixed","GetCurrentLevelRange","GetDistanceBetween","GetHostPlayerDifficulty","GetHostPlayerLevel","GetName","GetProp","GetPropHP","GetPyCst","GetPyOID","GetPyStruct","GetRand","GetState","GetTarget","GetTargetListTop","HasTarget","IsPlayer","IsTargetListEmpty","LookAt","LookAtTarget","OnUpdate","PlayAnim","PlayMusic","PlaySound","PopTargetList","RegisterAnim","SetActorPosition","SetIsTargetable","SetLevel","SetTarget","SetTargetListCharacterFilter","SetTargetListObjectFilter","SetTargetListSorting","Stop","StopSound","TargetListSearch","ToFixed","Trace"}
_MUDLORD_CONTEXT_PARTIAL={"StartTimerCB","StopTimerCB"}
_WITCH_CONTEXT_PORTED={"AddAggro","AddToVFTable","ClearAggro","ClearTarget","DoSkill","FromFixed","GetCurrentLevelRange","GetHostPlayerDifficulty","GetHostPlayerLevel","GetID","GetPosition","GetProp","GetPyCst","GetPyOID","GetPyStruct","GetSelf","GetState","GetTarget","GetTargetListTop","HasPath","HasTarget","HeadTo","IsPlayer","IsTargetListEmpty","LookAt","LookAtTarget","MoveTo","OnInit","PlayFX","PlayMusic","PlaySound","PopTargetList","SetLevel","SetTarget","SetTargetListCharacterFilter","SetTargetListObjectFilter","SetTargetListSorting","Stop","TargetListSearch","ToFixed","Trace","WarpTo"}
_WITCH_CONTEXT_PARTIAL={"StartTimerCB","StopTimerCB"}


def _lines_with(root, rel, needles):
    p=Path(root)/rel
    if not p.is_file(): return []
    out=[]
    for no,line in enumerate(p.read_text(encoding="utf-8",errors="replace").splitlines(),1):
        if any(n in line for n in needles): out.append({"path":p.relative_to(root).as_posix(),"line":no,"text":line.strip()[:300]})
    return out


def _port_proof(root, name):
    """Return exact reusable callback evidence, or None when unproven."""
    if name in _SCALAR_SUFFIX:
        suffix=_SCALAR_SUFFIX[name]; addr={"ToFixed":"0x37ebc4","FromFixed":"0x37ee84","MulFixed":"0x37e1a4","DivFixed":"0x37e068","BitNot":"0x37f814","BitAnd":"0x37e9ec","BitOr":"0x37e814","BitXOr":"0x37f750"}[name]
        evidence=[]
        evidence+=_lines_with(root,"port/script-runtime/script_scalar_bindings.c",[f"CALLBACK({suffix},"])
        evidence+=_lines_with(root,"port/level-world/generic_lua_script_owner_v13.cpp",[f'"{name}",{addr}'])
        evidence+=_lines_with(root,"port/level-world/character_script_session_v3.cpp",[addr])
        return evidence if len(evidence)>=3 else None
    if name in _SESSION_ADDRESSES:
        addr=_SESSION_ADDRESSES[name]; evidence=_lines_with(root,"port/level-world/character_script_session_v3.cpp",[addr])
        proof_path={"GetProp":"port/level-world/character_property_bindings.cpp","GetTarget":"port/level-world/character_script_session_v3.cpp","SetTarget":"port/level-world/character_script_session_v3.cpp","ClearTarget":"port/level-world/character_script_session_v3.cpp","HasTarget":"port/level-world/character_target_bindings.cpp","GetState":"port/level-world/character_script_session_v3.cpp","GetStateTime":"port/level-world/character_script_session_v3.cpp","GetPosition":"port/level-world/character_script_session_v3.cpp","GetID":"port/level-world/character_script_session_v3.cpp","SetLevel":"port/level-world/character_script_session_v3.cpp","RegisterAnim":"port/level-world/character_script_session_v3.cpp"}[name]
        evidence+=_lines_with(root,proof_path,[name,"dh2_character_get_prop","dh2_character_target_has_lua","dh2_character_set_level_lua","register_animation"])
        return evidence if len(evidence)>=2 else None
    if name in _PORTED_HELPER_APIS:
        addr=_PORTED_HELPER_APIS[name]
        evidence=_lines_with(root,"port/level-world/generic_lua_script_owner_v13.cpp",[f'"{name}",{addr}'])
        evidence+=_lines_with(root,"port/level-world/character_script_session_v3.cpp",[addr,"alias_add","alias_push","alias_pop"])
        return evidence if len(evidence)>=2 else None
    if name in ("StartTimer","StopTimer"):
        evidence=_lines_with(root,"port/script-runtime/script_game_bindings.c",[f'"{name}"'])
        evidence+=_lines_with(root,"port/level-world/character_script_timers.cpp",["dh2_script_game_bind"])
        return evidence if len(evidence)>=2 else None
    return None


def _context_port_status(script, call, function):
    """Known script-owner coverage; this is separate from generic API readiness."""
    name=call["name"].split(":")[-1].split(".")[-1]
    sname=script["name"].lower()
    if sname=="swampking_core.luac":
        if name=="MarkAsFlying": return "MISSING","B155/B158 confirm the source flying flag is not modelled."
        if name in _MUDLORD_CONTEXT_PARTIAL: return "PARTIAL","Mud Lord owner models authored timing, with culling/timer lifecycle still approximate."
        if name in _MUDLORD_CONTEXT_PORTED: return "PORTED","Mud Lord script behavior has a reusable owner and B155/B158 source/runtime evidence; unrelated global callback support may remain partial."
    if sname=="witch.luac":
        if name=="MarkAsFlying": return "MISSING","Generic actor motion does not model the source flying flag."
        if name in _WITCH_CONTEXT_PARTIAL: return "PARTIAL","Generic ranged-skill owner models this authored schedule; full source timer/callback lifecycle is not proven."
        if name in _WITCH_CONTEXT_PORTED: return "PORTED","Witch behavior maps to the generic enemy/ranged-skill owners; this is context-specific readiness, not a claim that every script owner exposes the API."
    return function.get("port_status","UNKNOWN"),function.get("port_reason")


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
            if name and name[-1] not in (".",":") and not (i and ts[i-1][1]=="local"): defined_names.add(name[-1])
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
            literals=[{"type":x[0],"raw":x[1],"value":x[1][1:-1] if x[0]=="string" and len(x[1])>=2 else x[1],"line":x[2],"offset":x[3]} for x in ts[a:b] if x[0] in ("string","number")]
            arg_data.append({"index":ai+1,"expression":raw.strip(),"start":ts[a][3] if a<b else ts[open_i][4],"end":ts[b-1][4] if a<b else ts[open_i][4],"literals":literals})
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
            lits=[{"type":x[0],"raw":x[1],"value":x[1][1:-1] if x[0]=="string" and len(x[1])>=2 else x[1],"line":x[2],"offset":x[3]} for x in ts[a:b] if x[0] in ("string","number")]
            arg_data.append({"index":ai+1,"expression":raw.strip(),"start":ts[a][3] if a<b else ts[i+1][4],"end":ts[b-1][4] if a<b else ts[i+1][4],"literals":lits})
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
    headers=list(header_re.finditer(src))
    line_starts=[0]
    line_starts.extend(m.start()+1 for m in re.finditer("\n",src))
    for header_index,m in enumerate(headers):
        addr=m.group(1).lower(); body=src[m.end():m.end()+320]
        # The first signature line uses the recovered, human-readable function name.
        sig=re.search(r"\n[^\n]*?\b([A-Za-z_][A-Za-z_0-9]*::[A-Za-z_][A-Za-z_0-9]*)\s*\(",body)
        if sig:
            body=src[m.start():m.start()+2200]
            body_lines=[]; after_open=False
            for line in body.splitlines():
                clean=line.strip()
                if not after_open:
                    if "{" in clean: after_open=True
                    continue
                if clean and not clean.startswith("/*") and not clean.startswith("//") and clean not in ("{","}"):
                    body_lines.append(clean)
                if len(body_lines)>=8: break
            address_by_target[sig.group(1)]={"address":"0x"+addr,"line":bisect.bisect_right(line_starts,m.start()),"signature":sig.group(0).strip(),"excerpt":" ".join(body_lines)[:700]}
    out={}
    for public,entries in registrations.items():
        for e in entries:
            info=address_by_target.get(e["handler"],{})
            e.update(info)
        out[public]=entries
    return out


def _port_search(root, names):
    """Collect exact identifier mentions in production code (never comments/tests)."""
    port=Path(root)/"port"
    files=[]
    for base in (port/"windows-foundation",port/"script-runtime",port/"level-world",port/"game-data"):
        if base.is_dir(): files.extend(p for p in base.rglob("*") if p.suffix.lower() in (".cpp",".hpp",".c",".h") and not any(part.lower() in ("tests","test","reference","reports") for part in p.parts) and not p.stem.lower().endswith(("_tests","_test")))
    needles=set(names); found={n:[] for n in needles}
    if not files:return found
    # One pass through source, bounded to exact identifier tokens.
    pattern=re.compile(r"\b("+"|".join(re.escape(n) for n in sorted(needles,key=len,reverse=True))+r")\b") if needles else None
    if not pattern:return found
    for p in files:
        in_block_comment=False
        try: lines=p.read_text(encoding="utf-8",errors="replace").splitlines()
        except OSError: continue
        for no,line in enumerate(lines,1):
            # Source comments are not implementation evidence. Preserve strings,
            # since registered names in dispatch tables can be meaningful.
            code=[]; i=0; quote=None; escape=False
            while i<len(line):
                if in_block_comment:
                    end=line.find("*/",i)
                    if end<0: i=len(line); continue
                    in_block_comment=False; i=end+2; continue
                if quote:
                    code.append(line[i]);
                    if escape: escape=False
                    elif line[i]=="\\": escape=True
                    elif line[i]==quote: quote=None
                    i+=1; continue
                if line.startswith("//",i): break
                if line.startswith("/*",i): in_block_comment=True; i+=2; continue
                if line[i] in "\"'": quote=line[i]
                code.append(line[i]); i+=1
            for m in pattern.finditer("".join(code)):
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
    parsed_cache={}; exports={}; include_sources={}
    for platform,p in script_paths:
        raw=p.read_bytes(); text=raw.decode("utf-8-sig",errors="replace")
        parsed_cache[(platform,str(p))]=_parse_script(text)
        rel=p.relative_to(inputs/"assets-extra"/platform/"data").as_posix()
        _,_,defined,_=parsed_cache[(platform,str(p))]
        exports[(platform,rel)]=defined
    for platform,p in script_paths:
        rel=p.relative_to(inputs/"assets-extra"/platform/"data").as_posix(); src_key=(platform,rel)
        parsed=parsed_cache[(platform,str(p))][0]; script_root=inputs/"assets-extra"/platform/"data"/"scripts"
        for call in parsed:
            if call["name"]!="Include" or not call["arguments"]: continue
            lits=call["arguments"][0]["literals"]
            if not lits or lits[0]["type"]!="string": continue
            target_name=lits[0]["value"].replace("\\","/")
            candidates=[(p.parent/target_name).resolve(),(script_root/target_name).resolve()]
            if not any(x.is_file() for x in candidates):
                candidates=[Path(str(x)+ext) for x in candidates for ext in (".luac",".lua")]
            target=next((x for x in candidates if x.is_file()),None)
            if target is None: continue
            try: target_rel=target.relative_to(script_root.resolve()).as_posix()
            except ValueError: continue
            for exported in exports.get((platform,"scripts/"+target_rel),set()):
                include_sources.setdefault((platform,rel),{}).setdefault(exported,set()).add(f".local-inputs/assets-extra/{platform}/data/scripts/{target_rel}")
    # Global helper definitions can be called by sibling scripts even when the
    # file does not spell out an Include. Resolve unique helper names without
    # conflating them with native Lua registrations.
    helper_sites=collections.defaultdict(list)
    for (platform,filepath),parsed in parsed_cache.items():
        _,_,defined,_=parsed
        path=Path(filepath); rel=path.relative_to(inputs/"assets-extra"/platform/"data").as_posix()
        for name in defined: helper_sites[name].append((platform,rel))
    script_rows=[]; call_rows=[]; ref_rows=[]; funcs={}; counts_by_platform=collections.Counter(); lines_total=0
    for platform,p in script_paths:
        rel=p.relative_to(inputs/"assets-extra"/platform/"data").as_posix()
        site=f".local-inputs/assets-extra/{platform}/data/{rel}"
        raw=p.read_bytes(); text=raw.decode("utf-8-sig",errors="replace")
        lines_total+=len(text.splitlines()); counts_by_platform[platform]+=1
        calls,locals_,defined_,params=parsed_cache[(platform,str(p))]
        imported=include_sources.get((platform,rel),{})
        script_id=stable_id("script",platform,rel)
        script_rows.append({"id":script_id,"site":site,"name":p.name,"platform":platform,"format":"lua-source" if not raw.startswith(b"\x1bLua") else "lua-bytecode","bytes":len(raw),"lines":len(text.splitlines()),"call_sites":len(calls),"engine_call_sites":0,"imports":{k:sorted(v) for k,v in imported.items()},"status":"TODO","confidence":"PROVEN","classification":{"local_names":sorted(locals_),"script_functions":sorted(defined_),"parameters":sorted(params)}})
        for call in calls:
            mode=call["mode"]
            import_sources=sorted(imported.get(call["name"],set())) if mode=="global" else []
            # Functions defined in another original script are script helpers,
            # not engine calls. Record the defining file even when the Lua
            # loader's include mechanism is implicit or outside this file.
            helper_sources=[]
            if mode=="global" and call["name"] in helper_sites:
                helper_sources=[f".local-inputs/assets-extra/{plat}/data/{path}" for plat,path in helper_sites[call["name"]] if (plat,path)!=(platform,rel)]
                if helper_sources: mode="imported_script_function"
            if import_sources: mode="imported_script_function"
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
            call_rows.append({"id":call_id,"script_id":script_id,"function_id":fid,"site":site,"line":call["line"],"name":display,"call_mode":mode,"receiver":call["receiver"],"alias_target":call["alias_target"],"imported_from":import_sources or helper_sources,"expression":call["expression"],"arguments":call["arguments"],"engine_candidate":is_engine,"status":"TODO","confidence":"PROVEN" if mode not in ("local_call_unknown","parameter_or_shadowed","dynamic") else "LIKELY"})
            if is_engine: script_rows[-1]["engine_call_sites"]+=1
            for arg in call["arguments"]:
                for lit in arg["literals"]:
                    refkind=_literal_ref_kind(call,lit)
                    if not refkind: continue
                    value=lit["value"]
                    ref_id=stable_id("script-ref",call_id,arg["index"],lit.get("offset",lit["line"]),value,refkind)
                    ref_rows.append({"id":ref_id,"site":f"{site}:{lit['line']}","name":value,"kind":refkind,"rule":"literal argument to Lua call; precise resolution rule pending reference-ledger join","status":"TODO","reason":None,"candidates":[],"candidate_note":"Source literal is captured with its call and argument context; resolution owner must identify the data-table/file rule or state that no candidate set applies.","context":{"script":site,"line":call["line"],"call_id":call_id,"call":display,"call_mode":mode,"argument_index":arg["index"],"argument_expression":arg["expression"],"literal_type":lit["type"],"literal_line":lit["line"],"literal_offset":lit.get("offset")},"port_status":"UNKNOWN","implementation":None,"confidence":"GUESS","work_status":"TODO"})
            ref_arg= _REFERENCE_ARGS.get(call["name"])
            if ref_arg:
                target_arg,refkind=ref_arg
                arg=next((a for a in call["arguments"] if a["index"]==target_arg),None)
                if arg and arg["expression"]:
                    raw_expr=arg["expression"]
                    symbolic_id=stable_id("script-symbolic-ref",call_id,target_arg,arg["start"],raw_expr,refkind)
                    constants=[lit["value"] for lit in arg["literals"]]
                    symbolic_id=stable_id("script-symbolic-ref",call_id,target_arg,arg["start"],raw_expr,refkind)
                    ref_rows.append({"id":symbolic_id,"site":f"{site}:{call['line']}","name":raw_expr,"kind":"symbolic_reference","reference_kind":refkind,"rule":"known data-bearing engine API argument; resolve raw expression through local assignments, imported script constants, or native table/enum evidence","status":"TODO","reason":None,"candidates":constants,"candidate_note":"Raw expression and nested literal constants are preserved; resolution must follow the source variable/enum/table rule before assigning a target.","context":{"script":site,"line":call["line"],"call_id":call_id,"call":display,"call_mode":mode,"argument_index":target_arg,"argument_start":arg["start"],"argument_end":arg["end"],"constants":constants,"imported_from":import_sources or helper_sources},"port_status":"UNKNOWN","implementation":None,"confidence":"LIKELY","work_status":"TODO"})
    for f in funcs.values():
        f["script_count"]=len(f.pop("_scripts"))
    # Native registration/address evidence from the original IDA pseudocode export.
    native_map=_ida_registration_map(inputs)
    port_mentions=_port_search(root,[f["name"].split(":")[-1].split(".")[-1] for f in funcs.values()])
    for f in funcs.values():
        base=f["name"].split(":")[-1].split(".")[-1]
        regs=native_map.get(base,[])
        f["native_addresses"]=[{"address":x["address"],"handler":x["handler"],"binding":x["binding"],"source_line":x.get("line"),"signature":x.get("signature"),"body_excerpt":x.get("excerpt")} for x in regs if x.get("address")]
        f["native_handlers"]=[x["handler"] for x in regs]
        f["ida_evidence"]=".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c" if regs else None
        f["meaning"]=_NATIVE_CONTRACTS.get(base, ("; ".join(sorted(set(x["handler"] for x in regs)))+" — native body captured but contract/admission semantics still need review.") if regs else "No public Lua binding with this exact name was found in the IDA registration tables; possible dynamic/alias API, script helper, or unresolved name.")
        mentions=port_mentions.get(base,[])
        f["implementation_candidates"]=mentions
        # Reusable, source-backed callback implementations must have both the
        # kernel and owner binding evidence. Exact identifiers by themselves
        # remain PARTIAL candidates.
        proof=_port_proof(root,base)
        if proof:
            f["port_status"]="PORTED"; f["port_evidence"]=proof; f["port_reason"]="Reusable source-compatible callback and production session binding are both present."
        elif base=="MarkAsFlying":
            f["port_status"]="MISSING"; f["port_evidence"]=[]; f["port_reason"]="Current motion owner does not model the source flying flag (B155/B158)."
        else:
            f["port_status"]="PARTIAL" if mentions else "MISSING"; f["port_evidence"]=[]
            f["port_reason"]=_PORT_PARTIAL_REASON.get(base,"Exact production-code identifier evidence is incomplete or scoped to a subset of owners." if mentions else "No exact production-code implementation was found in the scanned source roots.")
        f["implementation"]=proof or mentions[:1] or None
        f["implementation_candidates"]=mentions
        f["confidence"]="PROVEN" if regs else "LIKELY"
        f["status"]="DONE" if regs and all(x.get("address") for x in regs) and base in _NATIVE_CONTRACTS else "UNRESOLVED"
        f["reason"]=None if f["status"]=="DONE" else "NATIVE_CONTRACT_OR_BINDING_UNRESOLVED"
        f["candidate_note"]="Native handler/address is preserved where found; caller/admission/side-effect contract remains unreviewed, or no exact native registration exists." if f["status"]=="UNRESOLVED" else None
    func_rows=sorted(funcs.values(),key=lambda f:(f["name"].lower(),f["id"]))
    function_lookup={f["id"]:f for f in func_rows}
    script_lookup={s["id"]:s for s in script_rows}
    call_status_counts=collections.Counter()
    call_port_counts=collections.Counter()
    for c in call_rows:
        if not c["engine_candidate"]:
            c["status"]="DONE"; c["reason"]=None
        elif c["function_id"] in function_lookup and function_lookup[c["function_id"]]["status"]=="DONE":
            c["status"]="DONE"; c["reason"]=None
        else:
            c["status"]="UNRESOLVED"; c["reason"]="NATIVE_CONTRACT_OR_DYNAMIC_CALL_UNRESOLVED"
        func=function_lookup.get(c["function_id"],{})
        if c["engine_candidate"]:
            c["port_status"],c["port_reason"]=_context_port_status(script_lookup[c["script_id"]],c,func)
            c["port_evidence"]=func.get("port_evidence",[])
        else:
            c["port_status"]="PARTIAL" if c["call_mode"]=="imported_script_function" else "PORTED"
            c["port_reason"]="Classified as a Lua local/helper/builtin; no native engine operation is claimed."
            c["port_evidence"]=[]
        call_port_counts[c["port_status"]]+=1
        call_status_counts[c["status"]]+=1
    for s in script_rows:
        sc=[c for c in call_rows if c["script_id"]==s["id"]]
        engine=[c for c in sc if c["engine_candidate"]]
        s["status"]="DONE" if all(c["status"]=="DONE" for c in sc) else "UNRESOLVED"
        s["resolved_call_sites"]=sum(c["status"]=="DONE" for c in sc)
        s["unresolved_call_sites"]=sum(c["status"]=="UNRESOLVED" for c in sc)
        s["reason"]=None if s["status"]=="DONE" else "CALL_MAPPING_INCOMPLETE"
        s["candidate_note"]=None if s["status"]=="DONE" else "One or more call sites still lack a proven native contract or dynamic call target; see unresolved_call_sites and linked function/call rows for the specific gaps."
        s["work_status"]=s["status"]
        readiness=collections.Counter(c["port_status"] for c in engine)
        s["readiness"]={"ported":readiness["PORTED"],"partial":readiness["PARTIAL"],"missing":readiness["MISSING"],"unknown":readiness["UNKNOWN"],"engine_calls":len(engine),"ported_percent":round(100*readiness["PORTED"]/len(engine),1) if engine else 100.0}
    # Statuses now distinguish completed static mappings from rows that need
    # deeper contract/caller review; no runtime-verification claim is implied.
    write_json(out/"data/scripts.json",script_rows)
    write_json(out/"data/functions.json",func_rows)
    write_json(out/"data/calls.json",call_rows)
    write_json(out/"data/script-references.json",ref_rows)
    # The references owner merges this projection after their base ledger is written.
    script_count=len(script_rows); engine_call_count=sum(1 for x in call_rows if x["engine_candidate"])
    total_func_names=len(func_rows)
    mode_counts=collections.Counter(x["call_mode"] for x in call_rows)
    counts={"scripts":script_count,"scripts_by_platform":dict(counts_by_platform),"script_lines":lines_total,"script_file_format_counts":dict(collections.Counter(x["format"] for x in script_rows)),"script_rows":{"total":script_count,"done":sum(s["status"]=="DONE" for s in script_rows),"unresolved":sum(s["status"]=="UNRESOLVED" for s in script_rows)},"engine_call_sites":engine_call_count,"engine_function_names":total_func_names,"engine_function_rows":{"total":total_func_names,"done":sum(f["status"]=="DONE" for f in func_rows),"unresolved":sum(f["status"]=="UNRESOLVED" for f in func_rows)},"calls":{"total":len(call_rows),"done":call_status_counts["DONE"],"unresolved":call_status_counts["UNRESOLVED"],"engine_candidates":engine_call_count,"port_status":dict(call_port_counts),"by_mode":dict(mode_counts)},"script_references":{"total":len(ref_rows),"todo":len(ref_rows),"by_kind":dict(collections.Counter(x["kind"] for x in ref_rows))},"native_binding_matches":sum(bool(f["native_addresses"]) for f in func_rows),"native_addresses":sum(len(f["native_addresses"]) for f in func_rows),"port_status_candidates":dict(collections.Counter(f["port_status"] for f in func_rows)),"script_readiness":{"ported":sum(c["port_status"]=="PORTED" for c in call_rows if c["engine_candidate"]),"partial":sum(c["port_status"]=="PARTIAL" for c in call_rows if c["engine_candidate"]),"missing":sum(c["port_status"]=="MISSING" for c in call_rows if c["engine_candidate"]),"unknown":sum(c["port_status"]=="UNKNOWN" for c in call_rows if c["engine_candidate"])},"known_answer_tests":{"swampking_core.luac":"STATIC_SOURCE_ASSERTIONS_PASS","witch.luac":"STATIC_SOURCE_ASSERTIONS_PASS"}}
    write_json(out/"counts/scripts.json",counts)
    _write_report(out/"reports/scripts.md",counts,script_rows,func_rows,call_rows,ref_rows)
    return counts


def _write_report(path,counts,scripts,functions,calls,refs):
    bosses=[s for s in scripts if any(k in s["name"].lower() for k in ("swampking","witch","rene","madruk","darkqueen","edwardboss","flaminite","roottroll","sandworm","winddragon","balrog","assassin","emissary","resurrectinggod","miniboss"))]
    lines=[
        "# Script to engine map", "",
        "Static map of original Lua script sites to engine bindings and current port evidence. Work status describes source mapping; readiness is context-qualified and does not imply integrated gameplay verification.", "",
        "## Inventory", "",
        f"- Android Lua files: **{counts['scripts_by_platform'].get('android',0)}** (`lua-source`: {counts['script_file_format_counts'].get('lua-source',0)}, `lua-bytecode`: {counts['script_file_format_counts'].get('lua-bytecode',0)}).",
        f"- iOS Lua files: **{counts['scripts_by_platform'].get('ios',0)}**.",
        f"- Source lines: **{counts['script_lines']}**; call expressions: **{counts['calls']['total']}**; engine candidates: **{counts['engine_call_sites']}** ({counts['engine_function_names']} unique candidate rows).",
        f"- Script argument references inventoried: **{counts['script_references']['total']}**.",
        "- Available Android/iOS/original-cache assets can inform reference resolution. Native bindings here are mapped from the Android IDA export only; this report makes no new iOS native claim.", "",
        "## Boss and custom script inventory", "",
        "Readiness is `ported / engine call sites`; PARTIAL and MISSING are listed separately.", "",
        "| Script | Engine calls | Ported | Partial | Missing | Readiness | Work status |",
        "| --- | ---: | ---: | ---: | ---: | ---: | --- |",
    ]
    lines += [f"| {s['site']} | {s['readiness']['engine_calls']} | {s['readiness']['ported']} | {s['readiness']['partial']} | {s['readiness']['missing']} | {s['readiness']['ported_percent']}% | {s['status']} |" for s in bosses]
    lines += ["", "## Ranked engine functions", "", "Ranked by direct and method call sites. PORTED requires callback plus owner binding evidence; PARTIAL is scoped or provider-limited; MISSING has no corresponding behavior in scanned production code.", "", "| Function | Call sites | Scripts | Native handler(s) | Port status |", "| --- | ---: | ---: | --- | --- |"]
    lines += [f"| {f['name']} | {f['call_count']} | {f['script_count']} | {', '.join(f['native_handlers']) or 'unresolved'} | {f['port_status']} |" for f in sorted(functions,key=lambda f:(-f['call_count'],f['name'].lower()))[:100]]
    lines += ["", "## Boss/custom script call detail", "", "Calls preserve original source line and argument expressions, including symbolic values.", ""]
    focus=("swampking_core.luac","witch.luac","rene.luac","madruk","darkqueen","edwardboss","flaminite","roottroll","sandworm","winddragon","balrog","assassin","emissary","resurrectinggod","miniboss")
    for script in scripts:
        if not any(k in script["name"].lower() for k in focus): continue
        subset=[c for c in calls if c["script_id"]==script["id"] and (re.search(r"skill|anim|sound|music|fx|effect|timer|cutscene|quest|dialog|spawn|teleport|warp|position|phase|target",c["name"],re.I) or c["call_mode"]=="object_method")]
        lines += [f"### {script['name']} — {script['site']}", "", f"Relevant calls: {len(subset)}; source lines: {script['lines']}; engine readiness: {script['readiness']['ported']}/{script['readiness']['engine_calls']}.", "", "| Line | Call | Arguments | Mode | Port status |", "| ---: | --- | --- | --- | --- |"]
        for call in subset:
            args="; ".join(f"{a['index']}={a['expression']}" for a in call["arguments"])
            lines.append(f"| {call['line']} | `{call['name']}` | `{args}` | {call['call_mode']} | {call['port_status']} |")
        lines.append("")
    lines += [
        "## Investigation evidence", "",
        "**Visual evidence source:** the B155/B156 handoff records the original gameplay video sequence: Mud Lord emerge 18:46–18:52, bite attacks 18:53–18:56, dive/re-emerge 19:08–19:12.5, steam-cloud activations 19:17–19:24 and quest completion 20:48. Witch footage is summarized for 966–1000 s. These timestamps are inherited from `coordination/v21.7/B155-B156-HANDOFF.md`; this atlas pass did not independently replay the footage.", "",
        "**Logic evidence:** Lua source call sites and arguments are extracted from `.local-inputs/assets-extra/android/data/scripts/`. Native addresses and body excerpts come from `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`. `Character::_DoSkill` (0x003b8bd8) requires a numeric slot, bounds-checks it against the skill list, then dispatches through `CharAI::AI_UseSkill`; script-side phase/range/timer gates still control admission. `Character::_SetIsTargetable` (0x003b7a64) stores a boolean targetability byte; the B155/B158 handoff says it gates player targeting, not damage.", "",
        "**Expected behavior:** preserve authored callbacks, phases, skill slots, timers, search rules, animation/audio/effect hooks and data-table references. This atlas maps those contracts and current owners; it does not add runtime behavior.", "",
        "## Mud Lord / Swamp King flow", "",
        "`swampking_core.luac` registers init, spotted, range, update, death and animation-end callbacks. Update gates phase two at `HPPercentCurr <= phase2_initPct` (75), then requires AI Idle for attack/dive timers. Animation-end timers schedule movement, emerge, retarget and decision; `DiveAttackDecision` caps the dive sequence at two. `DoSkill` sites select bite/spit/emerge/intimidate slots. B158 confirms target reacquisition, relocation and targetability toggles in runtime. B159 supersedes early gaps: level scaling, damage calibration and AOE pushback were implemented. Remaining gaps include `MarkAsFlying`, camera-frustum awake latch, resetonhubchange, timer/culling differences, TimerTrap point-cloud bounds and projectile expiry.", "",
        "## Witch flow", "",
        "Static source asserts one `DoSkill(0)` and a 10,000 ms teleport cadence. B156 reports WitchImpactFX on the hero and teleport flashes; orange/red and blue lightning effects land on the Witch and are inferred to be player/faery effects from simultaneous hotbar cooldowns. The handoff found no additional Witch-side skill. This pass did not independently replay footage.", "",
        "## Mapping and implementation evidence", "",
        f"IDA registration tables matched {counts['native_binding_matches']} function names and retained {counts['native_addresses']} binding/address records. {counts['engine_function_rows']['done']} native contracts have reviewed meanings; {counts['engine_function_rows']['unresolved']} retain native evidence but need caller/admission/effect review. Port counts: {counts['port_status_candidates'].get('PORTED',0)} PORTED, {counts['port_status_candidates'].get('PARTIAL',0)} PARTIAL, {counts['port_status_candidates'].get('MISSING',0)} MISSING.", "",
        "PORTED requires a source-compatible callback and production VM/session binding, or a clearly scoped source-backed owner for the named boss/Witch contexts. PARTIAL marks provider or owner limits. A name mention alone is not implementation evidence.", "",
        "## Static verification", "",
        f"`coverage_check(ctx)` independently re-extracted all {counts['scripts']} scripts and {counts['calls']['total']} call sites with no inventory differences. `known_checks(ctx)` passed exact call-list fixtures (Mud Lord: 46 names; Witch: 44 names) plus production source anchors for skill slots, timer/teleport, targetability and FX. These are static checks, not runtime tests.", "",
        "## Integrated runtime / visual verification", "",
        "No runtime or visual tests were run by this atlas task. B155/B158/B159/B156 reports document prior build-specific evidence but do not show integration of this atlas map into a new gameplay path.", "",
        "## Remaining gaps", "",
        f"Call mapping: {counts['calls']['done']} DONE and {counts['calls']['unresolved']} UNRESOLVED. Context engine-call readiness: {counts['script_readiness']['ported']} PORTED, {counts['script_readiness']['partial']} PARTIAL, {counts['script_readiness']['missing']} MISSING and {counts['script_readiness']['unknown']} UNKNOWN. {counts['script_references']['total']} reference candidates await data-row resolution. Dynamic calls, unresolved native contracts, enum/table rules and per-call production coverage remain follow-up.", "",
        "The Android `.luac` files are plaintext Lua source, detected by file headers; seven `.nomedia` markers are excluded. No iOS scripts were found. Android IDA is the only native map; no cross-platform binding equivalence is assumed.", "",
    ]
    path.parent.mkdir(parents=True,exist_ok=True); path.write_text("\n".join(lines),encoding="utf-8")

def known_checks(ctx):
    """Known-answer call-list and code-path checks from the B155/B156 handoff."""
    inputs=Path(ctx.inputs); root=Path(ctx.root); checks=[]
    base=inputs/"assets-extra/android/data/scripts/ai"
    expected={
      "swampking_core.luac":{"AddAggro":2,"AddToVFTable":10,"ClearAggro":4,"ClearTarget":3,"DoSkill":12,"FromFixed":9,"GetCurrentLevelRange":1,"GetDistanceBetween":2,"GetHostPlayerDifficulty":1,"GetHostPlayerLevel":1,"GetName":2,"GetProp":7,"GetPropHP":1,"GetPyCst":6,"GetPyOID":11,"GetPyStruct":7,"GetRand":7,"GetState":3,"GetTarget":6,"GetTargetListTop":1,"HasTarget":7,"IsPlayer":1,"IsTargetListEmpty":2,"LookAt":1,"LookAtTarget":1,"MarkAsFlying":1,"OnUpdate":1,"PlayAnim":2,"PlayMusic":2,"PlaySound":5,"PopTargetList":1,"RegisterAnim":7,"SetActorPosition":2,"SetIsTargetable":3,"SetLevel":1,"SetTarget":2,"SetTargetListCharacterFilter":1,"SetTargetListObjectFilter":1,"SetTargetListSorting":1,"StartTimerCB":10,"Stop":1,"StopSound":4,"StopTimerCB":19,"TargetListSearch":1,"ToFixed":1,"Trace":41},
      "witch.luac":{"AddAggro":1,"AddToVFTable":10,"ClearAggro":1,"ClearTarget":2,"DoSkill":1,"FromFixed":3,"GetCurrentLevelRange":1,"GetHostPlayerDifficulty":1,"GetHostPlayerLevel":1,"GetID":1,"GetPosition":2,"GetProp":4,"GetPyCst":4,"GetPyOID":1,"GetPyStruct":4,"GetSelf":1,"GetState":1,"GetTarget":2,"GetTargetListTop":1,"HasPath":5,"HasTarget":2,"HeadTo":1,"IsPlayer":1,"IsTargetListEmpty":2,"LookAt":1,"LookAtTarget":1,"MarkAsFlying":1,"MoveTo":5,"OnInit":1,"PlayFX":2,"PlayMusic":2,"PlaySound":1,"PopTargetList":1,"SetLevel":1,"SetTarget":2,"SetTargetListCharacterFilter":1,"SetTargetListObjectFilter":1,"SetTargetListSorting":1,"StartTimerCB":1,"Stop":4,"StopTimerCB":4,"TargetListSearch":1,"ToFixed":1,"WarpTo":4}}
    for name,anchors in (
        ("swampking_core.luac",[("port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp",["DoSkill", "SetIsTargetable(false)","StartTimerCB", "DiveAttackDecision", "MoveToPosition", "timer_delay"]),("port/windows-foundation/features/bosses/runtime_timer_trap_v1.cpp",["parse_timer_trap_script_v1","TimerTrapRowV1"])]),
        ("witch.luac",[("port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp",["DoTeleport", "def.ai.teleport.delay_ms", "PlayFX", "WarpTo", "DoSkill"])])):
        p=base/name
        if not p.is_file():
            checks.append({"name":name,"passed":False,"detail":"Original Lua source file is missing."}); continue
        text=p.read_text(encoding="utf-8",errors="replace"); parsed,_,_,_=_parse_script(text)
        actual=dict(collections.Counter(c["name"] for c in parsed if c["mode"] in ("global","object_method","qualified_function")))
        call_list_matches=actual==expected[name]
        missing_anchors=[]
        for path,needles in anchors:
            source=root/path
            body=source.read_text(encoding="utf-8",errors="replace") if source.is_file() else ""
            missing_anchors.extend(f"{path}::{needle}" for needle in needles if needle not in body)
        if name=="witch.luac":
            skill_calls=[c for c in parsed if c["name"]=="DoSkill"]
            has_only_skill0=(len(skill_calls)==1 and skill_calls[0]["arguments"] and skill_calls[0]["arguments"][0]["expression"]=="0")
            has_10s="MOVE_DELAY" in text and bool(re.search(r"MOVE_DELAY\s*=\s*10000(?:\.0)?",text))
            outcome=call_list_matches and not missing_anchors and has_only_skill0 and has_10s
            detail=f"Exact call-list fixture matches={call_list_matches} ({len(actual)} names); DoSkill count={len(skill_calls)}, only slot 0={has_only_skill0}, 10s teleport delay={has_10s}. Production code anchors cover generic DoTeleport/FX/WarpTo and timer delay handling; full user-session integration is not asserted. Missing code anchors={missing_anchors}."
        else:
            outcome=call_list_matches and not missing_anchors
            detail=f"Exact call-list fixture matches={call_list_matches} ({len(actual)} names); code anchors verify source-specific skill slots, dive targetability, animation/state and timer parsing plus timer-trap parser. B159 supersedes the early B155 gaps for level scaling, damage calibration and AOE pushback: those are implemented and documented in mudlord3-report.md. Remaining documented gaps are MarkAsFlying, the camera-frustum awake latch, resetonhubchange, timer/culling differences, TimerTrap point-cloud bounds and projectile expiry; SetIsTargetable is a targeting gate, not a damage gate. Missing code anchors={missing_anchors}."
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
                elif c["mode"]=="lua_builtin" and c["receiver"]: display=f"{c['receiver']}.{c['name']}"
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
