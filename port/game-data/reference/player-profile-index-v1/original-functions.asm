_ZN8Savegame10_cacheFileEP12StreamBuffer 0x315ad0 1032
00315ad0 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315ad4 ldr r3, [r0, #0x1c]
00315ad8 ldr r6, [pc, #0x3ec]
00315adc sub sp, sp, #0x1c
00315ae0 cmp r3, #0
00315ae4 mov r4, r0
00315ae8 mov r5, r1
00315aec add r6, pc, r6
00315af0 beq #0x315b0c
00315af4 mov r0, r3
00315af8 ldr r3, [r3]
00315afc mov lr, pc
00315b00 ldr pc, [r3, #4]
00315b04 mov r3, #0
00315b08 str r3, [r4, #0x1c]
00315b0c cmp r5, #0
00315b10 beq #0x315e30
00315b14 mov r0, r5
00315b18 mov r2, #0
00315b1c mov r3, #0
00315b20 ldr r1, [r5]
00315b24 mov lr, pc
00315b28 ldr pc, [r1, #0x20]
00315b2c mov r2, #0
00315b30 mov r3, #0
00315b34 mov r0, r5
00315b38 ldr r1, [r5]
00315b3c mov lr, pc
00315b40 ldr pc, [r1, #0x2c]
00315b44 mov r1, #0
00315b48 mov r0, #0x30
00315b4c bl #0x310570
00315b50 mov r1, r5
00315b54 mov r7, r0
00315b58 bl #0x3172d8
00315b5c str r7, [r4, #0x1c]
00315b60 ldrb r3, [r4, #0x38]
00315b64 cmp r3, #0
00315b68 bne #0x315df4
00315b6c ldr r3, [r4, #0x1c]
00315b70 cmp r3, #0
00315b74 beq #0x315bbc
00315b78 mov r0, r3
00315b7c ldr r3, [r3]
00315b80 mov lr, pc
00315b84 ldr pc, [r3, #8]
00315b88 cmp r1, #0
00315b8c bne #0x315dfc
00315b90 cmp r0, #3
00315b94 bhi #0x315dfc
00315b98 ldr r3, [r4, #0x1c]
00315b9c cmp r3, #0
00315ba0 beq #0x315bbc
00315ba4 mov r0, r3
00315ba8 ldr r3, [r3]
00315bac mov lr, pc
00315bb0 ldr pc, [r3, #4]
00315bb4 mov r3, #0
00315bb8 str r3, [r4, #0x1c]
00315bbc ldr r3, [r4, #0x18]
00315bc0 ldr r0, [r4, #0x14]
00315bc4 mov r1, #0
00315bc8 ldr r7, [pc, #0x300]
00315bcc rsb r0, r3, r0
00315bd0 add r0, r0, #5
00315bd4 bl #0x31056c
00315bd8 ldr r1, [r4, #0x18]
00315bdc mov r5, r0
00315be0 bl #0x30e520
00315be4 mov r0, r5
00315be8 bl #0x30de54
00315bec ldr r1, [pc, #0x2e0]
00315bf0 mov r2, #5
00315bf4 add r0, r5, r0
00315bf8 add r1, pc, r1
00315bfc bl #0x30e868
00315c00 ldr r3, [r6, r7]
00315c04 mov r1, r5
00315c08 mov r2, #0
00315c0c ldr r3, [r3, #0x10]
00315c10 ldr r3, [r3, #0x34]
00315c14 mov r0, r3
00315c18 ldr r3, [r3]
00315c1c mov lr, pc
00315c20 ldr pc, [r3, #0x94]
00315c24 cmp r5, #0
00315c28 str r0, [sp, #0x14]
00315c2c beq #0x315c3c
00315c30 mov r0, r5
00315c34 bl #0x310440
00315c38 ldr r0, [sp, #0x14]
00315c3c cmp r0, #0
00315c40 beq #0x315c94
00315c44 mov r1, #0
00315c48 mov r0, #0x30
00315c4c bl #0x310570
00315c50 add r5, sp, #0x18
00315c54 ldr r1, [r5, #-4]!
00315c58 mov r8, r0
00315c5c bl #0x3172d8
00315c60 ldr r3, [r6, r7]
00315c64 str r8, [r4, #0x1c]
00315c68 mov r1, r5
00315c6c ldr r3, [r3, #0x10]
00315c70 ldr r3, [r3, #0x34]
00315c74 mov r0, r3
00315c78 ldr r3, [r3]
00315c7c mov lr, pc
00315c80 ldr pc, [r3, #0x78]
00315c84 ldr r0, [r4, #0x1c]
00315c88 bl #0x313a90
00315c8c cmn r0, #1
00315c90 beq #0x315ea4
00315c94 ldr r3, [r4, #0x1c]
00315c98 cmp r3, #0
00315c9c beq #0x315df4
00315ca0 mov r0, r3
00315ca4 ldr r3, [r3]
00315ca8 mov lr, pc
00315cac ldr pc, [r3, #8]
00315cb0 cmp r1, #0
00315cb4 bne #0x315cc0
00315cb8 cmp r0, #3
00315cbc bls #0x315df4
00315cc0 ldr r1, [r4, #0x1c]
00315cc4 mov r2, #0
00315cc8 mov r3, #0
00315ccc mov r0, r1
00315cd0 ldr r1, [r1]
00315cd4 mov lr, pc
00315cd8 ldr pc, [r1, #0x20]
00315cdc ldr r0, [r4, #0x1c]
00315ce0 bl #0x313a90
00315ce4 cmp r0, #0
00315ce8 str r0, [sp, #4]
00315cec beq #0x315df4
00315cf0 mov r5, #0
00315cf4 add r7, r4, #0x20
00315cf8 mov sb, r5
00315cfc add r8, sp, #0xc
00315d00 ldr r3, [r4, #0x1c]
00315d04 mov r0, r3
00315d08 ldr r3, [r3]
00315d0c mov lr, pc
00315d10 ldr pc, [r3, #0x24]
00315d14 ldr r3, [r4, #0x1c]
00315d18 mov sl, r0
00315d1c mov r6, r1
00315d20 mov r0, r3
00315d24 ldr r3, [r3]
00315d28 mov lr, pc
00315d2c ldr pc, [r3, #8]
00315d30 cmp r1, r6
00315d34 bhi #0x315d44
00315d38 bne #0x315df4
00315d3c cmp r0, sl
00315d40 bls #0x315df4
00315d44 ldr r0, [r4, #0x1c]
00315d48 bl #0x313a90
00315d4c mov r2, #4
00315d50 mov r3, #0
00315d54 mov r1, r8
00315d58 mov r6, r0
00315d5c ldr r0, [r4, #0x1c]
00315d60 strb sb, [sp, #0xc]
00315d64 strb sb, [sp, #0xd]
00315d68 strb sb, [sp, #0xe]
00315d6c strb sb, [sp, #0xf]
00315d70 strb sb, [sp, #0x10]
00315d74 bl #0x317454
00315d78 ldr r3, [r4, #0x1c]
00315d7c mov r0, r3
00315d80 ldr r3, [r3]
00315d84 mov lr, pc
00315d88 ldr pc, [r3, #0x24]
00315d8c mov sl, r0
00315d90 mov fp, r1
00315d94 mov r0, r7
00315d98 mov r1, r8
00315d9c bl #0x314380
00315da0 cmp r7, r0
00315da4 mov r1, r8
00315da8 beq #0x315e10
00315dac mov r0, r7
00315db0 bl #0x315978
00315db4 mov r1, r8
00315db8 strd sl, fp, [r0]
00315dbc mov r0, r7
00315dc0 bl #0x315978
00315dc4 str r6, [r0, #8]
00315dc8 ldr r1, [r4, #0x1c]
00315dcc adds r2, sl, r6
00315dd0 adc r3, fp, #0
00315dd4 mov r0, r1
00315dd8 ldr r1, [r1]
00315ddc mov lr, pc
00315de0 ldr pc, [r1, #0x20]
00315de4 ldr r3, [sp, #4]
00315de8 add r5, r5, #1
00315dec cmp r5, r3
00315df0 bne #0x315d00
00315df4 add sp, sp, #0x1c
00315df8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315dfc ldr r0, [r4, #0x1c]
00315e00 bl #0x313a90
00315e04 cmn r0, #1
00315e08 bne #0x315c94
00315e0c b #0x315b98
00315e10 mov r1, r8
00315e14 bl #0x315978
00315e18 strd sl, fp, [r0]
00315e1c str sb, [r0, #0x14]
00315e20 str sb, [r0, #0x10]
00315e24 str sb, [r0, #0xc]
00315e28 str r6, [r0, #8]
00315e2c b #0x315dc8
00315e30 ldr r3, [pc, #0x98]
00315e34 ldr r1, [r4, #0x18]
00315e38 mov r2, r5
00315e3c ldr r7, [r6, r3]
00315e40 ldr r3, [r7, #0x10]
00315e44 ldr r3, [r3, #0x34]
00315e48 mov r0, r3
00315e4c ldr r3, [r3]
00315e50 mov lr, pc
00315e54 ldr pc, [r3, #0x94]
00315e58 cmp r0, #0
00315e5c str r0, [sp, #0x14]
00315e60 beq #0x315b60
00315e64 mov r1, r5
00315e68 mov r0, #0x30
00315e6c bl #0x310570
00315e70 add r5, sp, #0x18
00315e74 mov r8, r0
00315e78 ldr r1, [r5, #-4]!
00315e7c bl #0x3172d8
00315e80 str r8, [r4, #0x1c]
00315e84 ldr r3, [r7, #0x10]
00315e88 mov r1, r5
00315e8c ldr r3, [r3, #0x34]
00315e90 mov r0, r3
00315e94 ldr r3, [r3]
00315e98 mov lr, pc
00315e9c ldr pc, [r3, #0x78]
00315ea0 b #0x315b60
00315ea4 ldr r3, [r4, #0x1c]
00315ea8 cmp r3, #0
00315eac beq #0x315df4
00315eb0 mov r0, r3
00315eb4 ldr r3, [r3]
00315eb8 mov lr, pc
00315ebc ldr pc, [r3, #4]
00315ec0 mov r3, #0
00315ec4 str r3, [r4, #0x1c]
00315ec8 b #0x315df4
00315ecc rsbeq lr, r7, r4, lsr #31
00315ed0 strdeq r3, r4, [r0], -r4
00315ed4 subseq r8, sl, r8, ror #18

_ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_ 0x315848 188
00315848 push {r4, r5, r6, r7, r8, sl, lr}
0031584c sub sp, sp, #0xc
00315850 add r4, sp, #8
00315854 str r1, [r4, #-4]!
00315858 add r5, r0, #0x20
0031585c mov r6, r0
00315860 mov r1, r4
00315864 mov r0, r5
00315868 mov sl, r3
0031586c mov r7, r2
00315870 ldr r8, [sp, #0x28]
00315874 bl #0x314120
00315878 cmp r0, r5
0031587c mov r3, r0
00315880 beq #0x3158d8
00315884 ldr r2, [r0, #0x30]
00315888 str sl, [r0, #0x38]
0031588c str r7, [r0, #0x34]
00315890 cmp r2, #0
00315894 str r8, [r0, #0x3c]
00315898 beq #0x3158d0
0031589c ldr r1, [r6, #0x1c]
003158a0 cmp r1, #0
003158a4 beq #0x3158d0
003158a8 mov r0, r1
003158ac ldrd r2, r3, [r3, #0x28]
003158b0 ldr r1, [r1]
003158b4 mov lr, pc
003158b8 ldr pc, [r1, #0x20]
003158bc cmp r7, #0
003158c0 beq #0x3158d0
003158c4 ldr r0, [r6, #0x1c]
003158c8 mov r1, r8
003158cc blx r7
003158d0 add sp, sp, #0xc
003158d4 pop {r4, r5, r6, r7, r8, sl, pc}
003158d8 mov r1, r4
003158dc bl #0x3156f0
003158e0 mov r3, #0
003158e4 mov r2, #0
003158e8 strd r2, r3, [r0]
003158ec mov r3, #0
003158f0 str r8, [r0, #0x14]
003158f4 str sl, [r0, #0x10]
003158f8 str r7, [r0, #0xc]
003158fc str r3, [r0, #8]
00315900 b #0x3158d0

_ZN14PlayerSavegame7SG_LoadEi 0x465430 32
00465430 push {r4, r5, r6, lr}
00465434 mov r5, r0
00465438 mov r4, r1
0046543c bl #0x464f4c
00465440 mov r0, r5
00465444 mov r1, r4
00465448 pop {r4, r5, r6, lr}
0046544c b #0x468574

_ZN14PlayerSavegame5_LoadEi 0x464f4c 1252
00464f4c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464f50 ldr r5, [pc, #0x40c]
00464f54 ldr r7, [pc, #0x40c]
00464f58 ldr r8, [r0, #8]
00464f5c add r5, pc, r5
00464f60 ldr r3, [r5, r7]
00464f64 sub sp, sp, #0x2c
00464f68 cmp r8, #0
00464f6c ldr r3, [r3]
00464f70 mov r4, r0
00464f74 mov r6, r1
00464f78 str r3, [sp, #0x24]
00464f7c beq #0x4652f0
00464f80 tst r6, #1
00464f84 beq #0x46508c
00464f88 ldr r0, [r4, #8]
00464f8c cmp r0, #0
00464f90 beq #0x46508c
00464f94 ldr r3, [pc, #0x3d0]
00464f98 ldr r1, [pc, #0x3d0]
00464f9c str r4, [sp]
00464fa0 ldr r2, [r5, r3]
00464fa4 ldr r3, [pc, #0x3c8]
00464fa8 add r1, pc, r1
00464fac ldr r3, [r5, r3]
00464fb0 bl #0x315848
00464fb4 ldr r3, [pc, #0x3bc]
00464fb8 ldr r1, [pc, #0x3bc]
00464fbc ldr r0, [r4, #8]
00464fc0 ldr r2, [r5, r3]
00464fc4 ldr r3, [pc, #0x3b4]
00464fc8 add r1, pc, r1
00464fcc str r4, [sp]
00464fd0 ldr r3, [r5, r3]
00464fd4 bl #0x315848
00464fd8 ldr r3, [pc, #0x3a4]
00464fdc ldr r1, [pc, #0x3a4]
00464fe0 ldr r0, [r4, #8]
00464fe4 ldr r2, [r5, r3]
00464fe8 ldr r3, [pc, #0x39c]
00464fec add r1, pc, r1
00464ff0 str r4, [sp]
00464ff4 ldr r3, [r5, r3]
00464ff8 bl #0x315848
00464ffc ldr r3, [pc, #0x38c]
00465000 ldr r1, [pc, #0x38c]
00465004 ldr r0, [r4, #8]
00465008 ldr r2, [r5, r3]
0046500c ldr r3, [pc, #0x384]
00465010 add r1, pc, r1
00465014 str r4, [sp]
00465018 ldr r3, [r5, r3]
0046501c bl #0x315848
00465020 ldr r3, [pc, #0x374]
00465024 ldr r1, [pc, #0x374]
00465028 ldr r0, [r4, #8]
0046502c ldr r2, [r5, r3]
00465030 ldr r3, [pc, #0x36c]
00465034 add r1, pc, r1
00465038 str r4, [sp]
0046503c ldr r3, [r5, r3]
00465040 bl #0x315848
00465044 ldr r3, [pc, #0x35c]
00465048 ldr r1, [pc, #0x35c]
0046504c ldr r0, [r4, #8]
00465050 ldr r2, [r5, r3]
00465054 ldr r3, [pc, #0x354]
00465058 add r1, pc, r1
0046505c str r4, [sp]
00465060 ldr r3, [r5, r3]
00465064 bl #0x315848
00465068 ldr r3, [pc, #0x344]
0046506c ldr r1, [pc, #0x344]
00465070 ldr r0, [r4, #8]
00465074 ldr r2, [r5, r3]
00465078 ldr r3, [pc, #0x33c]
0046507c add r1, pc, r1
00465080 str r4, [sp]
00465084 ldr r3, [r5, r3]
00465088 bl #0x315848
0046508c tst r6, #2
00465090 bne #0x4652c4
00465094 tst r6, #4
00465098 beq #0x4651f8
0046509c ldr r0, [r4, #8]
004650a0 cmp r0, #0
004650a4 beq #0x4651f8
004650a8 ldr r3, [pc, #0x310]
004650ac ldr r1, [pc, #0x310]
004650b0 str r4, [sp]
004650b4 ldr r2, [r5, r3]
004650b8 ldr r3, [pc, #0x308]
004650bc add r1, pc, r1
004650c0 ldr r3, [r5, r3]
004650c4 bl #0x315848
004650c8 ldr r3, [pc, #0x2fc]
004650cc ldr r1, [pc, #0x2fc]
004650d0 ldr r0, [r4, #8]
004650d4 ldr r2, [r5, r3]
004650d8 ldr r3, [pc, #0x2f4]
004650dc add r1, pc, r1
004650e0 str r4, [sp]
004650e4 ldr r3, [r5, r3]
004650e8 bl #0x315848
004650ec ldr r3, [pc, #0x2e4]
004650f0 ldr r1, [pc, #0x2e4]
004650f4 ldr r0, [r4, #8]
004650f8 ldr r2, [r5, r3]
004650fc ldr r3, [pc, #0x2dc]
00465100 add r1, pc, r1
00465104 str r4, [sp]
00465108 ldr r3, [r5, r3]
0046510c bl #0x315848
00465110 ldr r8, [r4, #8]
00465114 bl #0x7fd794
00465118 ldrb r3, [r0, #5]
0046511c cmp r3, #0
00465120 beq #0x465148
00465124 ldr r3, [pc, #0x2b8]
00465128 ldr r3, [r5, r3]
0046512c ldr r3, [r3, #0x40]
00465130 ldrb r3, [r3, #0x71b]
00465134 cmp r3, #0
00465138 bne #0x465148
0046513c ldr r3, [pc, #0x2a4]
00465140 ldr r2, [r5, r3]
00465144 b #0x46514c
00465148 mov r2, #0
0046514c ldr r3, [pc, #0x298]
00465150 ldr r1, [pc, #0x298]
00465154 mov r0, r8
00465158 ldr r3, [r5, r3]
0046515c add r1, pc, r1
00465160 str r4, [sp]
00465164 bl #0x315848
00465168 ldr r3, [pc, #0x284]
0046516c ldr r1, [pc, #0x284]
00465170 ldr r0, [r4, #8]
00465174 ldr r2, [r5, r3]
00465178 ldr r3, [pc, #0x27c]
0046517c add r1, pc, r1
00465180 str r4, [sp]
00465184 ldr r3, [r5, r3]
00465188 bl #0x315848
0046518c ldr r3, [pc, #0x26c]
00465190 ldr r1, [pc, #0x26c]
00465194 ldr r0, [r4, #8]
00465198 ldr r2, [r5, r3]
0046519c ldr r3, [pc, #0x264]
004651a0 add r1, pc, r1
004651a4 str r4, [sp]
004651a8 ldr r3, [r5, r3]
004651ac bl #0x315848
004651b0 ldr r3, [pc, #0x254]
004651b4 ldr r1, [pc, #0x254]
004651b8 ldr r0, [r4, #8]
004651bc ldr r2, [r5, r3]
004651c0 ldr r3, [pc, #0x24c]
004651c4 add r1, pc, r1
004651c8 str r4, [sp]
004651cc ldr r3, [r5, r3]
004651d0 bl #0x315848
004651d4 ldr r3, [pc, #0x23c]
004651d8 ldr r1, [pc, #0x23c]
004651dc ldr r0, [r4, #8]
004651e0 ldr r2, [r5, r3]
004651e4 ldr r3, [pc, #0x234]
004651e8 add r1, pc, r1
004651ec str r4, [sp]
004651f0 ldr r3, [r5, r3]
004651f4 bl #0x315848
004651f8 tst r6, #8
004651fc beq #0x46522c
00465200 ldr r0, [r4, #8]
00465204 cmp r0, #0
00465208 beq #0x46522c
0046520c ldr r3, [pc, #0x1b8]
00465210 ldr r1, [pc, #0x20c]
00465214 str r4, [sp]
00465218 ldr r2, [r5, r3]
0046521c ldr r3, [pc, #0x1b0]
00465220 add r1, pc, r1
00465224 ldr r3, [r5, r3]
00465228 bl #0x315848
0046522c tst r6, #0x20
00465230 beq #0x465260
00465234 ldr r0, [r4, #8]
00465238 cmp r0, #0
0046523c beq #0x465260
00465240 ldr r3, [pc, #0x1b8]
00465244 ldr r1, [pc, #0x1dc]
00465248 str r4, [sp]
0046524c ldr r2, [r5, r3]
00465250 ldr r3, [pc, #0x1b0]
00465254 add r1, pc, r1
00465258 ldr r3, [r5, r3]
0046525c bl #0x315848
00465260 tst r6, #0x10
00465264 beq #0x4652a8
00465268 ldr r3, [r4, #8]
0046526c cmp r3, #0
00465270 beq #0x4652a8
00465274 add r0, r4, #0xb8
00465278 bl #0x46c1a8
0046527c add r0, r4, #0x118
00465280 bl #0x46c1a8
00465284 ldr r3, [pc, #0x168]
00465288 ldr r1, [pc, #0x19c]
0046528c ldr r0, [r4, #8]
00465290 ldr r2, [r5, r3]
00465294 ldr r3, [pc, #0x160]
00465298 add r1, pc, r1
0046529c str r4, [sp]
004652a0 ldr r3, [r5, r3]
004652a4 bl #0x315848
004652a8 ldr r3, [r5, r7]
004652ac ldr r2, [sp, #0x24]
004652b0 ldr r3, [r3]
004652b4 cmp r2, r3
004652b8 bne #0x465360
004652bc add sp, sp, #0x2c
004652c0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004652c4 mov r0, r4
004652c8 bl #0x46954c
004652cc mov r0, r4
004652d0 bl #0x469764
004652d4 mov r0, r4
004652d8 bl #0x4694c8
004652dc add r0, r4, #0xb8
004652e0 bl #0x46c1a8
004652e4 add r0, r4, #0x118
004652e8 bl #0x46c1a8
004652ec b #0x465094
004652f0 ldr r3, [r0, #4]
004652f4 cmn r3, #1
004652f8 beq #0x464f80
004652fc add sl, sp, #0xc
00465300 mov r0, sl
00465304 mov r1, #0x10
00465308 str sl, [sp, #0x1c]
0046530c str sl, [sp, #0x20]
00465310 bl #0x31167c
00465314 ldr r2, [sp, #0x1c]
00465318 mov r3, r8
0046531c mov r1, sl
00465320 strb r8, [r2]
00465324 ldr r0, [r4, #4]
00465328 mov r2, r8
0046532c bl #0x463c84
00465330 mov r1, r8
00465334 mov r0, #0x3c
00465338 ldr fp, [sp, #0x20]
0046533c bl #0x310570
00465340 mov r1, fp
00465344 mov sb, r0
00465348 mov r2, r8
0046534c bl #0x315ed8
00465350 str sb, [r4, #8]
00465354 mov r0, sl
00465358 bl #0x3139ac
0046535c b #0x464f80
00465360 bl #0x30e310
00465364 subseq pc, r2, r4, lsr fp
00465368 andeq r4, r0, ip, lsr #1
0046536c andeq r1, r0, r8, lsl #10
00465370 subeq r8, r6, r0, asr #4
00465374 strdeq r1, r2, [r0], -r4
00465378 andeq r1, r0, r8, ror #30
0046537c subeq r8, r6, r8, lsr #4
00465380 strdeq r1, r2, [r0], -r0
00465384 strheq r2, [r0], -ip
00465388 subeq r8, r6, ip, lsl #4
0046538c andeq r2, r0, r8, asr #29
00465390 andeq r0, r0, r8, lsr #26
00465394 strdeq r8, sb, [r6], #-0x10
00465398 strheq r1, [r0], -r8
0046539c andeq r3, r0, r4, lsr #12
004653a0 ldrdeq r8, sb, [r6], #-0x14
004653a4 andeq r3, r0, r8, ror r7
004653a8 andeq r3, r0, ip, asr #26
004653ac strheq r8, [r6], #-0x18
004653b0 strheq r2, [r0], -r0
004653b4 andeq r4, r0, r0, lsr r0
004653b8 umaaleq r8, r6, ip, r1
004653bc andeq r2, r0, ip, ror #12
004653c0 andeq r3, r0, r0, lsl #30
004653c4 subeq r8, r6, r4, ror #2
004653c8 andeq r4, r0, ip, ror r6
004653cc muleq r0, r4, r8
004653d0 subeq r8, r6, ip, asr #2
004653d4 andeq r1, r0, r4, lsr fp
004653d8 andeq r1, r0, ip, lsr r3
004653dc subeq r8, r6, r0, lsr r1
004653e0 andeq r0, r0, r4, lsr #14
004653e4 strdeq r3, r4, [r0], -r4
004653e8 andeq r2, r0, ip, asr #21
004653ec strdeq r2, r3, [r0], -r4
004653f0 ldrdeq r8, sb, [r6], #-0xc
004653f4 andeq r2, r0, r8, asr #12
004653f8 subeq r8, r6, r4, asr #1
004653fc andeq r3, r0, r8, lsl r4
00465400 andeq r0, r0, r4, asr #14
00465404 subeq r8, r6, r8, lsr #1
00465408 andeq r4, r0, ip, ror #23
0046540c andeq r2, r0, ip, lsr r4
00465410 subeq r8, r6, ip, lsl #1
00465414 andeq r4, r0, r8, lsl #5
00465418 andeq r0, r0, ip, lsl #29
0046541c subeq r8, r6, r0, ror r0
00465420 andeq r1, r0, r0, ror #24
00465424 subeq r8, r6, r8
00465428 strdeq r7, r8, [r6], #-0xf4
0046542c subeq r7, r6, r8, lsr #31

_ZN14PlayerSavegame22_LoadVolatileQuestsLogEi 0x468574 188
00468574 push {r4, r5, r6, r7, r8, lr}
00468578 ldr r4, [pc, #0xa4]
0046857c tst r1, #0x14
00468580 mov r5, r0
00468584 add r4, pc, r4
00468588 bne #0x468590
0046858c pop {r4, r5, r6, r7, r8, pc}
00468590 bl #0x7fd794
00468594 ldrb r3, [r0, #5]
00468598 cmp r3, #0
0046859c beq #0x46858c
004685a0 ldr r3, [pc, #0x80]
004685a4 ldr r6, [r4, r3]
004685a8 ldr r0, [r6, #0x40]
004685ac bl #0x36f074
004685b0 cmp r0, #0
004685b4 ldreq r7, [r6, #0x40]
004685b8 bne #0x468610
004685bc add r6, r7, #0x6e0
004685c0 ldr r3, [r7, #0x6e0]
004685c4 mov r0, r6
004685c8 mov lr, pc
004685cc ldr pc, [r3, #8]
004685d0 orrs r1, r0, r1
004685d4 beq #0x46858c
004685d8 ldr r1, [r7, #0x6e0]
004685dc mov r0, r6
004685e0 mov r2, #0
004685e4 mov r3, #0
004685e8 mov lr, pc
004685ec ldr pc, [r1, #0x20]
004685f0 ldr r3, [pc, #0x34]
004685f4 add r0, r5, #0x118
004685f8 mov r2, r6
004685fc ldr r1, [r4, r3]
00468600 mov r3, #0
00468604 ldr r1, [r1]
00468608 pop {r4, r5, r6, r7, r8, lr}
0046860c b #0x46c48c
00468610 ldr r7, [r6, #0x40]
00468614 ldrb r3, [r7, #0x719]
00468618 cmp r3, #0
0046861c beq #0x46858c
00468620 b #0x4685bc
00468624 subseq ip, r2, ip, lsl #10
00468628 strdeq r3, r4, [r0], -r4
0046862c muleq r0, ip, sl

_ZN14PlayerSavegame16__LoadPlayerNameEP11IStreamBasePv 0x4698dc 8
004698dc add r1, r1, #0x18
004698e0 b #0x461da8

_ZN14PlayerSavegame17__LoadPlayerLevelEP11IStreamBasePv 0x4689a0 8
004689a0 add r1, r1, #0x30
004689a4 b #0x38b758

_ZN14PlayerSavegame17__LoadPlayerClassEP11IStreamBasePv 0x469d88 228
00469d88 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469d8c ldr r8, [pc, #0xc8]
00469d90 ldr sb, [pc, #0xc8]
00469d94 sub sp, sp, #0x24
00469d98 add r8, pc, r8
00469d9c ldr r3, [r8, sb]
00469da0 add sl, sp, #4
00469da4 mov r5, r0
00469da8 ldr r3, [r3]
00469dac mov r0, sl
00469db0 mov fp, r1
00469db4 mov r1, #0x10
00469db8 str r3, [sp, #0x1c]
00469dbc str sl, [sp, #0x14]
00469dc0 str sl, [sp, #0x18]
00469dc4 bl #0x31167c
00469dc8 ldr r3, [sp, #0x14]
00469dcc mov r4, #0
00469dd0 mov r0, r5
00469dd4 strb r4, [r3]
00469dd8 mov r1, sl
00469ddc bl #0x461da8
00469de0 ldr r3, [pc, #0x7c]
00469de4 ldr r6, [sp, #0x18]
00469de8 ldr r3, [r8, r3]
00469dec ldr r5, [r3]
00469df0 cmp r5, r4
00469df4 beq #0x469e50
00469df8 ldr r3, [pc, #0x68]
00469dfc ldr r3, [r8, r3]
00469e00 ldr r7, [r3]
00469e04 b #0x469e14
00469e08 add r4, r4, #1
00469e0c cmp r4, r5
00469e10 beq #0x469e50
00469e14 mov r0, r6
00469e18 ldr r1, [r7, r4, lsl #2]
00469e1c bl #0x30e31c
00469e20 cmp r0, #0
00469e24 bne #0x469e08
00469e28 str r4, [fp, #0x34]
00469e2c mov r0, sl
00469e30 bl #0x3139ac
00469e34 ldr r3, [r8, sb]
00469e38 ldr r2, [sp, #0x1c]
00469e3c ldr r3, [r3]
00469e40 cmp r2, r3
00469e44 bne #0x469e58
00469e48 add sp, sp, #0x24
00469e4c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469e50 mvn r4, #0
00469e54 b #0x469e28
00469e58 bl #0x30e310
00469e5c ldrsheq sl, [r2], #-0xc8
00469e60 andeq r4, r0, ip, lsr #1
00469e64 andeq r4, r0, r4, lsl #4
00469e68 andeq r3, r0, r8, lsl #24

_ZN14PlayerSavegame21__LoadDifficultyLevelEP11IStreamBasePv 0x468968 56
00468968 ldr r3, [pc, #0x28]
0046896c ldr r2, [pc, #0x28]
00468970 push {r4, r5, r6, lr}
00468974 add r3, pc, r3
00468978 mov r4, r1
0046897c ldr r1, [r3, r2]
00468980 mov r5, r0
00468984 bl #0x38b758
00468988 mov r0, r5
0046898c add r1, r4, #0x3c
00468990 pop {r4, r5, r6, lr}
00468994 b #0x38b758
00468998 subseq ip, r2, ip, lsl r1
0046899c muleq r0, ip, sl

_ZN14PlayerSavegame15__LoadLevelNameEP11IStreamBasePv 0x468b78 120
00468b78 push {r4, r5, r6, r7, r8, lr}
00468b7c mov r7, r1
00468b80 sub sp, sp, #8
00468b84 add r1, r1, #0x38
00468b88 mov r6, r0
00468b8c bl #0x313b48
00468b90 mov r5, r7
00468b94 mov r4, #0
00468b98 add r8, sp, #4
00468b9c add r1, r4, #0x14
00468ba0 add r1, r7, r1, lsl #2
00468ba4 mov r0, r6
00468ba8 bl #0x38b758
00468bac add r1, r7, r4, lsl #2
00468bb0 add r1, r1, #0x5c
00468bb4 mov r0, r6
00468bb8 bl #0x38b758
00468bbc mov r0, r6
00468bc0 mov r1, r8
00468bc4 bl #0x38b758
00468bc8 ldr r3, [sp, #4]
00468bcc add r4, r4, #1
00468bd0 cmp r4, #3
00468bd4 str r3, [r5, #0xfc]
00468bd8 ldr r3, [sp, #4]
00468bdc str r3, [r5, #0x15c]
00468be0 add r5, r5, #4
00468be4 bne #0x468b9c
00468be8 add sp, sp, #8
00468bec pop {r4, r5, r6, r7, r8, pc}

_ZN14PlayerSavegame21__LoadLevelEntryPointEP11IStreamBasePv 0x468938 48
00468938 push {r4, r5, r6, lr}
0046893c mov r4, r1
00468940 mov r5, r0
00468944 add r1, r1, #0x40
00468948 bl #0x38b758
0046894c mov r0, r5
00468950 add r1, r4, #0x44
00468954 bl #0x38b758
00468958 mov r0, r5
0046895c add r1, r4, #0x48
00468960 pop {r4, r5, r6, lr}
00468964 b #0x38b758

_ZN14PlayerSavegame19__LoadUseSpawnPointEP11IStreamBasePv 0x468af0 48
00468af0 push {r4, r5, r6, lr}
00468af4 mov r4, r1
00468af8 mov r5, r0
00468afc add r1, r1, #0x4c
00468b00 bl #0x33e040
00468b04 mov r0, r5
00468b08 add r1, r4, #0x4d
00468b0c bl #0x33e040
00468b10 mov r0, r5
00468b14 add r1, r4, #0x4e
00468b18 pop {r4, r5, r6, lr}
00468b1c b #0x33e040

_ZN11IStreamBase6readAsERSs 0x461da8 232
00461da8 push {r4, r5, lr}
00461dac sub sp, sp, #0x14
00461db0 mov r4, r1
00461db4 add r1, sp, #0xc
00461db8 mov r5, r0
00461dbc bl #0x38b758
00461dc0 ldr r1, [sp, #0xc]
00461dc4 ldr r3, [pc, #0xac]
00461dc8 cmp r1, #0
00461dcc add r3, pc, r3
00461dd0 ble #0x461e04
00461dd4 sub r1, r1, #1
00461dd8 mov r0, r4
00461ddc bl #0x461ca8
00461de0 ldr r2, [sp, #0xc]
00461de4 mov r0, r5
00461de8 ldr r1, [r4, #0x14]
00461dec asr r3, r2, #0x1f
00461df0 ldr ip, [r5]
00461df4 mov lr, pc
00461df8 ldr pc, [ip, #0x18]
00461dfc add sp, sp, #0x14
00461e00 pop {r4, r5, pc}
00461e04 ldr r2, [pc, #0x70]
00461e08 ldr r2, [r3, r2]
00461e0c ldr r2, [r2]
00461e10 cmp r2, #2
00461e14 moveq r3, #0
00461e18 streq r3, [r3]
00461e1c beq #0x461e28
00461e20 cmp r2, #1
00461e24 beq #0x461e38
00461e28 mov r0, r4
00461e2c mov r1, #0
00461e30 bl #0x461ca8
00461e34 b #0x461dfc
00461e38 ldr r0, [pc, #0x40]
00461e3c ldr r1, [pc, #0x40]
00461e40 ldr r2, [pc, #0x40]
00461e44 ldr r0, [r3, r0]
00461e48 ldr r3, [pc, #0x3c]
00461e4c add r1, pc, r1
00461e50 mov ip, #0x57
00461e54 add r0, r0, #0xa8
00461e58 add r2, pc, r2
00461e5c add r3, pc, r3
00461e60 str ip, [sp]
00461e64 bl #0x30e004
00461e68 ldr r1, [sp, #0xc]
00461e6c cmp r1, #0
00461e70 ble #0x461e28
00461e74 b #0x461dd4
00461e78 subseq r2, r3, r4, asr #25
00461e7c andeq r3, r0, r0, asr #19
00461e80 andeq r1, r0, r0, asr #19
00461e84 subeq ip, r5, ip, lsl #11
00461e88 subeq fp, r6, r8, ror #5
00461e8c strheq ip, [r5], #-0x6c

_ZN14PlayerSavegameC1Ev 0x465ae0 352
00465ae0 ldr r3, [pc, #0x150]
00465ae4 ldr r1, [pc, #0x150]
00465ae8 push {r4, r5, r6, r7, r8, lr}
00465aec add r3, pc, r3
00465af0 ldr r1, [r3, r1]
00465af4 mov r4, r0
00465af8 mov r5, #0
00465afc add r2, r0, #0x18
00465b00 add r1, r1, #8
00465b04 mvn r6, #0
00465b08 str r1, [r0]
00465b0c sub sp, sp, #0x18
00465b10 mov r0, r2
00465b14 str r2, [r4, #0x28]
00465b18 str r2, [r4, #0x2c]
00465b1c mov r1, #0x10
00465b20 str r6, [r4, #4]
00465b24 str r5, [r4, #8]
00465b28 strb r5, [r4, #0xc]
00465b2c str r5, [r4, #0x10]
00465b30 strb r5, [r4, #0x14]
00465b34 bl #0x31167c
00465b38 ldr r3, [r4, #0x28]
00465b3c add r0, r4, #0xb8
00465b40 strb r5, [r3]
00465b44 str r6, [r4, #0x34]
00465b48 str r5, [r4, #0x30]
00465b4c str r5, [r4, #0x3c]
00465b50 str r5, [r4, #0x80]
00465b54 str r5, [r4, #0x84]
00465b58 str r5, [r4, #0x88]
00465b5c str r5, [r4, #0x8c]
00465b60 str r5, [r4, #0x90]
00465b64 bl #0x46b0a4
00465b68 add r0, r4, #0x118
00465b6c bl #0x46b0a4
00465b70 mov r3, r5
00465b74 str r5, [r4, #0x178]
00465b78 add r1, r4, #0x17c
00465b7c mov r2, r5
00465b80 str r2, [r1, r3]
00465b84 add r0, r1, r3
00465b88 add r3, r3, #8
00465b8c cmp r3, #0x18
00465b90 str r2, [r0, #4]
00465b94 bne #0x465b80
00465b98 mov r5, r2
00465b9c strb r2, [r4, #0x194]
00465ba0 add r8, r4, #0x88
00465ba4 mov r6, sp
00465ba8 mov r7, r2
00465bac mov r0, r8
00465bb0 mov r1, sp
00465bb4 str r7, [sp, #4]
00465bb8 strb r7, [sp]
00465bbc str r6, [sp, #8]
00465bc0 str r6, [sp, #0xc]
00465bc4 str r7, [sp, #0x10]
00465bc8 bl #0x465a48
00465bcc ldr r3, [sp, #0x10]
00465bd0 add r5, r5, #1
00465bd4 cmp r3, #0
00465bd8 beq #0x465be8
00465bdc mov r0, sp
00465be0 ldr r1, [sp, #4]
00465be4 bl #0x345c94
00465be8 cmp r5, #2
00465bec bne #0x465bac
00465bf0 mov r1, #0
00465bf4 mov r3, r4
00465bf8 mov r2, r1
00465bfc add r1, r1, #1
00465c00 cmp r1, #3
00465c04 str r2, [r3, #0x94]
00465c08 str r2, [r3, #0xa0]
00465c0c str r2, [r3, #0xac]
00465c10 str r2, [r3, #0x40]
00465c14 str r2, [r3, #0x68]
00465c18 str r2, [r3, #0x74]
00465c1c str r2, [r3, #0x5c]
00465c20 str r2, [r3, #0x50]
00465c24 add r3, r3, #4
00465c28 bne #0x465bfc
00465c2c mov r0, r4
00465c30 add sp, sp, #0x18
00465c34 pop {r4, r5, r6, r7, r8, pc}
00465c38 subseq lr, r2, r4, lsr #31
00465c3c strheq r4, [r0], -ip

_ZN11IStreamBase6readAsIbEEvRT_ 0x33e040 176
0033e040 str lr, [sp, #-4]!
0033e044 mov r3, #0
0033e048 sub sp, sp, #0xc
0033e04c ldr ip, [r0]
0033e050 mov r2, #1
0033e054 mov lr, pc
0033e058 ldr pc, [ip, #0x18]
0033e05c ldr r3, [pc, #0x74]
0033e060 cmp r0, #1
0033e064 add r3, pc, r3
0033e068 beq #0x33e098
0033e06c ldr r2, [pc, #0x68]
0033e070 ldr r2, [r3, r2]
0033e074 ldr r2, [r2]
0033e078 cmp r2, #2
0033e07c moveq r3, #0
0033e080 streq r3, [r3]
0033e084 beq #0x33e090
0033e088 cmp r2, #1
0033e08c beq #0x33e0a4
0033e090 add sp, sp, #0xc
0033e094 ldm sp!, {pc}
0033e098 cmp r1, #0
0033e09c beq #0x33e090
0033e0a0 b #0x33e06c
0033e0a4 ldr r0, [pc, #0x34]
0033e0a8 ldr r1, [pc, #0x34]
0033e0ac ldr r2, [pc, #0x34]
0033e0b0 ldr r0, [r3, r0]
0033e0b4 ldr r3, [pc, #0x30]
0033e0b8 mov ip, #0x45
0033e0bc add r1, pc, r1
0033e0c0 add r2, pc, r2
0033e0c4 add r3, pc, r3
0033e0c8 add r0, r0, #0xa8
0033e0cc str ip, [sp]
0033e0d0 bl #0x30e004
0033e0d4 b #0x33e090
0033e0d8 rsbeq r6, r5, ip, lsr #20
0033e0dc andeq r3, r0, r0, asr #19
0033e0e0 andeq r1, r0, r0, asr #19
0033e0e4 subseq r0, r8, ip, lsl r3
0033e0e8 subseq r0, r8, r0, asr #8
0033e0ec subseq r0, r8, r4, asr r4

_ZN11IStreamBase6readAsIjEEvRT_ 0x313b48 176
00313b48 str lr, [sp, #-4]!
00313b4c mov r3, #0
00313b50 sub sp, sp, #0xc
00313b54 ldr ip, [r0]
00313b58 mov r2, #4
00313b5c mov lr, pc
00313b60 ldr pc, [ip, #0x18]
00313b64 ldr r3, [pc, #0x74]
00313b68 cmp r0, #4
00313b6c add r3, pc, r3
00313b70 beq #0x313ba0
00313b74 ldr r2, [pc, #0x68]
00313b78 ldr r2, [r3, r2]
00313b7c ldr r2, [r2]
00313b80 cmp r2, #2
00313b84 moveq r3, #0
00313b88 streq r3, [r3]
00313b8c beq #0x313b98
00313b90 cmp r2, #1
00313b94 beq #0x313bac
00313b98 add sp, sp, #0xc
00313b9c ldm sp!, {pc}
00313ba0 cmp r1, #0
00313ba4 beq #0x313b98
00313ba8 b #0x313b74
00313bac ldr r0, [pc, #0x34]
00313bb0 ldr r1, [pc, #0x34]
00313bb4 ldr r2, [pc, #0x34]
00313bb8 ldr r0, [r3, r0]
00313bbc ldr r3, [pc, #0x30]
00313bc0 mov ip, #0x45
00313bc4 add r1, pc, r1
00313bc8 add r2, pc, r2
00313bcc add r3, pc, r3
00313bd0 add r0, r0, #0xa8
00313bd4 str ip, [sp]
00313bd8 bl #0x30e004
00313bdc b #0x313b98
00313be0 rsbeq r0, r8, r4, lsr #30
00313be4 andeq r3, r0, r0, asr #19
00313be8 andeq r1, r0, r0, asr #19
00313bec subseq sl, sl, r4, lsl r8
00313bf0 subseq sl, sl, r8, lsr sb
00313bf4 subseq sl, sl, ip, asr #18

_ZN11IStreamBase6readAsIiEEvRT_ 0x38b758 176
0038b758 str lr, [sp, #-4]!
0038b75c mov r3, #0
0038b760 sub sp, sp, #0xc
0038b764 ldr ip, [r0]
0038b768 mov r2, #4
0038b76c mov lr, pc
0038b770 ldr pc, [ip, #0x18]
0038b774 ldr r3, [pc, #0x74]
0038b778 cmp r0, #4
0038b77c add r3, pc, r3
0038b780 beq #0x38b7b0
0038b784 ldr r2, [pc, #0x68]
0038b788 ldr r2, [r3, r2]
0038b78c ldr r2, [r2]
0038b790 cmp r2, #2
0038b794 moveq r3, #0
0038b798 streq r3, [r3]
0038b79c beq #0x38b7a8
0038b7a0 cmp r2, #1
0038b7a4 beq #0x38b7bc
0038b7a8 add sp, sp, #0xc
0038b7ac ldm sp!, {pc}
0038b7b0 cmp r1, #0
0038b7b4 beq #0x38b7a8
0038b7b8 b #0x38b784
0038b7bc ldr r0, [pc, #0x34]
0038b7c0 ldr r1, [pc, #0x34]
0038b7c4 ldr r2, [pc, #0x34]
0038b7c8 ldr r0, [r3, r0]
0038b7cc ldr r3, [pc, #0x30]
0038b7d0 mov ip, #0x45
0038b7d4 add r1, pc, r1
0038b7d8 add r2, pc, r2
0038b7dc add r3, pc, r3
0038b7e0 add r0, r0, #0xa8
0038b7e4 str ip, [sp]
0038b7e8 bl #0x30e004
0038b7ec b #0x38b7a8
0038b7f0 rsbeq sb, r0, r4, lsl r3
0038b7f4 andeq r3, r0, r0, asr #19
0038b7f8 andeq r1, r0, r0, asr #19
0038b7fc subseq r2, r3, r4, lsl #24
0038b800 subseq r2, r3, r8, lsr #26
0038b804 subseq r2, r3, ip, lsr sp
