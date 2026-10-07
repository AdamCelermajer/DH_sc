
# _ZN8RenderFX9GotoFrameEPN7gameswf9characterEib
007a7d34: push     {r4, r5, r6, lr}
007a7d38: subs     r4, r1, #0
007a7d3c: mov      r5, r2
007a7d40: mov      r6, r3
007a7d44: beq      #0x7a7d8c
007a7d48: ldr      r2, [r4]
007a7d4c: mov      r0, r4
007a7d50: mov      r1, #2
007a7d54: mov      lr, pc
007a7d58: ldr      pc, [r2, #8]
007a7d5c: cmp      r0, #0
007a7d60: beq      #0x7a7d8c
007a7d64: mov      r1, r5
007a7d68: ldr      r3, [r4]
007a7d6c: mov      r0, r4
007a7d70: mov      lr, pc
007a7d74: ldr      pc, [r3, #0x14c]
007a7d78: mov      r0, r4
007a7d7c: eor      r1, r6, #1
007a7d80: ldr      r3, [r4]
007a7d84: mov      lr, pc
007a7d88: ldr      pc, [r3, #0x94]
007a7d8c: pop      {r4, r5, r6, pc}
