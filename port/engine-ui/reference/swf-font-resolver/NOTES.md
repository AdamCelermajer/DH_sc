# Original game font resolver

`swf_font_resolver.hpp/.cpp` recover game `get_fontfile` at `0x42b38c`, with required borrowed Debug, language, file-manager path rewrite and real file-open/close services. Original instruction probes cover 1,152 cases; the optimized ARM64 module reproduces every path, found value and 4,544 ordered service requests. Callback context carries a high64bit identity. Native host replay adds six failure-prefix guards, nine malformed/budget guards and one synchronous nested resolver call.

This corrects the earlier static `font-text/NOTES.md` interpretation: `strcmp("Arial")` at `0x42b488` is followed by `bne 0x42b5f8`. **Arial falls through** to `0x42b4a4`, whose literal is `%sdata/wqy-zenhei.ttf`. The language branch applies to other names. Prior notes remain historical; their Arial/generic branch description is superseded by the executed source proof in `port/level-world/reference/font-text/font-resolver-corrected/original-probe.json`.

| Name | Exact source filename | Additional calls |
| --- | --- | --- |
| Arial | base + `data/wqy-zenhei.ttf` | rewrite, fopen rb, fclose on found |
| Other non-special name | language4 `japanese.ttf`;5 `nanumgothic.ttf`;6 `wqy-zenhei.ttf`; otherwise base + `data/<name>.ttf` | language, rewrite, fopen rb, fclose on found |
| _sans, _serif | `#/system/fonts/DroidSans.ttf` | return found without existence check |
| _typewriter regular | `#/system/DroidSans.ttf` | return found without existence check |
| _typewriter bold | `#/system/DroidSans-Bold.ttf` | return found without existence check |
| _typewriter italic | `#/system/DroidSerif-Italic.ttf` | return found without existence check |
| _typewriter bold italic | `#/system/DroidSerif-BoldItalic.ttf` | return found without existence check |
| Symbol | base + `data/menus/SCT_Font_3.fnt` | rewrite, return found without existence check |

Every valid source path begins with `Debug.load` then `Debug.GetSwitch("isTracingMenuFS")`; the switch value is unused. Styles affect only `_typewriter`. Arial does not query language. Successful and missing normal files both retain the resolved filename; source found is1 only when fopen succeeds. The output-size argument of the original routine is unused. The port adds a4KiB construction/rewrite budget and caller-span validation; these malformed policies are native safety boundaries, not original parity claims.

ABI: `FontResolveInput24{name,base_path,bold,italic}`, `FontResolveOutput32{bytes,capacity,length,found,reserved}`, `FontResolveServices16{context,invoke}`. Each mutable `FontResolveRequest40` contains service kind, text, optional rewrite output span and value. Language returns source uint32 through value; open_read returns genuine native handle or0 for missing; close_read receives that full handle. Services return0 delivered. Export `dh2_swf_font_resolve` returns0 delivered, -1 malformed/budget, -2 required service failure. Native outputs remain unchanged on negative results. Providers and input spans remain borrowed/stable through synchronous callbacks.

The actual shared+HUD integration resolves Fontin SmallCaps and Arial through this kernel and genuine original-cache bytes. With source-built FT2.3.7 it produces **245 alpha uploads,239 nonempty images and1,032 bitmap quads**, with four genuine fopen/fclose pairs and no font miss. Controlled Debug, language0 and directory-rewrite projections are labeled fixtures; texture/export identities, symbol-text echo and stencilfalse are also borrowed host fixtures. No GPU, Android package/device, localization parity or original file-manager ownership is claimed.

The official FT237 tree used by the earlier frozen Fontin proof is unchanged. Separate `vendor/freetype-2.3.7-hud` and `freetype237-hud.cmake` preserve the same `dh2_freetype237` target and license/provenance; the HUD snapshot corrects negative signed shifts in bounded embedded-bitmap metrics reached by wqy. The old570 Fontin corpus remains byte-exact. The actual HUD proof has ASan/UBSan/LSan0; GameSWF/core and HUD fixture exclude vptr instrumentation explicitly. Native resolver-only audit uses full ASan/UBSan. Matching FT release and source layout do not prove complete original FT options/raster parity.

Integration: switch the parent include to `freetype237-hud.cmake`, add `swf_font_resolver.cpp`, and provide genuine service deliveries. Map resolved URI to exact bundled source bytes through the caller's existing cache/index policy. Do not substitute SCT for Arial or cache fonts for missing system DroidSans. Root Application base path, language/menu ownership and file-manager rewrite remain explicit providers.
