# 0x369990 _ZN15VoxSoundManager13StopAllSoundsEi
00369990: ldr r3, [pc, #0x5c]
00369994: ldr r2, [pc, #0x5c]
00369998: push {r4, lr}
0036999c: add r3, pc, r3
003699a0: ldr r2, [r3, r2]
003699a4: ldrb r3, [r2]
003699a8: cmp r3, #0
003699ac: bne #0x3699e4
003699b0: ldr r4, [r0]
003699b4: cmp r4, #0
003699b8: beq #0x3699f0
003699bc: mov r0, r1
003699c0: bl #0x30e964
003699c4: mov r1, #0x44000000
003699c8: add r1, r1, #0x7a0000
003699cc: bl #0x30ec94
003699d0: mvn r1, #0
003699d4: mov r2, r0
003699d8: mov r0, r4
003699dc: pop {r4, lr}
003699e0: b #0x8620d0
003699e4: mvn r0, #0
003699e8: pop {r4, lr}
003699ec: b #0x531940
003699f0: pop {r4, pc}

# 0x369514 _ZN15VoxSoundManager13SetMusicStateEPKc
00369514: push {r4, r5, r6, r8, sb, lr}
