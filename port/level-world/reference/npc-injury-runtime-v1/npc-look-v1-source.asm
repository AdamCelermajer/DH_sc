# _ZN10GameObject6LookAtERK7Point3DIfE 393cec 92
00393cec: push     {r4, r5, r6, r7, lr}
00393cf0: mov      r4, r0
00393cf4: sub      sp, sp, #0x14
00393cf8: ldr      r0, [r1, #4]
00393cfc: mov      r5, r1
00393d00: ldr      r1, [r4, #0x164]
00393d04: bl       #0x30e3ac ; 
00393d08: ldr      r1, [r4, #0x168]
00393d0c: mov      r7, r0
00393d10: ldr      r0, [r5, #8]
00393d14: bl       #0x30e3ac ; 
00393d18: ldr      r1, [r4, #0x160]
00393d1c: mov      r6, r0
00393d20: ldr      r0, [r5]
00393d24: bl       #0x30e3ac ; 
00393d28: add      r1, sp, #4
00393d2c: str      r0, [sp, #4]
00393d30: mov      r0, r4
00393d34: str      r7, [sp, #8]
00393d38: str      r6, [sp, #0xc]
00393d3c: bl       #0x393b1c ; _ZN10GameObject11LookTowardsERK7Point3DIfE
00393d40: add      sp, sp, #0x14
00393d44: pop      {r4, r5, r6, r7, pc}