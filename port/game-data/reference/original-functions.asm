
# _ZN6Arrays9ModelDict4readEP11IStreamBase
004b86f0: push     {r4, r5, r6, r7, r8, sl, lr}
004b86f4: sub      sp, sp, #0xc
004b86f8: mov      sl, r0
004b86fc: bl       #0x313a90
004b8700: ldr      r6, [pc, #0x124]
004b8704: mov      r3, #1
004b8708: cmp      r3, #0
004b870c: str      r0, [sp, #4]
004b8710: str      r3, [sp]
004b8714: add      r6, pc, r6
004b8718: bne      #0x4b8760
004b871c: add      r3, sp, #4
004b8720: add      r2, r3, #2
004b8724: add      r3, r3, #1
004b8728: ldrb     r0, [r2, #1]
004b872c: ldrb     r1, [r3, #-1]
004b8730: cmp      r2, r3
004b8734: eor      r1, r0, r1
004b8738: strb     r1, [r3, #-1]
004b873c: ldrb     r0, [r2, #1]
004b8740: eor      r1, r1, r0
004b8744: strb     r1, [r2, #1]
004b8748: ldrb     r0, [r3, #-1]
004b874c: sub      r2, r2, #1
004b8750: eor      r1, r1, r0
004b8754: strb     r1, [r3, #-1]
004b8758: add      r3, r3, #1
004b875c: bhi      #0x4b8728
004b8760: bl       #0x4a41a4
004b8764: ldr      r7, [pc, #0xc4]
004b8768: ldr      r4, [sp, #4]
004b876c: mov      r5, #0xc
004b8770: ldr      r3, [r6, r7]
004b8774: mul      r0, r5, r4
004b8778: str      r4, [r3]
004b877c: add      r0, r0, #8
004b8780: mov      r1, #1
004b8784: bl       #0x31056c
004b8788: cmp      r4, #0
004b878c: str      r5, [r0]
004b8790: str      r4, [r0, #4]
004b8794: add      r3, r0, #8
004b8798: beq      #0x4b87c8
004b879c: ldr      r1, [pc, #0x90]
004b87a0: mov      r2, #0
004b87a4: mov      ip, r2
004b87a8: ldr      r1, [r6, r1]
004b87ac: add      r1, r1, #8
004b87b0: add      r2, r2, #1
004b87b4: cmp      r2, r4
004b87b8: str      r1, [r0, #8]
004b87bc: str      ip, [r0, #0x10]
004b87c0: add      r0, r0, #0xc
004b87c4: bne      #0x4b87b0
004b87c8: ldr      r2, [r6, r7]
004b87cc: ldr      r8, [pc, #0x64]
004b87d0: ldr      r1, [r2]
004b87d4: ldr      r2, [r6, r8]
004b87d8: cmp      r1, #0
004b87dc: str      r3, [r2]
004b87e0: beq      #0x4b8824
004b87e4: mov      r4, #0
004b87e8: mov      r5, r4
004b87ec: b        #0x4b87f8
004b87f0: ldr      r3, [r6, r8]
004b87f4: ldr      r3, [r3]
004b87f8: add      r0, r3, r4
004b87fc: mov      r1, sl
004b8800: ldr      r3, [r3, r4]
004b8804: mov      lr, pc
004b8808: ldr      pc, [r3, #0xc]
004b880c: ldr      r3, [r6, r7]
004b8810: add      r5, r5, #1
004b8814: add      r4, r4, #0xc
004b8818: ldr      r3, [r3]
004b881c: cmp      r3, r5
004b8820: bhi      #0x4b87f0
004b8824: add      sp, sp, #0xc
004b8828: pop      {r4, r5, r6, r7, r8, sl, pc}
004b882c: subeq    ip, sp, ip, ror r3
004b8830: andeq    r3, r0, ip, lsl #24
004b8834: strheq   r0, [r0], -ip
004b8838: andeq    r4, r0, r4, asr #6

# _ZN7Structs19CharacterProperties4readEP11IStreamBase
004f2750: push     {r4, r5, r6, lr}
004f2754: mov      r4, r0
004f2758: sub      sp, sp, #8
004f275c: mov      r0, r1
004f2760: mov      r5, r1
004f2764: add      r1, r4, #4
004f2768: bl       #0x459090
004f276c: mov      r3, #1
004f2770: cmp      r3, #0
004f2774: str      r3, [sp, #4]
004f2778: bne      #0x4f27bc
004f277c: add      r3, r4, #5
004f2780: add      r2, r4, #6
004f2784: ldrb     r0, [r2, #1]
004f2788: ldrb     r1, [r3, #-1]
004f278c: cmp      r3, r2
004f2790: eor      r1, r0, r1
004f2794: strb     r1, [r3, #-1]
004f2798: ldrb     r0, [r2, #1]
004f279c: eor      r1, r1, r0
004f27a0: strb     r1, [r2, #1]
004f27a4: ldrb     r0, [r3, #-1]
004f27a8: sub      r2, r2, #1
004f27ac: eor      r1, r1, r0
004f27b0: strb     r1, [r3, #-1]
004f27b4: add      r3, r3, #1
004f27b8: blo      #0x4f2784
004f27bc: mov      r0, r5
004f27c0: add      r1, r4, #8
004f27c4: bl       #0x459090
004f27c8: mov      r3, #1
004f27cc: cmp      r3, #0
004f27d0: str      r3, [sp, #4]
004f27d4: bne      #0x4f2818
004f27d8: add      r3, r4, #9
004f27dc: add      r2, r4, #0xa
004f27e0: ldrb     r0, [r2, #1]
004f27e4: ldrb     r1, [r3, #-1]
004f27e8: cmp      r3, r2
004f27ec: eor      r1, r0, r1
004f27f0: strb     r1, [r3, #-1]
004f27f4: ldrb     r0, [r2, #1]
004f27f8: eor      r1, r1, r0
004f27fc: strb     r1, [r2, #1]
004f2800: ldrb     r0, [r3, #-1]
004f2804: sub      r2, r2, #1
004f2808: eor      r1, r1, r0
004f280c: strb     r1, [r3, #-1]
004f2810: add      r3, r3, #1
004f2814: blo      #0x4f27e0
004f2818: mov      r0, r5
004f281c: add      r1, r4, #0xc
004f2820: bl       #0x459090
004f2824: mov      r3, #1
004f2828: cmp      r3, #0
004f282c: str      r3, [sp, #4]
004f2830: bne      #0x4f2874
004f2834: add      r3, r4, #0xd
004f2838: add      r2, r4, #0xe
004f283c: ldrb     r0, [r2, #1]
004f2840: ldrb     r1, [r3, #-1]
004f2844: cmp      r3, r2
004f2848: eor      r1, r0, r1
004f284c: strb     r1, [r3, #-1]
004f2850: ldrb     r0, [r2, #1]
004f2854: eor      r1, r1, r0
004f2858: strb     r1, [r2, #1]
004f285c: ldrb     r0, [r3, #-1]
004f2860: sub      r2, r2, #1
004f2864: eor      r1, r1, r0
004f2868: strb     r1, [r3, #-1]
004f286c: add      r3, r3, #1
004f2870: blo      #0x4f283c
004f2874: mov      r0, r5
004f2878: add      r1, r4, #0x10
004f287c: bl       #0x459090
004f2880: mov      r3, #1
004f2884: cmp      r3, #0
004f2888: str      r3, [sp, #4]
004f288c: bne      #0x4f28d0
004f2890: add      r3, r4, #0x11
004f2894: add      r2, r4, #0x12
004f2898: ldrb     r0, [r2, #1]
004f289c: ldrb     r1, [r3, #-1]
004f28a0: cmp      r3, r2
004f28a4: eor      r1, r0, r1
004f28a8: strb     r1, [r3, #-1]
004f28ac: ldrb     r0, [r2, #1]
004f28b0: eor      r1, r1, r0
004f28b4: strb     r1, [r2, #1]
004f28b8: ldrb     r0, [r3, #-1]
004f28bc: sub      r2, r2, #1
004f28c0: eor      r1, r1, r0
004f28c4: strb     r1, [r3, #-1]
004f28c8: add      r3, r3, #1
004f28cc: blo      #0x4f2898
004f28d0: mov      r0, r5
004f28d4: add      r1, r4, #0x14
004f28d8: bl       #0x459090
004f28dc: mov      r3, #1
004f28e0: cmp      r3, #0
004f28e4: str      r3, [sp, #4]
004f28e8: bne      #0x4f292c
004f28ec: add      r3, r4, #0x15
004f28f0: add      r2, r4, #0x16
004f28f4: ldrb     r0, [r2, #1]
004f28f8: ldrb     r1, [r3, #-1]
004f28fc: cmp      r3, r2
004f2900: eor      r1, r0, r1
004f2904: strb     r1, [r3, #-1]
004f2908: ldrb     r0, [r2, #1]
004f290c: eor      r1, r1, r0
004f2910: strb     r1, [r2, #1]
004f2914: ldrb     r0, [r3, #-1]
004f2918: sub      r2, r2, #1
004f291c: eor      r1, r1, r0
004f2920: strb     r1, [r3, #-1]
004f2924: add      r3, r3, #1
004f2928: blo      #0x4f28f4
004f292c: mov      r0, r5
004f2930: add      r1, r4, #0x18
004f2934: bl       #0x459090
004f2938: mov      r3, #1
004f293c: cmp      r3, #0
004f2940: str      r3, [sp, #4]
004f2944: bne      #0x4f2988
004f2948: add      r3, r4, #0x19
004f294c: add      r2, r4, #0x1a
004f2950: ldrb     r0, [r2, #1]
004f2954: ldrb     r1, [r3, #-1]
004f2958: cmp      r3, r2
004f295c: eor      r1, r0, r1
004f2960: strb     r1, [r3, #-1]
004f2964: ldrb     r0, [r2, #1]
004f2968: eor      r1, r1, r0
004f296c: strb     r1, [r2, #1]
004f2970: ldrb     r0, [r3, #-1]
004f2974: sub      r2, r2, #1
004f2978: eor      r1, r1, r0
004f297c: strb     r1, [r3, #-1]
004f2980: add      r3, r3, #1
004f2984: blo      #0x4f2950
004f2988: mov      r0, r5
004f298c: add      r1, r4, #0x1c
004f2990: bl       #0x459090
004f2994: mov      r3, #1
004f2998: cmp      r3, #0
004f299c: str      r3, [sp, #4]
004f29a0: bne      #0x4f29e4
004f29a4: add      r3, r4, #0x1d
004f29a8: add      r2, r4, #0x1e
004f29ac: ldrb     r0, [r2, #1]
004f29b0: ldrb     r1, [r3, #-1]
004f29b4: cmp      r3, r2
004f29b8: eor      r1, r0, r1
004f29bc: strb     r1, [r3, #-1]
004f29c0: ldrb     r0, [r2, #1]
004f29c4: eor      r1, r1, r0
004f29c8: strb     r1, [r2, #1]
004f29cc: ldrb     r0, [r3, #-1]
004f29d0: sub      r2, r2, #1
004f29d4: eor      r1, r1, r0
004f29d8: strb     r1, [r3, #-1]
004f29dc: add      r3, r3, #1
004f29e0: blo      #0x4f29ac
004f29e4: mov      r0, r5
004f29e8: add      r1, r4, #0x20
004f29ec: bl       #0x459090
004f29f0: mov      r3, #1
004f29f4: cmp      r3, #0
004f29f8: str      r3, [sp, #4]
004f29fc: bne      #0x4f2a40
004f2a00: add      r3, r4, #0x21
004f2a04: add      r2, r4, #0x22
004f2a08: ldrb     r0, [r2, #1]
004f2a0c: ldrb     r1, [r3, #-1]
004f2a10: cmp      r3, r2
004f2a14: eor      r1, r0, r1
004f2a18: strb     r1, [r3, #-1]
004f2a1c: ldrb     r0, [r2, #1]
004f2a20: eor      r1, r1, r0
004f2a24: strb     r1, [r2, #1]
004f2a28: ldrb     r0, [r3, #-1]
004f2a2c: sub      r2, r2, #1
004f2a30: eor      r1, r1, r0
004f2a34: strb     r1, [r3, #-1]
004f2a38: add      r3, r3, #1
004f2a3c: blo      #0x4f2a08
004f2a40: mov      r0, r5
004f2a44: add      r1, r4, #0x24
004f2a48: bl       #0x459090
004f2a4c: mov      r3, #1
004f2a50: cmp      r3, #0
004f2a54: str      r3, [sp, #4]
004f2a58: bne      #0x4f2a9c
004f2a5c: add      r3, r4, #0x25
004f2a60: add      r2, r4, #0x26
004f2a64: ldrb     r0, [r2, #1]
004f2a68: ldrb     r1, [r3, #-1]
004f2a6c: cmp      r3, r2
004f2a70: eor      r1, r0, r1
004f2a74: strb     r1, [r3, #-1]
004f2a78: ldrb     r0, [r2, #1]
004f2a7c: eor      r1, r1, r0
004f2a80: strb     r1, [r2, #1]
004f2a84: ldrb     r0, [r3, #-1]
004f2a88: sub      r2, r2, #1
004f2a8c: eor      r1, r1, r0
004f2a90: strb     r1, [r3, #-1]
004f2a94: add      r3, r3, #1
004f2a98: blo      #0x4f2a64
004f2a9c: mov      r0, r5
004f2aa0: add      r1, r4, #0x28
004f2aa4: bl       #0x459090
004f2aa8: mov      r3, #1
004f2aac: cmp      r3, #0
004f2ab0: str      r3, [sp, #4]
004f2ab4: bne      #0x4f2af8
004f2ab8: add      r3, r4, #0x29
004f2abc: add      r2, r4, #0x2a
004f2ac0: ldrb     r0, [r2, #1]
004f2ac4: ldrb     r1, [r3, #-1]
004f2ac8: cmp      r3, r2
004f2acc: eor      r1, r0, r1
004f2ad0: strb     r1, [r3, #-1]
004f2ad4: ldrb     r0, [r2, #1]
004f2ad8: eor      r1, r1, r0
004f2adc: strb     r1, [r2, #1]
004f2ae0: ldrb     r0, [r3, #-1]
004f2ae4: sub      r2, r2, #1
004f2ae8: eor      r1, r1, r0
004f2aec: strb     r1, [r3, #-1]
004f2af0: add      r3, r3, #1
004f2af4: blo      #0x4f2ac0
004f2af8: mov      r0, r5
004f2afc: add      r1, r4, #0x2c
004f2b00: bl       #0x459090
004f2b04: mov      r3, #1
004f2b08: cmp      r3, #0
004f2b0c: str      r3, [sp, #4]
004f2b10: bne      #0x4f2b54
004f2b14: add      r3, r4, #0x2d
004f2b18: add      r2, r4, #0x2e
004f2b1c: ldrb     r0, [r2, #1]
004f2b20: ldrb     r1, [r3, #-1]
004f2b24: cmp      r3, r2
004f2b28: eor      r1, r0, r1
004f2b2c: strb     r1, [r3, #-1]
004f2b30: ldrb     r0, [r2, #1]
004f2b34: eor      r1, r1, r0
004f2b38: strb     r1, [r2, #1]
004f2b3c: ldrb     r0, [r3, #-1]
004f2b40: sub      r2, r2, #1
004f2b44: eor      r1, r1, r0
004f2b48: strb     r1, [r3, #-1]
004f2b4c: add      r3, r3, #1
004f2b50: blo      #0x4f2b1c
004f2b54: mov      r0, r5
004f2b58: add      r1, r4, #0x30
004f2b5c: bl       #0x459090
004f2b60: mov      r3, #1
004f2b64: cmp      r3, #0
004f2b68: str      r3, [sp, #4]
004f2b6c: bne      #0x4f2bb0
004f2b70: add      r3, r4, #0x31
004f2b74: add      r2, r4, #0x32
004f2b78: ldrb     r0, [r2, #1]
004f2b7c: ldrb     r1, [r3, #-1]
004f2b80: cmp      r3, r2
004f2b84: eor      r1, r0, r1
004f2b88: strb     r1, [r3, #-1]
004f2b8c: ldrb     r0, [r2, #1]
004f2b90: eor      r1, r1, r0
004f2b94: strb     r1, [r2, #1]
004f2b98: ldrb     r0, [r3, #-1]
004f2b9c: sub      r2, r2, #1
004f2ba0: eor      r1, r1, r0
004f2ba4: strb     r1, [r3, #-1]
004f2ba8: add      r3, r3, #1
004f2bac: blo      #0x4f2b78
004f2bb0: mov      r0, r5
004f2bb4: add      r1, r4, #0x34
004f2bb8: bl       #0x459090
004f2bbc: mov      r3, #1
004f2bc0: cmp      r3, #0
004f2bc4: str      r3, [sp, #4]
004f2bc8: bne      #0x4f2c0c
004f2bcc: add      r3, r4, #0x35
004f2bd0: add      r2, r4, #0x36
004f2bd4: ldrb     r0, [r2, #1]
004f2bd8: ldrb     r1, [r3, #-1]
004f2bdc: cmp      r3, r2
004f2be0: eor      r1, r0, r1
004f2be4: strb     r1, [r3, #-1]
004f2be8: ldrb     r0, [r2, #1]
004f2bec: eor      r1, r1, r0
004f2bf0: strb     r1, [r2, #1]
004f2bf4: ldrb     r0, [r3, #-1]
004f2bf8: sub      r2, r2, #1
004f2bfc: eor      r1, r1, r0
004f2c00: strb     r1, [r3, #-1]
004f2c04: add      r3, r3, #1
004f2c08: blo      #0x4f2bd4
004f2c0c: mov      r0, r5
004f2c10: add      r1, r4, #0x38
004f2c14: bl       #0x459090
004f2c18: mov      r3, #1
004f2c1c: cmp      r3, #0
004f2c20: str      r3, [sp, #4]
004f2c24: bne      #0x4f2c68
004f2c28: add      r3, r4, #0x39
004f2c2c: add      r2, r4, #0x3a
004f2c30: ldrb     r0, [r2, #1]
004f2c34: ldrb     r1, [r3, #-1]
004f2c38: cmp      r3, r2
004f2c3c: eor      r1, r0, r1
004f2c40: strb     r1, [r3, #-1]
004f2c44: ldrb     r0, [r2, #1]
004f2c48: eor      r1, r1, r0
004f2c4c: strb     r1, [r2, #1]
004f2c50: ldrb     r0, [r3, #-1]
004f2c54: sub      r2, r2, #1
004f2c58: eor      r1, r1, r0
004f2c5c: strb     r1, [r3, #-1]
004f2c60: add      r3, r3, #1
004f2c64: blo      #0x4f2c30
004f2c68: mov      r0, r5
004f2c6c: add      r1, r4, #0x3c
004f2c70: bl       #0x459090
004f2c74: mov      r3, #1
004f2c78: cmp      r3, #0
004f2c7c: str      r3, [sp, #4]
004f2c80: bne      #0x4f2cc4
004f2c84: add      r3, r4, #0x3d
004f2c88: add      r2, r4, #0x3e
004f2c8c: ldrb     r0, [r2, #1]
004f2c90: ldrb     r1, [r3, #-1]
004f2c94: cmp      r3, r2
004f2c98: eor      r1, r0, r1
004f2c9c: strb     r1, [r3, #-1]
004f2ca0: ldrb     r0, [r2, #1]
004f2ca4: eor      r1, r1, r0
004f2ca8: strb     r1, [r2, #1]
004f2cac: ldrb     r0, [r3, #-1]
004f2cb0: sub      r2, r2, #1
004f2cb4: eor      r1, r1, r0
004f2cb8: strb     r1, [r3, #-1]
004f2cbc: add      r3, r3, #1
004f2cc0: blo      #0x4f2c8c
004f2cc4: mov      r0, r5
004f2cc8: add      r1, r4, #0x40
004f2ccc: bl       #0x459090
004f2cd0: mov      r3, #1
004f2cd4: cmp      r3, #0
004f2cd8: str      r3, [sp, #4]
004f2cdc: bne      #0x4f2d20
004f2ce0: add      r3, r4, #0x41
004f2ce4: add      r2, r4, #0x42
004f2ce8: ldrb     r0, [r2, #1]
004f2cec: ldrb     r1, [r3, #-1]
004f2cf0: cmp      r3, r2
004f2cf4: eor      r1, r0, r1
004f2cf8: strb     r1, [r3, #-1]
004f2cfc: ldrb     r0, [r2, #1]
004f2d00: eor      r1, r1, r0
004f2d04: strb     r1, [r2, #1]
004f2d08: ldrb     r0, [r3, #-1]
004f2d0c: sub      r2, r2, #1
004f2d10: eor      r1, r1, r0
004f2d14: strb     r1, [r3, #-1]
004f2d18: add      r3, r3, #1
004f2d1c: blo      #0x4f2ce8
004f2d20: mov      r0, r5
004f2d24: add      r1, r4, #0x44
004f2d28: bl       #0x459090
004f2d2c: mov      r3, #1
004f2d30: cmp      r3, #0
004f2d34: str      r3, [sp, #4]
004f2d38: bne      #0x4f2d7c
004f2d3c: add      r3, r4, #0x45
004f2d40: add      r2, r4, #0x46
004f2d44: ldrb     r0, [r2, #1]
004f2d48: ldrb     r1, [r3, #-1]
004f2d4c: cmp      r3, r2
004f2d50: eor      r1, r0, r1
004f2d54: strb     r1, [r3, #-1]
004f2d58: ldrb     r0, [r2, #1]
004f2d5c: eor      r1, r1, r0
004f2d60: strb     r1, [r2, #1]
004f2d64: ldrb     r0, [r3, #-1]
004f2d68: sub      r2, r2, #1
004f2d6c: eor      r1, r1, r0
004f2d70: strb     r1, [r3, #-1]
004f2d74: add      r3, r3, #1
004f2d78: blo      #0x4f2d44
004f2d7c: mov      r0, r5
004f2d80: add      r1, r4, #0x48
004f2d84: bl       #0x459090
004f2d88: mov      r3, #1
004f2d8c: cmp      r3, #0
004f2d90: str      r3, [sp, #4]
004f2d94: bne      #0x4f2dd8
004f2d98: add      r3, r4, #0x49
004f2d9c: add      r2, r4, #0x4a
004f2da0: ldrb     r0, [r2, #1]
004f2da4: ldrb     r1, [r3, #-1]
004f2da8: cmp      r3, r2
004f2dac: eor      r1, r0, r1
004f2db0: strb     r1, [r3, #-1]
004f2db4: ldrb     r0, [r2, #1]
004f2db8: eor      r1, r1, r0
004f2dbc: strb     r1, [r2, #1]
004f2dc0: ldrb     r0, [r3, #-1]
004f2dc4: sub      r2, r2, #1
004f2dc8: eor      r1, r1, r0
004f2dcc: strb     r1, [r3, #-1]
004f2dd0: add      r3, r3, #1
004f2dd4: blo      #0x4f2da0
004f2dd8: mov      r0, r5
004f2ddc: add      r1, r4, #0x4c
004f2de0: bl       #0x459090
004f2de4: mov      r3, #1
004f2de8: cmp      r3, #0
004f2dec: str      r3, [sp, #4]
004f2df0: bne      #0x4f2e34
004f2df4: add      r3, r4, #0x4d
004f2df8: add      r2, r4, #0x4e
004f2dfc: ldrb     r0, [r2, #1]
004f2e00: ldrb     r1, [r3, #-1]
004f2e04: cmp      r3, r2
004f2e08: eor      r1, r0, r1
004f2e0c: strb     r1, [r3, #-1]
004f2e10: ldrb     r0, [r2, #1]
004f2e14: eor      r1, r1, r0
004f2e18: strb     r1, [r2, #1]
004f2e1c: ldrb     r0, [r3, #-1]
004f2e20: sub      r2, r2, #1
004f2e24: eor      r1, r1, r0
004f2e28: strb     r1, [r3, #-1]
004f2e2c: add      r3, r3, #1
004f2e30: blo      #0x4f2dfc
004f2e34: mov      r0, r5
004f2e38: add      r1, r4, #0x50
004f2e3c: bl       #0x459090
004f2e40: mov      r3, #1
004f2e44: cmp      r3, #0
004f2e48: str      r3, [sp, #4]
004f2e4c: bne      #0x4f2e90
004f2e50: add      r3, r4, #0x51
004f2e54: add      r2, r4, #0x52
004f2e58: ldrb     r0, [r2, #1]
004f2e5c: ldrb     r1, [r3, #-1]
004f2e60: cmp      r3, r2
004f2e64: eor      r1, r0, r1
004f2e68: strb     r1, [r3, #-1]
004f2e6c: ldrb     r0, [r2, #1]
004f2e70: eor      r1, r1, r0
004f2e74: strb     r1, [r2, #1]
004f2e78: ldrb     r0, [r3, #-1]
004f2e7c: sub      r2, r2, #1
004f2e80: eor      r1, r1, r0
004f2e84: strb     r1, [r3, #-1]
004f2e88: add      r3, r3, #1
004f2e8c: blo      #0x4f2e58
004f2e90: mov      r0, r5
004f2e94: add      r1, r4, #0x54
004f2e98: bl       #0x459090
004f2e9c: mov      r3, #1
004f2ea0: cmp      r3, #0
004f2ea4: str      r3, [sp, #4]
004f2ea8: bne      #0x4f2eec
004f2eac: add      r3, r4, #0x55
004f2eb0: add      r2, r4, #0x56
004f2eb4: ldrb     r0, [r2, #1]
004f2eb8: ldrb     r1, [r3, #-1]
004f2ebc: cmp      r3, r2
004f2ec0: eor      r1, r0, r1
004f2ec4: strb     r1, [r3, #-1]
004f2ec8: ldrb     r0, [r2, #1]
004f2ecc: eor      r1, r1, r0
004f2ed0: strb     r1, [r2, #1]
004f2ed4: ldrb     r0, [r3, #-1]
004f2ed8: sub      r2, r2, #1
004f2edc: eor      r1, r1, r0
004f2ee0: strb     r1, [r3, #-1]
004f2ee4: add      r3, r3, #1
004f2ee8: blo      #0x4f2eb4
004f2eec: mov      r0, r5
004f2ef0: add      r1, r4, #0x58
004f2ef4: bl       #0x459090
004f2ef8: mov      r3, #1
004f2efc: cmp      r3, #0
004f2f00: str      r3, [sp, #4]
004f2f04: bne      #0x4f2f48
004f2f08: add      r3, r4, #0x59
004f2f0c: add      r2, r4, #0x5a
004f2f10: ldrb     r0, [r2, #1]
004f2f14: ldrb     r1, [r3, #-1]
004f2f18: cmp      r3, r2
004f2f1c: eor      r1, r0, r1
004f2f20: strb     r1, [r3, #-1]
004f2f24: ldrb     r0, [r2, #1]
004f2f28: eor      r1, r1, r0
004f2f2c: strb     r1, [r2, #1]
004f2f30: ldrb     r0, [r3, #-1]
004f2f34: sub      r2, r2, #1
004f2f38: eor      r1, r1, r0
004f2f3c: strb     r1, [r3, #-1]
004f2f40: add      r3, r3, #1
004f2f44: blo      #0x4f2f10
004f2f48: mov      r0, r5
004f2f4c: add      r1, r4, #0x5c
004f2f50: bl       #0x459090
004f2f54: mov      r3, #1
004f2f58: cmp      r3, #0
004f2f5c: str      r3, [sp, #4]
004f2f60: bne      #0x4f2fa4
004f2f64: add      r3, r4, #0x5d
004f2f68: add      r2, r4, #0x5e
004f2f6c: ldrb     r0, [r2, #1]
004f2f70: ldrb     r1, [r3, #-1]
004f2f74: cmp      r3, r2
004f2f78: eor      r1, r0, r1
004f2f7c: strb     r1, [r3, #-1]
004f2f80: ldrb     r0, [r2, #1]
004f2f84: eor      r1, r1, r0
004f2f88: strb     r1, [r2, #1]
004f2f8c: ldrb     r0, [r3, #-1]
004f2f90: sub      r2, r2, #1
004f2f94: eor      r1, r1, r0
004f2f98: strb     r1, [r3, #-1]
004f2f9c: add      r3, r3, #1
004f2fa0: blo      #0x4f2f6c
004f2fa4: mov      r0, r5
004f2fa8: add      r1, r4, #0x60
004f2fac: bl       #0x459090
004f2fb0: mov      r3, #1
004f2fb4: cmp      r3, #0
004f2fb8: str      r3, [sp, #4]
004f2fbc: bne      #0x4f3000
004f2fc0: add      r3, r4, #0x61
004f2fc4: add      r2, r4, #0x62
004f2fc8: ldrb     r0, [r2, #1]
004f2fcc: ldrb     r1, [r3, #-1]
004f2fd0: cmp      r3, r2
004f2fd4: eor      r1, r0, r1
004f2fd8: strb     r1, [r3, #-1]
004f2fdc: ldrb     r0, [r2, #1]
004f2fe0: eor      r1, r1, r0
004f2fe4: strb     r1, [r2, #1]
004f2fe8: ldrb     r0, [r3, #-1]
004f2fec: sub      r2, r2, #1
004f2ff0: eor      r1, r1, r0
004f2ff4: strb     r1, [r3, #-1]
004f2ff8: add      r3, r3, #1
004f2ffc: blo      #0x4f2fc8
004f3000: mov      r0, r5
004f3004: add      r1, r4, #0x64
004f3008: bl       #0x459090
004f300c: mov      r3, #1
004f3010: cmp      r3, #0
004f3014: str      r3, [sp, #4]
004f3018: bne      #0x4f305c
004f301c: add      r3, r4, #0x65
004f3020: add      r2, r4, #0x66
004f3024: ldrb     r0, [r2, #1]
004f3028: ldrb     r1, [r3, #-1]
004f302c: cmp      r3, r2
004f3030: eor      r1, r0, r1
004f3034: strb     r1, [r3, #-1]
004f3038: ldrb     r0, [r2, #1]
004f303c: eor      r1, r1, r0
004f3040: strb     r1, [r2, #1]
004f3044: ldrb     r0, [r3, #-1]
004f3048: sub      r2, r2, #1
004f304c: eor      r1, r1, r0
004f3050: strb     r1, [r3, #-1]
004f3054: add      r3, r3, #1
004f3058: blo      #0x4f3024
004f305c: mov      r0, r5
004f3060: add      r1, r4, #0x68
004f3064: bl       #0x459090
004f3068: mov      r3, #1
004f306c: cmp      r3, #0
004f3070: str      r3, [sp, #4]
004f3074: bne      #0x4f30b8
004f3078: add      r3, r4, #0x69
004f307c: add      r2, r4, #0x6a
004f3080: ldrb     r0, [r2, #1]
004f3084: ldrb     r1, [r3, #-1]
004f3088: cmp      r3, r2
004f308c: eor      r1, r0, r1
004f3090: strb     r1, [r3, #-1]
004f3094: ldrb     r0, [r2, #1]
004f3098: eor      r1, r1, r0
004f309c: strb     r1, [r2, #1]
004f30a0: ldrb     r0, [r3, #-1]
004f30a4: sub      r2, r2, #1
004f30a8: eor      r1, r1, r0
004f30ac: strb     r1, [r3, #-1]
004f30b0: add      r3, r3, #1
004f30b4: blo      #0x4f3080
004f30b8: mov      r0, r5
004f30bc: add      r1, r4, #0x6c
004f30c0: bl       #0x459090
004f30c4: mov      r3, #1
004f30c8: cmp      r3, #0
004f30cc: str      r3, [sp, #4]
004f30d0: bne      #0x4f3114
004f30d4: add      r3, r4, #0x6d
004f30d8: add      r2, r4, #0x6e
004f30dc: ldrb     r0, [r2, #1]
004f30e0: ldrb     r1, [r3, #-1]
004f30e4: cmp      r3, r2
004f30e8: eor      r1, r0, r1
004f30ec: strb     r1, [r3, #-1]
004f30f0: ldrb     r0, [r2, #1]
004f30f4: eor      r1, r1, r0
004f30f8: strb     r1, [r2, #1]
004f30fc: ldrb     r0, [r3, #-1]
004f3100: sub      r2, r2, #1
004f3104: eor      r1, r1, r0
004f3108: strb     r1, [r3, #-1]
004f310c: add      r3, r3, #1
004f3110: blo      #0x4f30dc
004f3114: mov      r0, r5
004f3118: add      r1, r4, #0x70
004f311c: bl       #0x459090
004f3120: mov      r3, #1
004f3124: cmp      r3, #0
004f3128: str      r3, [sp, #4]
004f312c: bne      #0x4f3170
004f3130: add      r3, r4, #0x71
004f3134: add      r2, r4, #0x72
004f3138: ldrb     r0, [r2, #1]
004f313c: ldrb     r1, [r3, #-1]
004f3140: cmp      r3, r2
004f3144: eor      r1, r0, r1
004f3148: strb     r1, [r3, #-1]
004f314c: ldrb     r0, [r2, #1]
004f3150: eor      r1, r1, r0
004f3154: strb     r1, [r2, #1]
004f3158: ldrb     r0, [r3, #-1]
004f315c: sub      r2, r2, #1
004f3160: eor      r1, r1, r0
004f3164: strb     r1, [r3, #-1]
004f3168: add      r3, r3, #1
004f316c: blo      #0x4f3138
004f3170: mov      r0, r5
004f3174: add      r1, r4, #0x74
004f3178: bl       #0x459090
004f317c: mov      r3, #1
004f3180: cmp      r3, #0
004f3184: str      r3, [sp, #4]
004f3188: bne      #0x4f31cc
004f318c: add      r3, r4, #0x75
004f3190: add      r2, r4, #0x76
004f3194: ldrb     r0, [r2, #1]
004f3198: ldrb     r1, [r3, #-1]
004f319c: cmp      r3, r2
004f31a0: eor      r1, r0, r1
004f31a4: strb     r1, [r3, #-1]
004f31a8: ldrb     r0, [r2, #1]
004f31ac: eor      r1, r1, r0
004f31b0: strb     r1, [r2, #1]
004f31b4: ldrb     r0, [r3, #-1]
004f31b8: sub      r2, r2, #1
004f31bc: eor      r1, r1, r0
004f31c0: strb     r1, [r3, #-1]
004f31c4: add      r3, r3, #1
004f31c8: blo      #0x4f3194
004f31cc: mov      r0, r5
004f31d0: add      r1, r4, #0x78
004f31d4: bl       #0x459090
004f31d8: mov      r3, #1
004f31dc: cmp      r3, #0
004f31e0: str      r3, [sp, #4]
004f31e4: bne      #0x4f3228
004f31e8: add      r3, r4, #0x79
004f31ec: add      r2, r4, #0x7a
004f31f0: ldrb     r0, [r2, #1]
004f31f4: ldrb     r1, [r3, #-1]
004f31f8: cmp      r3, r2
004f31fc: eor      r1, r0, r1
004f3200: strb     r1, [r3, #-1]
004f3204: ldrb     r0, [r2, #1]
004f3208: eor      r1, r1, r0
004f320c: strb     r1, [r2, #1]
004f3210: ldrb     r0, [r3, #-1]
004f3214: sub      r2, r2, #1
004f3218: eor      r1, r1, r0
004f321c: strb     r1, [r3, #-1]
004f3220: add      r3, r3, #1
004f3224: blo      #0x4f31f0
004f3228: mov      r0, r5
004f322c: add      r1, r4, #0x7c
004f3230: bl       #0x459090
004f3234: mov      r3, #1
004f3238: cmp      r3, #0
004f323c: str      r3, [sp, #4]
004f3240: bne      #0x4f3284
004f3244: add      r3, r4, #0x7d
004f3248: add      r2, r4, #0x7e
004f324c: ldrb     r0, [r2, #1]
004f3250: ldrb     r1, [r3, #-1]
004f3254: cmp      r3, r2
004f3258: eor      r1, r0, r1
004f325c: strb     r1, [r3, #-1]
004f3260: ldrb     r0, [r2, #1]
004f3264: eor      r1, r1, r0
004f3268: strb     r1, [r2, #1]
004f326c: ldrb     r0, [r3, #-1]
004f3270: sub      r2, r2, #1
004f3274: eor      r1, r1, r0
004f3278: strb     r1, [r3, #-1]
004f327c: add      r3, r3, #1
004f3280: blo      #0x4f324c
004f3284: mov      r0, r5
004f3288: add      r1, r4, #0x80
004f328c: bl       #0x459090
004f3290: mov      r3, #1
004f3294: cmp      r3, #0
004f3298: str      r3, [sp, #4]
004f329c: bne      #0x4f32e0
004f32a0: add      r3, r4, #0x81
004f32a4: add      r2, r4, #0x82
004f32a8: ldrb     r0, [r2, #1]
004f32ac: ldrb     r1, [r3, #-1]
004f32b0: cmp      r3, r2
004f32b4: eor      r1, r0, r1
004f32b8: strb     r1, [r3, #-1]
004f32bc: ldrb     r0, [r2, #1]
004f32c0: eor      r1, r1, r0
004f32c4: strb     r1, [r2, #1]
004f32c8: ldrb     r0, [r3, #-1]
004f32cc: sub      r2, r2, #1
004f32d0: eor      r1, r1, r0
004f32d4: strb     r1, [r3, #-1]
004f32d8: add      r3, r3, #1
004f32dc: blo      #0x4f32a8
004f32e0: mov      r0, r5
004f32e4: add      r1, r4, #0x84
004f32e8: bl       #0x459090
004f32ec: mov      r3, #1
004f32f0: cmp      r3, #0
004f32f4: str      r3, [sp, #4]
004f32f8: bne      #0x4f333c
004f32fc: add      r3, r4, #0x85
004f3300: add      r2, r4, #0x86
004f3304: ldrb     r0, [r2, #1]
004f3308: ldrb     r1, [r3, #-1]
004f330c: cmp      r3, r2
004f3310: eor      r1, r0, r1
004f3314: strb     r1, [r3, #-1]
004f3318: ldrb     r0, [r2, #1]
004f331c: eor      r1, r1, r0
004f3320: strb     r1, [r2, #1]
004f3324: ldrb     r0, [r3, #-1]
004f3328: sub      r2, r2, #1
004f332c: eor      r1, r1, r0
004f3330: strb     r1, [r3, #-1]
004f3334: add      r3, r3, #1
004f3338: blo      #0x4f3304
004f333c: mov      r0, r5
004f3340: add      r1, r4, #0x88
004f3344: bl       #0x459090
004f3348: mov      r3, #1
004f334c: cmp      r3, #0
004f3350: str      r3, [sp, #4]
004f3354: bne      #0x4f3398
004f3358: add      r3, r4, #0x89
004f335c: add      r2, r4, #0x8a
004f3360: ldrb     r0, [r2, #1]
004f3364: ldrb     r1, [r3, #-1]
004f3368: cmp      r3, r2
004f336c: eor      r1, r0, r1
004f3370: strb     r1, [r3, #-1]
004f3374: ldrb     r0, [r2, #1]
004f3378: eor      r1, r1, r0
004f337c: strb     r1, [r2, #1]
004f3380: ldrb     r0, [r3, #-1]
004f3384: sub      r2, r2, #1
004f3388: eor      r1, r1, r0
004f338c: strb     r1, [r3, #-1]
004f3390: add      r3, r3, #1
004f3394: blo      #0x4f3360
004f3398: mov      r0, r5
004f339c: add      r1, r4, #0x8c
004f33a0: bl       #0x459090
004f33a4: mov      r3, #1
004f33a8: cmp      r3, #0
004f33ac: str      r3, [sp, #4]
004f33b0: bne      #0x4f33f4
004f33b4: add      r3, r4, #0x8d
004f33b8: add      r2, r4, #0x8e
004f33bc: ldrb     r0, [r2, #1]
004f33c0: ldrb     r1, [r3, #-1]
004f33c4: cmp      r3, r2
004f33c8: eor      r1, r0, r1
004f33cc: strb     r1, [r3, #-1]
004f33d0: ldrb     r0, [r2, #1]
004f33d4: eor      r1, r1, r0
004f33d8: strb     r1, [r2, #1]
004f33dc: ldrb     r0, [r3, #-1]
004f33e0: sub      r2, r2, #1
004f33e4: eor      r1, r1, r0
004f33e8: strb     r1, [r3, #-1]
004f33ec: add      r3, r3, #1
004f33f0: blo      #0x4f33bc
004f33f4: mov      r0, r5
004f33f8: add      r1, r4, #0x90
004f33fc: bl       #0x459090
004f3400: mov      r3, #1
004f3404: cmp      r3, #0
004f3408: str      r3, [sp, #4]
004f340c: bne      #0x4f3450
004f3410: add      r3, r4, #0x91
004f3414: add      r2, r4, #0x92
004f3418: ldrb     r0, [r2, #1]
004f341c: ldrb     r1, [r3, #-1]
004f3420: cmp      r3, r2
004f3424: eor      r1, r0, r1
004f3428: strb     r1, [r3, #-1]
004f342c: ldrb     r0, [r2, #1]
004f3430: eor      r1, r1, r0
004f3434: strb     r1, [r2, #1]
004f3438: ldrb     r0, [r3, #-1]
004f343c: sub      r2, r2, #1
004f3440: eor      r1, r1, r0
004f3444: strb     r1, [r3, #-1]
004f3448: add      r3, r3, #1
004f344c: blo      #0x4f3418
004f3450: mov      r0, r5
004f3454: add      r1, r4, #0x94
004f3458: bl       #0x459090
004f345c: mov      r3, #1
004f3460: cmp      r3, #0
004f3464: str      r3, [sp, #4]
004f3468: bne      #0x4f34ac
004f346c: add      r3, r4, #0x95
004f3470: add      r2, r4, #0x96
004f3474: ldrb     r0, [r2, #1]
004f3478: ldrb     r1, [r3, #-1]
004f347c: cmp      r3, r2
004f3480: eor      r1, r0, r1
004f3484: strb     r1, [r3, #-1]
004f3488: ldrb     r0, [r2, #1]
004f348c: eor      r1, r1, r0
004f3490: strb     r1, [r2, #1]
004f3494: ldrb     r0, [r3, #-1]
004f3498: sub      r2, r2, #1
004f349c: eor      r1, r1, r0
004f34a0: strb     r1, [r3, #-1]
004f34a4: add      r3, r3, #1
004f34a8: blo      #0x4f3474
004f34ac: mov      r0, r5
004f34b0: add      r1, r4, #0x98
004f34b4: bl       #0x459090
004f34b8: mov      r3, #1
004f34bc: cmp      r3, #0
004f34c0: str      r3, [sp, #4]
004f34c4: bne      #0x4f3508
004f34c8: add      r3, r4, #0x99
004f34cc: add      r2, r4, #0x9a
004f34d0: ldrb     r0, [r2, #1]
004f34d4: ldrb     r1, [r3, #-1]
004f34d8: cmp      r3, r2
004f34dc: eor      r1, r0, r1
004f34e0: strb     r1, [r3, #-1]
004f34e4: ldrb     r0, [r2, #1]
004f34e8: eor      r1, r1, r0
004f34ec: strb     r1, [r2, #1]
004f34f0: ldrb     r0, [r3, #-1]
004f34f4: sub      r2, r2, #1
004f34f8: eor      r1, r1, r0
004f34fc: strb     r1, [r3, #-1]
004f3500: add      r3, r3, #1
004f3504: blo      #0x4f34d0
004f3508: mov      r0, r5
004f350c: add      r1, r4, #0x9c
004f3510: bl       #0x459090
004f3514: mov      r3, #1
004f3518: cmp      r3, #0
004f351c: str      r3, [sp, #4]
004f3520: bne      #0x4f3564
004f3524: add      r3, r4, #0x9d
004f3528: add      r2, r4, #0x9e
004f352c: ldrb     r0, [r2, #1]
004f3530: ldrb     r1, [r3, #-1]
004f3534: cmp      r3, r2
004f3538: eor      r1, r0, r1
004f353c: strb     r1, [r3, #-1]
004f3540: ldrb     r0, [r2, #1]
004f3544: eor      r1, r1, r0
004f3548: strb     r1, [r2, #1]
004f354c: ldrb     r0, [r3, #-1]
004f3550: sub      r2, r2, #1
004f3554: eor      r1, r1, r0
004f3558: strb     r1, [r3, #-1]
004f355c: add      r3, r3, #1
004f3560: blo      #0x4f352c
004f3564: mov      r0, r5
004f3568: add      r1, r4, #0xa0
004f356c: bl       #0x459090
004f3570: mov      r3, #1
004f3574: cmp      r3, #0
004f3578: str      r3, [sp, #4]
004f357c: bne      #0x4f35c0
004f3580: add      r3, r4, #0xa1
004f3584: add      r2, r4, #0xa2
004f3588: ldrb     r0, [r2, #1]
004f358c: ldrb     r1, [r3, #-1]
004f3590: cmp      r3, r2
004f3594: eor      r1, r0, r1
004f3598: strb     r1, [r3, #-1]
004f359c: ldrb     r0, [r2, #1]
004f35a0: eor      r1, r1, r0
004f35a4: strb     r1, [r2, #1]
004f35a8: ldrb     r0, [r3, #-1]
004f35ac: sub      r2, r2, #1
004f35b0: eor      r1, r1, r0
004f35b4: strb     r1, [r3, #-1]
004f35b8: add      r3, r3, #1
004f35bc: blo      #0x4f3588
004f35c0: mov      r0, r5
004f35c4: add      r1, r4, #0xa4
004f35c8: bl       #0x459090
004f35cc: mov      r3, #1
004f35d0: cmp      r3, #0
004f35d4: str      r3, [sp, #4]
004f35d8: bne      #0x4f361c
004f35dc: add      r3, r4, #0xa5
004f35e0: add      r2, r4, #0xa6
004f35e4: ldrb     r0, [r2, #1]
004f35e8: ldrb     r1, [r3, #-1]
004f35ec: cmp      r3, r2
004f35f0: eor      r1, r0, r1
004f35f4: strb     r1, [r3, #-1]
004f35f8: ldrb     r0, [r2, #1]
004f35fc: eor      r1, r1, r0
004f3600: strb     r1, [r2, #1]
004f3604: ldrb     r0, [r3, #-1]
004f3608: sub      r2, r2, #1
004f360c: eor      r1, r1, r0
004f3610: strb     r1, [r3, #-1]
004f3614: add      r3, r3, #1
004f3618: blo      #0x4f35e4
004f361c: mov      r0, r5
004f3620: add      r1, r4, #0xa8
004f3624: bl       #0x459090
004f3628: mov      r3, #1
004f362c: cmp      r3, #0
004f3630: str      r3, [sp, #4]
004f3634: bne      #0x4f3678
004f3638: add      r3, r4, #0xa9
004f363c: add      r2, r4, #0xaa
004f3640: ldrb     r0, [r2, #1]
004f3644: ldrb     r1, [r3, #-1]
004f3648: cmp      r3, r2
004f364c: eor      r1, r0, r1
004f3650: strb     r1, [r3, #-1]
004f3654: ldrb     r0, [r2, #1]
004f3658: eor      r1, r1, r0
004f365c: strb     r1, [r2, #1]
004f3660: ldrb     r0, [r3, #-1]
004f3664: sub      r2, r2, #1
004f3668: eor      r1, r1, r0
004f366c: strb     r1, [r3, #-1]
004f3670: add      r3, r3, #1
004f3674: blo      #0x4f3640
004f3678: mov      r0, r5
004f367c: add      r1, r4, #0xac
004f3680: bl       #0x459090
004f3684: mov      r3, #1
004f3688: cmp      r3, #0
004f368c: str      r3, [sp, #4]
004f3690: bne      #0x4f36d4
004f3694: add      r3, r4, #0xad
004f3698: add      r2, r4, #0xae
004f369c: ldrb     r0, [r2, #1]
004f36a0: ldrb     r1, [r3, #-1]
004f36a4: cmp      r3, r2
004f36a8: eor      r1, r0, r1
004f36ac: strb     r1, [r3, #-1]
004f36b0: ldrb     r0, [r2, #1]
004f36b4: eor      r1, r1, r0
004f36b8: strb     r1, [r2, #1]
004f36bc: ldrb     r0, [r3, #-1]
004f36c0: sub      r2, r2, #1
004f36c4: eor      r1, r1, r0
004f36c8: strb     r1, [r3, #-1]
004f36cc: add      r3, r3, #1
004f36d0: blo      #0x4f369c
004f36d4: mov      r0, r5
004f36d8: add      r1, r4, #0xb0
004f36dc: bl       #0x459090
004f36e0: mov      r3, #1
004f36e4: cmp      r3, #0
004f36e8: str      r3, [sp, #4]
004f36ec: bne      #0x4f3730
004f36f0: add      r3, r4, #0xb1
004f36f4: add      r2, r4, #0xb2
004f36f8: ldrb     r0, [r2, #1]
004f36fc: ldrb     r1, [r3, #-1]
004f3700: cmp      r3, r2
004f3704: eor      r1, r0, r1
004f3708: strb     r1, [r3, #-1]
004f370c: ldrb     r0, [r2, #1]
004f3710: eor      r1, r1, r0
004f3714: strb     r1, [r2, #1]
004f3718: ldrb     r0, [r3, #-1]
004f371c: sub      r2, r2, #1
004f3720: eor      r1, r1, r0
004f3724: strb     r1, [r3, #-1]
004f3728: add      r3, r3, #1
004f372c: blo      #0x4f36f8
004f3730: mov      r0, r5
004f3734: add      r1, r4, #0xb4
004f3738: bl       #0x459090
004f373c: mov      r3, #1
004f3740: cmp      r3, #0
004f3744: str      r3, [sp, #4]
004f3748: bne      #0x4f378c
004f374c: add      r3, r4, #0xb5
004f3750: add      r2, r4, #0xb6
004f3754: ldrb     r0, [r2, #1]
004f3758: ldrb     r1, [r3, #-1]
004f375c: cmp      r3, r2
004f3760: eor      r1, r0, r1
004f3764: strb     r1, [r3, #-1]
004f3768: ldrb     r0, [r2, #1]
004f376c: eor      r1, r1, r0
004f3770: strb     r1, [r2, #1]
004f3774: ldrb     r0, [r3, #-1]
004f3778: sub      r2, r2, #1
004f377c: eor      r1, r1, r0
004f3780: strb     r1, [r3, #-1]
004f3784: add      r3, r3, #1
004f3788: blo      #0x4f3754
004f378c: mov      r0, r5
004f3790: add      r1, r4, #0xb8
004f3794: bl       #0x459090
004f3798: mov      r3, #1
004f379c: cmp      r3, #0
004f37a0: str      r3, [sp, #4]
004f37a4: bne      #0x4f37e8
004f37a8: add      r3, r4, #0xb9
004f37ac: add      r2, r4, #0xba
004f37b0: ldrb     r0, [r2, #1]
004f37b4: ldrb     r1, [r3, #-1]
004f37b8: cmp      r3, r2
004f37bc: eor      r1, r0, r1
004f37c0: strb     r1, [r3, #-1]
004f37c4: ldrb     r0, [r2, #1]
004f37c8: eor      r1, r1, r0
004f37cc: strb     r1, [r2, #1]
004f37d0: ldrb     r0, [r3, #-1]
004f37d4: sub      r2, r2, #1
004f37d8: eor      r1, r1, r0
004f37dc: strb     r1, [r3, #-1]
004f37e0: add      r3, r3, #1
004f37e4: blo      #0x4f37b0
004f37e8: mov      r0, r5
004f37ec: add      r1, r4, #0xbc
004f37f0: bl       #0x459090
004f37f4: mov      r3, #1
004f37f8: cmp      r3, #0
004f37fc: str      r3, [sp, #4]
004f3800: bne      #0x4f3844
004f3804: add      r3, r4, #0xbd
004f3808: add      r2, r4, #0xbe
004f380c: ldrb     r0, [r2, #1]
004f3810: ldrb     r1, [r3, #-1]
004f3814: cmp      r3, r2
004f3818: eor      r1, r0, r1
004f381c: strb     r1, [r3, #-1]
004f3820: ldrb     r0, [r2, #1]
004f3824: eor      r1, r1, r0
004f3828: strb     r1, [r2, #1]
004f382c: ldrb     r0, [r3, #-1]
004f3830: sub      r2, r2, #1
004f3834: eor      r1, r1, r0
004f3838: strb     r1, [r3, #-1]
004f383c: add      r3, r3, #1
004f3840: blo      #0x4f380c
004f3844: mov      r0, r5
004f3848: add      r1, r4, #0xc0
004f384c: bl       #0x459090
004f3850: mov      r3, #1
004f3854: cmp      r3, #0
004f3858: str      r3, [sp, #4]
004f385c: bne      #0x4f38a0
004f3860: add      r3, r4, #0xc1
004f3864: add      r2, r4, #0xc2
004f3868: ldrb     r0, [r2, #1]
004f386c: ldrb     r1, [r3, #-1]
004f3870: cmp      r3, r2
004f3874: eor      r1, r0, r1
004f3878: strb     r1, [r3, #-1]
004f387c: ldrb     r0, [r2, #1]
004f3880: eor      r1, r1, r0
004f3884: strb     r1, [r2, #1]
004f3888: ldrb     r0, [r3, #-1]
004f388c: sub      r2, r2, #1
004f3890: eor      r1, r1, r0
004f3894: strb     r1, [r3, #-1]
004f3898: add      r3, r3, #1
004f389c: blo      #0x4f3868
004f38a0: mov      r0, r5
004f38a4: add      r1, r4, #0xc4
004f38a8: bl       #0x459090
004f38ac: mov      r3, #1
004f38b0: cmp      r3, #0
004f38b4: str      r3, [sp, #4]
004f38b8: bne      #0x4f38fc
004f38bc: add      r3, r4, #0xc5
004f38c0: add      r2, r4, #0xc6
004f38c4: ldrb     r0, [r2, #1]
004f38c8: ldrb     r1, [r3, #-1]
004f38cc: cmp      r3, r2
004f38d0: eor      r1, r0, r1
004f38d4: strb     r1, [r3, #-1]
004f38d8: ldrb     r0, [r2, #1]
004f38dc: eor      r1, r1, r0
004f38e0: strb     r1, [r2, #1]
004f38e4: ldrb     r0, [r3, #-1]
004f38e8: sub      r2, r2, #1
004f38ec: eor      r1, r1, r0
004f38f0: strb     r1, [r3, #-1]
004f38f4: add      r3, r3, #1
004f38f8: blo      #0x4f38c4
004f38fc: mov      r0, r5
004f3900: add      r1, r4, #0xc8
004f3904: bl       #0x459090
004f3908: mov      r3, #1
004f390c: cmp      r3, #0
004f3910: str      r3, [sp, #4]
004f3914: bne      #0x4f3958
004f3918: add      r3, r4, #0xc9
004f391c: add      r2, r4, #0xca
004f3920: ldrb     r0, [r2, #1]
004f3924: ldrb     r1, [r3, #-1]
004f3928: cmp      r3, r2
004f392c: eor      r1, r0, r1
004f3930: strb     r1, [r3, #-1]
004f3934: ldrb     r0, [r2, #1]
004f3938: eor      r1, r1, r0
004f393c: strb     r1, [r2, #1]
004f3940: ldrb     r0, [r3, #-1]
004f3944: sub      r2, r2, #1
004f3948: eor      r1, r1, r0
004f394c: strb     r1, [r3, #-1]
004f3950: add      r3, r3, #1
004f3954: blo      #0x4f3920
004f3958: mov      r0, r5
004f395c: add      r1, r4, #0xcc
004f3960: bl       #0x459090
004f3964: mov      r3, #1
004f3968: cmp      r3, #0
004f396c: str      r3, [sp, #4]
004f3970: bne      #0x4f39b4
004f3974: add      r3, r4, #0xcd
004f3978: add      r2, r4, #0xce
004f397c: ldrb     r0, [r2, #1]
004f3980: ldrb     r1, [r3, #-1]
004f3984: cmp      r3, r2
004f3988: eor      r1, r0, r1
004f398c: strb     r1, [r3, #-1]
004f3990: ldrb     r0, [r2, #1]
004f3994: eor      r1, r1, r0
004f3998: strb     r1, [r2, #1]
004f399c: ldrb     r0, [r3, #-1]
004f39a0: sub      r2, r2, #1
004f39a4: eor      r1, r1, r0
004f39a8: strb     r1, [r3, #-1]
004f39ac: add      r3, r3, #1
004f39b0: blo      #0x4f397c
004f39b4: mov      r0, r5
004f39b8: add      r1, r4, #0xd0
004f39bc: bl       #0x459090
004f39c0: mov      r3, #1
004f39c4: cmp      r3, #0
004f39c8: str      r3, [sp, #4]
004f39cc: bne      #0x4f3a10
004f39d0: add      r3, r4, #0xd1
004f39d4: add      r2, r4, #0xd2
004f39d8: ldrb     r0, [r2, #1]
004f39dc: ldrb     r1, [r3, #-1]
004f39e0: cmp      r3, r2
004f39e4: eor      r1, r0, r1
004f39e8: strb     r1, [r3, #-1]
004f39ec: ldrb     r0, [r2, #1]
004f39f0: eor      r1, r1, r0
004f39f4: strb     r1, [r2, #1]
004f39f8: ldrb     r0, [r3, #-1]
004f39fc: sub      r2, r2, #1
004f3a00: eor      r1, r1, r0
004f3a04: strb     r1, [r3, #-1]
004f3a08: add      r3, r3, #1
004f3a0c: blo      #0x4f39d8
004f3a10: mov      r0, r5
004f3a14: add      r1, r4, #0xd4
004f3a18: bl       #0x459090
004f3a1c: mov      r3, #1
004f3a20: cmp      r3, #0
004f3a24: str      r3, [sp, #4]
004f3a28: bne      #0x4f3a6c
004f3a2c: add      r3, r4, #0xd5
004f3a30: add      r2, r4, #0xd6
004f3a34: ldrb     r0, [r2, #1]
004f3a38: ldrb     r1, [r3, #-1]
004f3a3c: cmp      r3, r2
004f3a40: eor      r1, r0, r1
004f3a44: strb     r1, [r3, #-1]
004f3a48: ldrb     r0, [r2, #1]
004f3a4c: eor      r1, r1, r0
004f3a50: strb     r1, [r2, #1]
004f3a54: ldrb     r0, [r3, #-1]
004f3a58: sub      r2, r2, #1
004f3a5c: eor      r1, r1, r0
004f3a60: strb     r1, [r3, #-1]
004f3a64: add      r3, r3, #1
004f3a68: blo      #0x4f3a34
004f3a6c: mov      r0, r5
004f3a70: add      r1, r4, #0xd8
004f3a74: bl       #0x459090
004f3a78: mov      r3, #1
004f3a7c: cmp      r3, #0
004f3a80: str      r3, [sp, #4]
004f3a84: bne      #0x4f3ac8
004f3a88: add      r3, r4, #0xd9
004f3a8c: add      r2, r4, #0xda
004f3a90: ldrb     r0, [r2, #1]
004f3a94: ldrb     r1, [r3, #-1]
004f3a98: cmp      r3, r2
004f3a9c: eor      r1, r0, r1
004f3aa0: strb     r1, [r3, #-1]
004f3aa4: ldrb     r0, [r2, #1]
004f3aa8: eor      r1, r1, r0
004f3aac: strb     r1, [r2, #1]
004f3ab0: ldrb     r0, [r3, #-1]
004f3ab4: sub      r2, r2, #1
004f3ab8: eor      r1, r1, r0
004f3abc: strb     r1, [r3, #-1]
004f3ac0: add      r3, r3, #1
004f3ac4: blo      #0x4f3a90
004f3ac8: mov      r0, r5
004f3acc: add      r1, r4, #0xdc
004f3ad0: bl       #0x459090
004f3ad4: mov      r3, #1
004f3ad8: cmp      r3, #0
004f3adc: str      r3, [sp, #4]
004f3ae0: bne      #0x4f3b24
004f3ae4: add      r3, r4, #0xdd
004f3ae8: add      r2, r4, #0xde
004f3aec: ldrb     r0, [r2, #1]
004f3af0: ldrb     r1, [r3, #-1]
004f3af4: cmp      r3, r2
004f3af8: eor      r1, r0, r1
004f3afc: strb     r1, [r3, #-1]
004f3b00: ldrb     r0, [r2, #1]
004f3b04: eor      r1, r1, r0
004f3b08: strb     r1, [r2, #1]
004f3b0c: ldrb     r0, [r3, #-1]
004f3b10: sub      r2, r2, #1
004f3b14: eor      r1, r1, r0
004f3b18: strb     r1, [r3, #-1]
004f3b1c: add      r3, r3, #1
004f3b20: blo      #0x4f3aec
004f3b24: mov      r0, r5
004f3b28: add      r1, r4, #0xe0
004f3b2c: bl       #0x459090
004f3b30: mov      r3, #1
004f3b34: cmp      r3, #0
004f3b38: str      r3, [sp, #4]
004f3b3c: bne      #0x4f3b80
004f3b40: add      r3, r4, #0xe1
004f3b44: add      r2, r4, #0xe2
004f3b48: ldrb     r0, [r2, #1]
004f3b4c: ldrb     r1, [r3, #-1]
004f3b50: cmp      r3, r2
004f3b54: eor      r1, r0, r1
004f3b58: strb     r1, [r3, #-1]
004f3b5c: ldrb     r0, [r2, #1]
004f3b60: eor      r1, r1, r0
004f3b64: strb     r1, [r2, #1]
004f3b68: ldrb     r0, [r3, #-1]
004f3b6c: sub      r2, r2, #1
004f3b70: eor      r1, r1, r0
004f3b74: strb     r1, [r3, #-1]
004f3b78: add      r3, r3, #1
004f3b7c: blo      #0x4f3b48
004f3b80: mov      r0, r5
004f3b84: add      r1, r4, #0xe4
004f3b88: bl       #0x459090
004f3b8c: mov      r3, #1
004f3b90: cmp      r3, #0
004f3b94: str      r3, [sp, #4]
004f3b98: bne      #0x4f3bdc
004f3b9c: add      r3, r4, #0xe5
004f3ba0: add      r2, r4, #0xe6
004f3ba4: ldrb     r0, [r2, #1]
004f3ba8: ldrb     r1, [r3, #-1]
004f3bac: cmp      r3, r2
004f3bb0: eor      r1, r0, r1
004f3bb4: strb     r1, [r3, #-1]
004f3bb8: ldrb     r0, [r2, #1]
004f3bbc: eor      r1, r1, r0
004f3bc0: strb     r1, [r2, #1]
004f3bc4: ldrb     r0, [r3, #-1]
004f3bc8: sub      r2, r2, #1
004f3bcc: eor      r1, r1, r0
004f3bd0: strb     r1, [r3, #-1]
004f3bd4: add      r3, r3, #1
004f3bd8: blo      #0x4f3ba4
004f3bdc: mov      r0, r5
004f3be0: add      r1, r4, #0xe8
004f3be4: bl       #0x459090
004f3be8: mov      r3, #1
004f3bec: cmp      r3, #0
004f3bf0: str      r3, [sp, #4]
004f3bf4: bne      #0x4f3c38
004f3bf8: add      r3, r4, #0xe9
004f3bfc: add      r2, r4, #0xea
004f3c00: ldrb     r0, [r2, #1]
004f3c04: ldrb     r1, [r3, #-1]
004f3c08: cmp      r3, r2
004f3c0c: eor      r1, r0, r1
004f3c10: strb     r1, [r3, #-1]
004f3c14: ldrb     r0, [r2, #1]
004f3c18: eor      r1, r1, r0
004f3c1c: strb     r1, [r2, #1]
004f3c20: ldrb     r0, [r3, #-1]
004f3c24: sub      r2, r2, #1
004f3c28: eor      r1, r1, r0
004f3c2c: strb     r1, [r3, #-1]
004f3c30: add      r3, r3, #1
004f3c34: blo      #0x4f3c00
004f3c38: mov      r0, r5
004f3c3c: add      r1, r4, #0xec
004f3c40: bl       #0x459090
004f3c44: mov      r3, #1
004f3c48: cmp      r3, #0
004f3c4c: str      r3, [sp, #4]
004f3c50: bne      #0x4f3c94
004f3c54: add      r3, r4, #0xed
004f3c58: add      r2, r4, #0xee
004f3c5c: ldrb     r0, [r2, #1]
004f3c60: ldrb     r1, [r3, #-1]
004f3c64: cmp      r3, r2
004f3c68: eor      r1, r0, r1
004f3c6c: strb     r1, [r3, #-1]
004f3c70: ldrb     r0, [r2, #1]
004f3c74: eor      r1, r1, r0
004f3c78: strb     r1, [r2, #1]
004f3c7c: ldrb     r0, [r3, #-1]
004f3c80: sub      r2, r2, #1
004f3c84: eor      r1, r1, r0
004f3c88: strb     r1, [r3, #-1]
004f3c8c: add      r3, r3, #1
004f3c90: blo      #0x4f3c5c
004f3c94: mov      r0, r5
004f3c98: add      r1, r4, #0xf0
004f3c9c: bl       #0x459090
004f3ca0: mov      r3, #1
004f3ca4: cmp      r3, #0
004f3ca8: str      r3, [sp, #4]
004f3cac: bne      #0x4f3cf0
004f3cb0: add      r3, r4, #0xf1
004f3cb4: add      r2, r4, #0xf2
004f3cb8: ldrb     r0, [r2, #1]
004f3cbc: ldrb     r1, [r3, #-1]
004f3cc0: cmp      r3, r2
004f3cc4: eor      r1, r0, r1
004f3cc8: strb     r1, [r3, #-1]
004f3ccc: ldrb     r0, [r2, #1]
004f3cd0: eor      r1, r1, r0
004f3cd4: strb     r1, [r2, #1]
004f3cd8: ldrb     r0, [r3, #-1]
004f3cdc: sub      r2, r2, #1
004f3ce0: eor      r1, r1, r0
004f3ce4: strb     r1, [r3, #-1]
004f3ce8: add      r3, r3, #1
004f3cec: blo      #0x4f3cb8
004f3cf0: mov      r0, r5
004f3cf4: add      r1, r4, #0xf4
004f3cf8: bl       #0x459090
004f3cfc: mov      r3, #1
004f3d00: cmp      r3, #0
004f3d04: str      r3, [sp, #4]
004f3d08: bne      #0x4f3d4c
004f3d0c: add      r3, r4, #0xf5
004f3d10: add      r2, r4, #0xf6
004f3d14: ldrb     r0, [r2, #1]
004f3d18: ldrb     r1, [r3, #-1]
004f3d1c: cmp      r3, r2
004f3d20: eor      r1, r0, r1
004f3d24: strb     r1, [r3, #-1]
004f3d28: ldrb     r0, [r2, #1]
004f3d2c: eor      r1, r1, r0
004f3d30: strb     r1, [r2, #1]
004f3d34: ldrb     r0, [r3, #-1]
004f3d38: sub      r2, r2, #1
004f3d3c: eor      r1, r1, r0
004f3d40: strb     r1, [r3, #-1]
004f3d44: add      r3, r3, #1
004f3d48: blo      #0x4f3d14
004f3d4c: mov      r0, r5
004f3d50: add      r1, r4, #0xf8
004f3d54: bl       #0x459090
004f3d58: mov      r3, #1
004f3d5c: cmp      r3, #0
004f3d60: str      r3, [sp, #4]
004f3d64: bne      #0x4f3da8
004f3d68: add      r3, r4, #0xf9
004f3d6c: add      r2, r4, #0xfa
004f3d70: ldrb     r0, [r2, #1]
004f3d74: ldrb     r1, [r3, #-1]
004f3d78: cmp      r3, r2
004f3d7c: eor      r1, r0, r1
004f3d80: strb     r1, [r3, #-1]
004f3d84: ldrb     r0, [r2, #1]
004f3d88: eor      r1, r1, r0
004f3d8c: strb     r1, [r2, #1]
004f3d90: ldrb     r0, [r3, #-1]
004f3d94: sub      r2, r2, #1
004f3d98: eor      r1, r1, r0
004f3d9c: strb     r1, [r3, #-1]
004f3da0: add      r3, r3, #1
004f3da4: blo      #0x4f3d70
004f3da8: mov      r0, r5
004f3dac: add      r1, r4, #0xfc
004f3db0: bl       #0x459090
004f3db4: mov      r3, #1
004f3db8: cmp      r3, #0
004f3dbc: str      r3, [sp, #4]
004f3dc0: bne      #0x4f3e04
004f3dc4: add      r3, r4, #0xfd
004f3dc8: add      r2, r4, #0xfe
004f3dcc: ldrb     r0, [r2, #1]
004f3dd0: ldrb     r1, [r3, #-1]
004f3dd4: cmp      r3, r2
004f3dd8: eor      r1, r0, r1
004f3ddc: strb     r1, [r3, #-1]
004f3de0: ldrb     r0, [r2, #1]
004f3de4: eor      r1, r1, r0
004f3de8: strb     r1, [r2, #1]
004f3dec: ldrb     r0, [r3, #-1]
004f3df0: sub      r2, r2, #1
004f3df4: eor      r1, r1, r0
004f3df8: strb     r1, [r3, #-1]
004f3dfc: add      r3, r3, #1
004f3e00: blo      #0x4f3dcc
004f3e04: add      r6, r4, #0x100
004f3e08: mov      r0, r5
004f3e0c: mov      r1, r6
004f3e10: bl       #0x459090
004f3e14: mov      r3, #1
004f3e18: cmp      r3, #0
004f3e1c: str      r3, [sp, #4]
004f3e20: bne      #0x4f3e64
004f3e24: add      r3, r6, #2
004f3e28: add      r6, r6, #1
004f3e2c: ldrb     r1, [r3, #1]
004f3e30: ldrb     r2, [r6, #-1]
004f3e34: cmp      r6, r3
004f3e38: eor      r2, r1, r2
004f3e3c: strb     r2, [r6, #-1]
004f3e40: ldrb     r1, [r3, #1]
004f3e44: eor      r2, r2, r1
004f3e48: strb     r2, [r3, #1]
004f3e4c: ldrb     r1, [r6, #-1]
004f3e50: sub      r3, r3, #1
004f3e54: eor      r2, r2, r1
004f3e58: strb     r2, [r6, #-1]
004f3e5c: add      r6, r6, #1
004f3e60: blo      #0x4f3e2c
004f3e64: add      r6, r4, #0x104
004f3e68: mov      r0, r5
004f3e6c: mov      r1, r6
004f3e70: bl       #0x459090
004f3e74: mov      r3, #1
004f3e78: cmp      r3, #0
004f3e7c: str      r3, [sp, #4]
004f3e80: bne      #0x4f3ec4
004f3e84: add      r3, r6, #2
004f3e88: add      r6, r6, #1
004f3e8c: ldrb     r1, [r3, #1]
004f3e90: ldrb     r2, [r6, #-1]
004f3e94: cmp      r6, r3
004f3e98: eor      r2, r1, r2
004f3e9c: strb     r2, [r6, #-1]
004f3ea0: ldrb     r1, [r3, #1]
004f3ea4: eor      r2, r2, r1
004f3ea8: strb     r2, [r3, #1]
004f3eac: ldrb     r1, [r6, #-1]
004f3eb0: sub      r3, r3, #1
004f3eb4: eor      r2, r2, r1
004f3eb8: strb     r2, [r6, #-1]
004f3ebc: add      r6, r6, #1
004f3ec0: blo      #0x4f3e8c
004f3ec4: add      r6, r4, #0x108
004f3ec8: mov      r0, r5
004f3ecc: mov      r1, r6
004f3ed0: bl       #0x459090
004f3ed4: mov      r3, #1
004f3ed8: cmp      r3, #0
004f3edc: str      r3, [sp, #4]
004f3ee0: bne      #0x4f3f24
004f3ee4: add      r3, r6, #2
004f3ee8: add      r6, r6, #1
004f3eec: ldrb     r1, [r3, #1]
004f3ef0: ldrb     r2, [r6, #-1]
004f3ef4: cmp      r6, r3
004f3ef8: eor      r2, r1, r2
004f3efc: strb     r2, [r6, #-1]
004f3f00: ldrb     r1, [r3, #1]
004f3f04: eor      r2, r2, r1
004f3f08: strb     r2, [r3, #1]
004f3f0c: ldrb     r1, [r6, #-1]
004f3f10: sub      r3, r3, #1
004f3f14: eor      r2, r2, r1
004f3f18: strb     r2, [r6, #-1]
004f3f1c: add      r6, r6, #1
004f3f20: blo      #0x4f3eec
004f3f24: add      r6, r4, #0x10c
004f3f28: mov      r0, r5
004f3f2c: mov      r1, r6
004f3f30: bl       #0x459090
004f3f34: mov      r3, #1
004f3f38: cmp      r3, #0
004f3f3c: str      r3, [sp, #4]
004f3f40: bne      #0x4f3f84
004f3f44: add      r3, r6, #2
004f3f48: add      r6, r6, #1
004f3f4c: ldrb     r1, [r3, #1]
004f3f50: ldrb     r2, [r6, #-1]
004f3f54: cmp      r6, r3
004f3f58: eor      r2, r1, r2
004f3f5c: strb     r2, [r6, #-1]
004f3f60: ldrb     r1, [r3, #1]
004f3f64: eor      r2, r2, r1
004f3f68: strb     r2, [r3, #1]
004f3f6c: ldrb     r1, [r6, #-1]
004f3f70: sub      r3, r3, #1
004f3f74: eor      r2, r2, r1
004f3f78: strb     r2, [r6, #-1]
004f3f7c: add      r6, r6, #1
004f3f80: blo      #0x4f3f4c
004f3f84: add      r6, r4, #0x110
004f3f88: mov      r0, r5
004f3f8c: mov      r1, r6
004f3f90: bl       #0x459090
004f3f94: mov      r3, #1
004f3f98: cmp      r3, #0
004f3f9c: str      r3, [sp, #4]
004f3fa0: bne      #0x4f3fe4
004f3fa4: add      r3, r6, #2
004f3fa8: add      r6, r6, #1
004f3fac: ldrb     r1, [r3, #1]
004f3fb0: ldrb     r2, [r6, #-1]
004f3fb4: cmp      r6, r3
004f3fb8: eor      r2, r1, r2
004f3fbc: strb     r2, [r6, #-1]
004f3fc0: ldrb     r1, [r3, #1]
004f3fc4: eor      r2, r2, r1
004f3fc8: strb     r2, [r3, #1]
004f3fcc: ldrb     r1, [r6, #-1]
004f3fd0: sub      r3, r3, #1
004f3fd4: eor      r2, r2, r1
004f3fd8: strb     r2, [r6, #-1]
004f3fdc: add      r6, r6, #1
004f3fe0: blo      #0x4f3fac
004f3fe4: add      r6, r4, #0x114
004f3fe8: mov      r0, r5
004f3fec: mov      r1, r6
004f3ff0: bl       #0x459090
004f3ff4: mov      r3, #1
004f3ff8: cmp      r3, #0
004f3ffc: str      r3, [sp, #4]
004f4000: bne      #0x4f4044
004f4004: add      r3, r6, #2
004f4008: add      r6, r6, #1
004f400c: ldrb     r1, [r3, #1]
004f4010: ldrb     r2, [r6, #-1]
004f4014: cmp      r6, r3
004f4018: eor      r2, r1, r2
004f401c: strb     r2, [r6, #-1]
004f4020: ldrb     r1, [r3, #1]
004f4024: eor      r2, r2, r1
004f4028: strb     r2, [r3, #1]
004f402c: ldrb     r1, [r6, #-1]
004f4030: sub      r3, r3, #1
004f4034: eor      r2, r2, r1
004f4038: strb     r2, [r6, #-1]
004f403c: add      r6, r6, #1
004f4040: blo      #0x4f400c
004f4044: add      r6, r4, #0x118
004f4048: mov      r0, r5
004f404c: mov      r1, r6
004f4050: bl       #0x459090
004f4054: mov      r3, #1
004f4058: cmp      r3, #0
004f405c: str      r3, [sp, #4]
004f4060: bne      #0x4f40a4
004f4064: add      r3, r6, #2
004f4068: add      r6, r6, #1
004f406c: ldrb     r1, [r3, #1]
004f4070: ldrb     r2, [r6, #-1]
004f4074: cmp      r6, r3
004f4078: eor      r2, r1, r2
004f407c: strb     r2, [r6, #-1]
004f4080: ldrb     r1, [r3, #1]
004f4084: eor      r2, r2, r1
004f4088: strb     r2, [r3, #1]
004f408c: ldrb     r1, [r6, #-1]
004f4090: sub      r3, r3, #1
004f4094: eor      r2, r2, r1
004f4098: strb     r2, [r6, #-1]
004f409c: add      r6, r6, #1
004f40a0: blo      #0x4f406c
004f40a4: add      r6, r4, #0x11c
004f40a8: mov      r0, r5
004f40ac: mov      r1, r6
004f40b0: bl       #0x459090
004f40b4: mov      r3, #1
004f40b8: cmp      r3, #0
004f40bc: str      r3, [sp, #4]
004f40c0: bne      #0x4f4104
004f40c4: add      r3, r6, #2
004f40c8: add      r6, r6, #1
004f40cc: ldrb     r1, [r3, #1]
004f40d0: ldrb     r2, [r6, #-1]
004f40d4: cmp      r6, r3
004f40d8: eor      r2, r1, r2
004f40dc: strb     r2, [r6, #-1]
004f40e0: ldrb     r1, [r3, #1]
004f40e4: eor      r2, r2, r1
004f40e8: strb     r2, [r3, #1]
004f40ec: ldrb     r1, [r6, #-1]
004f40f0: sub      r3, r3, #1
004f40f4: eor      r2, r2, r1
004f40f8: strb     r2, [r6, #-1]
004f40fc: add      r6, r6, #1
004f4100: blo      #0x4f40cc
004f4104: add      r6, r4, #0x120
004f4108: mov      r0, r5
004f410c: mov      r1, r6
004f4110: bl       #0x459090
004f4114: mov      r3, #1
004f4118: cmp      r3, #0
004f411c: str      r3, [sp, #4]
004f4120: bne      #0x4f4164
004f4124: add      r3, r6, #2
004f4128: add      r6, r6, #1
004f412c: ldrb     r1, [r3, #1]
004f4130: ldrb     r2, [r6, #-1]
004f4134: cmp      r6, r3
004f4138: eor      r2, r1, r2
004f413c: strb     r2, [r6, #-1]
004f4140: ldrb     r1, [r3, #1]
004f4144: eor      r2, r2, r1
004f4148: strb     r2, [r3, #1]
004f414c: ldrb     r1, [r6, #-1]
004f4150: sub      r3, r3, #1
004f4154: eor      r2, r2, r1
004f4158: strb     r2, [r6, #-1]
004f415c: add      r6, r6, #1
004f4160: blo      #0x4f412c
004f4164: add      r6, r4, #0x124
004f4168: mov      r0, r5
004f416c: mov      r1, r6
004f4170: bl       #0x459090
004f4174: mov      r3, #1
004f4178: cmp      r3, #0
004f417c: str      r3, [sp, #4]
004f4180: bne      #0x4f41c4
004f4184: add      r3, r6, #2
004f4188: add      r6, r6, #1
004f418c: ldrb     r1, [r3, #1]
004f4190: ldrb     r2, [r6, #-1]
004f4194: cmp      r6, r3
004f4198: eor      r2, r1, r2
004f419c: strb     r2, [r6, #-1]
004f41a0: ldrb     r1, [r3, #1]
004f41a4: eor      r2, r2, r1
004f41a8: strb     r2, [r3, #1]
004f41ac: ldrb     r1, [r6, #-1]
004f41b0: sub      r3, r3, #1
004f41b4: eor      r2, r2, r1
004f41b8: strb     r2, [r6, #-1]
004f41bc: add      r6, r6, #1
004f41c0: blo      #0x4f418c
004f41c4: add      r6, r4, #0x128
004f41c8: mov      r0, r5
004f41cc: mov      r1, r6
004f41d0: bl       #0x459090
004f41d4: mov      r3, #1
004f41d8: cmp      r3, #0
004f41dc: str      r3, [sp, #4]
004f41e0: bne      #0x4f4224
004f41e4: add      r3, r6, #2
004f41e8: add      r6, r6, #1
004f41ec: ldrb     r1, [r3, #1]
004f41f0: ldrb     r2, [r6, #-1]
004f41f4: cmp      r6, r3
004f41f8: eor      r2, r1, r2
004f41fc: strb     r2, [r6, #-1]
004f4200: ldrb     r1, [r3, #1]
004f4204: eor      r2, r2, r1
004f4208: strb     r2, [r3, #1]
004f420c: ldrb     r1, [r6, #-1]
004f4210: sub      r3, r3, #1
004f4214: eor      r2, r2, r1
004f4218: strb     r2, [r6, #-1]
004f421c: add      r6, r6, #1
004f4220: blo      #0x4f41ec
004f4224: add      r6, r4, #0x12c
004f4228: mov      r0, r5
004f422c: mov      r1, r6
004f4230: bl       #0x459090
004f4234: mov      r3, #1
004f4238: cmp      r3, #0
004f423c: str      r3, [sp, #4]
004f4240: bne      #0x4f4284
004f4244: add      r3, r6, #2
004f4248: add      r6, r6, #1
004f424c: ldrb     r1, [r3, #1]
004f4250: ldrb     r2, [r6, #-1]
004f4254: cmp      r6, r3
004f4258: eor      r2, r1, r2
004f425c: strb     r2, [r6, #-1]
004f4260: ldrb     r1, [r3, #1]
004f4264: eor      r2, r2, r1
004f4268: strb     r2, [r3, #1]
004f426c: ldrb     r1, [r6, #-1]
004f4270: sub      r3, r3, #1
004f4274: eor      r2, r2, r1
004f4278: strb     r2, [r6, #-1]
004f427c: add      r6, r6, #1
004f4280: blo      #0x4f424c
004f4284: add      r6, r4, #0x130
004f4288: mov      r0, r5
004f428c: mov      r1, r6
004f4290: bl       #0x459090
004f4294: mov      r3, #1
004f4298: cmp      r3, #0
004f429c: str      r3, [sp, #4]
004f42a0: bne      #0x4f42e4
004f42a4: add      r3, r6, #2
004f42a8: add      r6, r6, #1
004f42ac: ldrb     r1, [r3, #1]
004f42b0: ldrb     r2, [r6, #-1]
004f42b4: cmp      r6, r3
004f42b8: eor      r2, r1, r2
004f42bc: strb     r2, [r6, #-1]
004f42c0: ldrb     r1, [r3, #1]
004f42c4: eor      r2, r2, r1
004f42c8: strb     r2, [r3, #1]
004f42cc: ldrb     r1, [r6, #-1]
004f42d0: sub      r3, r3, #1
004f42d4: eor      r2, r2, r1
004f42d8: strb     r2, [r6, #-1]
004f42dc: add      r6, r6, #1
004f42e0: blo      #0x4f42ac
004f42e4: add      r6, r4, #0x134
004f42e8: mov      r0, r5
004f42ec: mov      r1, r6
004f42f0: bl       #0x459090
004f42f4: mov      r3, #1
004f42f8: cmp      r3, #0
004f42fc: str      r3, [sp, #4]
004f4300: bne      #0x4f4344
004f4304: add      r3, r6, #2
004f4308: add      r6, r6, #1
004f430c: ldrb     r1, [r3, #1]
004f4310: ldrb     r2, [r6, #-1]
004f4314: cmp      r6, r3
004f4318: eor      r2, r1, r2
004f431c: strb     r2, [r6, #-1]
004f4320: ldrb     r1, [r3, #1]
004f4324: eor      r2, r2, r1
004f4328: strb     r2, [r3, #1]
004f432c: ldrb     r1, [r6, #-1]
004f4330: sub      r3, r3, #1
004f4334: eor      r2, r2, r1
004f4338: strb     r2, [r6, #-1]
004f433c: add      r6, r6, #1
004f4340: blo      #0x4f430c
004f4344: add      r6, r4, #0x138
004f4348: mov      r0, r5
004f434c: mov      r1, r6
004f4350: bl       #0x459090
004f4354: mov      r3, #1
004f4358: cmp      r3, #0
004f435c: str      r3, [sp, #4]
004f4360: bne      #0x4f43a4
004f4364: add      r3, r6, #2
004f4368: add      r6, r6, #1
004f436c: ldrb     r1, [r3, #1]
004f4370: ldrb     r2, [r6, #-1]
004f4374: cmp      r6, r3
004f4378: eor      r2, r1, r2
004f437c: strb     r2, [r6, #-1]
004f4380: ldrb     r1, [r3, #1]
004f4384: eor      r2, r2, r1
004f4388: strb     r2, [r3, #1]
004f438c: ldrb     r1, [r6, #-1]
004f4390: sub      r3, r3, #1
004f4394: eor      r2, r2, r1
004f4398: strb     r2, [r6, #-1]
004f439c: add      r6, r6, #1
004f43a0: blo      #0x4f436c
004f43a4: add      r6, r4, #0x13c
004f43a8: mov      r0, r5
004f43ac: mov      r1, r6
004f43b0: bl       #0x459090
004f43b4: mov      r3, #1
004f43b8: cmp      r3, #0
004f43bc: str      r3, [sp, #4]
004f43c0: bne      #0x4f4404
004f43c4: add      r3, r6, #2
004f43c8: add      r6, r6, #1
004f43cc: ldrb     r1, [r3, #1]
004f43d0: ldrb     r2, [r6, #-1]
004f43d4: cmp      r6, r3
004f43d8: eor      r2, r1, r2
004f43dc: strb     r2, [r6, #-1]
004f43e0: ldrb     r1, [r3, #1]
004f43e4: eor      r2, r2, r1
004f43e8: strb     r2, [r3, #1]
004f43ec: ldrb     r1, [r6, #-1]
004f43f0: sub      r3, r3, #1
004f43f4: eor      r2, r2, r1
004f43f8: strb     r2, [r6, #-1]
004f43fc: add      r6, r6, #1
004f4400: blo      #0x4f43cc
004f4404: add      r6, r4, #0x140
004f4408: mov      r0, r5
004f440c: mov      r1, r6
004f4410: bl       #0x459090
004f4414: mov      r3, #1
004f4418: cmp      r3, #0
004f441c: str      r3, [sp, #4]
004f4420: bne      #0x4f4464
004f4424: add      r3, r6, #2
004f4428: add      r6, r6, #1
004f442c: ldrb     r1, [r3, #1]
004f4430: ldrb     r2, [r6, #-1]
004f4434: cmp      r6, r3
004f4438: eor      r2, r1, r2
004f443c: strb     r2, [r6, #-1]
004f4440: ldrb     r1, [r3, #1]
004f4444: eor      r2, r2, r1
004f4448: strb     r2, [r3, #1]
004f444c: ldrb     r1, [r6, #-1]
004f4450: sub      r3, r3, #1
004f4454: eor      r2, r2, r1
004f4458: strb     r2, [r6, #-1]
004f445c: add      r6, r6, #1
004f4460: blo      #0x4f442c
004f4464: add      r6, r4, #0x144
004f4468: mov      r0, r5
004f446c: mov      r1, r6
004f4470: bl       #0x459090
004f4474: mov      r3, #1
004f4478: cmp      r3, #0
004f447c: str      r3, [sp, #4]
004f4480: bne      #0x4f44c4
004f4484: add      r3, r6, #2
004f4488: add      r6, r6, #1
004f448c: ldrb     r1, [r3, #1]
004f4490: ldrb     r2, [r6, #-1]
004f4494: cmp      r6, r3
004f4498: eor      r2, r1, r2
004f449c: strb     r2, [r6, #-1]
004f44a0: ldrb     r1, [r3, #1]
004f44a4: eor      r2, r2, r1
004f44a8: strb     r2, [r3, #1]
004f44ac: ldrb     r1, [r6, #-1]
004f44b0: sub      r3, r3, #1
004f44b4: eor      r2, r2, r1
004f44b8: strb     r2, [r6, #-1]
004f44bc: add      r6, r6, #1
004f44c0: blo      #0x4f448c
004f44c4: add      r6, r4, #0x148
004f44c8: mov      r0, r5
004f44cc: mov      r1, r6
004f44d0: bl       #0x459090
004f44d4: mov      r3, #1
004f44d8: cmp      r3, #0
004f44dc: str      r3, [sp, #4]
004f44e0: bne      #0x4f4524
004f44e4: add      r3, r6, #2
004f44e8: add      r6, r6, #1
004f44ec: ldrb     r1, [r3, #1]
004f44f0: ldrb     r2, [r6, #-1]
004f44f4: cmp      r6, r3
004f44f8: eor      r2, r1, r2
004f44fc: strb     r2, [r6, #-1]
004f4500: ldrb     r1, [r3, #1]
004f4504: eor      r2, r2, r1
004f4508: strb     r2, [r3, #1]
004f450c: ldrb     r1, [r6, #-1]
004f4510: sub      r3, r3, #1
004f4514: eor      r2, r2, r1
004f4518: strb     r2, [r6, #-1]
004f451c: add      r6, r6, #1
004f4520: blo      #0x4f44ec
004f4524: add      r6, r4, #0x14c
004f4528: mov      r0, r5
004f452c: mov      r1, r6
004f4530: bl       #0x459090
004f4534: mov      r3, #1
004f4538: cmp      r3, #0
004f453c: str      r3, [sp, #4]
004f4540: bne      #0x4f4584
004f4544: add      r3, r6, #2
004f4548: add      r6, r6, #1
004f454c: ldrb     r1, [r3, #1]
004f4550: ldrb     r2, [r6, #-1]
004f4554: cmp      r6, r3
004f4558: eor      r2, r1, r2
004f455c: strb     r2, [r6, #-1]
004f4560: ldrb     r1, [r3, #1]
004f4564: eor      r2, r2, r1
004f4568: strb     r2, [r3, #1]
004f456c: ldrb     r1, [r6, #-1]
004f4570: sub      r3, r3, #1
004f4574: eor      r2, r2, r1
004f4578: strb     r2, [r6, #-1]
004f457c: add      r6, r6, #1
004f4580: blo      #0x4f454c
004f4584: add      r6, r4, #0x150
004f4588: mov      r0, r5
004f458c: mov      r1, r6
004f4590: bl       #0x459090
004f4594: mov      r3, #1
004f4598: cmp      r3, #0
004f459c: str      r3, [sp, #4]
004f45a0: bne      #0x4f45e4
004f45a4: add      r3, r6, #2
004f45a8: add      r6, r6, #1
004f45ac: ldrb     r1, [r3, #1]
004f45b0: ldrb     r2, [r6, #-1]
004f45b4: cmp      r6, r3
004f45b8: eor      r2, r1, r2
004f45bc: strb     r2, [r6, #-1]
004f45c0: ldrb     r1, [r3, #1]
004f45c4: eor      r2, r2, r1
004f45c8: strb     r2, [r3, #1]
004f45cc: ldrb     r1, [r6, #-1]
004f45d0: sub      r3, r3, #1
004f45d4: eor      r2, r2, r1
004f45d8: strb     r2, [r6, #-1]
004f45dc: add      r6, r6, #1
004f45e0: blo      #0x4f45ac
004f45e4: add      r6, r4, #0x154
004f45e8: mov      r0, r5
004f45ec: mov      r1, r6
004f45f0: bl       #0x459090
004f45f4: mov      r3, #1
004f45f8: cmp      r3, #0
004f45fc: str      r3, [sp, #4]
004f4600: bne      #0x4f4644
004f4604: add      r3, r6, #2
004f4608: add      r6, r6, #1
004f460c: ldrb     r1, [r3, #1]
004f4610: ldrb     r2, [r6, #-1]
004f4614: cmp      r6, r3
004f4618: eor      r2, r1, r2
004f461c: strb     r2, [r6, #-1]
004f4620: ldrb     r1, [r3, #1]
004f4624: eor      r2, r2, r1
004f4628: strb     r2, [r3, #1]
004f462c: ldrb     r1, [r6, #-1]
004f4630: sub      r3, r3, #1
004f4634: eor      r2, r2, r1
004f4638: strb     r2, [r6, #-1]
004f463c: add      r6, r6, #1
004f4640: blo      #0x4f460c
004f4644: add      r6, r4, #0x158
004f4648: mov      r0, r5
004f464c: mov      r1, r6
004f4650: bl       #0x459090
004f4654: mov      r3, #1
004f4658: cmp      r3, #0
004f465c: str      r3, [sp, #4]
004f4660: bne      #0x4f46a4
004f4664: add      r3, r6, #2
004f4668: add      r6, r6, #1
004f466c: ldrb     r1, [r3, #1]
004f4670: ldrb     r2, [r6, #-1]
004f4674: cmp      r6, r3
004f4678: eor      r2, r1, r2
004f467c: strb     r2, [r6, #-1]
004f4680: ldrb     r1, [r3, #1]
004f4684: eor      r2, r2, r1
004f4688: strb     r2, [r3, #1]
004f468c: ldrb     r1, [r6, #-1]
004f4690: sub      r3, r3, #1
004f4694: eor      r2, r2, r1
004f4698: strb     r2, [r6, #-1]
004f469c: add      r6, r6, #1
004f46a0: blo      #0x4f466c
004f46a4: add      r6, r4, #0x15c
004f46a8: mov      r0, r5
004f46ac: mov      r1, r6
004f46b0: bl       #0x459090
004f46b4: mov      r3, #1
004f46b8: cmp      r3, #0
004f46bc: str      r3, [sp, #4]
004f46c0: bne      #0x4f4704
004f46c4: add      r3, r6, #2
004f46c8: add      r6, r6, #1
004f46cc: ldrb     r1, [r3, #1]
004f46d0: ldrb     r2, [r6, #-1]
004f46d4: cmp      r6, r3
004f46d8: eor      r2, r1, r2
004f46dc: strb     r2, [r6, #-1]
004f46e0: ldrb     r1, [r3, #1]
004f46e4: eor      r2, r2, r1
004f46e8: strb     r2, [r3, #1]
004f46ec: ldrb     r1, [r6, #-1]
004f46f0: sub      r3, r3, #1
004f46f4: eor      r2, r2, r1
004f46f8: strb     r2, [r6, #-1]
004f46fc: add      r6, r6, #1
004f4700: blo      #0x4f46cc
004f4704: add      r6, r4, #0x160
004f4708: mov      r0, r5
004f470c: mov      r1, r6
004f4710: bl       #0x459090
004f4714: mov      r3, #1
004f4718: cmp      r3, #0
004f471c: str      r3, [sp, #4]
004f4720: bne      #0x4f4764
004f4724: add      r3, r6, #2
004f4728: add      r6, r6, #1
004f472c: ldrb     r1, [r3, #1]
004f4730: ldrb     r2, [r6, #-1]
004f4734: cmp      r6, r3
004f4738: eor      r2, r1, r2
004f473c: strb     r2, [r6, #-1]
004f4740: ldrb     r1, [r3, #1]
004f4744: eor      r2, r2, r1
004f4748: strb     r2, [r3, #1]
004f474c: ldrb     r1, [r6, #-1]
004f4750: sub      r3, r3, #1
004f4754: eor      r2, r2, r1
004f4758: strb     r2, [r6, #-1]
004f475c: add      r6, r6, #1
004f4760: blo      #0x4f472c
004f4764: add      r6, r4, #0x164
004f4768: mov      r0, r5
004f476c: mov      r1, r6
004f4770: bl       #0x459090
004f4774: mov      r3, #1
004f4778: cmp      r3, #0
004f477c: str      r3, [sp, #4]
004f4780: bne      #0x4f47c4
004f4784: add      r3, r6, #2
004f4788: add      r6, r6, #1
004f478c: ldrb     r1, [r3, #1]
004f4790: ldrb     r2, [r6, #-1]
004f4794: cmp      r6, r3
004f4798: eor      r2, r1, r2
004f479c: strb     r2, [r6, #-1]
004f47a0: ldrb     r1, [r3, #1]
004f47a4: eor      r2, r2, r1
004f47a8: strb     r2, [r3, #1]
004f47ac: ldrb     r1, [r6, #-1]
004f47b0: sub      r3, r3, #1
004f47b4: eor      r2, r2, r1
004f47b8: strb     r2, [r6, #-1]
004f47bc: add      r6, r6, #1
004f47c0: blo      #0x4f478c
004f47c4: add      r6, r4, #0x168
004f47c8: mov      r0, r5
004f47cc: mov      r1, r6
004f47d0: bl       #0x459090
004f47d4: mov      r3, #1
004f47d8: cmp      r3, #0
004f47dc: str      r3, [sp, #4]
004f47e0: bne      #0x4f4824
004f47e4: add      r3, r6, #2
004f47e8: add      r6, r6, #1
004f47ec: ldrb     r1, [r3, #1]
004f47f0: ldrb     r2, [r6, #-1]
004f47f4: cmp      r6, r3
004f47f8: eor      r2, r1, r2
004f47fc: strb     r2, [r6, #-1]
004f4800: ldrb     r1, [r3, #1]
004f4804: eor      r2, r2, r1
004f4808: strb     r2, [r3, #1]
004f480c: ldrb     r1, [r6, #-1]
004f4810: sub      r3, r3, #1
004f4814: eor      r2, r2, r1
004f4818: strb     r2, [r6, #-1]
004f481c: add      r6, r6, #1
004f4820: blo      #0x4f47ec
004f4824: add      r6, r4, #0x16c
004f4828: mov      r0, r5
004f482c: mov      r1, r6
004f4830: bl       #0x459090
004f4834: mov      r3, #1
004f4838: cmp      r3, #0
004f483c: str      r3, [sp, #4]
004f4840: bne      #0x4f4884
004f4844: add      r3, r6, #2
004f4848: add      r6, r6, #1
004f484c: ldrb     r1, [r3, #1]
004f4850: ldrb     r2, [r6, #-1]
004f4854: cmp      r6, r3
004f4858: eor      r2, r1, r2
004f485c: strb     r2, [r6, #-1]
004f4860: ldrb     r1, [r3, #1]
004f4864: eor      r2, r2, r1
004f4868: strb     r2, [r3, #1]
004f486c: ldrb     r1, [r6, #-1]
004f4870: sub      r3, r3, #1
004f4874: eor      r2, r2, r1
004f4878: strb     r2, [r6, #-1]
004f487c: add      r6, r6, #1
004f4880: blo      #0x4f484c
004f4884: add      r6, r4, #0x170
004f4888: mov      r0, r5
004f488c: mov      r1, r6
004f4890: bl       #0x459090
004f4894: mov      r3, #1
004f4898: cmp      r3, #0
004f489c: str      r3, [sp, #4]
004f48a0: bne      #0x4f48e4
004f48a4: add      r3, r6, #2
004f48a8: add      r6, r6, #1
004f48ac: ldrb     r1, [r3, #1]
004f48b0: ldrb     r2, [r6, #-1]
004f48b4: cmp      r6, r3
004f48b8: eor      r2, r1, r2
004f48bc: strb     r2, [r6, #-1]
004f48c0: ldrb     r1, [r3, #1]
004f48c4: eor      r2, r2, r1
004f48c8: strb     r2, [r3, #1]
004f48cc: ldrb     r1, [r6, #-1]
004f48d0: sub      r3, r3, #1
004f48d4: eor      r2, r2, r1
004f48d8: strb     r2, [r6, #-1]
004f48dc: add      r6, r6, #1
004f48e0: blo      #0x4f48ac
004f48e4: add      r6, r4, #0x174
004f48e8: mov      r0, r5
004f48ec: mov      r1, r6
004f48f0: bl       #0x459090
004f48f4: mov      r3, #1
004f48f8: cmp      r3, #0
004f48fc: str      r3, [sp, #4]
004f4900: bne      #0x4f4944
004f4904: add      r3, r6, #2
004f4908: add      r6, r6, #1
004f490c: ldrb     r1, [r3, #1]
004f4910: ldrb     r2, [r6, #-1]
004f4914: cmp      r6, r3
004f4918: eor      r2, r1, r2
004f491c: strb     r2, [r6, #-1]
004f4920: ldrb     r1, [r3, #1]
004f4924: eor      r2, r2, r1
004f4928: strb     r2, [r3, #1]
004f492c: ldrb     r1, [r6, #-1]
004f4930: sub      r3, r3, #1
004f4934: eor      r2, r2, r1
004f4938: strb     r2, [r6, #-1]
004f493c: add      r6, r6, #1
004f4940: blo      #0x4f490c
004f4944: add      r6, r4, #0x178
004f4948: mov      r0, r5
004f494c: mov      r1, r6
004f4950: bl       #0x459090
004f4954: mov      r3, #1
004f4958: cmp      r3, #0
004f495c: str      r3, [sp, #4]
004f4960: bne      #0x4f49a4
004f4964: add      r3, r6, #2
004f4968: add      r6, r6, #1
004f496c: ldrb     r1, [r3, #1]
004f4970: ldrb     r2, [r6, #-1]
004f4974: cmp      r6, r3
004f4978: eor      r2, r1, r2
004f497c: strb     r2, [r6, #-1]
004f4980: ldrb     r1, [r3, #1]
004f4984: eor      r2, r2, r1
004f4988: strb     r2, [r3, #1]
004f498c: ldrb     r1, [r6, #-1]
004f4990: sub      r3, r3, #1
004f4994: eor      r2, r2, r1
004f4998: strb     r2, [r6, #-1]
004f499c: add      r6, r6, #1
004f49a0: blo      #0x4f496c
004f49a4: add      r6, r4, #0x17c
004f49a8: mov      r0, r5
004f49ac: mov      r1, r6
004f49b0: bl       #0x459090
004f49b4: mov      r3, #1
004f49b8: cmp      r3, #0
004f49bc: str      r3, [sp, #4]
004f49c0: bne      #0x4f4a04
004f49c4: add      r3, r6, #2
004f49c8: add      r6, r6, #1
004f49cc: ldrb     r1, [r3, #1]
004f49d0: ldrb     r2, [r6, #-1]
004f49d4: cmp      r6, r3
004f49d8: eor      r2, r1, r2
004f49dc: strb     r2, [r6, #-1]
004f49e0: ldrb     r1, [r3, #1]
004f49e4: eor      r2, r2, r1
004f49e8: strb     r2, [r3, #1]
004f49ec: ldrb     r1, [r6, #-1]
004f49f0: sub      r3, r3, #1
004f49f4: eor      r2, r2, r1
004f49f8: strb     r2, [r6, #-1]
004f49fc: add      r6, r6, #1
004f4a00: blo      #0x4f49cc
004f4a04: add      r6, r4, #0x180
004f4a08: mov      r0, r5
004f4a0c: mov      r1, r6
004f4a10: bl       #0x459090
004f4a14: mov      r3, #1
004f4a18: cmp      r3, #0
004f4a1c: str      r3, [sp, #4]
004f4a20: bne      #0x4f4a64
004f4a24: add      r3, r6, #2
004f4a28: add      r6, r6, #1
004f4a2c: ldrb     r1, [r3, #1]
004f4a30: ldrb     r2, [r6, #-1]
004f4a34: cmp      r6, r3
004f4a38: eor      r2, r1, r2
004f4a3c: strb     r2, [r6, #-1]
004f4a40: ldrb     r1, [r3, #1]
004f4a44: eor      r2, r2, r1
004f4a48: strb     r2, [r3, #1]
004f4a4c: ldrb     r1, [r6, #-1]
004f4a50: sub      r3, r3, #1
004f4a54: eor      r2, r2, r1
004f4a58: strb     r2, [r6, #-1]
004f4a5c: add      r6, r6, #1
004f4a60: blo      #0x4f4a2c
004f4a64: add      r6, r4, #0x184
004f4a68: mov      r0, r5
004f4a6c: mov      r1, r6
004f4a70: bl       #0x459090
004f4a74: mov      r3, #1
004f4a78: cmp      r3, #0
004f4a7c: str      r3, [sp, #4]
004f4a80: bne      #0x4f4ac4
004f4a84: add      r3, r6, #2
004f4a88: add      r6, r6, #1
004f4a8c: ldrb     r1, [r3, #1]
004f4a90: ldrb     r2, [r6, #-1]
004f4a94: cmp      r6, r3
004f4a98: eor      r2, r1, r2
004f4a9c: strb     r2, [r6, #-1]
004f4aa0: ldrb     r1, [r3, #1]
004f4aa4: eor      r2, r2, r1
004f4aa8: strb     r2, [r3, #1]
004f4aac: ldrb     r1, [r6, #-1]
004f4ab0: sub      r3, r3, #1
004f4ab4: eor      r2, r2, r1
004f4ab8: strb     r2, [r6, #-1]
004f4abc: add      r6, r6, #1
004f4ac0: blo      #0x4f4a8c
004f4ac4: add      r6, r4, #0x188
004f4ac8: mov      r0, r5
004f4acc: mov      r1, r6
004f4ad0: bl       #0x459090
004f4ad4: mov      r3, #1
004f4ad8: cmp      r3, #0
004f4adc: str      r3, [sp, #4]
004f4ae0: bne      #0x4f4b24
004f4ae4: add      r3, r6, #2
004f4ae8: add      r6, r6, #1
004f4aec: ldrb     r1, [r3, #1]
004f4af0: ldrb     r2, [r6, #-1]
004f4af4: cmp      r6, r3
004f4af8: eor      r2, r1, r2
004f4afc: strb     r2, [r6, #-1]
004f4b00: ldrb     r1, [r3, #1]
004f4b04: eor      r2, r2, r1
004f4b08: strb     r2, [r3, #1]
004f4b0c: ldrb     r1, [r6, #-1]
004f4b10: sub      r3, r3, #1
004f4b14: eor      r2, r2, r1
004f4b18: strb     r2, [r6, #-1]
004f4b1c: add      r6, r6, #1
004f4b20: blo      #0x4f4aec
004f4b24: add      r6, r4, #0x18c
004f4b28: mov      r0, r5
004f4b2c: mov      r1, r6
004f4b30: bl       #0x459090
004f4b34: mov      r3, #1
004f4b38: cmp      r3, #0
004f4b3c: str      r3, [sp, #4]
004f4b40: bne      #0x4f4b84
004f4b44: add      r3, r6, #2
004f4b48: add      r6, r6, #1
004f4b4c: ldrb     r1, [r3, #1]
004f4b50: ldrb     r2, [r6, #-1]
004f4b54: cmp      r6, r3
004f4b58: eor      r2, r1, r2
004f4b5c: strb     r2, [r6, #-1]
004f4b60: ldrb     r1, [r3, #1]
004f4b64: eor      r2, r2, r1
004f4b68: strb     r2, [r3, #1]
004f4b6c: ldrb     r1, [r6, #-1]
004f4b70: sub      r3, r3, #1
004f4b74: eor      r2, r2, r1
004f4b78: strb     r2, [r6, #-1]
004f4b7c: add      r6, r6, #1
004f4b80: blo      #0x4f4b4c
004f4b84: add      r6, r4, #0x190
004f4b88: mov      r0, r5
004f4b8c: mov      r1, r6
004f4b90: bl       #0x459090
004f4b94: mov      r3, #1
004f4b98: cmp      r3, #0
004f4b9c: str      r3, [sp, #4]
004f4ba0: bne      #0x4f4be4
004f4ba4: add      r3, r6, #2
004f4ba8: add      r6, r6, #1
004f4bac: ldrb     r1, [r3, #1]
004f4bb0: ldrb     r2, [r6, #-1]
004f4bb4: cmp      r6, r3
004f4bb8: eor      r2, r1, r2
004f4bbc: strb     r2, [r6, #-1]
004f4bc0: ldrb     r1, [r3, #1]
004f4bc4: eor      r2, r2, r1
004f4bc8: strb     r2, [r3, #1]
004f4bcc: ldrb     r1, [r6, #-1]
004f4bd0: sub      r3, r3, #1
004f4bd4: eor      r2, r2, r1
004f4bd8: strb     r2, [r6, #-1]
004f4bdc: add      r6, r6, #1
004f4be0: blo      #0x4f4bac
004f4be4: add      r6, r4, #0x194
004f4be8: mov      r0, r5
004f4bec: mov      r1, r6
004f4bf0: bl       #0x459090
004f4bf4: mov      r3, #1
004f4bf8: cmp      r3, #0
004f4bfc: str      r3, [sp, #4]
004f4c00: bne      #0x4f4c44
004f4c04: add      r3, r6, #2
004f4c08: add      r6, r6, #1
004f4c0c: ldrb     r1, [r3, #1]
004f4c10: ldrb     r2, [r6, #-1]
004f4c14: cmp      r6, r3
004f4c18: eor      r2, r1, r2
004f4c1c: strb     r2, [r6, #-1]
004f4c20: ldrb     r1, [r3, #1]
004f4c24: eor      r2, r2, r1
004f4c28: strb     r2, [r3, #1]
004f4c2c: ldrb     r1, [r6, #-1]
004f4c30: sub      r3, r3, #1
004f4c34: eor      r2, r2, r1
004f4c38: strb     r2, [r6, #-1]
004f4c3c: add      r6, r6, #1
004f4c40: blo      #0x4f4c0c
004f4c44: add      r6, r4, #0x198
004f4c48: mov      r0, r5
004f4c4c: mov      r1, r6
004f4c50: bl       #0x459090
004f4c54: mov      r3, #1
004f4c58: cmp      r3, #0
004f4c5c: str      r3, [sp, #4]
004f4c60: bne      #0x4f4ca4
004f4c64: add      r3, r6, #2
004f4c68: add      r6, r6, #1
004f4c6c: ldrb     r1, [r3, #1]
004f4c70: ldrb     r2, [r6, #-1]
004f4c74: cmp      r6, r3
004f4c78: eor      r2, r1, r2
004f4c7c: strb     r2, [r6, #-1]
004f4c80: ldrb     r1, [r3, #1]
004f4c84: eor      r2, r2, r1
004f4c88: strb     r2, [r3, #1]
004f4c8c: ldrb     r1, [r6, #-1]
004f4c90: sub      r3, r3, #1
004f4c94: eor      r2, r2, r1
004f4c98: strb     r2, [r6, #-1]
004f4c9c: add      r6, r6, #1
004f4ca0: blo      #0x4f4c6c
004f4ca4: add      r6, r4, #0x19c
004f4ca8: mov      r0, r5
004f4cac: mov      r1, r6
004f4cb0: bl       #0x459090
004f4cb4: mov      r3, #1
004f4cb8: cmp      r3, #0
004f4cbc: str      r3, [sp, #4]
004f4cc0: bne      #0x4f4d04
004f4cc4: add      r3, r6, #2
004f4cc8: add      r6, r6, #1
004f4ccc: ldrb     r1, [r3, #1]
004f4cd0: ldrb     r2, [r6, #-1]
004f4cd4: cmp      r6, r3
004f4cd8: eor      r2, r1, r2
004f4cdc: strb     r2, [r6, #-1]
004f4ce0: ldrb     r1, [r3, #1]
004f4ce4: eor      r2, r2, r1
004f4ce8: strb     r2, [r3, #1]
004f4cec: ldrb     r1, [r6, #-1]
004f4cf0: sub      r3, r3, #1
004f4cf4: eor      r2, r2, r1
004f4cf8: strb     r2, [r6, #-1]
004f4cfc: add      r6, r6, #1
004f4d00: blo      #0x4f4ccc
004f4d04: add      r6, r4, #0x1a0
004f4d08: mov      r0, r5
004f4d0c: mov      r1, r6
004f4d10: bl       #0x459090
004f4d14: mov      r3, #1
004f4d18: cmp      r3, #0
004f4d1c: str      r3, [sp, #4]
004f4d20: bne      #0x4f4d64
004f4d24: add      r3, r6, #2
004f4d28: add      r6, r6, #1
004f4d2c: ldrb     r1, [r3, #1]
004f4d30: ldrb     r2, [r6, #-1]
004f4d34: cmp      r6, r3
004f4d38: eor      r2, r1, r2
004f4d3c: strb     r2, [r6, #-1]
004f4d40: ldrb     r1, [r3, #1]
004f4d44: eor      r2, r2, r1
004f4d48: strb     r2, [r3, #1]
004f4d4c: ldrb     r1, [r6, #-1]
004f4d50: sub      r3, r3, #1
004f4d54: eor      r2, r2, r1
004f4d58: strb     r2, [r6, #-1]
004f4d5c: add      r6, r6, #1
004f4d60: blo      #0x4f4d2c
004f4d64: add      r6, r4, #0x1a4
004f4d68: mov      r0, r5
004f4d6c: mov      r1, r6
004f4d70: bl       #0x459090
004f4d74: mov      r3, #1
004f4d78: cmp      r3, #0
004f4d7c: str      r3, [sp, #4]
004f4d80: bne      #0x4f4dc4
004f4d84: add      r3, r6, #2
004f4d88: add      r6, r6, #1
004f4d8c: ldrb     r1, [r3, #1]
004f4d90: ldrb     r2, [r6, #-1]
004f4d94: cmp      r6, r3
004f4d98: eor      r2, r1, r2
004f4d9c: strb     r2, [r6, #-1]
004f4da0: ldrb     r1, [r3, #1]
004f4da4: eor      r2, r2, r1
004f4da8: strb     r2, [r3, #1]
004f4dac: ldrb     r1, [r6, #-1]
004f4db0: sub      r3, r3, #1
004f4db4: eor      r2, r2, r1
004f4db8: strb     r2, [r6, #-1]
004f4dbc: add      r6, r6, #1
004f4dc0: blo      #0x4f4d8c
004f4dc4: add      r6, r4, #0x1a8
004f4dc8: mov      r0, r5
004f4dcc: mov      r1, r6
004f4dd0: bl       #0x459090
004f4dd4: mov      r3, #1
004f4dd8: cmp      r3, #0
004f4ddc: str      r3, [sp, #4]
004f4de0: bne      #0x4f4e24
004f4de4: add      r3, r6, #2
004f4de8: add      r6, r6, #1
004f4dec: ldrb     r1, [r3, #1]
004f4df0: ldrb     r2, [r6, #-1]
004f4df4: cmp      r6, r3
004f4df8: eor      r2, r1, r2
004f4dfc: strb     r2, [r6, #-1]
004f4e00: ldrb     r1, [r3, #1]
004f4e04: eor      r2, r2, r1
004f4e08: strb     r2, [r3, #1]
004f4e0c: ldrb     r1, [r6, #-1]
004f4e10: sub      r3, r3, #1
004f4e14: eor      r2, r2, r1
004f4e18: strb     r2, [r6, #-1]
004f4e1c: add      r6, r6, #1
004f4e20: blo      #0x4f4dec
004f4e24: add      r6, r4, #0x1ac
004f4e28: mov      r0, r5
004f4e2c: mov      r1, r6
004f4e30: bl       #0x459090
004f4e34: mov      r3, #1
004f4e38: cmp      r3, #0
004f4e3c: str      r3, [sp, #4]
004f4e40: bne      #0x4f4e84
004f4e44: add      r3, r6, #2
004f4e48: add      r6, r6, #1
004f4e4c: ldrb     r1, [r3, #1]
004f4e50: ldrb     r2, [r6, #-1]
004f4e54: cmp      r6, r3
004f4e58: eor      r2, r1, r2
004f4e5c: strb     r2, [r6, #-1]
004f4e60: ldrb     r1, [r3, #1]
004f4e64: eor      r2, r2, r1
004f4e68: strb     r2, [r3, #1]
004f4e6c: ldrb     r1, [r6, #-1]
004f4e70: sub      r3, r3, #1
004f4e74: eor      r2, r2, r1
004f4e78: strb     r2, [r6, #-1]
004f4e7c: add      r6, r6, #1
004f4e80: blo      #0x4f4e4c
004f4e84: add      r6, r4, #0x1b0
004f4e88: mov      r0, r5
004f4e8c: mov      r1, r6
004f4e90: bl       #0x459090
004f4e94: mov      r3, #1
004f4e98: cmp      r3, #0
004f4e9c: str      r3, [sp, #4]
004f4ea0: bne      #0x4f4ee4
004f4ea4: add      r3, r6, #2
004f4ea8: add      r6, r6, #1
004f4eac: ldrb     r1, [r3, #1]
004f4eb0: ldrb     r2, [r6, #-1]
004f4eb4: cmp      r6, r3
004f4eb8: eor      r2, r1, r2
004f4ebc: strb     r2, [r6, #-1]
004f4ec0: ldrb     r1, [r3, #1]
004f4ec4: eor      r2, r2, r1
004f4ec8: strb     r2, [r3, #1]
004f4ecc: ldrb     r1, [r6, #-1]
004f4ed0: sub      r3, r3, #1
004f4ed4: eor      r2, r2, r1
004f4ed8: strb     r2, [r6, #-1]
004f4edc: add      r6, r6, #1
004f4ee0: blo      #0x4f4eac
004f4ee4: add      r6, r4, #0x1b4
004f4ee8: mov      r0, r5
004f4eec: mov      r1, r6
004f4ef0: bl       #0x459090
004f4ef4: mov      r3, #1
004f4ef8: cmp      r3, #0
004f4efc: str      r3, [sp, #4]
004f4f00: bne      #0x4f4f44
004f4f04: add      r3, r6, #2
004f4f08: add      r6, r6, #1
004f4f0c: ldrb     r1, [r3, #1]
004f4f10: ldrb     r2, [r6, #-1]
004f4f14: cmp      r6, r3
004f4f18: eor      r2, r1, r2
004f4f1c: strb     r2, [r6, #-1]
004f4f20: ldrb     r1, [r3, #1]
004f4f24: eor      r2, r2, r1
004f4f28: strb     r2, [r3, #1]
004f4f2c: ldrb     r1, [r6, #-1]
004f4f30: sub      r3, r3, #1
004f4f34: eor      r2, r2, r1
004f4f38: strb     r2, [r6, #-1]
004f4f3c: add      r6, r6, #1
004f4f40: blo      #0x4f4f0c
004f4f44: add      r6, r4, #0x1b8
004f4f48: mov      r0, r5
004f4f4c: mov      r1, r6
004f4f50: bl       #0x459090
004f4f54: mov      r3, #1
004f4f58: cmp      r3, #0
004f4f5c: str      r3, [sp, #4]
004f4f60: bne      #0x4f4fa4
004f4f64: add      r3, r6, #2
004f4f68: add      r6, r6, #1
004f4f6c: ldrb     r1, [r3, #1]
004f4f70: ldrb     r2, [r6, #-1]
004f4f74: cmp      r6, r3
004f4f78: eor      r2, r1, r2
004f4f7c: strb     r2, [r6, #-1]
004f4f80: ldrb     r1, [r3, #1]
004f4f84: eor      r2, r2, r1
004f4f88: strb     r2, [r3, #1]
004f4f8c: ldrb     r1, [r6, #-1]
004f4f90: sub      r3, r3, #1
004f4f94: eor      r2, r2, r1
004f4f98: strb     r2, [r6, #-1]
004f4f9c: add      r6, r6, #1
004f4fa0: blo      #0x4f4f6c
004f4fa4: add      r6, r4, #0x1bc
004f4fa8: mov      r0, r5
004f4fac: mov      r1, r6
004f4fb0: bl       #0x459090
004f4fb4: mov      r3, #1
004f4fb8: cmp      r3, #0
004f4fbc: str      r3, [sp, #4]
004f4fc0: bne      #0x4f5004
004f4fc4: add      r3, r6, #2
004f4fc8: add      r6, r6, #1
004f4fcc: ldrb     r1, [r3, #1]
004f4fd0: ldrb     r2, [r6, #-1]
004f4fd4: cmp      r6, r3
004f4fd8: eor      r2, r1, r2
004f4fdc: strb     r2, [r6, #-1]
004f4fe0: ldrb     r1, [r3, #1]
004f4fe4: eor      r2, r2, r1
004f4fe8: strb     r2, [r3, #1]
004f4fec: ldrb     r1, [r6, #-1]
004f4ff0: sub      r3, r3, #1
004f4ff4: eor      r2, r2, r1
004f4ff8: strb     r2, [r6, #-1]
004f4ffc: add      r6, r6, #1
004f5000: blo      #0x4f4fcc
004f5004: add      r6, r4, #0x1c0
004f5008: mov      r0, r5
004f500c: mov      r1, r6
004f5010: bl       #0x459090
004f5014: mov      r3, #1
004f5018: cmp      r3, #0
004f501c: str      r3, [sp, #4]
004f5020: bne      #0x4f5064
004f5024: add      r3, r6, #2
004f5028: add      r6, r6, #1
004f502c: ldrb     r1, [r3, #1]
004f5030: ldrb     r2, [r6, #-1]
004f5034: cmp      r6, r3
004f5038: eor      r2, r1, r2
004f503c: strb     r2, [r6, #-1]
004f5040: ldrb     r1, [r3, #1]
004f5044: eor      r2, r2, r1
004f5048: strb     r2, [r3, #1]
004f504c: ldrb     r1, [r6, #-1]
004f5050: sub      r3, r3, #1
004f5054: eor      r2, r2, r1
004f5058: strb     r2, [r6, #-1]
004f505c: add      r6, r6, #1
004f5060: blo      #0x4f502c
004f5064: add      r6, r4, #0x1c4
004f5068: mov      r0, r5
004f506c: mov      r1, r6
004f5070: bl       #0x459090
004f5074: mov      r3, #1
004f5078: cmp      r3, #0
004f507c: str      r3, [sp, #4]
004f5080: bne      #0x4f50c4
004f5084: add      r3, r6, #2
004f5088: add      r6, r6, #1
004f508c: ldrb     r1, [r3, #1]
004f5090: ldrb     r2, [r6, #-1]
004f5094: cmp      r6, r3
004f5098: eor      r2, r1, r2
004f509c: strb     r2, [r6, #-1]
004f50a0: ldrb     r1, [r3, #1]
004f50a4: eor      r2, r2, r1
004f50a8: strb     r2, [r3, #1]
004f50ac: ldrb     r1, [r6, #-1]
004f50b0: sub      r3, r3, #1
004f50b4: eor      r2, r2, r1
004f50b8: strb     r2, [r6, #-1]
004f50bc: add      r6, r6, #1
004f50c0: blo      #0x4f508c
004f50c4: add      r6, r4, #0x1c8
004f50c8: mov      r0, r5
004f50cc: mov      r1, r6
004f50d0: bl       #0x459090
004f50d4: mov      r3, #1
004f50d8: cmp      r3, #0
004f50dc: str      r3, [sp, #4]
004f50e0: bne      #0x4f5124
004f50e4: add      r3, r6, #2
004f50e8: add      r6, r6, #1
004f50ec: ldrb     r1, [r3, #1]
004f50f0: ldrb     r2, [r6, #-1]
004f50f4: cmp      r6, r3
004f50f8: eor      r2, r1, r2
004f50fc: strb     r2, [r6, #-1]
004f5100: ldrb     r1, [r3, #1]
004f5104: eor      r2, r2, r1
004f5108: strb     r2, [r3, #1]
004f510c: ldrb     r1, [r6, #-1]
004f5110: sub      r3, r3, #1
004f5114: eor      r2, r2, r1
004f5118: strb     r2, [r6, #-1]
004f511c: add      r6, r6, #1
004f5120: blo      #0x4f50ec
004f5124: add      r6, r4, #0x1cc
004f5128: mov      r0, r5
004f512c: mov      r1, r6
004f5130: bl       #0x459090
004f5134: mov      r3, #1
004f5138: cmp      r3, #0
004f513c: str      r3, [sp, #4]
004f5140: bne      #0x4f5184
004f5144: add      r3, r6, #2
004f5148: add      r6, r6, #1
004f514c: ldrb     r1, [r3, #1]
004f5150: ldrb     r2, [r6, #-1]
004f5154: cmp      r6, r3
004f5158: eor      r2, r1, r2
004f515c: strb     r2, [r6, #-1]
004f5160: ldrb     r1, [r3, #1]
004f5164: eor      r2, r2, r1
004f5168: strb     r2, [r3, #1]
004f516c: ldrb     r1, [r6, #-1]
004f5170: sub      r3, r3, #1
004f5174: eor      r2, r2, r1
004f5178: strb     r2, [r6, #-1]
004f517c: add      r6, r6, #1
004f5180: blo      #0x4f514c
004f5184: add      r6, r4, #0x1d0
004f5188: mov      r0, r5
004f518c: mov      r1, r6
004f5190: bl       #0x459090
004f5194: mov      r3, #1
004f5198: cmp      r3, #0
004f519c: str      r3, [sp, #4]
004f51a0: bne      #0x4f51e4
004f51a4: add      r3, r6, #2
004f51a8: add      r6, r6, #1
004f51ac: ldrb     r1, [r3, #1]
004f51b0: ldrb     r2, [r6, #-1]
004f51b4: cmp      r6, r3
004f51b8: eor      r2, r1, r2
004f51bc: strb     r2, [r6, #-1]
004f51c0: ldrb     r1, [r3, #1]
004f51c4: eor      r2, r2, r1
004f51c8: strb     r2, [r3, #1]
004f51cc: ldrb     r1, [r6, #-1]
004f51d0: sub      r3, r3, #1
004f51d4: eor      r2, r2, r1
004f51d8: strb     r2, [r6, #-1]
004f51dc: add      r6, r6, #1
004f51e0: blo      #0x4f51ac
004f51e4: add      r6, r4, #0x1d4
004f51e8: mov      r0, r5
004f51ec: mov      r1, r6
004f51f0: bl       #0x459090
004f51f4: mov      r3, #1
004f51f8: cmp      r3, #0
004f51fc: str      r3, [sp, #4]
004f5200: bne      #0x4f5244
004f5204: add      r3, r6, #2
004f5208: add      r6, r6, #1
004f520c: ldrb     r1, [r3, #1]
004f5210: ldrb     r2, [r6, #-1]
004f5214: cmp      r6, r3
004f5218: eor      r2, r1, r2
004f521c: strb     r2, [r6, #-1]
004f5220: ldrb     r1, [r3, #1]
004f5224: eor      r2, r2, r1
004f5228: strb     r2, [r3, #1]
004f522c: ldrb     r1, [r6, #-1]
004f5230: sub      r3, r3, #1
004f5234: eor      r2, r2, r1
004f5238: strb     r2, [r6, #-1]
004f523c: add      r6, r6, #1
004f5240: blo      #0x4f520c
004f5244: add      r6, r4, #0x1d8
004f5248: mov      r0, r5
004f524c: mov      r1, r6
004f5250: bl       #0x459090
004f5254: mov      r3, #1
004f5258: cmp      r3, #0
004f525c: str      r3, [sp, #4]
004f5260: bne      #0x4f52a4
004f5264: add      r3, r6, #2
004f5268: add      r6, r6, #1
004f526c: ldrb     r1, [r3, #1]
004f5270: ldrb     r2, [r6, #-1]
004f5274: cmp      r6, r3
004f5278: eor      r2, r1, r2
004f527c: strb     r2, [r6, #-1]
004f5280: ldrb     r1, [r3, #1]
004f5284: eor      r2, r2, r1
004f5288: strb     r2, [r3, #1]
004f528c: ldrb     r1, [r6, #-1]
004f5290: sub      r3, r3, #1
004f5294: eor      r2, r2, r1
004f5298: strb     r2, [r6, #-1]
004f529c: add      r6, r6, #1
004f52a0: blo      #0x4f526c
004f52a4: add      r6, r4, #0x1dc
004f52a8: mov      r0, r5
004f52ac: mov      r1, r6
004f52b0: bl       #0x459090
004f52b4: mov      r3, #1
004f52b8: cmp      r3, #0
004f52bc: str      r3, [sp, #4]
004f52c0: bne      #0x4f5304
004f52c4: add      r3, r6, #2
004f52c8: add      r6, r6, #1
004f52cc: ldrb     r1, [r3, #1]
004f52d0: ldrb     r2, [r6, #-1]
004f52d4: cmp      r6, r3
004f52d8: eor      r2, r1, r2
004f52dc: strb     r2, [r6, #-1]
004f52e0: ldrb     r1, [r3, #1]
004f52e4: eor      r2, r2, r1
004f52e8: strb     r2, [r3, #1]
004f52ec: ldrb     r1, [r6, #-1]
004f52f0: sub      r3, r3, #1
004f52f4: eor      r2, r2, r1
004f52f8: strb     r2, [r6, #-1]
004f52fc: add      r6, r6, #1
004f5300: blo      #0x4f52cc
004f5304: add      r6, r4, #0x1e0
004f5308: mov      r0, r5
004f530c: mov      r1, r6
004f5310: bl       #0x459090
004f5314: mov      r3, #1
004f5318: cmp      r3, #0
004f531c: str      r3, [sp, #4]
004f5320: bne      #0x4f5364
004f5324: add      r3, r6, #2
004f5328: add      r6, r6, #1
004f532c: ldrb     r1, [r3, #1]
004f5330: ldrb     r2, [r6, #-1]
004f5334: cmp      r6, r3
004f5338: eor      r2, r1, r2
004f533c: strb     r2, [r6, #-1]
004f5340: ldrb     r1, [r3, #1]
004f5344: eor      r2, r2, r1
004f5348: strb     r2, [r3, #1]
004f534c: ldrb     r1, [r6, #-1]
004f5350: sub      r3, r3, #1
004f5354: eor      r2, r2, r1
004f5358: strb     r2, [r6, #-1]
004f535c: add      r6, r6, #1
004f5360: blo      #0x4f532c
004f5364: add      r6, r4, #0x1e4
004f5368: mov      r0, r5
004f536c: mov      r1, r6
004f5370: bl       #0x459090
004f5374: mov      r3, #1
004f5378: cmp      r3, #0
004f537c: str      r3, [sp, #4]
004f5380: bne      #0x4f53c4
004f5384: add      r3, r6, #2
004f5388: add      r6, r6, #1
004f538c: ldrb     r1, [r3, #1]
004f5390: ldrb     r2, [r6, #-1]
004f5394: cmp      r6, r3
004f5398: eor      r2, r1, r2
004f539c: strb     r2, [r6, #-1]
004f53a0: ldrb     r1, [r3, #1]
004f53a4: eor      r2, r2, r1
004f53a8: strb     r2, [r3, #1]
004f53ac: ldrb     r1, [r6, #-1]
004f53b0: sub      r3, r3, #1
004f53b4: eor      r2, r2, r1
004f53b8: strb     r2, [r6, #-1]
004f53bc: add      r6, r6, #1
004f53c0: blo      #0x4f538c
004f53c4: add      r6, r4, #0x1e8
004f53c8: mov      r0, r5
004f53cc: mov      r1, r6
004f53d0: bl       #0x459090
004f53d4: mov      r3, #1
004f53d8: cmp      r3, #0
004f53dc: str      r3, [sp, #4]
004f53e0: bne      #0x4f5424
004f53e4: add      r3, r6, #2
004f53e8: add      r6, r6, #1
004f53ec: ldrb     r1, [r3, #1]
004f53f0: ldrb     r2, [r6, #-1]
004f53f4: cmp      r6, r3
004f53f8: eor      r2, r1, r2
004f53fc: strb     r2, [r6, #-1]
004f5400: ldrb     r1, [r3, #1]
004f5404: eor      r2, r2, r1
004f5408: strb     r2, [r3, #1]
004f540c: ldrb     r1, [r6, #-1]
004f5410: sub      r3, r3, #1
004f5414: eor      r2, r2, r1
004f5418: strb     r2, [r6, #-1]
004f541c: add      r6, r6, #1
004f5420: blo      #0x4f53ec
004f5424: add      r6, r4, #0x1ec
004f5428: mov      r0, r5
004f542c: mov      r1, r6
004f5430: bl       #0x459090
004f5434: mov      r3, #1
004f5438: cmp      r3, #0
004f543c: str      r3, [sp, #4]
004f5440: bne      #0x4f5484
004f5444: add      r3, r6, #2
004f5448: add      r6, r6, #1
004f544c: ldrb     r1, [r3, #1]
004f5450: ldrb     r2, [r6, #-1]
004f5454: cmp      r6, r3
004f5458: eor      r2, r1, r2
004f545c: strb     r2, [r6, #-1]
004f5460: ldrb     r1, [r3, #1]
004f5464: eor      r2, r2, r1
004f5468: strb     r2, [r3, #1]
004f546c: ldrb     r1, [r6, #-1]
004f5470: sub      r3, r3, #1
004f5474: eor      r2, r2, r1
004f5478: strb     r2, [r6, #-1]
004f547c: add      r6, r6, #1
004f5480: blo      #0x4f544c
004f5484: add      r6, r4, #0x1f0
004f5488: mov      r0, r5
004f548c: mov      r1, r6
004f5490: bl       #0x459090
004f5494: mov      r3, #1
004f5498: cmp      r3, #0
004f549c: str      r3, [sp, #4]
004f54a0: bne      #0x4f54e4
004f54a4: add      r3, r6, #2
004f54a8: add      r6, r6, #1
004f54ac: ldrb     r1, [r3, #1]
004f54b0: ldrb     r2, [r6, #-1]
004f54b4: cmp      r6, r3
004f54b8: eor      r2, r1, r2
004f54bc: strb     r2, [r6, #-1]
004f54c0: ldrb     r1, [r3, #1]
004f54c4: eor      r2, r2, r1
004f54c8: strb     r2, [r3, #1]
004f54cc: ldrb     r1, [r6, #-1]
004f54d0: sub      r3, r3, #1
004f54d4: eor      r2, r2, r1
004f54d8: strb     r2, [r6, #-1]
004f54dc: add      r6, r6, #1
004f54e0: blo      #0x4f54ac
004f54e4: add      r6, r4, #0x1f4
004f54e8: mov      r0, r5
004f54ec: mov      r1, r6
004f54f0: bl       #0x459090
004f54f4: mov      r3, #1
004f54f8: cmp      r3, #0
004f54fc: str      r3, [sp, #4]
004f5500: bne      #0x4f5544
004f5504: add      r3, r6, #2
004f5508: add      r6, r6, #1
004f550c: ldrb     r1, [r3, #1]
004f5510: ldrb     r2, [r6, #-1]
004f5514: cmp      r6, r3
004f5518: eor      r2, r1, r2
004f551c: strb     r2, [r6, #-1]
004f5520: ldrb     r1, [r3, #1]
004f5524: eor      r2, r2, r1
004f5528: strb     r2, [r3, #1]
004f552c: ldrb     r1, [r6, #-1]
004f5530: sub      r3, r3, #1
004f5534: eor      r2, r2, r1
004f5538: strb     r2, [r6, #-1]
004f553c: add      r6, r6, #1
004f5540: blo      #0x4f550c
004f5544: add      r6, r4, #0x1f8
004f5548: mov      r0, r5
004f554c: mov      r1, r6
004f5550: bl       #0x459090
004f5554: mov      r3, #1
004f5558: cmp      r3, #0
004f555c: str      r3, [sp, #4]
004f5560: bne      #0x4f55a4
004f5564: add      r3, r6, #2
004f5568: add      r6, r6, #1
004f556c: ldrb     r1, [r3, #1]
004f5570: ldrb     r2, [r6, #-1]
004f5574: cmp      r6, r3
004f5578: eor      r2, r1, r2
004f557c: strb     r2, [r6, #-1]
004f5580: ldrb     r1, [r3, #1]
004f5584: eor      r2, r2, r1
004f5588: strb     r2, [r3, #1]
004f558c: ldrb     r1, [r6, #-1]
004f5590: sub      r3, r3, #1
004f5594: eor      r2, r2, r1
004f5598: strb     r2, [r6, #-1]
004f559c: add      r6, r6, #1
004f55a0: blo      #0x4f556c
004f55a4: add      r6, r4, #0x1fc
004f55a8: mov      r0, r5
004f55ac: mov      r1, r6
004f55b0: bl       #0x459090
004f55b4: mov      r3, #1
004f55b8: cmp      r3, #0
004f55bc: str      r3, [sp, #4]
004f55c0: bne      #0x4f5604
004f55c4: add      r3, r6, #2
004f55c8: add      r6, r6, #1
004f55cc: ldrb     r1, [r3, #1]
004f55d0: ldrb     r2, [r6, #-1]
004f55d4: cmp      r6, r3
004f55d8: eor      r2, r1, r2
004f55dc: strb     r2, [r6, #-1]
004f55e0: ldrb     r1, [r3, #1]
004f55e4: eor      r2, r2, r1
004f55e8: strb     r2, [r3, #1]
004f55ec: ldrb     r1, [r6, #-1]
004f55f0: sub      r3, r3, #1
004f55f4: eor      r2, r2, r1
004f55f8: strb     r2, [r6, #-1]
004f55fc: add      r6, r6, #1
004f5600: blo      #0x4f55cc
004f5604: add      r6, r4, #0x200
004f5608: mov      r0, r5
004f560c: mov      r1, r6
004f5610: bl       #0x459090
004f5614: mov      r3, #1
004f5618: cmp      r3, #0
004f561c: str      r3, [sp, #4]
004f5620: bne      #0x4f5664
004f5624: add      r3, r6, #2
004f5628: add      r6, r6, #1
004f562c: ldrb     r1, [r3, #1]
004f5630: ldrb     r2, [r6, #-1]
004f5634: cmp      r6, r3
004f5638: eor      r2, r1, r2
004f563c: strb     r2, [r6, #-1]
004f5640: ldrb     r1, [r3, #1]
004f5644: eor      r2, r2, r1
004f5648: strb     r2, [r3, #1]
004f564c: ldrb     r1, [r6, #-1]
004f5650: sub      r3, r3, #1
004f5654: eor      r2, r2, r1
004f5658: strb     r2, [r6, #-1]
004f565c: add      r6, r6, #1
004f5660: blo      #0x4f562c
004f5664: add      r6, r4, #0x204
004f5668: mov      r0, r5
004f566c: mov      r1, r6
004f5670: bl       #0x459090
004f5674: mov      r3, #1
004f5678: cmp      r3, #0
004f567c: str      r3, [sp, #4]
004f5680: bne      #0x4f56c4
004f5684: add      r3, r6, #2
004f5688: add      r6, r6, #1
004f568c: ldrb     r1, [r3, #1]
004f5690: ldrb     r2, [r6, #-1]
004f5694: cmp      r6, r3
004f5698: eor      r2, r1, r2
004f569c: strb     r2, [r6, #-1]
004f56a0: ldrb     r1, [r3, #1]
004f56a4: eor      r2, r2, r1
004f56a8: strb     r2, [r3, #1]
004f56ac: ldrb     r1, [r6, #-1]
004f56b0: sub      r3, r3, #1
004f56b4: eor      r2, r2, r1
004f56b8: strb     r2, [r6, #-1]
004f56bc: add      r6, r6, #1
004f56c0: blo      #0x4f568c
004f56c4: add      r6, r4, #0x208
004f56c8: mov      r0, r5
004f56cc: mov      r1, r6
004f56d0: bl       #0x459090
004f56d4: mov      r3, #1
004f56d8: cmp      r3, #0
004f56dc: str      r3, [sp, #4]
004f56e0: bne      #0x4f5724
004f56e4: add      r3, r6, #2
004f56e8: add      r6, r6, #1
004f56ec: ldrb     r1, [r3, #1]
004f56f0: ldrb     r2, [r6, #-1]
004f56f4: cmp      r6, r3
004f56f8: eor      r2, r1, r2
004f56fc: strb     r2, [r6, #-1]
004f5700: ldrb     r1, [r3, #1]
004f5704: eor      r2, r2, r1
004f5708: strb     r2, [r3, #1]
004f570c: ldrb     r1, [r6, #-1]
004f5710: sub      r3, r3, #1
004f5714: eor      r2, r2, r1
004f5718: strb     r2, [r6, #-1]
004f571c: add      r6, r6, #1
004f5720: blo      #0x4f56ec
004f5724: add      r6, r4, #0x20c
004f5728: mov      r0, r5
004f572c: mov      r1, r6
004f5730: bl       #0x459090
004f5734: mov      r3, #1
004f5738: cmp      r3, #0
004f573c: str      r3, [sp, #4]
004f5740: bne      #0x4f5784
004f5744: add      r3, r6, #2
004f5748: add      r6, r6, #1
004f574c: ldrb     r1, [r3, #1]
004f5750: ldrb     r2, [r6, #-1]
004f5754: cmp      r6, r3
004f5758: eor      r2, r1, r2
004f575c: strb     r2, [r6, #-1]
004f5760: ldrb     r1, [r3, #1]
004f5764: eor      r2, r2, r1
004f5768: strb     r2, [r3, #1]
004f576c: ldrb     r1, [r6, #-1]
004f5770: sub      r3, r3, #1
004f5774: eor      r2, r2, r1
004f5778: strb     r2, [r6, #-1]
004f577c: add      r6, r6, #1
004f5780: blo      #0x4f574c
004f5784: add      r6, r4, #0x210
004f5788: mov      r0, r5
004f578c: mov      r1, r6
004f5790: bl       #0x459090
004f5794: mov      r3, #1
004f5798: cmp      r3, #0
004f579c: str      r3, [sp, #4]
004f57a0: bne      #0x4f57e4
004f57a4: add      r3, r6, #2
004f57a8: add      r6, r6, #1
004f57ac: ldrb     r1, [r3, #1]
004f57b0: ldrb     r2, [r6, #-1]
004f57b4: cmp      r6, r3
004f57b8: eor      r2, r1, r2
004f57bc: strb     r2, [r6, #-1]
004f57c0: ldrb     r1, [r3, #1]
004f57c4: eor      r2, r2, r1
004f57c8: strb     r2, [r3, #1]
004f57cc: ldrb     r1, [r6, #-1]
004f57d0: sub      r3, r3, #1
004f57d4: eor      r2, r2, r1
004f57d8: strb     r2, [r6, #-1]
004f57dc: add      r6, r6, #1
004f57e0: blo      #0x4f57ac
004f57e4: add      r6, r4, #0x214
004f57e8: mov      r0, r5
004f57ec: mov      r1, r6
004f57f0: bl       #0x459090
004f57f4: mov      r3, #1
004f57f8: cmp      r3, #0
004f57fc: str      r3, [sp, #4]
004f5800: bne      #0x4f5844
004f5804: add      r3, r6, #2
004f5808: add      r6, r6, #1
004f580c: ldrb     r1, [r3, #1]
004f5810: ldrb     r2, [r6, #-1]
004f5814: cmp      r6, r3
004f5818: eor      r2, r1, r2
004f581c: strb     r2, [r6, #-1]
004f5820: ldrb     r1, [r3, #1]
004f5824: eor      r2, r2, r1
004f5828: strb     r2, [r3, #1]
004f582c: ldrb     r1, [r6, #-1]
004f5830: sub      r3, r3, #1
004f5834: eor      r2, r2, r1
004f5838: strb     r2, [r6, #-1]
004f583c: add      r6, r6, #1
004f5840: blo      #0x4f580c
004f5844: add      r6, r4, #0x218
004f5848: mov      r0, r5
004f584c: mov      r1, r6
004f5850: bl       #0x459090
004f5854: mov      r3, #1
004f5858: cmp      r3, #0
004f585c: str      r3, [sp, #4]
004f5860: bne      #0x4f58a4
004f5864: add      r3, r6, #2
004f5868: add      r6, r6, #1
004f586c: ldrb     r1, [r3, #1]
004f5870: ldrb     r2, [r6, #-1]
004f5874: cmp      r6, r3
004f5878: eor      r2, r1, r2
004f587c: strb     r2, [r6, #-1]
004f5880: ldrb     r1, [r3, #1]
004f5884: eor      r2, r2, r1
004f5888: strb     r2, [r3, #1]
004f588c: ldrb     r1, [r6, #-1]
004f5890: sub      r3, r3, #1
004f5894: eor      r2, r2, r1
004f5898: strb     r2, [r6, #-1]
004f589c: add      r6, r6, #1
004f58a0: blo      #0x4f586c
004f58a4: add      r6, r4, #0x21c
004f58a8: mov      r0, r5
004f58ac: mov      r1, r6
004f58b0: bl       #0x459090
004f58b4: mov      r3, #1
004f58b8: cmp      r3, #0
004f58bc: str      r3, [sp, #4]
004f58c0: bne      #0x4f5904
004f58c4: add      r3, r6, #2
004f58c8: add      r6, r6, #1
004f58cc: ldrb     r1, [r3, #1]
004f58d0: ldrb     r2, [r6, #-1]
004f58d4: cmp      r6, r3
004f58d8: eor      r2, r1, r2
004f58dc: strb     r2, [r6, #-1]
004f58e0: ldrb     r1, [r3, #1]
004f58e4: eor      r2, r2, r1
004f58e8: strb     r2, [r3, #1]
004f58ec: ldrb     r1, [r6, #-1]
004f58f0: sub      r3, r3, #1
004f58f4: eor      r2, r2, r1
004f58f8: strb     r2, [r6, #-1]
004f58fc: add      r6, r6, #1
004f5900: blo      #0x4f58cc
004f5904: add      r6, r4, #0x220
004f5908: mov      r0, r5
004f590c: mov      r1, r6
004f5910: bl       #0x459090
004f5914: mov      r3, #1
004f5918: cmp      r3, #0
004f591c: str      r3, [sp, #4]
004f5920: bne      #0x4f5964
004f5924: add      r3, r6, #2
004f5928: add      r6, r6, #1
004f592c: ldrb     r1, [r3, #1]
004f5930: ldrb     r2, [r6, #-1]
004f5934: cmp      r6, r3
004f5938: eor      r2, r1, r2
004f593c: strb     r2, [r6, #-1]
004f5940: ldrb     r1, [r3, #1]
004f5944: eor      r2, r2, r1
004f5948: strb     r2, [r3, #1]
004f594c: ldrb     r1, [r6, #-1]
004f5950: sub      r3, r3, #1
004f5954: eor      r2, r2, r1
004f5958: strb     r2, [r6, #-1]
004f595c: add      r6, r6, #1
004f5960: blo      #0x4f592c
004f5964: add      r6, r4, #0x224
004f5968: mov      r0, r5
004f596c: mov      r1, r6
004f5970: bl       #0x459090
004f5974: mov      r3, #1
004f5978: cmp      r3, #0
004f597c: str      r3, [sp, #4]
004f5980: bne      #0x4f59c4
004f5984: add      r3, r6, #2
004f5988: add      r6, r6, #1
004f598c: ldrb     r1, [r3, #1]
004f5990: ldrb     r2, [r6, #-1]
004f5994: cmp      r6, r3
004f5998: eor      r2, r1, r2
004f599c: strb     r2, [r6, #-1]
004f59a0: ldrb     r1, [r3, #1]
004f59a4: eor      r2, r2, r1
004f59a8: strb     r2, [r3, #1]
004f59ac: ldrb     r1, [r6, #-1]
004f59b0: sub      r3, r3, #1
004f59b4: eor      r2, r2, r1
004f59b8: strb     r2, [r6, #-1]
004f59bc: add      r6, r6, #1
004f59c0: blo      #0x4f598c
004f59c4: add      r6, r4, #0x228
004f59c8: mov      r0, r5
004f59cc: mov      r1, r6
004f59d0: bl       #0x459090
004f59d4: mov      r3, #1
004f59d8: cmp      r3, #0
004f59dc: str      r3, [sp, #4]
004f59e0: bne      #0x4f5a24
004f59e4: add      r3, r6, #2
004f59e8: add      r6, r6, #1
004f59ec: ldrb     r1, [r3, #1]
004f59f0: ldrb     r2, [r6, #-1]
004f59f4: cmp      r6, r3
004f59f8: eor      r2, r1, r2
004f59fc: strb     r2, [r6, #-1]
004f5a00: ldrb     r1, [r3, #1]
004f5a04: eor      r2, r2, r1
004f5a08: strb     r2, [r3, #1]
004f5a0c: ldrb     r1, [r6, #-1]
004f5a10: sub      r3, r3, #1
004f5a14: eor      r2, r2, r1
004f5a18: strb     r2, [r6, #-1]
004f5a1c: add      r6, r6, #1
004f5a20: blo      #0x4f59ec
004f5a24: add      r6, r4, #0x22c
004f5a28: mov      r0, r5
004f5a2c: mov      r1, r6
004f5a30: bl       #0x459090
004f5a34: mov      r3, #1
004f5a38: cmp      r3, #0
004f5a3c: str      r3, [sp, #4]
004f5a40: bne      #0x4f5a84
004f5a44: add      r3, r6, #2
004f5a48: add      r6, r6, #1
004f5a4c: ldrb     r1, [r3, #1]
004f5a50: ldrb     r2, [r6, #-1]
004f5a54: cmp      r6, r3
004f5a58: eor      r2, r1, r2
004f5a5c: strb     r2, [r6, #-1]
004f5a60: ldrb     r1, [r3, #1]
004f5a64: eor      r2, r2, r1
004f5a68: strb     r2, [r3, #1]
004f5a6c: ldrb     r1, [r6, #-1]
004f5a70: sub      r3, r3, #1
004f5a74: eor      r2, r2, r1
004f5a78: strb     r2, [r6, #-1]
004f5a7c: add      r6, r6, #1
004f5a80: blo      #0x4f5a4c
004f5a84: add      r6, r4, #0x230
004f5a88: mov      r0, r5
004f5a8c: mov      r1, r6
004f5a90: bl       #0x459090
004f5a94: mov      r3, #1
004f5a98: cmp      r3, #0
004f5a9c: str      r3, [sp, #4]
004f5aa0: bne      #0x4f5ae4
004f5aa4: add      r3, r6, #2
004f5aa8: add      r6, r6, #1
004f5aac: ldrb     r1, [r3, #1]
004f5ab0: ldrb     r2, [r6, #-1]
004f5ab4: cmp      r6, r3
004f5ab8: eor      r2, r1, r2
004f5abc: strb     r2, [r6, #-1]
004f5ac0: ldrb     r1, [r3, #1]
004f5ac4: eor      r2, r2, r1
004f5ac8: strb     r2, [r3, #1]
004f5acc: ldrb     r1, [r6, #-1]
004f5ad0: sub      r3, r3, #1
004f5ad4: eor      r2, r2, r1
004f5ad8: strb     r2, [r6, #-1]
004f5adc: add      r6, r6, #1
004f5ae0: blo      #0x4f5aac
004f5ae4: add      r6, r4, #0x234
004f5ae8: mov      r0, r5
004f5aec: mov      r1, r6
004f5af0: bl       #0x459090
004f5af4: mov      r3, #1
004f5af8: cmp      r3, #0
004f5afc: str      r3, [sp, #4]
004f5b00: bne      #0x4f5b44
004f5b04: add      r3, r6, #2
004f5b08: add      r6, r6, #1
004f5b0c: ldrb     r1, [r3, #1]
004f5b10: ldrb     r2, [r6, #-1]
004f5b14: cmp      r6, r3
004f5b18: eor      r2, r1, r2
004f5b1c: strb     r2, [r6, #-1]
004f5b20: ldrb     r1, [r3, #1]
004f5b24: eor      r2, r2, r1
004f5b28: strb     r2, [r3, #1]
004f5b2c: ldrb     r1, [r6, #-1]
004f5b30: sub      r3, r3, #1
004f5b34: eor      r2, r2, r1
004f5b38: strb     r2, [r6, #-1]
004f5b3c: add      r6, r6, #1
004f5b40: blo      #0x4f5b0c
004f5b44: add      r6, r4, #0x238
004f5b48: mov      r0, r5
004f5b4c: mov      r1, r6
004f5b50: bl       #0x459090
004f5b54: mov      r3, #1
004f5b58: cmp      r3, #0
004f5b5c: str      r3, [sp, #4]
004f5b60: bne      #0x4f5ba4
004f5b64: add      r3, r6, #2
004f5b68: add      r6, r6, #1
004f5b6c: ldrb     r1, [r3, #1]
004f5b70: ldrb     r2, [r6, #-1]
004f5b74: cmp      r6, r3
004f5b78: eor      r2, r1, r2
004f5b7c: strb     r2, [r6, #-1]
004f5b80: ldrb     r1, [r3, #1]
004f5b84: eor      r2, r2, r1
004f5b88: strb     r2, [r3, #1]
004f5b8c: ldrb     r1, [r6, #-1]
004f5b90: sub      r3, r3, #1
004f5b94: eor      r2, r2, r1
004f5b98: strb     r2, [r6, #-1]
004f5b9c: add      r6, r6, #1
004f5ba0: blo      #0x4f5b6c
004f5ba4: add      r6, r4, #0x23c
004f5ba8: mov      r0, r5
004f5bac: mov      r1, r6
004f5bb0: bl       #0x459090
004f5bb4: mov      r3, #1
004f5bb8: cmp      r3, #0
004f5bbc: str      r3, [sp, #4]
004f5bc0: bne      #0x4f5c04
004f5bc4: add      r3, r6, #2
004f5bc8: add      r6, r6, #1
004f5bcc: ldrb     r1, [r3, #1]
004f5bd0: ldrb     r2, [r6, #-1]
004f5bd4: cmp      r6, r3
004f5bd8: eor      r2, r1, r2
004f5bdc: strb     r2, [r6, #-1]
004f5be0: ldrb     r1, [r3, #1]
004f5be4: eor      r2, r2, r1
004f5be8: strb     r2, [r3, #1]
004f5bec: ldrb     r1, [r6, #-1]
004f5bf0: sub      r3, r3, #1
004f5bf4: eor      r2, r2, r1
004f5bf8: strb     r2, [r6, #-1]
004f5bfc: add      r6, r6, #1
004f5c00: blo      #0x4f5bcc
004f5c04: add      r6, r4, #0x240
004f5c08: mov      r0, r5
004f5c0c: mov      r1, r6
004f5c10: bl       #0x459090
004f5c14: mov      r3, #1
004f5c18: cmp      r3, #0
004f5c1c: str      r3, [sp, #4]
004f5c20: bne      #0x4f5c64
004f5c24: add      r3, r6, #2
004f5c28: add      r6, r6, #1
004f5c2c: ldrb     r1, [r3, #1]
004f5c30: ldrb     r2, [r6, #-1]
004f5c34: cmp      r6, r3
004f5c38: eor      r2, r1, r2
004f5c3c: strb     r2, [r6, #-1]
004f5c40: ldrb     r1, [r3, #1]
004f5c44: eor      r2, r2, r1
004f5c48: strb     r2, [r3, #1]
004f5c4c: ldrb     r1, [r6, #-1]
004f5c50: sub      r3, r3, #1
004f5c54: eor      r2, r2, r1
004f5c58: strb     r2, [r6, #-1]
004f5c5c: add      r6, r6, #1
004f5c60: blo      #0x4f5c2c
004f5c64: add      r6, r4, #0x244
004f5c68: mov      r0, r5
004f5c6c: mov      r1, r6
004f5c70: bl       #0x459090
004f5c74: mov      r3, #1
004f5c78: cmp      r3, #0
004f5c7c: str      r3, [sp, #4]
004f5c80: bne      #0x4f5cc4
004f5c84: add      r3, r6, #2
004f5c88: add      r6, r6, #1
004f5c8c: ldrb     r1, [r3, #1]
004f5c90: ldrb     r2, [r6, #-1]
004f5c94: cmp      r6, r3
004f5c98: eor      r2, r1, r2
004f5c9c: strb     r2, [r6, #-1]
004f5ca0: ldrb     r1, [r3, #1]
004f5ca4: eor      r2, r2, r1
004f5ca8: strb     r2, [r3, #1]
004f5cac: ldrb     r1, [r6, #-1]
004f5cb0: sub      r3, r3, #1
004f5cb4: eor      r2, r2, r1
004f5cb8: strb     r2, [r6, #-1]
004f5cbc: add      r6, r6, #1
004f5cc0: blo      #0x4f5c8c
004f5cc4: add      r6, r4, #0x248
004f5cc8: mov      r0, r5
004f5ccc: mov      r1, r6
004f5cd0: bl       #0x459090
004f5cd4: mov      r3, #1
004f5cd8: cmp      r3, #0
004f5cdc: str      r3, [sp, #4]
004f5ce0: bne      #0x4f5d24
004f5ce4: add      r3, r6, #2
004f5ce8: add      r6, r6, #1
004f5cec: ldrb     r1, [r3, #1]
004f5cf0: ldrb     r2, [r6, #-1]
004f5cf4: cmp      r6, r3
004f5cf8: eor      r2, r1, r2
004f5cfc: strb     r2, [r6, #-1]
004f5d00: ldrb     r1, [r3, #1]
004f5d04: eor      r2, r2, r1
004f5d08: strb     r2, [r3, #1]
004f5d0c: ldrb     r1, [r6, #-1]
004f5d10: sub      r3, r3, #1
004f5d14: eor      r2, r2, r1
004f5d18: strb     r2, [r6, #-1]
004f5d1c: add      r6, r6, #1
004f5d20: blo      #0x4f5cec
004f5d24: add      r6, r4, #0x24c
004f5d28: mov      r0, r5
004f5d2c: mov      r1, r6
004f5d30: bl       #0x459090
004f5d34: mov      r3, #1
004f5d38: cmp      r3, #0
004f5d3c: str      r3, [sp, #4]
004f5d40: bne      #0x4f5d84
004f5d44: add      r3, r6, #2
004f5d48: add      r6, r6, #1
004f5d4c: ldrb     r1, [r3, #1]
004f5d50: ldrb     r2, [r6, #-1]
004f5d54: cmp      r6, r3
004f5d58: eor      r2, r1, r2
004f5d5c: strb     r2, [r6, #-1]
004f5d60: ldrb     r1, [r3, #1]
004f5d64: eor      r2, r2, r1
004f5d68: strb     r2, [r3, #1]
004f5d6c: ldrb     r1, [r6, #-1]
004f5d70: sub      r3, r3, #1
004f5d74: eor      r2, r2, r1
004f5d78: strb     r2, [r6, #-1]
004f5d7c: add      r6, r6, #1
004f5d80: blo      #0x4f5d4c
004f5d84: add      r6, r4, #0x250
004f5d88: mov      r0, r5
004f5d8c: mov      r1, r6
004f5d90: bl       #0x459090
004f5d94: mov      r3, #1
004f5d98: cmp      r3, #0
004f5d9c: str      r3, [sp, #4]
004f5da0: bne      #0x4f5de4
004f5da4: add      r3, r6, #2
004f5da8: add      r6, r6, #1
004f5dac: ldrb     r1, [r3, #1]
004f5db0: ldrb     r2, [r6, #-1]
004f5db4: cmp      r6, r3
004f5db8: eor      r2, r1, r2
004f5dbc: strb     r2, [r6, #-1]
004f5dc0: ldrb     r1, [r3, #1]
004f5dc4: eor      r2, r2, r1
004f5dc8: strb     r2, [r3, #1]
004f5dcc: ldrb     r1, [r6, #-1]
004f5dd0: sub      r3, r3, #1
004f5dd4: eor      r2, r2, r1
004f5dd8: strb     r2, [r6, #-1]
004f5ddc: add      r6, r6, #1
004f5de0: blo      #0x4f5dac
004f5de4: add      r6, r4, #0x254
004f5de8: mov      r0, r5
004f5dec: mov      r1, r6
004f5df0: bl       #0x459090
004f5df4: mov      r3, #1
004f5df8: cmp      r3, #0
004f5dfc: str      r3, [sp, #4]
004f5e00: bne      #0x4f5e44
004f5e04: add      r3, r6, #2
004f5e08: add      r6, r6, #1
004f5e0c: ldrb     r1, [r3, #1]
004f5e10: ldrb     r2, [r6, #-1]
004f5e14: cmp      r6, r3
004f5e18: eor      r2, r1, r2
004f5e1c: strb     r2, [r6, #-1]
004f5e20: ldrb     r1, [r3, #1]
004f5e24: eor      r2, r2, r1
004f5e28: strb     r2, [r3, #1]
004f5e2c: ldrb     r1, [r6, #-1]
004f5e30: sub      r3, r3, #1
004f5e34: eor      r2, r2, r1
004f5e38: strb     r2, [r6, #-1]
004f5e3c: add      r6, r6, #1
004f5e40: blo      #0x4f5e0c
004f5e44: add      r6, r4, #0x258
004f5e48: mov      r0, r5
004f5e4c: mov      r1, r6
004f5e50: bl       #0x459090
004f5e54: mov      r3, #1
004f5e58: cmp      r3, #0
004f5e5c: str      r3, [sp, #4]
004f5e60: bne      #0x4f5ea4
004f5e64: add      r3, r6, #2
004f5e68: add      r6, r6, #1
004f5e6c: ldrb     r1, [r3, #1]
004f5e70: ldrb     r2, [r6, #-1]
004f5e74: cmp      r6, r3
004f5e78: eor      r2, r1, r2
004f5e7c: strb     r2, [r6, #-1]
004f5e80: ldrb     r1, [r3, #1]
004f5e84: eor      r2, r2, r1
004f5e88: strb     r2, [r3, #1]
004f5e8c: ldrb     r1, [r6, #-1]
004f5e90: sub      r3, r3, #1
004f5e94: eor      r2, r2, r1
004f5e98: strb     r2, [r6, #-1]
004f5e9c: add      r6, r6, #1
004f5ea0: blo      #0x4f5e6c
004f5ea4: add      r6, r4, #0x25c
004f5ea8: mov      r0, r5
004f5eac: mov      r1, r6
004f5eb0: bl       #0x459090
004f5eb4: mov      r3, #1
004f5eb8: cmp      r3, #0
004f5ebc: str      r3, [sp, #4]
004f5ec0: bne      #0x4f5f04
004f5ec4: add      r3, r6, #2
004f5ec8: add      r6, r6, #1
004f5ecc: ldrb     r1, [r3, #1]
004f5ed0: ldrb     r2, [r6, #-1]
004f5ed4: cmp      r6, r3
004f5ed8: eor      r2, r1, r2
004f5edc: strb     r2, [r6, #-1]
004f5ee0: ldrb     r1, [r3, #1]
004f5ee4: eor      r2, r2, r1
004f5ee8: strb     r2, [r3, #1]
004f5eec: ldrb     r1, [r6, #-1]
004f5ef0: sub      r3, r3, #1
004f5ef4: eor      r2, r2, r1
004f5ef8: strb     r2, [r6, #-1]
004f5efc: add      r6, r6, #1
004f5f00: blo      #0x4f5ecc
004f5f04: add      r6, r4, #0x260
004f5f08: mov      r0, r5
004f5f0c: mov      r1, r6
004f5f10: bl       #0x459090
004f5f14: mov      r3, #1
004f5f18: cmp      r3, #0
004f5f1c: str      r3, [sp, #4]
004f5f20: bne      #0x4f5f64
004f5f24: add      r3, r6, #2
004f5f28: add      r6, r6, #1
004f5f2c: ldrb     r1, [r3, #1]
004f5f30: ldrb     r2, [r6, #-1]
004f5f34: cmp      r6, r3
004f5f38: eor      r2, r1, r2
004f5f3c: strb     r2, [r6, #-1]
004f5f40: ldrb     r1, [r3, #1]
004f5f44: eor      r2, r2, r1
004f5f48: strb     r2, [r3, #1]
004f5f4c: ldrb     r1, [r6, #-1]
004f5f50: sub      r3, r3, #1
004f5f54: eor      r2, r2, r1
004f5f58: strb     r2, [r6, #-1]
004f5f5c: add      r6, r6, #1
004f5f60: blo      #0x4f5f2c
004f5f64: add      r6, r4, #0x264
004f5f68: mov      r0, r5
004f5f6c: mov      r1, r6
004f5f70: bl       #0x459090
004f5f74: mov      r3, #1
004f5f78: cmp      r3, #0
004f5f7c: str      r3, [sp, #4]
004f5f80: bne      #0x4f5fc4
004f5f84: add      r3, r6, #2
004f5f88: add      r6, r6, #1
004f5f8c: ldrb     r1, [r3, #1]
004f5f90: ldrb     r2, [r6, #-1]
004f5f94: cmp      r6, r3
004f5f98: eor      r2, r1, r2
004f5f9c: strb     r2, [r6, #-1]
004f5fa0: ldrb     r1, [r3, #1]
004f5fa4: eor      r2, r2, r1
004f5fa8: strb     r2, [r3, #1]
004f5fac: ldrb     r1, [r6, #-1]
004f5fb0: sub      r3, r3, #1
004f5fb4: eor      r2, r2, r1
004f5fb8: strb     r2, [r6, #-1]
004f5fbc: add      r6, r6, #1
004f5fc0: blo      #0x4f5f8c
004f5fc4: add      r6, r4, #0x268
004f5fc8: mov      r0, r5
004f5fcc: mov      r1, r6
004f5fd0: bl       #0x459090
004f5fd4: mov      r3, #1
004f5fd8: cmp      r3, #0
004f5fdc: str      r3, [sp, #4]
004f5fe0: bne      #0x4f6024
004f5fe4: add      r3, r6, #2
004f5fe8: add      r6, r6, #1
004f5fec: ldrb     r1, [r3, #1]
004f5ff0: ldrb     r2, [r6, #-1]
004f5ff4: cmp      r6, r3
004f5ff8: eor      r2, r1, r2
004f5ffc: strb     r2, [r6, #-1]
004f6000: ldrb     r1, [r3, #1]
004f6004: eor      r2, r2, r1
004f6008: strb     r2, [r3, #1]
004f600c: ldrb     r1, [r6, #-1]
004f6010: sub      r3, r3, #1
004f6014: eor      r2, r2, r1
004f6018: strb     r2, [r6, #-1]
004f601c: add      r6, r6, #1
004f6020: blo      #0x4f5fec
004f6024: add      r6, r4, #0x26c
004f6028: mov      r0, r5
004f602c: mov      r1, r6
004f6030: bl       #0x459090
004f6034: mov      r3, #1
004f6038: cmp      r3, #0
004f603c: str      r3, [sp, #4]
004f6040: bne      #0x4f6084
004f6044: add      r3, r6, #2
004f6048: add      r6, r6, #1
004f604c: ldrb     r1, [r3, #1]
004f6050: ldrb     r2, [r6, #-1]
004f6054: cmp      r6, r3
004f6058: eor      r2, r1, r2
004f605c: strb     r2, [r6, #-1]
004f6060: ldrb     r1, [r3, #1]
004f6064: eor      r2, r2, r1
004f6068: strb     r2, [r3, #1]
004f606c: ldrb     r1, [r6, #-1]
004f6070: sub      r3, r3, #1
004f6074: eor      r2, r2, r1
004f6078: strb     r2, [r6, #-1]
004f607c: add      r6, r6, #1
004f6080: blo      #0x4f604c
004f6084: add      r6, r4, #0x270
004f6088: mov      r0, r5
004f608c: mov      r1, r6
004f6090: bl       #0x459090
004f6094: mov      r3, #1
004f6098: cmp      r3, #0
004f609c: str      r3, [sp, #4]
004f60a0: bne      #0x4f60e4
004f60a4: add      r3, r6, #2
004f60a8: add      r6, r6, #1
004f60ac: ldrb     r1, [r3, #1]
004f60b0: ldrb     r2, [r6, #-1]
004f60b4: cmp      r6, r3
004f60b8: eor      r2, r1, r2
004f60bc: strb     r2, [r6, #-1]
004f60c0: ldrb     r1, [r3, #1]
004f60c4: eor      r2, r2, r1
004f60c8: strb     r2, [r3, #1]
004f60cc: ldrb     r1, [r6, #-1]
004f60d0: sub      r3, r3, #1
004f60d4: eor      r2, r2, r1
004f60d8: strb     r2, [r6, #-1]
004f60dc: add      r6, r6, #1
004f60e0: blo      #0x4f60ac
004f60e4: add      r6, r4, #0x274
004f60e8: mov      r0, r5
004f60ec: mov      r1, r6
004f60f0: bl       #0x459090
004f60f4: mov      r3, #1
004f60f8: cmp      r3, #0
004f60fc: str      r3, [sp, #4]
004f6100: bne      #0x4f6144
004f6104: add      r3, r6, #2
004f6108: add      r6, r6, #1
004f610c: ldrb     r1, [r3, #1]
004f6110: ldrb     r2, [r6, #-1]
004f6114: cmp      r6, r3
004f6118: eor      r2, r1, r2
004f611c: strb     r2, [r6, #-1]
004f6120: ldrb     r1, [r3, #1]
004f6124: eor      r2, r2, r1
004f6128: strb     r2, [r3, #1]
004f612c: ldrb     r1, [r6, #-1]
004f6130: sub      r3, r3, #1
004f6134: eor      r2, r2, r1
004f6138: strb     r2, [r6, #-1]
004f613c: add      r6, r6, #1
004f6140: blo      #0x4f610c
004f6144: add      r6, r4, #0x278
004f6148: mov      r0, r5
004f614c: mov      r1, r6
004f6150: bl       #0x459090
004f6154: mov      r3, #1
004f6158: cmp      r3, #0
004f615c: str      r3, [sp, #4]
004f6160: bne      #0x4f61a4
004f6164: add      r3, r6, #2
004f6168: add      r6, r6, #1
004f616c: ldrb     r1, [r3, #1]
004f6170: ldrb     r2, [r6, #-1]
004f6174: cmp      r6, r3
004f6178: eor      r2, r1, r2
004f617c: strb     r2, [r6, #-1]
004f6180: ldrb     r1, [r3, #1]
004f6184: eor      r2, r2, r1
004f6188: strb     r2, [r3, #1]
004f618c: ldrb     r1, [r6, #-1]
004f6190: sub      r3, r3, #1
004f6194: eor      r2, r2, r1
004f6198: strb     r2, [r6, #-1]
004f619c: add      r6, r6, #1
004f61a0: blo      #0x4f616c
004f61a4: add      r6, r4, #0x27c
004f61a8: mov      r0, r5
004f61ac: mov      r1, r6
004f61b0: bl       #0x459090
004f61b4: mov      r3, #1
004f61b8: cmp      r3, #0
004f61bc: str      r3, [sp, #4]
004f61c0: bne      #0x4f6204
004f61c4: add      r3, r6, #2
004f61c8: add      r6, r6, #1
004f61cc: ldrb     r1, [r3, #1]
004f61d0: ldrb     r2, [r6, #-1]
004f61d4: cmp      r6, r3
004f61d8: eor      r2, r1, r2
004f61dc: strb     r2, [r6, #-1]
004f61e0: ldrb     r1, [r3, #1]
004f61e4: eor      r2, r2, r1
004f61e8: strb     r2, [r3, #1]
004f61ec: ldrb     r1, [r6, #-1]
004f61f0: sub      r3, r3, #1
004f61f4: eor      r2, r2, r1
004f61f8: strb     r2, [r6, #-1]
004f61fc: add      r6, r6, #1
004f6200: blo      #0x4f61cc
004f6204: add      r6, r4, #0x280
004f6208: mov      r0, r5
004f620c: mov      r1, r6
004f6210: bl       #0x459090
004f6214: mov      r3, #1
004f6218: cmp      r3, #0
004f621c: str      r3, [sp, #4]
004f6220: bne      #0x4f6264
004f6224: add      r3, r6, #2
004f6228: add      r6, r6, #1
004f622c: ldrb     r1, [r3, #1]
004f6230: ldrb     r2, [r6, #-1]
004f6234: cmp      r6, r3
004f6238: eor      r2, r1, r2
004f623c: strb     r2, [r6, #-1]
004f6240: ldrb     r1, [r3, #1]
004f6244: eor      r2, r2, r1
004f6248: strb     r2, [r3, #1]
004f624c: ldrb     r1, [r6, #-1]
004f6250: sub      r3, r3, #1
004f6254: eor      r2, r2, r1
004f6258: strb     r2, [r6, #-1]
004f625c: add      r6, r6, #1
004f6260: blo      #0x4f622c
004f6264: add      r6, r4, #0x284
004f6268: mov      r0, r5
004f626c: mov      r1, r6
004f6270: bl       #0x459090
004f6274: mov      r3, #1
004f6278: cmp      r3, #0
004f627c: str      r3, [sp, #4]
004f6280: bne      #0x4f62c4
004f6284: add      r3, r6, #2
004f6288: add      r6, r6, #1
004f628c: ldrb     r1, [r3, #1]
004f6290: ldrb     r2, [r6, #-1]
004f6294: cmp      r6, r3
004f6298: eor      r2, r1, r2
004f629c: strb     r2, [r6, #-1]
004f62a0: ldrb     r1, [r3, #1]
004f62a4: eor      r2, r2, r1
004f62a8: strb     r2, [r3, #1]
004f62ac: ldrb     r1, [r6, #-1]
004f62b0: sub      r3, r3, #1
004f62b4: eor      r2, r2, r1
004f62b8: strb     r2, [r6, #-1]
004f62bc: add      r6, r6, #1
004f62c0: blo      #0x4f628c
004f62c4: add      r6, r4, #0x288
004f62c8: mov      r0, r5
004f62cc: mov      r1, r6
004f62d0: bl       #0x459090
004f62d4: mov      r3, #1
004f62d8: cmp      r3, #0
004f62dc: str      r3, [sp, #4]
004f62e0: bne      #0x4f6324
004f62e4: add      r3, r6, #2
004f62e8: add      r6, r6, #1
004f62ec: ldrb     r1, [r3, #1]
004f62f0: ldrb     r2, [r6, #-1]
004f62f4: cmp      r6, r3
004f62f8: eor      r2, r1, r2
004f62fc: strb     r2, [r6, #-1]
004f6300: ldrb     r1, [r3, #1]
004f6304: eor      r2, r2, r1
004f6308: strb     r2, [r3, #1]
004f630c: ldrb     r1, [r6, #-1]
004f6310: sub      r3, r3, #1
004f6314: eor      r2, r2, r1
004f6318: strb     r2, [r6, #-1]
004f631c: add      r6, r6, #1
004f6320: blo      #0x4f62ec
004f6324: add      r6, r4, #0x28c
004f6328: mov      r0, r5
004f632c: mov      r1, r6
004f6330: bl       #0x459090
004f6334: mov      r3, #1
004f6338: cmp      r3, #0
004f633c: str      r3, [sp, #4]
004f6340: bne      #0x4f6384
004f6344: add      r3, r6, #2
004f6348: add      r6, r6, #1
004f634c: ldrb     r1, [r3, #1]
004f6350: ldrb     r2, [r6, #-1]
004f6354: cmp      r6, r3
004f6358: eor      r2, r1, r2
004f635c: strb     r2, [r6, #-1]
004f6360: ldrb     r1, [r3, #1]
004f6364: eor      r2, r2, r1
004f6368: strb     r2, [r3, #1]
004f636c: ldrb     r1, [r6, #-1]
004f6370: sub      r3, r3, #1
004f6374: eor      r2, r2, r1
004f6378: strb     r2, [r6, #-1]
004f637c: add      r6, r6, #1
004f6380: blo      #0x4f634c
004f6384: add      r6, r4, #0x290
004f6388: mov      r0, r5
004f638c: mov      r1, r6
004f6390: bl       #0x459090
004f6394: mov      r3, #1
004f6398: cmp      r3, #0
004f639c: str      r3, [sp, #4]
004f63a0: bne      #0x4f63e4
004f63a4: add      r3, r6, #2
004f63a8: add      r6, r6, #1
004f63ac: ldrb     r1, [r3, #1]
004f63b0: ldrb     r2, [r6, #-1]
004f63b4: cmp      r6, r3
004f63b8: eor      r2, r1, r2
004f63bc: strb     r2, [r6, #-1]
004f63c0: ldrb     r1, [r3, #1]
004f63c4: eor      r2, r2, r1
004f63c8: strb     r2, [r3, #1]
004f63cc: ldrb     r1, [r6, #-1]
004f63d0: sub      r3, r3, #1
004f63d4: eor      r2, r2, r1
004f63d8: strb     r2, [r6, #-1]
004f63dc: add      r6, r6, #1
004f63e0: blo      #0x4f63ac
004f63e4: add      r6, r4, #0x294
004f63e8: mov      r0, r5
004f63ec: mov      r1, r6
004f63f0: bl       #0x459090
004f63f4: mov      r3, #1
004f63f8: cmp      r3, #0
004f63fc: str      r3, [sp, #4]
004f6400: bne      #0x4f6444
004f6404: add      r3, r6, #2
004f6408: add      r6, r6, #1
004f640c: ldrb     r1, [r3, #1]
004f6410: ldrb     r2, [r6, #-1]
004f6414: cmp      r6, r3
004f6418: eor      r2, r1, r2
004f641c: strb     r2, [r6, #-1]
004f6420: ldrb     r1, [r3, #1]
004f6424: eor      r2, r2, r1
004f6428: strb     r2, [r3, #1]
004f642c: ldrb     r1, [r6, #-1]
004f6430: sub      r3, r3, #1
004f6434: eor      r2, r2, r1
004f6438: strb     r2, [r6, #-1]
004f643c: add      r6, r6, #1
004f6440: blo      #0x4f640c
004f6444: add      r6, r4, #0x298
004f6448: mov      r0, r5
004f644c: mov      r1, r6
004f6450: bl       #0x459090
004f6454: mov      r3, #1
004f6458: cmp      r3, #0
004f645c: str      r3, [sp, #4]
004f6460: bne      #0x4f64a4
004f6464: add      r3, r6, #2
004f6468: add      r6, r6, #1
004f646c: ldrb     r1, [r3, #1]
004f6470: ldrb     r2, [r6, #-1]
004f6474: cmp      r6, r3
004f6478: eor      r2, r1, r2
004f647c: strb     r2, [r6, #-1]
004f6480: ldrb     r1, [r3, #1]
004f6484: eor      r2, r2, r1
004f6488: strb     r2, [r3, #1]
004f648c: ldrb     r1, [r6, #-1]
004f6490: sub      r3, r3, #1
004f6494: eor      r2, r2, r1
004f6498: strb     r2, [r6, #-1]
004f649c: add      r6, r6, #1
004f64a0: blo      #0x4f646c
004f64a4: add      r6, r4, #0x29c
004f64a8: mov      r0, r5
004f64ac: mov      r1, r6
004f64b0: bl       #0x459090
004f64b4: mov      r3, #1
004f64b8: cmp      r3, #0
004f64bc: str      r3, [sp, #4]
004f64c0: bne      #0x4f6504
004f64c4: add      r3, r6, #2
004f64c8: add      r6, r6, #1
004f64cc: ldrb     r1, [r3, #1]
004f64d0: ldrb     r2, [r6, #-1]
004f64d4: cmp      r6, r3
004f64d8: eor      r2, r1, r2
004f64dc: strb     r2, [r6, #-1]
004f64e0: ldrb     r1, [r3, #1]
004f64e4: eor      r2, r2, r1
004f64e8: strb     r2, [r3, #1]
004f64ec: ldrb     r1, [r6, #-1]
004f64f0: sub      r3, r3, #1
004f64f4: eor      r2, r2, r1
004f64f8: strb     r2, [r6, #-1]
004f64fc: add      r6, r6, #1
004f6500: blo      #0x4f64cc
004f6504: add      r6, r4, #0x2a0
004f6508: mov      r0, r5
004f650c: mov      r1, r6
004f6510: bl       #0x459090
004f6514: mov      r3, #1
004f6518: cmp      r3, #0
004f651c: str      r3, [sp, #4]
004f6520: bne      #0x4f6564
004f6524: add      r3, r6, #2
004f6528: add      r6, r6, #1
004f652c: ldrb     r1, [r3, #1]
004f6530: ldrb     r2, [r6, #-1]
004f6534: cmp      r6, r3
004f6538: eor      r2, r1, r2
004f653c: strb     r2, [r6, #-1]
004f6540: ldrb     r1, [r3, #1]
004f6544: eor      r2, r2, r1
004f6548: strb     r2, [r3, #1]
004f654c: ldrb     r1, [r6, #-1]
004f6550: sub      r3, r3, #1
004f6554: eor      r2, r2, r1
004f6558: strb     r2, [r6, #-1]
004f655c: add      r6, r6, #1
004f6560: blo      #0x4f652c
004f6564: add      r6, r4, #0x2a4
004f6568: mov      r0, r5
004f656c: mov      r1, r6
004f6570: bl       #0x459090
004f6574: mov      r3, #1
004f6578: cmp      r3, #0
004f657c: str      r3, [sp, #4]
004f6580: bne      #0x4f65c4
004f6584: add      r3, r6, #2
004f6588: add      r6, r6, #1
004f658c: ldrb     r1, [r3, #1]
004f6590: ldrb     r2, [r6, #-1]
004f6594: cmp      r6, r3
004f6598: eor      r2, r1, r2
004f659c: strb     r2, [r6, #-1]
004f65a0: ldrb     r1, [r3, #1]
004f65a4: eor      r2, r2, r1
004f65a8: strb     r2, [r3, #1]
004f65ac: ldrb     r1, [r6, #-1]
004f65b0: sub      r3, r3, #1
004f65b4: eor      r2, r2, r1
004f65b8: strb     r2, [r6, #-1]
004f65bc: add      r6, r6, #1
004f65c0: blo      #0x4f658c
004f65c4: add      r6, r4, #0x2a8
004f65c8: mov      r0, r5
004f65cc: mov      r1, r6
004f65d0: bl       #0x459090
004f65d4: mov      r3, #1
004f65d8: cmp      r3, #0
004f65dc: str      r3, [sp, #4]
004f65e0: bne      #0x4f6624
004f65e4: add      r3, r6, #2
004f65e8: add      r6, r6, #1
004f65ec: ldrb     r1, [r3, #1]
004f65f0: ldrb     r2, [r6, #-1]
004f65f4: cmp      r6, r3
004f65f8: eor      r2, r1, r2
004f65fc: strb     r2, [r6, #-1]
004f6600: ldrb     r1, [r3, #1]
004f6604: eor      r2, r2, r1
004f6608: strb     r2, [r3, #1]
004f660c: ldrb     r1, [r6, #-1]
004f6610: sub      r3, r3, #1
004f6614: eor      r2, r2, r1
004f6618: strb     r2, [r6, #-1]
004f661c: add      r6, r6, #1
004f6620: blo      #0x4f65ec
004f6624: add      r6, r4, #0x2ac
004f6628: mov      r0, r5
004f662c: mov      r1, r6
004f6630: bl       #0x459090
004f6634: mov      r3, #1
004f6638: cmp      r3, #0
004f663c: str      r3, [sp, #4]
004f6640: bne      #0x4f6684
004f6644: add      r3, r6, #2
004f6648: add      r6, r6, #1
004f664c: ldrb     r1, [r3, #1]
004f6650: ldrb     r2, [r6, #-1]
004f6654: cmp      r6, r3
004f6658: eor      r2, r1, r2
004f665c: strb     r2, [r6, #-1]
004f6660: ldrb     r1, [r3, #1]
004f6664: eor      r2, r2, r1
004f6668: strb     r2, [r3, #1]
004f666c: ldrb     r1, [r6, #-1]
004f6670: sub      r3, r3, #1
004f6674: eor      r2, r2, r1
004f6678: strb     r2, [r6, #-1]
004f667c: add      r6, r6, #1
004f6680: blo      #0x4f664c
004f6684: add      r6, r4, #0x2b0
004f6688: mov      r0, r5
004f668c: mov      r1, r6
004f6690: bl       #0x459090
004f6694: mov      r3, #1
004f6698: cmp      r3, #0
004f669c: str      r3, [sp, #4]
004f66a0: bne      #0x4f66e4
004f66a4: add      r3, r6, #2
004f66a8: add      r6, r6, #1
004f66ac: ldrb     r1, [r3, #1]
004f66b0: ldrb     r2, [r6, #-1]
004f66b4: cmp      r6, r3
004f66b8: eor      r2, r1, r2
004f66bc: strb     r2, [r6, #-1]
004f66c0: ldrb     r1, [r3, #1]
004f66c4: eor      r2, r2, r1
004f66c8: strb     r2, [r3, #1]
004f66cc: ldrb     r1, [r6, #-1]
004f66d0: sub      r3, r3, #1
004f66d4: eor      r2, r2, r1
004f66d8: strb     r2, [r6, #-1]
004f66dc: add      r6, r6, #1
004f66e0: blo      #0x4f66ac
004f66e4: add      r6, r4, #0x2b4
004f66e8: mov      r0, r5
004f66ec: mov      r1, r6
004f66f0: bl       #0x459090
004f66f4: mov      r3, #1
004f66f8: cmp      r3, #0
004f66fc: str      r3, [sp, #4]
004f6700: bne      #0x4f6744
004f6704: add      r3, r6, #2
004f6708: add      r6, r6, #1
004f670c: ldrb     r1, [r3, #1]
004f6710: ldrb     r2, [r6, #-1]
004f6714: cmp      r6, r3
004f6718: eor      r2, r1, r2
004f671c: strb     r2, [r6, #-1]
004f6720: ldrb     r1, [r3, #1]
004f6724: eor      r2, r2, r1
004f6728: strb     r2, [r3, #1]
004f672c: ldrb     r1, [r6, #-1]
004f6730: sub      r3, r3, #1
004f6734: eor      r2, r2, r1
004f6738: strb     r2, [r6, #-1]
004f673c: add      r6, r6, #1
004f6740: blo      #0x4f670c
004f6744: add      r6, r4, #0x2b8
004f6748: mov      r0, r5
004f674c: mov      r1, r6
004f6750: bl       #0x459090
004f6754: mov      r3, #1
004f6758: cmp      r3, #0
004f675c: str      r3, [sp, #4]
004f6760: bne      #0x4f67a4
004f6764: add      r3, r6, #2
004f6768: add      r6, r6, #1
004f676c: ldrb     r1, [r3, #1]
004f6770: ldrb     r2, [r6, #-1]
004f6774: cmp      r6, r3
004f6778: eor      r2, r1, r2
004f677c: strb     r2, [r6, #-1]
004f6780: ldrb     r1, [r3, #1]
004f6784: eor      r2, r2, r1
004f6788: strb     r2, [r3, #1]
004f678c: ldrb     r1, [r6, #-1]
004f6790: sub      r3, r3, #1
004f6794: eor      r2, r2, r1
004f6798: strb     r2, [r6, #-1]
004f679c: add      r6, r6, #1
004f67a0: blo      #0x4f676c
004f67a4: add      r6, r4, #0x2bc
004f67a8: mov      r0, r5
004f67ac: mov      r1, r6
004f67b0: bl       #0x459090
004f67b4: mov      r3, #1
004f67b8: cmp      r3, #0
004f67bc: str      r3, [sp, #4]
004f67c0: bne      #0x4f6804
004f67c4: add      r3, r6, #2
004f67c8: add      r6, r6, #1
004f67cc: ldrb     r1, [r3, #1]
004f67d0: ldrb     r2, [r6, #-1]
004f67d4: cmp      r6, r3
004f67d8: eor      r2, r1, r2
004f67dc: strb     r2, [r6, #-1]
004f67e0: ldrb     r1, [r3, #1]
004f67e4: eor      r2, r2, r1
004f67e8: strb     r2, [r3, #1]
004f67ec: ldrb     r1, [r6, #-1]
004f67f0: sub      r3, r3, #1
004f67f4: eor      r2, r2, r1
004f67f8: strb     r2, [r6, #-1]
004f67fc: add      r6, r6, #1
004f6800: blo      #0x4f67cc
004f6804: add      r6, r4, #0x2c0
004f6808: mov      r0, r5
004f680c: mov      r1, r6
004f6810: bl       #0x459090
004f6814: mov      r3, #1
004f6818: cmp      r3, #0
004f681c: str      r3, [sp, #4]
004f6820: bne      #0x4f6864
004f6824: add      r3, r6, #2
004f6828: add      r6, r6, #1
004f682c: ldrb     r1, [r3, #1]
004f6830: ldrb     r2, [r6, #-1]
004f6834: cmp      r6, r3
004f6838: eor      r2, r1, r2
004f683c: strb     r2, [r6, #-1]
004f6840: ldrb     r1, [r3, #1]
004f6844: eor      r2, r2, r1
004f6848: strb     r2, [r3, #1]
004f684c: ldrb     r1, [r6, #-1]
004f6850: sub      r3, r3, #1
004f6854: eor      r2, r2, r1
004f6858: strb     r2, [r6, #-1]
004f685c: add      r6, r6, #1
004f6860: blo      #0x4f682c
004f6864: add      r6, r4, #0x2c4
004f6868: mov      r0, r5
004f686c: mov      r1, r6
004f6870: bl       #0x459090
004f6874: mov      r3, #1
004f6878: cmp      r3, #0
004f687c: str      r3, [sp, #4]
004f6880: bne      #0x4f68c4
004f6884: add      r3, r6, #2
004f6888: add      r6, r6, #1
004f688c: ldrb     r1, [r3, #1]
004f6890: ldrb     r2, [r6, #-1]
004f6894: cmp      r6, r3
004f6898: eor      r2, r1, r2
004f689c: strb     r2, [r6, #-1]
004f68a0: ldrb     r1, [r3, #1]
004f68a4: eor      r2, r2, r1
004f68a8: strb     r2, [r3, #1]
004f68ac: ldrb     r1, [r6, #-1]
004f68b0: sub      r3, r3, #1
004f68b4: eor      r2, r2, r1
004f68b8: strb     r2, [r6, #-1]
004f68bc: add      r6, r6, #1
004f68c0: blo      #0x4f688c
004f68c4: add      r6, r4, #0x2c8
004f68c8: mov      r0, r5
004f68cc: mov      r1, r6
004f68d0: bl       #0x459090
004f68d4: mov      r3, #1
004f68d8: cmp      r3, #0
004f68dc: str      r3, [sp, #4]
004f68e0: bne      #0x4f6924
004f68e4: add      r3, r6, #2
004f68e8: add      r6, r6, #1
004f68ec: ldrb     r1, [r3, #1]
004f68f0: ldrb     r2, [r6, #-1]
004f68f4: cmp      r6, r3
004f68f8: eor      r2, r1, r2
004f68fc: strb     r2, [r6, #-1]
004f6900: ldrb     r1, [r3, #1]
004f6904: eor      r2, r2, r1
004f6908: strb     r2, [r3, #1]
004f690c: ldrb     r1, [r6, #-1]
004f6910: sub      r3, r3, #1
004f6914: eor      r2, r2, r1
004f6918: strb     r2, [r6, #-1]
004f691c: add      r6, r6, #1
004f6920: blo      #0x4f68ec
004f6924: add      r6, r4, #0x2cc
004f6928: mov      r0, r5
004f692c: mov      r1, r6
004f6930: bl       #0x459090
004f6934: mov      r3, #1
004f6938: cmp      r3, #0
004f693c: str      r3, [sp, #4]
004f6940: bne      #0x4f6984
004f6944: add      r3, r6, #2
004f6948: add      r6, r6, #1
004f694c: ldrb     r1, [r3, #1]
004f6950: ldrb     r2, [r6, #-1]
004f6954: cmp      r6, r3
004f6958: eor      r2, r1, r2
004f695c: strb     r2, [r6, #-1]
004f6960: ldrb     r1, [r3, #1]
004f6964: eor      r2, r2, r1
004f6968: strb     r2, [r3, #1]
004f696c: ldrb     r1, [r6, #-1]
004f6970: sub      r3, r3, #1
004f6974: eor      r2, r2, r1
004f6978: strb     r2, [r6, #-1]
004f697c: add      r6, r6, #1
004f6980: blo      #0x4f694c
004f6984: add      r6, r4, #0x2d0
004f6988: mov      r0, r5
004f698c: mov      r1, r6
004f6990: bl       #0x459090
004f6994: mov      r3, #1
004f6998: cmp      r3, #0
004f699c: str      r3, [sp, #4]
004f69a0: bne      #0x4f69e4
004f69a4: add      r3, r6, #2
004f69a8: add      r6, r6, #1
004f69ac: ldrb     r1, [r3, #1]
004f69b0: ldrb     r2, [r6, #-1]
004f69b4: cmp      r6, r3
004f69b8: eor      r2, r1, r2
004f69bc: strb     r2, [r6, #-1]
004f69c0: ldrb     r1, [r3, #1]
004f69c4: eor      r2, r2, r1
004f69c8: strb     r2, [r3, #1]
004f69cc: ldrb     r1, [r6, #-1]
004f69d0: sub      r3, r3, #1
004f69d4: eor      r2, r2, r1
004f69d8: strb     r2, [r6, #-1]
004f69dc: add      r6, r6, #1
004f69e0: blo      #0x4f69ac
004f69e4: add      r6, r4, #0x2d4
004f69e8: mov      r0, r5
004f69ec: mov      r1, r6
004f69f0: bl       #0x459090
004f69f4: mov      r3, #1
004f69f8: cmp      r3, #0
004f69fc: str      r3, [sp, #4]
004f6a00: bne      #0x4f6a44
004f6a04: add      r3, r6, #2
004f6a08: add      r6, r6, #1
004f6a0c: ldrb     r1, [r3, #1]
004f6a10: ldrb     r2, [r6, #-1]
004f6a14: cmp      r3, r6
004f6a18: eor      r2, r1, r2
004f6a1c: strb     r2, [r6, #-1]
004f6a20: ldrb     r1, [r3, #1]
004f6a24: eor      r2, r2, r1
004f6a28: strb     r2, [r3, #1]
004f6a2c: ldrb     r1, [r6, #-1]
004f6a30: sub      r3, r3, #1
004f6a34: eor      r2, r2, r1
004f6a38: strb     r2, [r6, #-1]
004f6a3c: add      r6, r6, #1
004f6a40: bhi      #0x4f6a0c
004f6a44: add      r6, r4, #0x2d8
004f6a48: mov      r0, r5
004f6a4c: mov      r1, r6
004f6a50: bl       #0x459090
004f6a54: mov      r3, #1
004f6a58: cmp      r3, #0
004f6a5c: str      r3, [sp, #4]
004f6a60: bne      #0x4f6aa4
004f6a64: add      r3, r6, #2
004f6a68: add      r6, r6, #1
004f6a6c: ldrb     r1, [r3, #1]
004f6a70: ldrb     r2, [r6, #-1]
004f6a74: cmp      r3, r6
004f6a78: eor      r2, r1, r2
004f6a7c: strb     r2, [r6, #-1]
004f6a80: ldrb     r1, [r3, #1]
004f6a84: eor      r2, r2, r1
004f6a88: strb     r2, [r3, #1]
004f6a8c: ldrb     r1, [r6, #-1]
004f6a90: sub      r3, r3, #1
004f6a94: eor      r2, r2, r1
004f6a98: strb     r2, [r6, #-1]
004f6a9c: add      r6, r6, #1
004f6aa0: bhi      #0x4f6a6c
004f6aa4: add      r6, r4, #0x2dc
004f6aa8: mov      r0, r5
004f6aac: mov      r1, r6
004f6ab0: bl       #0x459090
004f6ab4: mov      r3, #1
004f6ab8: cmp      r3, #0
004f6abc: str      r3, [sp, #4]
004f6ac0: bne      #0x4f6b04
004f6ac4: add      r3, r6, #2
004f6ac8: add      r6, r6, #1
004f6acc: ldrb     r1, [r3, #1]
004f6ad0: ldrb     r2, [r6, #-1]
004f6ad4: cmp      r6, r3
004f6ad8: eor      r2, r1, r2
004f6adc: strb     r2, [r6, #-1]
004f6ae0: ldrb     r1, [r3, #1]
004f6ae4: eor      r2, r2, r1
004f6ae8: strb     r2, [r3, #1]
004f6aec: ldrb     r1, [r6, #-1]
004f6af0: sub      r3, r3, #1
004f6af4: eor      r2, r2, r1
004f6af8: strb     r2, [r6, #-1]
004f6afc: add      r6, r6, #1
004f6b00: blo      #0x4f6acc
004f6b04: add      r6, r4, #0x2e0
004f6b08: mov      r0, r5
004f6b0c: mov      r1, r6
004f6b10: bl       #0x459090
004f6b14: mov      r3, #1
004f6b18: cmp      r3, #0
004f6b1c: str      r3, [sp, #4]
004f6b20: bne      #0x4f6b64
004f6b24: add      r3, r6, #2
004f6b28: add      r6, r6, #1
004f6b2c: ldrb     r1, [r3, #1]
004f6b30: ldrb     r2, [r6, #-1]
004f6b34: cmp      r3, r6
004f6b38: eor      r2, r1, r2
004f6b3c: strb     r2, [r6, #-1]
004f6b40: ldrb     r1, [r3, #1]
004f6b44: eor      r2, r2, r1
004f6b48: strb     r2, [r3, #1]
004f6b4c: ldrb     r1, [r6, #-1]
004f6b50: sub      r3, r3, #1
004f6b54: eor      r2, r2, r1
004f6b58: strb     r2, [r6, #-1]
004f6b5c: add      r6, r6, #1
004f6b60: bhi      #0x4f6b2c
004f6b64: add      r6, r4, #0x2e4
004f6b68: mov      r0, r5
004f6b6c: mov      r1, r6
004f6b70: bl       #0x459090
004f6b74: mov      r3, #1
004f6b78: cmp      r3, #0
004f6b7c: str      r3, [sp, #4]
004f6b80: bne      #0x4f6bc4
004f6b84: add      r3, r6, #2
004f6b88: add      r6, r6, #1
004f6b8c: ldrb     r1, [r3, #1]
004f6b90: ldrb     r2, [r6, #-1]
004f6b94: cmp      r3, r6
004f6b98: eor      r2, r1, r2
004f6b9c: strb     r2, [r6, #-1]
004f6ba0: ldrb     r1, [r3, #1]
004f6ba4: eor      r2, r2, r1
004f6ba8: strb     r2, [r3, #1]
004f6bac: ldrb     r1, [r6, #-1]
004f6bb0: sub      r3, r3, #1
004f6bb4: eor      r2, r2, r1
004f6bb8: strb     r2, [r6, #-1]
004f6bbc: add      r6, r6, #1
004f6bc0: bhi      #0x4f6b8c
004f6bc4: add      r6, r4, #0x2e8
004f6bc8: mov      r0, r5
004f6bcc: mov      r1, r6
004f6bd0: bl       #0x459090
004f6bd4: mov      r3, #1
004f6bd8: cmp      r3, #0
004f6bdc: str      r3, [sp, #4]
004f6be0: bne      #0x4f6c24
004f6be4: add      r3, r6, #2
004f6be8: add      r6, r6, #1
004f6bec: ldrb     r1, [r3, #1]
004f6bf0: ldrb     r2, [r6, #-1]
004f6bf4: cmp      r6, r3
004f6bf8: eor      r2, r1, r2
004f6bfc: strb     r2, [r6, #-1]
004f6c00: ldrb     r1, [r3, #1]
004f6c04: eor      r2, r2, r1
004f6c08: strb     r2, [r3, #1]
004f6c0c: ldrb     r1, [r6, #-1]
004f6c10: sub      r3, r3, #1
004f6c14: eor      r2, r2, r1
004f6c18: strb     r2, [r6, #-1]
004f6c1c: add      r6, r6, #1
004f6c20: blo      #0x4f6bec
004f6c24: add      r6, r4, #0x2ec
004f6c28: mov      r0, r5
004f6c2c: mov      r1, r6
004f6c30: bl       #0x459090
004f6c34: mov      r3, #1
004f6c38: cmp      r3, #0
004f6c3c: str      r3, [sp, #4]
004f6c40: bne      #0x4f6c84
004f6c44: add      r3, r6, #2
004f6c48: add      r6, r6, #1
004f6c4c: ldrb     r1, [r3, #1]
004f6c50: ldrb     r2, [r6, #-1]
004f6c54: cmp      r3, r6
004f6c58: eor      r2, r1, r2
004f6c5c: strb     r2, [r6, #-1]
004f6c60: ldrb     r1, [r3, #1]
004f6c64: eor      r2, r2, r1
004f6c68: strb     r2, [r3, #1]
004f6c6c: ldrb     r1, [r6, #-1]
004f6c70: sub      r3, r3, #1
004f6c74: eor      r2, r2, r1
004f6c78: strb     r2, [r6, #-1]
004f6c7c: add      r6, r6, #1
004f6c80: bhi      #0x4f6c4c
004f6c84: add      r6, r4, #0x2f0
004f6c88: mov      r0, r5
004f6c8c: mov      r1, r6
004f6c90: bl       #0x459090
004f6c94: mov      r3, #1
004f6c98: cmp      r3, #0
004f6c9c: str      r3, [sp, #4]
004f6ca0: bne      #0x4f6ce4
004f6ca4: add      r3, r6, #2
004f6ca8: add      r6, r6, #1
004f6cac: ldrb     r1, [r3, #1]
004f6cb0: ldrb     r2, [r6, #-1]
004f6cb4: cmp      r3, r6
004f6cb8: eor      r2, r1, r2
004f6cbc: strb     r2, [r6, #-1]
004f6cc0: ldrb     r1, [r3, #1]
004f6cc4: eor      r2, r2, r1
004f6cc8: strb     r2, [r3, #1]
004f6ccc: ldrb     r1, [r6, #-1]
004f6cd0: sub      r3, r3, #1
004f6cd4: eor      r2, r2, r1
004f6cd8: strb     r2, [r6, #-1]
004f6cdc: add      r6, r6, #1
004f6ce0: bhi      #0x4f6cac
004f6ce4: add      r6, r4, #0x2f4
004f6ce8: mov      r0, r5
004f6cec: mov      r1, r6
004f6cf0: bl       #0x459090
004f6cf4: mov      r3, #1
004f6cf8: cmp      r3, #0
004f6cfc: str      r3, [sp, #4]
004f6d00: bne      #0x4f6d44
004f6d04: add      r3, r6, #2
004f6d08: add      r6, r6, #1
004f6d0c: ldrb     r1, [r3, #1]
004f6d10: ldrb     r2, [r6, #-1]
004f6d14: cmp      r6, r3
004f6d18: eor      r2, r1, r2
004f6d1c: strb     r2, [r6, #-1]
004f6d20: ldrb     r1, [r3, #1]
004f6d24: eor      r2, r2, r1
004f6d28: strb     r2, [r3, #1]
004f6d2c: ldrb     r1, [r6, #-1]
004f6d30: sub      r3, r3, #1
004f6d34: eor      r2, r2, r1
004f6d38: strb     r2, [r6, #-1]
004f6d3c: add      r6, r6, #1
004f6d40: blo      #0x4f6d0c
004f6d44: add      r6, r4, #0x2f8
004f6d48: mov      r0, r5
004f6d4c: mov      r1, r6
004f6d50: bl       #0x459090
004f6d54: mov      r3, #1
004f6d58: cmp      r3, #0
004f6d5c: str      r3, [sp, #4]
004f6d60: bne      #0x4f6da4
004f6d64: add      r3, r6, #2
004f6d68: add      r6, r6, #1
004f6d6c: ldrb     r1, [r3, #1]
004f6d70: ldrb     r2, [r6, #-1]
004f6d74: cmp      r3, r6
004f6d78: eor      r2, r1, r2
004f6d7c: strb     r2, [r6, #-1]
004f6d80: ldrb     r1, [r3, #1]
004f6d84: eor      r2, r2, r1
004f6d88: strb     r2, [r3, #1]
004f6d8c: ldrb     r1, [r6, #-1]
004f6d90: sub      r3, r3, #1
004f6d94: eor      r2, r2, r1
004f6d98: strb     r2, [r6, #-1]
004f6d9c: add      r6, r6, #1
004f6da0: bhi      #0x4f6d6c
004f6da4: add      r6, r4, #0x2fc
004f6da8: mov      r0, r5
004f6dac: mov      r1, r6
004f6db0: bl       #0x459090
004f6db4: mov      r3, #1
004f6db8: cmp      r3, #0
004f6dbc: str      r3, [sp, #4]
004f6dc0: bne      #0x4f6e04
004f6dc4: add      r3, r6, #2
004f6dc8: add      r6, r6, #1
004f6dcc: ldrb     r1, [r3, #1]
004f6dd0: ldrb     r2, [r6, #-1]
004f6dd4: cmp      r3, r6
004f6dd8: eor      r2, r1, r2
004f6ddc: strb     r2, [r6, #-1]
004f6de0: ldrb     r1, [r3, #1]
004f6de4: eor      r2, r2, r1
004f6de8: strb     r2, [r3, #1]
004f6dec: ldrb     r1, [r6, #-1]
004f6df0: sub      r3, r3, #1
004f6df4: eor      r2, r2, r1
004f6df8: strb     r2, [r6, #-1]
004f6dfc: add      r6, r6, #1
004f6e00: bhi      #0x4f6dcc
004f6e04: add      r6, r4, #0x300
004f6e08: mov      r0, r5
004f6e0c: mov      r1, r6
004f6e10: bl       #0x459090
004f6e14: mov      r3, #1
004f6e18: cmp      r3, #0
004f6e1c: str      r3, [sp, #4]
004f6e20: bne      #0x4f6e64
004f6e24: add      r3, r6, #2
004f6e28: add      r6, r6, #1
004f6e2c: ldrb     r1, [r3, #1]
004f6e30: ldrb     r2, [r6, #-1]
004f6e34: cmp      r6, r3
004f6e38: eor      r2, r1, r2
004f6e3c: strb     r2, [r6, #-1]
004f6e40: ldrb     r1, [r3, #1]
004f6e44: eor      r2, r2, r1
004f6e48: strb     r2, [r3, #1]
004f6e4c: ldrb     r1, [r6, #-1]
004f6e50: sub      r3, r3, #1
004f6e54: eor      r2, r2, r1
004f6e58: strb     r2, [r6, #-1]
004f6e5c: add      r6, r6, #1
004f6e60: blo      #0x4f6e2c
004f6e64: add      r6, r4, #0x304
004f6e68: mov      r0, r5
004f6e6c: mov      r1, r6
004f6e70: bl       #0x459090
004f6e74: mov      r3, #1
004f6e78: cmp      r3, #0
004f6e7c: str      r3, [sp, #4]
004f6e80: bne      #0x4f6ec4
004f6e84: add      r3, r6, #2
004f6e88: add      r6, r6, #1
004f6e8c: ldrb     r1, [r3, #1]
004f6e90: ldrb     r2, [r6, #-1]
004f6e94: cmp      r3, r6
004f6e98: eor      r2, r1, r2
004f6e9c: strb     r2, [r6, #-1]
004f6ea0: ldrb     r1, [r3, #1]
004f6ea4: eor      r2, r2, r1
004f6ea8: strb     r2, [r3, #1]
004f6eac: ldrb     r1, [r6, #-1]
004f6eb0: sub      r3, r3, #1
004f6eb4: eor      r2, r2, r1
004f6eb8: strb     r2, [r6, #-1]
004f6ebc: add      r6, r6, #1
004f6ec0: bhi      #0x4f6e8c
004f6ec4: add      r6, r4, #0x308
004f6ec8: mov      r0, r5
004f6ecc: mov      r1, r6
004f6ed0: bl       #0x459090
004f6ed4: mov      r3, #1
004f6ed8: cmp      r3, #0
004f6edc: str      r3, [sp, #4]
004f6ee0: bne      #0x4f6f24
004f6ee4: add      r3, r6, #2
004f6ee8: add      r6, r6, #1
004f6eec: ldrb     r1, [r3, #1]
004f6ef0: ldrb     r2, [r6, #-1]
004f6ef4: cmp      r3, r6
004f6ef8: eor      r2, r1, r2
004f6efc: strb     r2, [r6, #-1]
004f6f00: ldrb     r1, [r3, #1]
004f6f04: eor      r2, r2, r1
004f6f08: strb     r2, [r3, #1]
004f6f0c: ldrb     r1, [r6, #-1]
004f6f10: sub      r3, r3, #1
004f6f14: eor      r2, r2, r1
004f6f18: strb     r2, [r6, #-1]
004f6f1c: add      r6, r6, #1
004f6f20: bhi      #0x4f6eec
004f6f24: add      r6, r4, #0x30c
004f6f28: mov      r0, r5
004f6f2c: mov      r1, r6
004f6f30: bl       #0x459090
004f6f34: mov      r3, #1
004f6f38: cmp      r3, #0
004f6f3c: str      r3, [sp, #4]
004f6f40: bne      #0x4f6f84
004f6f44: add      r3, r6, #2
004f6f48: add      r6, r6, #1
004f6f4c: ldrb     r1, [r3, #1]
004f6f50: ldrb     r2, [r6, #-1]
004f6f54: cmp      r6, r3
004f6f58: eor      r2, r1, r2
004f6f5c: strb     r2, [r6, #-1]
004f6f60: ldrb     r1, [r3, #1]
004f6f64: eor      r2, r2, r1
004f6f68: strb     r2, [r3, #1]
004f6f6c: ldrb     r1, [r6, #-1]
004f6f70: sub      r3, r3, #1
004f6f74: eor      r2, r2, r1
004f6f78: strb     r2, [r6, #-1]
004f6f7c: add      r6, r6, #1
004f6f80: blo      #0x4f6f4c
004f6f84: add      r6, r4, #0x310
004f6f88: mov      r0, r5
004f6f8c: mov      r1, r6
004f6f90: bl       #0x459090
004f6f94: mov      r3, #1
004f6f98: cmp      r3, #0
004f6f9c: str      r3, [sp, #4]
004f6fa0: bne      #0x4f6fe4
004f6fa4: add      r3, r6, #2
004f6fa8: add      r6, r6, #1
004f6fac: ldrb     r1, [r3, #1]
004f6fb0: ldrb     r2, [r6, #-1]
004f6fb4: cmp      r3, r6
004f6fb8: eor      r2, r1, r2
004f6fbc: strb     r2, [r6, #-1]
004f6fc0: ldrb     r1, [r3, #1]
004f6fc4: eor      r2, r2, r1
004f6fc8: strb     r2, [r3, #1]
004f6fcc: ldrb     r1, [r6, #-1]
004f6fd0: sub      r3, r3, #1
004f6fd4: eor      r2, r2, r1
004f6fd8: strb     r2, [r6, #-1]
004f6fdc: add      r6, r6, #1
004f6fe0: bhi      #0x4f6fac
004f6fe4: add      r6, r4, #0x314
004f6fe8: mov      r0, r5
004f6fec: mov      r1, r6
004f6ff0: bl       #0x459090
004f6ff4: mov      r3, #1
004f6ff8: cmp      r3, #0
004f6ffc: str      r3, [sp, #4]
004f7000: bne      #0x4f7044
004f7004: add      r3, r6, #2
004f7008: add      r6, r6, #1
004f700c: ldrb     r1, [r3, #1]
004f7010: ldrb     r2, [r6, #-1]
004f7014: cmp      r3, r6
004f7018: eor      r2, r1, r2
004f701c: strb     r2, [r6, #-1]
004f7020: ldrb     r1, [r3, #1]
004f7024: eor      r2, r2, r1
004f7028: strb     r2, [r3, #1]
004f702c: ldrb     r1, [r6, #-1]
004f7030: sub      r3, r3, #1
004f7034: eor      r2, r2, r1
004f7038: strb     r2, [r6, #-1]
004f703c: add      r6, r6, #1
004f7040: bhi      #0x4f700c
004f7044: add      r6, r4, #0x318
004f7048: mov      r0, r5
004f704c: mov      r1, r6
004f7050: bl       #0x459090
004f7054: mov      r3, #1
004f7058: cmp      r3, #0
004f705c: str      r3, [sp, #4]
004f7060: bne      #0x4f70a4
004f7064: add      r3, r6, #2
004f7068: add      r6, r6, #1
004f706c: ldrb     r1, [r3, #1]
004f7070: ldrb     r2, [r6, #-1]
004f7074: cmp      r6, r3
004f7078: eor      r2, r1, r2
004f707c: strb     r2, [r6, #-1]
004f7080: ldrb     r1, [r3, #1]
004f7084: eor      r2, r2, r1
004f7088: strb     r2, [r3, #1]
004f708c: ldrb     r1, [r6, #-1]
004f7090: sub      r3, r3, #1
004f7094: eor      r2, r2, r1
004f7098: strb     r2, [r6, #-1]
004f709c: add      r6, r6, #1
004f70a0: blo      #0x4f706c
004f70a4: add      r6, r4, #0x31c
004f70a8: mov      r0, r5
004f70ac: mov      r1, r6
004f70b0: bl       #0x459090
004f70b4: mov      r3, #1
004f70b8: cmp      r3, #0
004f70bc: str      r3, [sp, #4]
004f70c0: bne      #0x4f7104
004f70c4: add      r3, r6, #2
004f70c8: add      r6, r6, #1
004f70cc: ldrb     r1, [r3, #1]
004f70d0: ldrb     r2, [r6, #-1]
004f70d4: cmp      r3, r6
004f70d8: eor      r2, r1, r2
004f70dc: strb     r2, [r6, #-1]
004f70e0: ldrb     r1, [r3, #1]
004f70e4: eor      r2, r2, r1
004f70e8: strb     r2, [r3, #1]
004f70ec: ldrb     r1, [r6, #-1]
004f70f0: sub      r3, r3, #1
004f70f4: eor      r2, r2, r1
004f70f8: strb     r2, [r6, #-1]
004f70fc: add      r6, r6, #1
004f7100: bhi      #0x4f70cc
004f7104: add      r6, r4, #0x320
004f7108: mov      r0, r5
004f710c: mov      r1, r6
004f7110: bl       #0x459090
004f7114: mov      r3, #1
004f7118: cmp      r3, #0
004f711c: str      r3, [sp, #4]
004f7120: bne      #0x4f7164
004f7124: add      r3, r6, #2
004f7128: add      r6, r6, #1
004f712c: ldrb     r1, [r3, #1]
004f7130: ldrb     r2, [r6, #-1]
004f7134: cmp      r3, r6
004f7138: eor      r2, r1, r2
004f713c: strb     r2, [r6, #-1]
004f7140: ldrb     r1, [r3, #1]
004f7144: eor      r2, r2, r1
004f7148: strb     r2, [r3, #1]
004f714c: ldrb     r1, [r6, #-1]
004f7150: sub      r3, r3, #1
004f7154: eor      r2, r2, r1
004f7158: strb     r2, [r6, #-1]
004f715c: add      r6, r6, #1
004f7160: bhi      #0x4f712c
004f7164: add      r6, r4, #0x324
004f7168: mov      r0, r5
004f716c: mov      r1, r6
004f7170: bl       #0x459090
004f7174: mov      r3, #1
004f7178: cmp      r3, #0
004f717c: str      r3, [sp, #4]
004f7180: bne      #0x4f71c4
004f7184: add      r3, r6, #2
004f7188: add      r6, r6, #1
004f718c: ldrb     r1, [r3, #1]
004f7190: ldrb     r2, [r6, #-1]
004f7194: cmp      r6, r3
004f7198: eor      r2, r1, r2
004f719c: strb     r2, [r6, #-1]
004f71a0: ldrb     r1, [r3, #1]
004f71a4: eor      r2, r2, r1
004f71a8: strb     r2, [r3, #1]
004f71ac: ldrb     r1, [r6, #-1]
004f71b0: sub      r3, r3, #1
004f71b4: eor      r2, r2, r1
004f71b8: strb     r2, [r6, #-1]
004f71bc: add      r6, r6, #1
004f71c0: blo      #0x4f718c
004f71c4: add      r6, r4, #0x328
004f71c8: mov      r0, r5
004f71cc: mov      r1, r6
004f71d0: bl       #0x459090
004f71d4: mov      r3, #1
004f71d8: cmp      r3, #0
004f71dc: str      r3, [sp, #4]
004f71e0: bne      #0x4f7224
004f71e4: add      r3, r6, #2
004f71e8: add      r6, r6, #1
004f71ec: ldrb     r1, [r3, #1]
004f71f0: ldrb     r2, [r6, #-1]
004f71f4: cmp      r3, r6
004f71f8: eor      r2, r1, r2
004f71fc: strb     r2, [r6, #-1]
004f7200: ldrb     r1, [r3, #1]
004f7204: eor      r2, r2, r1
004f7208: strb     r2, [r3, #1]
004f720c: ldrb     r1, [r6, #-1]
004f7210: sub      r3, r3, #1
004f7214: eor      r2, r2, r1
004f7218: strb     r2, [r6, #-1]
004f721c: add      r6, r6, #1
004f7220: bhi      #0x4f71ec
004f7224: add      r6, r4, #0x32c
004f7228: mov      r0, r5
004f722c: mov      r1, r6
004f7230: bl       #0x459090
004f7234: mov      r3, #1
004f7238: cmp      r3, #0
004f723c: str      r3, [sp, #4]
004f7240: bne      #0x4f7284
004f7244: add      r3, r6, #2
004f7248: add      r6, r6, #1
004f724c: ldrb     r1, [r3, #1]
004f7250: ldrb     r2, [r6, #-1]
004f7254: cmp      r3, r6
004f7258: eor      r2, r1, r2
004f725c: strb     r2, [r6, #-1]
004f7260: ldrb     r1, [r3, #1]
004f7264: eor      r2, r2, r1
004f7268: strb     r2, [r3, #1]
004f726c: ldrb     r1, [r6, #-1]
004f7270: sub      r3, r3, #1
004f7274: eor      r2, r2, r1
004f7278: strb     r2, [r6, #-1]
004f727c: add      r6, r6, #1
004f7280: bhi      #0x4f724c
004f7284: add      r6, r4, #0x330
004f7288: mov      r0, r5
004f728c: mov      r1, r6
004f7290: bl       #0x459090
004f7294: mov      r3, #1
004f7298: cmp      r3, #0
004f729c: str      r3, [sp, #4]
004f72a0: bne      #0x4f72e4
004f72a4: add      r3, r6, #2
004f72a8: add      r6, r6, #1
004f72ac: ldrb     r1, [r3, #1]
004f72b0: ldrb     r2, [r6, #-1]
004f72b4: cmp      r3, r6
004f72b8: eor      r2, r1, r2
004f72bc: strb     r2, [r6, #-1]
004f72c0: ldrb     r1, [r3, #1]
004f72c4: eor      r2, r2, r1
004f72c8: strb     r2, [r3, #1]
004f72cc: ldrb     r1, [r6, #-1]
004f72d0: sub      r3, r3, #1
004f72d4: eor      r2, r2, r1
004f72d8: strb     r2, [r6, #-1]
004f72dc: add      r6, r6, #1
004f72e0: bhi      #0x4f72ac
004f72e4: add      r6, r4, #0x334
004f72e8: mov      r0, r5
004f72ec: mov      r1, r6
004f72f0: bl       #0x459090
004f72f4: mov      r3, #1
004f72f8: cmp      r3, #0
004f72fc: str      r3, [sp, #4]
004f7300: bne      #0x4f7344
004f7304: add      r3, r6, #2
004f7308: add      r6, r6, #1
004f730c: ldrb     r1, [r3, #1]
004f7310: ldrb     r2, [r6, #-1]
004f7314: cmp      r6, r3
004f7318: eor      r2, r1, r2
004f731c: strb     r2, [r6, #-1]
004f7320: ldrb     r1, [r3, #1]
004f7324: eor      r2, r2, r1
004f7328: strb     r2, [r3, #1]
004f732c: ldrb     r1, [r6, #-1]
004f7330: sub      r3, r3, #1
004f7334: eor      r2, r2, r1
004f7338: strb     r2, [r6, #-1]
004f733c: add      r6, r6, #1
004f7340: blo      #0x4f730c
004f7344: add      r6, r4, #0x338
004f7348: mov      r0, r5
004f734c: mov      r1, r6
004f7350: bl       #0x459090
004f7354: mov      r3, #1
004f7358: cmp      r3, #0
004f735c: str      r3, [sp, #4]
004f7360: bne      #0x4f73a4
004f7364: add      r3, r6, #2
004f7368: add      r6, r6, #1
004f736c: ldrb     r1, [r3, #1]
004f7370: ldrb     r2, [r6, #-1]
004f7374: cmp      r3, r6
004f7378: eor      r2, r1, r2
004f737c: strb     r2, [r6, #-1]
004f7380: ldrb     r1, [r3, #1]
004f7384: eor      r2, r2, r1
004f7388: strb     r2, [r3, #1]
004f738c: ldrb     r1, [r6, #-1]
004f7390: sub      r3, r3, #1
004f7394: eor      r2, r2, r1
004f7398: strb     r2, [r6, #-1]
004f739c: add      r6, r6, #1
004f73a0: bhi      #0x4f736c
004f73a4: add      r6, r4, #0x33c
004f73a8: mov      r0, r5
004f73ac: mov      r1, r6
004f73b0: bl       #0x459090
004f73b4: mov      r3, #1
004f73b8: cmp      r3, #0
004f73bc: str      r3, [sp, #4]
004f73c0: bne      #0x4f7404
004f73c4: add      r3, r6, #2
004f73c8: add      r6, r6, #1
004f73cc: ldrb     r1, [r3, #1]
004f73d0: ldrb     r2, [r6, #-1]
004f73d4: cmp      r3, r6
004f73d8: eor      r2, r1, r2
004f73dc: strb     r2, [r6, #-1]
004f73e0: ldrb     r1, [r3, #1]
004f73e4: eor      r2, r2, r1
004f73e8: strb     r2, [r3, #1]
004f73ec: ldrb     r1, [r6, #-1]
004f73f0: sub      r3, r3, #1
004f73f4: eor      r2, r2, r1
004f73f8: strb     r2, [r6, #-1]
004f73fc: add      r6, r6, #1
004f7400: bhi      #0x4f73cc
004f7404: add      r6, r4, #0x340
004f7408: mov      r0, r5
004f740c: mov      r1, r6
004f7410: bl       #0x459090
004f7414: mov      r3, #1
004f7418: cmp      r3, #0
004f741c: str      r3, [sp, #4]
004f7420: bne      #0x4f7464
004f7424: add      r3, r6, #2
004f7428: add      r6, r6, #1
004f742c: ldrb     r1, [r3, #1]
004f7430: ldrb     r2, [r6, #-1]
004f7434: cmp      r6, r3
004f7438: eor      r2, r1, r2
004f743c: strb     r2, [r6, #-1]
004f7440: ldrb     r1, [r3, #1]
004f7444: eor      r2, r2, r1
004f7448: strb     r2, [r3, #1]
004f744c: ldrb     r1, [r6, #-1]
004f7450: sub      r3, r3, #1
004f7454: eor      r2, r2, r1
004f7458: strb     r2, [r6, #-1]
004f745c: add      r6, r6, #1
004f7460: blo      #0x4f742c
004f7464: add      r6, r4, #0x344
004f7468: mov      r0, r5
004f746c: mov      r1, r6
004f7470: bl       #0x459090
004f7474: mov      r3, #1
004f7478: cmp      r3, #0
004f747c: str      r3, [sp, #4]
004f7480: bne      #0x4f74c4
004f7484: add      r3, r6, #2
004f7488: add      r6, r6, #1
004f748c: ldrb     r1, [r3, #1]
004f7490: ldrb     r2, [r6, #-1]
004f7494: cmp      r3, r6
004f7498: eor      r2, r1, r2
004f749c: strb     r2, [r6, #-1]
004f74a0: ldrb     r1, [r3, #1]
004f74a4: eor      r2, r2, r1
004f74a8: strb     r2, [r3, #1]
004f74ac: ldrb     r1, [r6, #-1]
004f74b0: sub      r3, r3, #1
004f74b4: eor      r2, r2, r1
004f74b8: strb     r2, [r6, #-1]
004f74bc: add      r6, r6, #1
004f74c0: bhi      #0x4f748c
004f74c4: add      r6, r4, #0x348
004f74c8: mov      r0, r5
004f74cc: mov      r1, r6
004f74d0: bl       #0x459090
004f74d4: mov      r3, #1
004f74d8: cmp      r3, #0
004f74dc: str      r3, [sp, #4]
004f74e0: bne      #0x4f7524
004f74e4: add      r3, r6, #2
004f74e8: add      r6, r6, #1
004f74ec: ldrb     r1, [r3, #1]
004f74f0: ldrb     r2, [r6, #-1]
004f74f4: cmp      r3, r6
004f74f8: eor      r2, r1, r2
004f74fc: strb     r2, [r6, #-1]
004f7500: ldrb     r1, [r3, #1]
004f7504: eor      r2, r2, r1
004f7508: strb     r2, [r3, #1]
004f750c: ldrb     r1, [r6, #-1]
004f7510: sub      r3, r3, #1
004f7514: eor      r2, r2, r1
004f7518: strb     r2, [r6, #-1]
004f751c: add      r6, r6, #1
004f7520: bhi      #0x4f74ec
004f7524: add      r6, r4, #0x34c
004f7528: mov      r0, r5
004f752c: mov      r1, r6
004f7530: bl       #0x459090
004f7534: mov      r3, #1
004f7538: cmp      r3, #0
004f753c: str      r3, [sp, #4]
004f7540: bne      #0x4f7584
004f7544: add      r3, r6, #2
004f7548: add      r6, r6, #1
004f754c: ldrb     r1, [r3, #1]
004f7550: ldrb     r2, [r6, #-1]
004f7554: cmp      r6, r3
004f7558: eor      r2, r1, r2
004f755c: strb     r2, [r6, #-1]
004f7560: ldrb     r1, [r3, #1]
004f7564: eor      r2, r2, r1
004f7568: strb     r2, [r3, #1]
004f756c: ldrb     r1, [r6, #-1]
004f7570: sub      r3, r3, #1
004f7574: eor      r2, r2, r1
004f7578: strb     r2, [r6, #-1]
004f757c: add      r6, r6, #1
004f7580: blo      #0x4f754c
004f7584: add      r6, r4, #0x350
004f7588: mov      r0, r5
004f758c: mov      r1, r6
004f7590: bl       #0x459090
004f7594: mov      r3, #1
004f7598: cmp      r3, #0
004f759c: str      r3, [sp, #4]
004f75a0: bne      #0x4f75e4
004f75a4: add      r3, r6, #2
004f75a8: add      r6, r6, #1
004f75ac: ldrb     r1, [r3, #1]
004f75b0: ldrb     r2, [r6, #-1]
004f75b4: cmp      r3, r6
004f75b8: eor      r2, r1, r2
004f75bc: strb     r2, [r6, #-1]
004f75c0: ldrb     r1, [r3, #1]
004f75c4: eor      r2, r2, r1
004f75c8: strb     r2, [r3, #1]
004f75cc: ldrb     r1, [r6, #-1]
004f75d0: sub      r3, r3, #1
004f75d4: eor      r2, r2, r1
004f75d8: strb     r2, [r6, #-1]
004f75dc: add      r6, r6, #1
004f75e0: bhi      #0x4f75ac
004f75e4: add      r6, r4, #0x354
004f75e8: mov      r0, r5
004f75ec: mov      r1, r6
004f75f0: bl       #0x459090
004f75f4: mov      r3, #1
004f75f8: cmp      r3, #0
004f75fc: str      r3, [sp, #4]
004f7600: bne      #0x4f7644
004f7604: add      r3, r6, #2
004f7608: add      r6, r6, #1
004f760c: ldrb     r1, [r3, #1]
004f7610: ldrb     r2, [r6, #-1]
004f7614: cmp      r3, r6
004f7618: eor      r2, r1, r2
004f761c: strb     r2, [r6, #-1]
004f7620: ldrb     r1, [r3, #1]
004f7624: eor      r2, r2, r1
004f7628: strb     r2, [r3, #1]
004f762c: ldrb     r1, [r6, #-1]
004f7630: sub      r3, r3, #1
004f7634: eor      r2, r2, r1
004f7638: strb     r2, [r6, #-1]
004f763c: add      r6, r6, #1
004f7640: bhi      #0x4f760c
004f7644: add      r6, r4, #0x358
004f7648: mov      r0, r5
004f764c: mov      r1, r6
004f7650: bl       #0x459090
004f7654: mov      r3, #1
004f7658: cmp      r3, #0
004f765c: str      r3, [sp, #4]
004f7660: bne      #0x4f76a4
004f7664: add      r3, r6, #2
004f7668: add      r6, r6, #1
004f766c: ldrb     r1, [r3, #1]
004f7670: ldrb     r2, [r6, #-1]
004f7674: cmp      r6, r3
004f7678: eor      r2, r1, r2
004f767c: strb     r2, [r6, #-1]
004f7680: ldrb     r1, [r3, #1]
004f7684: eor      r2, r2, r1
004f7688: strb     r2, [r3, #1]
004f768c: ldrb     r1, [r6, #-1]
004f7690: sub      r3, r3, #1
004f7694: eor      r2, r2, r1
004f7698: strb     r2, [r6, #-1]
004f769c: add      r6, r6, #1
004f76a0: blo      #0x4f766c
004f76a4: add      r6, r4, #0x35c
004f76a8: mov      r0, r5
004f76ac: mov      r1, r6
004f76b0: bl       #0x459090
004f76b4: mov      r3, #1
004f76b8: cmp      r3, #0
004f76bc: str      r3, [sp, #4]
004f76c0: bne      #0x4f7704
004f76c4: add      r3, r6, #2
004f76c8: add      r6, r6, #1
004f76cc: ldrb     r1, [r3, #1]
004f76d0: ldrb     r2, [r6, #-1]
004f76d4: cmp      r3, r6
004f76d8: eor      r2, r1, r2
004f76dc: strb     r2, [r6, #-1]
004f76e0: ldrb     r1, [r3, #1]
004f76e4: eor      r2, r2, r1
004f76e8: strb     r2, [r3, #1]
004f76ec: ldrb     r1, [r6, #-1]
004f76f0: sub      r3, r3, #1
004f76f4: eor      r2, r2, r1
004f76f8: strb     r2, [r6, #-1]
004f76fc: add      r6, r6, #1
004f7700: bhi      #0x4f76cc
004f7704: add      r6, r4, #0x360
004f7708: mov      r0, r5
004f770c: mov      r1, r6
004f7710: bl       #0x459090
004f7714: mov      r3, #1
004f7718: cmp      r3, #0
004f771c: str      r3, [sp, #4]
004f7720: bne      #0x4f7764
004f7724: add      r3, r6, #2
004f7728: add      r6, r6, #1
004f772c: ldrb     r1, [r3, #1]
004f7730: ldrb     r2, [r6, #-1]
004f7734: cmp      r3, r6
004f7738: eor      r2, r1, r2
004f773c: strb     r2, [r6, #-1]
004f7740: ldrb     r1, [r3, #1]
004f7744: eor      r2, r2, r1
004f7748: strb     r2, [r3, #1]
004f774c: ldrb     r1, [r6, #-1]
004f7750: sub      r3, r3, #1
004f7754: eor      r2, r2, r1
004f7758: strb     r2, [r6, #-1]
004f775c: add      r6, r6, #1
004f7760: bhi      #0x4f772c
004f7764: add      r6, r4, #0x364
004f7768: mov      r0, r5
004f776c: mov      r1, r6
004f7770: bl       #0x459090
004f7774: mov      r3, #1
004f7778: cmp      r3, #0
004f777c: str      r3, [sp, #4]
004f7780: bne      #0x4f77c4
004f7784: add      r3, r6, #2
004f7788: add      r6, r6, #1
004f778c: ldrb     r1, [r3, #1]
004f7790: ldrb     r2, [r6, #-1]
004f7794: cmp      r6, r3
004f7798: eor      r2, r1, r2
004f779c: strb     r2, [r6, #-1]
004f77a0: ldrb     r1, [r3, #1]
004f77a4: eor      r2, r2, r1
004f77a8: strb     r2, [r3, #1]
004f77ac: ldrb     r1, [r6, #-1]
004f77b0: sub      r3, r3, #1
004f77b4: eor      r2, r2, r1
004f77b8: strb     r2, [r6, #-1]
004f77bc: add      r6, r6, #1
004f77c0: blo      #0x4f778c
004f77c4: add      r6, r4, #0x368
004f77c8: mov      r0, r5
004f77cc: mov      r1, r6
004f77d0: bl       #0x459090
004f77d4: mov      r3, #1
004f77d8: cmp      r3, #0
004f77dc: str      r3, [sp, #4]
004f77e0: bne      #0x4f7824
004f77e4: add      r3, r6, #2
004f77e8: add      r6, r6, #1
004f77ec: ldrb     r1, [r3, #1]
004f77f0: ldrb     r2, [r6, #-1]
004f77f4: cmp      r3, r6
004f77f8: eor      r2, r1, r2
004f77fc: strb     r2, [r6, #-1]
004f7800: ldrb     r1, [r3, #1]
004f7804: eor      r2, r2, r1
004f7808: strb     r2, [r3, #1]
004f780c: ldrb     r1, [r6, #-1]
004f7810: sub      r3, r3, #1
004f7814: eor      r2, r2, r1
004f7818: strb     r2, [r6, #-1]
004f781c: add      r6, r6, #1
004f7820: bhi      #0x4f77ec
004f7824: add      r6, r4, #0x36c
004f7828: mov      r0, r5
004f782c: mov      r1, r6
004f7830: bl       #0x459090
004f7834: mov      r3, #1
004f7838: cmp      r3, #0
004f783c: str      r3, [sp, #4]
004f7840: bne      #0x4f7884
004f7844: add      r3, r6, #2
004f7848: add      r6, r6, #1
004f784c: ldrb     r1, [r3, #1]
004f7850: ldrb     r2, [r6, #-1]
004f7854: cmp      r3, r6
004f7858: eor      r2, r1, r2
004f785c: strb     r2, [r6, #-1]
004f7860: ldrb     r1, [r3, #1]
004f7864: eor      r2, r2, r1
004f7868: strb     r2, [r3, #1]
004f786c: ldrb     r1, [r6, #-1]
004f7870: sub      r3, r3, #1
004f7874: eor      r2, r2, r1
004f7878: strb     r2, [r6, #-1]
004f787c: add      r6, r6, #1
004f7880: bhi      #0x4f784c
004f7884: add      r6, r4, #0x370
004f7888: mov      r0, r5
004f788c: mov      r1, r6
004f7890: bl       #0x459090
004f7894: mov      r3, #1
004f7898: cmp      r3, #0
004f789c: str      r3, [sp, #4]
004f78a0: bne      #0x4f78e4
004f78a4: add      r3, r6, #2
004f78a8: add      r6, r6, #1
004f78ac: ldrb     r1, [r3, #1]
004f78b0: ldrb     r2, [r6, #-1]
004f78b4: cmp      r6, r3
004f78b8: eor      r2, r1, r2
004f78bc: strb     r2, [r6, #-1]
004f78c0: ldrb     r1, [r3, #1]
004f78c4: eor      r2, r2, r1
004f78c8: strb     r2, [r3, #1]
004f78cc: ldrb     r1, [r6, #-1]
004f78d0: sub      r3, r3, #1
004f78d4: eor      r2, r2, r1
004f78d8: strb     r2, [r6, #-1]
004f78dc: add      r6, r6, #1
004f78e0: blo      #0x4f78ac
004f78e4: add      r6, r4, #0x374
004f78e8: mov      r0, r5
004f78ec: mov      r1, r6
004f78f0: bl       #0x459090
004f78f4: mov      r3, #1
004f78f8: cmp      r3, #0
004f78fc: str      r3, [sp, #4]
004f7900: bne      #0x4f7944
004f7904: add      r3, r6, #2
004f7908: add      r6, r6, #1
004f790c: ldrb     r1, [r3, #1]
004f7910: ldrb     r2, [r6, #-1]
004f7914: cmp      r3, r6
004f7918: eor      r2, r1, r2
004f791c: strb     r2, [r6, #-1]
004f7920: ldrb     r1, [r3, #1]
004f7924: eor      r2, r2, r1
004f7928: strb     r2, [r3, #1]
004f792c: ldrb     r1, [r6, #-1]
004f7930: sub      r3, r3, #1
004f7934: eor      r2, r2, r1
004f7938: strb     r2, [r6, #-1]
004f793c: add      r6, r6, #1
004f7940: bhi      #0x4f790c
004f7944: add      r6, r4, #0x378
004f7948: mov      r0, r5
004f794c: mov      r1, r6
004f7950: bl       #0x459090
004f7954: mov      r3, #1
004f7958: cmp      r3, #0
004f795c: str      r3, [sp, #4]
004f7960: bne      #0x4f79a4
004f7964: add      r3, r6, #2
004f7968: add      r6, r6, #1
004f796c: ldrb     r1, [r3, #1]
004f7970: ldrb     r2, [r6, #-1]
004f7974: cmp      r3, r6
004f7978: eor      r2, r1, r2
004f797c: strb     r2, [r6, #-1]
004f7980: ldrb     r1, [r3, #1]
004f7984: eor      r2, r2, r1
004f7988: strb     r2, [r3, #1]
004f798c: ldrb     r1, [r6, #-1]
004f7990: sub      r3, r3, #1
004f7994: eor      r2, r2, r1
004f7998: strb     r2, [r6, #-1]
004f799c: add      r6, r6, #1
004f79a0: bhi      #0x4f796c
004f79a4: add      r6, r4, #0x37c
004f79a8: mov      r0, r5
004f79ac: mov      r1, r6
004f79b0: bl       #0x459090
004f79b4: mov      r3, #1
004f79b8: cmp      r3, #0
004f79bc: str      r3, [sp, #4]
004f79c0: bne      #0x4f7a04
004f79c4: add      r3, r6, #2
004f79c8: add      r6, r6, #1
004f79cc: ldrb     r1, [r3, #1]
004f79d0: ldrb     r2, [r6, #-1]
004f79d4: cmp      r6, r3
004f79d8: eor      r2, r1, r2
004f79dc: strb     r2, [r6, #-1]
004f79e0: ldrb     r1, [r3, #1]
004f79e4: eor      r2, r2, r1
004f79e8: strb     r2, [r3, #1]
004f79ec: ldrb     r1, [r6, #-1]
004f79f0: sub      r3, r3, #1
004f79f4: eor      r2, r2, r1
004f79f8: strb     r2, [r6, #-1]
004f79fc: add      r6, r6, #1
004f7a00: blo      #0x4f79cc
004f7a04: add      r4, r4, #0x380
004f7a08: mov      r0, r5
004f7a0c: mov      r1, r4
004f7a10: bl       #0x459090
004f7a14: mov      r3, #1
004f7a18: cmp      r3, #0
004f7a1c: str      r3, [sp, #4]
004f7a20: bne      #0x4f7a64
004f7a24: add      r3, r4, #2
004f7a28: add      r4, r4, #1
004f7a2c: ldrb     r1, [r3, #1]
004f7a30: ldrb     r2, [r4, #-1]
004f7a34: cmp      r3, r4
004f7a38: eor      r2, r1, r2
004f7a3c: strb     r2, [r4, #-1]
004f7a40: ldrb     r1, [r3, #1]
004f7a44: eor      r2, r2, r1
004f7a48: strb     r2, [r3, #1]
004f7a4c: ldrb     r1, [r4, #-1]
004f7a50: sub      r3, r3, #1
004f7a54: eor      r2, r2, r1
004f7a58: strb     r2, [r4, #-1]
004f7a5c: add      r4, r4, #1
004f7a60: bhi      #0x4f7a2c
004f7a64: add      sp, sp, #8
004f7a68: pop      {r4, r5, r6, pc}

# _ZN6Arrays9ModelDict9readNamesEP11IStreamBase
004b1ac0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b1ac4: mov      r7, r0
004b1ac8: sub      sp, sp, #0x1c
004b1acc: bl       #0x4a4108
004b1ad0: mov      r0, r7
004b1ad4: bl       #0x313a90
004b1ad8: ldr      r6, [pc, #0x16c]
004b1adc: mov      r3, #1
004b1ae0: cmp      r3, #0
004b1ae4: add      r6, pc, r6
004b1ae8: str      r0, [sp, #0x14]
004b1aec: str      r3, [sp, #0xc]
004b1af0: bne      #0x4b1b40
004b1af4: add      r3, sp, #0x14
004b1af8: add      r2, r3, #2
004b1afc: add      r3, r3, #1
004b1b00: ldrb     r0, [r2, #1]
004b1b04: ldrb     r1, [r3, #-1]
004b1b08: cmp      r2, r3
004b1b0c: mov      r4, r2
004b1b10: eor      r1, r0, r1
004b1b14: strb     r1, [r3, #-1]
004b1b18: ldrb     r0, [r2, #1]
004b1b1c: eor      r1, r1, r0
004b1b20: strb     r1, [r2, #1]
004b1b24: ldrb     r0, [r3, #-1]
004b1b28: sub      r2, r2, #1
004b1b2c: eor      r1, r1, r0
004b1b30: strb     r1, [r3, #-1]
004b1b34: add      r3, r3, #1
004b1b38: bhi      #0x4b1b00
004b1b3c: ldr      r0, [sp, #0x14]
004b1b40: ldr      r3, [pc, #0x108]
004b1b44: ldr      r3, [r6, r3]
004b1b48: ldr      r3, [r3]
004b1b4c: cmp      r3, r0
004b1b50: beq      #0x4b1b5c
004b1b54: add      sp, sp, #0x1c
004b1b58: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b1b5c: lsl      r0, r0, #2
004b1b60: mov      r1, #1
004b1b64: bl       #0x31056c
004b1b68: ldr      sb, [pc, #0xe4]
004b1b6c: ldr      r2, [sp, #0x14]
004b1b70: ldr      r3, [r6, sb]
004b1b74: cmp      r2, #0
004b1b78: str      r0, [r3]
004b1b7c: beq      #0x4b1b54
004b1b80: add      sl, sp, #0x10
004b1b84: mov      r8, #1
004b1b88: add      r1, sl, r8
004b1b8c: add      r3, sl, #2
004b1b90: mov      r4, #0
004b1b94: stm      sp, {r1, r3}
004b1b98: mov      r0, r7
004b1b9c: mov      r1, sl
004b1ba0: bl       #0x3df1a0
004b1ba4: cmp      r8, #0
004b1ba8: str      r8, [sp, #0xc]
004b1bac: bne      #0x4b1bf0
004b1bb0: ldr      r3, [sp]
004b1bb4: ldr      r2, [sp, #4]
004b1bb8: ldrb     r0, [r2, #1]
004b1bbc: ldrb     r1, [r3, #-1]
004b1bc0: cmp      r2, r3
004b1bc4: eor      r1, r0, r1
004b1bc8: strb     r1, [r3, #-1]
004b1bcc: ldrb     r0, [r2, #1]
004b1bd0: eor      r1, r1, r0
004b1bd4: strb     r1, [r2, #1]
004b1bd8: ldrb     r0, [r3, #-1]
004b1bdc: sub      r2, r2, #1
004b1be0: eor      r1, r1, r0
004b1be4: strb     r1, [r3, #-1]
004b1be8: add      r3, r3, #1
004b1bec: bhi      #0x4b1bb8
004b1bf0: ldr      r0, [sp, #0x10]
004b1bf4: ldr      r5, [r6, sb]
004b1bf8: mov      r1, #1
004b1bfc: add      r0, r0, r1
004b1c00: ldr      fp, [r5]
004b1c04: bl       #0x31056c
004b1c08: str      r0, [fp, r4, lsl #2]
004b1c0c: ldr      r3, [r5]
004b1c10: ldr      r2, [sp, #0x10]
004b1c14: mov      r0, r7
004b1c18: ldr      r1, [r3, r4, lsl #2]
004b1c1c: mov      r3, #0
004b1c20: bl       #0x317454
004b1c24: ldr      r3, [r5]
004b1c28: mov      r1, #0
004b1c2c: ldr      r2, [r3, r4, lsl #2]
004b1c30: ldr      r3, [sp, #0x10]
004b1c34: add      r4, r4, #1
004b1c38: strb     r1, [r2, r3]
004b1c3c: ldr      r3, [sp, #0x14]
004b1c40: cmp      r3, r4
004b1c44: bhi      #0x4b1b98
004b1c48: b        #0x4b1b54
004b1c4c: subeq    r2, lr, ip, lsr #31
004b1c50: andeq    r3, r0, ip, lsl #24
004b1c54: andeq    r3, r0, r8, ror #4

# _ZN6Arrays14CharacterTable4readEP11IStreamBase
004b4340: push     {r4, r5, r6, r7, r8, sl, lr}
004b4344: sub      sp, sp, #0xc
004b4348: mov      sl, r0
004b434c: bl       #0x313a90
004b4350: ldr      r6, [pc, #0x11c]
004b4354: mov      r3, #1
004b4358: cmp      r3, #0
004b435c: str      r0, [sp, #4]
004b4360: str      r3, [sp]
004b4364: add      r6, pc, r6
004b4368: bne      #0x4b43b0
004b436c: add      r3, sp, #4
004b4370: add      r2, r3, #2
004b4374: add      r3, r3, #1
004b4378: ldrb     r0, [r2, #1]
004b437c: ldrb     r1, [r3, #-1]
004b4380: cmp      r2, r3
004b4384: eor      r1, r0, r1
004b4388: strb     r1, [r3, #-1]
004b438c: ldrb     r0, [r2, #1]
004b4390: eor      r1, r1, r0
004b4394: strb     r1, [r2, #1]
004b4398: ldrb     r0, [r3, #-1]
004b439c: sub      r2, r2, #1
004b43a0: eor      r1, r1, r0
004b43a4: strb     r1, [r3, #-1]
004b43a8: add      r3, r3, #1
004b43ac: bhi      #0x4b4378
004b43b0: bl       #0x4a9838
004b43b4: ldr      r7, [pc, #0xbc]
004b43b8: ldr      r4, [sp, #4]
004b43bc: mov      r5, #0x384
004b43c0: ldr      r3, [r6, r7]
004b43c4: mul      r0, r5, r4
004b43c8: str      r4, [r3]
004b43cc: add      r0, r0, #8
004b43d0: mov      r1, #1
004b43d4: bl       #0x31056c
004b43d8: cmp      r4, #0
004b43dc: str      r5, [r0]
004b43e0: str      r4, [r0, #4]
004b43e4: add      r3, r0, #8
004b43e8: beq      #0x4b4410
004b43ec: ldr      r1, [pc, #0x88]
004b43f0: mov      r2, #0
004b43f4: ldr      r1, [r6, r1]
004b43f8: add      r1, r1, #8
004b43fc: add      r2, r2, #1
004b4400: cmp      r2, r4
004b4404: str      r1, [r0, #8]
004b4408: add      r0, r0, #0x384
004b440c: bne      #0x4b43fc
004b4410: ldr      r2, [r6, r7]
004b4414: ldr      r8, [pc, #0x64]
004b4418: ldr      r1, [r2]
004b441c: ldr      r2, [r6, r8]
004b4420: cmp      r1, #0
004b4424: str      r3, [r2]
004b4428: beq      #0x4b446c
004b442c: mov      r4, #0
004b4430: mov      r5, r4
004b4434: b        #0x4b4440
004b4438: ldr      r3, [r6, r8]
004b443c: ldr      r3, [r3]
004b4440: add      r0, r3, r4
004b4444: mov      r1, sl
004b4448: ldr      r3, [r3, r4]
004b444c: mov      lr, pc
004b4450: ldr      pc, [r3, #0xc]
004b4454: ldr      r3, [r6, r7]
004b4458: add      r5, r5, #1
004b445c: add      r4, r4, #0x384
004b4460: ldr      r3, [r3]
004b4464: cmp      r3, r5
004b4468: bhi      #0x4b4438
004b446c: add      sp, sp, #0xc
004b4470: pop      {r4, r5, r6, r7, r8, sl, pc}
004b4474: subeq    r0, lr, ip, lsr #14
004b4478: andeq    r4, r0, r4, lsl #4
004b447c: andeq    r2, r0, ip, lsl #19
004b4480: andeq    r2, r0, r0, asr fp

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str      lr, [sp, #-4]!
00459094: mov      r3, #0
00459098: sub      sp, sp, #0xc
0045909c: ldr      ip, [r0]
004590a0: mov      r2, #4
004590a4: mov      lr, pc
004590a8: ldr      pc, [ip, #0x18]
004590ac: ldr      r3, [pc, #0x74]
004590b0: cmp      r0, #4
004590b4: add      r3, pc, r3
004590b8: beq      #0x4590e8
004590bc: ldr      r2, [pc, #0x68]
004590c0: ldr      r2, [r3, r2]
004590c4: ldr      r2, [r2]
004590c8: cmp      r2, #2
004590cc: moveq    r3, #0
004590d0: streq    r3, [r3]
004590d4: beq      #0x4590e0
004590d8: cmp      r2, #1
004590dc: beq      #0x4590f4
004590e0: add      sp, sp, #0xc
004590e4: ldm      sp!, {pc}
004590e8: cmp      r1, #0
004590ec: beq      #0x4590e0
004590f0: b        #0x4590bc
004590f4: ldr      r0, [pc, #0x34]
004590f8: ldr      r1, [pc, #0x34]
004590fc: ldr      r2, [pc, #0x34]
00459100: ldr      r0, [r3, r0]
00459104: ldr      r3, [pc, #0x30]
00459108: mov      ip, #0x50
0045910c: add      r1, pc, r1
00459110: add      r2, pc, r2
00459114: add      r3, pc, r3
00459118: add      r0, r0, #0xa8
0045911c: str      ip, [sp]
00459120: bl       #0x30e004
00459124: b        #0x4590e0
00459128: ldrsbeq  fp, [r3], #-0x9c
0045912c: andeq    r3, r0, r0, asr #19
00459130: andeq    r1, r0, r0, asr #19
00459134: subeq    r5, r6, ip, asr #5
00459138: strdeq   r5, r6, [r6], #-0x30
0045913c: subeq    r6, r6, ip, lsr #24
