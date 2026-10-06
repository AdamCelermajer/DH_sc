# _ZN9Character11_InitSoundsEv
003b3b00 push     {r4, r5, r6, r7, r8, lr}
003b3b04 bl       #0x3a32d0
003b3b08 ldr      r6, [pc, #0xf0]
003b3b0c ldr      r7, [pc, #0xf0]
003b3b10 mov      r4, r0
003b3b14 add      r6, pc, r6
003b3b18 ldr      r3, [r6, r7]
003b3b1c ldr      r0, [r3]
003b3b20 cmp      r0, #0
003b3b24 beq      #0x3b3bfc
003b3b28 ldr      r3, [r4, #0xc]
003b3b2c cmp      r3, #0
003b3b30 beq      #0x3b3b60
003b3b34 mov      r5, #0
003b3b38 b        #0x3b3b44
003b3b3c ldr      r3, [r6, r7]
003b3b40 ldr      r0, [r3]
003b3b44 ldr      r3, [r4, #0x10]
003b3b48 ldr      r1, [r3, r5, lsl #2]
003b3b4c bl       #0x3699fc
003b3b50 ldr      r3, [r4, #0xc]
003b3b54 add      r5, r5, #1
003b3b58 cmp      r3, r5
003b3b5c bhi      #0x3b3b3c
003b3b60 ldr      r3, [r4, #4]
003b3b64 cmp      r3, #0
003b3b68 beq      #0x3b3b94
003b3b6c ldr      r8, [r6, r7]
003b3b70 mov      r5, #0
003b3b74 ldr      r3, [r4, #8]
003b3b78 ldr      r0, [r8]
003b3b7c ldr      r1, [r3, r5, lsl #2]
003b3b80 bl       #0x3699fc
003b3b84 ldr      r3, [r4, #4]
003b3b88 add      r5, r5, #1
003b3b8c cmp      r3, r5
003b3b90 bhi      #0x3b3b74
003b3b94 ldr      r3, [r4, #0x14]
003b3b98 cmp      r3, #0
003b3b9c beq      #0x3b3bc8
003b3ba0 ldr      r8, [r6, r7]
003b3ba4 mov      r5, #0
003b3ba8 ldr      r3, [r4, #0x18]
003b3bac ldr      r0, [r8]
003b3bb0 ldr      r1, [r3, r5, lsl #2]
003b3bb4 bl       #0x3699fc
003b3bb8 ldr      r3, [r4, #0x14]
003b3bbc add      r5, r5, #1
003b3bc0 cmp      r3, r5
003b3bc4 bhi      #0x3b3ba8
003b3bc8 ldr      r3, [r4, #0x1c]
003b3bcc cmp      r3, #0
003b3bd0 beq      #0x3b3bfc
003b3bd4 ldr      r6, [r6, r7]
003b3bd8 mov      r5, #0
003b3bdc ldr      r3, [r4, #0x20]
003b3be0 ldr      r0, [r6]
003b3be4 ldr      r1, [r3, r5, lsl #2]
003b3be8 bl       #0x3699fc
003b3bec ldr      r3, [r4, #0x1c]
003b3bf0 add      r5, r5, #1
003b3bf4 cmp      r3, r5
003b3bf8 bhi      #0x3b3bdc
003b3bfc pop      {r4, r5, r6, r7, r8, pc}
003b3c00 subseq   r0, lr, ip, ror pc
003b3c04 andeq    r0, r0, r4, lsr #27
