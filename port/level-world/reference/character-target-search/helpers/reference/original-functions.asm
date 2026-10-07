
# _ZN10ObjectBase9GetHandleEv
0033dd2c: ldr      r3, [pc, #0x34]
0033dd30: ldr      r2, [pc, #0x34]
0033dd34: push     {r4, lr}
0033dd38: add      r3, pc, r3
0033dd3c: ldr      r2, [r3, r2]
0033dd40: ldr      ip, [r1, #0x2c]
0033dd44: mov      r4, r0
0033dd48: ldr      lr, [r2, #0x38]
0033dd4c: mov      r2, #0xc
0033dd50: ldr      r3, [lr, #0x78]
0033dd54: str      r3, [ip, #8]
0033dd58: ldr      r1, [r1, #0x2c]
0033dd5c: bl       #0x30df38
0033dd60: mov      r0, r4
0033dd64: pop      {r4, pc}
0033dd68: rsbeq    r6, r5, r8, asr sp
0033dd6c: strdeq   r3, r4, [r0], -r4

# _ZNSt4priv20_Deque_iterator_baseIN14ObjectSearcher10TargetInfoEE10_M_advanceEi
0038d684: ldr      r3, [r0]
0038d688: ldr      r2, [r0, #4]
0038d68c: str      r4, [sp, #-4]!
0038d690: rsb      r2, r2, r3
0038d694: asr      r2, r2, #2
0038d698: add      ip, r2, r2, lsl #1
0038d69c: add      ip, ip, ip, lsl #4
0038d6a0: add      ip, ip, ip, lsl #8
0038d6a4: add      ip, ip, ip, lsl #16
0038d6a8: add      r2, r2, ip, lsl #2
0038d6ac: add      r2, r1, r2
0038d6b0: mvn      ip, r2
0038d6b4: lsr      r4, ip, #0x1f
0038d6b8: cmp      r2, #5
0038d6bc: movgt    r4, #0
0038d6c0: andle    r4, r4, #1
0038d6c4: cmp      r4, #0
0038d6c8: bne      #0x38d720
0038d6cc: movw     r3, #0xaaab
0038d6d0: movt     r3, #0xaaaa
0038d6d4: cmp      r2, #0
0038d6d8: umullle  r1, r3, r3, ip
0038d6dc: umullgt  r1, r3, r3, r2
0038d6e0: ldr      r1, [r0, #0xc]
0038d6e4: lsrgt    r3, r3, #2
0038d6e8: mvnle    r3, r3, lsr #2
0038d6ec: mov      ip, #6
0038d6f0: mls      r2, ip, r3, r2
0038d6f4: add      ip, r1, r3, lsl #2
0038d6f8: str      ip, [r0, #0xc]
0038d6fc: ldr      r3, [r1, r3, lsl #2]
0038d700: mov      r1, #0x14
0038d704: mla      r2, r1, r2, r3
0038d708: add      r1, r3, #0x78
0038d70c: str      r2, [r0]
0038d710: str      r1, [r0, #8]
0038d714: str      r3, [r0, #4]
0038d718: ldm      sp!, {r4}
0038d71c: bx       lr
0038d720: mov      r2, #0x14
0038d724: mla      r3, r2, r1, r3
0038d728: str      r3, [r0]
0038d72c: b        #0x38d718

# _ZN12ObjectHandlecvP9CharacterEv
0033ff54: push     {r4, lr}
0033ff58: mov      r1, #0
0033ff5c: bl       #0x33fdc0
0033ff60: subs     r4, r0, #0
0033ff64: bne      #0x33ff70
0033ff68: mov      r0, #0
0033ff6c: pop      {r4, pc}
0033ff70: ldr      r3, [r4]
0033ff74: mov      lr, pc
0033ff78: ldr      pc, [r3, #0x24]
0033ff7c: cmp      r0, #0
0033ff80: beq      #0x33ff68
0033ff84: mov      r0, r4
0033ff88: pop      {r4, pc}

# _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a191c: cmp      r1, #0
004a1920: push     {r4, lr}
004a1924: mov      r4, r0
004a1928: beq      #0x4a194c
004a192c: str      r1, [r0, #0x2c]
004a1930: ldr      r3, [r1]
004a1934: mov      r0, r1
004a1938: mov      lr, pc
004a193c: ldr      pc, [r3, #0x24]
004a1940: cmp      r0, #0
004a1944: ldrne    r3, [r4, #0x2c]
004a1948: strne    r3, [r4, #0x30]
004a194c: pop      {r4, pc}
