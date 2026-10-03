
# _ZN6glitch7collada14CEventsManager27getEventTimeFromEventNameExIiLi1000EEEiPKc
0060fcf8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060fcfc: ldr      sb, [r0, #0x14]
0060fd00: sub      sp, sp, #0xc
0060fd04: mov      r7, r1
0060fd08: ldr      r2, [sb, #0x10]
0060fd0c: cmp      r2, #0
0060fd10: str      r2, [sp, #4]
0060fd14: mvnle    fp, #0
0060fd18: ble      #0x60fd90
0060fd1c: ldr      r3, [sb, #0x14]
0060fd20: mov      r8, #0
0060fd24: mvn      fp, #0
0060fd28: str      r3, [sp]
0060fd2c: ldr      r3, [sp]
0060fd30: ldr      r5, [r3, r8, lsl #3]
0060fd34: add      r3, r3, r8, lsl #3
0060fd38: cmp      r5, #0
0060fd3c: ble      #0x60fd80
0060fd40: ldr      r6, [r3, #4]
0060fd44: lsl      sl, r8, #2
0060fd48: mov      r4, #0
0060fd4c: ldr      r1, [r6, r4, lsl #2]
0060fd50: mov      r0, r7
0060fd54: bl       #0x30e31c
0060fd58: cmp      r0, #0
0060fd5c: add      r4, r4, #1
0060fd60: bne      #0x60fd78
0060fd64: ldr      r3, [sb, #0xc]
0060fd68: ldr      r0, [r3, sl]
0060fd6c: bl       #0x30e964
0060fd70: bl       #0x30e4cc
0060fd74: mov      fp, r0
0060fd78: cmp      r4, r5
0060fd7c: bne      #0x60fd4c
0060fd80: ldr      r2, [sp, #4]
0060fd84: add      r8, r8, #1
0060fd88: cmp      r8, r2
0060fd8c: bne      #0x60fd2c
0060fd90: mov      r0, fp
0060fd94: add      sp, sp, #0xc
0060fd98: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager27getEventTimeFromEventNameExItLi30EEEiPKc
0060fd9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060fda0: ldr      r3, [r0, #0x14]
0060fda4: sub      sp, sp, #0xc
0060fda8: mov      r7, r1
0060fdac: ldr      r2, [r3, #0x10]
0060fdb0: cmp      r2, #0
0060fdb4: mvnle    sl, #0
0060fdb8: ble      #0x60fe3c
0060fdbc: lsl      r2, r2, #1
0060fdc0: str      r2, [sp, #4]
0060fdc4: ldr      fp, [r3, #0x14]
0060fdc8: add      sb, r3, #8
0060fdcc: mov      r8, #0
0060fdd0: mvn      sl, #0
0060fdd4: ldr      r5, [fp, r8, lsl #2]
0060fdd8: add      r3, fp, r8, lsl #2
0060fddc: cmp      r5, #0
0060fde0: ble      #0x60fe2c
0060fde4: ldr      r6, [r3, #4]
0060fde8: mov      r4, #0
0060fdec: ldr      r1, [r6, r4, lsl #2]
0060fdf0: mov      r0, r7
0060fdf4: bl       #0x30e31c
0060fdf8: cmp      r0, #0
0060fdfc: add      r4, r4, #1
0060fe00: bne      #0x60fe24
0060fe04: ldr      r3, [sb, #4]
0060fe08: ldrh     r0, [r3, r8]
0060fe0c: bl       #0x30e964
0060fe10: movw     r1, #0x5555
0060fe14: movt     r1, #0x4205
0060fe18: bl       #0x30ed6c
0060fe1c: bl       #0x30e4cc
0060fe20: mov      sl, r0
0060fe24: cmp      r4, r5
0060fe28: bne      #0x60fdec
0060fe2c: ldr      r3, [sp, #4]
0060fe30: add      r8, r8, #2
0060fe34: cmp      r8, r3
0060fe38: bne      #0x60fdd4
0060fe3c: mov      r0, sl
0060fe40: add      sp, sp, #0xc
0060fe44: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada14CEventsManager25getEventTimeFromEventNameEPKc
0060feec: ldr      r3, [r0, #0x14]
0060fef0: ldr      r3, [r3]
0060fef4: cmp      r3, #3
0060fef8: beq      #0x60ff1c
0060fefc: cmp      r3, #4
0060ff00: beq      #0x60ff18
0060ff04: cmp      r3, #1
0060ff08: beq      #0x60ff14
0060ff0c: mov      r0, #0
0060ff10: bx       lr
0060ff14: b        #0x60fe48
0060ff18: b        #0x60fcf8
0060ff1c: b        #0x60fd9c

# _ZN6glitch7collada14CEventsManager27getEventTimeFromEventNameExIhLi30EEEiPKc
0060fe48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0060fe4c: ldr      sl, [r0, #0x14]
0060fe50: sub      sp, sp, #0xc
0060fe54: mov      r7, r1
0060fe58: ldr      r3, [sl, #0x10]
0060fe5c: cmp      r3, #0
0060fe60: str      r3, [sp, #4]
0060fe64: mvnle    sb, #0
0060fe68: ble      #0x60fee0
0060fe6c: ldr      fp, [sl, #0x14]
0060fe70: mov      r8, #0
0060fe74: mvn      sb, #0
0060fe78: ldr      r5, [fp, r8, lsl #3]
0060fe7c: add      r3, fp, r8, lsl #3
0060fe80: cmp      r5, #0
0060fe84: ble      #0x60fed0
0060fe88: ldr      r6, [r3, #4]
0060fe8c: mov      r4, #0
0060fe90: ldr      r1, [r6, r4, lsl #2]
0060fe94: mov      r0, r7
0060fe98: bl       #0x30e31c
0060fe9c: cmp      r0, #0
0060fea0: add      r4, r4, #1
0060fea4: bne      #0x60fec8
0060fea8: ldr      r3, [sl, #0xc]
0060feac: ldrb     r0, [r3, r8]
0060feb0: bl       #0x30e964
0060feb4: movw     r1, #0x5555
0060feb8: movt     r1, #0x4205
0060febc: bl       #0x30ed6c
0060fec0: bl       #0x30e4cc
0060fec4: mov      sb, r0
0060fec8: cmp      r4, r5
0060fecc: bne      #0x60fe90
0060fed0: ldr      r3, [sp, #4]
0060fed4: add      r8, r8, #1
0060fed8: cmp      r8, r3
0060fedc: bne      #0x60fe78
0060fee0: mov      r0, sb
0060fee4: add      sp, sp, #0xc
0060fee8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
