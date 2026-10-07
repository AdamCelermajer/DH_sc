
# _ZN7gameswf4root9stop_dragEv
0077415c: mov      r3, #0
00774160: str      r3, [r0, #0x58]
00774164: bx       lr

# _ZN7gameswf15sprite_instance14set_drag_stateERKNS_9character10drag_stateE
0077fdd0: push     {r4, r5}
0077fdd4: ldr      ip, [r0, #0xa4]
0077fdd8: mov      r5, r1
0077fddc: mov      r4, r1
0077fde0: add      ip, ip, #0x58
0077fde4: ldm      r5!, {r0, r1, r2, r3}
0077fde8: stm      ip!, {r0, r1, r2, r3}
0077fdec: ldm      r5, {r0, r1, r2, r3}
0077fdf0: stm      ip, {r0, r1, r2, r3}
0077fdf4: ldr      r0, [r4]
0077fdf8: cmp      r0, #0
0077fdfc: beq      #0x77fe08
0077fe00: pop      {r4, r5}
0077fe04: b        #0x7750e8
0077fe08: pop      {r4, r5}
0077fe0c: bx       lr

# _ZN7gameswf15sprite_instance14get_drag_stateEPNS_9character10drag_stateE
0077e02c: push     {r4, r5}
0077e030: ldr      ip, [r0, #0xa4]
0077e034: mov      r4, r1
0077e038: mov      r5, r4
0077e03c: add      ip, ip, #0x58
0077e040: ldm      ip!, {r0, r1, r2, r3}
0077e044: stm      r5!, {r0, r1, r2, r3}
0077e048: ldm      ip, {r0, r1, r2, r3}
0077e04c: stm      r5, {r0, r1, r2, r3}
0077e050: pop      {r4, r5}
0077e054: bx       lr

# _ZN7gameswf4root10start_dragEPNS_9characterEbbRNS_4rectE
00775154: push     {r4, r5, r6, r7, r8, lr}
00775158: ldr      ip, [r0, #0x58]
0077515c: mov      r4, r0
00775160: mov      r5, r1
00775164: cmp      ip, #0
00775168: mov      r6, r2
0077516c: mov      r8, r3
00775170: ldr      r7, [sp, #0x18]
00775174: beq      #0x77517c
00775178: bl       #0x77415c
0077517c: strb     r6, [r4, #0x5d]
00775180: strb     r8, [r4, #0x5e]
00775184: str      r5, [r4, #0x58]
00775188: ldr      r3, [r7]
0077518c: mov      r2, #0
00775190: mov      r0, r5
00775194: str      r3, [r4, #0x60]
00775198: ldr      r3, [r7, #8]
0077519c: str      r3, [r4, #0x64]
007751a0: ldr      r3, [r7, #4]
007751a4: str      r3, [r4, #0x68]
007751a8: ldr      r3, [r7, #0xc]
007751ac: strb     r2, [r4, #0x5c]
007751b0: str      r3, [r4, #0x6c]
007751b4: pop      {r4, r5, r6, r7, r8, lr}
007751b8: b        #0x7750e8
