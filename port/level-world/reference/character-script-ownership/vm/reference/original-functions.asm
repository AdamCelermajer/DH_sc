
# _ZN3sfc6script3luaL8newstateEv
0031b224: ldr      r3, [pc, #0x30]
0031b228: ldr      r2, [pc, #0x30]
0031b22c: push     {r4, lr}
0031b230: add      r3, pc, r3
0031b234: mov      r1, #0
0031b238: ldr      r0, [r3, r2]
0031b23c: bl       #0x8579f8
0031b240: subs     r4, r0, #0
0031b244: beq      #0x31b254
0031b248: ldr      r1, [pc, #0x14]
0031b24c: add      r1, pc, r1
0031b250: bl       #0x84b11c
0031b254: mov      r0, r4
0031b258: pop      {r4, pc}
0031b25c: rsbeq    sb, r7, r0, ror #16
0031b260: andeq    r2, r0, ip, ror #22

# lua_close
0085797c: push     {r4, r5, r6, lr}
00857980: ldr      r3, [r0, #0x10]
00857984: ldr      r5, [pc, #0x68]
00857988: ldr      r4, [r3, #0x64]
0085798c: add      r5, pc, r5
00857990: ldr      r1, [r4, #0x20]
00857994: mov      r0, r4
00857998: bl       #0x852a18
0085799c: mov      r0, r4
008579a0: mov      r1, #1
008579a4: bl       #0x852dac
008579a8: mov      r3, #0
008579ac: str      r3, [r4, #0x64]
008579b0: ldr      r3, [r4, #0x28]
008579b4: mov      r2, #0
008579b8: mov      r0, r4
008579bc: str      r3, [r4, #0x14]
008579c0: ldr      r3, [r3]
008579c4: mov      r1, r5
008579c8: strh     r2, [r4, #0x36]
008579cc: strh     r2, [r4, #0x34]
008579d0: str      r3, [r4, #0xc]
008579d4: str      r3, [r4, #8]
008579d8: mov      r2, #0
008579dc: bl       #0x8517fc
008579e0: cmp      r0, #0
008579e4: bne      #0x8579b0
008579e8: mov      r0, r4
008579ec: pop      {r4, r5, r6, lr}
008579f0: b        #0x8578fc
008579f4: strheq   r0, [r0], -r4
