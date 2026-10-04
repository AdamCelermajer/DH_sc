
# _ZN7gameswf14glyph_provider11draw_bitmapERK10FT_Bitmap_
007d06c4: push     {r4, r5, r6, r7, r8, lr}
007d06c8: ldr      r3, [r1, #8]
007d06cc: mov      r4, r1
007d06d0: mov      r0, #1
007d06d4: lsl      r0, r0, #1
007d06d8: cmp      r0, r3
007d06dc: cmpge    r0, #3
007d06e0: ble      #0x7d06d4
007d06e4: ldr      r3, [r4]
007d06e8: cmp      r3, #1
007d06ec: movle    r1, #1
007d06f0: ble      #0x7d0704
007d06f4: mov      r1, #1
007d06f8: lsl      r1, r1, #1
007d06fc: cmp      r1, r3
007d0700: blt      #0x7d06f8
007d0704: bl       #0x7b5c84
007d0708: ldr      r3, [r0, #0x10]
007d070c: ldr      r2, [r0, #0xc]
007d0710: mov      r5, r0
007d0714: mov      r1, #0
007d0718: mul      r2, r2, r3
007d071c: ldr      r0, [r0, #8]
007d0720: bl       #0x30e460
007d0724: ldr      r3, [r4]
007d0728: cmp      r3, #0
007d072c: ble      #0x7d078c
007d0730: ldr      r7, [r4, #8]
007d0734: ldr      r0, [r5, #0x14]
007d0738: mov      r6, #0
007d073c: ldr      r1, [r4, #4]
007d0740: ldr      ip, [r4, #0xc]
007d0744: ldr      r2, [r5, #8]
007d0748: cmp      r1, #0
007d074c: ble      #0x7d0774
007d0750: mla      ip, r6, r7, ip
007d0754: mla      r0, r6, r0, r2
007d0758: mov      r3, #0
007d075c: ldrb     r2, [ip, r3]
007d0760: strb     r2, [r0, r3]
007d0764: add      r3, r3, #1
007d0768: cmp      r3, r1
007d076c: bne      #0x7d075c
007d0770: ldr      r3, [r4]
007d0774: add      r6, r6, #1
007d0778: cmp      r3, r6
007d077c: ble      #0x7d078c
007d0780: ldr      r7, [r4, #8]
007d0784: ldr      r0, [r5, #0x14]
007d0788: b        #0x7d073c
007d078c: mov      r0, r5
007d0790: pop      {r4, r5, r6, r7, r8, pc}
