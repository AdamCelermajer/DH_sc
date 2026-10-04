
# _ZN7gameswf14movie_def_impl15create_instanceEv
0076020c: push     {r4, lr}
00760210: bl       #0x75ffb0
00760214: mov      r4, r0
00760218: bl       #0x774154
0076021c: mov      r1, #0
00760220: ldr      r3, [r0]
00760224: mov      r2, r1
00760228: mov      lr, pc
0076022c: ldr      pc, [r3, #0xc8]
00760230: mov      r0, r4
00760234: pop      {r4, pc}

# _ZN7gameswf9character7advanceEf
00752d88: mov      r3, #0
00752d8c: strb     r3, [r0, #0x9d]
00752d90: bx       lr

# _ZN7gameswf25button_character_instance7advanceEf
007c6f40: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007c6f44: mov      r5, r0
007c6f48: sub      sp, sp, #0x34
007c6f4c: mov      fp, r1
007c6f50: bl       #0x75e3a8
007c6f54: mov      r0, r5
007c6f58: bl       #0x753f74
007c6f5c: add      sl, sp, #0x18
007c6f60: mov      ip, r0
007c6f64: mov      r4, sl
007c6f68: ldm      ip!, {r0, r1, r2, r3}
007c6f6c: stm      r4!, {r0, r1, r2, r3}
007c6f70: ldm      ip, {r0, r1}
007c6f74: ldr      sb, [r5, #0xa0]
007c6f78: stm      r4, {r0, r1}
007c6f7c: ldr      r3, [sb, #0x28]
007c6f80: cmp      r3, #0
007c6f84: ble      #0x7c7074
007c6f88: mov      r6, #0
007c6f8c: mov      r4, r6
007c6f90: mov      r8, sp
007c6f94: b        #0x7c6fd8
007c6f98: ldrb     r3, [sb, #5]
007c6f9c: cmp      r3, #0
007c6fa0: beq      #0x7c7044
007c6fa4: ldr      r3, [r5, #0xa4]
007c6fa8: mov      r1, fp
007c6fac: ldr      r3, [r3, r7]
007c6fb0: mov      r0, r3
007c6fb4: ldr      r3, [r3]
007c6fb8: mov      lr, pc
007c6fbc: ldr      pc, [r3, #0x5c]
007c6fc0: ldr      sb, [r5, #0xa0]
007c6fc4: ldr      r3, [sb, #0x28]
007c6fc8: add      r4, r4, #1
007c6fcc: add      r6, r6, #0x64
007c6fd0: cmp      r4, r3
007c6fd4: bge      #0x7c7074
007c6fd8: ldr      r3, [r5, #0xa4]
007c6fdc: ldr      ip, [sb, #0x24]
007c6fe0: lsl      r7, r4, #2
007c6fe4: ldr      r3, [r3, r4, lsl #2]
007c6fe8: cmp      r3, #0
007c6fec: beq      #0x7c6fc4
007c6ff0: mov      lr, sl
007c6ff4: mov      sb, r8
007c6ff8: ldm      lr!, {r0, r1, r2, r3}
007c6ffc: stm      sb!, {r0, r1, r2, r3}
007c7000: ldm      lr, {r0, r1}
007c7004: mov      r3, sb
007c7008: add      sb, ip, r6
007c700c: stm      r3, {r0, r1}
007c7010: add      r1, sb, #0x14
007c7014: mov      r0, sp
007c7018: bl       #0x4165b8
007c701c: ldr      r3, [r5, #0xbc]
007c7020: cmp      r3, #0
007c7024: beq      #0x7c6f98
007c7028: cmp      r3, #1
007c702c: beq      #0x7c707c
007c7030: cmp      r3, #2
007c7034: bne      #0x7c7044
007c7038: ldrb     r3, [sb, #4]
007c703c: cmp      r3, #0
007c7040: bne      #0x7c6fa4
007c7044: ldr      r3, [r5, #0xa4]
007c7048: add      r4, r4, #1
007c704c: add      r6, r6, #0x64
007c7050: ldr      r3, [r3, r7]
007c7054: mov      r0, r3
007c7058: ldr      r3, [r3]
007c705c: mov      lr, pc
007c7060: ldr      pc, [r3, #0x44]
007c7064: ldr      sb, [r5, #0xa0]
007c7068: ldr      r3, [sb, #0x28]
007c706c: cmp      r4, r3
007c7070: blt      #0x7c6fd8
007c7074: add      sp, sp, #0x34
007c7078: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007c707c: ldrb     r3, [sb, #3]
007c7080: cmp      r3, #0
007c7084: bne      #0x7c6fa4
007c7088: b        #0x7c7044

# _ZN7gameswf19edit_text_character7advanceEf
0078a384: push     {r4, lr}
0078a388: ldr      r3, [r0]
0078a38c: mov      lr, pc
0078a390: ldr      pc, [r3, #0xc]
0078a394: pop      {r4, pc}
