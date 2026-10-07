# _ZN13TriggerObject8InteractEP10GameObject
00399af0 push     {r4, r5, r6, r7, r8, lr}
00399af4 ldr      r3, [r0, #0x300]
00399af8 ldr      r5, [pc, #0x22c]
00399afc sub      sp, sp, #0x40
00399b00 cmp      r3, #0
00399b04 mov      r4, r0
00399b08 mov      r7, r1
00399b0c add      r5, pc, r5
00399b10 beq      #0x399b20
00399b14 bl       #0x3987ac
00399b18 cmp      r0, #0
00399b1c bne      #0x399ca0
00399b20 mov      r0, r4
00399b24 bl       #0x398794
00399b28 ldr      r3, [r4, #0x2d8]
00399b2c cmp      r3, #0
00399b30 beq      #0x399b5c
00399b34 ldr      ip, [r3, #0x38]
00399b38 ldr      r1, [pc, #0x1f0]
00399b3c mov      r3, #0
00399b40 mov      r0, ip
00399b44 mov      r2, r3
00399b48 ldr      ip, [ip]
00399b4c add      r1, pc, r1
00399b50 str      r3, [sp]
00399b54 mov      lr, pc
00399b58 ldr      pc, [ip, #0x20]
00399b5c ldr      r3, [r4, #0x730]
00399b60 cmn      r3, #1
00399b64 beq      #0x399bc8
00399b68 ldr      r2, [pc, #0x1c4]
00399b6c ldr      r1, [pc, #0x1c4]
00399b70 ldr      lr, [r4, #0x168]
00399b74 ldr      r2, [r5, r2]
00399b78 ldr      r1, [r5, r1]
00399b7c ldr      r6, [r4, #0x164]
00399b80 ldr      r2, [r2]
00399b84 ldr      r0, [r1]
00399b88 mov      r1, #0x18
00399b8c mla      r3, r1, r3, r2
00399b90 ldr      r7, [r4, #0x160]
00399b94 mov      ip, #0xbf000000
00399b98 ldr      r1, [r3, #0x10]
00399b9c add      ip, ip, #0x800000
00399ba0 str      lr, [sp, #0x34]
00399ba4 add      r2, sp, #0x2c
00399ba8 mov      lr, #1
00399bac mov      r3, #0
00399bb0 str      r7, [sp, #0x2c]
00399bb4 str      r6, [sp, #0x30]
00399bb8 str      lr, [sp]
00399bbc str      ip, [sp, #8]
00399bc0 str      ip, [sp, #4]
00399bc4 bl       #0x36b5d8
00399bc8 ldr      r1, [r4, #0x768]
00399bcc cmn      r1, #1
00399bd0 beq      #0x399bf8
00399bd4 ldr      r3, [r4, #0x3b4]
00399bd8 tst      r3, #1
00399bdc beq      #0x399bf8
00399be0 ldr      r3, [pc, #0x154]
00399be4 ldr      r2, [r4, #0x64]
00399be8 ldr      r0, [r5, r3]
00399bec mov      r3, #0
00399bf0 bl       #0x4605c0
00399bf4 b        #0x399c98
00399bf8 ldr      r1, [r4, #0x74c]
00399bfc cmn      r1, #1
00399c00 beq      #0x399c18
00399c04 ldr      r3, [pc, #0x130]
00399c08 ldr      r2, [r4, #0x64]
00399c0c ldr      r0, [r5, r3]
00399c10 mov      r3, #0
00399c14 bl       #0x4605c0
00399c18 ldr      r6, [pc, #0x120]
00399c1c ldr      r0, [r5, r6]
00399c20 bl       #0x31f594
00399c24 subs     r7, r0, #0
00399c28 beq      #0x399cd8
00399c2c ldr      r3, [r5, r6]
00399c30 ldr      r1, [pc, #0x10c]
00399c34 ldr      r2, [pc, #0x10c]
00399c38 ldr      r0, [r3, #0x2c]
00399c3c add      r1, pc, r1
00399c40 add      r2, pc, r2
00399c44 ldr      r6, [r4, #0x730]
00399c48 ldr      r8, [r4, #0x64]
00399c4c bl       #0x4c4bdc
00399c50 ldr      r2, [pc, #0xf4]
00399c54 add      r1, sp, #0x40
00399c58 mov      r3, #0
00399c5c ldr      r2, [r5, r2]
00399c60 str      r0, [sp, #0x14]
00399c64 mov      r0, r7
00399c68 add      r2, r2, #8
00399c6c str      r2, [r1, #-0x30]!
00399c70 mvn      r2, #0
00399c74 strb     r3, [sp, #0x21]
00399c78 str      r3, [sp, #0x18]
00399c7c strb     r3, [sp, #0x20]
00399c80 str      r8, [sp, #0x1c]
00399c84 str      r2, [sp, #0x24]
00399c88 str      r6, [sp, #0x28]
00399c8c bl       #0x339090
00399c90 mov      r3, #1
00399c94 strb     r3, [r4, #0x373]
00399c98 add      sp, sp, #0x40
00399c9c pop      {r4, r5, r6, r7, r8, pc}
00399ca0 add      r6, sp, #0x38
00399ca4 mov      r0, r6
00399ca8 bl       #0x3192b4
00399cac mov      r0, r6
00399cb0 mov      r1, r7
00399cb4 bl       #0x386f28
00399cb8 ldr      r1, [pc, #0x90]
00399cbc ldr      r0, [r4, #0x300]
00399cc0 mov      r2, r6
00399cc4 add      r1, pc, r1
00399cc8 bl       #0x37c41c
00399ccc mov      r0, r6
00399cd0 bl       #0x319228
00399cd4 b        #0x399b20
00399cd8 ldr      r3, [pc, #0x74]
00399cdc ldr      r3, [r5, r3]
00399ce0 ldr      r3, [r3]
00399ce4 cmp      r3, #2
00399ce8 streq    r7, [r7]
00399cec beq      #0x399c2c
00399cf0 cmp      r3, #1
00399cf4 bne      #0x399c2c
00399cf8 ldr      r0, [pc, #0x58]
00399cfc ldr      r1, [pc, #0x58]
00399d00 ldr      r2, [pc, #0x58]
00399d04 ldr      r0, [r5, r0]
00399d08 ldr      r3, [pc, #0x54]
00399d0c movw     ip, #0x11e
00399d10 add      r1, pc, r1
00399d14 add      r2, pc, r2
00399d18 add      r3, pc, r3
00399d1c add      r0, r0, #0xa8
00399d20 str      ip, [sp]
00399d24 bl       #0x30e004
00399d28 b        #0x399c2c
00399d2c subseq   sl, pc, r4, lsl #31
00399d30 subseq   r8, r2, ip, lsl #31
00399d34 andeq    r0, r0, ip, lsl lr
00399d38 andeq    r0, r0, r4, lsr #27
00399d3c andeq    r1, r0, r0, lsr #20
00399d40 strdeq   r3, r4, [r0], -r4
00399d44 subseq   r8, r2, ip, lsr #26
00399d48 subseq   r8, r2, r8, lsl #30
00399d4c andeq    r3, r0, r8, lsr #13
00399d50 subseq   r8, r2, r4, lsr #28
00399d54 andeq    r3, r0, r0, asr #19
00399d58 andeq    r1, r0, r0, asr #19
00399d5c subseq   r4, r2, r8, asr #13
00399d60 subseq   r5, r7, r4, asr #24
00399d64 subseq   r8, r2, r0, ror #27
