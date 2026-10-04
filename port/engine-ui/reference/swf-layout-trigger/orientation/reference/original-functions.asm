
# _ZN8RenderFX23RegisterDisplayCallbackEPN7gameswf9characterEPFvRNS0_12render_stateEPvES5_
007a7e80: subs     ip, r1, #0
007a7e84: push     {r4, lr}
007a7e88: beq      #0x7a7eac
007a7e8c: mov      r0, ip
007a7e90: mov      r1, r2
007a7e94: mov      r2, r3
007a7e98: ldr      r3, [ip]
007a7e9c: mov      lr, pc
007a7ea0: ldr      pc, [r3, #0x154]
007a7ea4: mov      r0, #1
007a7ea8: pop      {r4, pc}
007a7eac: mov      r0, ip
007a7eb0: pop      {r4, pc}

# _ZN8RenderFX14SetOrientationEN7gameswf16orientation_modeE
007a7eb4: ldr      r3, [pc, #0x24]
007a7eb8: ldr      r2, [pc, #0x24]
007a7ebc: push     {r4, lr}
007a7ec0: add      r3, pc, r3
007a7ec4: ldr      r2, [r3, r2]
007a7ec8: ldr      r3, [r2]
007a7ecc: mov      r0, r3
007a7ed0: ldr      r3, [r3]
007a7ed4: mov      lr, pc
007a7ed8: ldr      pc, [r3, #0xa8]
007a7edc: pop      {r4, pc}
007a7ee0: ldrsbeq  ip, [lr], -r0
007a7ee4: strheq   r3, [r0], -r4

# _Z23NativeUpdateOrientationRKN7gameswf7fn_callE
0043a8b0: ldr      r3, [pc, #0x2c]
0043a8b4: ldr      r2, [pc, #0x2c]
0043a8b8: push     {r4, lr}
0043a8bc: add      r3, pc, r3
0043a8c0: ldr      r4, [r3, r2]
0043a8c4: ldr      r1, [pc, #0x20]
0043a8c8: mov      r0, r4
0043a8cc: add      r1, pc, r1
0043a8d0: bl       #0x320e44
0043a8d4: mov      r1, r0
0043a8d8: mov      r0, r4
0043a8dc: pop      {r4, lr}
0043a8e0: b        #0x31f748
0043a8e4: ldrsbeq  sl, [r5], #-0x14
0043a8e8: strdeq   r3, r4, [r0], -r4
0043a8ec: subeq    r7, r8, r4, lsl #13

# _ZN8RenderFX23RegisterDisplayCallbackEPKcPFvRN7gameswf12render_stateEPvES5_
007a91d8: push     {r4, r5, r6, lr}
007a91dc: mov      r5, r2
007a91e0: mov      r4, r3
007a91e4: mov      r6, r0
007a91e8: bl       #0x7a9160
007a91ec: mov      r2, r5
007a91f0: mov      r1, r0
007a91f4: mov      r3, r4
007a91f8: mov      r0, r6
007a91fc: pop      {r4, r5, r6, lr}
007a9200: b        #0x7a7e80
