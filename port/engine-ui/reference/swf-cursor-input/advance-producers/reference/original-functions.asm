
# _ZN7gameswf12display_list9constructEv
00755a10: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00755a14: ldr      r3, [r0, #4]
00755a18: sub      sp, sp, #8
00755a1c: mov      sl, r0
00755a20: cmp      r3, #0
00755a24: ble      #0x755ad0
00755a28: ldr      r3, [r0]
00755a2c: ldr      r4, [r3]
00755a30: add      r0, r4, #0x2c
00755a34: bl       #0x755260
00755a38: ldr      r7, [sl, #4]
00755a3c: ldr      r6, [r4, #0x30]
00755a40: subs     r5, r7, #1
00755a44: add      r8, r6, #0x1c
00755a48: bmi      #0x755a80
00755a4c: lsl      r5, r5, #2
00755a50: mov      r4, #0
00755a54: add      sb, sp, #4
00755a58: ldr      r3, [sl]
00755a5c: add      r4, r4, #1
00755a60: mov      r0, r8
00755a64: ldr      r3, [r3, r5]
00755a68: mov      r1, sb
00755a6c: sub      r5, r5, #4
00755a70: str      r3, [sp, #4]
00755a74: bl       #0x75581c
00755a78: cmp      r4, r7
00755a7c: bne      #0x755a58
00755a80: cmp      r7, #0
00755a84: ble      #0x755ad0
00755a88: mov      r4, #0
00755a8c: ldr      r1, [r6, #0x20]
00755a90: ldr      r3, [r6, #0x1c]
00755a94: add      r4, r4, #1
00755a98: sub      r1, r1, #1
00755a9c: ldr      r3, [r3, r1, lsl #2]
00755aa0: cmp      r3, #0
00755aa4: beq      #0x755ac0
00755aa8: mov      r0, r3
00755aac: ldr      r3, [r3]
00755ab0: mov      lr, pc
00755ab4: ldr      pc, [r3, #0x148]
00755ab8: ldr      r1, [r6, #0x20]
00755abc: sub      r1, r1, #1
00755ac0: mov      r0, r8
00755ac4: bl       #0x75586c
00755ac8: cmp      r4, r7
00755acc: bne      #0x755a8c
00755ad0: add      sp, sp, #8
00755ad4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN7gameswf4rootC2EPNS_6playerEPNS_14movie_def_implE
00775f70: push     {r4, r5, r6, r7, r8, sl, lr}
00775f74: ldr      r5, [pc, #0x1bc]
00775f78: sub      sp, sp, #0xc
00775f7c: mov      r7, r2
00775f80: mov      r4, r0
00775f84: mov      r6, r1
00775f88: bl       #0x759c04
00775f8c: ldr      r3, [pc, #0x1a8]
00775f90: add      r5, pc, r5
00775f94: cmp      r7, #0
00775f98: ldr      r3, [r5, r3]
00775f9c: str      r7, [r4, #0xc]
00775fa0: add      r3, r3, #8
00775fa4: str      r3, [r4]
00775fa8: beq      #0x775fb4
00775fac: mov      r0, r7
00775fb0: bl       #0x759c64
00775fb4: mov      r7, #0
00775fb8: mov      r3, #0
00775fbc: mov      r2, #1
00775fc0: mov      r5, #0x3f800000
00775fc4: mvn      r1, #0
00775fc8: str      r2, [r4, #0x20]
00775fcc: str      r2, [r4, #0x1c]
00775fd0: str      r3, [r4, #0x48]
00775fd4: str      r3, [r4, #0x4c]
00775fd8: str      r3, [r4, #0x60]
00775fdc: str      r3, [r4, #0x64]
00775fe0: str      r3, [r4, #0x70]
00775fe4: str      r3, [r4, #0x74]
00775fe8: strb     r1, [r4, #0x3b]
00775fec: add      r0, r4, #0xc8
00775ff0: mov      r1, r6
00775ff4: str      r7, [r4, #0x10]
00775ff8: str      r7, [r4, #0x14]
00775ffc: str      r7, [r4, #0x18]
00776000: str      r5, [r4, #0x34]
00776004: strb     r7, [r4, #0x38]
00776008: strb     r7, [r4, #0x39]
0077600c: strb     r7, [r4, #0x3a]
00776010: str      r7, [r4, #0x3c]
00776014: str      r7, [r4, #0x40]
00776018: str      r7, [r4, #0x44]
0077601c: str      r7, [r4, #0x50]
00776020: str      r7, [r4, #0x54]
00776024: str      r7, [r4, #0x58]
00776028: strb     r7, [r4, #0x5c]
0077602c: strb     r7, [r4, #0x5d]
00776030: strb     r7, [r4, #0x5e]
00776034: str      r5, [r4, #0x68]
00776038: str      r5, [r4, #0x6c]
0077603c: str      r7, [r4, #0x78]
00776040: str      r7, [r4, #0x7c]
00776044: strb     r7, [r4, #0x80]
00776048: strb     r7, [r4, #0x81]
0077604c: strb     r7, [r4, #0x82]
00776050: strb     r7, [r4, #0x84]
00776054: strb     r7, [r4, #0x85]
00776058: str      r3, [r4, #0x94]
0077605c: strb     r7, [r4, #0x86]
00776060: strb     r7, [r4, #0x87]
00776064: str      r7, [r4, #0x88]
00776068: str      r5, [r4, #0x8c]
0077606c: str      r5, [r4, #0x90]
00776070: str      r7, [r4, #0x98]
00776074: str      r7, [r4, #0x9c]
00776078: str      r7, [r4, #0xa0]
0077607c: strb     r7, [r4, #0xa4]
00776080: str      r7, [r4, #0xa8]
00776084: str      r7, [r4, #0xac]
00776088: str      r7, [r4, #0xb0]
0077608c: strb     r7, [r4, #0xb4]
00776090: str      r7, [r4, #0xb8]
00776094: str      r7, [r4, #0xbc]
00776098: str      r7, [r4, #0xc0]
0077609c: strb     r7, [r4, #0xc4]
007760a0: str      r7, [r4, #0xc8]
007760a4: str      r7, [r4, #0xcc]
007760a8: bl       #0x75e9ac
007760ac: ldr      r3, [r4, #0xc]
007760b0: mov      r0, r3
007760b4: ldr      r3, [r3]
007760b8: mov      lr, pc
007760bc: ldr      pc, [r3, #0x30]
007760c0: ldr      r3, [r4, #0xc]
007760c4: mov      r8, r0
007760c8: mov      r0, r3
007760cc: ldr      r3, [r3]
007760d0: mov      lr, pc
007760d4: ldr      pc, [r3, #0x34]
007760d8: mov      sl, r0
007760dc: mov      r0, r8
007760e0: bl       #0x30e4cc
007760e4: mov      r8, r0
007760e8: mov      r0, sl
007760ec: bl       #0x30e4cc
007760f0: mov      r3, r8
007760f4: mov      r2, r7
007760f8: mov      r1, r7
007760fc: str      r0, [sp]
00776100: mov      r0, r4
00776104: bl       #0x775d38
00776108: mov      r0, r4
0077610c: bl       #0x7741a0
00776110: mov      r1, r0
00776114: mov      r0, r5
00776118: bl       #0x30ec94
0077611c: mov      r1, r4
00776120: str      r0, [r4, #0x90]
00776124: mov      r0, r6
00776128: bl       #0x76d71c
0077612c: mov      r0, r4
00776130: add      sp, sp, #0xc
00776134: pop      {r4, r5, r6, r7, r8, sl, pc}
00776138: eoreq    lr, r1, r0, lsl #22
0077613c: andeq    r3, r0, r0, asr #13

# _ZN7gameswf12display_list7advanceEf
00755908: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0075590c: ldr      r3, [r0, #4]
00755910: sub      sp, sp, #0xc
00755914: mov      sb, r0
00755918: cmp      r3, #0
0075591c: mov      sl, r1
00755920: ble      #0x755a08
00755924: ldr      r3, [r0]
00755928: ldr      r4, [r3]
0075592c: add      r0, r4, #0x2c
00755930: bl       #0x755260
00755934: ldr      r7, [sb, #4]
00755938: ldr      r6, [r4, #0x30]
0075593c: subs     r5, r7, #1
00755940: add      r8, r6, #0x1c
00755944: bmi      #0x75597c
00755948: lsl      r5, r5, #2
0075594c: mov      r4, #0
00755950: add      fp, sp, #4
00755954: ldr      r3, [sb]
00755958: add      r4, r4, #1
0075595c: mov      r0, r8
00755960: ldr      r3, [r3, r5]
00755964: mov      r1, fp
00755968: sub      r5, r5, #4
0075596c: str      r3, [sp, #4]
00755970: bl       #0x75581c
00755974: cmp      r4, r7
00755978: bne      #0x755954
0075597c: cmp      r7, #0
00755980: ble      #0x755a08
00755984: mov      r5, #0
00755988: mov      sb, r5
0075598c: b        #0x7559a0
00755990: mov      r0, r8
00755994: bl       #0x75586c
00755998: cmp      r5, r7
0075599c: beq      #0x7559fc
007559a0: ldr      r1, [r6, #0x20]
007559a4: ldr      r3, [r6, #0x1c]
007559a8: add      r5, r5, #1
007559ac: sub      r1, r1, #1
007559b0: ldr      r4, [r3, r1, lsl #2]
007559b4: cmp      r4, #0
007559b8: beq      #0x755990
007559bc: ldrb     r3, [r4, #0x9d]
007559c0: cmp      r3, #0
007559c4: beq      #0x755990
007559c8: mov      r0, r4
007559cc: mov      r1, sl
007559d0: ldr      r3, [r4]
007559d4: mov      lr, pc
007559d8: ldr      pc, [r3, #0x5c]
007559dc: ldr      r1, [r6, #0x20]
007559e0: ldrb     r3, [r4, #0x9d]
007559e4: mov      r0, r8
007559e8: sub      r1, r1, #1
007559ec: orr      sb, sb, r3
007559f0: bl       #0x75586c
007559f4: cmp      r5, r7
007559f8: bne      #0x7559a0
007559fc: mov      r0, sb
00755a00: add      sp, sp, #0xc
00755a04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00755a08: mov      sb, #0
00755a0c: b        #0x7559fc

# _ZN7gameswf9character22can_handle_mouse_eventEv
00752dac: mov      r0, #0
00752db0: bx       lr

# _ZN7gameswf15sprite_instance9constructEv
0077efd8: push     {r4, lr}
0077efdc: ldrb     r3, [r0, #0xeb]
0077efe0: mov      r4, r0
0077efe4: cmp      r3, #0
0077efe8: bne      #0x77f008
0077efec: ldr      r0, [r0, #0xa0]
0077eff0: mov      r1, r4
0077eff4: bl       #0x75fd18
0077eff8: add      r0, r4, #0xa8
0077effc: bl       #0x755a10
0077f000: mov      r3, #1
0077f004: strb     r3, [r4, #0xeb]
0077f008: pop      {r4, pc}

# _ZN7gameswf25button_character_instance22can_handle_mouse_eventEv
007c6bbc: mov      r0, #1
007c6bc0: bx       lr

# _ZN7gameswf19edit_text_character22can_handle_mouse_eventEv
0078a338: ldr      r3, [r0, #0xa0]
0078a33c: ldrb     r0, [r3, #0x4b]
0078a340: eor      r0, r0, #1
0078a344: bx       lr

# _ZN7gameswf15sprite_instance15do_init_actionsEv
0078069c: push     {r4, lr}
007806a0: ldr      r3, [r0, #0xdc]
007806a4: mov      r4, r0
007806a8: cmp      r3, #0
007806ac: beq      #0x7806f0
007806b0: bl       #0x759c64
007806b4: ldr      r3, [r4]
007806b8: mov      r0, r4
007806bc: mov      lr, pc
007806c0: ldr      pc, [r3, #0x58]
007806c4: ldr      r1, [r4, #0xdc]
007806c8: bl       #0x75b810
007806cc: ldr      r0, [r4, #0xdc]
007806d0: ldr      r3, [r0, #4]
007806d4: cmp      r3, #0
007806d8: ble      #0x7806f4
007806dc: mov      r3, #0
007806e0: str      r3, [r0, #4]
007806e4: mov      r0, r4
007806e8: pop      {r4, lr}
007806ec: b        #0x75a240
007806f0: pop      {r4, pc}
007806f4: bge      #0x7806dc
007806f8: lsl      r2, r3, #2
007806fc: mov      ip, #0
00780700: ldr      r1, [r0]
00780704: adds     r3, r3, #1
00780708: str      ip, [r1, r2]
0078070c: add      r2, r2, #4
00780710: bne      #0x780700
00780714: b        #0x7806dc
