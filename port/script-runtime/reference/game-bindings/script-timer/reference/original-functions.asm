
# _ZN10AISDefault13OnScriptTimerEj
003dcc80: push     {r4, r5, r6, lr}
003dcc84: sub      sp, sp, #8
003dcc88: mov      r5, r0
003dcc8c: mov      r6, r1
003dcc90: mov      r0, sp
003dcc94: bl       #0x3192b4
003dcc98: mov      r0, sp
003dcc9c: mov      r1, r6
003dcca0: bl       #0x3cdd78
003dcca4: ldr      r1, [pc, #0x20]
003dcca8: mov      r0, r5
003dccac: mov      r2, sp
003dccb0: add      r1, pc, r1
003dccb4: bl       #0x37c41c
003dccb8: mov      r0, sp
003dccbc: mov      r4, sp
003dccc0: bl       #0x319228
003dccc4: add      sp, sp, #8
003dccc8: pop      {r4, r5, r6, pc}
003dcccc: strdeq   r8, sb, [lr], #-0xc0
