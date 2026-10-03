// Small local MCP client for this project's Android Studio verification.
// No endpoint credentials or session identifiers are written to disk.
import {readFileSync} from 'node:fs';
const endpoint = process.env.DH2_IDE_MCP_URL ?? 'http://127.0.0.1:64342/stream';
let session;
async function rpc(body) {
  const headers = {'Content-Type': 'application/json', Accept: 'application/json, text/event-stream'};
  if (session) headers['Mcp-Session-Id'] = session;
  const r = await fetch(endpoint, {method: 'POST', headers, body: JSON.stringify(body), signal: AbortSignal.timeout(300000)});
  session = r.headers.get('mcp-session-id') || session;
  if (!r.ok) throw new Error(`MCP HTTP ${r.status}: ${(await r.text()).slice(0,1000)}`);
  if (!r.body) return null;
  const reader = r.body.getReader(); let text = '';
  for (;;) {
    const {value, done} = await reader.read();
    if (value) text += new TextDecoder().decode(value);
    if (done) break;
    for (const line of text.split(/\r?\n/)) {
      if (line.startsWith('data:')) {
        const message = JSON.parse(line.slice(5));
        if (message.id === body.id) { await reader.cancel(); return message; }
      }
    }
  }
  return text ? JSON.parse(text) : null;
}
const init = await rpc({jsonrpc:'2.0', id:1, method:'initialize', params:{protocolVersion:'2025-03-26',capabilities:{},clientInfo:{name:'DH2-native-source',version:'1.0'}}});
if (init.error) throw new Error(JSON.stringify(init.error));
await rpc({jsonrpc:'2.0', method:'notifications/initialized'});
const tool = process.argv[2];
const argument = process.argv[3];
const args = argument ? JSON.parse(argument.startsWith('@') ? readFileSync(argument.slice(1),'utf8').replace(/^\uFEFF/,'') : argument) : {};
const result = await rpc({jsonrpc:'2.0',id:2,method:tool==='tools/list'?'tools/list':'tools/call',params:tool==='tools/list'?{}:{name:tool,arguments:args}});
console.log(JSON.stringify(result));
if (result?.error || result?.result?.isError) process.exitCode = 1;
