# _ZN18NetStructByteArrayILj3EE9SetBufferEPKvi 370258 80
00370258 push {r4, r5, r6, lr}
0037025c ldr ip, [r0]
00370260 sub sp, sp, #8
00370264 mov r3, #0
00370268 mov r5, r0
0037026c mov r0, sp
00370270 ldr r6, [ip, #0x1c]
00370274 str r3, [sp, #4]
00370278 str r3, [sp]
0037027c bl #0x36ddac
00370280 mov r0, r5
00370284 mov r1, sp
00370288 blx r6
0037028c ldr r0, [sp]
00370290 mov r4, sp
00370294 cmp r0, #0
00370298 beq #0x3702a0
0037029c bl #0x310440
003702a0 add sp, sp, #8
003702a4 pop {r4, r5, r6, pc}
# _ZN18NetStructByteArrayILj30EE9SetBufferEPKvi 3702a8 80
003702a8 push {r4, r5, r6, lr}
003702ac ldr ip, [r0]
003702b0 sub sp, sp, #8
003702b4 mov r3, #0
003702b8 mov r5, r0
003702bc mov r0, sp
003702c0 ldr r6, [ip, #0x1c]
003702c4 str r3, [sp, #4]
003702c8 str r3, [sp]
003702cc bl #0x36ddac
003702d0 mov r0, r5
003702d4 mov r1, sp
003702d8 blx r6
003702dc ldr r0, [sp]
003702e0 mov r4, sp
003702e4 cmp r0, #0
003702e8 beq #0x3702f0
003702ec bl #0x310440
003702f0 add sp, sp, #8
003702f4 pop {r4, r5, r6, pc}
# _ZN13PlayerManager17_ManageCharactersEv 37280c 5072
0037280c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00372810 ldr r8, [pc, #0xeac]
00372814 ldr r1, [pc, #0xeac]
00372818 ldr r2, [pc, #0xeac]
0037281c add r8, pc, r8
00372820 ldr r3, [r8, r1]
00372824 sub sp, sp, #0x1d4
00372828 mov r7, r0
0037282c ldr r3, [r3]
00372830 ldr r0, [r8, r2]
00372834 str r1, [sp, #0x1c]
00372838 str r2, [sp, #0x18]
0037283c str r3, [sp, #0x1cc]
00372840 bl #0x31f594
00372844 str r0, [sp, #0x24]
00372848 mov r0, r7
0037284c bl #0x36d7a8
00372850 subs fp, r0, #0
00372854 ble #0x373a94
00372858 ldr r3, [pc, #0xe70]
0037285c ldr lr, [pc, #0xe70]
00372860 mov r0, #0
00372864 add r3, pc, r3
00372868 str r3, [sp, #0x30]
0037286c ldr r3, [pc, #0xe64]
00372870 mov r1, #1
00372874 str lr, [sp, #0x2c]
00372878 add r3, pc, r3
0037287c str r3, [sp, #0x34]
00372880 ldr r3, [pc, #0xe54]
00372884 str r0, [sp, #0x20]
00372888 mov r6, r0
0037288c add r3, pc, r3
00372890 str r3, [sp, #0x38]
00372894 ldr r3, [pc, #0xe44]
00372898 str r0, [sp, #0xc]
0037289c str r1, [sp, #0x10]
003728a0 add r3, pc, r3
003728a4 str r3, [sp, #0x3c]
003728a8 mov r1, r6
003728ac mov r2, #0
003728b0 mov r0, r7
003728b4 bl #0x36e744
003728b8 ldr r3, [r0]
003728bc mov r4, r0
003728c0 ldr r5, [r0, #0x660]
003728c4 mov lr, pc
003728c8 ldr pc, [r3, #0x5c]
003728cc cmp r0, #0
003728d0 beq #0x372ec0
003728d4 ldrb r3, [r4, #0x66c]
003728d8 cmp r3, #0
003728dc beq #0x372f50
003728e0 ldr sl, [r4, #0x664]
003728e4 cmn sl, #1
003728e8 beq #0x372b6c
003728ec ldr sb, [r4, #0x680]
003728f0 cmp sb, #0
003728f4 beq #0x3739d8
003728f8 bl #0x32bd08
003728fc ldrb r3, [r0, #0x11]
00372900 cmp r3, #0
00372904 beq #0x372f8c
00372908 add r2, sp, #0x1b4
0037290c mov r0, r2
00372910 add r1, r4, #0x2d0
00372914 str r2, [sp, #0x14]
00372918 bl #0x32b918
0037291c bl #0x32bd08
00372920 add sl, sp, #0x19c
00372924 mov r1, r0
00372928 mov r0, sl
0037292c bl #0x4995f4
00372930 ldr r0, [sp, #0x1c8]
00372934 ldr r1, [sp, #0x1b0]
00372938 ldr r2, [sp, #0x1c4]
0037293c ldr r3, [sp, #0x1ac]
00372940 rsb r2, r0, r2
00372944 rsb r3, r1, r3
00372948 cmp r2, r3
0037294c beq #0x373618
00372950 mov r0, sl
00372954 bl #0x318254
00372958 ldr r0, [sp, #0x14]
0037295c bl #0x318254
00372960 bl #0x32bd08
00372964 add sl, sp, #0x184
00372968 mov r1, r0
0037296c mov r0, sl
00372970 bl #0x4995f4
00372974 mov r0, r4
00372978 mov r1, sl
0037297c bl #0x371ccc
00372980 mov r0, sl
00372984 bl #0x318254
00372988 cmp r5, #0
0037298c beq #0x373028
00372990 ldr ip, [sp, #0x18]
00372994 ldr r0, [r8, ip]
00372998 bl #0x31f594
0037299c subs sl, r0, #0
003729a0 beq #0x3729b4
003729a4 ldr r3, [sl, #0x130]
003729a8 cmp r3, #0x26
003729ac ldrbeq r3, [sl, #0x144]
003729b0 beq #0x3729b8
003729b4 mov r3, #0
003729b8 ldrb r2, [r4, #0x4e5]
003729bc cmp r2, r3
003729c0 beq #0x372a78
003729c4 ldr ip, [pc, #0xd18]
003729c8 ldrb r1, [sp, #0xa5]
003729cc mov r2, #0xb0000000
003729d0 ldr ip, [r8, ip]
003729d4 cmp r1, r3
003729d8 asr r2, r2, #0x16
003729dc mov r0, #0
003729e0 mov r1, #0
003729e4 add lr, sp, #0x1d0
003729e8 mvn sb, #0
003729ec strd r0, r1, [lr, r2]
003729f0 add ip, ip, #8
003729f4 mov r2, #1
003729f8 mov r0, #0
003729fc mov r1, #0
00372a00 str sb, [sp, #0x9c]
00372a04 str sb, [sp, #0x98]
00372a08 str r2, [sp, #0x8c]
00372a0c strb r0, [sp, #0xa4]
00372a10 str ip, [sp, #0x88]
00372a14 str r1, [sp, #0xa0]
00372a18 addeq sb, sp, #0x88
00372a1c beq #0x372a30
00372a20 add sb, sp, #0x88
00372a24 mov r0, sb
00372a28 strb r3, [sp, #0xa5]
00372a2c bl #0x814f84
00372a30 ldr r3, [pc, #0xcb0]
00372a34 add r0, r4, #0x4c0
00372a38 add r1, sb, #0x1d
00372a3c ldr r3, [r8, r3]
00372a40 add r0, r0, #8
00372a44 add r3, r3, #8
00372a48 str r3, [sp, #0x88]
00372a4c ldr r3, [r4, #0x4c8]
00372a50 mov lr, pc
00372a54 ldr pc, [r3, #0x1c]
00372a58 ldr r3, [pc, #0xc8c]
00372a5c ldr r2, [pc, #0xc8c]
00372a60 mov r1, #0x7d0
00372a64 ldr r3, [r8, r3]
00372a68 add r2, pc, r2
00372a6c str r1, [r2]
00372a70 add r3, r3, #8
00372a74 str r3, [sp, #0x88]
00372a78 cmp sl, #0
00372a7c beq #0x372a90
00372a80 ldr sl, [sl, #0x130]
00372a84 cmp sl, #0x23
00372a88 movle sl, #0
00372a8c movgt sl, #1
00372a90 ldrb r3, [r4, #0x525]
00372a94 cmp r3, sl
00372a98 beq #0x372b4c
00372a9c bl #0x7fd794
00372aa0 ldrb r3, [r0, #5]
00372aa4 cmp r3, #0
00372aa8 bne #0x3735e4
00372aac ldr r0, [pc, #0xc30]
00372ab0 ldrb r2, [sp, #0x85]
00372ab4 mov r3, #0xa8000000
00372ab8 ldr r0, [r8, r0]
00372abc asr r3, r3, #0x16
00372ac0 mov r1, #0
00372ac4 add sb, r0, #8
00372ac8 add ip, sp, #0x1d0
00372acc mov r0, #0
00372ad0 cmp r2, sl
00372ad4 mvn lr, #0
00372ad8 mov r2, #0
00372adc strd r0, r1, [ip, r3]
00372ae0 mov r3, #1
00372ae4 str sb, [sp, #0x68]
00372ae8 str r3, [sp, #0x6c]
00372aec str lr, [sp, #0x7c]
00372af0 strb r2, [sp, #0x84]
00372af4 str lr, [sp, #0x78]
00372af8 str r2, [sp, #0x80]
00372afc addeq sb, sp, #0x68
00372b00 beq #0x372b14
00372b04 add sb, sp, #0x68
00372b08 mov r0, sb
00372b0c strb sl, [sp, #0x85]
00372b10 bl #0x814f84
00372b14 ldr r3, [pc, #0xbcc]
00372b18 add r0, r4, #0x500
00372b1c add r0, r0, #8
00372b20 ldr r3, [r8, r3]
00372b24 add r1, sb, #0x1d
00372b28 add r3, r3, #8
00372b2c str r3, [sp, #0x68]
00372b30 ldr r3, [r4, #0x508]
00372b34 mov lr, pc
00372b38 ldr pc, [r3, #0x1c]
00372b3c ldr r3, [pc, #0xba8]
00372b40 ldr r3, [r8, r3]
00372b44 add r3, r3, #8
00372b48 str r3, [sp, #0x68]
00372b4c bl #0x7fd794
00372b50 ldrb r3, [r0, #5]
00372b54 cmp r3, #0
00372b58 bne #0x3730b4
00372b5c bl #0x7fd794
00372b60 ldrb r3, [r0, #5]
00372b64 cmp r3, #0
00372b68 bne #0x373120
00372b6c ldrb r3, [r7, #0x6c9]
00372b70 cmp r3, #0
00372b74 beq #0x372ea0
00372b78 cmp r5, #0
00372b7c beq #0x37305c
00372b80 bl #0x7fd794
00372b84 ldrb r3, [r0, #5]
00372b88 cmp r3, #0
00372b8c beq #0x372ea0
00372b90 ldrb r3, [r4, #0x66c]
00372b94 cmp r3, #0
00372b98 beq #0x3731cc
00372b9c mov r1, r4
00372ba0 mov r0, r7
00372ba4 bl #0x370020
00372ba8 ldr r1, [r5, #0x39c]
00372bac ldr r3, [r4, #0x498]
00372bb0 cmp r3, r1
00372bb4 beq #0x372bc0
00372bb8 mov r0, r4
00372bbc bl #0x36fe04
00372bc0 add sl, r5, #0x37c
00372bc4 mov r0, sl
00372bc8 ldr sb, [r4, #0x358]
00372bcc bl #0x3fc690
00372bd0 cmp r0, sb
00372bd4 beq #0x372bec
00372bd8 mov r0, sl
00372bdc bl #0x3fc690
00372be0 mov r1, r0
00372be4 mov r0, r4
00372be8 bl #0x36fcb0
00372bec add sb, r5, #0x560
00372bf0 mov r0, sb
00372bf4 mov r1, #0x21
00372bf8 mov r2, #0
00372bfc ldr sl, [r4, #0x448]
00372c00 bl #0x3df6e0
00372c04 cmp r0, sl
00372c08 beq #0x372c28
00372c0c mov r1, #0x21
00372c10 mov r0, sb
00372c14 mov r2, #0
00372c18 bl #0x3df6e0
00372c1c mov r1, r0
00372c20 mov r0, r4
00372c24 bl #0x36fd58
00372c28 mov r0, sb
00372c2c mov r1, #0x24
00372c30 mov r2, #0
00372c34 ldr sl, [r4, #0x470]
00372c38 bl #0x3df6e0
00372c3c cmp r0, sl
00372c40 beq #0x372cf8
00372c44 mov r1, #0x24
00372c48 mov r2, #0
00372c4c mov r0, sb
00372c50 bl #0x3df6e0
00372c54 ldr ip, [pc, #0xa98]
00372c58 ldr r1, [sp, #0x60]
00372c5c mov r2, #0x9e000000
00372c60 ldr ip, [r8, ip]
00372c64 cmp r0, r1
00372c68 asr r2, r2, #0x16
00372c6c mov r1, #0
00372c70 mov r3, r0
00372c74 add lr, sp, #0x1d0
00372c78 mov r0, #0
00372c7c mvn sl, #0
00372c80 strd r0, r1, [lr, r2]
00372c84 add ip, ip, #8
00372c88 mov r2, #0x20
00372c8c mov r0, #0
00372c90 mov r1, #0
00372c94 str sl, [sp, #0x54]
00372c98 str sl, [sp, #0x50]
00372c9c str r2, [sp, #0x44]
00372ca0 strb r0, [sp, #0x5c]
00372ca4 str ip, [sp, #0x40]
00372ca8 str r1, [sp, #0x58]
00372cac addeq sl, sp, #0x40
00372cb0 beq #0x372cc4
00372cb4 add sl, sp, #0x40
00372cb8 mov r0, sl
00372cbc str r3, [sp, #0x60]
00372cc0 bl #0x814f84
00372cc4 ldr r3, [pc, #0xa2c]
00372cc8 add r1, sl, #0x20
00372ccc add r0, r4, #0x450
00372cd0 ldr r3, [r8, r3]
00372cd4 add r3, r3, #8
00372cd8 str r3, [sp, #0x40]
00372cdc ldr r3, [r4, #0x450]
00372ce0 mov lr, pc
00372ce4 ldr pc, [r3, #0x1c]
00372ce8 ldr r3, [pc, #0x9fc]
00372cec ldr r3, [r8, r3]
00372cf0 add r3, r3, #8
00372cf4 str r3, [sp, #0x40]
00372cf8 mov r0, sb
00372cfc mov r1, #0x13
00372d00 mov r2, #0
00372d04 ldr sl, [r4, #0x330]
00372d08 bl #0x3df6e0
00372d0c cmp r0, sl
00372d10 beq #0x372d30
00372d14 mov r1, #0x13
00372d18 mov r0, sb
00372d1c mov r2, #0
00372d20 bl #0x3df6e0
00372d24 mov r1, r0
00372d28 mov r0, r4
00372d2c bl #0x370e48
00372d30 mov r0, r5
00372d34 ldr sl, [r4, #0x380]
00372d38 bl #0x3bb7fc
00372d3c cmp r0, sl
00372d40 beq #0x372d58
00372d44 mov r0, r5
00372d48 bl #0x3bb7fc
00372d4c mov r1, r0
00372d50 mov r0, r4
00372d54 bl #0x370ef0
00372d58 mov r1, #0
00372d5c mov r0, r5
00372d60 bl #0x3bbe68
00372d64 mov r1, #1
00372d68 strb r0, [sp, #0xc4]
00372d6c mov r0, r5
00372d70 bl #0x3bbe68
00372d74 mov r1, #2
00372d78 strb r0, [sp, #0xc5]
00372d7c mov r0, r5
00372d80 bl #0x3bbe68
00372d84 movw sl, #0x14e8
00372d88 strb r0, [sp, #0xc6]
00372d8c add r1, sp, #0xc4
00372d90 add r0, r4, #0x3d8
00372d94 mov r2, #3
00372d98 bl #0x370258
00372d9c ldr r3, [r5, sl]
00372da0 cmp r3, #0
00372da4 beq #0x372dd8
00372da8 ldr r2, [r3, #0x84]
00372dac cmp r2, #0x1e
00372db0 bls #0x372dd8
00372db4 ldr r2, [pc, #0x964]
00372db8 ldr r2, [r8, r2]
00372dbc ldr r2, [r2]
00372dc0 cmp r2, #2
00372dc4 moveq r2, #0
00372dc8 streq r2, [r2]
00372dcc beq #0x372dd8
00372dd0 cmp r2, #1
00372dd4 beq #0x373ad0
00372dd8 add r2, sp, #0xd4
00372ddc str r4, [sp, #0x28]
00372de0 mov sl, #0
00372de4 str r2, [sp, #0x14]
00372de8 movw sb, #0x14e8
00372dec mov r4, r2
00372df0 b #0x372e04
00372df4 add sl, sl, #1
00372df8 cmp sl, #0x1e
00372dfc beq #0x372e50
00372e00 ldr r3, [r5, sb]
00372e04 cmp r3, #0
00372e08 beq #0x372df4
00372e0c ldr r3, [r3, #0x84]
00372e10 cmp r3, sl
00372e14 bls #0x372df4
00372e18 mov r0, r5
00372e1c mov r1, sl
00372e20 bl #0x3bbed0
00372e24 cmp r0, #0
00372e28 mvnlt r3, #0
00372e2c strblt r3, [r4, sl]
00372e30 blt #0x372df4
00372e34 mov r1, sl
00372e38 mov r0, r5
00372e3c bl #0x3bbed0
00372e40 strb r0, [r4, sl]
00372e44 add sl, sl, #1
00372e48 cmp sl, #0x1e
00372e4c bne #0x372e00
00372e50 ldr r4, [sp, #0x28]
00372e54 mov r2, sl
00372e58 ldr r1, [sp, #0x14]
00372e5c add r0, r4, #0x400
00372e60 bl #0x3702a8
00372e64 add sl, r4, #0x4c0
00372e68 add sl, sl, #8
00372e6c mov r0, sl
00372e70 bl #0x814f70
00372e74 cmp r0, #0
00372e78 beq #0x372ea0
00372e7c ldr r3, [r5]
00372e80 mov r0, r5
00372e84 ldrb r1, [r4, #0x4e5]
00372e88 mov lr, pc
00372e8c ldr pc, [r3, #0x40]
00372e90 ldr r2, [sp, #0x24]
00372e94 ldr r3, [r2, #0x130]
00372e98 cmp r3, #0x26
00372e9c beq #0x3738ec
00372ea0 ldrb r3, [r4, #0x4e5]
00372ea4 ldr r1, [sp, #0x10]
00372ea8 ldr r2, [sp, #0xc]
00372eac cmp r3, #0
00372eb0 moveq r1, r3
00372eb4 movne r2, #1
00372eb8 str r1, [sp, #0x10]
00372ebc str r2, [sp, #0xc]
00372ec0 add r6, r6, #1
00372ec4 cmp r6, fp
00372ec8 bne #0x3728a8
00372ecc bl #0x7fd794
00372ed0 ldrb r3, [r0, #5]
00372ed4 cmp r3, #0
00372ed8 bne #0x373734
00372edc bl #0x7fd794
00372ee0 ldrb r3, [r0, #5]
00372ee4 cmp r3, #0
00372ee8 bne #0x3732a8
00372eec mov r4, #0
00372ef0 bl #0x7fd794
00372ef4 ldrb r3, [r0, #5]
00372ef8 cmp r3, #0
00372efc bne #0x373638
00372f00 cmp r4, #0
00372f04 bne #0x373698
00372f08 ldrb r3, [r7, #0x6c9]
00372f0c cmp r3, #0
00372f10 bne #0x372f30
00372f14 ldr r3, [r7, #0x6c4]
00372f18 cmp r3, #0
00372f1c ble #0x372f30
00372f20 mov r0, r7
00372f24 bl #0x370e00
00372f28 mov r0, r7
00372f2c bl #0x3721b8
00372f30 ldr r0, [sp, #0x1c]
00372f34 ldr r2, [sp, #0x1cc]
00372f38 ldr r3, [r8, r0]
00372f3c ldr r3, [r3]
00372f40 cmp r2, r3
00372f44 bne #0x373b84
00372f48 add sp, sp, #0x1d4
00372f4c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00372f50 add r0, r4, #0x500
00372f54 add r0, r0, #8
00372f58 bl #0x814f70
00372f5c cmp r0, #0
00372f60 beq #0x372b6c
00372f64 ldrb r3, [r4, #0x525]
00372f68 cmp r3, #0
00372f6c beq #0x372b6c
00372f70 mov r0, r7
00372f74 bl #0x36f074
00372f78 ldr lr, [sp, #0x20]
00372f7c cmp r0, #0
00372f80 movne lr, #1
00372f84 str lr, [sp, #0x20]
00372f88 b #0x372b6c
00372f8c bl #0x320e98
00372f90 ldrb r3, [r0, #0x28]
00372f94 cmp r3, #0
00372f98 beq #0x37317c
00372f9c bl #0x81a6ac
00372fa0 add r3, sp, #0xd4
00372fa4 ldr r1, [r0, #4]
00372fa8 add r2, sp, #0xd0
00372fac mov r0, r3
00372fb0 add sl, sp, #0x16c
00372fb4 str r3, [sp, #0x14]
00372fb8 bl #0x3140ec
00372fbc add r1, r4, #0x2d0
00372fc0 mov r0, sl
00372fc4 bl #0x32b918
00372fc8 ldr r0, [sp, #0x180]
00372fcc ldr r1, [sp, #0xe8]
00372fd0 ldr r2, [sp, #0x17c]
00372fd4 ldr r3, [sp, #0xe4]
00372fd8 rsb r2, r0, r2
00372fdc rsb r3, r1, r3
00372fe0 cmp r2, r3
00372fe4 beq #0x3738d4
00372fe8 mov r0, sl
00372fec add sl, sp, #0x154
00372ff0 bl #0x318254
00372ff4 add r2, sp, #0xcc
00372ff8 ldr r1, [sp, #0xe8]
00372ffc mov r0, sl
00373000 bl #0x3140ec
00373004 mov r0, r4
00373008 mov r1, sl
0037300c bl #0x371ccc
00373010 mov r0, sl
00373014 bl #0x318254
00373018 ldr r0, [sp, #0x14]
0037301c bl #0x318254
00373020 cmp r5, #0
00373024 bne #0x372990
00373028 ldr r1, [sb, #0x34]
0037302c ldr r3, [r4, #0x380]
00373030 cmp r3, r1
00373034 beq #0x373040
00373038 mov r0, r4
0037303c bl #0x370ef0
00373040 ldr r1, [sb, #0x30]
00373044 ldr r3, [r4, #0x330]
00373048 cmp r3, r1
0037304c beq #0x372990
00373050 mov r0, r4
00373054 bl #0x370e48
00373058 b #0x372990
0037305c ldr lr, [sp, #0x24]
00373060 cmp lr, #0
00373064 beq #0x372ec0
00373068 ldr r3, [r4, #0x380]
0037306c cmn r3, #1
00373070 beq #0x372ec0
00373074 bl #0x7fd794
00373078 ldrb r3, [r0, #5]
0037307c cmp r3, #0
00373080 bne #0x373a64
00373084 ldr r1, [r4, #0x670]
00373088 mov r0, r7
0037308c bl #0x372220
00373090 ldrb r3, [r4, #0x4e5]
00373094 ldr r1, [sp, #0x10]
00373098 ldr r2, [sp, #0xc]
0037309c cmp r3, #0
003730a0 moveq r1, r3
003730a4 movne r2, #1
003730a8 str r1, [sp, #0x10]
003730ac str r2, [sp, #0xc]
003730b0 b #0x372ec0
003730b4 bl #0x320e98
003730b8 ldrb r3, [r0, #0x24]
003730bc cmp r3, #0
003730c0 beq #0x372b5c
003730c4 bl #0x800f8c
003730c8 ldr r3, [r0]
003730cc mov lr, pc
003730d0 ldr pc, [r3, #0x64]
003730d4 cmp r0, #0
003730d8 beq #0x372b5c
003730dc bl #0x8100dc
003730e0 bl #0x8100e0
003730e4 cmp r0, #0
003730e8 beq #0x372b5c
003730ec ldrb r3, [r4, #0x525]
003730f0 cmp r3, #0
003730f4 bne #0x372b5c
003730f8 mov r0, r7
003730fc bl #0x36e09c
00373100 ldrb r3, [r0, #0x4e5]
00373104 cmp r3, #0
00373108 beq #0x372b5c
0037310c ldrb r3, [r7, #0x71a]
00373110 cmp r3, #0
00373114 moveq r3, #1
00373118 strbeq r3, [r7, #0x71a]
0037311c b #0x372b5c
00373120 bl #0x320e98
00373124 ldrb r3, [r0, #0x24]
00373128 cmp r3, #0
0037312c beq #0x372b6c
00373130 bl #0x800f8c
00373134 ldr r3, [r0]
00373138 mov lr, pc
0037313c ldr pc, [r3, #0x64]
00373140 cmp r0, #0
00373144 beq #0x372b6c
00373148 bl #0x8100dc
0037314c bl #0x8100e0
00373150 cmp r0, #0
00373154 beq #0x372b6c
00373158 ldrb r3, [r4, #0x525]
0037315c cmp r3, #0
00373160 beq #0x372b6c
00373164 ldrb r3, [r4, #0x545]
00373168 cmp r3, #0
0037316c bne #0x372b6c
00373170 mov r0, r4
00373174 bl #0x3709b4
00373178 b #0x372b6c
0037317c add sl, sp, #0x13c
00373180 add r1, r4, #0x2d0
00373184 mov r0, sl
00373188 bl #0x32b918
0037318c ldr r1, [sb, #0x2c]
00373190 mov r0, sl
00373194 bl #0x313c48
00373198 mov r3, r0
0037319c mov r0, sl
003731a0 str r3, [sp, #8]
003731a4 bl #0x318254
003731a8 ldr r3, [sp, #8]
003731ac cmp r3, #0
003731b0 bne #0x372988
003731b4 add sl, sp, #0x124
003731b8 add r2, sp, #0xc8
003731bc mov r0, sl
003731c0 ldr r1, [sb, #0x2c]
003731c4 bl #0x3140ec
003731c8 b #0x372974
003731cc mov r1, r4
003731d0 mov r0, r7
003731d4 bl #0x36d850
003731d8 mov r0, r5
003731dc bl #0x3bb7e8
003731e0 cmp r0, #0
003731e4 addeq r3, r4, #0x2d0
003731e8 beq #0x373a00
003731ec add r3, r4, #0x2d0
003731f0 add sl, sp, #0x10c
003731f4 mov r1, r3
003731f8 mov r0, sl
003731fc str r3, [sp, #8]
00373200 bl #0x32b918
00373204 mov r0, r5
00373208 bl #0x3bb7e8
0037320c mov r1, r0
00373210 mov r0, sl
00373214 bl #0x313c48
00373218 mov sb, r0
0037321c eor sb, sb, #1
00373220 mov r0, sl
00373224 bl #0x318254
00373228 tst sb, #0xff
0037322c ldr r3, [sp, #8]
00373230 bne #0x373a00
00373234 mov r0, r5
00373238 ldr sl, [r4, #0x380]
0037323c bl #0x3bb7fc
00373240 cmp r0, sl
00373244 beq #0x373308
00373248 ldr ip, [sp, #0x18]
0037324c ldr sl, [r8, ip]
00373250 mov r0, sl
00373254 bl #0x31f594
00373258 cmp r0, #0
0037325c beq #0x373308
00373260 mov r0, sl
00373264 bl #0x31f594
00373268 ldrb r3, [r0, #0x144]
0037326c cmp r3, #0
00373270 beq #0x373308
00373274 ldr sl, [r4, #0x380]
00373278 add sb, r5, #0x560
0037327c mov r0, sb
00373280 mov r1, sl
00373284 bl #0x3e0854
00373288 mov r0, r5
0037328c mov r1, sl
00373290 bl #0x3bb814
00373294 add r0, r5, #0x3c8
00373298 bl #0x3d8cfc
0037329c mov lr, #1
003732a0 str lr, [sp, #0x28]
003732a4 b #0x373314
003732a8 bl #0x7fd794
003732ac bl #0x7fd5b4
003732b0 cmp r0, #0
003732b4 beq #0x372eec
003732b8 ldr r0, [sp, #0xc]
003732bc cmp r0, #0
003732c0 beq #0x372eec
003732c4 ldr r1, [sp, #0x10]
003732c8 cmp r1, #0
003732cc bne #0x372eec
003732d0 ldr r2, [sp, #0x18]
003732d4 ldr r4, [pc, #0x420]
003732d8 ldr r0, [r8, r2]
003732dc add r4, pc, r4
003732e0 ldr r5, [r4]
003732e4 bl #0x31f66c
003732e8 rsb r0, r0, r5
003732ec cmp r0, #0
003732f0 movle r3, #0x7d0
003732f4 str r0, [r4]
003732f8 strle r3, [r4]
003732fc movle r4, #1
00373300 bgt #0x372eec
00373304 b #0x372ef0
00373308 mov ip, #0
0037330c str ip, [sp, #0x28]
00373310 add sb, r5, #0x560
00373314 ldr r1, [r4, #0x498]
00373318 ldr r3, [r5, #0x39c]
0037331c cmp r1, r3
00373320 addeq sl, r5, #0x37c
00373324 beq #0x373334
00373328 add sl, r5, #0x37c
0037332c mov r0, sl
00373330 bl #0x3fdfd8
00373334 ldr r3, [r4, #0x358]
00373338 mov r0, sl
0037333c str r3, [sp, #8]
00373340 bl #0x3fc690
00373344 ldr r3, [sp, #8]
00373348 cmp r0, r3
0037334c beq #0x37335c
00373350 mov r0, sl
00373354 ldr r1, [r4, #0x358]
00373358 bl #0x3ffc40
0037335c mov r0, sb
00373360 mov r1, #0x21
00373364 mov r2, #0
00373368 ldr sl, [r4, #0x448]
0037336c bl #0x3df6e0
00373370 cmp r0, sl
00373374 beq #0x373388
00373378 mov r0, sb
0037337c mov r1, #0x21
00373380 ldr r2, [r4, #0x448]
00373384 bl #0x3e0808
00373388 mov r0, sb
0037338c mov r1, #0x24
00373390 mov r2, #0
00373394 ldr sl, [r4, #0x470]
00373398 bl #0x3df6e0
0037339c cmp r0, sl
003733a0 beq #0x3733d4
003733a4 ldr r3, [r5]
003733a8 mov r0, r5
003733ac mov lr, pc
003733b0 ldr pc, [r3, #0x34]
003733b4 mov r1, #0x24
003733b8 mov r2, #0
003733bc mov r0, sb
003733c0 bl #0x3df6e0
003733c4 mov r0, sb
003733c8 mov r1, #0x24
003733cc ldr r2, [r4, #0x470]
003733d0 bl #0x3e0808
003733d4 ldr sl, [r4, #0x660]
003733d8 mov r0, r5
003733dc ldr r2, [sl, #0x93c]
003733e0 ldr r3, [sl, #0x5b8]
003733e4 add r3, r2, r3
003733e8 asr r3, r3, #8
003733ec str r3, [sp, #0x14]
003733f0 bl #0x3bb828
003733f4 ldr r1, [r4, #0x330]
003733f8 mov r3, r0
003733fc cmp r0, r1
00373400 beq #0x373410
00373404 mov r0, r5
00373408 bl #0x3bb840
0037340c ldr r3, [r4, #0x330]
00373410 ldr r0, [sp, #0x14]
00373414 cmp r0, r3
00373418 beq #0x373448
0037341c ldr r2, [sl, #0x5b8]
00373420 mov r1, #0x13
00373424 mov r0, sb
00373428 rsb r2, r2, r3, lsl #8
0037342c asr r2, r2, #8
00373430 bl #0x3e0808
00373434 ldr r2, [r4, #0x330]
00373438 ldr r1, [sp, #0x14]
0037343c rsb r2, r1, r2
00373440 cmp r2, #1
00373444 beq #0x373978
00373448 add r0, r4, #0x3d8
0037344c bl #0x814f70
00373450 cmp r0, #0
00373454 beq #0x3738a8
00373458 ldr r1, [r4, #0x3f8]
0037345c cmp r1, #0
00373460 beq #0x3739d0
00373464 ldr r2, [r4, #0x3fc]
00373468 cmp r2, #0
0037346c ble #0x3739d0
00373470 add sb, sp, #0xc4
00373474 mov r0, sb
00373478 bl #0x30e868
0037347c mov sl, #0
00373480 ldrsb r2, [sb, sl]
00373484 mov r1, sl
00373488 mov r0, r5
0037348c cmn r2, #1
00373490 mvnlt lr, #0
00373494 strblt lr, [sb, sl]
00373498 mvnlt r2, #0
0037349c add sl, sl, #1
003734a0 bl #0x3bbe54
003734a4 cmp sl, #3
003734a8 bne #0x373480
003734ac add r0, r4, #0x400
003734b0 bl #0x814f70
003734b4 cmp r0, #0
003734b8 beq #0x3738c4
003734bc movw r3, #0x14e8
003734c0 ldr r3, [r5, r3]
003734c4 cmp r3, #0
003734c8 beq #0x3734fc
003734cc ldr r3, [r3, #0x84]
003734d0 cmp r3, #0x1e
003734d4 bls #0x3734fc
003734d8 ldr r3, [pc, #0x240]
003734dc ldr r3, [r8, r3]
003734e0 ldr r3, [r3]
003734e4 cmp r3, #2
003734e8 moveq r3, #0
003734ec streq r3, [r3]
003734f0 beq #0x3734fc
003734f4 cmp r3, #1
003734f8 beq #0x373b04
003734fc ldr r1, [r4, #0x420]
00373500 cmp r1, #0
00373504 beq #0x37351c
00373508 ldr r2, [r4, #0x424]
0037350c cmp r2, #0
00373510 ble #0x37351c
00373514 add r0, sp, #0xd4
00373518 bl #0x30e868
0037351c mov sl, #0
00373520 add r3, sp, #0xd4
00373524 str r4, [sp, #0x14]
00373528 mov r1, sl
0037352c movw sb, #0x14e8
00373530 mov r4, r3
00373534 ldr r3, [r5, sb]
00373538 cmp r3, #0
0037353c beq #0x373568
00373540 ldr r3, [r3, #0x84]
00373544 cmp sl, r3
00373548 bhs #0x373568
0037354c ldrsb r2, [r4, sl]
00373550 cmp r2, #0
00373554 blt #0x373568
00373558 mov r1, sl
0037355c mov r0, r5
00373560 bl #0x3bbebc
00373564 mov r1, #1
00373568 add sl, sl, #1
0037356c cmp sl, #0x1e
00373570 bne #0x373534
00373574 cmp r1, #0
00373578 ldr r4, [sp, #0x14]
0037357c bne #0x37396c
00373580 ldrb r3, [r4, #0x4e5]
00373584 cmp r3, #0
00373588 ldrne r1, [sp, #0x2c]
0037358c moveq sb, r3
00373590 ldrne r2, [r8, r1]
00373594 ldrbne sb, [r2, #0x30]
00373598 ldrb r2, [r5, #0x80]
0037359c eorne sb, sb, #1
003735a0 cmp r2, sb
003735a4 addne sl, r5, #0x4f0
003735a8 addne sl, sl, #0xc
003735ac beq #0x373a28
003735b0 mov r0, sl
003735b4 mov r1, #0
003735b8 bl #0x3c1a00
003735bc add sl, r4, #0x4c0
003735c0 mov r0, r5
003735c4 mov r1, sb
003735c8 add sl, sl, #8
003735cc ldr r3, [r5]
003735d0 mov lr, pc
003735d4 ldr pc, [r3, #0x40]
003735d8 mov r0, sl
003735dc bl #0x814f70
003735e0 b #0x372e6c
003735e4 bl #0x320e98
003735e8 ldrb r3, [r0, #0x24]
003735ec cmp r3, #0
003735f0 beq #0x372aac
003735f4 bl #0x800f8c
003735f8 ldr r3, [r0]
003735fc mov lr, pc
00373600 ldr pc, [r3, #0x64]
00373604 cmp r0, #0
00373608 beq #0x372aac
0037360c bl #0x8100dc
00373610 bl #0x8100e0
00373614 b #0x372aac
00373618 bl #0x30e5e0
0037361c cmp r0, #0
00373620 bne #0x372950
00373624 mov r0, sl
00373628 bl #0x318254
0037362c ldr r0, [sp, #0x14]
00373630 bl #0x318254
00373634 b #0x372988
00373638 ldr r3, [sp, #0x18]
0037363c ldr r5, [r8, r3]
00373640 mov r0, r5
00373644 bl #0x31f594
00373648 cmp r0, #0
0037364c beq #0x372f00
00373650 mov r1, #0
00373654 mov r0, r7
00373658 mov r2, r1
0037365c bl #0x36e478
00373660 ldrb r3, [r0, #0x4e5]
00373664 cmp r3, #0
00373668 bne #0x372f00
0037366c ldr r6, [pc, #0x8c]
00373670 mov r0, r5
00373674 add r6, pc, r6
00373678 ldr r5, [r6, #4]
0037367c bl #0x31f66c
00373680 rsb r0, r0, r5
00373684 cmp r0, #0
00373688 movwle r3, #0x1388
0037368c str r0, [r6, #4]
00373690 strle r3, [r6, #4]
00373694 bgt #0x372f00
00373698 bl #0x8100dc
0037369c bl #0x8103f4
003736a0 mov r1, #0
003736a4 mov r2, r1
003736a8 mov r0, r7
003736ac bl #0x36e478
003736b0 bl #0x81347c
003736b4 bl #0x8151e8
003736b8 mov r3, #0
003736bc str r3, [r0, #0x2c]
003736c0 b #0x372f08
003736c4 rsbeq r2, r2, r4, ror r2
003736c8 andeq r4, r0, ip, lsr #1
003736cc strdeq r3, r4, [r0], -r4
003736d0 subseq lr, r4, r4, asr #30
003736d4 andeq r1, r0, r0, lsr #20
003736d8 subseq lr, r4, r0, asr #30
# _ZN10PlayerInfo5ResetEv 373bdc 1456
00373bdc push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00373be0 ldr r5, [pc, #0x57c]
00373be4 ldr r1, [pc, #0x57c]
00373be8 sub sp, sp, #0x17c
00373bec add r5, pc, r5
00373bf0 ldr r3, [r5, r1]
00373bf4 mov r4, r0
00373bf8 str r1, [sp, #0x14]
00373bfc ldr r3, [r3]
00373c00 add r6, sp, #0x15c
00373c04 mov r7, #0
00373c08 str r3, [sp, #0x174]
00373c0c bl #0x80f27c
00373c10 ldr r1, [pc, #0x554]
00373c14 mov r3, #1
00373c18 strb r3, [r4, #0x66c]
00373c1c add r2, sp, #0x114
00373c20 add r1, pc, r1
00373c24 str r7, [r4, #0x660]
00373c28 mov r0, r6
00373c2c bl #0x3140ec
00373c30 mov r1, r6
00373c34 mov r0, r4
00373c38 bl #0x371ccc
00373c3c mov r0, r6
00373c40 bl #0x318254
00373c44 mov r0, r4
00373c48 mvn r1, #0
00373c4c bl #0x370e48
00373c50 mov r0, r4
00373c54 mvn r1, #0
00373c58 bl #0x36fcb0
00373c5c mov r0, r4
00373c60 mvn r1, #0
00373c64 bl #0x370ef0
00373c68 mvn r1, #0
00373c6c mov r0, r4
00373c70 bl #0x370bf4
00373c74 mov r2, r7
00373c78 add r1, sp, #0x118
00373c7c mvn r3, #0
00373c80 strb r3, [r1, r2]
00373c84 add r2, r2, #1
00373c88 cmp r2, #0x24
00373c8c mvn r6, #0
00373c90 bne #0x373c80
00373c94 add r0, r4, #0x3b0
00373c98 bl #0x36ffd0
00373c9c add r1, sp, #0x110
00373ca0 mov r2, #3
00373ca4 add r0, r4, #0x3d8
00373ca8 strb r6, [sp, #0x110]
00373cac strb r6, [sp, #0x111]
00373cb0 strb r6, [sp, #0x112]
00373cb4 bl #0x370258
00373cb8 mov r2, #0
00373cbc add r1, sp, #0x13c
00373cc0 strb r6, [r1, r2]
00373cc4 add r2, r2, #1
00373cc8 cmp r2, #0x1e
00373ccc bne #0x373cc0
00373cd0 ldr r3, [pc, #0x498]
00373cd4 add r0, r4, #0x400
00373cd8 ldr sb, [pc, #0x494]
00373cdc str r3, [sp, #0x10]
00373ce0 bl #0x3702a8
00373ce4 mov r0, r4
00373ce8 mov r1, #0
00373cec bl #0x36fd58
00373cf0 mov r1, #0
00373cf4 mov r0, r4
00373cf8 bl #0x36fe04
00373cfc ldr ip, [sp, #0x10]
00373d00 ldr lr, [pc, #0x470]
00373d04 add r1, sp, #0x40
00373d08 ldr r3, [r5, ip]
00373d0c mov r6, #0
00373d10 mov fp, #0xb4000000
00373d14 add r3, r3, #8
00373d18 add r2, r1, #0x20
00373d1c str lr, [sp]
00373d20 str r1, [sp, #0xc]
00373d24 asr fp, fp, #0x16
00373d28 str r3, [sp, #4]
00373d2c mov r7, r4
00373d30 mvn sl, #0
00373d34 mov r8, r6
00373d38 str r2, [sp, #8]
00373d3c mov r3, #0x10
00373d40 str r3, [sp, #0x44]
00373d44 mov r2, #0
00373d48 mov r3, #0
00373d4c add ip, sp, #0x178
00373d50 strd r2, r3, [ip, fp]
00373d54 ldr r3, [sp, #0x60]
00373d58 ldr lr, [sp, #4]
00373d5c str sl, [sp, #0x50]
00373d60 cmp r3, #0
00373d64 str sl, [sp, #0x54]
00373d68 str r8, [sp, #0x58]
00373d6c strb r8, [sp, #0x5c]
00373d70 str lr, [sp, #0x40]
00373d74 beq #0x373d84
00373d78 ldr r0, [sp, #0xc]
00373d7c str r8, [sp, #0x60]
00373d80 bl #0x814f84
00373d84 ldr r1, [sp]
00373d88 mov r2, #0x28
00373d8c mul r0, r2, r6
00373d90 ldr r3, [r5, r1]
00373d94 add r0, r0, #0x540
00373d98 add r0, r0, #8
00373d9c add r3, r3, #8
00373da0 str r3, [sp, #0x40]
00373da4 ldr r3, [r7, #0x548]
00373da8 add r0, r4, r0
00373dac ldr r1, [sp, #8]
00373db0 mov lr, pc
00373db4 ldr pc, [r3, #0x1c]
00373db8 ldr r3, [r5, sb]
00373dbc add r6, r6, #1
00373dc0 cmp r6, #7
00373dc4 add r3, r3, #8
00373dc8 str r3, [sp, #0x40]
00373dcc add r7, r7, #0x28
00373dd0 bne #0x373d3c
00373dd4 ldr r3, [sp, #0x10]
00373dd8 mov r1, #0xaa000000
00373ddc asr r1, r1, #0x16
00373de0 ldr r0, [r5, r3]
00373de4 ldr r3, [sp, #0x38]
00373de8 mov r6, #0
00373dec mov r7, #0
00373df0 add ip, sp, #0x178
00373df4 mvn r2, #0
00373df8 strd r6, r7, [ip, r1]
00373dfc cmp r3, #0
00373e00 add r0, r0, #8
00373e04 mov r3, #0
00373e08 mov r1, #8
00373e0c str r1, [sp, #0x1c]
00373e10 str r2, [sp, #0x2c]
00373e14 str r0, [sp, #0x18]
00373e18 addeq r6, sp, #0x18
00373e1c str r2, [r4, #0x678]
00373e20 str r2, [sp, #0x28]
00373e24 str r3, [sp, #0x30]
00373e28 strb r3, [sp, #0x34]
00373e2c beq #0x373e40
00373e30 add r6, sp, #0x18
00373e34 mov r0, r6
00373e38 str r3, [sp, #0x38]
00373e3c bl #0x814f84
00373e40 ldr r2, [pc, #0x334]
00373e44 add r1, r6, #0x20
00373e48 ldr r3, [r4, #0x288]
00373e4c ldr r2, [r5, r2]
00373e50 add r0, r4, #0x288
00373e54 mov r6, #0
00373e58 add r2, r2, #8
00373e5c str r2, [sp, #0x18]
00373e60 mov lr, pc
00373e64 ldr pc, [r3, #0x1c]
00373e68 ldr lr, [sp, #0x10]
00373e6c ldr r3, [sp, #0x88]
00373e70 ldr ip, [r5, sb]
00373e74 ldr r0, [r5, lr]
00373e78 mov r1, #0xbe000000
00373e7c asr r1, r1, #0x16
00373e80 mov r7, #0
00373e84 add lr, sp, #0x178
00373e88 mvn r2, #0
00373e8c strd r6, r7, [lr, r1]
00373e90 cmp r3, #0
00373e94 add ip, ip, #8
00373e98 mov r3, #0
00373e9c add r0, r0, #8
00373ea0 mov r1, #0x20
00373ea4 str ip, [sp, #0x18]
00373ea8 str r1, [sp, #0x6c]
00373eac str r2, [sp, #0x7c]
00373eb0 str r0, [sp, #0x68]
00373eb4 addeq r6, sp, #0x68
00373eb8 str r2, [r4, #0x664]
00373ebc str r2, [r4, #0x668]
00373ec0 str r2, [r4, #0x670]
00373ec4 str r2, [r4, #0x674]
00373ec8 str r2, [r4, #0x67c]
00373ecc str r2, [sp, #0x78]
00373ed0 str r3, [sp, #0x80]
00373ed4 strb r3, [sp, #0x84]
00373ed8 beq #0x373eec
00373edc add r6, sp, #0x68
00373ee0 mov r0, r6
00373ee4 str r3, [sp, #0x88]
00373ee8 bl #0x814f84
00373eec ldr r2, [pc, #0x28c]
00373ef0 ldr r7, [pc, #0x28c]
00373ef4 ldr r3, [r4, #0x4a0]
00373ef8 ldr r2, [r5, r2]
00373efc add r1, r6, #0x20
00373f00 add r0, r4, #0x4a0
00373f04 add r2, r2, #8
00373f08 str r2, [sp, #0x68]
00373f0c mov lr, pc
00373f10 ldr pc, [r3, #0x1c]
00373f14 ldr r0, [r5, sb]
00373f18 ldrb r3, [sp, #0x10d]
00373f1c ldr r1, [r5, r7]
00373f20 add r0, r0, #8
00373f24 str r0, [sp, #0x68]
00373f28 cmp r3, #0
00373f2c mvn r2, #0
00373f30 mov r3, #0
00373f34 add r1, r1, #8
00373f38 mov r0, #1
00373f3c mov sl, #0
00373f40 mov fp, #0
00373f44 str r0, [sp, #0xf4]
00373f48 strd sl, fp, [sp, #0xf8]
00373f4c str r2, [sp, #0x104]
00373f50 str r1, [sp, #0xf0]
00373f54 addeq r8, sp, #0xf0
00373f58 str r3, [r4, #0x684]
00373f5c str r3, [r4, #0x680]
00373f60 str r2, [sp, #0x100]
00373f64 str r3, [sp, #0x108]
00373f68 strb r3, [sp, #0x10c]
00373f6c beq #0x373f80
00373f70 add r8, sp, #0xf0
00373f74 mov r0, r8
00373f78 strb r3, [sp, #0x10d]
00373f7c bl #0x814f84
00373f80 ldr r6, [pc, #0x200]
00373f84 add r0, r4, #0x4c0
00373f88 add r1, r8, #0x1d
00373f8c ldr r2, [r5, r6]
00373f90 ldr r3, [r4, #0x4c8]
00373f94 add r0, r0, #8
00373f98 add r2, r2, #8
00373f9c str r2, [sp, #0xf0]
00373fa0 mov lr, pc
00373fa4 ldr pc, [r3, #0x1c]
00373fa8 ldr r0, [r5, sb]
00373fac ldrb r3, [sp, #0xed]
00373fb0 ldr r1, [r5, r7]
00373fb4 add r0, r0, #8
00373fb8 cmp r3, #0
00373fbc mvn r2, #0
00373fc0 mov r3, #0
00373fc4 add r1, r1, #8
00373fc8 str r0, [sp, #0xf0]
00373fcc mov sl, #0
00373fd0 mov r0, #1
00373fd4 mov fp, #0
00373fd8 str r0, [sp, #0xd4]
00373fdc strd sl, fp, [sp, #0xd8]
00373fe0 str r2, [sp, #0xe4]
00373fe4 str r1, [sp, #0xd0]
00373fe8 str r2, [sp, #0xe0]
00373fec str r3, [sp, #0xe8]
00373ff0 strb r3, [sp, #0xec]
00373ff4 addeq r8, sp, #0xd0
00373ff8 beq #0x37400c
00373ffc add r8, sp, #0xd0
00374000 mov r0, r8
00374004 strb r3, [sp, #0xed]
00374008 bl #0x814f84
0037400c ldr r3, [r5, r6]
00374010 add r0, r4, #0x4e0
00374014 add r1, r8, #0x1d
00374018 add r3, r3, #8
0037401c str r3, [sp, #0xd0]
00374020 add r0, r0, #8
00374024 ldr r3, [r4, #0x4e8]
00374028 mov lr, pc
0037402c ldr pc, [r3, #0x1c]
00374030 ldr r0, [r5, sb]
00374034 ldrb r3, [sp, #0xcd]
00374038 ldr r1, [r5, r7]
0037403c add r0, r0, #8
00374040 cmp r3, #0
00374044 mvn r2, #0
00374048 mov r3, #0
0037404c add r1, r1, #8
00374050 str r0, [sp, #0xd0]
00374054 mov sl, #0
00374058 mov r0, #1
0037405c mov fp, #0
00374060 str r0, [sp, #0xb4]
00374064 strd sl, fp, [sp, #0xb8]
00374068 str r2, [sp, #0xc4]
0037406c str r1, [sp, #0xb0]
00374070 str r2, [sp, #0xc0]
00374074 str r3, [sp, #0xc8]
00374078 strb r3, [sp, #0xcc]
0037407c addeq r8, sp, #0xb0
00374080 beq #0x374094
00374084 add r8, sp, #0xb0
00374088 mov r0, r8
0037408c strb r3, [sp, #0xcd]
00374090 bl #0x814f84
00374094 ldr r3, [r5, r6]
00374098 add r0, r4, #0x500
0037409c add r1, r8, #0x1d
003740a0 add r3, r3, #8
003740a4 str r3, [sp, #0xb0]
003740a8 add r0, r0, #8
003740ac ldr r3, [r4, #0x508]
003740b0 mov lr, pc
003740b4 ldr pc, [r3, #0x1c]
003740b8 ldr r0, [r5, sb]
003740bc ldr r1, [r5, r7]
003740c0 ldrb r3, [sp, #0xad]
003740c4 add r0, r0, #8
003740c8 mvn r2, #0
003740cc cmp r3, #0
003740d0 add r1, r1, #8
003740d4 mov r3, #0
003740d8 str r0, [sp, #0xb0]
003740dc mov r8, #0
003740e0 mov r0, #1
003740e4 mov sb, #0
003740e8 str r0, [sp, #0x94]
003740ec strd r8, sb, [sp, #0x98]
003740f0 str r2, [sp, #0xa4]
003740f4 str r1, [sp, #0x90]
003740f8 str r2, [sp, #0xa0]
003740fc str r3, [sp, #0xa8]
00374100 strb r3, [sp, #0xac]
00374104 addeq r7, sp, #0x90
00374108 beq #0x37411c
0037410c add r7, sp, #0x90
00374110 mov r0, r7
00374114 strb r3, [sp, #0xad]
00374118 bl #0x814f84
0037411c ldr r3, [r5, r6]
00374120 add r0, r4, #0x520
00374124 add r1, r7, #0x1d
00374128 add r3, r3, #8
0037412c str r3, [sp, #0x90]
00374130 ldr r3, [r4, #0x528]
00374134 add r0, r0, #8
00374138 mov lr, pc
0037413c ldr pc, [r3, #0x1c]
00374140 ldr r1, [sp, #0x14]
00374144 ldr r2, [sp, #0x174]
00374148 ldr r3, [r5, r1]
0037414c ldr r3, [r3]
00374150 cmp r2, r3
00374154 bne #0x374160
00374158 add sp, sp, #0x17c
0037415c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00374160 bl #0x30e310
00374164 rsbeq r0, r2, r4, lsr #29
00374168 andeq r4, r0, ip, lsr #1
0037416c subseq r7, r5, r8, ror #23
00374170 andeq r2, r0, r4, lsl #19
00374174 andeq r1, r0, r8, lsr #1
00374178 andeq r3, r0, ip, lsr r5
0037417c andeq r1, r0, r0, asr r5
00374180 andeq r1, r0, r8, asr #1
00374184 andeq r3, r0, r8, lsl r0
00374188 andeq r0, r0, r8, asr #21
# _ZNSt5dequeI9StatusMsgSaIS0_EE9pop_frontEv 383d10 152
00383d10 push {r4, lr}
00383d14 ldr r3, [r0]
00383d18 mov r4, r0
00383d1c ldr r0, [r3, #0x14]
00383d20 cmp r0, r3
00383d24 beq #0x383d48
00383d28 cmp r0, #0
00383d2c beq #0x383d48
00383d30 ldr r1, [r3]
00383d34 rsb r1, r0, r1
00383d38 cmp r1, #0x80
00383d3c bhi #0x383d9c
00383d40 bl #0x708f00
00383d44 ldr r3, [r4]
00383d48 ldr r2, [r4, #8]
00383d4c sub r2, r2, #0x1c
00383d50 cmp r3, r2
00383d54 beq #0x383d64
00383d58 add r3, r3, #0x1c
00383d5c str r3, [r4]
00383d60 pop {r4, pc}
00383d64 ldr r0, [r4, #4]
00383d68 cmp r0, #0
00383d6c beq #0x383d78
00383d70 mov r1, #0x70
00383d74 bl #0x708f00
00383d78 ldr r3, [r4, #0xc]
00383d7c add r2, r3, #4
00383d80 str r2, [r4, #0xc]
00383d84 ldr r3, [r3, #4]
00383d88 add r2, r3, #0x70
00383d8c str r3, [r4]
00383d90 str r2, [r4, #8]
00383d94 str r3, [r4, #4]
00383d98 pop {r4, pc}
00383d9c bl #0x310440
00383da0 ldr r3, [r4]
00383da4 b #0x383d48
# _ZN18MenuMessageManagerI9StatusMsgLi4EE14EnqueueMessageERKS0_ib.clone.18 3ecff8 332
003ecff8 push {r4, r5, r6, r7, lr}
003ecffc ldr r4, [pc, #0x12c]
003ed000 ldr r5, [pc, #0x12c]
003ed004 sub sp, sp, #0x34
003ed008 add r4, pc, r4
003ed00c ldr r5, [r4, r5]
003ed010 mov r1, r0
003ed014 add r6, r5, #4
003ed018 mov r0, r6
003ed01c bl #0x3be28c
003ed020 ldm r6, {r0, r1, r2, r3}
003ed024 add ip, sp, #0xc
003ed028 stm ip, {r0, r1, r2, r3}
003ed02c add r0, r5, #0x14
003ed030 mov r1, ip
003ed034 bl #0x3bcf8c
003ed038 cmp r0, #1
003ed03c beq #0x3ed048
003ed040 add sp, sp, #0x34
003ed044 pop {r4, r5, r6, r7, pc}
003ed048 ldr r3, [pc, #0xe8]
003ed04c ldr r3, [r4, r3]
003ed050 ldr r7, [r3]
003ed054 bl #0x42ca8c
003ed058 bl #0x42cb8c
003ed05c subs r6, r0, #0
003ed060 beq #0x3ed040
003ed064 ldr r5, [pc, #0xd0]
003ed068 ldr r3, [r4, r5]
003ed06c ldr r2, [r3, #0x2c]
003ed070 cmp r2, #0
003ed074 beq #0x3ed0ac
003ed078 ldr r0, [r3, #0x28]
003ed07c ldrb r3, [r0, #4]
003ed080 cmp r3, #0
003ed084 bne #0x3ed0c8
003ed088 ldr r1, [r0]
003ed08c sub r1, r1, #1
003ed090 cmp r1, #0
003ed094 str r1, [r0]
003ed098 beq #0x3ed128
003ed09c ldr r3, [r4, r5]
003ed0a0 mov r2, #0
003ed0a4 str r2, [r3, #0x2c]
003ed0a8 str r2, [r3, #0x28]
003ed0ac ldr r3, [pc, #0x8c]
003ed0b0 ldr r0, [r4, r5]
003ed0b4 mov r2, r6
003ed0b8 ldr r1, [r4, r3]
003ed0bc mov r3, #0
003ed0c0 ldr r1, [r1]
003ed0c4 bl #0x427ca0
003ed0c8 ldr r0, [r4, r5]
003ed0cc bl #0x427d50
003ed0d0 mov ip, #0
003ed0d4 strb ip, [sp, #0x1c]
003ed0d8 mov r2, #0
003ed0dc mov r3, #0
003ed0e0 mov ip, #2
003ed0e4 strd r2, r3, [sp, #0x28]
003ed0e8 strb ip, [sp, #0x1d]
003ed0ec mov ip, #0
003ed0f0 str ip, [sp, #0x20]
003ed0f4 ldr ip, [sp, #0x2c]
003ed0f8 add r4, sp, #0x1c
003ed0fc mov r1, r0
003ed100 str ip, [r4, #8]
003ed104 mov r0, r6
003ed108 mov ip, #1
003ed10c mov r2, r7
003ed110 mov r3, r4
003ed114 str ip, [sp]
003ed118 bl #0x7abe0c
003ed11c mov r0, r4
003ed120 bl #0x797124
003ed124 b #0x3ed040
003ed128 bl #0x752b38
003ed12c b #0x3ed09c
003ed130 subseq r7, sl, r8, lsl #21
003ed134 andeq r1, r0, ip, ror #9
003ed138 andeq r4, r0, ip, lsr #21
003ed13c andeq r2, r0, r0, ror r0
003ed140 muleq r0, r4, r5
# _ZN18MenuMessageManagerI9StatusMsgLi4EE21FlushEnqueuedMessagesEi.clone.25 3f9018 76
003f9018 ldr r3, [pc, #0x3c]
003f901c ldr r2, [pc, #0x3c]
003f9020 push {r4, r5, r6, lr}
003f9024 add r3, pc, r3
003f9028 ldr r4, [r3, r2]
003f902c ldr r2, [r4, #0x14]
003f9030 ldr r3, [r4, #4]
003f9034 cmp r2, r3
003f9038 beq #0x3f9058
003f903c add r5, r4, #4
003f9040 mov r0, r5
003f9044 bl #0x383d10
003f9048 ldr r2, [r4, #0x14]
003f904c ldr r3, [r4, #4]
003f9050 cmp r2, r3
003f9054 bne #0x3f9040
003f9058 pop {r4, r5, r6, pc}
003f905c subseq fp, sb, ip, ror #20
003f9060 andeq r1, r0, ip, ror #9
# _GLOBAL__I_.._.._sources_Game_Menus_MenuMessageManager.cpp 434624 1948
00434624 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00434628 ldr r5, [pc, #0x6f0]
0043462c ldr r2, [pc, #0x6f0]
00434630 ldr r3, [pc, #0x6f0]
00434634 add r5, pc, r5
00434638 ldr r4, [r5, r2]
0043463c add r3, pc, r3
00434640 mov r2, #0x3f000000
00434644 sub sp, sp, #0x1c
00434648 str r2, [r3, #8]
0043464c str r2, [r3]
00434650 str r2, [r3, #4]
00434654 mov r0, r4
00434658 bl #0x41aeec
0043465c ldr r3, [pc, #0x6c8]
00434660 ldr r7, [pc, #0x6c8]
00434664 mov r0, r4
00434668 ldr r6, [r5, r3]
0043466c ldr r4, [r5, r7]
00434670 mov r1, r6
00434674 mov r2, r4
00434678 bl #0x30e304
0043467c ldr r3, [pc, #0x6b0]
00434680 ldr r8, [r5, r3]
00434684 mov r0, r8
00434688 bl #0x41aeec
0043468c mov r1, r6
00434690 mov r2, r4
00434694 mov r0, r8
00434698 bl #0x30e304
0043469c ldr r3, [pc, #0x694]
004346a0 ldr r8, [r5, r3]
004346a4 mov r0, r8
004346a8 bl #0x41aeec
004346ac mov r1, r6
004346b0 mov r2, r4
004346b4 mov r0, r8
004346b8 bl #0x30e304
004346bc ldr r3, [pc, #0x678]
004346c0 ldr r8, [r5, r3]
004346c4 mov r0, r8
004346c8 bl #0x41aeec
004346cc mov r1, r6
004346d0 mov r2, r4
004346d4 mov r0, r8
004346d8 bl #0x30e304
004346dc ldr r3, [pc, #0x65c]
004346e0 ldr r8, [r5, r3]
004346e4 mov r0, r8
004346e8 bl #0x41aeec
004346ec mov r1, r6
004346f0 mov r2, r4
004346f4 mov r0, r8
004346f8 bl #0x30e304
004346fc ldr r3, [pc, #0x640]
00434700 ldr r8, [r5, r3]
00434704 mov r0, r8
00434708 bl #0x41aeec
0043470c mov r2, r4
00434710 mov r0, r8
00434714 mov r1, r6
00434718 bl #0x30e304
0043471c ldr r3, [pc, #0x624]
00434720 ldr r3, [r5, r3]
00434724 ldr r2, [r3]
00434728 tst r2, #1
0043472c beq #0x434cb0
00434730 ldr r3, [pc, #0x614]
00434734 ldr r3, [r5, r3]
00434738 ldr r2, [r3]
0043473c tst r2, #1
00434740 beq #0x434cf0
00434744 ldr r3, [pc, #0x604]
00434748 ldr r3, [r5, r3]
0043474c ldr r6, [r3]
00434750 ands r6, r6, #1
00434754 bne #0x434848
00434758 ldr r1, [pc, #0x5f4]
0043475c ldr r2, [pc, #0x5f4]
00434760 add sb, sp, #0x14
00434764 ldr sl, [r5, r1]
00434768 ldr r2, [r5, r2]
0043476c str r1, [sp, #0xc]
00434770 mov r4, sl
00434774 mov r1, #1
00434778 add r2, r2, #8
0043477c str r1, [r3]
00434780 str r2, [r4], #4
00434784 add sl, sl, #0xa4
00434788 mov r8, #8
0043478c mov fp, #0x70
00434790 mov r1, r8
00434794 str r6, [r4]
00434798 str r6, [r4, #4]
0043479c str r6, [r4, #8]
004347a0 str r6, [r4, #0xc]
004347a4 str r6, [r4, #0x10]
004347a8 str r6, [r4, #0x14]
004347ac str r6, [r4, #0x18]
004347b0 str r6, [r4, #0x1c]
004347b4 str r6, [r4, #0x20]
004347b8 str r8, [r4, #0x24]
004347bc add r0, r4, #0x20
004347c0 mov r2, r6
004347c4 bl #0x329510
004347c8 ldr r2, [r4, #0x24]
004347cc str r0, [r4, #0x20]
004347d0 mov r3, r0
004347d4 sub r2, r2, #1
004347d8 lsr r2, r2, #1
004347dc mov r0, sb
004347e0 stmib sp, {r2, r3}
004347e4 str fp, [sp, #0x14]
004347e8 bl #0x708ec0
004347ec ldmib sp, {r2, r3}
004347f0 add r1, r3, r2, lsl #2
004347f4 str r0, [r3, r2, lsl #2]
004347f8 str r1, [r4, #0xc]
004347fc ldr r0, [r3, r2, lsl #2]
00434800 str r1, [r4, #0x1c]
00434804 add r1, r0, #0x70
00434808 stmib r4, {r0, r1}
0043480c ldr r3, [r3, r2, lsl #2]
00434810 str r0, [r4]
00434814 add r2, r3, #0x70
00434818 str r2, [r4, #0x18]
0043481c str r3, [r4, #0x10]
00434820 str r3, [r4, #0x14]
00434824 add r4, r4, #0x28
00434828 cmp r4, sl
0043482c bne #0x434790
00434830 ldr r2, [sp, #0xc]
00434834 ldr r3, [pc, #0x520]
00434838 ldr r0, [r5, r2]
0043483c ldr r1, [r5, r3]
00434840 ldr r2, [r5, r7]
00434844 bl #0x30e304
00434848 ldr r3, [pc, #0x510]
0043484c ldr ip, [r5, r3]
00434850 ldr r3, [ip]
00434854 ands r3, r3, #1
00434858 bne #0x434928
0043485c ldr r6, [pc, #0x500]
00434860 ldr r2, [pc, #0x500]
00434864 mov lr, #8
00434868 ldr r6, [r5, r6]
0043486c ldr r4, [r5, r2]
00434870 mov r8, #1
00434874 add r6, r6, lr
00434878 str r8, [ip]
0043487c mov r1, lr
00434880 mov r2, r3
00434884 str lr, [r4, #0x28]
00434888 str r6, [r4]
0043488c str r3, [r4, #4]
00434890 str r3, [r4, #8]
00434894 str r3, [r4, #0xc]
00434898 str r3, [r4, #0x10]
0043489c str r3, [r4, #0x14]
004348a0 str r3, [r4, #0x18]
004348a4 str r3, [r4, #0x1c]
004348a8 str r3, [r4, #0x20]
004348ac str r3, [r4, #0x24]
004348b0 add r0, r4, #0x24
004348b4 bl #0x329570
004348b8 mov r3, #0x70
004348bc mov r6, r0
004348c0 add r0, sp, #0x18
004348c4 ldr r8, [r4, #0x28]
004348c8 str r3, [r0, #-4]!
004348cc str r6, [r4, #0x24]
004348d0 bl #0x708ec0
004348d4 sub r8, r8, #1
004348d8 lsr r8, r8, #1
004348dc str r0, [r6, r8, lsl #2]
004348e0 add r3, r6, r8, lsl #2
004348e4 str r3, [r4, #0x10]
004348e8 ldr ip, [r6, r8, lsl #2]
004348ec str r3, [r4, #0x20]
004348f0 ldr r2, [pc, #0x474]
004348f4 add r3, ip, #0x70
004348f8 str r3, [r4, #0xc]
004348fc str ip, [r4, #8]
00434900 ldr r3, [r6, r8, lsl #2]
00434904 ldr r1, [r5, r2]
00434908 mov r0, r4
0043490c add lr, r3, #0x70
00434910 ldr r2, [r5, r7]
00434914 str lr, [r4, #0x1c]
00434918 str ip, [r4, #4]
0043491c str r3, [r4, #0x14]
00434920 str r3, [r4, #0x18]
00434924 bl #0x30e304
00434928 ldr r3, [pc, #0x440]
0043492c ldr ip, [r5, r3]
00434930 ldr r3, [ip]
00434934 ands r3, r3, #1
00434938 bne #0x434a08
0043493c ldr r6, [pc, #0x430]
00434940 ldr r2, [pc, #0x430]
00434944 mov lr, #8
00434948 ldr r6, [r5, r6]
0043494c ldr r4, [r5, r2]
00434950 mov r8, #1
00434954 add r6, r6, lr
00434958 str r8, [ip]
0043495c mov r1, lr
00434960 mov r2, r3
00434964 str lr, [r4, #0x28]
00434968 str r6, [r4]
0043496c str r3, [r4, #4]
00434970 str r3, [r4, #8]
00434974 str r3, [r4, #0xc]
00434978 str r3, [r4, #0x10]
0043497c str r3, [r4, #0x14]
00434980 str r3, [r4, #0x18]
00434984 str r3, [r4, #0x1c]
00434988 str r3, [r4, #0x20]
0043498c str r3, [r4, #0x24]
00434990 add r0, r4, #0x24
00434994 bl #0x3295d0
00434998 mov r3, #0x80
0043499c mov r6, r0
004349a0 add r0, sp, #0x18
004349a4 ldr r8, [r4, #0x28]
004349a8 str r3, [r0, #-4]!
004349ac str r6, [r4, #0x24]
004349b0 bl #0x708ec0
004349b4 sub r8, r8, #1
004349b8 lsr r8, r8, #1
004349bc str r0, [r6, r8, lsl #2]
004349c0 add r3, r6, r8, lsl #2
004349c4 str r3, [r4, #0x10]
004349c8 ldr ip, [r6, r8, lsl #2]
004349cc str r3, [r4, #0x20]
004349d0 ldr r2, [pc, #0x3a4]
004349d4 add r3, ip, #0x80
004349d8 str r3, [r4, #0xc]
004349dc str ip, [r4, #8]
004349e0 ldr r3, [r6, r8, lsl #2]
004349e4 ldr r1, [r5, r2]
004349e8 mov r0, r4
004349ec add lr, r3, #0x80
004349f0 ldr r2, [r5, r7]
004349f4 str lr, [r4, #0x1c]
004349f8 str ip, [r4, #4]
004349fc str r3, [r4, #0x14]
00434a00 str r3, [r4, #0x18]
00434a04 bl #0x30e304
00434a08 ldr r3, [pc, #0x370]
00434a0c ldr ip, [r5, r3]
00434a10 ldr r3, [ip]
00434a14 ands r3, r3, #1
00434a18 bne #0x434ae8
00434a1c ldr r6, [pc, #0x360]
00434a20 ldr r2, [pc, #0x360]
00434a24 mov lr, #8
00434a28 ldr r6, [r5, r6]
00434a2c ldr r4, [r5, r2]
00434a30 mov r8, #1
00434a34 add r6, r6, lr
00434a38 str r8, [ip]
00434a3c mov r1, lr
00434a40 mov r2, r3
00434a44 str lr, [r4, #0x28]
00434a48 str r6, [r4]
00434a4c str r3, [r4, #4]
00434a50 str r3, [r4, #8]
00434a54 str r3, [r4, #0xc]
00434a58 str r3, [r4, #0x10]
00434a5c str r3, [r4, #0x14]
00434a60 str r3, [r4, #0x18]
00434a64 str r3, [r4, #0x1c]
00434a68 str r3, [r4, #0x20]
00434a6c str r3, [r4, #0x24]
00434a70 add r0, r4, #0x24
00434a74 bl #0x329368
00434a78 mov r3, #0x4c
00434a7c mov r6, r0
00434a80 add r0, sp, #0x18
00434a84 ldr r8, [r4, #0x28]
00434a88 str r3, [r0, #-4]!
00434a8c str r6, [r4, #0x24]
00434a90 bl #0x708ec0
00434a94 sub r8, r8, #1
00434a98 lsr r8, r8, #1
00434a9c str r0, [r6, r8, lsl #2]
00434aa0 add r3, r6, r8, lsl #2
00434aa4 str r3, [r4, #0x10]
00434aa8 ldr ip, [r6, r8, lsl #2]
00434aac str r3, [r4, #0x20]
00434ab0 ldr r2, [pc, #0x2d4]
00434ab4 add r3, ip, #0x4c
00434ab8 str r3, [r4, #0xc]
00434abc str ip, [r4, #8]
00434ac0 ldr r3, [r6, r8, lsl #2]
00434ac4 ldr r1, [r5, r2]
00434ac8 mov r0, r4
00434acc add lr, r3, #0x4c
00434ad0 ldr r2, [r5, r7]
00434ad4 str lr, [r4, #0x1c]
00434ad8 str ip, [r4, #4]
00434adc str r3, [r4, #0x14]
00434ae0 str r3, [r4, #0x18]
00434ae4 bl #0x30e304
00434ae8 ldr r3, [pc, #0x2a0]
00434aec ldr ip, [r5, r3]
00434af0 ldr r3, [ip]
00434af4 ands r3, r3, #1
00434af8 bne #0x434bc8
00434afc ldr r6, [pc, #0x290]
00434b00 ldr r2, [pc, #0x290]
00434b04 mov lr, #8
00434b08 ldr r6, [r5, r6]
00434b0c ldr r4, [r5, r2]
00434b10 mov r8, #1
00434b14 add r6, r6, lr
00434b18 str r8, [ip]
00434b1c mov r1, lr
00434b20 mov r2, r3
00434b24 str lr, [r4, #0x28]
00434b28 str r6, [r4]
00434b2c str r3, [r4, #4]
00434b30 str r3, [r4, #8]
00434b34 str r3, [r4, #0xc]
00434b38 str r3, [r4, #0x10]
00434b3c str r3, [r4, #0x14]
00434b40 str r3, [r4, #0x18]
00434b44 str r3, [r4, #0x1c]
00434b48 str r3, [r4, #0x20]
00434b4c str r3, [r4, #0x24]
00434b50 add r0, r4, #0x24
00434b54 bl #0x3293c8
00434b58 mov r3, #0x68
00434b5c mov r6, r0
00434b60 add r0, sp, #0x18
00434b64 ldr r8, [r4, #0x28]
00434b68 str r3, [r0, #-4]!
00434b6c str r6, [r4, #0x24]
00434b70 bl #0x708ec0
00434b74 sub r8, r8, #1
00434b78 lsr r8, r8, #1
00434b7c str r0, [r6, r8, lsl #2]
00434b80 add r3, r6, r8, lsl #2
00434b84 str r3, [r4, #0x10]
00434b88 ldr ip, [r6, r8, lsl #2]
00434b8c str r3, [r4, #0x20]
00434b90 ldr r2, [pc, #0x204]
00434b94 add r3, ip, #0x68
00434b98 str r3, [r4, #0xc]
00434b9c str ip, [r4, #8]
00434ba0 ldr r3, [r6, r8, lsl #2]
00434ba4 ldr r1, [r5, r2]
00434ba8 mov r0, r4
00434bac add lr, r3, #0x68
00434bb0 ldr r2, [r5, r7]
00434bb4 str lr, [r4, #0x1c]
00434bb8 str ip, [r4, #4]
00434bbc str r3, [r4, #0x14]
00434bc0 str r3, [r4, #0x18]
00434bc4 bl #0x30e304
00434bc8 ldr r3, [pc, #0x1d0]
00434bcc ldr ip, [r5, r3]
00434bd0 ldr r3, [ip]
00434bd4 ands r3, r3, #1
00434bd8 bne #0x434ca8
00434bdc ldr r6, [pc, #0x1c0]
00434be0 ldr r2, [pc, #0x1c0]
00434be4 mov lr, #8
00434be8 ldr r6, [r5, r6]
00434bec ldr r4, [r5, r2]
00434bf0 mov r8, #1
00434bf4 add r6, r6, lr
00434bf8 str r8, [ip]
00434bfc mov r1, lr
00434c00 mov r2, r3
00434c04 str lr, [r4, #0x28]
00434c08 str r6, [r4]
00434c0c str r3, [r4, #4]
00434c10 str r3, [r4, #8]
00434c14 str r3, [r4, #0xc]
00434c18 str r3, [r4, #0x10]
00434c1c str r3, [r4, #0x14]
00434c20 str r3, [r4, #0x18]
00434c24 str r3, [r4, #0x1c]
00434c28 str r3, [r4, #0x20]
00434c2c str r3, [r4, #0x24]
00434c30 add r0, r4, #0x24
00434c34 bl #0x329428
00434c38 mov r3, #0x78
00434c3c mov r6, r0
00434c40 add r0, sp, #0x18
00434c44 ldr r8, [r4, #0x28]
00434c48 str r3, [r0, #-4]!
00434c4c str r6, [r4, #0x24]
00434c50 bl #0x708ec0
00434c54 sub r8, r8, #1
00434c58 lsr r8, r8, #1
00434c5c str r0, [r6, r8, lsl #2]
00434c60 add r3, r6, r8, lsl #2
00434c64 str r3, [r4, #0x10]
00434c68 ldr ip, [r6, r8, lsl #2]
00434c6c str r3, [r4, #0x20]
00434c70 ldr r1, [pc, #0x134]
00434c74 add r3, ip, #0x78
00434c78 str r3, [r4, #0xc]
00434c7c str ip, [r4, #8]
00434c80 ldr r3, [r6, r8, lsl #2]
00434c84 ldr r2, [r5, r7]
00434c88 mov r0, r4
00434c8c add lr, r3, #0x78
00434c90 ldr r1, [r5, r1]
00434c94 str lr, [r4, #0x1c]
00434c98 str ip, [r4, #4]
00434c9c str r3, [r4, #0x14]
00434ca0 str r3, [r4, #0x18]
00434ca4 bl #0x30e304
00434ca8 add sp, sp, #0x1c
00434cac pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00434cb0 mov r2, #1
00434cb4 str r2, [r3]
00434cb8 ldr r3, [pc, #0xf0]
00434cbc ldr r6, [r5, r3]
00434cc0 mov r0, r6
00434cc4 bl #0x3790a8
00434cc8 ldr r3, [pc, #0xe4]
00434ccc mov r2, r4
00434cd0 mov r0, r6
00434cd4 ldr r1, [r5, r3]
00434cd8 bl #0x30e304
00434cdc ldr r3, [pc, #0x68]
00434ce0 ldr r3, [r5, r3]
00434ce4 ldr r2, [r3]
00434ce8 tst r2, #1
00434cec bne #0x434744
00434cf0 mov r2, #1
00434cf4 str r2, [r3]
00434cf8 ldr r3, [pc, #0xb8]
00434cfc ldr r4, [r5, r3]
00434d00 mov r0, r4
00434d04 bl #0x32d79c
00434d08 ldr r3, [pc, #0xac]
00434d0c mov r0, r4
00434d10 ldr r2, [r5, r7]
00434d14 ldr r1, [r5, r3]
00434d18 bl #0x30e304
00434d1c b #0x434744
00434d20 subseq r0, r6, ip, asr r4
00434d24 andeq r2, r0, r0, ror r0
00434d28 subseq r0, r7, r0, ror #26
00434d2c andeq r2, r0, r4, asr sl
00434d30 muleq r0, r0, r8
00434d34 andeq r3, r0, r4, asr #11
00434d38 andeq r0, r0, r8, lsl #18
00434d3c andeq r2, r0, r4, lsr r2
00434d40 andeq r4, r0, r4, asr r0
00434d44 andeq r4, r0, r8, lsl #10
00434d48 strdeq r0, r1, [r0], -r4
00434d4c andeq r0, r0, ip, lsr #31
00434d50 andeq r2, r0, ip, asr sb
00434d54 andeq r1, r0, ip, ror #9
00434d58 ldrdeq r1, r2, [r0], -r0
00434d5c strheq r3, [r0], -ip
00434d60 andeq r2, r0, r4, lsl r2
00434d64 andeq r2, r0, r0, lsl #4
00434d68 ldrdeq r0, r1, [r0], -r8
00434d6c strdeq r2, r3, [r0], -r4
00434d70 andeq r1, r0, r0, lsl r3
00434d74 andeq r3, r0, r8, ror r1
00434d78 andeq r1, r0, r0, lsl ip
00434d7c andeq r1, r0, r8, lsl #18
00434d80 andeq r3, r0, ip, lsr #18
00434d84 andeq r1, r0, r8, ror r4
00434d88 andeq r1, r0, r4, ror lr
00434d8c muleq r0, r4, sb
00434d90 muleq r0, r8, r6
00434d94 andeq r1, r0, r8, ror r1
00434d98 andeq r4, r0, r0, lsl #18
00434d9c strdeq r1, r2, [r0], -r8
00434da0 andeq r3, r0, r8, asr r8
00434da4 andeq r2, r0, r0, asr #5
00434da8 strheq r2, [r0], -ip
00434dac andeq r1, r0, ip, asr #28
00434db0 andeq r2, r0, r4, lsl r7
00434db4 muleq r0, ip, r5
00434db8 strdeq r3, r4, [r0], -r4
00434dbc andeq r0, r0, r0, asr #17
# _Z26NativeGetNextStatusMessageRKN7gameswf7fn_callE 43fec4 444
0043fec4 push {r4, r5, r6, r7, r8, sb, sl, lr}
0043fec8 ldr r4, [pc, #0x19c]
0043fecc ldr r5, [pc, #0x19c]
0043fed0 ldr r7, [r0, #0x10]
0043fed4 add r4, pc, r4
0043fed8 ldr r3, [r4, r5]
0043fedc sub r2, r7, #1
0043fee0 sub sp, sp, #0x50
0043fee4 ldr r3, [r3]
0043fee8 cmp r2, #1
0043feec mov r6, r0
0043fef0 str r3, [sp, #0x4c]
0043fef4 bls #0x43ff14
0043fef8 ldr r3, [r4, r5]
0043fefc ldr r2, [sp, #0x4c]
0043ff00 ldr r3, [r3]
0043ff04 cmp r2, r3
0043ff08 bne #0x440068
0043ff0c add sp, sp, #0x50
0043ff10 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0043ff14 ldr r8, [r0, #0xc]
0043ff18 ldr sl, [r0, #0x14]
0043ff1c mov sb, #0xc
0043ff20 ldr r3, [r8]
0043ff24 mla r3, sb, sl, r3
0043ff28 ldrsb r2, [r3, #1]
0043ff2c cmp r2, #2
0043ff30 bne #0x43fef8
0043ff34 ldr ip, [r3, #8]
0043ff38 ldr lr, [r3, #4]
0043ff3c str ip, [sp, #4]
0043ff40 str lr, [sp]
0043ff44 ldrd r0, r1, [sp]
0043ff48 mov r2, r0
0043ff4c mov r3, r1
0043ff50 str ip, [sp, #0x10]
0043ff54 str lr, [sp, #0xc]
0043ff58 bl #0x30e2bc
0043ff5c cmp r0, #0
0043ff60 bne #0x43fef8
0043ff64 cmp r7, #2
0043ff68 beq #0x440024
0043ff6c ldr r1, [pc, #0x100]
0043ff70 add sl, sp, #0x34
0043ff74 add r2, sp, #0x14
0043ff78 add r1, pc, r1
0043ff7c mov r0, sl
0043ff80 bl #0x3140ec
0043ff84 ldr r3, [pc, #0xec]
0043ff88 mov r1, #0
0043ff8c mov r2, r1
0043ff90 ldr r3, [r4, r3]
0043ff94 ldr r0, [r3, #0x40]
0043ff98 bl #0x36e478
0043ff9c ldr r3, [r6, #0x10]
0043ffa0 cmp r3, #2
0043ffa4 beq #0x440048
0043ffa8 add r7, sp, #0x18
0043ffac mov r1, #0x10
0043ffb0 mov r0, r7
0043ffb4 str r7, [sp, #0x28]
0043ffb8 str r7, [sp, #0x2c]
0043ffbc bl #0x31167c
0043ffc0 ldr r3, [pc, #0xb4]
0043ffc4 ldr r2, [sp, #0x28]
0043ffc8 mov r1, #0
0043ffcc ldr r3, [r4, r3]
0043ffd0 strb r1, [r2]
0043ffd4 ldr r2, [r3, #0x14]
0043ffd8 ldr r8, [r3, #4]
0043ffdc cmp r2, r8
0043ffe0 beq #0x440010
0043ffe4 cmp r8, r7
0043ffe8 beq #0x43fffc
0043ffec mov r0, r7
0043fff0 ldr r1, [r8, #0x14]
0043fff4 ldr r2, [r8, #0x10]
0043fff8 bl #0x3109e0
0043fffc ldr r3, [r8, #0x18]
00440000 ldr r0, [r6]
00440004 ldr r1, [sp, #0x2c]
00440008 str r3, [sp, #0x30]
0044000c bl #0x797350
00440010 mov r0, r7
00440014 bl #0x3139ac
00440018 mov r0, sl
0044001c bl #0x3139ac
00440020 b #0x43fef8
00440024 ldr r3, [r8]
00440028 sub sl, sl, #1
0044002c mla sb, sb, sl, r3
00440030 ldrb r3, [sb, #1]
00440034 cmp r3, #1
00440038 beq #0x43ff6c
0044003c cmp r3, #0
00440040 bne #0x43fef8
00440044 b #0x43ff6c
00440048 ldr r3, [r6, #0xc]
0044004c ldr r2, [r6, #0x14]
00440050 mov r0, #0xc
00440054 ldr r3, [r3]
00440058 sub r2, r2, #1
0044005c mla r0, r0, r2, r3
00440060 bl #0x797960
00440064 b #0x43ffa8
00440068 bl #0x30e310
0044006c ldrheq r4, [r5], #-0xbc
00440070 andeq r4, r0, ip, lsr #1
00440074 subeq fp, r8, r0, lsr #27
00440078 strdeq r3, r4, [r0], -r4
0044007c andeq r1, r0, ip, ror #9
# _ZNK18MenuMessageManagerI9StatusMsgLi4EE6InvokeEPKci.clone.52 442c9c 260
00442c9c push {r4, r5, r6, r7, lr}
00442ca0 sub sp, sp, #0x24
00442ca4 mov r6, r0
00442ca8 bl #0x42ca8c
00442cac bl #0x42cb8c
00442cb0 ldr r4, [pc, #0xdc]
00442cb4 subs r5, r0, #0
00442cb8 add r4, pc, r4
00442cbc beq #0x442d40
00442cc0 ldr r7, [pc, #0xd0]
00442cc4 ldr r3, [r4, r7]
00442cc8 ldr r2, [r3, #0x2c]
00442ccc cmp r2, #0
00442cd0 beq #0x442d6c
00442cd4 ldr r0, [r3, #0x28]
00442cd8 ldrb r3, [r0, #4]
00442cdc cmp r3, #0
00442ce0 beq #0x442d48
00442ce4 ldr r0, [r4, r7]
00442ce8 bl #0x427d50
00442cec mov ip, #0
00442cf0 mov r2, #0
00442cf4 mov r3, #0
00442cf8 strb ip, [sp, #0xc]
00442cfc mov ip, #2
00442d00 strd r2, r3, [sp, #0x18]
00442d04 strb ip, [sp, #0xd]
00442d08 mov ip, #0
00442d0c str ip, [sp, #0x10]
00442d10 ldr ip, [sp, #0x1c]
00442d14 add r4, sp, #0xc
00442d18 mov r1, r0
00442d1c str ip, [r4, #8]
00442d20 mov r0, r5
00442d24 mov ip, #1
00442d28 mov r2, r6
00442d2c mov r3, r4
00442d30 str ip, [sp]
00442d34 bl #0x7abe0c
00442d38 mov r0, r4
00442d3c bl #0x797124
00442d40 add sp, sp, #0x24
00442d44 pop {r4, r5, r6, r7, pc}
00442d48 ldr r1, [r0]
00442d4c sub r1, r1, #1
00442d50 cmp r1, #0
00442d54 str r1, [r0]
00442d58 beq #0x442d8c
00442d5c ldr r3, [r4, r7]
00442d60 mov r2, #0
00442d64 str r2, [r3, #0x2c]
00442d68 str r2, [r3, #0x28]
00442d6c ldr r3, [pc, #0x28]
00442d70 ldr r0, [r4, r7]
00442d74 mov r2, r5
00442d78 ldr r1, [r4, r3]
00442d7c mov r3, #0
00442d80 ldr r1, [r1]
00442d84 bl #0x427ca0
00442d88 b #0x442ce4
00442d8c bl #0x752b38
00442d90 b #0x442d5c
00442d94 ldrsbeq r1, [r5], #-0xd8
00442d98 andeq r2, r0, r0, ror r0
00442d9c muleq r0, r4, r5
# _Z17NativeStopMessageRKN7gameswf7fn_callE 442ea4 1448
00442ea4 push {r4, r5, r6, r7, r8, sl, lr}
00442ea8 ldr r4, [pc, #0x52c]
00442eac ldr r6, [pc, #0x52c]
00442eb0 ldr r2, [r0, #0x10]
00442eb4 add r4, pc, r4
00442eb8 ldr r3, [r4, r6]
00442ebc sub sp, sp, #0x3c
00442ec0 cmp r2, #0
00442ec4 ldr r3, [r3]
00442ec8 mov r5, r0
00442ecc str r3, [sp, #0x34]
00442ed0 ble #0x442efc
00442ed4 ldr r2, [r0, #0xc]
00442ed8 ldr r3, [r0, #0x14]
00442edc mov r1, #0xc
00442ee0 ldr r2, [r2]
00442ee4 mla r3, r1, r3, r2
00442ee8 ldrb r3, [r3, #1]
00442eec sub r3, r3, #3
00442ef0 uxtb r3, r3
00442ef4 cmp r3, #1
00442ef8 bls #0x442f18
00442efc ldr r3, [r4, r6]
00442f00 ldr r2, [sp, #0x34]
00442f04 ldr r3, [r3]
00442f08 cmp r2, r3
00442f0c bne #0x4433d8
00442f10 add sp, sp, #0x3c
00442f14 pop {r4, r5, r6, r7, r8, sl, pc}
00442f18 ldr r8, [pc, #0x4c4]
00442f1c add r8, pc, r8
00442f20 ldr r3, [r8, #0x1c]
00442f24 tst r3, #1
00442f28 beq #0x443124
00442f2c ldr r8, [pc, #0x4b4]
00442f30 add r8, pc, r8
00442f34 ldr r3, [r8, #0x38]
00442f38 tst r3, #1
00442f3c beq #0x4430d4
00442f40 ldr r8, [pc, #0x4a4]
00442f44 add r8, pc, r8
00442f48 ldr r3, [r8, #0x54]
00442f4c tst r3, #1
00442f50 beq #0x443084
00442f54 ldr r8, [pc, #0x494]
00442f58 add r8, pc, r8
00442f5c ldr r3, [r8, #0x70]
00442f60 tst r3, #1
00442f64 beq #0x4431c4
00442f68 ldr r8, [pc, #0x484]
00442f6c add r8, pc, r8
00442f70 ldr r3, [r8, #0x8c]
00442f74 tst r3, #1
00442f78 beq #0x443174
00442f7c ldr r3, [r5, #0xc]
00442f80 ldr r2, [r5, #0x14]
00442f84 mov r0, #0xc
00442f88 ldr r3, [r3]
00442f8c ldr sl, [pc, #0x464]
00442f90 add r5, sp, #0x1c
00442f94 mla r0, r0, r2, r3
00442f98 bl #0x796f5c
00442f9c add r2, sp, #4
00442fa0 mov r1, r0
00442fa4 mov r0, r5
00442fa8 bl #0x3140ec
00442fac ldr r3, [r4, sl]
00442fb0 mov r1, #0
00442fb4 mov r2, r1
00442fb8 ldr r0, [r3, #0x40]
00442fbc bl #0x36e478
00442fc0 ldr r3, [pc, #0x434]
00442fc4 ldr r8, [sp, #0x30]
00442fc8 ldr r7, [sp, #0x2c]
00442fcc add r3, pc, r3
00442fd0 ldr r2, [r3, #0x30]
00442fd4 ldr r1, [r3, #0x34]
00442fd8 rsb r7, r8, r7
00442fdc rsb r3, r1, r2
00442fe0 cmp r7, r3
00442fe4 beq #0x443214
00442fe8 ldr r3, [pc, #0x410]
00442fec add r3, pc, r3
00442ff0 ldr r2, [r3, #0x4c]
00442ff4 ldr r1, [r3, #0x50]
00442ff8 rsb r3, r1, r2
00442ffc cmp r7, r3
00443000 beq #0x443288
00443004 ldr r7, [pc, #0x3f8]
00443008 mov r0, r5
0044300c add r7, pc, r7
00443010 add r1, r7, #0x58
00443014 bl #0x43a7d4
00443018 cmp r0, #0
0044301c beq #0x443314
00443020 bl #0x7fd794
00443024 ldrb r3, [r0, #5]
00443028 cmp r3, #0
0044302c beq #0x44307c
00443030 ldr r3, [r4, sl]
00443034 ldr r0, [r3, #0x40]
00443038 bl #0x36f074
0044303c cmp r0, #0
00443040 beq #0x443308
00443044 bl #0x80b1bc
00443048 mov r7, r0
0044304c ldr r0, [pc, #0x3b4]
00443050 mov r1, #1
00443054 add r0, pc, r0
00443058 bl #0x80a244
0044305c mvn r3, #0
00443060 mov r2, #3
00443064 mov r1, r0
00443068 str r2, [r0, #0x50]
0044306c str r3, [r0, #0x58]
00443070 str r3, [r0, #0x54]
00443074 mov r0, r7
00443078 bl #0x80e2a4
0044307c bl #0x442c1c
00443080 b #0x443308
00443084 add r7, r8, #0x54
00443088 mov r0, r7
0044308c bl #0x30e76c
00443090 cmp r0, #0
00443094 beq #0x442f54
00443098 ldr r1, [pc, #0x36c]
0044309c add r8, r8, #0x58
004430a0 add r2, sp, #0x10
004430a4 add r1, pc, r1
004430a8 mov r0, r8
004430ac bl #0x3140ec
004430b0 mov r0, r7
004430b4 bl #0x30ea3c
004430b8 ldr r3, [pc, #0x350]
004430bc mov r0, r8
004430c0 ldr r1, [r4, r3]
004430c4 ldr r3, [pc, #0x348]
004430c8 ldr r2, [r4, r3]
004430cc bl #0x30e304
004430d0 b #0x442f54
004430d4 add r7, r8, #0x38
004430d8 mov r0, r7
004430dc bl #0x30e76c
004430e0 cmp r0, #0
004430e4 beq #0x442f40
004430e8 ldr r1, [pc, #0x328]
004430ec add r8, r8, #0x3c
004430f0 add r2, sp, #0x14
004430f4 add r1, pc, r1
004430f8 mov r0, r8
004430fc bl #0x3140ec
00443100 mov r0, r7
00443104 bl #0x30ea3c
00443108 ldr r3, [pc, #0x300]
0044310c mov r0, r8
00443110 ldr r1, [r4, r3]
00443114 ldr r3, [pc, #0x2f8]
00443118 ldr r2, [r4, r3]
0044311c bl #0x30e304
00443120 b #0x442f40
00443124 add r7, r8, #0x1c
00443128 mov r0, r7
0044312c bl #0x30e76c
00443130 cmp r0, #0
00443134 beq #0x442f2c
00443138 ldr r1, [pc, #0x2dc]
0044313c add r8, r8, #0x20
00443140 add r2, sp, #0x18
00443144 add r1, pc, r1
00443148 mov r0, r8
0044314c bl #0x3140ec
00443150 mov r0, r7
00443154 bl #0x30ea3c
00443158 ldr r3, [pc, #0x2b0]
0044315c mov r0, r8
00443160 ldr r1, [r4, r3]
00443164 ldr r3, [pc, #0x2a8]
00443168 ldr r2, [r4, r3]
0044316c bl #0x30e304
00443170 b #0x442f2c
00443174 add r7, r8, #0x8c
00443178 mov r0, r7
0044317c bl #0x30e76c
00443180 cmp r0, #0
00443184 beq #0x442f7c
00443188 ldr r1, [pc, #0x290]
0044318c add r8, r8, #0x90
00443190 add r2, sp, #8
00443194 add r1, pc, r1
00443198 mov r0, r8
0044319c bl #0x3140ec
004431a0 mov r0, r7
004431a4 bl #0x30ea3c
004431a8 ldr r3, [pc, #0x260]
004431ac mov r0, r8
004431b0 ldr r1, [r4, r3]
004431b4 ldr r3, [pc, #0x258]
004431b8 ldr r2, [r4, r3]
004431bc bl #0x30e304
004431c0 b #0x442f7c
004431c4 add r7, r8, #0x70
004431c8 mov r0, r7
004431cc bl #0x30e76c
004431d0 cmp r0, #0
004431d4 beq #0x442f68
004431d8 ldr r1, [pc, #0x244]
004431dc add r8, r8, #0x74
004431e0 add r2, sp, #0xc
004431e4 add r1, pc, r1
004431e8 mov r0, r8
004431ec bl #0x3140ec
004431f0 mov r0, r7
004431f4 bl #0x30ea3c
004431f8 ldr r3, [pc, #0x210]
004431fc mov r0, r8
00443200 ldr r1, [r4, r3]
00443204 ldr r3, [pc, #0x208]
00443208 ldr r2, [r4, r3]
0044320c bl #0x30e304
00443210 b #0x442f68
00443214 mov r0, r8
00443218 mov r2, r7
0044321c bl #0x30e5e0
00443220 cmp r0, #0
00443224 bne #0x442fe8
00443228 ldr r7, [pc, #0x1f8]
0044322c ldr r0, [r4, r7]
00443230 ldr r2, [r0, #0x14]
00443234 ldr r3, [r0, #4]
00443238 cmp r2, r3
0044323c beq #0x443308
00443240 add r0, r0, #4
00443244 bl #0x383d10
00443248 ldr r3, [pc, #0x1dc]
0044324c ldr r3, [r4, r3]
00443250 ldr r0, [r3]
00443254 cmp r0, #0
00443258 beq #0x443260
0044325c bl #0x442c9c
00443260 ldr r3, [r4, r7]
00443264 ldr r2, [r3, #4]
00443268 ldr r3, [r3, #0x14]
0044326c cmp r3, r2
00443270 beq #0x443308
00443274 ldr r3, [pc, #0x1b4]
00443278 ldr r3, [r4, r3]
0044327c ldr r0, [r3]
00443280 bl #0x442c9c
00443284 b #0x443308
00443288 mov r0, r8
0044328c mov r2, r7
00443290 bl #0x30e5e0
00443294 cmp r0, #0
00443298 bne #0x443004
0044329c ldr r7, [pc, #0x190]
004432a0 ldr r3, [r4, r7]
004432a4 ldr r2, [r3, #4]
004432a8 ldr r1, [r3, #0x14]
004432ac cmp r1, r2
004432b0 beq #0x443308
004432b4 ldr r1, [r3, #0xc]
004432b8 sub r1, r1, #8
004432bc cmp r2, r1
004432c0 addne r2, r2, #8
004432c4 strne r2, [r3, #4]
004432c8 beq #0x44339c
004432cc ldr r3, [pc, #0x164]
004432d0 ldr r3, [r4, r3]
004432d4 ldr r0, [r3]
004432d8 cmp r0, #0
004432dc beq #0x4432e4
004432e0 bl #0x442a14
004432e4 ldr r3, [r4, r7]
004432e8 ldr r2, [r3, #4]
004432ec ldr r3, [r3, #0x14]
004432f0 cmp r3, r2
004432f4 beq #0x443308
004432f8 ldr r3, [pc, #0x13c]
004432fc ldr r3, [r4, r3]
00443300 ldr r0, [r3]
00443304 bl #0x442a14
00443308 mov r0, r5
0044330c bl #0x3139ac
00443310 b #0x442efc
00443314 mov r0, r5
00443318 add r1, r7, #0x74
0044331c bl #0x43a7d4
00443320 cmp r0, #0
00443324 bne #0x44307c
00443328 add r1, r7, #0x90
0044332c mov r0, r5
00443330 bl #0x43a7d4
00443334 cmp r0, #0
00443338 beq #0x443308
0044333c ldr r7, [pc, #0xfc]
00443340 ldr r0, [r4, r7]
00443344 ldr r2, [r0, #0x14]
00443348 ldr r3, [r0, #4]
0044334c cmp r2, r3
00443350 beq #0x443308
00443354 add r0, r0, #4
00443358 bl #0x383da8
0044335c ldr r3, [pc, #0xe0]
00443360 ldr r3, [r4, r3]
00443364 ldr r0, [r3]
00443368 cmp r0, #0
0044336c beq #0x443374
00443370 bl #0x442da0
00443374 ldr r3, [r4, r7]
00443378 ldr r2, [r3, #4]
0044337c ldr r3, [r3, #0x14]
00443380 cmp r3, r2
00443384 beq #0x443308
00443388 ldr r3, [pc, #0xb8]
0044338c ldr r3, [r4, r3]
00443390 ldr r0, [r3]
00443394 bl #0x442da0
00443398 b #0x443308
0044339c ldr r0, [r3, #8]
004433a0 cmp r0, #0
004433a4 beq #0x4433b0
004433a8 mov r1, #0x80
004433ac bl #0x31bb44
004433b0 ldr r3, [r4, r7]
004433b4 ldr r2, [r3, #0x10]
004433b8 add r1, r2, #4
004433bc str r1, [r3, #0x10]
004433c0 ldr r2, [r2, #4]
004433c4 add r1, r2, #0x80
004433c8 str r2, [r3, #4]
004433cc str r1, [r3, #0xc]
004433d0 str r2, [r3, #8]
004433d4 b #0x4432cc
004433d8 bl #0x30e310
004433dc ldrsbeq r1, [r5], #-0xbc
004433e0 andeq r4, r0, ip, lsr #1
004433e4 subseq r2, r6, r8, lsr ip
004433e8 subseq r2, r6, r4, lsr #24
004433ec subseq r2, r6, r0, lsl ip
004433f0 ldrsheq r2, [r6], #-0xbc
004433f4 subseq r2, r6, r8, ror #23
004433f8 strdeq r3, r4, [r0], -r4
004433fc subseq r2, r6, r8, lsl #23
00443400 subseq r2, r6, r8, ror #22
00443404 subseq r2, r6, r8, asr #22
00443408 subeq fp, r7, ip, ror #28
0044340c subeq r8, r8, ip, lsr #11
00443410 andeq r1, r0, r8, lsr #6
00443414 muleq r0, r0, r8
00443418 subeq r8, r8, r4, asr sp
0044341c strdeq r8, sb, [r8], #-0xcc
00443420 subeq r8, r8, ip, asr #25
00443424 subeq r8, r8, r4, ror ip
00443428 andeq r1, r0, ip, ror #9
0044342c strdeq r3, r4, [r0], -r0
00443430 andeq r4, r0, ip, lsr #21
00443434 andeq r1, r0, r0, lsl ip
00443438 strheq r1, [r0], -r8
0044343c andeq r3, r0, r8, lsr r3
00443440 ldrdeq r0, r1, [r0], -r8
00443444 andeq r3, r0, r4, ror r7
00443448 andeq r4, r0, ip, asr #4
