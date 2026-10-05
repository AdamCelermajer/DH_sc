# _ZN7gameswf13texture_cache27get_region_size_requirementERiS1_ 793560
793560 str r4, [sp, #-4]!
793564 ldr r3, [r0]
793568 asr r2, r3, #0x1f
79356c add ip, r3, #0xf
793570 lsr r2, r2, #0x1c
793574 add r4, r3, r2
793578 cmp r3, #0
79357c and r4, r4, #0xf
793580 movlt r3, ip
793584 rsb r2, r2, r4
793588 cmp r2, #0
79358c asr r3, r3, #4
793590 addgt r3, r3, #1
793594 lsl r3, r3, #4
793598 cmp r3, #0x10
79359c movlt r3, #0x10
7935a0 str r3, [r0]
7935a4 ldr r3, [r1]
7935a8 asr r2, r3, #0x1f
7935ac cmp r3, #0
7935b0 lsr r2, r2, #0x1c
7935b4 add ip, r3, r2
7935b8 add r0, r3, #0xf
7935bc and ip, ip, #0xf
7935c0 movlt r3, r0
7935c4 rsb r2, r2, ip
7935c8 cmp r2, #0
7935cc asr r3, r3, #4
7935d0 addgt r3, r3, #1
7935d4 lsl r3, r3, #4
7935d8 cmp r3, #0x10
7935dc movlt r3, #0x10
7935e0 str r3, [r1]
7935e4 ldm sp!, {r4}
7935e8 bx lr
# _ZN7gameswf14glyph_provider15get_face_entityERKNS_9tu_stringEbb 7d113c
7d113c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
7d1140 ldr r4, [pc, #0x4b0]
7d1144 ldr ip, [pc, #0x4b0]
7d1148 sub sp, sp, #0x6c
7d114c add r4, pc, r4
7d1150 str r0, [sp, #8]
7d1154 ldr r0, [r4, ip]
7d1158 str ip, [sp, #0x10]
7d115c ldr ip, [sp, #8]
7d1160 mov r7, r2
7d1164 ldr r2, [r0]
7d1168 add r6, ip, #0xc
7d116c mov r0, r6
7d1170 mov sl, r3
7d1174 str r2, [sp, #0x64]
7d1178 mov r8, r1
7d117c bl #0x752f50 ; _ZN7gameswf9tu_stringaSERKS0_
7d1180 cmp r7, #0
7d1184 bne #0x7d1464
7d1188 cmp sl, #0
7d118c bne #0x7d1450
7d1190 ldr r2, [sp, #8]
7d1194 add r5, sp, #0x68
7d1198 mov r3, #0
7d119c add r2, r2, #0x24
7d11a0 str r3, [r5, #-0x1c]!
7d11a4 str r2, [sp, #0x14]
7d11a8 mov r0, r2
7d11ac mov r1, r6
7d11b0 mov r2, r5
7d11b4 bl #0x7d0cac ; _ZNK7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE3getERKS1_PS4_
7d11b8 cmp r0, #0
7d11bc beq #0x7d11f8
7d11c0 ldr r8, [sp, #0x4c]
7d11c4 mov r0, r8
7d11c8 cmp r0, #0
7d11cc beq #0x7d11d4
7d11d0 bl #0x75a240 ; _ZN7gameswf11ref_counted8drop_refEv
7d11d4 ldr ip, [sp, #0x10]
7d11d8 ldr r2, [sp, #0x64]
7d11dc mov r0, r8
7d11e0 ldr r3, [r4, ip]
7d11e4 ldr r3, [r3]
7d11e8 cmp r2, r3
7d11ec bne #0x7d15f4
7d11f0 add sp, sp, #0x6c
7d11f4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
7d11f8 ldr r3, [sp, #0x60]
7d11fc ldrsb r1, [r8]
7d1200 mvn r2, #0
7d1204 bfi r3, r2, #0, #0x18
7d1208 lsr r2, r3, #0x18
7d120c bfi r2, r0, #0, #1
7d1210 cmn r1, #1
7d1214 mov r1, #1
7d1218 str r3, [sp, #0x60]
7d121c strb r1, [sp, #0x50]
7d1220 strb r2, [sp, #0x63]
7d1224 strb r0, [sp, #0x51]
7d1228 add r3, sp, #0x50
7d122c ldreq r0, [r8, #0xc]
7d1230 str r3, [sp, #0xc]
7d1234 addne r0, r8, r1
7d1238 mov r2, r7
7d123c mov r3, sl
7d1240 ldr r1, [sp, #0xc]
7d1244 bl #0x7d0fe4 ; _ZN7gameswf12get_fontfileEPKcRNS_9tu_stringEbb
7d1248 cmp r0, #0
7d124c beq #0x7d13b0
7d1250 ldr ip, [sp, #8]
7d1254 ldr sl, [ip, #0x24]
7d1258 cmp sl, #0
7d125c beq #0x7d1284
7d1260 ldr r3, [sl, #4]
7d1264 cmp r3, #0
7d1268 movlt r7, #0
7d126c bge #0x7d1418
7d1270 ldr r2, [sp, #0x14]
7d1274 cmp r2, #0
7d1278 beq #0x7d1284
7d127c cmp sl, #0
7d1280 bne #0x7d1524
7d1284 ldr ip, [sp, #8]
7d1288 mov r2, #0
7d128c ldrb r3, [ip, #8]
7d1290 str r2, [sp, #0x40]
7d1294 cmp r3, r2
7d1298 beq #0x7d1478
7d129c ldrsb r3, [sp, #0x50]
7d12a0 add r8, sp, #0x18
7d12a4 mov r0, r8
7d12a8 cmn r3, #1
7d12ac ldrne r2, [sp, #0xc]
7d12b0 ldreq r1, [sp, #0x5c]
7d12b4 addne r1, r2, #1
7d12b8 ldr r2, [pc, #0x340]
7d12bc add r2, pc, r2
7d12c0 bl #0x7b68c8 ; _ZN7gameswf7tu_fileC1EPKcS2_
7d12c4 ldr r0, [sp, #0x18]
7d12c8 cmp r0, #0
7d12cc beq #0x7d14f4
7d12d0 mov lr, pc
7d12d4 ldr pc, [sp, #0x2c]
7d12d8 ldr r0, [sp, #0x18]
7d12dc mov lr, pc
7d12e0 ldr pc, [sp, #0x30]
7d12e4 ldr r1, [sp, #0x18]
7d12e8 mov sl, r0
7d12ec mov r0, #0
7d12f0 mov lr, pc
7d12f4 ldr pc, [sp, #0x28]
7d12f8 mov r1, #0
7d12fc mov r0, #0x10
7d1300 bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7d1304 mov r7, r0
7d1308 bl #0x7b628c ; _ZN7gameswf6membufC1Ev
7d130c mov r1, sl
7d1310 mov r0, r7
7d1314 bl #0x75ae5c ; _ZN7gameswf6membuf6resizeEi
7d1318 mov r0, r8
7d131c mov r1, r7
7d1320 mvn r2, #0
7d1324 bl #0x7b6a80 ; _ZN7gameswf7tu_file10read_fullyEPNS_6membufEi
7d1328 ldr r3, [sp, #8]
7d132c ldr r1, [r7, #8]
7d1330 mov r2, sl
7d1334 ldr r0, [r3]
7d1338 add ip, sp, #0x40
7d133c mov r3, #0
7d1340 str ip, [sp]
7d1344 bl #0x70c890 ; FT_New_Memory_Face
7d1348 ldr sl, [sp, #0x40]
7d134c cmp sl, #0
7d1350 beq #0x7d14e0
7d1354 mov r1, #0
7d1358 mov r0, #0x2c
7d135c bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7d1360 ldr r3, [sp, #0xc]
7d1364 mov r2, r7
7d1368 mov sl, r0
7d136c ldr r1, [sp, #0x40]
7d1370 bl #0x7d0cf8 ; _ZN7gameswf11face_entityC1EP11FT_FaceRec_PNS_6membufERNS_9tu_stringE
7d1374 mov r0, r5
7d1378 mov r1, sl
7d137c bl #0x7d07dc ; _ZN7gameswf9smart_ptrINS_11face_entityEE7set_refEPS1_
7d1380 ldr r0, [sp, #0x14]
7d1384 mov r1, r6
7d1388 mov r2, r5
7d138c bl #0x7d089c ; _ZN7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE3addERKS1_RKS4_
7d1390 mov r0, r8
7d1394 bl #0x7b69e8 ; _ZN7gameswf7tu_fileD1Ev
7d1398 ldr r8, [sp, #0x4c]
7d139c ldrsb r3, [sp, #0x50]
7d13a0 cmn r3, #1
7d13a4 beq #0x7d1404
7d13a8 ldr r0, [sp, #0x4c]
7d13ac b #0x7d11c8 ; 
7d13b0 ldrsb r3, [r8]
7d13b4 ldr r0, [pc, #0x248]
7d13b8 cmn r3, #1
7d13bc addne r1, r8, #1
7d13c0 ldreq r1, [r8, #0xc]
7d13c4 add r0, pc, r0
7d13c8 bl #0x761184 ; _ZN7gameswf9log_errorEPKcz
7d13cc mov r8, #0
7d13d0 add r2, sp, #0x68
7d13d4 str r8, [r2, #-0x20]!
7d13d8 ldr r0, [sp, #0x14]
7d13dc mov r1, r6
7d13e0 bl #0x7d089c ; _ZN7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE3addERKS1_RKS4_
7d13e4 ldr r0, [sp, #0x48]
7d13e8 cmp r0, r8
7d13ec moveq r8, r0
7d13f0 beq #0x7d13f8
7d13f4 bl #0x75a240 ; _ZN7gameswf11ref_counted8drop_refEv
7d13f8 ldrsb r3, [sp, #0x50]
7d13fc cmn r3, #1
7d1400 bne #0x7d13a8
7d1404 ldr r0, [sp, #0x5c]
7d1408 ldr r1, [sp, #0x58]
7d140c bl #0x752b38 ; _ZN7gameswf13free_internalEPvj
7d1410 ldr r0, [sp, #0x4c]
7d1414 b #0x7d11c8 ; 
7d1418 mov r2, #8
7d141c mov r7, #0
7d1420 ldr r1, [sl, r2]
7d1424 add r0, sl, r2
7d1428 cmn r1, #2
7d142c beq #0x7d143c
7d1430 ldr r1, [r0, #4]
7d1434 cmn r1, #1
7d1438 bne #0x7d1270
7d143c add r7, r7, #1
7d1440 cmp r7, r3
7d1444 add r2, r2, #0x20
7d1448 ble #0x7d1420
7d144c b #0x7d1270 ; 
7d1450 ldr r1, [pc, #0x1b0]
7d1454 mov r0, r6
7d1458 add r1, pc, r1
7d145c bl #0x7521cc ; _ZN7gameswf9tu_stringpLEPKc
7d1460 b #0x7d1190 ; 
7d1464 ldr r1, [pc, #0x1a0]
7d1468 mov r0, r6
7d146c add r1, pc, r1
7d1470 bl #0x7521cc ; _ZN7gameswf9tu_stringpLEPKc
7d1474 b #0x7d1188 ; 
7d1478 ldrsb r3, [sp, #0x50]
7d147c ldr r2, [sp, #8]
7d1480 cmn r3, #1
7d1484 ldrne r3, [sp, #0xc]
7d1488 ldreq r1, [sp, #0x5c]
7d148c ldr r0, [r2]
7d1490 addne r1, r3, #1
7d1494 mov r2, #0
7d1498 add r3, sp, #0x40
7d149c bl #0x70c8cc ; FT_New_Face
7d14a0 mov r1, #0
7d14a4 mov r0, #0x2c
7d14a8 bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7d14ac ldr r2, [sp, #0xc]
7d14b0 mov r7, r0
7d14b4 ldr r1, [sp, #0x40]
7d14b8 bl #0x7d0d7c ; _ZN7gameswf11face_entityC1EP11FT_FaceRec_RNS_9tu_stringE
7d14bc mov r0, r5
7d14c0 mov r1, r7
7d14c4 bl #0x7d07dc ; _ZN7gameswf9smart_ptrINS_11face_entityEE7set_refEPS1_
7d14c8 ldr r0, [sp, #0x14]
7d14cc mov r1, r6
7d14d0 mov r2, r5
7d14d4 bl #0x7d089c ; _ZN7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE3addERKS1_RKS4_
7d14d8 ldr r8, [sp, #0x4c]
7d14dc b #0x7d139c ; 
7d14e0 mov r0, r7
7d14e4 bl #0x7b6548 ; _ZN7gameswf6membufD1Ev
7d14e8 mov r0, r7
7d14ec mov r1, sl
7d14f0 bl #0x752b38 ; _ZN7gameswf13free_internalEPvj
7d14f4 mov r0, r8
7d14f8 bl #0x7b69e8 ; _ZN7gameswf7tu_fileD1Ev
7d14fc ldrsb r3, [sp, #0x50]
7d1500 cmn r3, #1
7d1504 ldreq r1, [sp, #0x5c]
7d1508 ldrne ip, [sp, #0xc]
7d150c addne r1, ip, #1
7d1510 ldr r0, [pc, #0xf8]
7d1514 add r0, pc, r0
7d1518 bl #0x761184 ; _ZN7gameswf9log_errorEPKcz
7d151c ldr r8, [sp, #0x4c]
7d1520 b #0x7d139c ; 
7d1524 ldr r3, [sp, #0xc]
7d1528 ldr sb, [sl, #4]
7d152c add fp, r3, #1
7d1530 cmp sb, r7
7d1534 blt #0x7d1284
7d1538 add r3, sl, r7, lsl #5
7d153c ldr r8, [r3, #0x24]
7d1540 ldr r2, [sp, #0xc]
7d1544 add r3, r8, #0xc
7d1548 cmp r2, r3
7d154c beq #0x7d15c0
7d1550 ldrsb r3, [r8, #0xc]
7d1554 cmn r3, #1
7d1558 ldrsb r3, [sp, #0x50]
7d155c addne r0, r8, #0xd
7d1560 ldreq r0, [r8, #0x18]
7d1564 cmn r3, #1
7d1568 movne r1, fp
7d156c ldreq r1, [sp, #0x5c]
7d1570 bl #0x30e31c ; 
7d1574 cmp r0, #0
7d1578 beq #0x7d15c0
7d157c add r7, r7, #1
7d1580 cmp r7, sb
7d1584 bgt #0x7d1530
7d1588 lsl r3, r7, #5
7d158c add r3, r3, #8
7d1590 ldr r2, [sl, r3]
7d1594 add r1, sl, r3
7d1598 cmn r2, #2
7d159c beq #0x7d15ac
7d15a0 ldr r2, [r1, #4]
7d15a4 cmn r2, #1
7d15a8 bne #0x7d1530
7d15ac add r7, r7, #1
7d15b0 cmp r7, sb
7d15b4 add r3, r3, #0x20
7d15b8 ble #0x7d1590
7d15bc b #0x7d1530 ; 
7d15c0 cmp r8, #0
7d15c4 str r8, [sp, #0x44]
7d15c8 beq #0x7d15d4
7d15cc mov r0, r8
7d15d0 bl #0x759c64 ; _ZNK7gameswf11ref_counted7add_refEv
7d15d4 ldr r0, [sp, #0x14]
7d15d8 mov r1, r6
7d15dc add r2, sp, #0x44
7d15e0 bl #0x7d089c ; _ZN7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE3addERKS1_RKS4_
7d15e4 ldr r0, [sp, #0x44]
7d15e8 cmp r0, #0
7d15ec bne #0x7d13f4
7d15f0 b #0x7d13f8 ; 
7d15f4 bl #0x30e310 ; 
7d15f8 andseq r3, ip, r4, asr #18
7d15fc andeq r4, r0, ip, lsr #1
7d1600 andeq pc, lr, r4, ror #9
7d1604 andseq sl, r3, r4, lsl ip
7d1608 andseq sb, r3, r0, lsr #25
7d160c andseq r2, r2, r4, asr #19
7d1610 andseq sl, r3, r4, ror #21
# _ZN12GameSWFUtils17GetInvPixelScaleXEPN7gameswf4rootE 416538
416538 push {r4, r5, r6, lr}
41653c ldr r3, [r0, #0xc]
416540 mov r4, r0
416544 ldr r1, [r3, #0xb4]
416548 ldr r0, [r3, #0xb8]
41654c bl #0x30e3ac ; 
416550 mov r1, #0x41000000
416554 add r1, r1, #0xa00000
416558 bl #0x30ec94 ; 
41655c mov r5, r0
416560 ldr r0, [r4, #0x2c]
416564 bl #0x30e964 ; 
416568 mov r1, r0
41656c mov r0, r5
416570 bl #0x30ec94 ; 
416574 pop {r4, r5, r6, pc}
# _ZN11MenuManagerC1Ev 431c38
431c38 push {r4, r5, r6, r7, r8, sb, sl, lr}
431c3c ldr r6, [pc, #0x238]
431c40 ldr r2, [pc, #0x238]
431c44 ldr r3, [pc, #0x238]
431c48 add r6, pc, r6
431c4c ldr sb, [r6, r2]
431c50 ldr r3, [r6, r3]
431c54 sub sp, sp, #0x80
431c58 ldr r2, [sb]
431c5c add r1, r3, #0x28
431c60 add r3, r3, #8
431c64 mov r4, r0
431c68 str r1, [r0, #4]
431c6c str r3, [r0]
431c70 add r0, r0, #0x20
431c74 mov r5, #0
431c78 str r2, [sp, #0x7c]
431c7c bl #0x431428 ; 
431c80 mov r2, #0
431c84 mov r3, r4
431c88 str r2, [r4, #0x58]
431c8c str r2, [r4, #0x54]
431c90 str r2, [r4, #0x50]
431c94 mov r7, #1
431c98 str r5, [r4, #0x5c]
431c9c str r5, [r4, #0x60]
431ca0 str r5, [r4, #0x64]
431ca4 str r5, [r4, #0x68]
431ca8 str r5, [r4, #0x6c]
431cac str r5, [r4, #0x74]
431cb0 strb r5, [r3, #0x70]!
431cb4 str r3, [r4, #0x7c]
431cb8 str r3, [r4, #0x78]
431cbc strb r7, [r4, #0xc4]
431cc0 add r0, r4, #0xcc
431cc4 str r5, [r4, #0x80]
431cc8 strb r5, [r4, #0x88]
431ccc str r5, [r4, #0xac]
431cd0 str r5, [r4, #0xb0]
431cd4 str r5, [r4, #0xbc]
431cd8 str r5, [r4, #0xc0]
431cdc str r5, [r4, #0xcc]
431ce0 str r5, [r4, #0xd0]
431ce4 str r5, [r4, #0xd4]
431ce8 str r5, [r4, #0xd8]
431cec str r5, [r4, #0xdc]
431cf0 str r5, [r4, #0xe0]
431cf4 str r5, [r4, #0xe4]
431cf8 str r5, [r4, #0xe8]
431cfc str r5, [r4, #0xec]
431d00 str r5, [r4, #0xf0]
431d04 bl #0x431948 ; 
431d08 mvn r3, #0
431d0c str r3, [r4, #0x108]
431d10 ldr r3, [pc, #0x170]
431d14 str r5, [r4, #0x10c]
431d18 strb r5, [r4, #0x110]
431d1c ldr r0, [r6, r3]
431d20 bl #0x761170 ; _ZN7gameswf21register_log_callbackEPFvbPKcE
431d24 ldr r3, [pc, #0x160]
431d28 mov r2, #0x3f800000
431d2c strb r7, [sp, #0x20]
431d30 ldr r3, [r6, r3]
431d34 str r2, [sp, #0x24]
431d38 str r5, [sp, #4]
431d3c ldr r3, [r3, #0x10]
431d40 str r5, [sp, #8]
431d44 str r5, [sp, #0x10]
431d48 str r5, [sp, #0x14]
431d4c str r5, [sp, #0xc]
431d50 str r5, [sp, #0x18]
431d54 str r5, [sp, #0x1c]
431d58 ldr r1, [r3, #0x10]
431d5c ldr r3, [pc, #0x12c]
431d60 add r0, sp, #4
431d64 str r1, [sp, #4]
431d68 ldr r2, [r6, r3]
431d6c mov r3, #0x200
431d70 str r3, [sp, #0x10]
431d74 str r2, [sp, #8]
431d78 str r3, [sp, #0x14]
431d7c strb r5, [sp, #0x20]
431d80 bl #0x7a9814 ; _ZN8RenderFX10InitializeERNS_24InitializationParametersE
431d84 ldr r3, [pc, #0x108]
431d88 add sl, sp, #0x64
431d8c add r8, sp, #0x4c
431d90 ldr r6, [r6, r3]
431d94 add r7, sp, #0x34
431d98 mov r0, r6
431d9c bl #0x337888 ; _ZN13DebugSwitches4loadEv
431da0 ldr r1, [pc, #0xf0]
431da4 add r2, sp, #0x30
431da8 mov r0, sl
431dac add r1, pc, r1
431db0 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
431db4 mov r1, sl
431db8 mov r2, r5
431dbc mov r0, r6
431dc0 bl #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
431dc4 mov r0, sl
431dc8 bl #0x318254 ; _ZNSsD1Ev
431dcc mov r0, r6
431dd0 bl #0x337888 ; _ZN13DebugSwitches4loadEv
431dd4 ldr r1, [pc, #0xc0]
431dd8 add r2, sp, #0x2c
431ddc mov r0, r8
431de0 add r1, pc, r1
431de4 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
431de8 mov r1, r8
431dec mov r2, r5
431df0 mov r0, r6
431df4 bl #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
431df8 mov r0, r8
431dfc bl #0x318254 ; _ZNSsD1Ev
431e00 mov r0, r6
431e04 bl #0x337888 ; _ZN13DebugSwitches4loadEv
431e08 ldr r1, [pc, #0x90]
431e0c add r2, sp, #0x28
431e10 mov r0, r7
431e14 add r1, pc, r1
431e18 bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
431e1c mov r2, r5
431e20 mov r0, r6
431e24 mov r1, r7
431e28 bl #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
431e2c mov r0, r7
431e30 bl #0x318254 ; _ZNSsD1Ev
431e34 mov r1, #8
431e38 mov r0, #0x164
431e3c bl #0x310570 ; _Znwj15MemoryHintState
431e40 mov r6, r0
431e44 bl #0x437e24 ; _ZN16MultiMenuManagerC1Ev
431e48 str r6, [r4, #0xf4]
431e4c str r5, [r4, #0x104]
431e50 str r5, [r4, #0xf8]
431e54 str r5, [r4, #0xfc]
431e58 str r5, [r4, #0x100]
431e5c ldr r2, [sp, #0x7c]
431e60 ldr r3, [sb]
431e64 mov r0, r4
431e68 cmp r2, r3
431e6c bne #0x431e78
431e70 add sp, sp, #0x80
431e74 pop {r4, r5, r6, r7, r8, sb, sl, pc}
431e78 bl #0x30e310 ; 
431e7c subseq r2, r6, r8, asr #28
431e80 andeq r4, r0, ip, lsr #1
431e84 strdeq r3, r4, [r0], -ip
431e88 andeq r2, r0, ip, lsl #13
431e8c strdeq r3, r4, [r0], -r4
431e90 ldrdeq r3, r4, [r0], -r0
431e94 andeq r0, r0, r4, lsl #17
431e98 subeq lr, r8, r4, lsr #1
431e9c umaaleq lr, r8, r0, r0
431ea0 subeq lr, r8, ip, ror r0
# _ZN8RenderFX13CreateContextERNS_24InitializationParametersE 7a9708
7a9708 push {r4, r5, r6, r7, lr}
7a970c mov r1, #0
7a9710 mov r4, r0
7a9714 sub sp, sp, #0xc
7a9718 mov r0, #0x2c
7a971c bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7a9720 mov r5, r0
7a9724 bl #0x76ce38 ; _ZN7gameswf14player_contextC1Ev
7a9728 mov r1, #0
7a972c mov r0, #0x2c
7a9730 bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7a9734 ldr ip, [r4, #0x20]
7a9738 ldr r2, [r4, #0x10]
7a973c ldrb r3, [r4, #0x1c]
7a9740 ldr r1, [r4, #0xc]
7a9744 mov r6, r0
7a9748 str ip, [sp]
7a974c bl #0x7d0dfc ; _ZN7gameswf14glyph_providerC1Eiibf
7a9750 str r6, [r5, #0xc]
7a9754 mov r1, #0
7a9758 mov r0, #0x10
7a975c bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7a9760 ldr r6, [pc, #0x3c]
7a9764 ldrb r3, [r4, #0x1c]
7a9768 ldr r1, [r4, #0x14]
7a976c ldr r2, [r4, #0x18]
7a9770 mov r7, r0
7a9774 bl #0x7a9698 ; _ZN7gameswf21bitmap_glyph_providerC2Eiib
7a9778 ldr r3, [pc, #0x28]
7a977c add r6, pc, r6
7a9780 mov r0, r5
7a9784 ldr r3, [r6, r3]
7a9788 add r3, r3, #8
7a978c str r3, [r7]
7a9790 str r7, [r5, #0x10]
7a9794 ldr r3, [r4]
7a9798 str r3, [r5, #0x28]
7a979c add sp, sp, #0xc
7a97a0 pop {r4, r5, r6, r7, pc}
7a97a4 andseq fp, lr, r4, lsl r3
7a97a8 andeq r2, r0, r4, ror r5
# _ZN7gameswf21bitmap_glyph_providerC2Eiib 7a9698
7a9698 ldr ip, [pc, #0x60]
7a969c push {r4, r5, r6, r7, r8, lr}
7a96a0 ldr r5, [pc, #0x5c]
7a96a4 add ip, pc, ip
7a96a8 mov r7, r1
7a96ac ldr r5, [ip, r5]
7a96b0 cmp r2, #0
7a96b4 cmpgt r1, #0
7a96b8 mov r1, #0
7a96bc add r5, r5, #8
7a96c0 mov r6, r2
7a96c4 mov r4, r0
7a96c8 str r5, [r0]
7a96cc strb r3, [r0, #8]
7a96d0 str r1, [r0, #4]
7a96d4 str r1, [r0, #0xc]
7a96d8 ble #0x7a96f8
7a96dc mov r0, #0x40
7a96e0 bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7a96e4 mov r1, r7
7a96e8 mov r5, r0
7a96ec mov r2, r6
7a96f0 bl #0x7c4648 ; _ZN7gameswf26bitmap_glyph_texture_cacheC1Eii
7a96f4 str r5, [r4, #0xc]
7a96f8 mov r0, r4
7a96fc pop {r4, r5, r6, r7, r8, pc}
7a9700 andseq fp, lr, ip, ror #7
7a9704 andeq r1, r0, ip, lsr #12
# _ZN7gameswf18bitmap_font_entity14get_char_imageEtiPNS_4rectEPf 7c59dc
7c59dc push {r4, r5, r6, r7, r8, sb, sl, lr}
7c59e0 sub sp, sp, #0x40
7c59e4 add sl, r0, #0x24
7c59e8 add r8, sp, #0x3c
7c59ec mov r4, r0
7c59f0 mov r6, r1
7c59f4 mov r5, r2
7c59f8 mov r7, #0
7c59fc orr r2, r1, r2, lsl #16
7c5a00 mov r0, sl
7c5a04 mov r1, r8
7c5a08 str r2, [sp, #0x3c]
7c5a0c mov sb, r3
7c5a10 str r7, [sp, #0x38]
7c5a14 bl #0x7c4418 ; _ZNK7gameswf4hashIiPNS_12glyph_entityENS_15fixed_size_hashIiEEE10find_indexERKi
7c5a18 cmp r0, #0
7c5a1c blt #0x7c5a60
7c5a20 ldr r3, [r4, #0x24]
7c5a24 add r0, r3, r0, lsl #4
7c5a28 ldr r3, [r0, #0x14]
7c5a2c str r3, [sp, #0x38]
7c5a30 mov ip, r3
7c5a34 add r3, r3, #8
7c5a38 ldm r3, {r0, r1, r2, r3}
7c5a3c stm sb, {r0, r1, r2, r3}
7c5a40 ldr r3, [sp, #0x60]
7c5a44 ldr r2, [ip, #4]
7c5a48 str r2, [r3]
7c5a4c ldr r3, [r4, #0xc]
7c5a50 ldr r3, [r3, #0xc]
7c5a54 ldr r0, [r3, #0x34]
7c5a58 add sp, sp, #0x40
7c5a5c pop {r4, r5, r6, r7, r8, sb, sl, pc}
7c5a60 add r3, sp, #0xc
7c5a64 str r3, [sp]
7c5a68 mov r2, r6
7c5a6c mov r3, r5
7c5a70 ldr ip, [r4]
7c5a74 mov r0, r4
7c5a78 add r1, sp, #0x20
7c5a7c mov lr, pc
7c5a80 ldr pc, [ip, #8]
7c5a84 cmp r0, #0
7c5a88 beq #0x7c5a58
7c5a8c ldr r3, [r4, #0xc]
7c5a90 ldr r5, [r3, #0xc]
7c5a94 cmp r5, #0
7c5a98 beq #0x7c5bf8
7c5a9c mov r1, r7
7c5aa0 mov r0, #0x18
7c5aa4 bl #0x752ba8 ; _ZnwjN3swf7MemHintE
7c5aa8 mov r2, #0
7c5aac str r7, [r0]
7c5ab0 str r2, [r0, #0x14]
7c5ab4 str r2, [r0, #4]
7c5ab8 str r2, [r0, #8]
7c5abc str r2, [r0, #0xc]
7c5ac0 str r2, [r0, #0x10]
7c5ac4 ldr ip, [sp, #0x24]
7c5ac8 ldr r2, [sp, #0x28]
7c5acc mov r3, r0
7c5ad0 add ip, ip, #1
7c5ad4 add r2, r2, #1
7c5ad8 add r1, sp, #0x30
7c5adc add r0, sp, #0x34
7c5ae0 str ip, [sp, #0x34]
7c5ae4 str r2, [sp, #0x30]
7c5ae8 str r3, [sp, #0x38]
7c5aec bl #0x793560 ; _ZN7gameswf13texture_cache27get_region_size_requirementERiS1_
7c5af0 ldr r0, [sp, #0x24]
7c5af4 bl #0x30e964 ; 
7c5af8 mov r5, r0
7c5afc ldr r0, [sp, #0x34]
7c5b00 bl #0x30e964 ; 
7c5b04 mov r1, r0
7c5b08 mov r0, r5
7c5b0c bl #0x30ec94 ; 
7c5b10 ldr r3, [sp, #0x38]
7c5b14 str r0, [r3, #0xc]
7c5b18 ldr r0, [sp, #0x28]
7c5b1c bl #0x30e964 ; 
7c5b20 mov r5, r0
7c5b24 ldr r0, [sp, #0x30]
7c5b28 bl #0x30e964 ; 
7c5b2c mov r1, r0
7c5b30 mov r0, r5
7c5b34 bl #0x30ec94 ; 
7c5b38 ldr r3, [sp, #0x38]
7c5b3c str r0, [r3, #0x14]
7c5b40 ldr r0, [sp, #0xc]
7c5b44 rsb r0, r0, #0
7c5b48 bl #0x30e964 ; 
7c5b4c mov r5, r0
7c5b50 ldr r0, [sp, #0x14]
7c5b54 bl #0x30e964 ; 
7c5b58 mov r1, r0
7c5b5c mov r0, r5
7c5b60 bl #0x30ec94 ; 
7c5b64 ldr r3, [sp, #0x38]
7c5b68 str r0, [r3, #8]
7c5b6c ldr r0, [sp, #0x10]
7c5b70 bl #0x30e964 ; 
7c5b74 mov r5, r0
7c5b78 ldr r0, [sp, #0x18]
7c5b7c bl #0x30e964 ; 
7c5b80 mov r1, r0
7c5b84 mov r0, r5
7c5b88 bl #0x30ec94 ; 
7c5b8c ldr r3, [sp, #0x38]
7c5b90 str r0, [r3, #0x10]
7c5b94 ldr r5, [sp, #0x38]
7c5b98 ldr r1, [r5, #0xc]
7c5b9c ldr r0, [r5, #8]
7c5ba0 add r1, r1, #0x80000000
7c5ba4 bl #0x30ed6c ; 
7c5ba8 str r0, [r5, #8]
7c5bac ldr r5, [sp, #0x38]
7c5bb0 ldr r1, [r5, #0x14]
7c5bb4 ldr r0, [r5, #0x10]
7c5bb8 bl #0x30ed6c ; 
7c5bbc str r0, [r5, #0x10]
7c5bc0 ldr r0, [sp, #0x1c]
7c5bc4 bl #0x30e964 ; 
7c5bc8 mov r1, #0x41000000
7c5bcc add r1, r1, #0xa00000
7c5bd0 bl #0x30ed6c ; 
7c5bd4 ldr r3, [sp, #0x38]
7c5bd8 mov r1, r8
7c5bdc add r2, sp, #0x38
7c5be0 str r0, [r3, #4]
7c5be4 mov r0, sl
7c5be8 bl #0x7c5878 ; _ZN7gameswf4hashIiPNS_12glyph_entityENS_15fixed_size_hashIiEEE3addERKiRKS2_
7c5bec ldr ip, [sp, #0x38]
7c5bf0 mov r3, ip
7c5bf4 b #0x7c5a34 ; 
7c5bf8 ldr r0, [pc, #0xc]
7c5bfc add r0, pc, r0
7c5c00 bl #0x761184 ; _ZN7gameswf9log_errorEPKcz
7c5c04 mov r0, r5
7c5c08 b #0x7c5a58 ; 
7c5c0c ldrsbeq r5, [r4], -r4
# _ZN12GameSWFUtils17GetInvPixelScaleYEPN7gameswf4rootE 416578
416578 push {r4, r5, r6, lr}
41657c ldr r3, [r0, #0xc]
416580 mov r4, r0
416584 ldr r1, [r3, #0xbc]
416588 ldr r0, [r3, #0xc0]
41658c bl #0x30e3ac ; 
416590 mov r1, #0x41000000
416594 add r1, r1, #0xa00000
416598 bl #0x30ec94 ; 
41659c mov r5, r0
4165a0 ldr r0, [r4, #0x30]
4165a4 bl #0x30e964 ; 
4165a8 mov r1, r0
4165ac mov r0, r5
4165b0 bl #0x30ec94 ; 
4165b4 pop {r4, r5, r6, pc}
# _ZN21render_handler_glitch11draw_bitmapERKN7gameswf6matrixEPNS0_11bitmap_infoERKNS0_4rectES8_NS0_4rgbaE 7d8df4
7d8df4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
7d8df8 sub sp, sp, #0x54
7d8dfc ldrb r6, [sp, #0x7f]
7d8e00 mov r4, r0
7d8e04 mov r5, r1
7d8e08 mov r0, r6
7d8e0c str r2, [sp, #0x1c]
7d8e10 mov sb, r3
7d8e14 bl #0x30e964 ; 
7d8e18 mov r1, #0
7d8e1c bl #0x30df8c ; 
7d8e20 cmp r0, #0
7d8e24 ldrb r0, [sp, #0x7c]
7d8e28 ldr r7, [sp, #0x78]
7d8e2c ldrb sl, [sp, #0x7d]
7d8e30 str r0, [sp, #0x2c]
7d8e34 ldrb r8, [sp, #0x7e]
7d8e38 bne #0x7d934c
7d8e3c ldr r3, [r5]
7d8e40 ldr r0, [sb]
7d8e44 mov r1, r3
7d8e48 str r3, [sp, #8]
7d8e4c bl #0x30ed6c ; 
7d8e50 str r0, [sp, #0x28]
7d8e54 ldr r1, [r5, #4]
7d8e58 str r1, [sp, #0x20]
7d8e5c ldr r0, [sb, #8]
7d8e60 bl #0x30ed6c ; 
7d8e64 ldr fp, [r5, #8]
7d8e68 mov r2, r0
7d8e6c mov r1, r2
7d8e70 ldr r0, [sp, #0x28]
7d8e74 str r2, [sp, #0x10]
7d8e78 bl #0x30eba4 ; 
7d8e7c mov r1, fp
7d8e80 bl #0x30eba4 ; 
7d8e84 str r0, [sp, #0x30]
7d8e88 ldr ip, [r5, #0xc]
7d8e8c ldr r0, [sb]
7d8e90 mov r1, ip
7d8e94 str ip, [sp, #0xc]
7d8e98 bl #0x30ed6c ; 
7d8e9c str r0, [sp, #0x38]
7d8ea0 ldr r0, [r5, #0x10]
7d8ea4 str r0, [sp, #0x24]
7d8ea8 ldr r0, [sb, #8]
7d8eac ldr r1, [sp, #0x24]
7d8eb0 bl #0x30ed6c ; 
7d8eb4 str r0, [sp, #0x3c]
7d8eb8 ldr r5, [r5, #0x14]
7d8ebc ldr r1, [sp, #0x3c]
7d8ec0 ldr r0, [sp, #0x38]
7d8ec4 bl #0x30eba4 ; 
7d8ec8 mov r1, r5
7d8ecc bl #0x30eba4 ; 
7d8ed0 ldr r3, [sp, #8]
7d8ed4 str r0, [sp, #0x34]
7d8ed8 ldr r0, [sb, #4]
7d8edc mov r1, r3
7d8ee0 bl #0x30ed6c ; 
7d8ee4 ldr r2, [sp, #0x10]
7d8ee8 mov r1, r0
7d8eec mov r0, r2
7d8ef0 bl #0x30eba4 ; 
7d8ef4 mov r1, r0
7d8ef8 mov r0, fp
7d8efc bl #0x30eba4 ; 
7d8f00 ldr ip, [sp, #0xc]
7d8f04 str r0, [sp, #0x14]
7d8f08 ldr r0, [sb, #4]
7d8f0c mov r1, ip
7d8f10 bl #0x30ed6c ; 
7d8f14 mov r1, r0
7d8f18 ldr r0, [sp, #0x3c]
7d8f1c bl #0x30eba4 ; 
7d8f20 mov r1, r0
7d8f24 mov r0, r5
7d8f28 bl #0x30eba4 ; 
7d8f2c str r0, [sp, #0x18]
7d8f30 ldr r3, [sb, #0xc]
7d8f34 ldr r1, [sp, #0x20]
7d8f38 mov r0, r3
7d8f3c str r3, [sp, #8]
7d8f40 bl #0x30ed6c ; 
7d8f44 mov r1, r0
7d8f48 ldr r0, [sp, #0x28]
7d8f4c bl #0x30eba4 ; 
7d8f50 mov r1, r0
7d8f54 mov r0, fp
7d8f58 bl #0x30eba4 ; 
7d8f5c ldr r3, [sp, #8]
7d8f60 ldr r1, [sp, #0x24]
7d8f64 mov sb, r0
7d8f68 mov r0, r3
7d8f6c bl #0x30ed6c ; 
7d8f70 mov r1, r0
7d8f74 ldr r0, [sp, #0x38]
7d8f78 bl #0x30eba4 ; 
7d8f7c mov r1, r0
7d8f80 mov r0, r5
7d8f84 bl #0x30eba4 ; 
7d8f88 mov r1, sb
7d8f8c mov r5, r0
7d8f90 ldr r0, [sp, #0x14]
7d8f94 bl #0x30eba4 ; 
7d8f98 ldr r1, [sp, #0x30]
7d8f9c bl #0x30e3ac ; 
7d8fa0 mov r1, r5
7d8fa4 str r0, [sp, #0x28]
7d8fa8 ldr r0, [sp, #0x18]
7d8fac bl #0x30eba4 ; 
7d8fb0 ldr r1, [sp, #0x34]
7d8fb4 bl #0x30e3ac ; 
7d8fb8 ldr r1, [sp, #0x1c]
7d8fbc str r0, [sp, #0x24]
7d8fc0 ldr r3, [r1]
7d8fc4 mov r0, r1
7d8fc8 mov lr, pc
7d8fcc ldr pc, [r3, #8]
7d8fd0 ldr r2, [sp, #0x1c]
7d8fd4 ldr r0, [r2, #0x10]
7d8fd8 cmp r0, #0
7d8fdc beq #0x7d8fe8
7d8fe0 mov r1, #1
7d8fe4 bl #0x7d3bb4 ; _ZN6glitch5video8ITexture7setWrapENS0_15E_TEXTURE_CLAMPE
7d8fe8 ldr ip, [sp, #0x1c]
7d8fec add r3, r4, #0x1f0
7d8ff0 mov r0, r3
7d8ff4 add r1, ip, #0x10
7d8ff8 str r3, [sp, #0x20]
7d8ffc bl #0x7d6a48 ; _ZN16BufferedRenderer10setTextureERN5boost13intrusive_ptrIN6glitch5video8ITextureEEE
7d9000 ldr r3, [r4, #0x374]
7d9004 ldr r2, [r4, #0x348]
7d9008 ldr r0, [sp, #0x30]
7d900c movw fp, #0x6667
7d9010 str r2, [r3, #0x14]
7d9014 str r0, [r3, #0xc]
7d9018 ldr r1, [sp, #0x34]
7d901c movt fp, #0x6666
7d9020 str r1, [r3, #0x10]
7d9024 ldr r3, [r4, #0x374]
7d9028 ldr r2, [r4, #0x348]
7d902c add r3, r3, #0x18
7d9030 str r2, [r3, #0x14]
7d9034 ldr r2, [sp, #0x14]
7d9038 str r2, [r3, #0xc]
7d903c ldr ip, [sp, #0x18]
7d9040 str ip, [r3, #0x10]
7d9044 ldr r2, [r4, #0x374]
7d9048 ldr r1, [r4, #0x348]
7d904c mov r3, #0
7d9050 add r2, r2, #0x30
7d9054 str r5, [r2, #0x10]
7d9058 str r1, [r2, #0x14]
7d905c str sb, [r2, #0xc]
7d9060 ldr r2, [r4, #0x374]
7d9064 ldr r1, [r4, #0x348]
7d9068 mov r5, #0x14
7d906c add r2, r2, #0x48
7d9070 str r1, [r2, #0x14]
7d9074 ldr r0, [sp, #0x28]
7d9078 str r0, [r2, #0xc]
7d907c ldr r1, [sp, #0x24]
7d9080 str r1, [r2, #0x10]
7d9084 ldr r0, [r7]
7d9088 ldr r1, [r7, #8]
7d908c ldr r2, [r4, #0x374]
7d9090 str r0, [r2]
7d9094 str r1, [r2, #4]
7d9098 ldr r1, [r7, #8]
7d909c ldr r2, [r4, #0x374]
7d90a0 ldr r0, [r7, #4]
7d90a4 str r0, [r2, #0x18]
7d90a8 str r1, [r2, #0x1c]
7d90ac ldr r1, [r7, #0xc]
7d90b0 ldr r0, [r7]
7d90b4 ldr r2, [r4, #0x374]
7d90b8 str r0, [r2, #0x30]
7d90bc str r1, [r2, #0x34]
7d90c0 ldr r0, [r7, #0xc]
7d90c4 ldr r1, [r7, #4]
7d90c8 ldr r2, [r4, #0x374]
7d90cc mov r7, r3
7d90d0 str r0, [r2, #0x4c]
7d90d4 str r1, [r2, #0x48]
7d90d8 str fp, [sp, #0x14]
7d90dc mov fp, r6
7d90e0 ldr r6, [sp, #0x2c]
7d90e4 ldr r3, [r4, #0x374]
7d90e8 add r3, r3, r7
7d90ec strb r6, [r3, #8]
7d90f0 strb fp, [r3, #0xb]
7d90f4 strb r8, [r3, #0xa]
7d90f8 strb sl, [r3, #9]
7d90fc ldrb r3, [r4, #4]
7d9100 cmp r3, #0
7d9104 beq #0x7d9168
7d9108 ldr sb, [r4, #0x374]
7d910c add sb, sb, r7
7d9110 ldr r0, [sb, #0xc]
7d9114 bl #0x30e4cc ; 
7d9118 ldr ip, [sp, #0x14]
7d911c add r0, r0, #0xa
7d9120 smull ip, r3, ip, r0
7d9124 asr r0, r0, #0x1f
7d9128 rsb r0, r0, r3, asr #3
7d912c mul r0, r5, r0
7d9130 bl #0x30e964 ; 
7d9134 str r0, [sb, #0xc]
7d9138 ldr sb, [r4, #0x374]
7d913c add sb, sb, r7
7d9140 ldr r0, [sb, #0x10]
7d9144 bl #0x30e4cc ; 
7d9148 ldr r1, [sp, #0x14]
7d914c add r0, r0, #0xa
7d9150 smull r1, r3, r1, r0
7d9154 asr r0, r0, #0x1f
7d9158 rsb r0, r0, r3, asr #3
7d915c mul r0, r5, r0
7d9160 bl #0x30e964 ; 
7d9164 str r0, [sb, #0x10]
7d9168 add r7, r7, #0x18
7d916c cmp r7, #0x60
7d9170 bne #0x7d90e4
7d9174 ldr r3, [pc, #0x22c]
7d9178 ldr r1, [r4, #0x378]
7d917c mov r2, #4
7d9180 add r3, pc, r3
7d9184 ldr ip, [r3, #0x18]
7d9188 ldr r0, [r3, #0x1c]
7d918c str r2, [r1, #8]
7d9190 ldr lr, [r3, #0x20]
7d9194 add r5, sp, #0x50
7d9198 ldr r1, [r4, #0x374]
7d919c str ip, [r5, #-0xc]!
7d91a0 add ip, sp, #0x48
7d91a4 str r0, [ip], #4
7d91a8 str lr, [ip]
7d91ac mov r6, #6
7d91b0 mov r0, r4
7d91b4 mov r3, r5
7d91b8 str r6, [sp]
7d91bc str r6, [sp, #4]
7d91c0 bl #0x7d860c ; _ZN21render_handler_glitch25process_mask_intersectionEP6VertexiPKtiN6glitch5video16E_PRIMITIVE_TYPEE
7d91c4 cmp r0, #0
7d91c8 beq #0x7d9354
7d91cc ldr r6, [r4, #0xc]
7d91d0 cmp r6, #0
7d91d4 beq #0x7d934c
7d91d8 ldr r2, [r6, #0x44]
7d91dc str r2, [sp, #0x14]
7d91e0 ldr r7, [r4, #0x374]
7d91e4 adds r8, r2, #6
7d91e8 ldr fp, [r6, #0x24]
7d91ec add r4, r7, #0xc
7d91f0 beq #0x7d9200
7d91f4 ldr r3, [r6, #0x48]
7d91f8 cmp r8, r3
7d91fc bgt #0x7d9378
7d9200 ldr ip, [sp, #0x14]
7d9204 mov r2, #0
7d9208 lsl r3, ip, #1
7d920c ldr r0, [r6, #0x40]
7d9210 add r1, r3, r2
7d9214 add r2, r2, #2
7d9218 mov ip, #0
7d921c cmp r2, #0xc
7d9220 strh ip, [r0, r1]
7d9224 bne #0x7d920c
7d9228 ldr r0, [r6, #0x40]
7d922c mov r1, r5
7d9230 str r8, [r6, #0x44]
7d9234 add r0, r0, r3
7d9238 bl #0x30e868 ; 
7d923c ldr r5, [r6, #0x24]
7d9240 adds r5, r5, #4
7d9244 beq #0x7d9254
7d9248 ldr r3, [r6, #0x28]
7d924c cmp r5, r3
7d9250 bgt #0x7d9398
7d9254 ldr sl, [r6, #0x34]
7d9258 str r5, [r6, #0x24]
7d925c adds sl, sl, #4
7d9260 beq #0x7d9270
7d9264 ldr r3, [r6, #0x38]
7d9268 cmp sl, r3
7d926c bgt #0x7d9388
7d9270 ldr r3, [r6, #0x20]
7d9274 ldr sb, [r6, #0x30]
7d9278 mov r8, #0xc
7d927c mla r8, r8, fp, r3
7d9280 mov r5, #0
7d9284 str sl, [r6, #0x34]
7d9288 add sb, sb, fp, lsl #3
7d928c mov r1, r5
7d9290 mov r2, r5
7d9294 ldr r3, [r4, r2]
7d9298 add r0, r4, r2
7d929c add r0, r0, #4
7d92a0 str r3, [r8, r1]
7d92a4 ldr ip, [r0], #4
7d92a8 add r3, r8, r1
7d92ac add r3, r3, #4
7d92b0 str ip, [r3], #4
7d92b4 ldr sl, [r0]
7d92b8 mov ip, r7
7d92bc mov r0, sb
7d92c0 str sl, [r3]
7d92c4 ldr r3, [ip, r2]!
7d92c8 add r2, r2, #0x18
7d92cc cmp r2, #0x480
7d92d0 str r3, [r0, r5]!
7d92d4 ldr r3, [ip, #4]
7d92d8 add r1, r1, #0xc
7d92dc add r5, r5, #8
7d92e0 str r3, [r0, #4]
7d92e4 bne #0x7d9294
7d92e8 ldr r3, [r6, #0x14]
7d92ec ldr r2, [r6, #0x18]
7d92f0 add r4, r3, #1
7d92f4 cmp r4, r2
7d92f8 ble #0x7d930c
7d92fc add r0, r6, #0x10
7d9300 add r1, r4, r4, asr #1
7d9304 bl #0x78a6f8 ; _ZN7gameswf5arrayINS_12render_cache5entryEE7reserveEi
7d9308 ldr r3, [r6, #0x14]
7d930c mov r2, #0x18
7d9310 ldr r1, [r6, #0x10]
7d9314 mul r2, r2, r3
7d9318 ldr r0, [sp, #0x2c]
7d931c add r3, r1, r2
7d9320 str r0, [r3, #4]
7d9324 ldr ip, [sp, #0x1c]
7d9328 str ip, [r1, r2]
7d932c mov r2, #6
7d9330 str r2, [r3, #0x14]
7d9334 str fp, [r3, #8]
7d9338 ldr r0, [sp, #0x14]
7d933c mov r2, #4
7d9340 str r2, [r3, #0xc]
7d9344 str r0, [r3, #0x10]
7d9348 str r4, [r6, #0x14]
7d934c add sp, sp, #0x54
7d9350 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
7d9354 mov r3, r6
7d9358 ldr r0, [sp, #0x20]
7d935c add r1, r4, #0x378
7d9360 mov r2, r5
7d9364 bl #0x7d7094 ; _ZN16BufferedRenderer21queueIndexedTrianglesERKN5boost13intrusive_ptrIN6glitch5video14CVertexStreamsEEEPKti
7d9368 ldr r6, [r4, #0xc]
7d936c cmp r6, #0
7d9370 bne #0x7d91d8
7d9374 b #0x7d934c ; 
7d9378 add r0, r6, #0x40
7d937c add r1, r8, r8, asr #1
7d9380 bl #0x779e7c ; _ZN7gameswf5arrayItE7reserveEi
7d9384 b #0x7d9200 ; 
7d9388 add r0, r6, #0x30
7d938c add r1, sl, sl, asr #1
7d9390 bl #0x7d4208 ; _ZN7gameswf5arrayINS_9vector2dfEE7reserveEi
7d9394 b #0x7d9270 ; 
7d9398 add r0, r6, #0x20
7d939c add r1, r5, r5, asr #1
7d93a0 bl #0x7d4180 ; _ZN7gameswf5arrayINS_9vector3dfEE7reserveEi
7d93a4 b #0x7d9254 ; 
7d93a8 ldrheq r2, [r3], -ip
