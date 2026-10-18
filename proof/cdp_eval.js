// Evaluate one expression on a DevTools target and print the result. usage: node cdp_eval.js <port> <url part> <expression>
"use strict";
const { connect } = require("./cdp");
(async () => { const c = await connect(+process.argv[2], process.argv[3]); const r = await c.eval(process.argv[4]); console.log(JSON.stringify(r)); c.close(); })()
    .catch((e) => { console.error(e); process.exit(1); });
